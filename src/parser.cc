/* Copyright 2012 <MulticorewareInc> */

#include "parser.h"
#include <string>
#include <map>
#include "tokens.h"
#include "lexer.h"
#include "error_reporter_interface.h"


// variables returned by lexer
namespace hsa {
namespace brig {

int QueryOpPart2(Context* context, BrigDataType16_t* pDataType, BrigOpcode32_t* pOpcode) {
  switch (context->token_to_scan) {
    case QUERY_ORDER:
      *pOpcode = BrigQueryOrder;
      *pDataType = Brigb32;
      return 0;
    case QUERY_DATA:
      *pOpcode = BrigQueryData;
      *pDataType = Brigb32;
      return 0;
    case QUERY_ARRAY:
      *pOpcode = BrigQueryArray;
      *pDataType = Brigb32;
      return 0;
    case QUERY_NORMALIZED:
      *pOpcode = BrigQueryNormalized;
      *pDataType = Brigb32;
      return 0;
    case QUERY_FILTERING:
      *pOpcode = BrigQueryFiltering;
      *pDataType = Brigb32;
      return 0;
    case QUERY_WIDTH:
      *pOpcode = BrigQueryWidth;
      *pDataType = Brigu32;
      return 0;
    case QUERY_DEPTH:
      *pOpcode = BrigQueryDepth;
      *pDataType = Brigu32;
      return 0;
    case QUERY_HEIGHT:
      *pOpcode = BrigQueryHeight;
      *pDataType = Brigu32;
      return 0;
    default:
      context->set_error(MISSING_DECLPREFIX);
      return 1;
  }
  return 1;
}
int QueryOp(Context* context) {
  BrigDataType16_t dataType;
  BrigOpcode32_t opcode;
  return QueryOpPart2(context, &dataType, &opcode);
}


int Query(Context* context) {
  // Chuang
  BrigInstBase query_inst = {
    32,                    // size
    BrigEInstBase,         // kind
    0,                     // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };

  if (!QueryOpPart2(context, &query_inst.type, &query_inst.opcode)) {
    context->token_to_scan = yylex();

    // According to Query Image (query) Operations in v1.2 PRM 7.7.1
    // If opcode is width, depth, height. The data type must be Brigu32.
    // If not, it must be Brigb32.
    if (context->token_value.data_type == query_inst.type) {
      context->token_to_scan = yylex();
      // According to the Data Type , The register must be a 32bit Register.
      if (context->token_to_scan == TOKEN_SREGISTER) {
        // just use Idenifier rule to fill $s register to operand section.
        std::string opName = context->token_value.string_val;
        if (Identifier(context)) {
          return 1;
        }
        query_inst.o_operands[0] = context->operand_map[opName];

        context->token_to_scan = yylex();
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
          if (context->token_to_scan != '[') {
            context->set_error(MISSING_OPERAND);
          } else {
            context->token_to_scan = yylex();
          }

          if (!AddressableOperandPart2(context, &query_inst.o_operands[1], true)) {
            if (context->token_to_scan == ';') {

              context->append_code(&query_inst);
              context->token_to_scan = yylex();
              return 0;
            } else {
              context->set_error(MISSING_SEMICOLON);
            }
          } else {
            context->set_error(INVALID_OPERAND);
          }
        } else {
          context->set_error(MISSING_COMMA);
        }
      } else {
        context->set_error(MISSING_OPERAND);
      }
    } else {
      context->set_error(MISSING_DATA_TYPE);
    }
  }
  return 1;
}

int Operand(Context* context) {
  if (!Identifier(context)) {  // an Identifier
    context->token_to_scan = yylex();  // set token for next function
    return 0;
  } else if (!BaseOperand(context)) {    // a base Operand
    context->token_to_scan = yylex();  // set token for next function
    return 0;
  }
  return 1;
}

int Identifier(Context* context) {
  if (context->token_to_scan == TOKEN_GLOBAL_IDENTIFIER) {
    return 0;
  } else if (context->token_to_scan == TOKEN_LOCAL_IDENTIFIER) {
    return 0;
  } else if (context->token_type == REGISTER) {
    BrigOperandReg bor;
    bor.size = sizeof(BrigOperandReg);
    bor.kind = BrigEOperandReg;
    switch (context->token_to_scan) {
      case TOKEN_CREGISTER:
        bor.type = Brigb1;
        // refresh context type.
        context->set_type(Brigb1);
        break;
      case TOKEN_DREGISTER:
        bor.type = Brigb64;
        context->set_type(Brigb64);
        break;
      case TOKEN_SREGISTER:
        bor.type = Brigb32;
        context->set_type(Brigb32);
        break;
      case TOKEN_QREGISTER:
        bor.type = Brigb128;
        context->set_type(Brigb128);
        break;
    }
    bor.reserved = 0;
    std::string name(context->token_value.string_val);
    bor.name = context->add_symbol(name);
    if (!context->operand_map.count(name)) {
      context->operand_map[name] = context->get_operand_offset();
      context->append_operand(&bor);
    }
    return 0;
  }
  return 1;
}

int BaseOperand(Context* context) {
  if (context->token_to_scan == TOKEN_DOUBLE_CONSTANT) {
    BrigOperandImmed boi = {
      sizeof(boi),        // size
      BrigEOperandImmed,  // kind
      Brigb64,            // type
      0,                  // reserved
      { 0 }
    };
    boi.bits.d = context->token_value.double_val;
    context->append_operand(&boi);

    return 0;
  } else if (context->token_to_scan == TOKEN_SINGLE_CONSTANT) {
    BrigOperandImmed boi = {
      sizeof(boi),        // size
      BrigEOperandImmed,  // kind
      Brigb32,            // type
      0,                  // reserved
      { 0 }
    };
    boi.bits.f = context->token_value.float_val;
    context->append_operand(&boi);
    return 0;
  } else if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
    BrigOperandImmed boi = {
      sizeof(boi),        // size
      BrigEOperandImmed,  // kind
      Brigb32,            // type
      0,                  // reserved
      { 0 }
    };
    // TODO(Huy): check context for operation type and decide the type
    boi.bits.u = context->token_value.int_val;
    context->append_operand(&boi);

    return 0;
  } else if (context->token_to_scan == TOKEN_WAVESIZE) {
    return 0;  // currently not supported
  } else if (context->token_to_scan == '-') {
    context->token_to_scan = yylex();
    if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
      BrigOperandImmed boi = {
      sizeof(boi),        // size
      BrigEOperandImmed,  // kind
      Brigb32,            // type
      0,                  // reserved
      { 0 }
      };
      // TODO(Huy): check context for operation type and decide the type
      boi.bits.u = -context->token_value.int_val;
      context->append_operand(&boi);

     return 0;
    }
  } else if (context->token_type == DATA_TYPE_ID) {
    // scan next token
    context->token_to_scan = yylex();
    if (context->token_to_scan == '(') {   // should be '('
      // check if we have a decimal list single or float list single
      context->token_to_scan = yylex();
      if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
        BrigOperandImmed boi = {
        sizeof(boi),        // size
        BrigEOperandImmed,  // kind
        Brigb32,            // type
        0,                  // reserved
        { 0 }
        };
        // TODO(Huy): check context for operation type and decide the type
        boi.bits.u = -context->token_value.int_val;
        context->append_operand(&boi);

        context->token_to_scan = yylex();
        if (context->token_to_scan == ')') {
          return 0;
        } else {
          while (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
              BrigOperandImmed boi = {
                sizeof(boi),        // size
                BrigEOperandImmed,  // kind
                Brigb32,            // type
                0,                  // reserved
                { 0 }
              };
  // TODO(Huy): check context for operation type and decide the type
              boi.bits.u = context->token_value.int_val;
              context->append_operand(&boi);

              context->token_to_scan = yylex();
              if (context->token_to_scan == ')') {
                return 0;
              } else if (context->token_to_scan != ',') {
                context->set_error(MISSING_COMMA);
                return 1;
              }
            } else {
              context->set_error(MISSING_INTEGER_CONSTANT);
              return 1;
            }
          }  // while
        }
      } else if (context->token_to_scan == TOKEN_DOUBLE_CONSTANT)   {
        BrigOperandImmed boi = {
        sizeof(boi),        // size
        BrigEOperandImmed,  // kind
        Brigb64,            // type
        0,                  // reserved
        { 0 }
        };
        boi.bits.d = context->token_value.double_val;
        context->append_operand(&boi);

        context->token_to_scan = yylex();
        if (context->token_to_scan == ')') {
          return 0;
        } else {
          while (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if (context->token_to_scan == TOKEN_DOUBLE_CONSTANT) {
              BrigOperandImmed boi = {
                sizeof(boi),        // size
                BrigEOperandImmed,  // kind
                Brigb64,            // type
                0,                  // reserved
                { 0 }
              };
              boi.bits.d = context->token_value.double_val;
              context->append_operand(&boi);
              context->token_to_scan = yylex();
              if (context->token_to_scan == ')') {
                return 0;
              } else if (context->token_to_scan != ',') {
                context->set_error(MISSING_COMMA);
                return 1;
              }
            } else {
              context->set_error(MISSING_DOUBLE_CONSTANT);
              return 1;
            }
          }  // while
        }
      }
    }
  }
  return 1;
}

int AddressableOperand(Context* context) {
  BrigoOffset32_t opOffset;
  return AddressableOperandPart2(context, &opOffset, true);
}

int AddressableOperandPart2(Context* context, BrigoOffset32_t* pRetOpOffset, bool IsImageOrSampler){
  // context->token_to_scan must be non register
  if ((context->token_to_scan == TOKEN_GLOBAL_IDENTIFIER) ||
      (context->token_to_scan == TOKEN_LOCAL_IDENTIFIER)) {
    std::string name(context->token_value.string_val);

    context->token_to_scan = yylex();

    if (context->token_to_scan == ']') {
      if (!IsImageOrSampler) {
        BrigOperandAddress boa = {
          sizeof(boa),            // size
          BrigEOperandAddress,    // kind
          Brigb32,                // type
          0,                      // reserved
          0/*,                      // directive
          0*/
        };
        // TODO(Chuang) name isn't declared in directive.
        // if (!context->symbol_map.count(name)) {
        //   context->set_error(MISSING_OPERAND);
        //   return 1;
        // }
        boa.directive = context->symbol_map[name];

        if (context->get_machine() == BrigELarge) {
          boa.type = Brigb64;
        }
        *pRetOpOffset = context->get_operand_offset();
        context->append_operand(&boa);
      } else {
        BrigOperandOpaque boo = {
          16,
          BrigEOperandOpaque,
          0,                      // name
          0,                      // reg
          0                       // offset
        };
        // TODO(Chuang) name isn't declared in directive.
        // if (!context->symbol_map.count(name)) {
        //   context->set_error(MISSING_OPERAND);
        //   return 1;
        // }
        boo.name = context->symbol_map[name];
        *pRetOpOffset = context->get_operand_offset();
        context->append_operand(&boo);
      }
      context->token_to_scan = yylex();
      return 0;
    } else if (context->token_to_scan == '<') {
      if (!IsImageOrSampler) {
        context->set_error(INVALID_OPERATION);
        return 1;
      }
      // TODO(Chuang) name isn't declared in directive.
      // if (!context->symbol_map.count(name)) {
      //   context->set_error(MISSING_OPERAND);
      //   return 1;
      // }
      BrigOperandOpaque boo = {
        16,
        BrigEOperandOpaque,
        context->symbol_map[name], // name
        0,                         // reg
        0                          // offset
      };
      context->token_to_scan = yylex();
      if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
        boo.offset = context->token_value.int_val;

        context->token_to_scan = yylex();
        if (context->token_to_scan == '>') {
          context->token_to_scan = yylex();
          if (context->token_to_scan == ']') {
            *pRetOpOffset = context->get_operand_offset();
            context->append_operand(&boo);
            context->token_to_scan = yylex();
            return 0;
          } else {
            context->set_error(MISSING_CLOSING_BRACKET);
          }
        } else {
          context->set_error(MISSING_CLOSING_BRACKET);
        }
      } else if (context->token_to_scan == TOKEN_SREGISTER) {
        name = context->token_value.string_val;
        if (Identifier(context)) {
          return 1;
        }
        boo.reg = context->operand_map[name];
        context->token_to_scan = yylex();
        if (context->token_to_scan == '>') {
          context->token_to_scan = yylex();
          if (context->token_to_scan == ']') {
            *pRetOpOffset = context->get_operand_offset();
            context->append_operand(&boo);
            context->token_to_scan = yylex();
            return 0;
          } else {
            context->set_error(MISSING_CLOSING_BRACKET);
          }
        } else if ((context->token_to_scan == '+') ||
                   (context->token_to_scan == '-')) {
          int sign = 1;
          if (context->token_to_scan == '-') {
            sign = -1;
          }
          context->token_to_scan = yylex();
          if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
            boo.offset = context->token_value.int_val * sign;
            context->token_to_scan = yylex();
            if (context->token_to_scan == '>') {
              context->token_to_scan = yylex();
              if (context->token_to_scan == ']') {
                *pRetOpOffset = context->get_operand_offset();
                context->append_operand(&boo);
                context->token_to_scan = yylex();
                return 0;
              } else {
                context->set_error(MISSING_CLOSING_BRACKET);
              }
            } else {
              context->set_error(MISSING_CLOSING_BRACKET);
            }
          } else {
            context->set_error(MISSING_INTEGER_CONSTANT);
          }
        } else {
          context->set_error(MISSING_CLOSING_BRACKET);
        }
      }
    }
  }
  return 1;
}

int ArrayOperandList(Context* context) {
  BrigoOffset32_t opOffset;
  return ArrayOperandListPart2(context, &opOffset);
}

int ArrayOperandListPart2(Context* context, BrigoOffset32_t* pRetOpOffset) {
  // assumed first_token is '('
  unsigned int count_op = 0;
  BrigoOffset32_t regs[4] = {0};
  BrigDataType16_t type = Brigb32;
  std::string iden_name;
  while (1) {
    context->token_to_scan = yylex();
    // set context for Identifier()
    iden_name = context->token_value.string_val;
    if (!Identifier(context)) {
      if (count_op <= 3) {
        regs[count_op] = context->operand_map[iden_name];
      } else {
        context->set_error(INVALID_OPERAND);
        return 1;
      }
      if (count_op == 0) {
        type = context->get_type();
      } else if (type != context->get_type()) {
        context->set_error(INVALID_OPERAND);
        return 1;
      }
      ++count_op;
      context->token_to_scan = yylex();
      if (context->token_to_scan == ')') {
        break;
      } else if (context->token_to_scan == ',') {
        continue;
      } else {
        context->set_error(MISSING_CLOSING_PARENTHESIS);
        return 1;
      }
    } else {
      context->set_error(MISSING_IDENTIFIER);
      return 1;
    }
  }
  switch (count_op) {
    case 0:
    case 3: {
      context->set_error(MISSING_IDENTIFIER);
      return 1;
    }
    case 1: {
      // just have one operand.
      // e.g. ($s1)
      *pRetOpOffset = regs[0];
      break;
    }
    case 2: {
      BrigOperandRegV2 oper_regV2 = {
        16,                    // size
        BrigEOperandRegV2,     // kind
        Brigb32,               // type
        0,                     // reserved
        {0, 0}                 // regs
      };
      oper_regV2.regs[0] = regs[0];
      oper_regV2.regs[1] = regs[1];
      oper_regV2.type = type;

      *pRetOpOffset = context->get_operand_offset();
      context->append_operand(&oper_regV2);

      break;
    }
    case 4: {
      BrigOperandRegV4 oper_regV4 = {
        24,                    // size
        BrigEOperandRegV4,     // kind
        Brigb32,               // type
        0,                     // reserved
        {0, 0, 0, 0}           // regs
      };

      oper_regV4.regs[0] = regs[0];
      oper_regV4.regs[1] = regs[1];
      oper_regV4.regs[2] = regs[2];
      oper_regV4.regs[3] = regs[3];
      oper_regV4.type = type;

      *pRetOpOffset = context->get_operand_offset();
      context->append_operand(&oper_regV4);

      break;
    }
    default: {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
  }
  // set context for following functions
  context->token_to_scan = yylex();
  return 0;
}
int CallTargets(Context* context) {
  // assumed first_token is '['
  while (1) {
    context->token_to_scan = yylex();  // set context for Identifier()
    if (!Identifier(context)) {
      context->token_to_scan = yylex();
      if (context->token_to_scan == ']') {
        context->token_to_scan = yylex();  // set context for following function
        break;
      } else if (context->token_to_scan == ',') {
      } else {
        context->set_error(MISSING_CLOSING_BRACKET);
        return 1;
      }
    } else {
      context->set_error(MISSING_IDENTIFIER);
      return 1;
    }
  }
  return 0;
}

int CallArgs(Context* context) {
  // assumed first_token is '('
  int n_elements = 0;  // the size of the arglist.
  BrigoOffset32_t arg_offset[256] = {0};

  // [CAUTION] Assume the arg numbers cannot exceed 256
  // Limited by the structure, we can only access one element.

  context->token_to_scan = yylex();  // set context for Operand()
  unsigned int saved_token;
  std::string arg_name;
  while (1) {
    saved_token = context->token_to_scan;
    if ((saved_token == TOKEN_GLOBAL_IDENTIFIER)||
        (saved_token == TOKEN_LOCAL_IDENTIFIER)) {
      arg_name = context->token_value.string_val;
    }

    if (context->token_to_scan == ')') {
      size_t list_size = sizeof(BrigOperandArgumentList);
      if (n_elements > 1)
        list_size += sizeof(BrigoOffset32_t) * (n_elements - 1);

      char *array = new char[list_size];
      BrigOperandArgumentList &arg_list =
        *reinterpret_cast<BrigOperandArgumentList *>(array);
      arg_list.size = list_size;
      arg_list.kind = BrigEOperandArgumentList;
      arg_list.elementCount = n_elements;
      for (int32_t i = 0; i < n_elements; ++i) {
        arg_list.o_args[i] = arg_offset[i];
      }
      context->current_argList_offset = context->get_operand_offset();
      context->append_operand(&arg_list);
      context->token_to_scan = yylex();

      delete[] array;
      break;
    } else if (!Operand(context)) {
      if ((saved_token == TOKEN_GLOBAL_IDENTIFIER)||
          (saved_token == TOKEN_LOCAL_IDENTIFIER)) {
        arg_offset[n_elements] = context->arg_map[arg_name];
      }

      n_elements++;
      continue;
    } else if (context->token_to_scan == ',') {
      context->token_to_scan = yylex();
      continue;
    } else {
      context->set_error(MISSING_CLOSING_PARENTHESIS);
      return 1;
    }
  }
  return 0;
}

int RoundingMode(Context* context) {
  unsigned int first_token = context->token_to_scan;
  // get current alu modifier from context
  BrigAluModifier mod = context->get_alu_modifier();

  if (first_token == _FTZ) {
    mod.ftz = 1;
    context->token_to_scan = yylex();

    if (context->token_type == FLOAT_ROUNDING) {
      // next is floatRounding
      mod.floatOrInt = 1;
      switch (context->token_to_scan) {
        case _UP:
          mod.rounding = 2;
          break;
        case _DOWN:
          mod.rounding = 3;
          break;
        case _ZERO:
          mod.rounding = 1;
          break;
        case _NEAR:
          mod.rounding = 0;
          break;
      }
      context->token_to_scan = yylex();  // set context for following functions
    }
    context->set_alu_modifier(mod);
    return 0;
  } else if (context->token_type == INT_ROUNDING) {
    mod.floatOrInt = 0;
    switch (first_token) {
      case _UPI:
        mod.rounding = 2;
        break;
      case _DOWNI:
        mod.rounding = 3;
        break;
      case _ZEROI:
        mod.rounding = 1;
        break;
      case _NEARI:
        mod.rounding = 0;
        break;
    }
    context->token_to_scan = yylex();  // set context for following functions
    context->set_alu_modifier(mod);
    return 0;
  } else if (context->token_type == FLOAT_ROUNDING) {
    mod.floatOrInt = 1;
    switch (first_token) {
      case _UP:
        mod.rounding = 2;
        break;
      case _DOWN:
        mod.rounding = 3;
        break;
      case _ZERO:
        mod.rounding = 1;
        break;
      case _NEAR:
        mod.rounding = 0;
        break;
    }
    context->token_to_scan = yylex();  // set context for following functions
    context->set_alu_modifier(mod);
    return 0;
  } else {
    return 1;
  }
}

int Instruction2(Context* context) {
  // First token must be an Instruction2Opcode
  BrigOpcode32_t opcode = context->token_value.opcode;

  if (context->token_type == INSTRUCTION2_OPCODE) {
    context->token_to_scan = yylex();  // set context for Rounding mode
    if (!RoundingMode(context)) {
      // need to use BrigInstMod
      // there is a rounding mode specified
      BrigInstMod inst_op = {
        sizeof(inst_op),    // size
        BrigEInstMod,       // kind
        opcode,  // opcode
        0,  // type
        BrigNoPacking,    // packing
        {0, 0, 0, 0, 0},   // o_operands
        context->get_alu_modifier()   // aluModifier;
      };

      // check whether there is a Packing
      if (context->token_type == PACKING) {
        // there is packing
        inst_op.packing = context->token_value.packing;
        context->token_to_scan = yylex();
      }

      // now we must have a dataTypeId
      if (context->token_type == DATA_TYPE_ID) {
        // check the operands
        inst_op.type = context->token_value.data_type;

        context->token_to_scan = yylex();  // set context for Operand()
        std::string oper_str = context->token_value.string_val;

        if (!Operand(context)) {
          inst_op.o_operands[0] = context->operand_map[oper_str];
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();  // set context for Operand()
            oper_str = context->token_value.string_val;

            if (!Operand(context)) {
              inst_op.o_operands[1] = context->operand_map[oper_str];
              if (context->token_to_scan == ';') {
                context->append_code(&inst_op);
                // if the rule is valid, just write to the .code section,
                // may need to edit others, worry about that later.

                // update context for later functions
                context->token_to_scan = yylex();
                return 0;
              } else {
                context->set_error(MISSING_SEMICOLON);
              }

            } else {
              context->set_error(MISSING_OPERAND);
            }

          } else {
            context->set_error(MISSING_COMMA);
          }
        } else {
          context->set_error(MISSING_OPERAND);
        }
      } else {
        context->set_error(MISSING_DATA_TYPE);
      }
      return 1;
    } else {  // use BrigInstBase
       // default value.
      BrigInstBase inst_op = {
      sizeof(inst_op),
      BrigEInstBase,
      opcode,
      Brigb32,
      BrigNoPacking,
      {0, 0, 0, 0, 0}
      };

      // check whether there is a Packing (optional)
      if (context->token_type == PACKING) {
        // there is packing
        inst_op.packing = context->token_value.packing;
        context->token_to_scan = yylex();
      }

      // now we must have a dataTypeId
      if (context->token_type == DATA_TYPE_ID) {
        // check the operands
        inst_op.type = context->token_value.data_type;

        context->token_to_scan = yylex();  // set context for Operand()
        std::string oper_str = context->token_value.string_val;

        if (!Operand(context)) {
          inst_op.o_operands[0] = context->operand_map[oper_str];
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();  // set context for Operand()
            oper_str = context->token_value.string_val;

            if (!Operand(context)) {
              inst_op.o_operands[1] = context->operand_map[oper_str];
              if (context->token_to_scan == ';') {
                context->append_code(&inst_op);
                // if the rule is valid, just write to the .code section,
                // may need to edit others, worry about that later.

                // set context for later functions
                context->token_to_scan = yylex();
                return 0;
              } else {
                context->set_error(MISSING_SEMICOLON);
              }

            } else {
              context->set_error(MISSING_OPERAND);
            }

          } else {
            context->set_error(MISSING_COMMA);
          }
        } else {
          context->set_error(MISSING_OPERAND);
        }
      } else {
        context->set_error(MISSING_DATA_TYPE);
      }
      return 1;
    }
  } else if (context->token_type == INSTRUCTION2_OPCODE_NODT) {
    context->token_to_scan = yylex();  // set context for RoundingMode
    if (!RoundingMode(context)) {
      // check the operands
      if (!Operand(context)) {
        if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();  // set context for Operand()
          if (!Operand(context)) {
            if (context->token_to_scan == ';') {
              // set context for later functions
              context->token_to_scan = yylex();
              return 0;
            } else {
              context->set_error(MISSING_SEMICOLON);
            }
          } else {
            context->set_error(MISSING_OPERAND);
          }
        } else {
          context->set_error(MISSING_COMMA);
        }
      } else {
        context->set_error(MISSING_OPERAND);
      }
      return 1;
    }else{
      // use BrigInstBase
       // default value.
      BrigInstBase inst_op = {
      sizeof(inst_op),
      BrigEInstBase,
      opcode,
      Brigb32,
      BrigNoPacking,
      {0, 0, 0, 0, 0}
      };

      std::string oper_str = context->token_value.string_val;

      if (!Operand(context)) {
        inst_op.o_operands[0] = context->operand_map[oper_str];
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();  // set context for Operand()
          oper_str = context->token_value.string_val;

          if (!Operand(context)) {
            inst_op.o_operands[1] = context->operand_map[oper_str];
            if (context->token_to_scan == ';') {
              context->append_code(&inst_op);
              // if the rule is valid, just write to the .code section,
              // may need to edit others, worry about that later.

              // set context for later functions
              context->token_to_scan = yylex();
              return 0;
            } else {
              context->set_error(MISSING_SEMICOLON);
            }

          } else {
            context->set_error(MISSING_OPERAND);
          }

        } else {
          context->set_error(MISSING_COMMA);
        }
      } else {
        context->set_error(MISSING_OPERAND);
      }
      return 1;
    }
  } else if (context->token_type == INSTRUCTION2_OPCODE_FTZ) {
    context->token_to_scan = yylex();
    // Optional FTZ
    if (context->token_to_scan == _FTZ) {  // use BrigInstMod
      // has a _ftz
      BrigAluModifier bam;
      bam.ftz = 1;
      context->set_alu_modifier(bam);

      yylex();  // get next token

      // default value.
      BrigInstMod inst_op = {
        sizeof(inst_op),    // size
        BrigEInstMod,       // kind
        opcode,  // opcode
        0,  // type
        BrigNoPacking,    // packing
        {0, 0, 0, 0, 0},   // o_operands
        context->get_alu_modifier()   // aluModifier;
      };

      // now we must have a dataTypeId
      if (context->token_type == DATA_TYPE_ID) {
        // check the operands
        inst_op.type = context->token_value.data_type;


        context->token_to_scan = yylex();  // set context for Operand()
        std::string oper_str = context->token_value.string_val;

        if (!Operand(context)) {
          inst_op.o_operands[0] = context->operand_map[oper_str];
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();  // set context for Operand()
            oper_str = context->token_value.string_val;

            if (!Operand(context)) {
              inst_op.o_operands[1] = context->operand_map[oper_str];
              if (context->token_to_scan == ';') {
                context->append_code(&inst_op);
                // if the rule is valid, just write to the .code section,
                // may need to edit others, worry about that later.

                // set context for later functions
                context->token_to_scan = yylex();
                return 0;
              } else {
                context->set_error(MISSING_SEMICOLON);
              }
            } else {
              context->set_error(MISSING_OPERAND);
            }
          } else {
            context->set_error(MISSING_COMMA);
          }
        } else {
          context->set_error(MISSING_OPERAND);
        }
      } else {
        context->set_error(MISSING_DATA_TYPE);
      }
      return 1;

    } else {  // use BrigInstBase
      // default value.
      BrigInstBase inst_op = {
      sizeof(inst_op),
      BrigEInstBase,
      opcode,
      Brigb32,
      BrigNoPacking,
      {0, 0, 0, 0, 0}
      };

      // now we must have a dataTypeId
      if (context->token_type == DATA_TYPE_ID) {
        // check the operands
        inst_op.type = context->token_value.data_type;

        context->token_to_scan = yylex();  // set context for Operand()
        std::string oper_str = context->token_value.string_val;

        if (!Operand(context)) {
          inst_op.o_operands[0] = context->operand_map[oper_str];
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();  // set context for Operand()
            oper_str = context->token_value.string_val;

            if (!Operand(context)) {
              inst_op.o_operands[1] = context->operand_map[oper_str];
              if (context->token_to_scan == ';') {
                context->append_code(&inst_op);
                // if the rule is valid, just write to the .code section,
                // may need to edit others, worry about that later.

                context->token_to_scan = yylex();  // set context for later
                return 0;
              } else {
                context->set_error(MISSING_SEMICOLON);
              }
            } else {
              context->set_error(MISSING_OPERAND);
            }
          } else {
            context->set_error(MISSING_COMMA);
          }
        } else {
          context->set_error(MISSING_OPERAND);
        }
      } else {
        context->set_error(MISSING_DATA_TYPE);
      }
      return 1;
    }
  } else {
    return 1;
  }
}

int Instruction3(Context* context) {
  // First token must be an Instruction3Opcode
  BrigOpcode32_t opcode = context->token_value.opcode;

  // default value.
  BrigInstBase inst_op = {
    32,
    BrigEInstBase,
    opcode,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };

  if (context->token_type == INSTRUCTION3_OPCODE) {
    context->token_to_scan= yylex();
    if (!RoundingMode(context)) {
    }

    // check whether there is a Packing
    if (context->token_type == PACKING) {
      // there is packing
      inst_op.packing = context->token_value.packing;
      yylex();
    }

    // now we must have a dataTypeId
    if (context->token_type == DATA_TYPE_ID) {
      // check the operands
      inst_op.type = context->token_value.data_type;
      context->token_to_scan = yylex();
      std::string oper_str = context->token_value.string_val;

      if (!Operand(context)) {
        inst_op.o_operands[0] = context->operand_map[oper_str];
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
          oper_str = context->token_value.string_val;

          if (!Operand(context)) {
            inst_op.o_operands[1] = context->operand_map[oper_str];
            if (context->token_to_scan == ',') {
              context->token_to_scan = yylex();
              oper_str = context->token_value.string_val;

              if (!Operand(context)) {
                inst_op.o_operands[2] = context->operand_map[oper_str];
                if (context->token_to_scan == ';') {
                  context->append_code(&inst_op);
                  context->token_to_scan = yylex();
                  return 0;
                } else {
                  context->set_error(MISSING_SEMICOLON);
                }
              } else {  // 3rd operand
                context->set_error(MISSING_OPERAND);
              }
            } else {  // 2nd comma
              context->set_error(MISSING_COMMA);
            }
          } else {  // 2nd operand
            context->set_error(MISSING_OPERAND);
          }
        } else {  // 1st comma
          context->set_error(MISSING_COMMA);
        }
      } else {  // 1st operand
        context->set_error(MISSING_OPERAND);
      }
    } else {  // DATA_TYPE_ID
      context->set_error(MISSING_DATA_TYPE);
    }
    return 1;
  } else if (context->token_type == INSTRUCTION3_OPCODE_FTZ) {
    // Optional FTZ
    if (yylex() == _FTZ) {
      // has a _ftz
      yylex();
    }

    // check whether there is a Packing
    if (context->token_type == PACKING) {
      // there is packing
      inst_op.packing = context->token_value.packing;
      context->token_to_scan = yylex();
    }
    // now we must have a dataTypeId
    if (context->token_type == DATA_TYPE_ID) {
      // check the operands
      inst_op.type = context->token_value.data_type;

      context->token_to_scan = yylex();
      std::string oper_str = context->token_value.string_val;

      if (!Operand(context)) {
        inst_op.o_operands[0] = context->operand_map[oper_str];
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
          oper_str = context->token_value.string_val;

          if (!Operand(context)) {
            inst_op.o_operands[1] = context->operand_map[oper_str];
            if (context->token_to_scan == ',') {
              context->token_to_scan = yylex();
              oper_str = context->token_value.string_val;

              if (!Operand(context)) {
                inst_op.o_operands[2] = context->operand_map[oper_str];
                if (context->token_to_scan == ';') {
                  context->append_code(&inst_op);
                  context->token_to_scan = yylex();
                  return 0;
                } else {
                  context->set_error(MISSING_SEMICOLON);
                }
              } else {  // 3rd operand
                context->set_error(MISSING_OPERAND);
              }
            } else {  // 2nd comma
              context->set_error(MISSING_COMMA);
            }
          } else {  // 2nd operand
            context->set_error(MISSING_OPERAND);
          }
        } else {  // 1st comma
          context->set_error(MISSING_COMMA);
        }
      } else {  // 1st operand
        context->set_error(MISSING_OPERAND);
      }
    } else {  // DATA_TYPE_ID
      context->set_error(MISSING_DATA_TYPE);
    }
    return 1;
  } else {
    return 1;
  }
}

int Version(Context* context) {
  // first token must be version keyword
  BrigDirectiveVersion bdv;
  bdv.kind = BrigEDirectiveVersion;
  bdv.size = 20;
  bdv.reserved = 0;

  // set default values
  bdv.machine = BrigELarge;
  bdv.profile = BrigEFull;
  bdv.ftz = BrigENosftz;
  context->token_to_scan = yylex();
  if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
    bdv.major = context->token_value.int_val;
    context->token_to_scan = yylex();
    if (context->token_to_scan == ':') {
      context->token_to_scan = yylex();
      // check for minor
      if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
        bdv.minor = context->token_value.int_val;
        context->token_to_scan = yylex();

        if (context->token_to_scan == ';') {
        } else if (context->token_to_scan == ':') {
          // check for target
          context->token_to_scan = yylex();
          while (context->token_to_scan != ';') {
            if (context->token_type == TARGET) {
              switch (context->token_to_scan) {
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

              // update context
              context->set_machine(bdv.machine);
              context->set_profile(bdv.profile);
              context->set_ftz(bdv.ftz);

              context->token_to_scan = yylex();
              if (context->token_to_scan == ',') {
                context->token_to_scan = yylex();      // next target
              } else {
                if (context->token_to_scan != ';') {
                  context->set_error(MISSING_SEMICOLON);
                  return 1;
                }
              }
            } else {
              context->set_error(INVALID_TARGET);
              return 1;
            }
          }
        } else {
          context->set_error(MISSING_SEMICOLON);
          return 1;
        }
        context->append_directive(&bdv);
        context->token_to_scan = yylex();
        context->set_error(OK);
        return 0;
      } else {
      context->set_error(MISSING_INTEGER_CONSTANT);
      }
    } else {
    context->set_error(MISSING_COLON);
    }
  } else {
    context->set_error(MISSING_INTEGER_CONSTANT);
  }

  return 1;
}

int Alignment(Context* context) {
  // first token must be "align" keyword
  context->token_to_scan = yylex();
  if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
    context->set_alignment(context->token_value.int_val);
    context->token_to_scan = yylex();
    return 0;
  } else {
    context->set_error(MISSING_INTEGER_CONSTANT);
    return 1;
  }
}

int DeclPrefix(Context* context) {
  if (context->token_to_scan == ALIGN) {
    if (!Alignment(context)) {
      // first is alignment
      if (context->token_to_scan == CONST) {
        context->set_symbol_modifier(BrigConst);
        // alignment const
        context->token_to_scan = yylex();

        if ((context->token_to_scan == EXTERN) ||
            (context->token_to_scan == STATIC)) {
          if (context->token_to_scan == EXTERN)
            context->set_attribute(BrigExtern);
          else
            context->set_attribute(BrigStatic);

          // alignment const externOrStatic
          context->token_to_scan = yylex();
        }
      } else if ((context->token_to_scan == EXTERN) ||
                 (context->token_to_scan == STATIC)) {
        // alignment externOrStatic
        if (context->token_to_scan == EXTERN)
          context->set_attribute(BrigExtern);
        else
          context->set_attribute(BrigStatic);

        context->token_to_scan = yylex();

        if (context->token_to_scan == CONST) {
          // alignment externOrStatic const
          context->set_symbol_modifier(BrigConst);
          context->token_to_scan = yylex();
        }
      }
    } else {
      context->set_error(INVALID_ALIGNMENT);
    }
  } else if (context->token_to_scan == CONST) {
    // first is const
    context->set_symbol_modifier(BrigConst);
    context->token_to_scan = yylex();

    if (context->token_to_scan == ALIGN) {
      if (!Alignment(context)) {
        // const alignment
       if ((context->token_to_scan == EXTERN) ||
           (context->token_to_scan == STATIC)) {
         // const alignment externOrStatic
         if (context->token_to_scan == EXTERN)
            context->set_attribute(BrigExtern);
          else
            context->set_attribute(BrigStatic);

          context->token_to_scan = yylex();
        }
      } else {
        context->set_error(INVALID_ALIGNMENT);
      }
    } else if ((context->token_to_scan == EXTERN) ||
               (context->token_to_scan == STATIC)) {
      // const externOrStatic
      if (context->token_to_scan == EXTERN)
        context->set_attribute(BrigExtern);
      else
        context->set_attribute(BrigStatic);

      context->token_to_scan = yylex();

      if (context->token_to_scan == ALIGN) {
        if (!Alignment(context)) {
          // const externOrStatic alignment
        } else {
          context->set_error(INVALID_ALIGNMENT);
          return 1;
        }
      }
    } else {  // const does not stand alone
      return 1;
    }
  } else if ((context->token_to_scan == EXTERN) ||
             (context->token_to_scan == STATIC)) {
    // externOrStatic first
    if (context->token_to_scan == EXTERN)
      context->set_attribute(BrigExtern);
    else
      context->set_attribute(BrigStatic);

    context->token_to_scan = yylex();

    if (context->token_to_scan == ALIGN) {
      if (!Alignment(context)) {
        // externOrStatic alignment
        if (context->token_to_scan == CONST) {
          // externOrStatic alignment const
          context->set_symbol_modifier(BrigConst);
          context->token_to_scan = yylex();
        }
      } else {
        context->set_error(INVALID_ALIGNMENT);
      }
    } else if (context->token_to_scan == CONST) {
      // externOrStatic const
      context->set_symbol_modifier(BrigConst);

      context->token_to_scan = yylex();

      if (context->token_to_scan == ALIGN) {
        if (!Alignment(context)) {
        } else {
          context->set_error(INVALID_ALIGNMENT);
          return 1;
        }
        // externOrStatic const alignment
      }
    }
  }
  return 0;
}

int FBar(Context* context) {
  // first token must be _FBAR
  context->token_to_scan = yylex();
  if (context->token_to_scan == '(') {
    context->token_to_scan = yylex();
    if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
      context->set_fbar(context->token_value.int_val);
      context->token_to_scan = yylex();
      if (context->token_to_scan == ')') {
        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(MISSING_CLOSING_PARENTHESIS);
      }
    } else {
      context->set_error(MISSING_INTEGER_CONSTANT);
    }
  }
  return 1;
}

int ArrayDimensionSet(Context* context) {
  // first token must be '['
  uint32_t dim = 1;
  bool have_size = false;
  context->set_isArray(true);
  context->token_to_scan = yylex();

  while (1) {
    if (context->token_to_scan == ']') {
      context->token_to_scan = yylex();  // check if there is more item
      if (context->token_to_scan == '[') {  // more item
        context->token_to_scan = yylex();
      } else {  // no more item
        break;
      }
    } else if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
      have_size = true;
      dim *= context->token_value.int_val;
      context->token_to_scan = yylex();  // scan next
    } else {
      context->set_error(MISSING_CLOSING_BRACKET);
      return 1;
    }
  }
  if (!have_size){
    context->set_dim(0);// flexiable array
    context->set_symbol_modifier(BrigFlex);
  } else {
    context->set_dim(dim); // vector(size in dim)
    context->set_symbol_modifier(BrigArray);
  }
  return 0;
}

int ArgumentDecl(Context* context) {
  if (!DeclPrefix(context)) {
    BrigStorageClass32_t storage_class = context->token_value.storage_class;

    // skip over "arg" in context->token_to_scan
    context->token_to_scan = yylex();
    if ((context->token_type == DATA_TYPE_ID)||
        (context->token_to_scan == _RWIMG) ||
        (context->token_to_scan == _SAMP) ||
        (context->token_to_scan == _ROIMG)) {
      context->set_type(context->token_value.data_type);
      context->token_to_scan = yylex();
      if (context->token_to_scan == TOKEN_LOCAL_IDENTIFIER) {
        // for argument, we need to set a BrigDirectiveSymbol
        // and write the corresponding string into .string section.

        std::string arg_name = context->token_value.string_val;
        int arg_name_offset = context->add_symbol(arg_name);

        // scan for arrayDimensions
        context->token_to_scan = yylex();
        // set default value(scalar)
        context->set_dim(0);
        context->set_symbol_modifier(BrigArray);
        if (context->token_to_scan == '[') {
          if (!ArrayDimensionSet(context)) {
            // context->token_to_scan has been set in ArrayDimensionSet()
            return 0;
          }
        }
        BrigdOffset32_t dsize = context->get_directive_offset();
        BrigDirectiveSymbol sym_decl = {
        sizeof(sym_decl),                 // size
        BrigEDirectiveSymbol,             // kind
        {
          context->get_code_offset(),       // c_code
          storage_class,                    // storageClass
          context->get_attribute(),         // attribute
          0,                                // reserved
          context->get_symbol_modifier(),   // symbol modifier
          context->get_dim(),               // dim
          arg_name_offset,                  // s_name
          context->get_type(),              // data type
          context->get_alignment(),         // alignment
        },
        0,                                // d_init = 0 for arg
        0                                 // reserved
        };
        // append the DirectiveSymbol to .directive section.
        context->append_directive(&sym_decl);
        context->symbol_map[arg_name] = dsize;

        // update the current DirectiveFunction.
        // 1. update the directive offset.
        BrigDirectiveFunction bdf;
        context->get_directive(context->current_bdf_offset, &bdf);
        BrigdOffset32_t first_scope = bdf.d_firstScopedDirective;
        BrigdOffset32_t next_directive = bdf.d_nextDirective;
        if (first_scope == next_directive) {
          bdf.d_nextDirective += sizeof(sym_decl);
          bdf.d_firstScopedDirective = bdf.d_nextDirective;
        } else {
          bdf.d_nextDirective += sizeof(sym_decl);
        }

        // update param count
        if (context->is_arg_output()) {
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

        context->get_directive(context->current_bdf_offset, &bdf);
        return 0;

      } else {
        context->set_error(MISSING_IDENTIFIER);
      }
    } else {
      context->set_error(MISSING_DATA_TYPE);
    }
  }

  return 1;
}

int ArgumentListBody(Context* context) {
  while (1) {
    if (!ArgumentDecl(context)) {
      if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
      } else {
        break;  // context was set in ArgumentDecl
      }
    } else {
      context->set_error(MISSING_ARGUMENT);
      return 1;
    }
  }
  return 0;
}

int FunctionDefinition(Context* context) {
  if (!DeclPrefix(context)) {
    if (context->token_to_scan == FUNCTION) {
      // add default struct (Miao)

      context->current_bdf_offset = context->get_directive_offset();
      BrigdOffset32_t bdf_offset = context->current_bdf_offset;

      BrigDirectiveFunction bdf = {
      40,                      // size
      BrigEDirectiveFunction,  // kind
      context->get_code_offset(),   // c_code
      0,  // name
      0,  // in param count
      bdf_offset+40,          // d_firstScopedDirective
      0,  // operation count
      bdf_offset+40,          // d_nextDirective
      context->get_attribute(),  // attribute
      context->get_fbar(),   // fbar count
      0,    // out param count
      0     // d_firstInParam
      };


      // update it when necessary.
      // the later functions should have a entry point of bdf
      // just update it in time.
      //
      context->token_to_scan = yylex();
      if (context->token_to_scan == TOKEN_GLOBAL_IDENTIFIER) {
        // should have meaning of Global_Identifier,
        // and check if there is existing global identifier
        // if there is, just use the current string,
        // if not write into string.

        std::string func_name = context->token_value.string_val;

        BrigsOffset32_t check_result = context->add_symbol(func_name);

        // add the func_name to the func_map.
        context->func_map[func_name] = context->current_bdf_offset;

        bdf.s_name = check_result;
        context->append_directive(&bdf);

        /* Debug */
        // BrigDirectiveFunction get;

        // check return argument list
        context->token_to_scan = yylex();
        if (context->token_to_scan == '(') {
          context->set_arg_output(true);
          context->token_to_scan = yylex();

          if (context->token_to_scan == ')') {   // empty argument list body
            context->token_to_scan = yylex();
          } else if (!ArgumentListBody(context)) {
            if (context->token_to_scan == ')') {
              context->token_to_scan = yylex();
            } else {
              context->set_error(MISSING_CLOSING_PARENTHESIS);
              return 1;
            }
          } else {
            context->set_error(INVALID_ARGUMENT_LIST);
            return 1;
          }
        } else {
          context->set_error(MISSING_ARGUMENT_LIST);
          return 1;
        }
        // check argument list
        if (context->token_to_scan == '(') {
          context->set_arg_output(false);
          context->token_to_scan = yylex();

          if (context->token_to_scan == ')') {   // empty argument list body
              context->token_to_scan = yylex();
          } else if (!ArgumentListBody(context)) {
            if (context->token_to_scan == ')') {
              context->token_to_scan = yylex();
            } else {
              context->set_error(MISSING_CLOSING_PARENTHESIS);
              return 1;
            }
          } else {
            context->set_error(INVALID_ARGUMENT_LIST);
            return 1;
          }
        } else {
          context->set_error(MISSING_ARGUMENT_LIST);
          return 1;
        }
        // check for optional FBar
        if (context->token_to_scan == _FBAR) {
          if (!FBar(context)) {
            // context->token_to_scan = yylex();
            return 0;
          } else {
            context->set_error(INVALID_FBAR);
          }
        } else {
          return 0;
        }
      } else {
        context->set_error(MISSING_IDENTIFIER);
      }
    }
  }
  return 1;
}

/*
int FunctionDeclpart1(Context* context) {
  //if (!DeclPrefix(context)) {
    if (context->token_to_scan == FUNCTION) {
      context->token_to_scan = yylex();
      if (context->token_to_scan == TOKEN_GLOBAL_IDENTIFIER) {
        // check return argument list
        context->token_to_scan = yylex();
        if (context->token_to_scan == '(') {
          context->token_to_scan = yylex();

          if (context->token_to_scan == ')') {   // empty argument list body
              context->token_to_scan = yylex();
          } else if (!ArgumentListBody(context)) {
            if (context->token_to_scan == ')') {
                context->token_to_scan = yylex();
            } else {
              context->set_error(MISSING_CLOSING_PARENTHESIS);
              return 1;
            }
          } else {
            context->set_error(INVALID_ARGUMENT_LIST);
            return 1;
          }
        } else {
          context->set_error(MISSING_ARGUMENT_LIST);
          return 1;
        }
        // check argument list
        if (context->token_to_scan == '(') {
            context->token_to_scan = yylex();

          if (context->token_to_scan == ')') {   // empty argument list body
              context->token_to_scan = yylex();
          } else if (!ArgumentListBody(context)) {
            if (context->token_to_scan == ')') {
                context->token_to_scan = yylex();
            } else {
              context->set_error(MISSING_CLOSING_PARENTHESIS);
              return 1;
            }
          } else {
            context->set_error(INVALID_ARGUMENT_LIST);
            return 1;
          }
        } else {
          context->set_error(MISSING_ARGUMENT_LIST);
          return 1;
        }


        // check for optional FBar
        if (context->token_to_scan == _FBAR) {
          if (!FBar(context)) {
            return 0;
          } else {
            context->set_error(INVALID_FBAR);
          }
        } else {
          return 0;
        }
      } else {
        context->set_error(MISSING_IDENTIFIER);
      }
    }
 // } else {
 //   context->set_error(MISSING_DECLPREFIX);
 // }
  return 1;
}

int FunctionDeclpart2(Context *context){
  if (';' == context->token_to_scan){
    context->token_to_scan = yylex();
    return 0 ;
  }

  return 1;
}*/
int FunctionDecl(Context *context){
  if (!DeclPrefix(context)) {
    if (!FunctionDefinition(context)){
      if (';' == context->token_to_scan){
        return 0 ;
      } else {
        context->set_error(MISSING_IDENTIFIER);
      }
    }
  }
  return 1;
}

int ArgBlock(Context* context) {
  // first token should be {
  // add BrigDirectiveScope
  BrigDirectiveScope argblock_start = {
    8,
    BrigEDirectiveArgStart,
    context->get_code_offset()
  };

  // needed for update d_firstScopedDirective
  BrigdOffset32_t arg_offset = context->get_directive_offset();
  context->append_directive(&argblock_start);

  // update the d_nextDirective.
  BrigDirectiveFunction bdf;
  context->get_directive(context->current_bdf_offset, &bdf);
  if (bdf.d_firstScopedDirective == bdf.d_nextDirective)
    // check if the firstScopedDirective is modified before.
    bdf.d_firstScopedDirective = arg_offset;
  unsigned char * bdf_charp = reinterpret_cast<unsigned char*>(&bdf);
  context->update_directive_bytes(bdf_charp,
                                  context->current_bdf_offset,
                                  bdf.size);

  context->token_to_scan = yylex();
  while (1) {
    if ((context->token_type == INSTRUCTION2_OPCODE) ||
        (context->token_type == INSTRUCTION2_OPCODE_NODT) ||
        (context->token_type == INSTRUCTION2_OPCODE_FTZ)) {
      // Instruction 2 Operation
      if (!Instruction2(context)) {
        // update the operationCount.
        BrigDirectiveFunction bdf;
        context->get_directive(context->current_bdf_offset, &bdf);
        bdf.operationCount++;

        unsigned char * bdf_charp =
          reinterpret_cast<unsigned char*>(&bdf);
        context->update_directive_bytes(bdf_charp,
                                        context->current_bdf_offset,
                                        bdf.size);
      } else {
        return 1;
      }
    } else if ((context->token_type == INSTRUCTION3_OPCODE) ||
               (context->token_type == INSTRUCTION3_OPCODE_FTZ)) {
      // Instruction 3 Operation
      if (!Instruction3(context)) {
        // update the operationCount.
        BrigDirectiveFunction bdf;
        context->get_directive(context->current_bdf_offset, &bdf);
        bdf.operationCount++;

        unsigned char * bdf_charp =
          reinterpret_cast<unsigned char*>(&bdf);
        context->update_directive_bytes(bdf_charp,
                                        context->current_bdf_offset,
                                        bdf.size);
      } else {
        return 1;
      }
    } else if (context->token_type == QUERY_OP) {  // Query Operation
      if (!Query(context)) {
      } else {
        return 1;
      }
    } else if (context->token_to_scan == RET) {  // ret operation
      if (yylex() == ';') {
        BrigInstBase op_ret = {
          32,
          BrigEInstBase,
          BrigRet,
          Brigb32,
          BrigNoPacking,
          {0, 0, 0, 0, 0}
          };
        // write to .code section
        context->append_code(&op_ret);
        BrigDirectiveFunction bdf;
        context->get_directive(context->current_bdf_offset, &bdf);
        bdf.operationCount++;

        unsigned char * bdf_charp =
          reinterpret_cast<unsigned char*>(&bdf);
        context->update_directive_bytes(bdf_charp,
                                        context->current_bdf_offset,
                                        bdf.size);
        context->token_to_scan = yylex();
      } else {
        context->set_error(MISSING_SEMICOLON);
        return 1;
      }
    } else if ((context->token_to_scan == BRN) ||
               (context->token_to_scan == CBR)) {
      if (!Branch(context)) {
      } else {
        return 1;
      }
    } else if (context->token_to_scan == TOKEN_LABEL) {  // label
      // need to check the lable_map, and update when necessary.
      // std::string label_name = context->token_value.string_val;
      // context->lable_o_map[label_name] = context->get_operand_offset();
      // set the lable_map
      // add operand in .operand;
      // check if any entry with the same key in label_c_map;
      // update the corresponding instructions.
      //
      // if no correlated instructions, then later instructions
      // can directly use the label.
      if (context->token_to_scan == ':') {
        context->token_to_scan = yylex();
      } else {
        context->set_error(MISSING_COLON);
        return 1;
      }
    } else if (context->token_to_scan == CALL) {  // call (only inside argblock
      if (!Call(context)) {
        // update the operationCount.
        BrigDirectiveFunction bdf;
        context->get_directive(context->current_bdf_offset, &bdf);
        bdf.operationCount++;

        unsigned char * bdf_charp =
          reinterpret_cast<unsigned char*>(&bdf);
        context->update_directive_bytes(bdf_charp,
                                        context->current_bdf_offset,
                                        bdf.size);
      } else {
        return 1;
      }
    } else if ((context->token_to_scan == ALIGN) ||
               (context->token_to_scan == CONST) ||
               (context->token_to_scan == STATIC) ||
               (context->token_to_scan == EXTERN)) {
      if (!DeclPrefix(context)) {
        if (context->token_type == INITIALIZABLE_ADDRESS) {
          // initializable decl
          if (!InitializableDecl(context)) {
          }
        } else if (context->token_type == UNINITIALIZABLE_ADDRESS) {
          // uninitializable decl
          if (!UninitializableDecl(context)) {
          }
        } else if (context->token_to_scan == ARG) {
          // arg uninitializable decl
          if (!ArgUninitializableDecl(context)) {
          }
        }
      }
    } else if (context->token_type == INITIALIZABLE_ADDRESS) {
      if (!InitializableDecl(context)) {
      } else {
        return 1;
      }
    } else if (context->token_type == UNINITIALIZABLE_ADDRESS) {
      if (!UninitializableDecl(context)) {
      } else {
        return 1;
      }
    } else if (context->token_to_scan == ARG) {
      if (!ArgUninitializableDecl(context)) {
      } else {
        return 1;
      }
    } else if (context->token_to_scan == '{') {
      context->set_error(INVALID_NESTED_ARGUMENT_SCOPE);
      return 1;
    } else if (context->token_to_scan == '}') {
      // add BrigDirectiveScope
      BrigDirectiveScope argblock_start = {
        8,
        BrigEDirectiveArgEnd,
        context->get_code_offset()
      };
      context->append_directive(&argblock_start);

      // update the d_nextDirective.
      BrigDirectiveFunction bdf;
      context->get_directive(context->current_bdf_offset, &bdf);
      bdf.d_nextDirective = context->get_directive_offset();
      unsigned char * bdf_charp = reinterpret_cast<unsigned char*>(&bdf);
      context->update_directive_bytes(bdf_charp,
                                      context->current_bdf_offset,
                                      bdf.size);
      context->token_to_scan = yylex();
      return 0;
    } else {
      break;
    }
  }
  return 1;
}

int Codeblock(Context* context) {
  // first token should be '{'
  context->token_to_scan = yylex();
  while (1) {
    if ((context->token_type == INSTRUCTION2_OPCODE) ||
        (context->token_type == INSTRUCTION2_OPCODE_NODT) ||
        (context->token_type == INSTRUCTION2_OPCODE_FTZ)) {
      // Instruction 2 Operation
      if (!Instruction2(context)) {
        // update the operationCount.
        BrigDirectiveFunction bdf;
        context->get_directive(context->current_bdf_offset, &bdf);
        bdf.operationCount++;

        unsigned char * bdf_charp =
          reinterpret_cast<unsigned char*>(&bdf);
        context->update_directive_bytes(bdf_charp,
                                        context->current_bdf_offset,
                                        sizeof(bdf));

      } else {
        return 1;
      }
    } else if ((context->token_type == INSTRUCTION3_OPCODE) ||
               (context->token_type == INSTRUCTION3_OPCODE_FTZ)) {
      // Instruction 3 Operation
      if (!Instruction3(context)) {
        // update the operationCount.
        BrigDirectiveFunction bdf;
        context->get_directive(context->current_bdf_offset, &bdf);
        bdf.operationCount++;

        unsigned char * bdf_charp =
          reinterpret_cast<unsigned char*>(&bdf);
        context->update_directive_bytes(bdf_charp,
                                        context->current_bdf_offset,
                                        bdf.size);
      } else {
        return 1;
      }
    } else if (context->token_type == QUERY_OP) {  // Query Operation
      if (!Query(context)) {
      } else {
        return 1;
      }
    } else if (context->token_to_scan == RET) {  // ret operation
      if (yylex() == ';') {
      BrigInstBase op_ret = {
        32,
        BrigEInstBase,
        BrigRet,
        Brigb32,
        BrigNoPacking,
        {0, 0, 0, 0, 0}
        };
      // write to .code section
      context->append_code(&op_ret);
      BrigDirectiveFunction bdf;
      context->get_directive(context->current_bdf_offset, &bdf);
      bdf.operationCount++;

      unsigned char * bdf_charp =
        reinterpret_cast<unsigned char*>(&bdf);
      context->update_directive_bytes(bdf_charp,
                                      context->current_bdf_offset,
                                      sizeof(bdf));

      context->token_to_scan = yylex();
      } else {
        context->set_error(MISSING_SEMICOLON);
        return 1;
      }
    } else if ((context->token_to_scan == BRN) ||
               (context->token_to_scan == CBR)) {
      if (!Branch(context)) {
      } else {
        return 1;
      }
    } else if (context->token_to_scan == TOKEN_LABEL) {  // label
      // add to the .directive section
      std::string label_name = context->token_value.string_val;

      BrigDirectiveLabel label_directive = {
        12,
        BrigEDirectiveLabel,
        context->get_code_offset(),
        context->add_symbol(label_name)
      };

      BrigdOffset32_t label_directive_offset = context->get_directive_offset();
      context->append_directive(&label_directive);

      // add to the .operand section
      BrigoOffset32_t label_operand_offset = context->get_operand_offset();
      // TODO(Chuang): BrigOperandLabelRef shouldn't be generated in here.
      // context->append_operand(&label_operand);
      // context->label_o_map[label_name] = label_operand_offset;

      // update the d_nextDirective.
      BrigDirectiveFunction bdf;
      context->get_directive(context->current_bdf_offset, &bdf);
      if (bdf.d_firstScopedDirective == bdf.d_nextDirective)
        // check if the firstScopedDirective is modified before.
        bdf.d_firstScopedDirective = label_directive_offset;
      bdf.d_nextDirective = context->get_directive_offset();
      unsigned char * bdf_charp = reinterpret_cast<unsigned char*>(&bdf);
      context->update_directive_bytes(bdf_charp,
                                      context->current_bdf_offset,
                                      bdf.size);

      // check if there are any operation in .code need update
      if (context->label_c_map.count(label_name)) {
      // previously, there maybe brn, cbr need label.
      // update all
        typedef std::multimap<std::string, BrigcOffset32_t>::size_type sz_type;
        sz_type entries = context->label_c_map.count(label_name);

        std::multimap<std::string, BrigcOffset32_t>::iterator iter =
                                        context->label_c_map.find(label_name);
        for (sz_type cnt = 0; cnt != entries; ++cnt, ++iter) {
          BrigcOffset32_t offset = iter->second;
          unsigned char* value =
                    reinterpret_cast<unsigned char*> (&label_operand_offset);
          context->update_code_bytes(value, offset, 4);
        }
      }

      context->token_to_scan = yylex();
      if (context->token_to_scan == ':') {
        context->token_to_scan = yylex();
      } else {
        context->set_error(MISSING_COLON);
        return 1;
      }
    } else if (context->token_to_scan == '{') {
      // argument scope -> inner codeblock
      if (!ArgBlock(context)) {
      } else {
        return 1;
      }
    } else if ((context->token_to_scan == ALIGN) ||
               (context->token_to_scan == CONST) ||
               (context->token_to_scan == STATIC) ||
               (context->token_to_scan == EXTERN)) {
      if (!DeclPrefix(context)) {
        if (context->token_type == INITIALIZABLE_ADDRESS) {
          // initializable decl
          if (!InitializableDecl(context)) {
          } else {
            return 1;
          }
        } else if (context->token_type == UNINITIALIZABLE_ADDRESS) {
          // uninitializable decl
          if (!UninitializableDecl(context)) {
          } else {
            return 1;
          }
        } else {
          return 1;
        }
      } else {
        return 1;
      }
    } else if (context->token_type == INITIALIZABLE_ADDRESS) {
      if (!InitializableDecl(context)) {
      } else {
        return 1;
      }
    } else if (context->token_type == UNINITIALIZABLE_ADDRESS) {
      if (!UninitializableDecl(context)) {
      } else {
        return 1;
      }
    } else if (context->token_to_scan == '}') {
      context->token_to_scan = yylex();
      return 0;
    } else {
      break;
    }
  }
  return 1;
}
int Functionpart2(Context *context){
  if (!Codeblock(context)){
    if (';' == context->token_to_scan){
      context->token_to_scan = yylex();
      return 0;
    } else {
      context->set_error(MISSING_SEMICOLON);
    }
  }
  return 1;
}
int Function(Context* context) {
  if (!FunctionDefinition(context)) {
    return Functionpart2(context);
  } else {
    context->set_error(INVALID_FUNCTION_DEFINITION);
  }
  return 1;
}

int Program(Context* context) {
  if (context->token_to_scan == VERSION) {
    if (!Version(context)) {
      // parse topLevelStatement
      return TopLevelStatements(context);
    }   // if (!Version)
  } else {
    context->set_error(MISSING_VERSION_STATEMENT);
  }
  return 1;
}

int OptionalWidth(Context* context) {
  // first token must be _WIDTH

  if (context->token_to_scan == _WIDTH) {
    BrigOperandImmed op_width = {
      24,
      BrigEOperandImmed,
      Brigb32,
      0,
      { 0 }
    } ;
    context->token_to_scan = yylex();
    if (context->token_to_scan == '(') {
      context->token_to_scan = yylex();
      if (context->token_to_scan == ALL) {
        op_width.bits.u = 0 ;

        context->token_to_scan = yylex();
      } else if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
        uint32_t n = context->token_value.int_val;
        if((1<= n && n <= 1024) && ((n&0x01) == 0))
          op_width.bits.u  = n ;
        else
          context->set_error(INVALID_WIDTH_NUMBER);

        context->token_to_scan = yylex();
      } else {
        context->set_error(MISSING_WIDTH_INFO);
        return 1;
      }
      if (context->token_to_scan == ')') {
        context->append_operand(&op_width);

        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(MISSING_CLOSING_PARENTHESIS);
        return 1;
      }
    } else {
      context->set_error(MISSING_WIDTH_INFO);
      return 1;
    }
  }
  return 1;
}

int Branch(Context* context) {
  unsigned int op = context->token_to_scan;  // CBR or BRN

  BrigAluModifier mod = context->get_alu_modifier();

  context->token_to_scan = yylex();
  // check for optionalWidth
  if (context->token_to_scan == _WIDTH) {
    if (!OptionalWidth(context)) {
    } else {
      return 1;
    }
  }

  // check for optional _fbar modifier
  if (context->token_to_scan == __FBAR) {
    mod.fbar = 1;
    context->set_alu_modifier(mod);
    context->token_to_scan = yylex();
  }

  // parse operands
  if (op == CBR) {
    // add structures for CBR.
    // default value.
    BrigInstBase inst_op = {
      32,
      BrigEInstBase,
      BrigCbr,
      Brigb32,  // no specification of datatype in Brn and Cbr.
      BrigNoPacking,
      {0, 0, 0, 0, 0}
    };

    if (context->token_to_scan == TOKEN_CREGISTER)
      inst_op.type = Brigb1;

    std::string operand_name = context->token_value.string_val;

    if (!Operand(context)) {
      inst_op.o_operands[1] = context->operand_map[operand_name];
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        if (context->token_to_scan == TOKEN_LABEL) {
          // if the next operand is label, which is the case in example4
          // 1. check if the label is already defined,
          // 2. if defined, just set it up
          // 3. if not, add it to the multimap

              std::string label_name = context->token_value.string_val;

              if (context->label_o_map.count(label_name)) {
                inst_op.o_operands[2] = context->label_o_map[label_name];
              } else {
                BrigOperandLabelRef opLabelRef = {
                  8,
                  BrigEOperandLabelRef,
                  -1
                };
                inst_op.o_operands[2] = context->get_operand_offset();
                context->label_o_map[label_name] = context->get_operand_offset();
                if (context->symbol_map.count(label_name)) {
                  opLabelRef.labeldirective = context->symbol_map[label_name];
                }
                context->append_operand(&opLabelRef);
              }

          context->token_to_scan = yylex();  // should be ';'
        } else if (!Identifier(context)) {
          context->token_to_scan = yylex();  // should be ';'
        } else if (!Operand(context)) {
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();

            if (context->token_to_scan == TOKEN_LABEL) {  // LABEL
              context->token_to_scan = yylex();  // should be ';'
            } else if (context->token_to_scan == '[') {
              context->token_to_scan = yylex();
              if (!Identifier(context)) {
                context->token_to_scan = yylex();  // should be ']'
              } else if (context->token_to_scan == TOKEN_LABEL) {
                context->token_to_scan = yylex();

                while (context->token_to_scan != ']') {
                  if (context->token_to_scan == ',') {
                    if (yylex() == TOKEN_LABEL) {
                      context->token_to_scan = yylex();  // scan next;
                    } else {
                      context->set_error(MISSING_OPERAND);
                      return 1;
                    }
                  } else {
                    context->set_error(MISSING_COMMA);
                    return 1;
                  }
                }   // while
              } else {
                context->set_error(MISSING_OPERAND);
              }
              // current token should be ']'
              context->token_to_scan = yylex();  // should be ';'
            } else {
              context->set_error(MISSING_OPERAND);
            }
          }  else {  // yylex() = ','
            context->set_error(MISSING_COMMA);
          }
        } else {
          context->set_error(MISSING_OPERAND);
          return 1;
        }
        if (context->token_to_scan == ';') {
          context->append_code(&inst_op);
          // update the operationCount.
          BrigDirectiveFunction bdf;
          context->get_directive(context->current_bdf_offset, &bdf);
          bdf.operationCount++;
          unsigned char * bdf_charp = reinterpret_cast<unsigned char*>(&bdf);
          context->update_directive_bytes(bdf_charp,
                                          context->current_bdf_offset,
                                          sizeof(bdf));
          context->token_to_scan = yylex();
          return 0;
        } else {
          context->set_error(MISSING_SEMICOLON);
        }
      } else {  // yylex = ','
        context->set_error(MISSING_COMMA);
      }
    } else {  // first operand
      context->set_error(MISSING_OPERAND);
    }
    return 1;
  } else if (op == BRN) {
    // add structures for CBR.
    // default value.
    BrigInstBar inst_op = {
      36,
      BrigEInstBar,
      BrigBrn,
      0,  // no specification of datatype in Brn and Cbr.
      BrigNoPacking,
      {0, 0, 0, 0, 0},
      0
    };

    if (context->token_to_scan == TOKEN_LABEL) {
      // if the next operand is label, which is the case in example4
      // 1. check if the label is already defined,
      // 2. if defined, just set it up
      // 3. if not, add it to the multimap
              std::string label_name = context->token_value.string_val;

              if (context->label_o_map.count(label_name)) {
                inst_op.o_operands[1] = context->label_o_map[label_name];
              } else {
                BrigOperandLabelRef opLabelRef = {
                  8,
                  BrigEOperandLabelRef,
                  -1
                };
                inst_op.o_operands[1] = context->get_operand_offset();
                context->label_o_map[label_name] = context->get_operand_offset();
                if (context->symbol_map.count(label_name)) {
                  opLabelRef.labeldirective = context->symbol_map[label_name];
                }
                context->append_operand(&opLabelRef);
              }
      context->token_to_scan = yylex();
      if (context->token_to_scan == ';') {
        context->append_code(&inst_op);
          // update the operationCount.
          BrigDirectiveFunction bdf;
          context->get_directive(context->current_bdf_offset, &bdf);
          bdf.operationCount++;
          unsigned char * bdf_charp = reinterpret_cast<unsigned char*>(&bdf);
          context->update_directive_bytes(bdf_charp,
                                          context->current_bdf_offset,
                                          sizeof(bdf));
        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(MISSING_SEMICOLON);
      }
    } else if (!Identifier(context)) {
        context->token_to_scan = yylex();

      if (context->token_to_scan == ';') {
        context->token_to_scan = yylex();
        return 0;
      } else if (context->token_to_scan == ',') {
        if (yylex() == '[') {
          context->token_to_scan = yylex();

        if (context->token_to_scan == TOKEN_LABEL) {
          context->token_to_scan = yylex();    // should be ']'
        } else if (!Identifier(context)) {
          context->token_to_scan = yylex();    // should be ']'
          }
        }

        if (context->token_to_scan == ']') {
        } else {
          context->set_error(MISSING_CLOSING_BRACKET);
        }

        context->token_to_scan = yylex();
        if (context->token_to_scan == ';') {
          context->token_to_scan = yylex();
          return 0;
        } else {
          context->set_error(MISSING_SEMICOLON);
        }
      } else {
        context->set_error(MISSING_SEMICOLON);
      }
    } else {
      context->set_error(MISSING_OPERAND);
    }
    return 1;
  }
  return 1;
}

int Call(Context* context) {
  // first token is "call"

  context->token_to_scan = yylex();
  // optional width
  if (context->token_to_scan == _WIDTH) {
    if (!OptionalWidth(context)) {
    } else {
      return 1;
    }
  }

  unsigned int temp = context->token_to_scan;
  std::string func_name;
  if (temp == TOKEN_GLOBAL_IDENTIFIER) {
    func_name.assign(context->token_value.string_val);
  }

  if (!Operand(context)) {
    // the operand should be a register or a Func name.
    // Assuming the function must be defined before the call.

    // Default Structure
    BrigInstBase call_op = {
      32,
      BrigEInstBase,
      BrigCall,
      Brigb32,
      BrigNoPacking,
      {0, 0, 0, 0, 0}
    };

    // If Call by its name.
    if (temp == TOKEN_GLOBAL_IDENTIFIER) {
      if (context->func_o_map.count(func_name)) {
        call_op.o_operands[2] = context->func_o_map[func_name];
      } else {
        BrigOperandFunctionRef func_o_ref = {
          8,
          BrigEOperandFunctionRef,
          context->func_map[func_name]
        };
        context->func_o_map[func_name] = context->get_operand_offset();
        context->append_operand(&func_o_ref);
        call_op.o_operands[2] = context->func_o_map[func_name];
      }
    }
    // check for twoCallArgs
    if (context->token_to_scan == '(') {
      if (!CallArgs(context)) {
        call_op.o_operands[1] = context->current_argList_offset;
      } else {
        return 1;
      }
    }

    if (context->token_to_scan == '(') {
      if (!CallArgs(context)) {
        call_op.o_operands[3] = context->current_argList_offset;
      } else {
        return 1;
      }
    }

    // check for CallTarget
    if (context->token_to_scan == ';') {
      context->append_code(&call_op);
      context->token_to_scan = yylex();
      return 0;
    } else if (context->token_to_scan == '[') {
      if (!CallTargets(context)) {
        if (context->token_to_scan == ';') {
            context->token_to_scan = yylex();
           return 0;
        } else {
          context->set_error(MISSING_SEMICOLON);
        }
      }
      return 1;
    } else {
      context->set_error(MISSING_SEMICOLON);
    }
  } else {
    context->set_error(MISSING_OPERAND);
  }
  return 1;
}

int Initializer(Context* context) {
  // first token should be '='
  if (context->token_to_scan == '=') {
    bool hasCurlyBrackets = false;
    context->token_to_scan = yylex();
    if (context->token_to_scan == '{') {
      context->token_to_scan = yylex();
      hasCurlyBrackets = true;
    }
    BrigDirectiveSymbol bds ;
    //BrigdOffset32_t sym_offset = context->get_directive_offset() - sizeof(BrigDirectiveSymbol);
    BrigdOffset32_t sym_offset = context->current_argdecl_offset;
    context->get_directive(sym_offset,&bds);
    BrigDataType16_t init_type ;
    BrigDataType16_t data_type = context->get_type();
    switch(data_type){
      case Brigb1: init_type = Brigb1; break;

      case Brigs8:
      case Brigu8:
      case Brigb8: init_type = Brigb8; break;

      case Brigs16:
      case Brigu16:
      case Brigf16:
      case Brigb16: init_type = Brigb16; break;

      case Brigs32:
      case Brigu32:
      case Brigf32:
      case Brigb32: init_type = Brigb32; break;

      case Brigs64:
      case Brigu64:
      case Brigf64:
      case Brigb64: init_type = Brigb64; break;
	  default: context->set_error(UNKNOWN_ERROR);
				return 1;
    }
    context->set_type(init_type);

    switch (context->token_to_scan) {
      case TOKEN_DOUBLE_CONSTANT:
        if (!FloatListSingle(context)) {
          break;
        } else {
          return 1;
        }
      case TOKEN_INTEGER_CONSTANT:
        if (!DecimalListSingle(context)) {
          break;
        } else {
          return 1;
        }
      case TOKEN_SINGLE_CONSTANT:
        if (!SingleListSingle(context)) {
          break;
        } else {
          return 1;
        }
      case TOKEN_LABEL:
        if (hasCurlyBrackets) {
          if (!LabelList(context)) {
            break;
          }
        } else {
          return 1;
        }
      default:
        context->set_error(INVALID_INITIALIZER);
        return 1;
    }
    if (hasCurlyBrackets) {
      if (context->token_to_scan == '}') {
        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(MISSING_CLOSING_BRACKET);
        return 1;
      }
    } else {
      return 0;
    }
  } // '='
  return 0;
}

int InitializableDecl(Context* context) {

  if ((context->token_to_scan==GLOBAL)
     || (context->token_to_scan == READONLY)){
    BrigStorageClass32_t storage_class = context->token_value.storage_class;
    context->token_to_scan = yylex();
    return (InitializableDeclPart2(context, storage_class));
  } else {
    return 1;
  }
}

int InitializableDeclPart2(Context *context, BrigStorageClass32_t storage_class)
{

  //First token already verified as GLOBAL/READONLY
  if (context->token_type == DATA_TYPE_ID) {
    context->set_type(context->token_value.data_type);

    context->token_to_scan = yylex();

    if (!Identifier(context)) {
      std::string var_name = context->token_value.string_val;
      int var_name_offset = context->add_symbol(var_name);

      // scan for arrayDimensions
      context->token_to_scan = yylex();

      // set default value(scalar)
      context->set_dim(0);
      context->set_symbol_modifier(BrigArray);
      if (context->token_to_scan == '[') {
        if (!ArrayDimensionSet(context)) {
        }
      }
      BrigDirectiveSymbol sym_decl = {
        sizeof(sym_decl),                 // size
        BrigEDirectiveSymbol,             // kind
        {
          context->get_code_offset(),       // c_code
          storage_class,                    // storageClass ??
          context->get_attribute(),         // attribute
          0,                                // reserved
          context->get_symbol_modifier(),   // symbol modifier
          context->get_dim(),                                // dim
          var_name_offset,                  // s_name
          context->get_type(),                        // data type
          context->get_alignment(),         // alignment
        },
        0,                                // d_init = 0 for arg
        0                                 // reserved
      };

      context->current_argdecl_offset = context->get_directive_offset();
      context->append_directive(&sym_decl);

      BrigDirectiveFunction bdf;
      context->get_directive(context->current_bdf_offset, &bdf);
      BrigdOffset32_t first_scope = bdf.d_firstScopedDirective;
      BrigdOffset32_t next_directive = bdf.d_nextDirective;
      if (first_scope == next_directive) {
        bdf.d_nextDirective += sizeof(sym_decl);
        bdf.d_firstScopedDirective = bdf.d_nextDirective;
      } else {
        bdf.d_nextDirective += sizeof(sym_decl);
      }

      unsigned char *bdf_charp = reinterpret_cast<unsigned char*>(&bdf);
      context->update_directive_bytes(bdf_charp ,
                                      context->current_bdf_offset,
                                      sizeof(BrigDirectiveFunction));

      if (!Initializer(context)) {
        if (context->token_to_scan == ';') {
            context->token_to_scan = yylex();
          return 0;
        } else {
          context->set_error(MISSING_SEMICOLON);
        }
      }
    } else {
      context->set_error(MISSING_IDENTIFIER);
    }
  }
  return 1;
}

int UninitializableDecl(Context* context) {
  // first_token is PRIVATE, GROUP or SPILL

  BrigStorageClass32_t storage_class = context->token_value.storage_class;

  context->token_to_scan = yylex();
  if (context->token_type == DATA_TYPE_ID) {
    context->token_to_scan = yylex();

    if (!Identifier(context)) {
      std::string var_name = context->token_value.string_val;
      int var_name_offset = context->add_symbol(var_name);

      // scan for arrayDimensions
      context->token_to_scan = yylex();
      // set default value(scalar)
      context->set_dim(0);
      context->set_symbol_modifier(BrigArray);
      if (context->token_to_scan == '[') {
        if (!ArrayDimensionSet(context)) {
        }
      }
      BrigDirectiveSymbol sym_decl = {
        sizeof(sym_decl),                 // size
        BrigEDirectiveSymbol,             // kind
        {
          context->get_code_offset(),       // c_code
          storage_class,                    // storageClass
          context->get_attribute(),         // attribute
          0,                                // reserved
          context->get_symbol_modifier(),   // symbol modifier
          context->get_dim(),               // dim
          var_name_offset,                  // s_name
          context->token_value.data_type,   // data type
          context->get_alignment(),         // alignment
        },
        0,                                // d_init = 0 for arg
        0                                 // reserved
      };
      context->append_directive(&sym_decl);

      BrigDirectiveFunction bdf;
      context->get_directive(context->current_bdf_offset, &bdf);
      BrigdOffset32_t first_scope = bdf.d_firstScopedDirective;
      BrigdOffset32_t next_directive = bdf.d_nextDirective;
      if (first_scope == next_directive) {
        bdf.d_nextDirective += sizeof(sym_decl);
        bdf.d_firstScopedDirective = bdf.d_nextDirective;
      } else {
        bdf.d_nextDirective += sizeof(sym_decl);
      }

      unsigned char *bdf_charp = reinterpret_cast<unsigned char*>(&bdf);
      context->update_directive_bytes(bdf_charp ,
                                          context->current_bdf_offset,
                                          sizeof(sym_decl));

      if (context->token_to_scan == ';') {
        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(MISSING_SEMICOLON);
      }
    } else {
      context->set_error(MISSING_IDENTIFIER);
    }
  } else {
    context->set_error(MISSING_DATA_TYPE);
  }
  return 1;
}

int ArgUninitializableDecl(Context* context) {
  // first token is ARG
  BrigStorageClass32_t storage_class = context->token_value.storage_class;
  context->token_to_scan = yylex();

  if (context->token_type == DATA_TYPE_ID) {
    BrigDataType16_t data_type = context->token_value.data_type;
    context->token_to_scan = yylex();
    if (!Identifier(context)) {
      // need to add to both .operand BrigOperandArgumentRef,
      // and .directive BrigDirectiveSymbol,
      // also should have a map, for BrigOperandArgumentList.

      // default value for BrigDirectiveSymbol.
      // for Now, assume this is a scalar. [CAUTION]
      std::string arg_name = context->token_value.string_val;

      // scan for arrayDimensions
      context->token_to_scan = yylex();
      // set default value(scalar)
      context->set_dim(0);
      context->set_symbol_modifier(BrigArray);
      if (context->token_to_scan == '[') {
        if (!ArrayDimensionSet(context)) {
        }
      }
      if (context->token_to_scan == ';') {
        BrigDirectiveSymbol arg_decl = {
          sizeof(arg_decl),                 // size
          BrigEDirectiveSymbol,             // kind
          {
            context->get_code_offset(),       // c_code
            storage_class,                    // storageClass
            context->get_attribute(),         // attribute
            0,                                // reserved
            context->get_symbol_modifier(),     // symbol modifier
            context->get_dim(),               // dim
            context->add_symbol(arg_name),    // s_name
            data_type,                        // data_type
            context->get_alignment(),         // alignment
          },
          0,                                // d_init = 0 for arg
          0                                 // reserved
        };

        BrigOperandArgumentRef arg_ref = {
          8,
          BrigEOperandArgumentRef,
          context->get_directive_offset()
        };

        context->symbol_map[arg_name]= context->get_operand_offset();
        context->append_directive(&arg_decl);
        // add the operand to the map.
        context->arg_map[arg_name] = context->get_operand_offset();
        context->append_operand(&arg_ref);

        context->token_to_scan = yylex();
        arg_name.clear();
        return 0;
      } else {
        context->set_error(MISSING_SEMICOLON);
      }
    } else {
      context->set_error(MISSING_IDENTIFIER);
    }
  } else {
    context->set_error(MISSING_DATA_TYPE);
  }
  return 1;
}

int FileDecl(Context* context) {
  // first token is _FILE "file"
  if (_FILE != context->token_to_scan)
    return 1;

  context->token_to_scan = yylex();

  if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
    int fileid = context->token_value.int_val;
    context->token_to_scan = yylex();

    if (context->token_to_scan == TOKEN_STRING) {
      std::string path(context->token_value.string_val);
      BrigsOffset32_t path_offset = context->add_symbol(path);
      context->token_to_scan = yylex();

      if (context->token_to_scan == ';') {

        BrigDirectiveFile bdfile = {
           sizeof(BrigDirectiveFile),   //size
           BrigEDirectiveFile,          //kind
           context->get_code_offset(),  //c_code??
           fileid,                      //fileid
           path_offset                  //s_filename
        };
        context->append_directive(&bdfile);

        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(MISSING_SEMICOLON);
      }
    } else {
      context->set_error(MISSING_STRING);
    }
  } else {
    context->set_error(MISSING_INTEGER_CONSTANT);
  }

  return 1;
}

int VectorToken(Context *context) {
  if ((_V2 == context->token_to_scan) || (_V4 == context->token_to_scan)) {
    context->token_to_scan = yylex();
    return 0;
  } else {
    return 1;
  }
}

int SignatureType(Context *context) {
  // first token is ARG
  context->token_to_scan = yylex();

  if (DATA_TYPE_ID == context->token_type) {
    context->token_to_scan = yylex();
    if (TOKEN_LOCAL_IDENTIFIER == context->token_to_scan)
    // ignore the local identifier
      context->token_to_scan = yylex();
    return 0;
  } else if (_ROIMG == context->token_to_scan
           || _RWIMG == context->token_to_scan
           || _SAMP == context->token_to_scan ) {
    context->token_to_scan = yylex();
    return 0;
  }
  return 1;
}

int SysCall(Context* context) {
  // first token is _SYSCALL "syscall"

  BrigInstBase syscallInst = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigSyscall,         // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };
  std::string opName;
  context->token_to_scan = yylex();

  if (context->valid_string) {
    opName = context->token_value.string_val;
  } else {
    context->set_error(INVALID_FIRST_OPERAND);
    return 1;
  }
  // Note: dest: Destination. Must be a 32-bit register.
  if (context->token_to_scan == TOKEN_SREGISTER) {
    if (Operand(context)) {
      return 1;
    }
    syscallInst.o_operands[0] = context->operand_map[opName];
    if (context->token_to_scan == ',') {
      context->token_to_scan = yylex();
      BrigoOffset32_t opSize = 0;
      // n: An integer literal. Valid values fit into a u32 data type.
      opSize = context->get_operand_offset();
      opSize += opSize & 0x7;
      if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
        if (Operand(context)) {
          return 1;
        }
        syscallInst.o_operands[1] = opSize;
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();

          if (((context->token_to_scan == TOKEN_SREGISTER) ||
               (context->token_to_scan == TOKEN_WAVESIZE) ||
               (context->token_to_scan == TOKEN_INTEGER_CONSTANT))) {
            opSize = context->get_operand_offset();
            if (context->token_to_scan == TOKEN_SREGISTER) {
              opName = context->token_value.string_val;
            } else {
              if (context->token_type == CONSTANT) {
                opSize += opSize & 0x7;
              }            
            }
            if (Operand(context)) {
              return 1;
            }
            if (context->get_operand_offset() == opSize) {
              syscallInst.o_operands[2] = context->operand_map[opName];
            } else {
              syscallInst.o_operands[2] = opSize;
            }
            if (context->token_to_scan == ',') {
              context->token_to_scan = yylex();

              if ((context->token_to_scan == TOKEN_SREGISTER) ||
                  (context->token_to_scan == TOKEN_WAVESIZE) ||
                  (context->token_to_scan == TOKEN_INTEGER_CONSTANT)) {
                opSize = context->get_operand_offset();
                if (context->token_to_scan == TOKEN_SREGISTER) {
                  opName = context->token_value.string_val;
                } else {
                  if (context->token_type == CONSTANT) {
                    opSize += opSize & 0x7;
                  }                
                }
                if (Operand(context)) {
                  return 1;
                }
                if (context->get_operand_offset() == opSize) {
                  syscallInst.o_operands[3] = context->operand_map[opName];
                } else {
                  syscallInst.o_operands[3] = opSize;
                }
                if (context->token_to_scan == ',') {
                  context->token_to_scan = yylex();

                  if ((context->token_to_scan == TOKEN_SREGISTER) ||
                      (context->token_to_scan == TOKEN_WAVESIZE) ||
                      (context->token_to_scan == TOKEN_INTEGER_CONSTANT)) {
                    opSize = context->get_operand_offset();
                    if (context->token_to_scan == TOKEN_SREGISTER) {
                      opName = context->token_value.string_val;
                    } else {
                      if (context->token_type == CONSTANT) {
                        opSize += opSize & 0x7;
                      }
                    }
                    if (Operand(context)) {
                      return 1;
                    }
                    if (context->get_operand_offset() == opSize) {
                      syscallInst.o_operands[4] = context->operand_map[opName];
                    } else {
                      syscallInst.o_operands[4] = opSize;
                    }

                    if (context->token_to_scan == ';') {
                      context->append_code(&syscallInst);
                      context->token_to_scan = yylex();
                      return 0;
                    } else {  // ';'
                      context->set_error(MISSING_SEMICOLON);
                    }
                  } else {  // 5 operand
                    context->set_error(INVALID_FIFTH_OPERAND);
                  }
                } else {
                  context->set_error(MISSING_COMMA);
                }
              } else {  // 4 operand
                context->set_error(INVALID_FOURTH_OPERAND);
              }
            } else {
              context->set_error(MISSING_COMMA);
            }
          } else {  // 3 operand
            context->set_error(INVALID_THIRD_OPERAND);
          }
        } else {
          context->set_error(MISSING_COMMA);
        }
      } else {  // 2 base operand
        context->set_error(INVALID_SECOND_OPERAND);
      }
    } else {
      context-> set_error(MISSING_COMMA);
    }
  } else {  // 1 operand
    context->set_error(INVALID_FIRST_OPERAND);
  }
  return 1;
}

int SignatureArgumentList(Context *context) {
  while (1) {
    if (context->token_to_scan == ARG) {
      if (!SignatureType(context)) {
        if (',' == context->token_to_scan) {
          context->token_to_scan = yylex();
          continue;
        } else {
          break;
        }
      }
    } else {
      context->set_error(MISSING_ARGUMENT);
      return 1;
    }
  }
  return 0;
}

int FunctionSignature(Context *context) {
  // first token is SIGNATURE
  context->token_to_scan = yylex();

  if (TOKEN_GLOBAL_IDENTIFIER == context->token_to_scan) {
    context->token_to_scan = yylex();
    // check return argument list
    if ('(' == context->token_to_scan) {
      context->token_to_scan = yylex();
      if (')' == context->token_to_scan) {  // empty signature Argument List
        context->token_to_scan = yylex();
      } else if (!SignatureArgumentList(context)) {
        if (context->token_to_scan == ')')
          context->token_to_scan = yylex();
        else
          context->set_error(MISSING_CLOSING_PARENTHESIS);
      }
    } else {
      context->set_error(UNKNOWN_ERROR);
      return 1;
    }

    // for input argument
    if ('(' == context->token_to_scan) {
      context->token_to_scan = yylex();
      if (')' == context->token_to_scan) {  // empty
        context->token_to_scan = yylex();
      } else if (!SignatureArgumentList(context)) {
        if (context->token_to_scan == ')')
          context->token_to_scan = yylex();
        else
          context->set_error(MISSING_CLOSING_PARENTHESIS);
      }
    } else {
      context->set_error(UNKNOWN_ERROR);
      return 1;
    }

    if (_FBAR == context->token_to_scan) {
      if (!FBar(context)) {
      } else {
        context->set_error(INVALID_FBAR);
        return 1;
      }
    }

    if (';' == context->token_to_scan) {  // end with ;
      context->token_to_scan = yylex();
      return 0;
    } else {
      context->set_error(MISSING_SEMICOLON);
      return 1;
    }
  }
  return 1;
}

int Label(Context* context) {
  if (context->token_to_scan == TOKEN_LABEL) {
    BrigDirectiveLabel label_directive = {
      12,                     // size
      BrigEDirectiveLabel,    // kind
      0,                      // c_code
      0                       // s_name
    };
    std::string s_name = context->token_value.string_val;
    if (yylex() == ':') {
      if (!context->symbol_map.count(s_name)) {
        label_directive.c_code = context->get_code_offset();
        int str_offset = context->lookup_symbol(s_name);
        if (str_offset == -1) {
          str_offset = context->get_string_offset();
          context->add_symbol(s_name);
        }
        label_directive.s_name = str_offset;
        if (context->label_o_map.count(s_name)) {
          BrigoOffset32_t ope_offset = context->label_o_map[s_name];
          BrigdOffset32_t lab_dir_offset = context->get_directive_offset();
          unsigned char* lab_d_Offset = reinterpret_cast<unsigned char*>(&lab_dir_offset);
          // sizeof(uint16_t) << 1:
          // leaped over BrigOperandLabelRef.size and .kind
          // make the offset to point the address of labeldirecitive in BrigOperandLabelRef
          context->update_operand_bytes(lab_d_Offset,
                                        ope_offset + sizeof(uint16_t) * 2,
                                        sizeof(BrigdOffset32_t));
        }
        context->symbol_map[s_name] = context->get_directive_offset();
        context->append_directive(&label_directive);
      }
      context->token_to_scan = yylex();
      return 0;
    } else {
      context->set_error(MISSING_COLON);
      return 1;
    }
  }
  return 1;
}

int LabelTargets(Context* context) {
  if (!Label(context)) {
    if (context->token_to_scan == LABELTARGETS) {
      while (1) {
        if (yylex() == TOKEN_LABEL) {
          context->token_to_scan = yylex();
          if (context->token_to_scan == ',') {
            continue;
          } else if (context->token_to_scan == ';') {
            return 0;
          } else {
            context->set_error(MISSING_SEMICOLON);
            return 1;
          }
        } else {
          context->set_error(MISSING_LABEL);
          return 1;
        }
      }
    } else {
      context->set_error(UNKNOWN_ERROR);
    }
  }
  return 1;
}

int Instruction4(Context* context) {
  if (context->token_type == INSTRUCTION4_OPCODE) {
    context->token_to_scan = yylex();
    if (!RoundingMode(context)) {
    }
    if (context->token_type == DATA_TYPE_ID) {
      context->token_to_scan = yylex();
      if (!Operand(context)) {
        if (context->token_to_scan != ',') {
          context->set_error(MISSING_COMMA);
          return 1;
        }
        context->token_to_scan = yylex();
        if (!Operand(context)) {
          if (context->token_to_scan != ',') {
            context->set_error(MISSING_COMMA);
            return 1;
          }
          context->token_to_scan = yylex();
          if (!Operand(context)) {
            if (context->token_to_scan != ',') {
              context->set_error(MISSING_COMMA);
              return 1;
            }
            context->token_to_scan = yylex();
            if (!Operand(context)) {
              if (context->token_to_scan == ';') {
                context->token_to_scan = yylex();
                return 0;
              } else {
                context->set_error(MISSING_SEMICOLON);
                return 1;
              }  // ';'
            } else {  // 4 operand
              context->set_error(INVALID_FOURTH_OPERAND);
              return 1;
            }
          } else {  // 3 operand
            context->set_error(INVALID_THIRD_OPERAND);
            return 1;
          }
        } else {  // 2 operand
          context->set_error(INVALID_SECOND_OPERAND);
          return 1;
        }
      } else {  // 1 operand
        context->set_error(INVALID_FIRST_OPERAND);
        return 1;
      }
    } else {  // DATA_TYPE_ID
      context->set_error(MISSING_DATA_TYPE);
      return 1;
    }
  } else {  // INSTRUCTION4_OPCODE
  context->set_error(INVALID_INSTRUCTION);
  }
  return 1;
}

int Instruction4MultiMediaOperationPart1(Context* context) {
  const unsigned int first_token = context->token_to_scan;
  
  BrigInstBase mmoInst = {
    sizeof(BrigInstBase),   // size
    BrigEInstBase,         // kind
    0,                     // opcode
    0,                     // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}       // o_operands[5]
  };
  
  switch(first_token) {
    case SAD:
      mmoInst.opcode = BrigSad;
      break;
    case SAD2:
      mmoInst.opcode = BrigSad2;
      break;
    case SAD4:
      mmoInst.opcode = BrigSad4;
      break;
    case SAD4HI:
      mmoInst.opcode = BrigSad4hi;
      break;
    case LERP:
      mmoInst.opcode = BrigLerp;
      break;
    case BITALIGN:
      mmoInst.opcode = BrigBitAlign;
      break;
    case BYTEALIGN:
      mmoInst.opcode = BrigByteAlign;
      break;
  }
  
  context->token_to_scan = yylex();
  // Note: must be b32
  if (context->token_to_scan == _B32) {

    mmoInst.type = context->token_value.data_type;
    context->token_to_scan = yylex();
    
    // TODO(Chuang): dest:  The destination is an f32.
    std::string opName;
    BrigoOffset32_t opSize;

    if (context->token_type == REGISTER) {
      opName = context->token_value.string_val;
    } else {
      context->set_error(INVALID_FIRST_OPERAND);
      return 1;
    } 

    if (!Operand(context)) {
      mmoInst.o_operands[0] = context->operand_map[opName];
      if (context->token_to_scan != ',') {
        context->set_error(MISSING_COMMA);
        return 1;
      }
      context->token_to_scan = yylex();
      // TODO(Chuang):src0, src1, src2, src3: Sources. All are b32
      opSize = context->get_operand_offset();
      if (context->token_type == REGISTER) {
        opName = context->token_value.string_val;
      } else if (context->token_type == CONSTANT) {
        opSize += opSize & 0x7;
      }  // else WaveSize

      if (!Operand(context)) {
        if (opSize == context->get_operand_offset()) {
          mmoInst.o_operands[1] = context->operand_map[opName];
        } else {
          mmoInst.o_operands[1] = opSize;
        }
        if (context->token_to_scan != ',') {
          context->set_error(MISSING_COMMA);
          return 1;
        }
        context->token_to_scan = yylex();
        opSize = context->get_operand_offset();
        if (context->token_type == REGISTER) {
          opName = context->token_value.string_val;
        } else if (context->token_type == CONSTANT) {
          opSize += opSize & 0x7;
        }  // else WaveSize
        
        if (!Operand(context)) {
          if (opSize == context->get_operand_offset()) {
            mmoInst.o_operands[2] = context->operand_map[opName];
          } else {
            mmoInst.o_operands[2] = opSize;
          }
          if (context->token_to_scan != ',') {
              context->set_error(MISSING_COMMA);
              return 1;
          }
          context->token_to_scan = yylex();

          opSize = context->get_operand_offset();
          if (context->token_type == REGISTER) {
            opName = context->token_value.string_val;
          } else if (context->token_type == CONSTANT) {
            opSize += opSize & 0x7;
          }  // else WaveSize 

          if (!Operand(context)) {
            if (opSize == context->get_operand_offset()) {
              mmoInst.o_operands[3] = context->operand_map[opName];
            } else {
              mmoInst.o_operands[3] = opSize;
            }
            if (context->token_to_scan == ';') {
              context->append_code(&mmoInst);
              context->token_to_scan = yylex();
              return 0;
            } else {
              context->set_error(MISSING_SEMICOLON);
              return 1;
            }  // ';'
          } else {  // 4 operand
              context->set_error(INVALID_FOURTH_OPERAND);
              return 1;
          }
        } else {  // 3 operand
          context->set_error(INVALID_THIRD_OPERAND);
          return 1;
        }
      } else {  // 2 operand
        context->set_error(INVALID_SECOND_OPERAND);
        return 1;
      }
    } else {  // 1 operand
      context->set_error(INVALID_FIRST_OPERAND);
      return 1;
    }
  } else {  // DATA_TYPE_ID
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  return 1;
}

int Instruction4FmaPart2(Context* context) {
  BrigInstBase fmaInst = {
    sizeof(BrigInstBase),   // size
    BrigEInstBase,         // kind
    BrigFma,               // opcode
    0,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}       // o_operands[5]
  };
 
  BrigAluModifier aluModifier = {0};

  context->token_to_scan = yylex();
  if (!RoundingMode(context)) {
    aluModifier = context->get_alu_modifier();
  }
  // TODO(Chuang):Type Length: 16 (in some implementations), 32, 64
  if (context->token_to_scan == _F32 ||
      context->token_to_scan == _F64) {
    fmaInst.type = context->token_value.data_type;
    context->token_to_scan = yylex();
    
    // Note: dest: Destination register.
    std::string opName;
    BrigoOffset32_t opSize;

    if (context->token_type == REGISTER) {
      opName = context->token_value.string_val;
    } else {
      context->set_error(INVALID_OPERAND);
      return 1;
    } 

    if (!Operand(context)) {
      fmaInst.o_operands[0] = context->operand_map[opName];
      if (context->token_to_scan != ',') {
        context->set_error(MISSING_COMMA);
        return 1;
      }
      context->token_to_scan = yylex();
      // Sources. Can be a register or immediate value.
      opSize = context->get_operand_offset();
      if (context->token_type == REGISTER) {
        opName = context->token_value.string_val;
      } else if (context->token_type == CONSTANT) {
        opSize += opSize & 0x7;
      } else {
        context->set_error(INVALID_OPERAND);
        return 1;
      }

      if (!Operand(context)) {
        if (opSize == context->get_operand_offset()) {
          fmaInst.o_operands[1] = context->operand_map[opName];
        } else {
          fmaInst.o_operands[1] = opSize;
        }
        if (context->token_to_scan != ',') {
          context->set_error(MISSING_COMMA);
          return 1;
        }
        context->token_to_scan = yylex();
        opSize = context->get_operand_offset();
        if (context->token_type == REGISTER) {
          opName = context->token_value.string_val;
        } else if (context->token_type == CONSTANT) {
          opSize += opSize & 0x7;
        } else {
          context->set_error(INVALID_OPERAND);
          return 1;
        }
        
        if (!Operand(context)) {
          if (opSize == context->get_operand_offset()) {
            fmaInst.o_operands[2] = context->operand_map[opName];
          } else {
            fmaInst.o_operands[2] = opSize;
          }
          if (context->token_to_scan != ',') {
              context->set_error(MISSING_COMMA);
              return 1;
          }
          context->token_to_scan = yylex();

          opSize = context->get_operand_offset();
          if (context->token_type == REGISTER) {
            opName = context->token_value.string_val;
          } else if (context->token_type == CONSTANT) {
            opSize += opSize & 0x7;
          } else {
            context->set_error(INVALID_OPERAND);
            return 1;
          }

          if (!Operand(context)) {
            if (opSize == context->get_operand_offset()) {
              fmaInst.o_operands[3] = context->operand_map[opName];
            } else {
              fmaInst.o_operands[3] = opSize;
            }
            if (context->token_to_scan == ';') {
              int* pCmp = reinterpret_cast<int*>(&aluModifier);
              if (*pCmp != 0) {
                BrigInstMod fmaMod = {
                  sizeof(BrigInstMod),  // size
                  BrigEInstMod,         // kind
                  BrigFma,              // opcode
                  fmaInst.type,         // type
                  BrigNoPacking,        // packing
                  {0, 0, 0, 0, 0},      // o_operands[5]
                  {0}  // aluModifier
                };
                for (int i = 0 ; i < 5 ; ++i) {
                  fmaMod.o_operands[i] = fmaInst.o_operands[i];
                }
                fmaMod.aluModifier = aluModifier;
                context->append_code(&fmaMod);
              }
              else {
                context->append_code(&fmaInst);
              }
              context->token_to_scan = yylex();
              return 0;
            } else {
              context->set_error(MISSING_SEMICOLON);
              return 1;
            }  // ';'
          } else {  // 4 operand
              context->set_error(INVALID_FOURTH_OPERAND);
              return 1;
          }
        } else {  // 3 operand
          context->set_error(INVALID_THIRD_OPERAND);
          return 1;
        }
      } else {  // 2 operand
        context->set_error(INVALID_SECOND_OPERAND);
        return 1;
      }
    } else {  // 1 operand
      context->set_error(INVALID_FIRST_OPERAND);
      return 1;
    }
  } else {  // DATA_TYPE_ID
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  return 1;

}

int Instruction4MadPart3(Context* context) {
  BrigInstBase madInst = {
    sizeof(BrigInstBase),   // size
    BrigEInstBase,         // kind
    BrigMad,               // opcode
    0,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}       // o_operands[5]
  };
 
  BrigAluModifier aluModifier = {0};

  context->token_to_scan = yylex();
  if (!RoundingMode(context)) {
    aluModifier = context->get_alu_modifier();
  }
  // TODO(Chuang):Type f Length: 16 (in some implementations), 32, 64
  // Or Type u, s Length 32, 64
  if (context->token_to_scan == _F32 ||
      context->token_to_scan == _F64 ||
      context->token_to_scan == _U32 ||
      context->token_to_scan == _U64 ||
      context->token_to_scan == _S32 ||
      context->token_to_scan == _S64) {

    madInst.type = context->token_value.data_type;
    context->token_to_scan = yylex();
    
    // Note: dest: Destination register.
    std::string opName;
    BrigoOffset32_t opSize;

    if (context->token_type == REGISTER) {
      opName = context->token_value.string_val;
    } else {
      context->set_error(INVALID_OPERAND);
      return 1;
    } 

    if (!Operand(context)) {
      madInst.o_operands[0] = context->operand_map[opName];
      if (context->token_to_scan != ',') {
        context->set_error(MISSING_COMMA);
        return 1;
      }
      context->token_to_scan = yylex();
      // Sources. Can be a register or immediate value.
      opSize = context->get_operand_offset();
      if (context->token_type == REGISTER) {
        opName = context->token_value.string_val;
      } else if (context->token_type == CONSTANT) {
        opSize += opSize & 0x7;
      } else {
        context->set_error(INVALID_OPERAND);
        return 1;
      }

      if (!Operand(context)) {
        if (opSize == context->get_operand_offset()) {
          madInst.o_operands[1] = context->operand_map[opName];
        } else {
          madInst.o_operands[1] = opSize;
        }
        if (context->token_to_scan != ',') {
          context->set_error(MISSING_COMMA);
          return 1;
        }
        context->token_to_scan = yylex();
        opSize = context->get_operand_offset();
        if (context->token_type == REGISTER) {
          opName = context->token_value.string_val;
        } else if (context->token_type == CONSTANT) {
          opSize += opSize & 0x7;
        } else {
          context->set_error(INVALID_OPERAND);
          return 1;
        }
        
        if (!Operand(context)) {
          if (opSize == context->get_operand_offset()) {
            madInst.o_operands[2] = context->operand_map[opName];
          } else {
            madInst.o_operands[2] = opSize;
          }
          if (context->token_to_scan != ',') {
              context->set_error(MISSING_COMMA);
              return 1;
          }
          context->token_to_scan = yylex();

          opSize = context->get_operand_offset();
          if (context->token_type == REGISTER) {
            opName = context->token_value.string_val;
          } else if (context->token_type == CONSTANT) {
            opSize += opSize & 0x7;
          } else {
            context->set_error(INVALID_OPERAND);
            return 1;
          }

          if (!Operand(context)) {
            if (opSize == context->get_operand_offset()) {
              madInst.o_operands[3] = context->operand_map[opName];
            } else {
              madInst.o_operands[3] = opSize;
            }
            if (context->token_to_scan == ';') {
              int* pCmp = reinterpret_cast<int*>(&aluModifier);
              if (*pCmp != 0) { 
                // Note: mad_u/s without _ftz/rounding
                if (madInst.type != Brigf32 && madInst.type != Brigf64) {
                  context->set_error(INVALID_ROUNDING_MODE);
                  return 1;
                }
                BrigInstMod madMod = {
                  sizeof(BrigInstMod),  // size
                  BrigEInstMod,         // kind
                  BrigMad,              // opcode
                  madInst.type,         // type
                  BrigNoPacking,        // packing
                  {0, 0, 0, 0, 0},      // o_operands[5]
                  {0}  // aluModifier
                };
                for (int i = 0 ; i < 5 ; ++i) {
                  madMod.o_operands[i] = madInst.o_operands[i];
                }
                madMod.aluModifier = aluModifier;
                context->append_code(&madMod);
              }
              else {
                context->append_code(&madInst);
              }
              context->token_to_scan = yylex();
              return 0;
            } else {
              context->set_error(MISSING_SEMICOLON);
              return 1;
            }  // ';'
          } else {  // 4 operand
              context->set_error(INVALID_FOURTH_OPERAND);
              return 1;
          }
        } else {  // 3 operand
          context->set_error(INVALID_THIRD_OPERAND);
          return 1;
        }
      } else {  // 2 operand
        context->set_error(INVALID_SECOND_OPERAND);
        return 1;
      }
    } else {  // 1 operand
      context->set_error(INVALID_FIRST_OPERAND);
      return 1;
    }
  } else {  // DATA_TYPE_ID
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  return 1;
}

int Instruction4BitStringOperationPart4(Context* context) {
  const unsigned int first_token = context->token_to_scan;
  
  BrigInstBase bsoInst = {
    sizeof(BrigInstBase),   // size
    BrigEInstBase,         // kind
    0,                     // opcode
    0,                     // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}       // o_operands[5]
  };
  
  switch(first_token) {
    case EXTRACT:
      bsoInst.opcode = BrigExtract;
      break;
    case INSERT:
      bsoInst.opcode = BrigInsert;
      break;
    case BITSELECT:
      bsoInst.opcode = BrigBitselect;
      break;
  }
  
  context->token_to_scan = yylex();
  // Note: Type s, u; Length 32, 64;
  // TODO(Chuang):  I think "b" should be in its type
  if (context->token_to_scan == _S32 ||
      context->token_to_scan == _S64 ||
      context->token_to_scan == _U32 ||
      context->token_to_scan == _U64 ||
      context->token_to_scan == _B32 ||
      context->token_to_scan == _B64) {

    bsoInst.type = context->token_value.data_type;
    context->token_to_scan = yylex();
    
    // Note: dest: Destination register.
    std::string opName;
    BrigoOffset32_t opSize;

    if (context->token_type == REGISTER) {
      opName = context->token_value.string_val;
    } else {
      context->set_error(INVALID_OPERAND);
      return 1;
    } 

    if (!Operand(context)) {
      bsoInst.o_operands[0] = context->operand_map[opName];
      if (context->token_to_scan != ',') {
        context->set_error(MISSING_COMMA);
        return 1;
      }
      context->token_to_scan = yylex();

      opSize = context->get_operand_offset();
      if (context->token_type == REGISTER) {
        opName = context->token_value.string_val;
      } else if (context->token_type == CONSTANT) {
        opSize += opSize & 0x7;
      }  // else WaveSize

      if (!Operand(context)) {
        if (opSize == context->get_operand_offset()) {
          bsoInst.o_operands[1] = context->operand_map[opName];
        } else {
          bsoInst.o_operands[1] = opSize;
        }
        if (context->token_to_scan != ',') {
          context->set_error(MISSING_COMMA);
          return 1;
        }
        context->token_to_scan = yylex();
        opSize = context->get_operand_offset();
        if (context->token_type == REGISTER) {
          opName = context->token_value.string_val;
        } else if (context->token_type == CONSTANT) {
          opSize += opSize & 0x7;
        }  // else WaveSize
        
        // TODO(Chuang): src1, src2: type follow as the kind of opcode.
        // different opcode will have different rule of types.

        if (!Operand(context)) {
          if (opSize == context->get_operand_offset()) {
            bsoInst.o_operands[2] = context->operand_map[opName];
          } else {
            bsoInst.o_operands[2] = opSize;
          }
          if (context->token_to_scan != ',') {
              context->set_error(MISSING_COMMA);
              return 1;
          }
          context->token_to_scan = yylex();

          opSize = context->get_operand_offset();
          if (context->token_type == REGISTER) {
            opName = context->token_value.string_val;
          } else if (context->token_type == CONSTANT) {
            opSize += opSize & 0x7;
          }  // else WaveSize 

          if (!Operand(context)) {
            if (opSize == context->get_operand_offset()) {
              bsoInst.o_operands[3] = context->operand_map[opName];
            } else {
              bsoInst.o_operands[3] = opSize;
            }
            if (context->token_to_scan == ';') {
              context->append_code(&bsoInst);
              context->token_to_scan = yylex();
              return 0;
            } else {
              context->set_error(MISSING_SEMICOLON);
              return 1;
            }  // ';'
          } else {  // 4 operand
              context->set_error(INVALID_FOURTH_OPERAND);
              return 1;
          }
        } else {  // 3 operand
          context->set_error(INVALID_THIRD_OPERAND);
          return 1;
        }
      } else {  // 2 operand
        context->set_error(INVALID_SECOND_OPERAND);
        return 1;
      }
    } else {  // 1 operand
      context->set_error(INVALID_FIRST_OPERAND);
      return 1;
    }
  } else {  // DATA_TYPE_ID
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  return 1;
}


int Instruction4CmovPart5(Context* context) {
  BrigInstBase cmovInst = {
    sizeof(BrigInstBase),                    // size
    BrigEInstBase,         // kind
    BrigCmov,               // opcode
    0,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}       // o_operands[5]
  };
 
  context->token_to_scan = yylex();

  // TODO(Chuang):Type: For the regular operation: b. 
  // For the packed operation: s, u, f.
  // Length: For rhe regular operation, Length can be 1, 32, 64. 
  // Applies to src1, and src2. For the packed
  // operation, Length can be any packed type.

  if (context->token_type == DATA_TYPE_ID) {
    cmovInst.type = context->token_value.data_type;
    context->token_to_scan = yylex();
    
    // Note: dest: Destination register.
    std::string opName;
    BrigoOffset32_t opSize;

    if (context->token_type == REGISTER) {
      opName = context->token_value.string_val;
    } else {
      context->set_error(INVALID_FIRST_OPERAND);
      return 1;
    } 

    if (!Operand(context)) {
      cmovInst.o_operands[0] = context->operand_map[opName];
      if (context->token_to_scan != ',') {
        context->set_error(MISSING_COMMA);
        return 1;
      }
      context->token_to_scan = yylex();

      // TODO(Chuang): src0, src1, src2: Sources. For the regular operation, 
      // src0 must be a control (c) register or an immediate value. 
      // For the packed operation, if the Length is 32-bit, 
      // then src0 must be an s register or literal value; if
      // the Length is 64-bit, then src0 must be a d register or literal value.

      opSize = context->get_operand_offset();
      if (context->token_type == REGISTER) {
        opName = context->token_value.string_val;
      } else if (context->token_type == CONSTANT) {
        opSize += opSize & 0x7;
      } else {
        context->set_error(INVALID_OPERAND);
        return 1;
      }

      if (!Operand(context)) {
        if (opSize == context->get_operand_offset()) {
          cmovInst.o_operands[1] = context->operand_map[opName];
        } else {
          cmovInst.o_operands[1] = opSize;
        }
        if (context->token_to_scan != ',') {
          context->set_error(MISSING_COMMA);
          return 1;
        }
        context->token_to_scan = yylex();
        opSize = context->get_operand_offset();
        if (context->token_type == REGISTER) {
          opName = context->token_value.string_val;
        } else if (context->token_type == CONSTANT) {
          opSize += opSize & 0x7;
        } else {
          context->set_error(INVALID_OPERAND);
          return 1;
        }
        
        if (!Operand(context)) {
          if (opSize == context->get_operand_offset()) {
            cmovInst.o_operands[2] = context->operand_map[opName];
          } else {
            cmovInst.o_operands[2] = opSize;
          }
          if (context->token_to_scan != ',') {
              context->set_error(MISSING_COMMA);
              return 1;
          }
          context->token_to_scan = yylex();

          opSize = context->get_operand_offset();
          if (context->token_type == REGISTER) {
            opName = context->token_value.string_val;
          } else if (context->token_type == CONSTANT) {
            opSize += opSize & 0x7;
          } else {
            context->set_error(INVALID_OPERAND);
            return 1;
          }

          if (!Operand(context)) {
            if (opSize == context->get_operand_offset()) {
              cmovInst.o_operands[3] = context->operand_map[opName];
            } else {
              cmovInst.o_operands[3] = opSize;
            }
            if (context->token_to_scan == ';') {
              context->append_code(&cmovInst);
              context->token_to_scan = yylex();
              return 0;
            } else {
              context->set_error(MISSING_SEMICOLON);
              return 1;
            }  // ';'
          } else {  // 4 operand
              context->set_error(INVALID_FOURTH_OPERAND);
              return 1;
          }
        } else {  // 3 operand
          context->set_error(INVALID_THIRD_OPERAND);
          return 1;
        }
      } else {  // 2 operand
        context->set_error(INVALID_SECOND_OPERAND);
        return 1;
      }
    } else {  // 1 operand
      context->set_error(INVALID_FIRST_OPERAND);
      return 1;
    }
  } else {  // DATA_TYPE_ID
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  return 1;

}

int Instruction4ShufflePart6(Context* context) {
  BrigInstBase shuffleInst = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigShuffle,           // opcode
    0,                     // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };
 
  context->token_to_scan = yylex();
  // Type: s, u, f.
  // Length: 8x4, 16x2, 16x4, 32x2

  if (context->token_to_scan == _U8X4 ||
      context->token_to_scan == _S8X4 ||
      context->token_to_scan == _U16X2 ||
      context->token_to_scan == _S16X2 ||
      context->token_to_scan == _F16X2 ||
      context->token_to_scan == _U16X4 ||
      context->token_to_scan == _S16X4 ||
      context->token_to_scan == _F16X4 ||
      context->token_to_scan == _U32X2 ||
      context->token_to_scan == _S32X2 ||
      context->token_to_scan == _F32X2) {

    shuffleInst.type = context->token_value.data_type;
    context->token_to_scan = yylex();
    
    // Note: dest: Destination register.
    std::string opName;
    BrigoOffset32_t opSize;

    if (context->token_type == REGISTER) {
      opName = context->token_value.string_val;
    } else {
      context->set_error(INVALID_OPERAND);
      return 1;
    } 

    if (!Operand(context)) {
      shuffleInst.o_operands[0] = context->operand_map[opName];
      if (context->token_to_scan != ',') {
        context->set_error(MISSING_COMMA);
        return 1;
      }
      context->token_to_scan = yylex();

      opSize = context->get_operand_offset();
      if (context->token_type == REGISTER) {
        opName = context->token_value.string_val;
      } else if (context->token_type == CONSTANT) {
        opSize += opSize & 0x7;
      } else {
        context->set_error(INVALID_OPERAND);
        return 1;
      }

      if (!Operand(context)) {
        if (opSize == context->get_operand_offset()) {
          shuffleInst.o_operands[1] = context->operand_map[opName];
        } else {
          shuffleInst.o_operands[1] = opSize;
        }
        if (context->token_to_scan != ',') {
          context->set_error(MISSING_COMMA);
          return 1;
        }
        context->token_to_scan = yylex();
        opSize = context->get_operand_offset();
        if (context->token_type == REGISTER) {
          opName = context->token_value.string_val;
        } else if (context->token_type == CONSTANT) {
          opSize += opSize & 0x7;
        } else {
          context->set_error(INVALID_OPERAND);
          return 1;
        }
        
        if (!Operand(context)) {
          if (opSize == context->get_operand_offset()) {
            shuffleInst.o_operands[2] = context->operand_map[opName];
          } else {
            shuffleInst.o_operands[2] = opSize;
          }
          if (context->token_to_scan != ',') {
              context->set_error(MISSING_COMMA);
              return 1;
          }
          context->token_to_scan = yylex();

          opSize = context->get_operand_offset();

          // src2: Source. Must be a literal value used to select elements
          if (context->token_type == CONSTANT) {
            opSize += opSize & 0x7;
          } else {
            context->set_error(INVALID_OPERAND);
            return 1;
          }

          if (!Operand(context)) {
            shuffleInst.o_operands[3] = opSize;

            if (context->token_to_scan == ';') {
              context->append_code(&shuffleInst);
              context->token_to_scan = yylex();
              return 0;
            } else {
              context->set_error(MISSING_SEMICOLON);
              return 1;
            }  // ';'
          } else {  // 4 operand
              context->set_error(INVALID_FOURTH_OPERAND);
              return 1;
          }
        } else {  // 3 operand
          context->set_error(INVALID_THIRD_OPERAND);
          return 1;
        }
      } else {  // 2 operand
        context->set_error(INVALID_SECOND_OPERAND);
        return 1;
      }
    } else {  // 1 operand
      context->set_error(INVALID_FIRST_OPERAND);
      return 1;
    }
  } else {  // DATA_TYPE_ID
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  return 1;

}



int KernelArgumentDecl(Context *context) {
  // maybe  change in the future
  return  ArgumentDecl(context);
  // return 1;
}
int KernelArgumentListBody(Context *context) {
  // maybe change int the future
  return ArgumentListBody(context);
  // return 1;
}

int Kernel(Context *context) {
  // first must be KERNEL
  context->token_to_scan = yylex();
  if (TOKEN_GLOBAL_IDENTIFIER == context->token_to_scan) {
    context->current_bdf_offset = context->get_directive_offset();
    BrigdOffset32_t bdf_offset = context->current_bdf_offset;

    BrigDirectiveKernel bdk = {
      40,                          // size
      BrigEDirectiveKernel,        // kind
      context->get_code_offset(),  // c_code
      0,                           // name
      0,                           // in param count
      bdf_offset+40,               // d_firstScopedDirective
      0,                           // operation count
      bdf_offset+40,               // d_nextDirective
      context->get_attribute(),    // attribute
      context->get_fbar(),         // fbar count
      0,                           // out param count
      0                            // d_firstInParam
    };

    std::string kern_name = context->token_value.string_val;
    BrigsOffset32_t check_result = context->add_symbol(kern_name);
    bdk.s_name = check_result;
    context->append_directive(&bdk);
    context->func_map[kern_name] = context->current_bdf_offset;

    // check the input argumentlist
    context->token_to_scan = yylex();
    if ('(' == context->token_to_scan) {
      context->token_to_scan = yylex();

      if (')' == context->token_to_scan) {  // empty arguments
        context->token_to_scan = yylex();
      } else if (!KernelArgumentListBody(context)) {  // not empty arguments
        if (')' == context->token_to_scan) {
          context->token_to_scan = yylex();
        } else {
          context->set_error(MISSING_CLOSING_PARENTHESIS);
          return 1;
        }
      } else {
        context->set_error(INVALID_ARGUMENT_LIST);
        return 1;
      }

    } else {
      context->set_error(MISSING_ARGUMENT_LIST);
      return 1;
    }

    if (_FBAR == context->token_to_scan) {
      if (!FBar(context)) {
      } else {
        context->set_error(INVALID_FBAR);
        return 1;
      }
    } else {
      if (!Codeblock(context)) {
        if (';' == context->token_to_scan) {
          context->token_to_scan = yylex();
          return 0;
        } else {
          context->set_error(MISSING_SEMICOLON);
        }
      }
    }
  } else {
    context->set_error(MISSING_IDENTIFIER);
  }
  return 1;
}

int OperandList(Context* context) {
  if (!Operand(context)) {
    while (1) {
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        if (!Operand(context)) {
          continue;
        } else {
          context->set_error(MISSING_OPERAND);
          return 1;
        }
      } else {
        context->token_to_scan = yylex();
        return 0;
      }
    }
  } else {
    context->set_error(MISSING_OPERAND);
  }
  return 1;
}

int ComparisonId(Context* context) {
   BrigCompareOperation32_t cmpOperation;
   return ComparisonIdPart2(context, &cmpOperation);
}

int ComparisonIdPart2(Context* context, BrigCompareOperation32_t* pCmpOperation) {
  switch(context->token_to_scan) {
    case _EQ:
      *pCmpOperation = BrigEq;
      break;
    case _NE:
      *pCmpOperation = BrigNe;
      break;
    case _LT:
      *pCmpOperation = BrigLt;
      break;
    case _LE:
      *pCmpOperation = BrigLe;
      break;
    case _GT:
      *pCmpOperation = BrigGt;
      break;
    case _GE:
      *pCmpOperation = BrigGe;
      break;
    case _EQU:
      *pCmpOperation = BrigEqu;
      break;
    case _NEU:
      *pCmpOperation = BrigNeu;
      break;
    case _LTU:
      *pCmpOperation = BrigLtu;
      break;
    case _LEU:
      *pCmpOperation = BrigLeu;
      break;
    case _GTU:
      *pCmpOperation = BrigGtu;
      break;
    case _GEU:
      *pCmpOperation = BrigGeu;
      break;
    case _NUM:
      *pCmpOperation = BrigNum;
      break;
    case _NAN:
      *pCmpOperation = BrigNan;
      break;
    case _SEQ:
      *pCmpOperation = BrigSeq;
      break;
    case _SNE:
      *pCmpOperation = BrigSne;
      break;
    case _SLT:
      *pCmpOperation = BrigSlt;
      break;
    case _SLE:
      *pCmpOperation = BrigSle;
      break;
    case _SGT:
      *pCmpOperation = BrigSgt;
      break;
    case _SGE:
      *pCmpOperation = BrigSge;
      break;
    case _SGEU:
      *pCmpOperation = BrigSgeu;
      break;
    case _SEQU:
      *pCmpOperation = BrigSequ;
      break;
    case _SNEU:
      *pCmpOperation = BrigSneu;
      break;
    case _SLTU:
      *pCmpOperation = BrigSltu;
      break;
    case _SLEU:
      *pCmpOperation = BrigSleu;
      break;
    case _SNUM:
      *pCmpOperation = BrigSnum;
      break;
    case _SNAN:
      *pCmpOperation = BrigSnan;
      break;
    case _SGTU:
      *pCmpOperation = BrigSgtu;
      break;
    default:
      context->set_error(MISSING_DECLPREFIX);
      return 1;
  }
  return 0;
}


int Cmp(Context* context) {
  // Chuang
  // first token is PACKEDCMP or CMP
  BrigInstCmp cmpInst = {
    44,                 // size
    BrigEInstCmp,       // kind
    BrigCmp,            // opcode
    0,                  // type
    BrigNoPacking,      // packing
    {0, 0, 0, 0, 0},  // o_operands[5]
    {0, 0, 0, 0, 0, 0, 0},                  // aluModifier
    0,             // comparisonOperator
    0,             // sourceType
    0                   // reserved
  };
  const unsigned int first_token = context->token_to_scan;
  context->token_to_scan = yylex();
  if (!ComparisonIdPart2(context, &cmpInst.comparisonOperator)) {
    context->token_to_scan = yylex();
    // 1, 32. Can be 16 if the implementation supports f16.
    if (context->token_type == DATA_TYPE_ID) {
      cmpInst.type = context->token_value.data_type;
      context->token_to_scan = yylex();
      if (first_token == CMP) {
        // 1, 32. Can be 16 if the implementation supports f16.
        if (context->token_type == DATA_TYPE_ID) {
          cmpInst.sourceType = context->token_value.data_type;
          context->token_to_scan = yylex();
        } else {
          context->set_error(MISSING_DATA_TYPE);
          return 1;
        }
      } else {  // CMP
        cmpInst.opcode = BrigPackedCmp;
      }
      std::string opName;
      // Note: Dest must be a register.
      if (context->token_type == REGISTER) {
        opName = context->token_value.string_val;
        if (Operand(context)) {
          return 1;
        }
        cmpInst.o_operands[0] = context->operand_map[opName];
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
          BrigoOffset32_t opSize = 0;
          opSize = context->get_operand_offset();
          if (context->valid_string) {
            opName = context->token_value.string_val;
          } else {
            if (context->token_type == CONSTANT) {
              opSize += opSize & 0x7;
            }
          }
          if (!Operand(context)) {
            if (opSize == context->get_operand_offset()) {
              cmpInst.o_operands[1] = context->operand_map[opName];
            } else {
              cmpInst.o_operands[1] = opSize;
            }
            if (context->token_to_scan == ',') {
              context->token_to_scan = yylex();
              opSize = context->get_operand_offset();
              if (context->valid_string) {
                opName = context->token_value.string_val;
              } else {
                if (context->token_type == CONSTANT) {
                  opSize += opSize & 0x7;
                }
              }

              if (!Operand(context)) {
                if (opSize == context->get_operand_offset()) {
                  cmpInst.o_operands[2] = context->operand_map[opName];
                } else {
                  cmpInst.o_operands[2] = opSize;
                }
                if (context->token_to_scan == ';') {
                  context->append_code(&cmpInst);
                  context->token_to_scan = yylex();
                  return 0;
                } else {
                  context->set_error(MISSING_SEMICOLON);
                  return 1;
                }
              } else {  // 3 operand
                context->set_error(INVALID_THIRD_OPERAND);
                return 1;
              }
            } else {  // ','
              context->set_error(MISSING_COMMA);
            }
          } else {  // 2 operand
            context->set_error(INVALID_SECOND_OPERAND);
            return 1;
          }
        } else {  // ','
          context->set_error(MISSING_COMMA);
        }
      } else {  // 1 operand
        context->set_error(INVALID_FIRST_OPERAND);
        return 1;
      }
    } else {  // Data type
      context->set_error(MISSING_DATA_TYPE);
      return 1;
    }
  } else {
    context->set_error(MISSING_COMPARISON_TYPE);
  }
  return 1;
}
int GlobalPrivateDecl(Context* context) {
  // first token is PRIVATE
  context->token_to_scan = yylex();
  if (context->token_type == DATA_TYPE_ID) {
    BrigDataType16_t data_type = context->token_value.data_type ;

    context->token_to_scan = yylex();
    if (context->token_to_scan == TOKEN_GLOBAL_IDENTIFIER) {

      BrigsOffset32_t var_name_offset = context->add_symbol(context->token_value.string_val);
      context->token_to_scan = yylex();
      // set default value(scalar)
      context->set_dim(0);
      context->set_symbol_modifier(BrigArray);
      if (context->token_to_scan == '[') {
        if (!ArrayDimensionSet(context)) {}
      }

      if (context->token_to_scan == ';') {

        BrigDirectiveSymbol bds = {
        40,                       // size
        BrigEDirectiveSymbol ,    // kind
        {
          context->get_code_offset(),     // c_code
          BrigPrivateSpace,               // storag class
          BrigNone ,                      // attribut
          0,                              // reserved
          context->get_symbol_modifier(), // symbolModifier
          context->get_dim(),             // dim
          var_name_offset,                // s_name
          data_type,                      // type
          context->get_alignment(),       // align
        },
        0,                         // d_init
        0,                         // reserved
        };
        context->append_directive(&bds);

        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(MISSING_SEMICOLON);
      }
    } else {
      context->set_error(MISSING_GLOBAL_IDENTIFIER);
    }
  } else {
    context->set_error(MISSING_DATA_TYPE);
  }
  return 1;
}

int OffsetAddressableOperandPart2(Context* context, BrigoOffset32_t addrOpOffset) {

    std::string operand_name;
    // if the value of addrOpOffset is ZERO,
    // there won't be an NonRegister in the front of OffsetAddresssOperand rule.
    if (addrOpOffset) {
        BrigOperandCompound compound_op = {
          sizeof(BrigOperandCompound),     // size
          BrigEOperandCompound,            // kind
          Brigb32,                         // type
          0,                               // reserved
          0,                               // name
          0,                               // reg
          0                                // offset
        };
        if (context->get_machine() == BrigELarge) {
          compound_op.type = Brigb64;
        }
        compound_op.name = addrOpOffset;
        if (context->token_type == REGISTER) {
          // The register must be an s or d register (c registers are not allowed).
          switch (context->token_to_scan) {
            case TOKEN_DREGISTER:
            case TOKEN_SREGISTER:
              break;
            default:
              context->set_error(INVALID_OPERAND);
              return 1;
          }
          operand_name = context->token_value.string_val;
          // the token must be Register.
          // generate BrigOperandReg code.

          if(Identifier(context)) {
            context->set_error(INVALID_OPERAND);
            return 1;
          }
          compound_op.reg = context->operand_map[operand_name];
          context->token_to_scan = yylex();
          if (context->token_to_scan == '+' || context->token_to_scan == '-') {
            int imm_sign = 1;
            if (context->token_to_scan == '-') {
              imm_sign = -1;
            }
            context->token_to_scan = yylex();
            if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
              compound_op.offset = context->token_value.int_val * imm_sign;
              context->token_to_scan = yylex();
              if (context->token_to_scan == ']') {
                context->append_operand(&compound_op);
                context->token_to_scan = yylex();
                return 0;
              } else {
                context->set_error(MISSING_CLOSING_BRACKET);
                return 1;
              }
            } else {
              context->set_error(INVALID_SECOND_OPERAND);
              return 1;
            }
          } else if (context->token_to_scan == ']') {
            context->append_operand(&compound_op);
            context->token_to_scan = yylex();
            return 0;
          } else {
            context->set_error(MISSING_CLOSING_BRACKET);
            return 1;
          }
        } else if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
          context->token_to_scan = yylex();
          if (context->token_to_scan == ']') {
            context->token_to_scan = yylex();
            return 0;
          } else {
            context->set_error(MISSING_CLOSING_BRACKET);
            return 1;
          }
        } else if (context->token_to_scan == ']') {
          // empty body
          context->set_error(MISSING_OPERAND);
          return 1;
        } else {
          context->set_error(INVALID_OPERAND);
          return 1;
        }
    } else {  // addrOpOffset
      BrigOperandIndirect indirect_op = {
        sizeof(BrigOperandIndirect),    // size
        BrigEOperandIndirect,           // kind
        0,                              // reg
        Brigb32,                        // type
        0,                              // reserved
        0                               // offset
      };
      if (context->get_machine() == BrigELarge) {
        indirect_op.type = Brigb64;
      }
      if (context->token_type == REGISTER) {
        // The register must be an s or d register (c registers are not allowed).
        switch (context->token_to_scan) {
          case TOKEN_DREGISTER:
          case TOKEN_SREGISTER:
            break;
          default:
            context->set_error(INVALID_OPERAND);
            return 1;
        }
        operand_name = context->token_value.string_val;
        // the token must be Register.
        // generate BrigOperandReg code.
        if(Identifier(context)) {
          context->set_error(INVALID_OPERAND);
          return 1;
        }
        indirect_op.reg = context->operand_map[operand_name];
        context->token_to_scan = yylex();
        if (context->token_to_scan == '+' || context->token_to_scan == '-') {
          int imm_sign = 1;
          if (context->token_to_scan == '-') {
            imm_sign = -1;
          }
          context->token_to_scan = yylex();
          if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
            indirect_op.offset = context->token_value.int_val * imm_sign;
            context->token_to_scan = yylex();
          } else {
            return 1;
          }
        }
        if (context->token_to_scan == ']') {
          context->append_operand(&indirect_op);
          context->token_to_scan = yylex();
          return 0;
        } else {
          context->set_error(MISSING_CLOSING_BRACKET);
          return 1;
        }
      } else if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
        indirect_op.offset = context->token_value.int_val;
        context->token_to_scan = yylex();
        if (context->token_to_scan == ']') {
          context->append_operand(&indirect_op);
          context->token_to_scan = yylex();
          return 0;
        } else {
          context->set_error(MISSING_CLOSING_BRACKET);
          return 1;
        }
      } else if (context->token_to_scan == ']') {
        // empty body
        context->set_error(MISSING_OPERAND);
        return 1;
      }
  }  // addrOpOffset

  return 1;
}

int OffsetAddressableOperand(Context* context) {
  return OffsetAddressableOperandPart2(context, 0);
}

int MemoryOperand(Context* context) {
  // Chuang
  // this judge(first token == '[') is necessary in here
  if (context->token_to_scan == '[') {
    BrigoOffset32_t currentToOffset = 0;
    context->token_to_scan = yylex();
    // AddressableOperand
    if (!AddressableOperandPart2(context, &currentToOffset, false)) {
      if (context->token_to_scan == '[') {
        context->token_to_scan = yylex();
        if (!OffsetAddressableOperandPart2(context, currentToOffset)) {
          // Global/Local Identifier with offsetAddressOperand.
          return 0;
        }
      }
      // only Global/Local Identifier without offsetAddressOperand.
      return 0;
    } else if (!AddressableOperand(context)) {
      return 0;
    } else if (!OffsetAddressableOperandPart2(context, 0)) {
      return 0;
    }  // Global/Local Identifier/AddressableOperand/offsetAddressableOperand
  }  // '['
  return 1;
}

int Instruction5(Context* context) {
  // first token is F2U4 "f2u4"

  BrigInstBase f2u4Inst = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigF2u4,               // opcode
    Brigu32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}       // o_operands[5]
  };
  context->token_to_scan = yylex();
  // Note: must be _u32.
  if (context->token_to_scan == _U32) {
    context->token_to_scan = yylex();
    std::string opName;
    BrigoOffset32_t opSize = 0;

    // Note: dest: The destination is an f32.
    opSize = context->get_operand_offset();
    if (context->token_to_scan == TOKEN_SREGISTER) {
      opName = context->token_value.string_val;
    } else if (context->token_type == CONSTANT) {
      opSize += opSize & 0x7;
    }

    if (!Operand(context)) {
      if (opSize == context->get_operand_offset()) {
        f2u4Inst.o_operands[0] = context->operand_map[opName];
      } else {
        f2u4Inst.o_operands[0] = opSize;
      }

      if (context->token_to_scan != ',') {
        context->set_error(MISSING_COMMA);
        return 1;
      }
      context->token_to_scan = yylex();

      opSize = context->get_operand_offset();
      // Note: Sources. All are b32
      if (context->token_to_scan == TOKEN_SREGISTER) {
        opName = context->token_value.string_val;
      } else if (context->token_type == CONSTANT) {
        opSize += opSize & 0x7;
      }
      if (!Operand(context)) {
        if (opSize == context->get_operand_offset()) {
          f2u4Inst.o_operands[1] = context->operand_map[opName];
        } else {
          f2u4Inst.o_operands[1] = opSize;
        }
        if (context->token_to_scan != ',') {
          context->set_error(MISSING_COMMA);
          return 1;
        }
        context->token_to_scan = yylex();

        opSize = context->get_operand_offset();
        // Note: Sources. All are b32
        if (context->token_to_scan == TOKEN_SREGISTER) {
          opName = context->token_value.string_val;
        } else if (context->token_type == CONSTANT) {
          opSize += opSize & 0x7;
        }
        if (!Operand(context)) {
          if (opSize == context->get_operand_offset()) {
            f2u4Inst.o_operands[2] = context->operand_map[opName];
          } else {
            f2u4Inst.o_operands[2] = opSize;
          }

          if (context->token_to_scan != ',') {
            context->set_error(MISSING_COMMA);
            return 1;
          }
          context->token_to_scan = yylex();

          opSize = context->get_operand_offset();
          // Note: Sources. All are b32
          if (context->token_to_scan == TOKEN_SREGISTER) {
            opName = context->token_value.string_val;
          } else if (context->token_type == CONSTANT) {
            opSize += opSize & 0x7;
          }
          if (!Operand(context)) {
            if (opSize == context->get_operand_offset()) {
              f2u4Inst.o_operands[3] = context->operand_map[opName];
            } else {
              f2u4Inst.o_operands[3] = opSize;
            }
            if (context->token_to_scan != ',') {
              context->set_error(MISSING_COMMA);
              return 1;
            }
            context->token_to_scan = yylex();
            opSize = context->get_operand_offset();
            // Note: Sources. All are b32
            if (context->token_to_scan == TOKEN_SREGISTER) {
              opName = context->token_value.string_val;
            } else if (context->token_type == CONSTANT) {
              opSize += opSize & 0x7;
            }
            if (!Operand(context)) {
              if (opSize == context->get_operand_offset()) {
                f2u4Inst.o_operands[4] = context->operand_map[opName];
              } else {
                f2u4Inst.o_operands[4] = opSize;
              }
              if (context->token_to_scan == ';') {
                context->append_code(&f2u4Inst);
                context->token_to_scan = yylex();
                return 0;
              } else {
                context->set_error(MISSING_SEMICOLON);
                return 1;
              }
            } else {  // 5 operand
              context->set_error(INVALID_FIFTH_OPERAND);
              return 1;
            }
          } else {  // 4 operand
            context->set_error(INVALID_FOURTH_OPERAND);
            return 1;
          }
        } else {  // 3 operand
          context->set_error(INVALID_THIRD_OPERAND);
          return 1;
        }
      } else {  // 2 operand
        context->set_error(INVALID_SECOND_OPERAND);
        return 1;
      }
    } else {  // 1 operand
      context->set_error(INVALID_FIRST_OPERAND);
      return 1;
    }
  } else {  // DATA_TYPE_ID
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }

  return 1;
}

int Extension(Context* context) {
  // first token is EXTENSION "extension"
  context->token_to_scan = yylex();

  if (context->token_to_scan == TOKEN_STRING) {
    context->token_to_scan = yylex();

    if (context->token_to_scan == ';') {
      context->token_to_scan = yylex();
      return 0;
    } else {   // missing ";"
      context->set_error(MISSING_SEMICOLON);
    }
  }
  return 1;
}

int Ldc(Context* context) {
  // first token is LDC "ldc"
  context->token_to_scan = yylex();

  BrigInstBase ldc_op = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigLdc,               // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}       // o_operands[5]
  };

  if (context->token_type == DATA_TYPE_ID) {
    ldc_op.type = context->token_value.data_type;
    context->token_to_scan = yylex();
    std::string oper_name = context->token_value.string_val;
    if (!Operand(context)) {
      ldc_op.o_operands[0] = context->operand_map[oper_name];
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        // op[1] must be BrigEOperandLabelRef or BrigEOperandFunctionRef
        if (context->token_to_scan == TOKEN_LABEL ||
            context->token_to_scan == TOKEN_GLOBAL_IDENTIFIER) {
          oper_name = context->token_value.string_val;
          ldc_op.o_operands[1] = context->operand_map[oper_name];
          context->token_to_scan = yylex();
          if (context->token_to_scan == ';') {
            context->append_code(&ldc_op);
            context->token_to_scan = yylex();
            return 0;
          } else {  // ';'
            context->set_error(MISSING_SEMICOLON);
            return 1;
          }
        } else {  // label or identifier
          context->set_error(INVALID_SECOND_OPERAND);
          return 1;
        }
      } else {
        context->set_error(MISSING_COMMA);
      }
    } else {  // operand
      context->set_error(INVALID_FIRST_OPERAND);
      return 1;
    }
  } else {  // datatypeid
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }

  return 1;
}

int Atom(Context* context) {
  const unsigned int first_token = context->token_to_scan;
  context->token_to_scan = yylex();
  BrigInstAtomic atom_op = {
    44,                    // size
    BrigEInstAtomic,       // kind
    BrigAtomic,            // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0},       // o_operands[5]
    BrigAtomicCas,         // atomicOperation;
    BrigFlatSpace,         // storageClass;
    BrigRegular            // memorySemantic;
  };
  if (first_token == ATOMIC) {  // atomic
    if (context->token_type == ATOMIC_OP) {
      switch (context->token_to_scan) {  // without _CAS_
        case _AND_:
          atom_op.atomicOperation = BrigAtomicAnd;
          break;
        case _OR_:
          atom_op.atomicOperation = BrigAtomicOr;
          break;
        case _XOR_:
          atom_op.atomicOperation = BrigAtomicXor;
          break;
        case _EXCH_:
          atom_op.atomicOperation = BrigAtomicExch;
          break;
        case _ADD_:
          atom_op.atomicOperation = BrigAtomicAdd;
          break;
        case _INC_:
          atom_op.atomicOperation = BrigAtomicInc;
          break;
        case _DEC_:
          atom_op.atomicOperation = BrigAtomicDec;
          break;
        case _MIN_:
          atom_op.atomicOperation = BrigAtomicMin;
          break;
        case _MAX_:
          atom_op.atomicOperation = BrigAtomicMax;
          break;
        case _SUB_:
          atom_op.atomicOperation = BrigAtomicSub;
          break;
        default:
          context->set_error(MISSING_DECLPREFIX);
          return 1;
      }
      context->token_to_scan = yylex();
    } else {
      context->set_error(MISSING_DECLPREFIX);
      return 1;
    }
  }
  std::string oper_name;
  // with AtomModifiers
  if (!AtomModifiersPart2(context, &atom_op.storageClass, &atom_op.memorySemantic)) {
    if (context->token_type == DATA_TYPE_ID) {
      atom_op.type = context->token_value.data_type;
      context->token_to_scan = yylex();
      if (context->valid_string) {
        oper_name = context->token_value.string_val;
      }
      if (!Operand(context)) {
        atom_op.o_operands[0] = context->operand_map[oper_name];
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
          atom_op.o_operands[1] = context->get_operand_offset();
          if (!MemoryOperand(context)) {
            if (context->token_to_scan == ',') {
              context->token_to_scan = yylex();
              if (context->valid_string) {
                oper_name = context->token_value.string_val;
              }
              unsigned int cur_op_offset = context->get_operand_offset();
              if (cur_op_offset & 0x7) {
                cur_op_offset += 4;
              }
              atom_op.o_operands[2] = cur_op_offset;
              if (!Operand(context)) {

                // atom_op.o_operands[2] = context->operand_map[oper_name];
                if (first_token == ATOMIC_CAS) {
                  if (context->token_to_scan == ',') {
                    context->token_to_scan = yylex();
                    if (context->valid_string) {
                      oper_name = context->token_value.string_val;
                    }
                    atom_op.o_operands[3] = context->get_operand_offset();
                    if (!Operand(context)) {
                    // atom_op.o_operands[3] = context->operand_map[oper_name];
                    } else {  // 4 Operand
                      context->set_error(INVALID_FOURTH_OPERAND);
                      return 1;
                    }
                  } else {  // ','
                    context->set_error(MISSING_COMMA);
                    return 1;
                  }
                }
                if (context->token_to_scan == ';') {
                  context->append_code(&atom_op);
                  context->token_to_scan = yylex();
                  return 0;
                } else {  // ';'
                  context->set_error(MISSING_SEMICOLON);
                }
              } else {  // 3 Operand
                context->set_error(INVALID_THIRD_OPERAND);
              }
            } else {  // ','
              context->set_error(MISSING_COMMA);
            }
          } else {  // 2 MemoryOperand
            context->set_error(INVALID_SECOND_OPERAND);
          }
        } else {  // ','
          context->set_error(MISSING_COMMA);
        }
      } else {  // 1 Operand
        context->set_error(INVALID_FIRST_OPERAND);
      }
    } else {  // Data Type
      context->set_error(MISSING_DATA_TYPE);
    }
  }  // AtomModifiers
  return 1;
}

int CvtModifier1(Context* context) {
  unsigned int next;
  unsigned int first_token = context->token_to_scan;
  // get current alu modifier from context
  BrigAluModifier mod = {0, 0, 0, 0, 0, 0, 0};

  if (first_token == _FTZ) {
    mod.ftz = 1;
    next = yylex();

    if (context->token_type == FLOAT_ROUNDING) {
      // next is floatRounding
      mod.floatOrInt = 1;
      switch (next) {
        case _UP:
          mod.rounding = 2;
          break;
        case _DOWN:
          mod.rounding = 3;
          break;
        case _ZERO:
          mod.rounding = 1;
          break;
        case _NEAR:
          mod.rounding = 0;
          break;
      }
      context->token_to_scan = yylex();  // set context for following functions
    } else {
      context->token_to_scan = next;
    }

    context->set_alu_modifier(mod);
    return 0;
  } else if (context->token_type == INT_ROUNDING) {
    mod.floatOrInt = 0;
    switch (first_token) {
      case _UPI:
        mod.rounding = 2;
        break;
      case _DOWNI:
        mod.rounding = 3;
        break;
      case _ZEROI:
        mod.rounding = 1;
        break;
      case _NEARI:
        mod.rounding = 0;
        break;
    }
    context->token_to_scan = yylex();  // set context for following functions
    context->set_alu_modifier(mod);
    return 0;
  } else if (context->token_type == FLOAT_ROUNDING) {
    mod.floatOrInt = 1;
    switch (first_token) {
      case _UP:
        mod.rounding = 2;
        break;
      case _DOWN:
        mod.rounding = 3;
        break;
      case _ZERO:
        mod.rounding = 1;
        break;
      case _NEAR:
        mod.rounding = 0;
        break;
    }
    mod.reserved = 0;
    context->token_to_scan = yylex();  // set context for following functions
    context->set_alu_modifier(mod);
    return 0;
  } else {
    return 1;
  }
}

int Mov(Context* context) {
  // Chuang
  // first token is MOV "mov"

  BrigInstBase movInst = {
    32,                    // size
    BrigEInstBase,         // kind
    BrigMov,               // opcode
    0,                     // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };

  context->token_to_scan = yylex();
  // TODO(Chuang): When type is b128 or b64 etc, check whether the operands is correct.
  if (context->token_to_scan == _B1 ||
      context->token_to_scan == _B32 ||
      context->token_to_scan == _B64 ||
      context->token_to_scan == _B128) {

    movInst.type = context->token_value.data_type;
    context->token_to_scan = yylex();
    if (!ArrayOperandPart2(context, &movInst.o_operands[0])) {
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        if (!ArrayOperandPart2(context, &movInst.o_operands[1])) {
          if (context->token_to_scan == ';') {
            context->append_code(&movInst);
            context->token_to_scan = yylex();
            return 0;
          } else {  // ';'
            context->set_error(MISSING_SEMICOLON);
          }
        } else {  // Operand or ArrayOperandList
          context->set_error(INVALID_SECOND_OPERAND);
        }
      } else {  // ','
        context->set_error(MISSING_COMMA);
      }
    } else {  // Operand or ArrayOperandList
      context->set_error(INVALID_FIRST_OPERAND);
    }
  } else {  // datatypeId
    context->set_error(MISSING_DATA_TYPE);
  }
  return 1;
}

int GlobalGroupDecl(Context* context) {
  // first token is Group
  context->token_to_scan = yylex();
  if (context->token_type == DATA_TYPE_ID) {

    BrigDataType16_t data_type = context->token_value.data_type ;
    context->token_to_scan = yylex();
    if (context->token_to_scan == TOKEN_GLOBAL_IDENTIFIER) {

      BrigsOffset32_t var_name_offset = context->add_symbol(context->token_value.string_val);
      context->token_to_scan = yylex();
      // set default value(scalar)
      context->set_dim(0);
      context->set_symbol_modifier(BrigArray);
      if (context->token_to_scan == '[') {
        if (!ArrayDimensionSet(context)) {}
      }
      if (context->token_to_scan == ';') {

        BrigDirectiveSymbol bds = {
        40,                       // size
        BrigEDirectiveSymbol ,    // kind
        {
          context->get_code_offset(),     // c_code
          BrigGroupSpace,                 // storag class
          BrigNone ,                      // attribut
          0,                              // reserved
          context->get_symbol_modifier(), // symbolModifier
          context->get_dim(),             // dim
          var_name_offset,                // s_name
          data_type,                      // type
          context->get_alignment(),       // align
        },
        0,                         // d_init
        0,                         // reserved
        };
        context->append_directive(&bds);

        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(MISSING_SEMICOLON);
        return 1;
      }
    } else {
      context->set_error(MISSING_GLOBAL_IDENTIFIER);
      return 1;
    }
  } else {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_error(UNKNOWN_ERROR);
  return 1;
}

int MulInst(Context* context) {
  if (context->token_to_scan == MUL) {
    context->token_to_scan = yylex();
    if (!RoundingMode(context)) { // with roundingmode

      BrigInstMod mul_op = {
        sizeof(mul_op),     // size
        BrigEInstMod,       // kind
        BrigMul,            // opcode
        Brigb32,            // type
        BrigNoPacking,      // packing
        {0, 0, 0, 0, 0},    // o_operands
        context->get_alu_modifier()   // aluModifier;
      };

      if (context->token_type == PACKING) {
        mul_op.packing = context->token_value.packing;
        context->token_to_scan = yylex();
      }
      if (context->token_type == DATA_TYPE_ID) {
        mul_op.type = context->token_value.data_type;
        context->token_to_scan = yylex();
        if (context->token_type == REGISTER) {
          std::string oper_name = context->token_value.string_val;
          if (Operand(context)) {
            return 1;
          }
          mul_op.o_operands[0] = context->operand_map[oper_name];
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if(context->token_type == REGISTER ) {
              std::string oper_name = context->token_value.string_val;
              if (Operand(context)) {
                return 1;
              }
              mul_op.o_operands[1] = context->operand_map[oper_name];
            } else if (context->token_type == CONSTANT ||
                       context->token_to_scan == TOKEN_WAVESIZE) {
              mul_op.o_operands[1] = context->get_operand_offset();
              if (Operand(context)) {
                return 1;
              }
            } else {
              context->set_error(MISSING_OPERAND);
            }
            if (context->token_to_scan == ',') {
              context->token_to_scan = yylex();
              if (context->token_type == REGISTER) {
                std::string oper_name = context->token_value.string_val;
                if (Operand(context)) {
                  return 1;
                }
              mul_op.o_operands[2] = context->operand_map[oper_name];
              } else if (context->token_type == CONSTANT ||
                       context->token_to_scan == TOKEN_WAVESIZE) {
                mul_op.o_operands[2] = context->get_operand_offset();
                if (Operand(context)) {
                  return 1;
                }
              } else {
              context->set_error(MISSING_OPERAND);
              }
              if (context->token_to_scan == ';') {
                context->append_code(&mul_op);
                context->token_to_scan = yylex();
                return 0;
              } else {
                context->set_error(MISSING_SEMICOLON);
              }
            } else {
              context->set_error(MISSING_COMMA);
            }
          }  else {
            context->set_error(MISSING_COMMA);
          }
        } else {
          context->set_error(MISSING_OPERAND);
        }
      } else {
        context->set_error(MISSING_DATA_TYPE);
      }
    } else { // without roundingmode
      if (context->token_type == PACKING) {
        context->token_to_scan = yylex();
      }
      if (context->token_type == DATA_TYPE_ID) {
        context->token_to_scan = yylex();
        if (!Operand(context)) {
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if (!Operand(context)) {
              if (context->token_to_scan == ',') {
                context->token_to_scan = yylex();
                if (!Operand(context)) {
                  if (context->token_to_scan == ';') {
                    context->token_to_scan = yylex();
                    return 0;
                  } else {
                    context->set_error(MISSING_SEMICOLON);
                  }
                } else {
                  context->set_error(MISSING_OPERAND);
                }
              } else {
                context->set_error(MISSING_COMMA);
              }
            } else {
              context->set_error(MISSING_OPERAND);
            }
          } else {
            context->set_error(MISSING_COMMA);
          }
        } else {
          context->set_error(MISSING_OPERAND);
        }
      } else {
        context->set_error(MISSING_DATA_TYPE);
      }
    }
  } else if (context->token_to_scan == MUL_HI) {
    context->token_to_scan = yylex();
    if (context->token_type == PACKING) {
      context->token_to_scan = yylex();
    }
    if (context->token_type == DATA_TYPE_ID) {
      context->token_to_scan = yylex();
      if (!Operand(context)) {
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
          if (!Operand(context)) {
            if (context->token_to_scan == ',') {
              context->token_to_scan = yylex();
              if (!Operand(context)) {
                if (context->token_to_scan == ';') {
                  context->token_to_scan = yylex();
                  return 0;
                } else {
                    context->set_error(MISSING_SEMICOLON);
                }
              } else {
                context->set_error(MISSING_OPERAND);
              }
            } else {
              context->set_error(MISSING_COMMA);
            }
          } else {
            context->set_error(MISSING_OPERAND);
          }
        } else {
          context->set_error(MISSING_COMMA);
        }
      } else {
        context->set_error(MISSING_OPERAND);
      }
    } else {
      context->set_error(MISSING_DATA_TYPE);
    }
  }
  return 1;
}

int Mul24Inst(Context* context) {
  if (context->token_to_scan == MUL24_HI ||
      context->token_to_scan == MUL24) {
    context->token_to_scan = yylex();
    if (context->token_type == DATA_TYPE_ID) {
      context->token_to_scan = yylex();
      if (!Operand(context)) {
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
          if (!Operand(context)) {
            if (context->token_to_scan == ',') {
              context->token_to_scan = yylex();
              if (!Operand(context)) {
                if (context->token_to_scan == ';') {
                  context->token_to_scan = yylex();
                  return 0;
                } else {
                  context->set_error(MISSING_SEMICOLON);
                }
              } else {
                context->set_error(MISSING_OPERAND);
              }
            } else {
              context->set_error(MISSING_COMMA);
            }
          } else {
            context->set_error(MISSING_OPERAND);
          }
        } else {
          context->set_error(MISSING_COMMA);
        }
      } else {
        context->set_error(MISSING_OPERAND);
      }
    } else {
      context->set_error(MISSING_DATA_TYPE);
    }
  }
  return 1;
}

int Mad24Inst(Context* context) {
  if (context->token_to_scan == MAD24_HI ||
      context->token_to_scan == MAD24) {
    context->token_to_scan = yylex();
    if (context->token_type == DATA_TYPE_ID) {
      context->token_to_scan = yylex();
      if (!Operand(context)) {
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
          if (!Operand(context)) {
            if (context->token_to_scan == ',') {
              context->token_to_scan = yylex();
              if (!Operand(context)) {
                if (context->token_to_scan == ',') {
                context->token_to_scan = yylex();
                  if (!Operand(context)) {
                    if (context->token_to_scan == ';') {
                      context->token_to_scan = yylex();
                      return 0;
                    } else {
                      context->set_error(MISSING_SEMICOLON);
                    }
                  } else {
                    context->set_error(MISSING_OPERAND);
                  }
                } else {
                  context->set_error(MISSING_COMMA);
                }
              } else {
                context->set_error(MISSING_OPERAND);
              }
            } else {
              context->set_error(MISSING_COMMA);
            }
          } else {
            context->set_error(MISSING_OPERAND);
          }
        } else {
          context->set_error(MISSING_COMMA);
        }
      } else {
        context->set_error(MISSING_OPERAND);
      }
    } else {
      context->set_error(MISSING_DATA_TYPE);
    }
  }
  return 1;
}

int Mul(Context* context) {
  if (!MulInst(context)) {
    return 0;
  } else if (!Mul24Inst(context)) {
    return 0;
  } else if (!Mad24Inst(context)) {
    return 0;
  }
  return 1;
}

int LdModifierPart2(Context *context, BrigInstLdSt* pLdSt_op, int* pVec_size) {
  while (1) {
    if (context->token_type == VECTOR) {
      switch (context->token_to_scan) {
        case _V2:
          *pVec_size = 2;
          break;
        case _V4:
          *pVec_size = 4;
          break;
        default:
          *pVec_size = 0;
      }
      context->token_to_scan = yylex();
      continue;
    }

    if (context->token_type == ADDRESS_SPACE_IDENTIFIER) {
      pLdSt_op->storageClass = context->token_value.storage_class;
      context->token_to_scan = yylex();
      continue;
    }

    if (context->token_to_scan == _REL) {
      pLdSt_op->memorySemantic = BrigRelease;
      context->token_to_scan = yylex();
      continue;
    }
    if (context->token_to_scan == _ACQ) {
      pLdSt_op->memorySemantic = BrigAcquire;
      context->token_to_scan = yylex();
      continue;
    }
    if (context->token_to_scan == _DEP) {
      pLdSt_op->memorySemantic = BrigDep;
      context->token_to_scan = yylex();
      continue;
    }

    if (context->token_to_scan == _REGION) {
      // TODO(Chuang) need to check "_region" out again.
      pLdSt_op->memorySemantic = BrigRegular;
      context->token_to_scan = yylex();
      continue;
    }

    if (context->token_to_scan == _EQUIV) {
      context->token_to_scan = yylex();

      if (context->token_to_scan == '(') {
        context->token_to_scan = yylex();
        if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
          pLdSt_op->equivClass = context->token_to_scan;
          context->token_to_scan = yylex();
          if (context->token_to_scan == ')') {
            context->token_to_scan = yylex();
            continue;
          } else {  // ')'
            context->set_error(MISSING_CLOSING_PARENTHESIS);
            return 1;
          }
        } else {  // Integer Constant
          context->set_error(MISSING_INTEGER_CONSTANT);
          return 1;
        }
      } else {  // '('
        context->set_error(MISSING_OPENNING_BRACKET);
        return 1;
      }
    }
    return 0;
  }

  context->set_error(UNKNOWN_ERROR);
  return 1;
}

int LdModifier(Context *context){
 BrigInstLdSt ld_op = {
    sizeof(BrigInstLdSt),     // size
    BrigEInstLdSt,            // kind
    BrigLd,                   // opcode
    Brigb32,                  // type
    BrigNoPacking,            // packing
    {0, 0, 0, 0, 0},          // operand[5]
    BrigFlatSpace,            // storageClass
    BrigRegular,              // memorySemantic
    0                         // equivClass
  };
  int vector_size = 0;
  return LdModifierPart2(context, &ld_op, &vector_size);
}

int Ld(Context* context) {
  // first token is LD
  BrigInstLdSt ld_op = {
    sizeof(BrigInstLdSt),     // size
    BrigEInstLdSt,            // kind
    BrigLd,                   // opcode
    Brigb32,                  // type
    BrigNoPacking,            // packing
    {0, 0, 0, 0, 0},          // operand[5]
    BrigFlatSpace,            // storageClass
    BrigRegular,              // memorySemantic
    0                         // equivClass
  };
  int vector_size = 0;
  context->token_to_scan = yylex();
  if (context->token_to_scan == _WIDTH) {
    ld_op.o_operands[0] = context->get_operand_offset();
    if (!OptionalWidth(context)) {
    } else {
      context->set_error(UNKNOWN_ERROR);
      return 1;
    }
  }
  if (!LdModifierPart2(context, &ld_op, &vector_size)) {
  } else {
    context->set_error(UNKNOWN_ERROR);
    return 1;
  }

  if (context->token_type == DATA_TYPE_ID) {
    // Get Type value in here
    ld_op.type = context->token_value.data_type;
    context->token_to_scan = yylex();
    if (context->token_to_scan == '(') {
      ld_op.o_operands[1] = context->get_operand_offset();
      if (!ArrayOperandList(context)) {
      } else {
        context->set_error(MISSING_CLOSING_PARENTHESIS);
        return 1;
      }
    } else {
      std::string oper_name = context->token_value.string_val;
      if (!Operand(context)) {
      ld_op.o_operands[1] = context->operand_map[oper_name];
      } else {
        context->set_error(INVALID_OPERAND);
        return 1;
      }
    }
    if (context->token_to_scan == ',') {
      context->token_to_scan = yylex();

      ld_op.o_operands[2] = context->get_operand_offset();
      if (!MemoryOperand(context)) {
        if (context->token_to_scan == ';') {
          context->append_code(&ld_op);
          context->token_to_scan = yylex();
          return 0;
        } else {  // ';'
          context->set_error(MISSING_SEMICOLON);
          return 1;
        }
      } else {  // Memory Operand
        context->set_error(UNKNOWN_ERROR);
        return 1;
      }
    } else {  // ','
      context->set_error(MISSING_COMMA);
      return 1;
    }
  } else {  // Data Type
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_error(UNKNOWN_ERROR);
  return 1;
}

int St(Context* context) {
  // first token is St
  BrigInstLdSt st_op = {
    sizeof(BrigInstLdSt),     // size
    BrigEInstLdSt,            // kind
    BrigSt,                   // opcode
    Brigb32,                  // type
    BrigNoPacking,            // packing
    {0, 0, 0, 0, 0},          // operand[5]
    BrigFlatSpace,            // storageClass
    BrigRegular,              // memorySemantic
    0                         // equivClass
  };
  int vector_size = 0;
  context->token_to_scan = yylex();

  if (!LdModifierPart2(context, &st_op, &vector_size)) {
  } else {
    context->set_error(UNKNOWN_ERROR);
    return 1;
  }

  if (context->token_type == DATA_TYPE_ID) {
    st_op.type = context->token_value.data_type;
    context->token_to_scan = yylex();
    if (context->token_to_scan == '(') {
      st_op.o_operands[0] = context->get_operand_offset();

      if (!ArrayOperandList(context)) {
      } else {
        context->set_error(MISSING_CLOSING_PARENTHESIS);
        return 1;
      }
    } else {
      std::string oper_name = context->token_value.string_val;
      if (!Operand(context)) {
        st_op.o_operands[0] = context->operand_map[oper_name];
      } else {
        context->set_error(INVALID_OPERAND);
        return 1;
      }
    }
    if (context->token_to_scan == ',') {
      context->token_to_scan = yylex();
      st_op.o_operands[1] = context->get_operand_offset();
      if (!MemoryOperand(context)) {
        if (context->token_to_scan == ';') {
          context->token_to_scan = yylex();
          context->append_code(&st_op);
          return 0;
        } else {  // ';'
          context->set_error(MISSING_SEMICOLON);
          return 1;
        }
      } else {  // Memory Operand
        context->set_error(UNKNOWN_ERROR);
        return 1;
      }
    } else {  // ','
      context->set_error(MISSING_COMMA);
      return 1;
    }
  } else {  // Data Type
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_error(UNKNOWN_ERROR);
  return 1;
}

int Lda(Context* context) {
  // first token is LDA
  context->token_to_scan = yylex();

  BrigInstMem lda_op = {
    36,                    // size
    BrigEInstMem,          // kind
    BrigLda,               // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0},       // o_operands[5]
    BrigFlatSpace          // storageClass
  };

  if (context->token_type == ADDRESS_SPACE_IDENTIFIER) {
    lda_op.storageClass = context->token_value.storage_class;
    context->token_to_scan = yylex();
  }
  if (context->token_type == DATA_TYPE_ID) {
    lda_op.type = context->token_value.data_type;
    context->token_to_scan = yylex();

    std::string oper_name = context->token_value.string_val;

    if (!Operand(context)) {
      lda_op.o_operands[0] = context->operand_map[oper_name];
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        lda_op.o_operands[1] = context->get_operand_offset();
        if (!MemoryOperand(context)) {
          if (context->token_to_scan == ';') {
            context->append_code(&lda_op);
            context->token_to_scan = yylex();
            return 0;
          } else {  // ';'
            context->set_error(MISSING_SEMICOLON);
            return 1;
          }
        } else {  // MemoryOperand
          context->set_error(MISSING_OPERAND);
          return 1;
        }
      } else {  // ','
        context->set_error(MISSING_COMMA);
        return 1;
      }
    } else {  // Operand
      context->set_error(MISSING_OPERAND);
      return 1;
    }
  } else {  // Data Type
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_error(UNKNOWN_ERROR);
  return 1;
}

int OptacqregPart2(Context* context, BrigMemorySemantic32_t* pMemSemantic) {
  if (context->token_to_scan == _REL) {
    *pMemSemantic = BrigRelease;
    context->token_to_scan = yylex();
  } else if (context->token_to_scan == _ACQ) {
    *pMemSemantic = BrigAcquire;
    context->token_to_scan = yylex();
  } else if (context->token_to_scan == _AR) {
    *pMemSemantic = BrigAcquireRelease;
    context->token_to_scan = yylex();
  }
  return 0;
}
int Optacqreg(Context* context) {
  BrigMemorySemantic32_t temp;
  return OptacqregPart2(context, &temp);
}

int ImageRet(Context* context) {
  // first token is ATOMIC_IMAGE
  unsigned int second_token;
  BrigInstAtomicImage img_inst = {
    48,                     // size
    BrigEInstAtomicImage,   // kind
    BrigAtomicImage,        // opcode
    Brigb32,                // type
    BrigNoPacking,          // packing
    {0, 0, 0, 0, 0},        // o_operands[5]
    0,                      // atomicOperation
    BrigGlobalSpace,        // storageClass
    BrigRegular,            // memorySemantic
    0                       // geom
  };
  context->token_to_scan = yylex();
  second_token = context->token_to_scan;

  if (context->token_to_scan == _CAS) {
    img_inst.atomicOperation = BrigAtomicCas;
    context->token_to_scan = yylex();
  } else if (context->token_type == ATOMIC_OP) {
    switch (context->token_to_scan) {  // without _CAS_
      case _AND_:
        img_inst.atomicOperation = BrigAtomicAnd;
        break;
      case _OR_:
        img_inst.atomicOperation = BrigAtomicOr;
        break;
      case _XOR_:
        img_inst.atomicOperation = BrigAtomicXor;
        break;
      case _EXCH_:
        img_inst.atomicOperation = BrigAtomicExch;
        break;
      case _ADD_:
        img_inst.atomicOperation = BrigAtomicAdd;
        break;
      case _INC_:
        img_inst.atomicOperation = BrigAtomicInc;
        break;
      case _DEC_:
        img_inst.atomicOperation = BrigAtomicDec;
        break;
      case _MIN_:
        img_inst.atomicOperation = BrigAtomicMin;
        break;
      case _MAX_:
        img_inst.atomicOperation = BrigAtomicMax;
        break;
      case _SUB_:
        img_inst.atomicOperation = BrigAtomicSub;
        break;
      default:
        context->set_error(MISSING_DECLPREFIX);
        return 1;
    }
    context->token_to_scan =yylex();
  } else {
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }
  if (!OptacqregPart2(context, &img_inst.memorySemantic)) {
  }

  if (context->token_type == GEOMETRY_ID) {
    switch (context->token_to_scan) {
      case _1D:
        img_inst.geom = Briggeom_1d;
        break;
      case _2D:
        img_inst.geom = Briggeom_2d;
        break;
      case _3D:
        img_inst.geom = Briggeom_3d;
        break;
      case _1DB:
        img_inst.geom = Briggeom_1db;
        break;
      case _1DA:
        img_inst.geom = Briggeom_1da;
        break;
      case _2DA:
        img_inst.geom = Briggeom_2da;
        break;
      default:
        context->set_error(MISSING_DECLPREFIX);
        return 1;
    }
    context->token_to_scan = yylex();
    if (context->token_type == DATA_TYPE_ID) {
      img_inst.type = context->token_value.data_type;

      std::string op_name;
      context->token_to_scan = yylex();
      if (context->valid_string) {
        op_name = context->token_value.string_val;
      }
      if (!Operand(context)) {
        img_inst.o_operands[0] = context->operand_map[op_name];
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
          img_inst.o_operands[1] = context->get_operand_offset();
          if (context->token_to_scan == '[') {
            context->token_to_scan = yylex();
            if (!AddressableOperand(context)) {
              if (context->token_to_scan == ',') {
                context->token_to_scan = yylex();
                unsigned int curOpCount = 2;
                if (context->token_to_scan == '(') {
                  if (!ArrayOperandList(context)) {
                  } else {
                    context->set_error(MISSING_CLOSING_PARENTHESIS);
                    return 1;
                  }
                } else {
                  if (context->valid_string) {
                    op_name = context->token_value.string_val;
                  }
                  if (!Operand(context)) {
                    img_inst.o_operands[curOpCount++] = context->operand_map[op_name];
                  } else {  // Array Operand
                    context->set_error(MISSING_OPERAND);
                    return 1;
                  }
               }

                if (context->token_to_scan == ',') {
                  context->token_to_scan = yylex();
                  if (context->valid_string) {
                    op_name = context->token_value.string_val;
                  }
                  if (!Operand(context)) {
                    img_inst.o_operands[curOpCount++] = context->operand_map[op_name];
                    if (second_token == _CAS) {
                      if (context->token_to_scan == ',') {
                        context->token_to_scan = yylex();
                        if (context->valid_string) {
                          op_name = context->token_value.string_val;
                        }
                        if (!Operand(context)) {
                          img_inst.o_operands[curOpCount++] = context->operand_map[op_name];
                        } else {  // Operand
                          context->set_error(MISSING_OPERAND);
                          return 1;
                        }
                      } else {  // ','
                        context->set_error(MISSING_COMMA);
                        return 1;
                      }
                    }
                    if (context->token_to_scan == ';') {
                      context->append_code(&img_inst);
                      context->token_to_scan = yylex();
                      return 0;
                    } else {  // ';'
                      context->set_error(MISSING_SEMICOLON);
                      return 1;
                    }
                  } else {  // Operand
                    context->set_error(MISSING_OPERAND);
                    return 1;
                  }
                } else {  // ','
                  context->set_error(MISSING_COMMA);
                  return 1;
                }
              } else {  // ','
                context->set_error(MISSING_COMMA);
                return 1;
              }
            } else {  // Addressable Operand
              context->set_error(INVALID_OPERAND);
              return 1;
            }
          } else {
            context->set_error(MISSING_OPERAND);
          }
        } else {  // ','
          context->set_error(MISSING_COMMA);
        }
      } else {  // Operand
        context->set_error(MISSING_OPERAND);
      }
    } else {  // Data Type
      context->set_error(MISSING_DATA_TYPE);
    }
  } else {  // Geometry Id
    context->set_error(MISSING_DECLPREFIX);
  }
  return 1;
}
int ImageNoRet(Context* context) {
  // first token is ATOMICNORET_IMAGE
  BrigInstAtomicImage imgNoRet = {
    48,                     // size
    BrigEInstAtomicImage,   // kind
    BrigAtomicNoRetImage,   // opcode
    Brigb32,                // type
    BrigNoPacking,          // packing
    {0, 0, 0, 0, 0},        // o_operands[5]
    0,                      // atomicOperation
    BrigGlobalSpace,          // storageClass
    BrigRegular,            // memorySemantic
    0                       // geom
  };

  unsigned int second_token;
  context->token_to_scan = yylex();

  second_token = context->token_to_scan;

  if (context->token_to_scan == _CAS) {
    imgNoRet.atomicOperation = BrigAtomicCas;
    context->token_to_scan = yylex();
  } else if (context->token_type == ATOMIC_OP) {
    switch (context->token_to_scan) {  // without _CAS_
      case _AND_:
        imgNoRet.atomicOperation = BrigAtomicAnd;
        break;
      case _OR_:
        imgNoRet.atomicOperation = BrigAtomicOr;
        break;
      case _XOR_:
        imgNoRet.atomicOperation = BrigAtomicXor;
        break;
      case _EXCH_:
        imgNoRet.atomicOperation = BrigAtomicExch;
        break;
      case _ADD_:
        imgNoRet.atomicOperation = BrigAtomicAdd;
        break;
      case _INC_:
        imgNoRet.atomicOperation = BrigAtomicInc;
        break;
      case _DEC_:
        imgNoRet.atomicOperation = BrigAtomicDec;
        break;
      case _MIN_:
        imgNoRet.atomicOperation = BrigAtomicMin;
        break;
      case _MAX_:
        imgNoRet.atomicOperation = BrigAtomicMax;
        break;
      case _SUB_:
        imgNoRet.atomicOperation = BrigAtomicSub;
        break;
      default:
        context->set_error(MISSING_DECLPREFIX);
        return 1;
    }
    context->token_to_scan =yylex();
  } else {
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }
  if (!OptacqregPart2(context, &imgNoRet.memorySemantic)) {
  }

  if (context->token_type == GEOMETRY_ID) {
    switch (context->token_to_scan) {
      case _1D:
        imgNoRet.geom = Briggeom_1d;
        break;
      case _2D:
        imgNoRet.geom = Briggeom_2d;
        break;
      case _3D:
        imgNoRet.geom = Briggeom_3d;
        break;
      case _1DB:
        imgNoRet.geom = Briggeom_1db;
        break;
      case _1DA:
        imgNoRet.geom = Briggeom_1da;
        break;
      case _2DA:
        imgNoRet.geom = Briggeom_2da;
        break;
      default:
        context->set_error(MISSING_DECLPREFIX);
        return 1;
    }
    context->token_to_scan = yylex();
    if (context->token_type == DATA_TYPE_ID) {

      std::string op_name;

      imgNoRet.type = context->token_value.data_type;
      context->token_to_scan = yylex();
      if (context->token_to_scan == '[') {
        imgNoRet.o_operands[0] = context->get_operand_offset();
        context->token_to_scan = yylex();
        if (!AddressableOperand(context)) {
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if (!ArrayOperandPart2(context, &imgNoRet.o_operands[1])) {
              if (context->token_to_scan == ',') {
                context->token_to_scan = yylex();
                unsigned int opCount = 2;
                if (context->valid_string) {
                  op_name = context->token_value.string_val;
                }
                if (!Operand(context)) {
                  imgNoRet.o_operands[opCount++] = context->operand_map[op_name];
                  if (second_token == _CAS) {
                    if (context->token_to_scan == ',') {
                      context->token_to_scan = yylex();
                      if (context->valid_string) {
                        op_name = context->token_value.string_val;
                      }
                      if (!Operand(context)) {
                        imgNoRet.o_operands[opCount++] = context->operand_map[op_name];
                      } else {  // Operand
                        context->set_error(MISSING_OPERAND);
                        return 1;
                      }
                    } else {  // ','
                      context->set_error(MISSING_COMMA);
                      return 1;
                    }
                  }
                  if (context->token_to_scan == ';') {
                    context->append_code(&imgNoRet);
                    context->token_to_scan = yylex();
                    return 0;
                  } else {  // ';'
                    context->set_error(MISSING_SEMICOLON);
                    return 1;
                  }
                } else {  // Operand
                  context->set_error(MISSING_OPERAND);
                  return 1;
                }
              } else {  // ','
                context->set_error(MISSING_COMMA);
                return 1;
              }
            } else {  // Array Operand
              context->set_error(MISSING_OPERAND);
              return 1;
            }
          } else {  // ','
            context->set_error(MISSING_COMMA);
            return 1;
          }
        } else {  // Addressable Operand
          context->set_error(INVALID_OPERAND);
          return 1;
        }
      } else {
        context->set_error(MISSING_OPERAND);
      }
    } else {  // Data Type
      context->set_error(MISSING_DATA_TYPE);
      return 1;
    }
  } else {  // Geometry Id
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }

  return 1;
}

int Cvt(Context* context) {
  // first token is CVT "cvt"
  // TODO(Chuang): Extensions for Conversions

  BrigInstCvt cvtInst = {
    40,                    // size
    BrigEInstCvt,          // kind
    BrigCvt,               // opcode
    0,                     // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0},       // o_operands[5]
    {0, 0, 0, 0, 0, 0, 0}, // aluModifier
    0,                     // stype
    0                      // reserved
  };
  context->token_to_scan = yylex();

  if (!CvtModifier1(context)) {
    BrigAluModifier getAluMod = context->get_alu_modifier();
    memcpy(&cvtInst.aluModifier, &getAluMod, sizeof(cvtInst.aluModifier));
  }

  // destType: b, u, s, f. (For b, only b1 is supported.)
  // destLength: 1, 8, 16, 32, 64. (For 1, only b1 is supported.)

  if (context->token_to_scan == _B1 ||
      context->token_to_scan == _S8 ||
      context->token_to_scan == _S16 ||
      context->token_to_scan == _S32 ||
      context->token_to_scan == _S64 ||
      context->token_to_scan == _U8 ||
      context->token_to_scan == _U16 ||
      context->token_to_scan == _U32 ||
      context->token_to_scan == _U64 ||
      context->token_to_scan == _F16 ||
      context->token_to_scan == _F32 ||
      context->token_to_scan == _F64) {

    cvtInst.type = context->token_value.data_type;
    context->token_to_scan = yylex();

    if (context->token_type == DATA_TYPE_ID) {
      cvtInst.stype = context->token_value.data_type;
      context->token_to_scan = yylex();
      std::string opName;
      BrigoOffset32_t opSize = 0;

      // Note: dest: Destination register.
      if (context->token_type == REGISTER) {
        opName = context->token_value.string_val;
      } else {
        context->set_error(INVALID_OPERAND);
        return 1;
      }
      if (!Operand(context)) {
        cvtInst.o_operands[0] = context->operand_map[opName];
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();

          opSize = context->get_operand_offset();
          if (context->valid_string) {
            opName = context->token_value.string_val;
          } else {
            if (context->token_type == CONSTANT) {
              opSize += opSize & 0x7;
            }
          }
          if (!Operand(context)) {
            if (opSize == context->get_operand_offset()) {
              cvtInst.o_operands[1] = context->operand_map[opName];
            } else {
              cvtInst.o_operands[1] = opSize;
            }
            if (context->token_to_scan == ';') {
              context->append_code(&cvtInst);
              context->token_to_scan = yylex();
              return 0;
            } else {
              context->set_error(MISSING_SEMICOLON);
            }
          } else {
            context->set_error(MISSING_OPERAND);
          }
        } else {
          context->set_error(MISSING_COMMA);
        }
      } else {
        context->set_error(INVALID_OPERAND);
      }
    } else {
      context->set_error(MISSING_DATA_TYPE);
    }
  } else {
    context->set_error(MISSING_DATA_TYPE);
  }
  return 1;
}

int Instruction0(Context* context) {
  // first token is NOP "nop"
  context->token_to_scan = yylex();
  if (context->token_to_scan == ';') {
    context->token_to_scan = yylex();
    return 0;
  } else {
    context->set_error(MISSING_SEMICOLON);
  }
  return 1;
}

int Instruction1(Context* context) {

  BrigInstBase inst1_op = {
    sizeof(inst1_op),
    BrigEInstBase,
    BrigLaneId,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };

  inst1_op.opcode = context->token_value.opcode;

  if (context->token_type == INSTRUCTION1_OPCODE_NODT) {
    // Instruction1OpcodeNoDT
    // debugtrap require operand must be reg,imm,wavesize
    if (context->token_to_scan == DEBUGTRAP) {
      context->token_to_scan = yylex();
      if (!RoundingMode(context)) {
      }
      if (context->token_type == REGISTER) {
        std::string oper_name = context->token_value.string_val;
        if (Operand(context)) {
          return 1;
        }
        inst1_op.o_operands[0] = context->operand_map[oper_name];
        if (context->token_to_scan == ';') {
          context->append_code(&inst1_op);
          context->token_to_scan = yylex();
          return 0;
        } else {
           context->set_error(MISSING_SEMICOLON);
        }
      } else if (context->token_type == CONSTANT ||
                 context->token_to_scan == TOKEN_WAVESIZE) {
        inst1_op.o_operands[0] = context->get_operand_offset();
        if (Operand(context)) {
          return 1;
        }
        if (context->token_to_scan == ';') {
          context->append_code(&inst1_op);
          context->token_to_scan = yylex();
          return 0;
        } else {
          context->set_error(MISSING_SEMICOLON);
        }
      } else {
        context->set_error(MISSING_OPERAND);
      }
    } else {
    context->token_to_scan = yylex();
    if (!RoundingMode(context)) {
    }
      if (context->token_to_scan == TOKEN_SREGISTER) {
        std::string oper_name = context->token_value.string_val;
        if (Operand(context)) {
          return 1;
        }
        inst1_op.o_operands[0] = context->operand_map[oper_name];
        if (context->token_to_scan == ';') {
          context->append_code(&inst1_op);
          context->token_to_scan = yylex();
          return 0;
        } else {
          context->set_error(MISSING_SEMICOLON);
        }
      } else {
        context->set_error(MISSING_OPERAND);
      }
    }
  } else if (context->token_to_scan == CLOCK) {  // clock
    context->token_to_scan = yylex();
    if (context->token_to_scan == TOKEN_DREGISTER) {
      std::string oper_name = context->token_value.string_val;
      if (Operand(context)) {
        return 1;
      }
      inst1_op.o_operands[0] = context->operand_map[oper_name];
      if (context->token_to_scan == ';') {
        context->append_code(&inst1_op);
        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(MISSING_SEMICOLON);
      }
    } else {
      context->set_error(MISSING_OPERAND);
    }
  } else if (context->token_type == INSTRUCTION1_OPCODE) {
    // instruction1opcode
    // fbar_release can have one or two operands
    if (context->token_to_scan == FBAR_RELEASE) {
      context->token_to_scan = yylex();
      if (!RoundingMode(context)) {
      }
      if (context->token_to_scan == _B64) {
        inst1_op.type = context->token_value.data_type;
        context->token_to_scan = yylex();
        if (context->token_to_scan == TOKEN_DREGISTER) {
          std::string oper_name = context->token_value.string_val;
          if (Operand(context)) {
            return 1;
          }
          inst1_op.o_operands[0] = context->operand_map[oper_name];
          if (context->token_to_scan == ',') { // two operands
            context->token_to_scan = yylex();
            if (context->token_to_scan == TOKEN_DREGISTER) {
              std::string oper_name = context->token_value.string_val;
              if (Operand(context)) {
                return 1;
              }
              inst1_op.o_operands[1] = context->operand_map[oper_name];
                if (context->token_to_scan == ';') {
                  context->append_code(&inst1_op);
                  context->token_to_scan = yylex();
                  return 0;
                } else {
                  context->set_error(MISSING_SEMICOLON);
                }
            } else {
              context->set_error(MISSING_OPERAND);
            } // one operand
          } else if (context->token_to_scan == ';') {
            context->append_code(&inst1_op);
            context->token_to_scan = yylex();
            return 0;
          } else {
            context->set_error(UNKNOWN_ERROR);
          }
        } else {
          context->set_error(MISSING_OPERAND);
        }
      } else {
        context->set_error(MISSING_DATA_TYPE);
      }
    } else {
      context->token_to_scan = yylex();
      if (!RoundingMode(context)) {
      }
      if (context->token_type == DATA_TYPE_ID) {
        inst1_op.type = context->token_value.data_type;
        context->token_to_scan = yylex();
        if (context->token_type == REGISTER) {
          std::string oper_name = context->token_value.string_val;
          if (Operand(context)) {
            return 1;
          }
          inst1_op.o_operands[0] = context->operand_map[oper_name];
          if (context->token_to_scan == ';') {
            context->append_code(&inst1_op);
            context->token_to_scan = yylex();
            return 0;
          } else {
            context->set_error(MISSING_SEMICOLON);
          }
        } else {
          context->set_error(MISSING_OPERAND);
        }
      } else {
        context->set_error(MISSING_DATA_TYPE);
      }
    }
  }
  return 1;
}

// this function specifies operand must be register,immediate value,or WAVESIZE
int RIW_Operand(Context* context) {
  if (context->token_type == REGISTER) {
    context->token_to_scan = yylex(); // set token for next function
    return 0;
  } else if (context->token_type == CONSTANT) {
    context->token_to_scan = yylex();
    return 0;
  } else if (context->token_to_scan == TOKEN_WAVESIZE) {
    context->token_to_scan = yylex();
    return 0;
  } else {
    context->set_error(INVALID_OPERATION);
  }
  return 1;
}

int Segp(Context* context) {
  if (context->token_to_scan == SEGMENTP) { //segmentp
    context->token_to_scan = yylex();

    BrigInstMem segmentp_op = {
      36,                    // size
      BrigEInstMem,          // kind
      BrigSegmentp,          // opcode
      Brigb32,               // type
      BrigNoPacking,         // packing
      {0, 0, 0, 0, 0},       // o_operands[5]
      BrigFlatSpace          // storageClass
    };

    if (context->token_type == ADDRESS_SPACE_IDENTIFIER) {
      switch (context->token_to_scan) {
        case _GLOBAL:
          segmentp_op.storageClass = BrigGlobalSpace;
          break;
        case _GROUP:
          segmentp_op.storageClass = BrigGroupSpace;
          break;
        case _PRIVATE:
          segmentp_op.storageClass = BrigPrivateSpace;
          break;
        case _KERNARG:
          segmentp_op.storageClass = BrigKernargSpace;
          break;
        case _READONLY:
          segmentp_op.storageClass = BrigReadonlySpace;
          break;
        case _SPILL:
          segmentp_op.storageClass = BrigSpillSpace;
          break;
        case _ARG:
          segmentp_op.storageClass = BrigArgSpace;
          break;
        default:
          segmentp_op.storageClass = BrigFlatSpace;
      }
      context->token_to_scan = yylex();
      if (context->token_to_scan == _B1) { //datatypeId must be b1
        segmentp_op.type = context->token_value.data_type;
        context->token_to_scan = yylex();
        //dest must be c register
        if (context->token_to_scan == TOKEN_CREGISTER) {
          std::string oper_name = context->token_value.string_val;
          if (Operand(context)) {
            return 1;
          }
          segmentp_op.o_operands[0] = context->operand_map[oper_name];
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if(context->token_type == REGISTER ) {
              std::string oper_name = context->token_value.string_val;
              if (Operand(context)) {
                return 1;
              }
              segmentp_op.o_operands[1] = context->operand_map[oper_name];
              if (context->token_to_scan == ';') {
                context->append_code(&segmentp_op);
                context->token_to_scan = yylex();
                return 0;
              } else {
                context->set_error(MISSING_SEMICOLON);
              }
            } else if (context->token_type == CONSTANT ||
                       context->token_to_scan == TOKEN_WAVESIZE) {
              segmentp_op.o_operands[1] = context->get_operand_offset();
              if (Operand(context)) {
                return 1;
              }
              if (context->token_to_scan == ';') {
                context->append_code(&segmentp_op);
                context->token_to_scan = yylex();
                return 0;
              } else {
                context->set_error(MISSING_SEMICOLON);
              }
            } else {
              context->set_error(MISSING_OPERAND);
            }
          } else {
            context->set_error(MISSING_COMMA);
          }
        } else {
          context->set_error(MISSING_OPERAND);
        }
      } else {
        context->set_error(MISSING_DATA_TYPE);
      }
    } else {
      // should be missing ADDRESS_SPACE_IDENTIFIER
      context->set_error(UNKNOWN_ERROR);
    }
  } else if (context->token_to_scan == STOF || // stof or ftos
             context->token_to_scan == FTOS) {
    
    BrigInstMem sf_op = {
      36,                    // size
      BrigEInstMem,          // kind
      BrigStoF,              // opcode
      Brigb32,               // type
      BrigNoPacking,         // packing
      {0, 0, 0, 0, 0},       // o_operands[5]
      BrigFlatSpace          // storageClass
    };

    switch (context->token_to_scan) {
      case STOF:
        sf_op.opcode = BrigStoF;
        break;
      case FTOS:
        sf_op.opcode = BrigFtoS;
        break;
    }
    context->token_to_scan = yylex();
    if (context->token_type == ADDRESS_SPACE_IDENTIFIER) {
      switch (context->token_to_scan) {
        case _GLOBAL:
          sf_op.storageClass = BrigGlobalSpace;
          break;
        case _GROUP:
          sf_op.storageClass = BrigGroupSpace;
          break;
        case _PRIVATE:
          sf_op.storageClass = BrigPrivateSpace;
          break;
        case _KERNARG:
          sf_op.storageClass = BrigKernargSpace;
          break;
        case _READONLY:
          sf_op.storageClass = BrigReadonlySpace;
          break;
        case _SPILL:
          sf_op.storageClass = BrigSpillSpace;
          break;
        case _ARG:
          sf_op.storageClass = BrigArgSpace;
          break;
        default:
          sf_op.storageClass = BrigFlatSpace;
      }
      context->token_to_scan = yylex();
      if (context->token_to_scan == _U32 ||
          context->token_to_scan == _U64) { //datatypeId must be u32 or u64
        sf_op.type = context->token_value.data_type;
        context->token_to_scan = yylex();
        //dest must be d register
        if (context->token_to_scan == TOKEN_DREGISTER) {
          std::string oper_name = context->token_value.string_val;
          if (Operand(context)) {
            return 1;
          }
          sf_op.o_operands[0] = context->operand_map[oper_name];
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if(context->token_type == REGISTER ) {
              std::string oper_name = context->token_value.string_val;
              if (Operand(context)) {
                return 1;
              }
              sf_op.o_operands[1] = context->operand_map[oper_name];
              if (context->token_to_scan == ';') {
                context->append_code(&sf_op);
                context->token_to_scan = yylex();
                return 0;
              } else {
                context->set_error(MISSING_SEMICOLON);
              }
            } else if (context->token_type == CONSTANT ||
                       context->token_to_scan == TOKEN_WAVESIZE) {
              sf_op.o_operands[1] = context->get_operand_offset();
              if (Operand(context)) {
                return 1;
              }
              if (context->token_to_scan == ';') {
                context->append_code(&sf_op);
                context->token_to_scan = yylex();
                return 0;
              } else {
                context->set_error(MISSING_SEMICOLON);
              }
            } else {
              context->set_error(MISSING_OPERAND);
            }
          } else {
            context->set_error(MISSING_COMMA);
          }
        } else {
          context->set_error(MISSING_OPERAND);
        }
      } else {
        context->set_error(MISSING_DATA_TYPE);
      }
    } else {
      // should be missing ADDRESS_SPACE_IDENTIFIER
      context->set_error(UNKNOWN_ERROR);
    }
  }
  return 1;
}

int Operation(Context* context) {
  if (context->token_type == INSTRUCTION1_OPCODE_NODT ||
      context->token_to_scan == CLOCK ||
      context->token_type == INSTRUCTION1_OPCODE) {
    if (!Instruction1(context)) {
      return 0;
    }
  } else if (context->token_to_scan == NOP) {
    if (!Instruction0(context)) {
      return 0;
    }
  } else if (context->token_type == INSTRUCTION2_OPCODE_FTZ ||
             context->token_type == INSTRUCTION2_OPCODE ||
             context->token_type == INSTRUCTION2_OPCODE_NODT) {
    if (!Instruction2(context)) {
      return 0;
    }
  } else if (context->token_type == INSTRUCTION3_OPCODE ||
             context->token_type == INSTRUCTION3_OPCODE_FTZ) {
    if (!Instruction3(context)) {
      return 0;
    }
  } else if (context->token_type == INSTRUCTION4_OPCODE) {
    if (!Instruction4(context)) {
      return 0;
    }
  } else if (context->token_to_scan == PACKEDCMP ||
             context->token_to_scan == CMP) {
    if (!Cmp(context)) {
      return 0;
    }
  } else if (context->token_to_scan == MUL ||
             context->token_to_scan == MUL_HI ||
             context->token_to_scan == MUL24_HI ||
             context->token_to_scan == MUL24 ||
             context->token_to_scan == MAD24 ||
             context->token_to_scan == MAD24_HI) {
    if (!Mul(context)) {
      return 0;
    }
  } else if (context->token_to_scan == F2U4) {
    if (!Instruction5(context)) {
      return 0;
    }
  } else if (context->token_to_scan == MOV) {
    if (!Mov(context)) {
      return 0;
    }
  } else if (context->token_to_scan == SEGMENTP ||
             context->token_to_scan == FTOS ||
             context->token_to_scan == STOF) {
    if (!Segp(context)) {
      return 0;
    }
  } else if (context->token_to_scan == LDA) {
    if (!Lda(context)) {
      return 0;
    }
  } else if (context->token_to_scan == LDC) {
    if (!Ldc(context)) {
      return 0;
    }
  } else if (context->token_to_scan == ATOMIC ||
             context->token_to_scan == ATOMIC_CAS) {
    if (!Atom(context)) {
      return 0;
    }
  } else if (context->token_to_scan == RD_IMAGE) {
    if (!ImageRead(context)) {
      return 0;
    }
  } else if (context->token_to_scan == LD_IMAGE) {
    if (!ImageLoad(context)) {
      return 0;
    }
  } else if (context->token_to_scan == ST_IMAGE) {
    if (!ImageStore(context)) {
      return 0;
    }
  } else if (context->token_to_scan == LD) {
    if (!Ld(context)) {
      return 0;
    }
  } else if (context->token_to_scan == ST) {
    if (!St(context)) {
      return 0;
    }
  } else if (context->token_to_scan == CVT) {
    if (!Cvt(context)) {
      return 0;
    }
  } else if (context->token_to_scan == ATOMICNORET ||
             context->token_to_scan == ATOMICNORET_CAS) {
    if (!AtomicNoRet(context)) {
      return 0;
    }
  } else if (context->token_to_scan == ATOMIC_IMAGE) {
    if (!ImageRet(context)) {
      return 0;
    }
  } else if (context->token_to_scan == SYNC) {
    if (!Sync(context)) {
      return 0;
    }
  } else if (context->token_to_scan == BARRIER) {
    if (!Bar(context)) {
      return 0;
    }
  } else if (context->token_to_scan == SYSCALL) {
    if (!SysCall(context)) {
      return 0;
    }
  }  else if (context->token_to_scan == RET) {
    if (!Ret(context)) {
      return 0;
    }
  } else if (context->token_to_scan == CBR ||
             context->token_to_scan == BRN) {
    if (!Branch(context)) {
      return 0;
    }
  } else if (context->token_type == QUERY_OP) {
    if (!Query(context)) {
      return 0;
    }
  }
  return 1;
}

int BodyStatementNested(Context* context) {
  if (context->token_to_scan == TOKEN_COMMENT) {
    context->token_to_scan = yylex();
    return 0;
  } else if (context->token_to_scan == PRAGMA) {
    if (!Pragma(context)) {
      return 0;
    }
  } else if (context->token_to_scan == BLOCK) {
    if (!Block(context)) {
      return 0;
    }
  } else if (context->token_to_scan == ALIGN ||
             context->token_to_scan == CONST ||
             context->token_to_scan == EXTERN ||
             context->token_to_scan == STATIC) {
    if (!DeclPrefix(context)) {
      if (!InitializableDecl(context)) {
        return 0;
      } else if (!UninitializableDecl(context)) {
        return 0;
      }
    }
  } else if (context->token_to_scan == LOC) {
    if (!Location(context)) {
      return 0;
    }
  } else if (context->token_to_scan == TOKEN_LABEL) {
    context->token_to_scan = yylex();
    return 0;
  } else if (!LabelTargets(context)) {
    return 0;
  } else if (!Operation(context)) {
    return 0;
  }
  return 1;
}

int ArgStatement(Context* context) {
  if (context->token_to_scan == CALL) {
    if (!Call(context)) {
      return 0;
    }
  } else if (!BodyStatementNested(context)) {
    return 0;
  } else if (context->token_to_scan == ALIGN ||
             context->token_to_scan == CONST ||
             context->token_to_scan == EXTERN ||
             context->token_to_scan == STATIC) {
    if (!DeclPrefix(context)) {
      if (!ArgUninitializableDecl(context)) {
        return 0;
      }
    }
  }
  return 1;
}

int ArgStatements(Context* context) {
  if (!ArgStatement(context)) {
    while (!ArgStatement(context)) {
      ;
    }
    return 0;
  }
  return 1;
}

int BodyStatement(Context* context) {
  if (context->token_to_scan == TOKEN_COMMENT) {
    context->token_to_scan = yylex();
    return 0;
  } else if (context->token_to_scan == PRAGMA) {
    if (!Pragma(context)) {
      return 0;
    }
  } else if (context->token_to_scan == BLOCK) {
    if (!Block(context)) {
      return 0;
    }
  } else if (context->token_to_scan == ALIGN ||
             context->token_to_scan == CONST ||
             context->token_to_scan == EXTERN ||
             context->token_to_scan == STATIC) {
    if (!DeclPrefix(context)) {
      if (!InitializableDecl(context)) {
        return 0;
      } else if (!UninitializableDecl(context)) {
        return 0;
      }
    }
  } else if (context->token_to_scan == '{') {
    if (!ArgBlock(context)) {
      return 0;
    }
  } else if (context->token_to_scan == LOC) {
    if (!Location(context)) {
      return 0;
    }
  } else if (context->token_to_scan == TOKEN_LABEL) {
    context->token_to_scan = yylex();
    return 0;
  } else if (!LabelTargets(context)) {
    return 0;
  } else if (!Operation(context)) {
    return 0;
  }
  return 1;
}

int BodyStatements(Context* context) {
  if (!BodyStatement(context)) {
    while (!BodyStatement(context)) {
      ;
    }
    return 0;
  }
  return 1;
}

int ImageLoad(Context* context) {
  // first token is LD_IMAGE
  BrigInstImage imgLdInst = {
    40,                    // size
    BrigEInstImage,        // kind
    BrigLdImage,           // opcode
    {0, 0, 0, 0, 0},   // o_operands[5]
    0,                 // geom
    0,                 // type
    0,                 // stype
    BrigNoPacking,         // packing
    0                      // reserved
  };
  context->token_to_scan = yylex();
  if (context->token_to_scan == _V4) {
    context->token_to_scan = yylex();
    if (context->token_type == GEOMETRY_ID) {
      switch (context->token_to_scan) {
        case _1D:
          imgLdInst.geom = Briggeom_1d;
          break;
        case _2D:
          imgLdInst.geom = Briggeom_2d;
          break;
        case _3D:
          imgLdInst.geom = Briggeom_3d;
          break;
        case _1DB:
          imgLdInst.geom = Briggeom_1db;
          break;
        case _1DA:
          imgLdInst.geom = Briggeom_1da;
          break;
        case _2DA:
          imgLdInst.geom = Briggeom_2da;
          break;
        default:
          context->set_error(MISSING_DECLPREFIX);
          return 1;
      }
      context->token_to_scan = yylex();
      // Note: destLength: Destination length: 32 (f32, u32, or s32)
      if (context->token_to_scan == _F32 ||
          context->token_to_scan == _U32 ||
          context->token_to_scan == _S32) {

        imgLdInst.type = context->token_value.data_type;
        context->token_to_scan = yylex();
        // Note: srcLength: Source length: 32 srcType: u32.
        if (context->token_to_scan == _U32) {
          imgLdInst.stype = context->token_value.data_type;
          context->token_to_scan = yylex();
          if (!ArrayOperandPart2(context, &imgLdInst.o_operands[0])) {
            // Note: dest: Destination. Must be a vector of four s registers.
            uint16_t kind;
            context->get_operand(imgLdInst.o_operands[0] + sizeof(uint16_t), &kind);
            if (kind != BrigEOperandRegV4) {
              context->set_error(INVALID_OPERAND);
              return 1;
            }

            if (context->token_to_scan == ',') {
              context->token_to_scan = yylex();
              // TODO(Chuang): [image]: The image. Must be a read-write or read-only image.
              if (context->token_to_scan == '[') {
                context->token_to_scan = yylex();
                if (!AddressableOperandPart2(context, &imgLdInst.o_operands[1], true)) {
                  if (context->token_to_scan == ',') {
                    context->token_to_scan = yylex();
                    // TODO(Chuang): src: Register source for the coordinates.
                    // A scalar for 1D images; a 2-element vector for 2D images;
                    // a 4-element vector for 3D images, where the fourth element is ignored.
                    // Each coordinate must be in an s register

                    if (!ArrayOperandPart2(context, &imgLdInst.o_operands[2])) {
                      if (context->token_to_scan == ';') {
                        context->append_code(&imgLdInst);
                        context->token_to_scan = yylex();
                        return 0;
                      } else {  // ';'
                        context->set_error(MISSING_SEMICOLON);
                        return 1;
                      }
                    } else {  // ArrayOperand
                      context->set_error(INVALID_OPERAND);
                      return 1;
                    }
                  } else {  // ','
                    context->set_error(MISSING_COMMA);
                    return 1;
                  }
                } else {  // Addressable Operand
                  context->set_error(INVALID_OPERAND);
                  return 1;
                }
              } else {
                context->set_error(MISSING_OPERAND);
                return 1;
              }
            } else {  // ','
              context->set_error(MISSING_COMMA);
              return 1;
            }
          } else {  // ArrayOperand
            context->set_error(INVALID_OPERAND);
            return 1;
          }
        } else {  // Data Type
          context->set_error(MISSING_DATA_TYPE);
          return 1;
        }
      } else {  // Data Type
        context->set_error(MISSING_DATA_TYPE);
        return 1;
      }
    } else {  // Geometry ID
      context->set_error(MISSING_DECLPREFIX);
      return 1;
    }
  } else {  // _V4
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }
  return 1;
}

int ImageStore(Context* context) {
  // first token is St_image
  BrigInstImage imgStInst = {
    40,                    // size
    BrigEInstImage,        // kind
    BrigStImage,           // opcode
    {0, 0, 0, 0, 0},   // o_operands[5]
    0,                 // geom
    0,                 // type
    0,                 // stype
    BrigNoPacking,         // packing
    0                      // reserved
  };
  context->token_to_scan = yylex();
  if (context->token_to_scan == _V4) {
    context->token_to_scan = yylex();
    if (context->token_type == GEOMETRY_ID) {
      switch (context->token_to_scan) {
        case _1D:
          imgStInst.geom = Briggeom_1d;
          break;
        case _2D:
          imgStInst.geom = Briggeom_2d;
          break;
        case _3D:
          imgStInst.geom = Briggeom_3d;
          break;
        case _1DB:
          imgStInst.geom = Briggeom_1db;
          break;
        case _1DA:
          imgStInst.geom = Briggeom_1da;
          break;
        case _2DA:
          imgStInst.geom = Briggeom_2da;
          break;
        default:
          context->set_error(MISSING_DECLPREFIX);
          return 1;
      }
      context->token_to_scan = yylex();
      // Note: destLength: Destination length: 32 (f32, u32, or s32)
      if (context->token_to_scan == _F32 ||
          context->token_to_scan == _U32 ||
          context->token_to_scan == _S32) {

        imgStInst.type = context->token_value.data_type;
        context->token_to_scan = yylex();
        // Note: srcLength: Source length: 32 srcType: u32.
        if (context->token_to_scan == _U32) {
          imgStInst.stype = context->token_value.data_type;
          context->token_to_scan = yylex();
          if (!ArrayOperandPart2(context, &imgStInst.o_operands[0])) {
            // Note: dest: Destination. Must be a vector of four s registers.
            uint16_t kind;
            context->get_operand(imgStInst.o_operands[0] + sizeof(uint16_t), &kind);
            if (kind != BrigEOperandRegV4) {
              context->set_error(INVALID_OPERAND);
              return 1;
            }

            if (context->token_to_scan == ',') {
              context->token_to_scan = yylex();
              // TODO(Chuang): [image]: Destination image. A read-write image.
              if (context->token_to_scan == '[') {
                context->token_to_scan = yylex();
                if (!AddressableOperandPart2(context, &imgStInst.o_operands[1], true)) {
                  if (context->token_to_scan == ',') {
                    context->token_to_scan = yylex();
                    // TODO(Chuang): src: Register source for the coordinates.
                    // A scalar for 1D images; a 2-element vector for 2D images;
                    // a 4-element vector for 3D images, where the fourth element is ignored.
                    // Each coordinate must be in an s register
                    if (!ArrayOperandPart2(context, &imgStInst.o_operands[2])) {
                      if (context->token_to_scan == ';') {
                        context->append_code(&imgStInst);
                        context->token_to_scan = yylex();
                        return 0;
                      } else {  // ';'
                        context->set_error(MISSING_SEMICOLON);
                        return 1;
                      }
                    } else {  // ArrayOperand
                      context->set_error(INVALID_OPERAND);
                      return 1;
                    }
                  } else {  // ','
                    context->set_error(MISSING_COMMA);
                    return 1;
                  }
                } else {  // Addressable Operand
                  context->set_error(INVALID_OPERAND);
                  return 1;
                }
              } else {
                context->set_error(MISSING_OPERAND);
                return 1;
              }
            } else {  // ','
              context->set_error(MISSING_COMMA);
              return 1;
            }
          } else {  // ArrayOperand
            context->set_error(INVALID_OPERAND);
            return 1;
          }
        } else {  // Data Type
          context->set_error(MISSING_DATA_TYPE);
          return 1;
        }
      } else {  // Data Type
        context->set_error(MISSING_DATA_TYPE);
        return 1;
      }
    } else {  // Geometry ID
      context->set_error(MISSING_DECLPREFIX);
      return 1;
    }
  } else {  // _V4
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }
  return 1;
}

int SingleListSingle(Context * context) {
  if (context->token_to_scan == TOKEN_SINGLE_CONSTANT) {
    uint32_t elementCount = 0;
    std::vector<float> single_list ;

    while (1) {
      elementCount ++;
      single_list.push_back(context->token_value.float_val);

      context->token_to_scan = yylex();
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        if (context->token_to_scan == TOKEN_SINGLE_CONSTANT) {
          continue;
        } else {
          context->set_error(MISSING_SINGLE_CONSTANT);
          return 1;
        }
      } else {
        uint32_t n = 0;
	// elementCount
	switch(context->get_type()){
	case Brigb1:
	  // n = elementCount;
          break;
	case Brigb8:
          n = ((elementCount&0xfff8)>>3)+((elementCount&0x07)!=0) ;
          break;
	case Brigb16:
          n = ((elementCount&0xfffc)>>2)+((elementCount&0x03)!=0) ;
          break;
	case Brigb32:
          n = ((elementCount&0xfffe)>>1)+((elementCount&0x01)!=0) ;
          break;
        case Brigb64:
          n = elementCount ;
          break;
	  }
        size_t arraySize = sizeof(BrigDirectiveInit) + (n - 1) * sizeof(uint64_t) ;
        uint8_t *array = new uint8_t[arraySize];
        BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit*>(array);
        uint32_t init_length = 0;

        bdi->size = arraySize;
        bdi->kind = BrigEDirectiveInit;
        bdi->c_code = 0 ;
        bdi->elementCount = elementCount;
        bdi->type = context->get_type();
        bdi->reserved = 0 ;

        switch(context->get_type()){
	case Brigb1:
          break;
        case Brigb8:
          for(unsigned i = 0; i < elementCount; i ++ ){ // right ?? lose value??
            memmove(&bdi->initializationData.u8[i],&single_list[i],sizeof(uint8_t));
	  }
          init_length =  n * 8;
          for (unsigned i = elementCount; i < init_length; i ++){
            bdi->initializationData.u8[i] = 0;
          }
          break;
        case Brigb16:
          for(unsigned i = 0; i < elementCount; i ++ ){// right ?? lose value??
            memmove(&bdi->initializationData.u16[i],&single_list[i],sizeof(uint16_t));
	  }
          init_length =  n * 4;
          for (unsigned i = elementCount; i < init_length; i ++){
            bdi->initializationData.u16[i] = 0;
          }
          break;
        case Brigb32:
          for(unsigned i = 0; i < elementCount; i ++ ){
            memmove(&bdi->initializationData.u32[i],&single_list[i],sizeof(uint32_t));
	  }
          init_length =  n * 2;
          for (unsigned i = elementCount; i < init_length; i ++){
            bdi->initializationData.u32[i] = 0;
          }
          break;
        case Brigb64:
          for(unsigned i = 0; i < elementCount; i ++ ){
            memmove(&bdi->initializationData.u64[i],&single_list[i],sizeof(uint64_t));
	  }
          break;
        }

        // update the BrigDirectiveSymbol.d_init and dim
        BrigDirectiveSymbol bds ;
        BrigdOffset32_t bds_offset = context->current_argdecl_offset ;
        // BrigdOffset32_t bds_offset = context->get_directive_offset() - sizeof(BrigDirectiveSymbol);
        context->get_directive(bds_offset,&bds);
        bds.d_init = context->get_directive_offset();
        if (0 == context->get_dim() && context->get_isArray())
          bds.s.symbolModifier = BrigArray;
        if (context->get_dim() < init_length)
          bds.s.dim = init_length;

        unsigned char *bds_charp = reinterpret_cast<unsigned char*>(&bds);
        context->update_directive_bytes(bds_charp,
                                        bds_offset,
                                        sizeof(BrigDirectiveSymbol));

        context->append_directive(bdi);
        delete bdi;
        return 0;
      }
    }
  } else {
    return 1;
  }
  return 0;
}

int ImageInit(Context *context) {
  BrigDirectiveImage bdi ;
  uint32_t first_token ;

  if (FORMAT == context->token_to_scan
    || ORDER == context->token_to_scan) {
    first_token = context->token_to_scan;
    context->token_to_scan = yylex();

     if ('=' == context->token_to_scan) {
       context->token_to_scan = yylex();

       if (TOKEN_PROPERTY == context->token_to_scan) {
         context->get_directive(context->current_img_offset,&bdi);
         switch(first_token){
           case FORMAT:
             bdi.format = context->token_value.format;
             break;
           case ORDER:
             bdi.order  = context->token_value.order;
             break;
         }
       } else {
         context->set_error(MISSING_PROPERTY);
         return 1;
       }
     } else {
       context->set_error(INVALID_IMAGE_INIT);
       return 1;
     }
  } else if (WIDTH == context->token_to_scan
       || HEIGHT == context->token_to_scan
       || DEPTH == context->token_to_scan) {
    first_token = context->token_to_scan ;

    context->token_to_scan = yylex();
    if ('=' == context->token_to_scan) {
      context->token_to_scan = yylex();

      if (TOKEN_INTEGER_CONSTANT == context->token_to_scan) {
        context->get_directive(context->current_img_offset,&bdi);
        switch(first_token){
          case WIDTH:
            bdi.width  = context->token_value.int_val;
            break ;
          case HEIGHT:
            bdi.height = context->token_value.int_val;
            break ;
          case DEPTH:
            bdi.depth  = context->token_value.int_val;
            break ;
        }
      } else {
        context->set_error(MISSING_PROPERTY);
        return 1;
      }
    } else {
      context->set_error(INVALID_IMAGE_INIT);
      return 1;
    }
  }
  unsigned char *bdi_charp =
      reinterpret_cast<unsigned char *>(&bdi);

  context->update_directive_bytes(bdi_charp,
                                  context->current_img_offset,
                                  sizeof(bdi));

  context->token_to_scan = yylex();
  return 0;
}

int GlobalImageDecl(Context *context) {
  if(context->token_to_scan == GLOBAL){
    context->token_to_scan = yylex();
    return (GlobalImageDeclPart2(context));
  }else{
    return 1;
  }
}

int GlobalImageDeclPart2(Context *context){
 //First token has been scanned and verified as global. Read next token.
  BrigStorageClass32_t storage_class = context->token_value.storage_class ;

  if (_RWIMG == context->token_to_scan) {
    context->token_to_scan = yylex();
    if (TOKEN_GLOBAL_IDENTIFIER == context->token_to_scan) {
      std::string var_name(context->token_value.string_val);
      int var_name_offset = context->add_symbol(var_name);

      context->token_to_scan = yylex();
      // set default value(scalar)
      context->set_dim(0);
      context->set_symbol_modifier(BrigArray);
      if ('[' == context->token_to_scan) {
        if (!ArrayDimensionSet(context)) {
        } else {
          return 1;
        }
      }
      BrigDirectiveImage bdi = {
        56,                     //size
        BrigEDirectiveImage,    //kind
        {
          context->get_code_offset(),      // c_code
          storage_class,                   // storag class
          context->get_attribute(),        // attribut
          0,                               // reserved
          context->get_symbol_modifier(),  // symbolModifier
          context->get_dim(),              // dim
          var_name_offset,                 // s_name
          BrigRWImg,                       // type
          context->get_alignment()         // align
        },
        0,                      //width
        0,                      //height
        0,                      //depth
        1,                      //array
        BrigImageOrderUnknown,  //order
        BrigImageFormatUnknown  //format
      };
      context->current_img_offset = context->get_directive_offset();
      context->append_directive(&bdi);

      if ('=' == context->token_to_scan) {
        if (!ImageInitializer(context)) {
        } else {
          context->set_error(INVALID_IMAGE_INIT);
          return 1;
        }
      }

      if (';' == context->token_to_scan) {
        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(MISSING_SEMICOLON);
      }
    } else {
    context->set_error(MISSING_IDENTIFIER);
    }
  }

  return 1;
}

int GlobalReadOnlyImageDecl(Context *context) {
  if(GLOBAL == context->token_to_scan){
    context->token_to_scan = yylex();

    return GlobalReadOnlyImageDeclPart2(context);
  }else{
    return 1;
  }
}

int GlobalReadOnlyImageDeclPart2(Context *context){
  //First token has been scanned and verified as global. Scan next token.
 BrigStorageClass32_t storage_class = context->token_value.storage_class ;

 if (_ROIMG == context->token_to_scan) {
    context->token_to_scan = yylex();
    if (TOKEN_GLOBAL_IDENTIFIER == context->token_to_scan) {
      std::string var_name(context->token_value.string_val);
      int var_name_offset = context->add_symbol(var_name);

      context->token_to_scan = yylex();
      // set default value(scalar)
      context->set_dim(0);
      context->set_symbol_modifier(BrigArray);
      if ('[' == context->token_to_scan) {
        if (!ArrayDimensionSet(context)) {
        } else {
          return 1;
        }
      }
      BrigDirectiveImage bdi = {
        56,                     //size
        BrigEDirectiveImage,    //kind
        {
          context->get_code_offset(),      // c_code
          storage_class,                   // storag class
          context->get_attribute(),        // attribut
          0,                               // reserved
          context->get_symbol_modifier(),  // symbolModifier
          context->get_dim(),              // dim
          var_name_offset,                 // s_name
          BrigROImg,                       // type
          context->get_alignment()         // align
        },
        0,                      //width
        0,                      //height
        0,                      //depth
        1,                      //array
        BrigImageOrderUnknown,  //order
        BrigImageFormatUnknown  //format
      };
      context->current_img_offset = context->get_directive_offset();
      context->append_directive(&bdi);

      if ('=' == context->token_to_scan) {
        if (!ImageInitializer(context)) {
        } else {
          context->set_error(INVALID_IMAGE_INIT);
          return 1;
        }
      }

      if (';' == context->token_to_scan) {
        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(MISSING_SEMICOLON);
      }
    } else {
      context->set_error(INVALID_INITIALIZER);
    }
  }

  return 1;
}

int ImageInitializer(Context *context) {
  // first must be '='
  if('=' != context->token_to_scan)
    return 1;

  context->token_to_scan = yylex();
  if ('{' == context->token_to_scan) {
    while (1) {
      context->token_to_scan = yylex();
      if (!ImageInit(context)) {
        if (',' == context->token_to_scan) {
          continue;
        } else {
          break;
        }
      } else {
        context->set_error(MISSING_IDENTIFIER);
        return 1;
      }
    }  // end for while

    if ('}' == context->token_to_scan) {
      context->token_to_scan = yylex();
      return 0;
    } else {
      context->set_error(MISSING_CLOSING_BRACKET);
    }
  } else {
    context->set_error(MISSING_OPENNING_BRACKET);
  }
  return 1;
}

int Ret(Context* context) {
  // first token is RET
  if(RET != context->token_to_scan)
    return 1;

  BrigOpcode32_t opcode = context->token_value.opcode ;

  context->token_to_scan = yylex();
  if (context->token_to_scan == ';') {
     BrigInstBase op_ret = {
      32,
      BrigEInstBase,
      opcode,
      Brigb32,
      BrigNoPacking,
      {0, 0, 0, 0, 0}
    };
    // write to .code section
    context->append_code(&op_ret);
    context->update_bdf_operation_count();

    context->token_to_scan = yylex();
    return 0;
  } else {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }

}

int ImageRead(Context *context) {
  // first token is RD_IMAGE
  BrigInstRead imgRdInst = {
    40,                    // size
    BrigEInstRead,         // kind
    BrigRdImage,           // opcode
    {0, 0, 0, 0, 0},       // o_operands[5]
    0,                     // geom
    0,                     // stype
    0,                     // type
    BrigNoPacking,         // packing
    0                      // reserved
  };
  context->token_to_scan = yylex();
  if (context->token_to_scan == _V4) {
    context->token_to_scan = yylex();
    // Note: 1db is not supported
    if (context->token_type == GEOMETRY_ID) {
      switch (context->token_to_scan) {
        case _1D:
          imgRdInst.geom = Briggeom_1d;
          break;
        case _2D:
          imgRdInst.geom = Briggeom_2d;
          break;
        case _3D:
          imgRdInst.geom = Briggeom_3d;
          break;
        case _1DA:
          imgRdInst.geom = Briggeom_1da;
          break;
        case _2DA:
          imgRdInst.geom = Briggeom_2da;
          break;
        default:
          context->set_error(MISSING_DECLPREFIX);
          return 1;
      }

      context->token_to_scan = yylex();
      // Note: destLength: Destination length: 32 (f32, u32, or s32)
      if (context->token_to_scan == _F32 ||
          context->token_to_scan == _U32 ||
          context->token_to_scan == _S32) {

        imgRdInst.type = context->token_value.data_type;
        context->token_to_scan = yylex();
        // Note: srcLength: Source length: 32 srcType: f32 or u32.
        if (context->token_to_scan == _F32 ||
            context->token_to_scan == _U32) {

          imgRdInst.stype = context->token_value.data_type;
          context->token_to_scan = yylex();

          if (!ArrayOperandPart2(context, &imgRdInst.o_operands[0])) {
            // Note: dest: Destination. Must be a vector of four s registers.
            uint16_t kind;
            context->get_operand(imgRdInst.o_operands[0] + sizeof(uint16_t), &kind);
            if (kind != BrigEOperandRegV4) {
              context->set_error(INVALID_OPERAND);
              return 1;
            }
            if (context->token_to_scan == ',') {
              context->token_to_scan = yylex();
              // TODO(Chuang): Whether the sampler("[Samp]") can be omitted.
              // rd_image used with integer coordinates has restrictions on the sampler:
              // coord must be unnormalized.
              // filter must be nearest.
              // The boundary mode must be clamp or border.

              if (context->token_to_scan == '[') {
                context->token_to_scan = yylex();
                if (!AddressableOperandPart2(context, &imgRdInst.o_operands[1], true)) {
                  if (context->token_to_scan == ',') {
                    unsigned int opCount = 2;
                    context->token_to_scan = yylex();
                    if (context->token_to_scan == '[') {
                      context->token_to_scan = yylex();
                      if (!AddressableOperandPart2(context, &imgRdInst.o_operands[opCount++], true)) {
                        if (context->token_to_scan == ',') {
                          context->token_to_scan = yylex();
                        } else {  // ','
                          context->set_error(MISSING_COMMA);
                          return 1;
                        }
                      } else {  // Addressable Operand
                        context->set_error(INVALID_OPERAND);
                        return 1;
                      }
                    } else {  // '['
                      context->set_error(MISSING_OPERAND);
                      return 1;
                    }
                    // TODO(Chuang): src: Register source for the coordinates.
                    // A scalar for 1D images; a 2-element vector for 2D images;
                    // a 4-element vector for 3D images, where the fourth element is ignored.
                    // Each coordinate must be in an s register

                    if (!ArrayOperandPart2(context, &imgRdInst.o_operands[opCount])) {

                      if (context->token_to_scan == ';') {
                        context->append_code(&imgRdInst);
                        context->token_to_scan = yylex();
                        return 0;
                      } else {  // ';'
                        context->set_error(MISSING_SEMICOLON);
                        return 1;
                      }
                    } else {  // Array Operand
                      context->set_error(INVALID_OPERAND);
                      return 1;
                    }
                  } else {  // ','
                    context->set_error(MISSING_COMMA);
                    return 1;
                  }
                } else {  // Addressable Operand
                  context->set_error(INVALID_OPERAND);
                  return 1;
                }
              } else {  // '['
                context->set_error(MISSING_OPERAND);
                return 1;
              }
            } else {  // ','
              context->set_error(MISSING_COMMA);
              return 1;
            }
          } else {  // Array Operand
            context->set_error(MISSING_OPERAND);
            return 1;
          }
        } else {  // Data Type
          context->set_error(MISSING_DATA_TYPE);
          return 1;
        }
      } else {  // Data Type
        context->set_error(MISSING_DATA_TYPE);
        return 1;
      }
    } else {  // Geometry ID
      context->set_error(MISSING_DECLPREFIX);
      return 1;
    }
  } else {  // _V4
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }
  context->set_error(UNKNOWN_ERROR);
  return 1;
}

int Sync(Context* context) {
  // first token is SYNC
  if(SYNC != context->token_to_scan)
    return 1;

  BrigOpcode32_t opcode = context->token_value.opcode;
  uint32_t syncFlags = BrigPartialLevel; //default

  context->token_to_scan = yylex();
  if (context->token_to_scan == _GLOBAL) {
    syncFlags = BrigGlobalLevel;
    context->token_to_scan = yylex();
  } else if (context->token_to_scan == _GROUP) {
    syncFlags = BrigGroupLevel;
    context->token_to_scan = yylex();
  }
  if (context->token_to_scan == ';') {
    BrigInstBar op_sync = {
      36,
      BrigEInstBar,
      opcode ,
      Brigb32 ,
      BrigNoPacking,
      {0,0,0,0,0},
      syncFlags
    };
    context->append_code(&op_sync);
    context->update_bdf_operation_count();

    context->token_to_scan = yylex();
    return 0;
  } else {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }

}
int Bar(Context* context) {
  // first token is BARRIER
  if(BARRIER != context->token_to_scan)
    return 1;

  BrigOpcode32_t opcode = context->token_value.opcode;
  uint32_t syncFlags = BrigPartialLevel; //default

  context->token_to_scan = yylex();
  BrigoOffset32_t offset = context->get_operand_offset();

  if (OptionalWidth(context)) { // no width
    offset = 0;
  }
    if (context->token_to_scan == _GLOBAL) {
      syncFlags = BrigGlobalLevel;
      context->token_to_scan = yylex();
    } else if (context->token_to_scan == _GROUP) {
      syncFlags = BrigGroupLevel;
      context->token_to_scan = yylex();
    }
    if (context->token_to_scan == ';') {
      BrigInstBar op_bar = {
        36,
        BrigEInstBar,
        opcode ,
        Brigb32 ,
        BrigNoPacking,
        {offset,0,0,0,0},
        syncFlags
      };
      context->append_code(&op_bar);
      context->update_bdf_operation_count();

      context->token_to_scan = yylex();
      return 0;
    } else {
      context->set_error(MISSING_SEMICOLON);
      return 1;
    }
}



int AtomModifiers(Context* context) {
  BrigMemorySemantic32_t memorySemantic;
  BrigStorageClass32_t storageClass;
  return AtomModifiersPart2(context, &storageClass, &memorySemantic );
}

int AtomModifiersPart2(Context* context, BrigStorageClass32_t* pStorageClass,
                       BrigMemorySemantic32_t* pMemorySemantic) {
  while (1) {
    if (context->token_type == ADDRESS_SPACE_IDENTIFIER) {
      switch (context->token_to_scan) {
        case _GLOBAL:
          *pStorageClass = BrigGlobalSpace;
          break;
        case _GROUP:
          *pStorageClass = BrigGroupSpace;
          break;
        case _PRIVATE:
          *pStorageClass = BrigPrivateSpace;
          break;
        case _KERNARG:
          *pStorageClass = BrigKernargSpace;
          break;
        case _READONLY:
          *pStorageClass = BrigReadonlySpace;
          break;
        case _SPILL:
          *pStorageClass = BrigSpillSpace;
          break;
        case _ARG:
          *pStorageClass = BrigArgSpace;
          break;
        default:
          *pStorageClass = BrigFlatSpace;
      }
      context->token_to_scan = yylex();
      continue;
    }

    if (context->token_to_scan == _AR) {
      *pMemorySemantic = BrigAcquireRelease;
      context->token_to_scan = yylex();
      continue;
    }

    if (context->token_to_scan == _REL) {
      *pMemorySemantic = BrigRelease;
      context->token_to_scan = yylex();
      continue;
    }
    if (context->token_to_scan == _ACQ) {
      *pMemorySemantic = BrigAcquire;
      context->token_to_scan = yylex();
      continue;
    }

    if (context->token_to_scan == _REGION) {
      // TODO(Chuang) need to check "_region" out again.
      *pMemorySemantic = BrigRegular;
      context->token_to_scan = yylex();
      continue;
    }
    return 0;
  }
  context->set_error(UNKNOWN_ERROR);
  return 1;
}

int AtomicNoRet(Context* context) {
  // first token is ATOMICNORET or ATOMICNORET_CAS

  BrigInstAtomic aNoRetInst = {
    44,                    // size
    BrigEInstAtomic,       // kind
    BrigAtomicNoRet,       // opcode
    0,                     // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0},       // o_operands[5]
    BrigAtomicCas,         // atomicOperation
    BrigGlobalSpace,       // storageClass
    0                      // memorySemantic
  };

  const unsigned int first_token = context->token_to_scan;
  context->token_to_scan = yylex();
  if (first_token == ATOMICNORET) {
    if (context->token_type == ATOMIC_OP) {
      switch (context->token_to_scan) {  // without _CAS_
        case _AND_:
          aNoRetInst.atomicOperation = BrigAtomicAnd;
          break;
        case _OR_:
          aNoRetInst.atomicOperation = BrigAtomicOr;
          break;
        case _XOR_:
          aNoRetInst.atomicOperation = BrigAtomicXor;
          break;
        case _EXCH_:
          aNoRetInst.atomicOperation = BrigAtomicExch;
          break;
        case _ADD_:
          aNoRetInst.atomicOperation = BrigAtomicAdd;
          break;
        case _INC_:
          aNoRetInst.atomicOperation = BrigAtomicInc;
          break;
        case _DEC_:
          aNoRetInst.atomicOperation = BrigAtomicDec;
          break;
        case _MIN_:
          aNoRetInst.atomicOperation = BrigAtomicMin;
          break;
        case _MAX_:
          aNoRetInst.atomicOperation = BrigAtomicMax;
          break;
        case _SUB_:
          aNoRetInst.atomicOperation = BrigAtomicSub;
          break;
        default:
          context->set_error(MISSING_DECLPREFIX);
          return 1;
      }
      context->token_to_scan = yylex();
    } else {
      context->set_error(MISSING_OPERAND);
      return 1;
    }
  }

  if (!AtomModifiersPart2(context, &aNoRetInst.storageClass, &aNoRetInst.memorySemantic)) {

    // Type: b, u, s, f (f is supported only for max and min).
    // Length: 32, 64.
    if ((context->token_to_scan == _U32 ||
         context->token_to_scan == _S32 ||
         context->token_to_scan == _B32 ||
         context->token_to_scan == _U64 ||
         context->token_to_scan == _S64 ||
         context->token_to_scan == _B64) ||
        ((aNoRetInst.atomicOperation == BrigAtomicMax ||
         aNoRetInst.atomicOperation == BrigAtomicMin) &&
         (context->token_to_scan == _F32 ||
          context->token_to_scan == _F64))) {

      aNoRetInst.type = context->token_value.data_type;
      context->token_to_scan = yylex();
      aNoRetInst.o_operands[0] = context->get_operand_offset();
      if (!MemoryOperand(context)) {
        unsigned int opCount = 1;
        BrigoOffset32_t opSize = 0;
        std::string opName;

        if (first_token == ATOMICNORET_CAS) {
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            opSize = context->get_operand_offset();
            if (context->valid_string) {
              opName = context->token_value.string_val;
            } else {
              if (context->token_type == CONSTANT) {
                opSize += opSize & 0x7;
              }
            }
            if (!Operand(context)) {
              if (opSize == context->get_operand_offset()) {
                aNoRetInst.o_operands[opCount++] = context->operand_map[opName];
              } else {
                aNoRetInst.o_operands[opCount++] = opSize;
              }
            } else {
              context->set_error(MISSING_OPERAND);
              return 1;
            }
          } else {  // ','
            context->set_error(MISSING_COMMA);
            return 1;
          }
        }
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
          opSize = context->get_operand_offset();
          if (context->valid_string) {
            opName = context->token_value.string_val;
          } else {
            if (context->token_type == CONSTANT) {
              opSize += opSize & 0x7;
            }
          }
          if (!Operand(context)) {
            if (opSize == context->get_operand_offset()) {
              aNoRetInst.o_operands[opCount++] = context->operand_map[opName];
            } else {
              aNoRetInst.o_operands[opCount++] = opSize;
            }
            if (context->token_to_scan == ';') {
              context->append_code(&aNoRetInst);
              context->token_to_scan = yylex();
              return 0;
            } else {
              context->set_error(MISSING_SEMICOLON);
              return 1;
            }
          } else {  // Operand
            context->set_error(MISSING_OPERAND);
            return 1;
          }
        } else {  // ','
          context->set_error(MISSING_COMMA);
          return 1;
        }
      } else {
        context->set_error(MISSING_OPERAND);
        return 1;
      }
    } else {  // Data Type
      context->set_error(MISSING_DATA_TYPE);
      return 1;
    }
  } else {  // Atom Modifiers
    context->set_error(MISSING_IDENTIFIER);
    return 1;
  }
  context->set_error(UNKNOWN_ERROR);
  return 1;
}

int Location(Context* context) {
  // first token is LOC
  if (LOC != context->token_to_scan)
    return 1;

  context->token_to_scan = yylex();
  if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
    uint32_t sourceFile = context->token_value.int_val;
    context->token_to_scan = yylex();
    if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
      uint32_t sourceLine = context->token_value.int_val;
      context->token_to_scan = yylex();
      if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
        uint32_t sourceColumn = context->token_value.int_val;
        context->token_to_scan = yylex();
        if (context->token_to_scan == ';') {

          BrigDirectiveLoc bdl = {
            sizeof(BrigDirectiveLoc),   //size
            BrigEDirectiveLoc,          //kind
            context->get_code_offset(), //c_code
            sourceFile,                 //sourceFile
            sourceLine,                 //sourceLine
            sourceColumn                //sourceColumn
          };
          context->append_directive(&bdl);

          context->token_to_scan = yylex();
          return 0;
        } else {  // ';'
          context->set_error(MISSING_SEMICOLON);
          return 1;
        }
      } else {  // Integer Constant
        context->set_error(MISSING_INTEGER_CONSTANT);
        return 1;
      }
    } else {  // Integer Constant
      context->set_error(MISSING_INTEGER_CONSTANT);
      return 1;
    }
  } else {  // Integer Constant
    context->set_error(MISSING_INTEGER_CONSTANT);
    return 1;
  }
  return 1;
}
int Control(Context* context) {
  if (context->token_to_scan == MEMOPT_ON) {
  } else if (context->token_to_scan == MEMOPT_OFF) {
  } else if (context->token_to_scan == WORKGROUPS_PER_CU) {
    context->token_to_scan = yylex();
    if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
    } else {  // Integer Constant
      context->set_error(MISSING_INTEGER_CONSTANT);
      return 1;
    }
  } else if (context->token_to_scan == ITEMS_PER_WORKGROUP) {
    context->token_to_scan = yylex();
    if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
      context->token_to_scan = yylex();
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
          context->token_to_scan = yylex();
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
            } else {  // Integer Constant
              context->set_error(MISSING_INTEGER_CONSTANT);
              return 1;
            }
          } else {  // ','
             context->set_error(MISSING_COMMA);
             return 1;
          }
        } else {  // Integer Constant
          context->set_error(MISSING_INTEGER_CONSTANT);
          return 1;
        }
      } else {  // ','
        context->set_error(MISSING_COMMA);
        return 1;
      }
    } else {  // Integer Constant
      context->set_error(MISSING_INTEGER_CONSTANT);
      return 1;
    }
  } else {
    context->set_error(UNKNOWN_ERROR);
    return 1;
  }
  context->token_to_scan = yylex();
  if (context->token_to_scan == ';') {
    context->token_to_scan = yylex();
    return 0;
  }
  context->set_error(UNKNOWN_ERROR);
  return 1;
}

int Pragma(Context* context) {
  // first token is PRAGMA
  context->token_to_scan = yylex();
  if (context->token_to_scan == TOKEN_STRING) {
    context->token_to_scan = yylex();
    if (context->token_to_scan == ';') {
      context->token_to_scan = yylex();
      return 0;
    } else {  // ';'
      context->set_error(MISSING_SEMICOLON);
      return 1;
    }
  } else {  // String
    context->set_error(MISSING_STRING);
    return 1;
  }
  context->set_error(UNKNOWN_ERROR);
  return 1;
}

// the type must be u or s
// size must 32 or 64
int LabelList(Context* context) {
  uint32_t elementCount = 0;
  std::vector<BrigDirectiveLabel> label_list;

  while (context->token_to_scan == TOKEN_LABEL) {
    std::string label_name = context->token_value.string_val;
    BrigsOffset32_t label_name_offset = context->add_symbol(label_name);

    BrigDirectiveLabel bdl = {
      sizeof(BrigDirectiveLabel),  //size
      BrigEDirectiveLabel,         //kind
      0,                           //c_code
      label_name_offset            //s_name
    };

    label_list.push_back(bdl);
    elementCount ++;
    context->token_to_scan = yylex();
    if (context->token_to_scan == ',') {
      context->token_to_scan = yylex();
      continue;
    } else {
      // create
      size_t arraySize = sizeof(BrigDirectiveLabelInit) +
                         (elementCount - 1) * sizeof(BrigdOffset32_t);
      uint8_t *array = new uint8_t[arraySize];
      BrigDirectiveLabelInit *bdli = reinterpret_cast<BrigDirectiveLabelInit*>(array);

      // update the BrigDirectiveSymbol.d_init and dim
      BrigDirectiveSymbol bds ;
      BrigdOffset32_t bds_offset = context->current_argdecl_offset ;
      context->get_directive(bds_offset,&bds);
      bds.d_init = context->get_directive_offset();
      bds.s.dim = elementCount;
      if (0 == context->get_dim() && context->get_isArray())
        bds.s.symbolModifier = BrigArray;

      unsigned char *bds_charp = reinterpret_cast<unsigned char*>(&bds);
      context->update_directive_bytes(bds_charp,
                                      bds_offset,
                                      sizeof(BrigDirectiveSymbol));


      // fill the data of BrigDirectiveLabelInit
      bdli->size = arraySize;
      bdli->kind = BrigEDirectiveLabelInit;
      bdli->c_code = 0;
      bdli->elementCount = elementCount;
      context->append_directive(bdli);

      for (unsigned i = 0 ; i < elementCount ; i ++){
        bdli->d_labels[i] = context->get_directive_offset();
        // put into the symbo_map,not need to check,
        // will be first time to put ?
        std::string label_name = context->get_string(label_list[i].s_name);
        context->symbol_map[label_name] = context->get_directive_offset();

        context->append_directive(&label_list[i]);
      }

      unsigned char *bdli_charp = reinterpret_cast<unsigned char*>(bdli);
      context->update_directive_bytes(bdli_charp,
                                      bds.d_init,
                                      arraySize);
      delete bdli;

      return 0;
    }
  }  // While

  return 1;
}
int FloatListSingle(Context* context) {
  uint32_t elementCount = 0;
  std::vector<double> float_list ;

  while (context->token_to_scan == TOKEN_DOUBLE_CONSTANT) {
    elementCount ++ ;
    float_list.push_back(context->token_value.double_val);

    context->token_to_scan = yylex();
    if (context->token_to_scan == ',') {
      context->token_to_scan = yylex();
      continue;
    } else {
    // Note: the token has been updated
     uint32_t n = 0;
    // elementCount
    switch(context->get_type()){
      case Brigb1:
	// n = elementCount;
        break;
      case Brigb8:
        n = ((elementCount&0xfff8)>>3)+((elementCount&0x07)!=0) ;
        break;
      case Brigb16:
        n = ((elementCount&0xfffc)>>2)+((elementCount&0x03)!=0) ;
        break;
      case Brigb32:
        n = ((elementCount&0xfffe)>>1)+((elementCount&0x01)!=0) ;
        break;
      case Brigb64:
        n = elementCount ;
        break;
	}
      size_t arraySize = sizeof(BrigDirectiveInit) + (n - 1) * sizeof(uint64_t) ;
      uint8_t *array = new uint8_t[arraySize];
      BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit*>(array);
      uint32_t init_length = 0;

      bdi->size = arraySize;
      bdi->kind = BrigEDirectiveInit;
      bdi->c_code = 0 ;
      bdi->elementCount = elementCount;
      bdi->type = context->get_type();
      bdi->reserved = 0;

      switch(context->get_type()){
      case Brigb1:
        break;
      case Brigb8:
        for (unsigned i = 0; i < elementCount; i ++ ){// right ?? lose value??
          memmove(&bdi->initializationData.u8[i], &float_list[i],sizeof(uint8_t));
	}
        init_length = 8 * n ;
        for (unsigned i = elementCount; i < init_length; i ++){
          bdi->initializationData.u8[i] = 0;
        }
        break;
      case Brigb16:
        for (unsigned i = 0; i < elementCount; i ++ ){ // right ?? lose value??
          memmove(&bdi->initializationData.u16[i], &float_list[i],sizeof(uint16_t));
	}
        init_length = 4 * n ;
        for (unsigned i = elementCount; i < init_length; i ++){
          bdi->initializationData.u16[i] = 0;
        }
        break;
      case Brigb32:
        for (unsigned i = 0; i < elementCount; i ++ ){// right ?? lose value??
          memmove(&bdi->initializationData.u32[i], &float_list[i],sizeof(uint32_t));
	}
        init_length = 2 * n ;
        for (unsigned i = elementCount; i < init_length; i ++){
          bdi->initializationData.u32[i] = 0;
        }
        break;
      case Brigb64:
        for (unsigned i = 0; i < elementCount; i ++ ){
          memmove(&bdi->initializationData.u64[i], &float_list[i],sizeof(uint64_t));
	}
        init_length = n;
        break;
      }

      // update the BrigDirectiveSymbol.d_init and dim
      BrigDirectiveSymbol bds ;
      BrigdOffset32_t bds_offset = context->current_argdecl_offset ;
      // BrigdOffset32_t bds_offset = context->get_directive_offset() - sizeof(BrigDirectiveSymbol);
      context->get_directive(bds_offset,&bds);
      bds.d_init = context->get_directive_offset();
      if (0 == context->get_dim() && context->get_isArray())
        bds.s.symbolModifier = BrigArray;
      if (context->get_dim() < init_length)
        bds.s.dim = init_length;

      unsigned char *bds_charp = reinterpret_cast<unsigned char*>(&bds);
      context->update_directive_bytes(bds_charp,
                                      bds_offset,
                                      sizeof(BrigDirectiveSymbol));

      context->append_directive(bdi);

      delete bdi;
      return 0;
    }
  }  // While
  context->set_error(MISSING_DOUBLE_CONSTANT);
  return 1;
}

int DecimalListSingle(Context* context) {
  uint32_t elementCount = 0;
  std::vector<int32_t> decimal_list ;

  while (1) {
    if (context->token_to_scan == '-') {
      context->token_to_scan = yylex();
      if (context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
        context->set_error(MISSING_INTEGER_CONSTANT);
        return 1;
      } else {
        context->token_value.int_val *= (-1);
      }
    }
    if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
      elementCount ++;
      decimal_list.push_back(context->token_value.int_val);

      context->token_to_scan = yylex();
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        continue;
      } else {
        // Note: the token has been updated
        uint32_t n = 0;
	// elementCount
	switch(context->get_type()){
	case Brigb1:
	  // n = elementCount;
          break;
	case Brigb8:
          n = ((elementCount&0xfff8)>>3)+((elementCount&0x07)!=0) ;
          break;
	case Brigb16:
          n = ((elementCount&0xfffc)>>2)+((elementCount&0x03)!=0) ;
          break;
	case Brigb32:
          n = ((elementCount&0xfffe)>>1)+((elementCount&0x01)!=0) ;
          break;
        case Brigb64:
          n = elementCount ;
          break;
	  }
        size_t arraySize = sizeof(BrigDirectiveInit) + (n - 1) * sizeof(uint64_t) ;
        uint8_t *array = new uint8_t[arraySize];
        BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit*>(array);
        uint32_t init_length = 0;

        bdi->size = arraySize;
        bdi->kind = BrigEDirectiveInit;
        bdi->c_code = 0 ;
        bdi->elementCount = elementCount;
        bdi->type = context->get_type();
        bdi->reserved = 0;

        switch(context->get_type()){
	case Brigb1:
          break;
        case Brigb8:
          for (unsigned i = 0; i < elementCount; i ++ ){
            bdi->initializationData.u8[i] = decimal_list[i];
	  }
          init_length = 8 * n ;
          for (unsigned i = elementCount; i < init_length; i ++){
            bdi->initializationData.u8[i] = 0;
          }
          break;
        case Brigb16:
          for (unsigned i = 0; i < elementCount; i ++ ){
            bdi->initializationData.u16[i] = decimal_list[i];
	  }
          init_length = 4 * n ;
          for (unsigned i = elementCount; i < init_length; i ++){
            bdi->initializationData.u8[i] = 0;
          }
          break;
        case Brigb32:
          for (unsigned i = 0; i < elementCount; i ++ ){
            bdi->initializationData.u32[i] = decimal_list[i];
	  }
          init_length = 2 * n ;
          for (unsigned i = elementCount; i < init_length; i ++){
            bdi->initializationData.u8[i] = 0;
          }
          break;
        case Brigb64:
          for (unsigned i = 0; i < elementCount; i ++ ){
            bdi->initializationData.u64[i] = decimal_list[i];
	  }
          init_length =  n ;
          for (unsigned i = elementCount; i < init_length; i ++){
            bdi->initializationData.u8[i] = 0;
          }
          break;
        }

        // update the BrigDirectiveSymbol.d_init and dim
        BrigDirectiveSymbol bds ;
        BrigdOffset32_t bds_offset = context->current_argdecl_offset ;
        context->get_directive(bds_offset,&bds);
        bds.d_init = context->get_directive_offset();
        if (0 == context->get_dim() && context->get_isArray()){
          bds.s.symbolModifier = BrigArray;
          context->set_symbol_modifier(BrigArray);
        }
        if (context->get_dim() < init_length){
          bds.s.dim = init_length;
          context->set_dim(init_length);
        }

        unsigned char *bds_charp = reinterpret_cast<unsigned char*>(&bds);
        context->update_directive_bytes(bds_charp,
                                        bds_offset,
                                        sizeof(BrigDirectiveSymbol));

        context->append_directive(bdi);

        delete bdi;
        return 0;
      }  // ','
    }  // integer constant
  }  // While
  return 1;
}


int Block(Context* context) {
  // first token is BLOCK
  context->token_to_scan = yylex();
  if (context->token_to_scan == TOKEN_STRING) {
    context->token_to_scan = yylex();
    if (context->token_to_scan == ENDBLOCK) {
      context->set_error(MISSING_SECTION_ITEM);
      return 1;
    }
    while (context->token_to_scan != ENDBLOCK) {
      if (context->token_to_scan == BLOCKSTRING) {
        context->token_to_scan = yylex();
        if (context->token_to_scan == TOKEN_STRING) {
          context->token_to_scan = yylex();
        } else {  // String
          context->set_error(INVALID_SECTION_ITEM);
          return 1;
        }
      } else if (context->token_to_scan == BLOCKNUMERIC) {
        context->token_to_scan = yylex();
        if (context->token_type == DATA_TYPE_ID) {
          context->token_to_scan = yylex();
          if (context->token_to_scan == TOKEN_LABEL) {
            context->token_to_scan = yylex();
          } else if (!DecimalListSingle(context)) {
          } else if (!FloatListSingle(context)) {
          } else {
            context->set_error(INVALID_SECTION_ITEM);
            return 1;
          }
        } else {  // Data Type
          context->set_error(MISSING_DATA_TYPE);
          return 1;
        }
      } else {  // Block Numeric or Block String
        context->set_error(MISSING_BLOCK_TYPE);
        return 1;
      }
      if (context->token_to_scan == ';') {
        context->token_to_scan = yylex();
        continue;
      } else {  // ';'
        context->set_error(MISSING_SEMICOLON);
        return 1;
      }
    }  // While
    context->token_to_scan = yylex();
    if (context->token_to_scan == ';') {
      context->token_to_scan = yylex();
      return 0;
    } else {  // ';'
      context->set_error(MISSING_SEMICOLON);
      return 1;
    }
  } else {  // String
    context->set_error(MISSING_STRING);
    return 1;
  }
}

int GlobalSymbolDeclpart2(Context* context) {
  if (context->token_to_scan == GROUP)
    return (GlobalGroupDecl(context));
  else if (context->token_to_scan == PRIVATE)
    return (GlobalPrivateDecl(context));

  return 1;
}

int GlobalSymbolDecl(Context* context) {
  if (!DeclPrefix(context)) {
    return GlobalSymbolDeclpart2(context);
  }
  return 1;
}

int Directive(Context* context) {
  switch (context->token_to_scan) {
    case PRAGMA:
      if (!Pragma(context)) {
        return 0;
      }
      return 1;
    case EXTENSION:
      if (!Extension(context)) {
        return 0;
      }
      return 1;
    case BLOCK:
      if (!Block(context)) {
        return 0;
      }
      return 1;
    case ITEMS_PER_WORKGROUP:
    case WORKGROUPS_PER_CU:
    case MEMOPT_OFF:
    case MEMOPT_ON:
      if (!Control(context)) {
        return 0;
      }
      return 1;
    case _FILE:
      if (!FileDecl(context)) {
        return 0;
      }
      return 1;
    default:
      return 1;
  }
}

int SobInit(Context *context){
 unsigned int first_token = context->token_to_scan ;

 if(COORD == context->token_to_scan
    ||FILTER == context->token_to_scan
    ||BOUNDARYU == context->token_to_scan
    ||BOUNDARYV == context->token_to_scan
    ||BOUNDARYW == context->token_to_scan){
  BrigDirectiveSampler bds ;
  context->get_directive(context->current_samp_offset,&bds);
  bds.valid = 1;

    context->token_to_scan = yylex();
    if('=' == context->token_to_scan){
      context->token_to_scan = yylex();
      if(TOKEN_PROPERTY == context->token_to_scan){
        switch(first_token){
	  case COORD:
            bds.normalized = context->token_value.normalized;
            break ;
          case FILTER:
            bds.filter = context->token_value.filter;
            break ;
	  case BOUNDARYU:
            bds.boundaryU = context->token_value.boundary_mode;
            break ;
	  case BOUNDARYV:
            bds.boundaryV = context->token_value.boundary_mode;
            break ;
	  case BOUNDARYW:
            bds.boundaryW = context->token_value.boundary_mode;
            break ;
	}
        unsigned char *bds_charp =
              reinterpret_cast<unsigned char*> (&bds);
        context->update_directive_bytes(bds_charp,
                                        context->current_samp_offset,
                                        sizeof(bds));

        context->token_to_scan = yylex();
        return 0;
      }else{
	context->set_error(MISSING_PROPERTY);
      }
    }else{ //for '='
      context->set_error(MISSING_IDENTIFIER);
    }
  }

  return 1;
}

int SobInitializer(Context *context){
  //first must be '='
  if('=' != context->token_to_scan){
     return 1;
  }

  context->token_to_scan = yylex();
  if('{' == context->token_to_scan){
    while(1){
      context->token_to_scan = yylex();
      if(!SobInit(context)){
        if(',' == context->token_to_scan){
          continue ;
        }else {
          break ;
        }
      }else {
        context->set_error(MISSING_IDENTIFIER);
        return 1;
      }
    }//end for while

    if('}' == context->token_to_scan){
      context->token_to_scan = yylex();
      return 0 ;
    }else{
      context->set_error(MISSING_CLOSING_BRACKET);
    }
  }else{
     context->set_error(MISSING_OPENNING_BRACKET);
  }
  return 1;
}

int GlobalSamplerDecl(Context *context){
  if(GLOBAL == context->token_to_scan){
    context->token_to_scan = yylex();
    return (GlobalSamplerDeclPart2(context));
  }else{
    return 1;
  }
}

int GlobalSamplerDeclPart2(Context *context){
  // First token has already been verified as GLOBAL
  BrigStorageClass32_t storage_class = context->token_value.storage_class ;

  if (_SAMP == context->token_to_scan){
    context->token_to_scan = yylex();
    if (TOKEN_GLOBAL_IDENTIFIER == context->token_to_scan){
      std::string var_name(context->token_value.string_val);
      int var_name_offset = context->add_symbol(var_name);

      context->token_to_scan = yylex();

      // set default value(scalar)
      context->set_dim(0);
      context->set_symbol_modifier(BrigArray);
      if ('[' == context->token_to_scan) {
        if (!ArrayDimensionSet(context)) {
        } else {
          return 1;
        }
      }

      BrigDirectiveSampler bds = {
        40,                                //size
        BrigEDirectiveSampler,             //kind
        {
          context->get_code_offset(),      // c_code
          storage_class,                   // storag class
          context->get_attribute(),        // attribut
          0,                               // reserved
          context->get_symbol_modifier(),  // symbolModifier
          context->get_dim(),              // dim
          var_name_offset,                 // s_name
          BrigSamp,                        // type
          context->get_alignment()         // align
        },
        0,                      //valid
        0,                      //normalized
        0,                      //filter
        0,                      //boundaryU
        0,                      //boundaryV
        0,                      //boundaryW
        0                       //reserved1
      };
      context->current_samp_offset = context->get_directive_offset();
      context->append_directive(&bds);

      if ('=' == context->token_to_scan) {
        if (!SobInitializer(context)) {
        } else {
          context->set_error(INVALID_IMAGE_INIT);
          return 1;
        }
      }

      if (';' == context->token_to_scan) {
        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(MISSING_SEMICOLON);
      }
    }
  } else {
    context->set_error(MISSING_IDENTIFIER);
  }
  return 1;
}

int GlobalInitializablePart2(Context* context){

  if (GLOBAL == context->token_to_scan){
    BrigStorageClass32_t storage_class = context->token_value.storage_class;
    context->token_to_scan = yylex();
    switch(context->token_to_scan){
      case _RWIMG:
        return(GlobalImageDeclPart2(context));
      case _ROIMG:
        return(GlobalReadOnlyImageDeclPart2(context));
      case _SAMP :
        return(GlobalSamplerDeclPart2(context));
      default:
        if (context->token_type==DATA_TYPE_ID)
          return (InitializableDeclPart2(context, storage_class));
        else {
          context->set_error(MISSING_IDENTIFIER);
          return 1;
        }
    }
  } else if (READONLY == context->token_to_scan){
    return InitializableDecl(context);
  } else {
    return 1;
  }
  return 0;
}

int GlobalInitializable(Context* context){
  if (!DeclPrefix(context)){
    return GlobalInitializablePart2(context);
  } else{
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }
}

int GlobalDeclpart2(Context *context){
  // the header is declPrefix
  if(FUNCTION == context->token_to_scan){
    if(!FunctionDefinition(context)){
	   if(';'== context->token_to_scan)
		  return 0;
		else return 1;
	}
	else return 1;
  } else if (!GlobalInitializablePart2(context)){
    return 0;
  } else if (!GlobalSymbolDeclpart2(context)){
    return 0;
  }
  return 1;
}

int GlobalDecl(Context *context){

  if (SIGNATURE == context->token_to_scan){ // functionSignature
    return FunctionSignature(context);
  } else if (!DeclPrefix(context)){
      return GlobalDeclpart2(context);
  }

  return 1;
}

int SequenceOfPrograms(Context *context){
  while(VERSION == context->token_to_scan){
    if(!Program(context)){
      continue ;
    }else{
      context->set_error(INVALID_PROGRAM);
      return 1;
    }
  }
  return 0;
}

int PairAddressableOperand(Context* context) {
  // this judge(first token == '[') is necessary in here
  if (context->token_to_scan == '[') {
    int CurrentoOffset = 0;
    context->token_to_scan = yylex();
    CurrentoOffset = context->get_operand_offset();
    // AddressableOperand
    if ((context->token_to_scan == TOKEN_GLOBAL_IDENTIFIER) ||
        (context->token_to_scan == TOKEN_LOCAL_IDENTIFIER)) {
      std::string name(context->token_value.string_val);

      context->token_to_scan = yylex();

      if (context->token_to_scan == ']') {
        BrigOperandAddress boa = {
          sizeof(boa),            // size
          BrigEOperandAddress,    // kind
          Brigb32,                // type
          0,                      // reserved
          0/*,                      // directive
          0*/
        };

        boa.directive = context->symbol_map[name];

        if (context->get_machine() == BrigELarge) {
          boa.type = Brigb64;
        }
        context->append_operand(&boa);
        context->token_to_scan = yylex();
        if (context->token_to_scan == '[') {
          context->token_to_scan = yylex();
          if (!OffsetAddressableOperandPart2(context, CurrentoOffset)) {
            // Global/Local Identifier with offsetAddressOperand.
            return 0;
          } else {
            return 1;
          }
        } else {  // '['
          context->set_error(MISSING_OPENNING_BRACKET);
          return 1;
        }
      } else {
        context->set_error(MISSING_CLOSING_BRACKET);
        return 1;
      }
    } else {  // Global/Local Identifier
      context->set_error(MISSING_IDENTIFIER);
      return 1;
    }
  } else {  // '['
    context->set_error(MISSING_OPENNING_BRACKET);
    return 1;
  }
  return 1;
}

int LdaMod(Context* context) {
  if (context->token_type == ADDRESS_SPACE_IDENTIFIER) {
    return 0;
  }
  return 0;
}

int TopLevelStatement(Context *context){
  if(!Directive(context)) {
    return 0 ;
  }else if(KERNEL == context->token_to_scan) {
    return Kernel(context) ;
  }else if(SIGNATURE == context->token_to_scan){
    return GlobalDecl(context) ;
  }else if ( (context->token_to_scan == ALIGN) ||
             (context->token_to_scan == CONST) ||
             (context->token_to_scan == EXTERN) ||
             (context->token_to_scan == STATIC) ) {
    if(DeclPrefix(context)){
        return 1;
    }
  }

  if(FUNCTION == context->token_to_scan){
	if(!FunctionDefinition(context)){
		if(';' == context->token_to_scan){
			context->token_to_scan = yylex();
			return 0;
		}	
		else {
			if(!Codeblock(context)){
				context->token_to_scan = yylex();
				return 0;
			} else 
				return 1;
		}
	} else return 1;
  } else if (!GlobalInitializablePart2(context)){
    return 0;
  } else if (!GlobalSymbolDeclpart2(context)){
    return 0;
  }
}

int TopLevelStatements(Context *context){
 while (context->token_to_scan && (context->token_to_scan != VERSION)){
	if (TopLevelStatement(context)){
		return 1;
	}
	else continue;
  }
  return 0;
}

int ArrayOperandPart2(Context* context, BrigoOffset32_t* pRetOpOffset) {
  std::string op_name;
  if (context->token_to_scan == '(') {
    if (!ArrayOperandListPart2(context ,pRetOpOffset)) {
      return 0;
    } else {
      context->set_error(MISSING_CLOSING_PARENTHESIS);
      return 1;
    }
  } else {
    BrigoOffset32_t opSize = context->get_operand_offset();

    if (context->valid_string) {
      op_name = context->token_value.string_val;
    } else {
      if (context->token_type == CONSTANT) {
        opSize += opSize & 0x7;
      }
    }

    if (!Operand(context)) {
      if (opSize == context->get_operand_offset()) {
        *pRetOpOffset = context->operand_map[op_name];
      } else {
        *pRetOpOffset = opSize;
      }
      return 0;

    } else {
      context->set_error(MISSING_OPERAND);
      return 1;
    }
  }
  return 1;
}
int ArrayOperand(Context* context) {
  BrigoOffset32_t opOffset;
  return ArrayOperandPart2(context, &opOffset);
}

}  // namespace brig
}  // namespace hsa
