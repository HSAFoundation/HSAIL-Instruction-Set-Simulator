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

int Query(Context* context) {
  // next token should be a dataTypeId
  context->token_to_scan = yylex();
  if (context->token_type == DATA_TYPE_ID) {
    // next token should be an Operand
    context->token_to_scan = yylex();  // set token for Operand()
    if (!Operand(context)) {
      // next should be a comma
      if (context->token_to_scan == ',') {
        // then finally an addressable Operand
        context->token_to_scan = yylex();  // set token for addressableOperand()
        if (context->token_to_scan != '[') {
          context->set_error(MISSING_OPERAND);
        } else {
          context->token_to_scan = yylex();
        }

        if (!AddressableOperand(context)) {
          if (context->token_to_scan == ';') {
            context->token_to_scan = yylex();  // set token for next function
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
  // context->token_to_scan must be non register
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
        0,                      // directive
        0
      };

      boa.directive = context->symbol_map[name];

      if (context->get_machine() == BrigELarge)
        boa.type = Brigb64;

      context->append_operand(&boa);
      context->token_to_scan = yylex();
      return 0;
    } else if (context->token_to_scan == '<') {
      context->token_to_scan = yylex();
      if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
        context->token_to_scan = yylex();
        if (context->token_to_scan == '>') {
          context->token_to_scan = yylex();
          if (context->token_to_scan == ']') {
            context->token_to_scan = yylex();
            return 0;
          } else {
            context->set_error(MISSING_CLOSING_BRACKET);
          }
        } else {
          context->set_error(MISSING_CLOSING_BRACKET);
        }
      } else if (context->token_type == REGISTER) {
        context->token_to_scan = yylex();
        if (context->token_to_scan == '>') {
          context->token_to_scan = yylex();
          if (context->token_to_scan == ']') {
            context->token_to_scan = yylex();
            return 0;
          } else {
            context->set_error(MISSING_CLOSING_BRACKET);
          }
        } else if ((context->token_to_scan == '+') ||
                   (context->token_to_scan == '-')) {
          context->token_to_scan = yylex();
          if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
            context->token_to_scan = yylex();
            if (context->token_to_scan == '>') {
              context->token_to_scan = yylex();
              if (context->token_to_scan == ']') {
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
      context->token_to_scan = yylex();  // scan next
    } else {
      context->set_error(MISSING_CLOSING_BRACKET);
      return 1;
    }
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
        if (context->token_to_scan == '[') {
          if (!ArrayDimensionSet(context)) {
            // context->token_to_scan has been set in ArrayDimensionSet()
            return 0;
          }
        } else {
          // no arrayDimensions
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
            0,                                // dim
            arg_name_offset,                  // s_name
            context->token_value.data_type,   // data type
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
        }
      } else {
        context->set_error(MISSING_IDENTIFIER);
      }
    } else {
      context->set_error(MISSING_DATA_TYPE);
    }
  }
  context->set_error(MISSING_DECLPREFIX);
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

int FunctionDecl(Context* context) {
  if (!DeclPrefix(context)) {
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
          } else {
            context->set_error(INVALID_FBAR);
          }
        }
        if (context->token_to_scan == ';') {
          context->token_to_scan = yylex();
          return 0;
        } else {
          context->set_error(MISSING_SEMICOLON);
        }
      } else {
        context->set_error(MISSING_IDENTIFIER);
      }
    }
  } else {
    context->set_error(MISSING_DECLPREFIX);
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
      BrigOperandLabelRef label_operand = {
      8,
      BrigEOperandLabelRef,
      label_directive_offset
      };

      context->append_operand(&label_operand);
      context->label_o_map[label_name] = label_operand_offset;

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

int Function(Context* context) {
  if (!FunctionDefinition(context)) {
    if (!Codeblock(context)) {
      if (context->token_to_scan == ';') {
        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(MISSING_SEMICOLON);
      }
    } else {
      context->set_error(INVALID_CODEBLOCK);
    }

  } else {
    context->set_error(INVALID_FUNCTION_DEFINITION);
  }
  return 1;
}

int Program(Context* context) {
  int result;

  if (context->token_to_scan == VERSION) {
    if (!Version(context)) {
      // parse topLevelStatement
      while (context->token_to_scan && (context->token_to_scan != VERSION)) {
        if(!Directive(context)) {
          continue ;
        }else if(KERNEL == context->token_to_scan) {
          if(!Kernel(context))
            continue ;
          return 1 ;
        }else if(SIGNATURE == context->token_to_scan){
          if(!FunctionSignature(context))
            continue ;
           return 1;
        }else if ( (context->token_to_scan == ALIGN) ||
             (context->token_to_scan == CONST) ||
             (context->token_to_scan == EXTERN) ||
             (context->token_to_scan == STATIC) ) {
          if(DeclPrefix(context)){
              return 1;
          }
        }
        if(context->token_to_scan == FUNCTION){//functionDecl or functionDefinition
          if(TOKEN_GLOBAL_IDENTIFIER == yylex()){
            // check return argument list
            if ('(' == yylex()) {
            // context->set_arg_output(true);
              context->token_to_scan = yylex();

              if (')' == context->token_to_scan) {   // empty argument list body
                context->token_to_scan = yylex();
              } else if (!ArgumentListBody(context)) {
                if (')' == context->token_to_scan) {
                  context->token_to_scan = yylex();
                } else {
                  context->set_error(MISSING_CLOSING_PARENTHESIS);
                  return 1;  // missing closing )
                }
              } else {
                context->set_error(INVALID_ARGUMENT_LIST);
                return 1;
              }
            } else {  // missing '('
               context->set_error(MISSING_ARGUMENT_LIST);
                return 1;
            }          // if found '(' - returnArgList

            // check argument list
            if ('(' == context->token_to_scan) {
              //context->set_arg_output(true);
              context->token_to_scan = yylex();

              if (')' == context->token_to_scan) {   // empty argument list body
                context->token_to_scan = yylex();
              } else if (!ArgumentListBody(context)) {
                if (')' == context->token_to_scan) {
                  context->token_to_scan = yylex();
                } else {
                  context->set_error(MISSING_CLOSING_PARENTHESIS);
                  return 1;  // missing closing )
                }
              } else {
                context->set_error(INVALID_ARGUMENT_LIST);
                return 1;
              }
            } else {  // missing '('
               context->set_error(MISSING_ARGUMENT_LIST);
               return 1;
            }          // if found '(' - returnArgList

            // check for optional FBar
            if (context->token_to_scan == _FBAR) {
              if (!FBar(context)) {
              } else {
                context->set_error(INVALID_FBAR);
                return 1;
              }
            }

            if (context->token_to_scan == ';') {  // this is a functionDecl
              context->token_to_scan = yylex();
              continue;
            } else if (context->token_to_scan == '{') {
              // so this must be a functionDefinition
              if (!Codeblock(context)) {
                 // check codeblock of function
                if (context->token_to_scan == ';') {
                  context->token_to_scan = yylex();
                  continue;
                } else {
                  context->set_error(MISSING_SEMICOLON);
                  return 1;
                }
              } else {
                return 1;
              }
            } else {
              context->set_error(MISSING_SEMICOLON);
            }
          }else{ //TOKEN_GLOBAL_IDENTIFIER
             return 1;
          }
        }else if(GLOBAL == context->token_to_scan){
			BrigStorageClass32_t storage_class = context->token_value.storage_class;
			context->token_to_scan = yylex();
			switch(context->token_to_scan){
				case _RWIMG: 
							if(!GlobalImageDeclPart2(context))
								break;
							else{
								return 1;
							}
				case _ROIMG: 
							if(!GlobalReadOnlyImageDeclPart2(context))
								break;
							else{
								return 1;
							}
				case _SAMP : 	
							if(!GlobalSamplerDeclPart2(context))
								break;
							else{
								return 1;
							}
				default: 	
							if(context->token_type==DATA_TYPE_ID){
								if(!InitializableDeclPart2(context, storage_class))
									break;
								else 
									return 1;
							}
							else{
								context->set_error(MISSING_IDENTIFIER);
								return 1;
							}
				}
		}else if(GROUP == context->token_to_scan){
          if(!GlobalGroupDecl(context))
           continue ;
           return 1;
        }else if(PRIVATE == context->token_to_scan){
          if(!GlobalPrivateDecl(context))
            continue;
            return 1;
        }else if(READONLY == context->token_to_scan){
          if(!InitializableDecl(context))
            continue;
            return 1;
        }else{ 
            return  1;
        }
       }    // while (first_token)
       return 0;
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
      0
    } ;
    context->token_to_scan = yylex();
    if (context->token_to_scan == '(') {
      context->token_to_scan = yylex();
      if (context->token_to_scan == ALL) {
        op_width.bits.u = 0 ;

        context->token_to_scan = yylex();
      } else if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
        op_width.bits.u  = context->token_value.int_val;

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
            context->label_c_map.insert(make_pair(
                                          label_name,
                                          context->get_code_offset()+20));
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
        context->label_c_map.insert(make_pair(
                                      label_name,
                                      context->get_code_offset()+16));
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

	if((context->token_to_scan==GLOBAL) || (context->token_to_scan == READONLY)){
		context->token_to_scan = yylex();
		BrigStorageClass32_t storage_class = context->token_value.storage_class;
		return (InitializableDeclPart2(context, storage_class));
	} else{
		context->set_error(MISSING_GLOBAL_IDENTIFIER);
		return 1;
	}
}

int InitializableDeclPart2(Context *context, BrigStorageClass32_t storage_class)
{
  
  //First token already verified as GLOBAL/READONLY
  if (context->token_type == DATA_TYPE_ID) {
    context->token_to_scan = yylex();

    if (!Identifier(context)) {
      std::string var_name = context->token_value.string_val;
      int var_name_offset = context->add_symbol(var_name);

      // scan for arrayDimensions
      context->token_to_scan = yylex();
      if (context->token_to_scan == '[') {
        if (!ArrayDimensionSet(context)) {
        }
      } else {  // no arrayDimension
          // add by fxiaopeng
          BrigDirectiveSymbol sym_decl = {
            sizeof(sym_decl),                 // size
            BrigEDirectiveSymbol,             // kind
            {
              context->get_code_offset(),       // c_code
              storage_class,                    // storageClass ??
              context->get_attribute(),         // attribute
              0,                                // reserved
              context->get_symbol_modifier(),   // symbol modifier
              0,                                // dim
              var_name_offset,                  // s_name
              context->token_value.data_type,                        // data type
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
            bdf.d_nextDirective += 40;
            bdf.d_firstScopedDirective = bdf.d_nextDirective;
          } else {
            bdf.d_nextDirective += 40;
          }

          unsigned char *bdf_charp = reinterpret_cast<unsigned char*>(&bdf);
          context->update_directive_bytes(bdf_charp ,
                                          context->current_bdf_offset,
                                          40);
      }

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
  } else {
    context->set_error(MISSING_DATA_TYPE);
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
      if (context->token_to_scan == '[') {
        if (!ArrayDimensionSet(context)) {
        }
      } else {  // no arrayDimension
          // add by fxiaopeng
          BrigDirectiveSymbol sym_decl = {
            sizeof(sym_decl),                 // size
            BrigEDirectiveSymbol,             // kind
            {
              context->get_code_offset(),       // c_code
              storage_class,                    // storageClass ??
              context->get_attribute(),           // attribute
              0,                                // reserved
              context->get_symbol_modifier(),   // symbol modifier
              0,                                // dim
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
            bdf.d_nextDirective += 40;
            bdf.d_firstScopedDirective = bdf.d_nextDirective;
          } else {
            bdf.d_nextDirective += 40;
          }

          unsigned char *bdf_charp = reinterpret_cast<unsigned char*>(&bdf);
          context->update_directive_bytes(bdf_charp ,
                                          context->current_bdf_offset,
                                          40);
      }

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

      BrigDirectiveSymbol arg_decl = {
        sizeof(arg_decl),                 // size
        BrigEDirectiveSymbol,             // kind
        {
          context->get_code_offset(),       // c_code
          storage_class,                    // storageClass
          context->get_attribute(),         // attribute
          0,                                // reserved
          context->get_symbol_modifier(),   // symbol modifier
          0,                                // dim
          context->add_symbol(arg_name),    // s_name
          data_type,   // data_type
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

      // scan for arrayDimensions
      context->token_to_scan = yylex();
      if (context->token_to_scan == '[') {
        if (!ArrayDimensionSet(context)) {
        }
      }
      if (context->token_to_scan == ';') {
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
  context->token_to_scan = yylex();

  if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
    context->token_to_scan = yylex();

    if (context->token_to_scan == TOKEN_STRING) {
      context->token_to_scan = yylex();

      if (context->token_to_scan == ';') {
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
  context->token_to_scan = yylex();

  if (context->token_to_scan == TOKEN_SREGISTER) {
    if (yylex() == ',') {
      context->token_to_scan = yylex();

      if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
        if (yylex() == ',') {
          context->token_to_scan = yylex();
          if (((context->token_to_scan == TOKEN_SREGISTER) ||
               (context->token_to_scan == TOKEN_WAVESIZE) ||
               (context->token_to_scan == TOKEN_INTEGER_CONSTANT))) {
            if (yylex() == ',') {
              context->token_to_scan = yylex();

              if ((context->token_to_scan == TOKEN_SREGISTER) ||
                  (context->token_to_scan == TOKEN_WAVESIZE) ||
                  (context->token_to_scan == TOKEN_INTEGER_CONSTANT)) {
                if (yylex() == ',') {
                  context->token_to_scan = yylex();

                  if ((context->token_to_scan == TOKEN_SREGISTER) ||
                      (context->token_to_scan == TOKEN_WAVESIZE) ||
                      (context->token_to_scan == TOKEN_INTEGER_CONSTANT)) {
                    context->token_to_scan = yylex();

                    if (context->token_to_scan == ';') {
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
      context->set_error(MISSING_ARGUMENT_LIST);
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
    // TODO(xiaopeng) SignatureArgumentList read one more word
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
        // TODO(xiaopeng) SignatureArgumentList read one more word
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
    if (yylex() == ':') {
      context->token_to_scan = yylex();
      return 0;
    } else {
      context->set_error(MISSING_COLON);
      return 1;
    }
  } else {
    context->set_error(INVALID_LABEL);
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
  } else {
    context->set_error(INVALID_LABEL);
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

int Cmp(Context* context) {
  // first token is PACKEDCMP or CMP
  unsigned int first_token = context->token_to_scan;
  context->token_to_scan = yylex();
  if (context->token_type == COMPARISON) {
    context->token_to_scan = yylex();
    if (first_token == CMP) {
      if (context->token_type == DATA_TYPE_ID) {
        context->token_to_scan = yylex();
      } else {
        context->set_error(MISSING_DATA_TYPE);
        return 1;
      }
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
                  return 1;
                }
              } else {  // 3 operand
                context->set_error(INVALID_THIRD_OPERAND);
                return 1;
              }
            } else {
              context->set_error(MISSING_COMMA);
            }
          } else {  // 2 operand
            context->set_error(INVALID_SECOND_OPERAND);
            return 1;
          }
        } else {
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
    context->token_to_scan = yylex();
    if (context->token_to_scan == TOKEN_GLOBAL_IDENTIFIER) {
      context->token_to_scan = yylex();

      if (context->token_to_scan == '[') {
        if (!ArrayDimensionSet(context)) {}
      }

      if (context->token_to_scan == ';') {
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

int OffsetAddressableOperand(Context* context, BrigoOffset32_t addrOpOffset) {

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
int MemoryOperand(Context* context) {
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
          0,                      // directive
          0
        };
  
        boa.directive = context->symbol_map[name];
  
        if (context->get_machine() == BrigELarge) {
          boa.type = Brigb64;
        }
        context->append_operand(&boa);
        context->token_to_scan = yylex();
      } else {
        context->set_error(MISSING_CLOSING_BRACKET);
        return 1;
      }
      if (context->token_to_scan == '[') {
        context->token_to_scan = yylex();
        if (!OffsetAddressableOperand(context, CurrentoOffset)) {
          // Global/Local Identifier with offsetAddressOperand.
          return 0;
        }
      }
      // only Global/Local Identifier without offsetAddressOperand.
      return 0;
    } else if (!AddressableOperand(context)) {
      return 0;
    } else if (!OffsetAddressableOperand(context, 0)) {
      return 0;
    }  // Global/Local Identifier/AddressableOperand/offsetAddressableOperand 
  }  // '['
  return 1;
}

int Instruction5(Context* context) {
  // first token is F2U4 "f2u4"
  context->token_to_scan = yylex();
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

  if (context->token_type == DATA_TYPE_ID) {
    context->token_to_scan = yylex();
    if (!Operand(context)) {
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        if (context->token_to_scan == TOKEN_LABEL ||
           !Identifier(context)) {
          context->token_to_scan = yylex();
          if (context->token_to_scan == ';') {
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
  unsigned int first_token = context->token_to_scan;
  context->token_to_scan = yylex();
  if (first_token == ATOMIC) {   // atomic
    if (context->token_type == ATOMIC_OP) {
      context->token_to_scan = yylex();
      // with AtomModifiers
      if (context->token_type == ADDRESS_SPACE_IDENTIFIER ||
          context->token_to_scan == _AR ||
          context->token_to_scan == _ACQ ||
          context->token_to_scan == _REL ||
          context->token_to_scan == _REGION) {
        context->token_to_scan = yylex();
        while (1) {  // AtomModifiers repetition
          if (context->token_type == ADDRESS_SPACE_IDENTIFIER ||
              context->token_to_scan == _AR ||
              context->token_to_scan == _ACQ ||
              context->token_to_scan == _REL ||
              context->token_to_scan == _REGION) {
            context->token_to_scan = yylex();
          } else {
            break;
          }
        }
        if (context->token_type == DATA_TYPE_ID) {
          context->token_to_scan = yylex();
          if (!Operand(context)) {
            if (context->token_to_scan == ',') {
              context->token_to_scan = yylex();
              if (!MemoryOperand(context)) {
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
      } else if (context->token_type == DATA_TYPE_ID) {
        // without AtomModifiers
        context->token_to_scan = yylex();
        if (!Operand(context)) {
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if (!MemoryOperand(context)) {
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
      }  else {
        context->set_error(MISSING_DATA_TYPE);
      }
    } else {
      context->set_error(UNKNOWN_ERROR);
    }
  } else if (first_token == ATOMIC_CAS) {   // atomic_cas
    if (context->token_type == ADDRESS_SPACE_IDENTIFIER ||
        // with AtomModifiers
        context->token_to_scan == _AR ||
        context->token_to_scan == _ACQ ||
        context->token_to_scan == _REL ||
        context->token_to_scan == _REGION) {
        context->token_to_scan = yylex();
      while (1) {  // AtomModifiers repetition
        if (context->token_type == ADDRESS_SPACE_IDENTIFIER ||
            context->token_to_scan == _AR ||
            context->token_to_scan == _ACQ ||
            context->token_to_scan == _REL ||
            context->token_to_scan == _REGION) {
            context->token_to_scan = yylex();
        } else {
          break;
        }
      }
      if (context->token_type == DATA_TYPE_ID) {
        context->token_to_scan = yylex();
        if (!Operand(context)) {
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if (!MemoryOperand(context)) {
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
    } else if (context->token_type == DATA_TYPE_ID) {  // without AtomModifiers
      context->token_to_scan = yylex();
      if (!Operand(context)) {
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
          if (!MemoryOperand(context)) {
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

int CvtModifier1(Context* context) {
  unsigned int next;
  unsigned int first_token = context->token_to_scan;
  // get current alu modifier from context
  BrigAluModifier mod = context->get_alu_modifier();

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
    context->token_to_scan = yylex();  // set context for following functions
    context->set_alu_modifier(mod);
    return 0;
  } else {
    return 1;
  }
}

int Mov(Context* context) {
// first token is MOV "mov"
  context->token_to_scan = yylex();

  if (context->token_type == DATA_TYPE_ID) {
    context->token_to_scan = yylex();
    if (!Operand(context) || !ArrayOperandList(context)) {
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        if (!Operand(context) || !ArrayOperandList(context)) {
          if (context->token_to_scan == ';') {
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
    context->token_to_scan = yylex();
    if (context->token_to_scan == TOKEN_GLOBAL_IDENTIFIER) {
      context->token_to_scan = yylex();
      if (context->token_to_scan == '[') {
        if (!ArrayDimensionSet(context)) {}
      }
      if (context->token_to_scan == ';') {
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
  if (context->token_type == PACKING) {
    context->token_to_scan = yylex();
  }
  if (context->token_type == DATA_TYPE_ID) {
    context->token_to_scan = yylex();
    return 0;
  } else {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  return 1;
}
int Mul24Inst(Context* context) {
  if (context->token_type == DATA_TYPE_ID) {
    context->token_to_scan = yylex();
    return 0;
  } else {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  return 1;
}
int Mad24Inst(Context* context) {
  if (context->token_type == DATA_TYPE_ID) {
    context->token_to_scan = yylex();
    if (!Operand(context)) {
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(MISSING_COMMA);
        return 1;
      }
    } else {
      context->set_error(MISSING_OPERAND);
      return 1;
    }
  } else {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  return 1;
}

int Mul(Context* context) {
  unsigned int first_token = context->token_to_scan;
  context->token_to_scan = yylex();
  switch (first_token) {
    case MUL: {
      if (!RoundingMode(context)) {
      }
    }
    case MUL_HI: {
      if (!MulInst(context)) {
        break;
      } else {
        return 1;
      }
    }
    case MUL24:
    case MUL24_HI: {
      if (!Mul24Inst(context)) {
        break;
      } else {
        return 1;
      }
    }
    case MAD24:
    case MAD24_HI: {
      if (!Mad24Inst(context)) {
        break;
      } else {
        return 1;
      }
    }
    default: {
      return 1;
    }
  }
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
          return 0;
        } else {  // ';'
          context->set_error(MISSING_OPERAND);
          return 1;
        }
      } else {  // Operand 3 or 4
        context->set_error(MISSING_OPERAND);
        return 1;
      }
    } else {  // Operand 2 or 3
      context->set_error(MISSING_OPERAND);
      return 1;
    }
  } else {  // Operand 1 or 2
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  context->set_error(UNKNOWN_ERROR);
  return 1;
}

int LdModifier(Context *context, BrigInstLdSt* pLdSt_op, int* pVec_size) {
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
      switch (context->token_to_scan) {
        case _GLOBAL:
          pLdSt_op->storageClass = BrigGlobalSpace;
          break;
        case _GROUP:
          pLdSt_op->storageClass = BrigGroupSpace;
          break;
        case _PRIVATE:
          pLdSt_op->storageClass = BrigPrivateSpace;
          break;
        case _KERNARG:
          pLdSt_op->storageClass = BrigKernargSpace;
          break;
        case _READONLY:
          pLdSt_op->storageClass = BrigReadonlySpace;
          break;
        case _SPILL:
          pLdSt_op->storageClass = BrigSpillSpace;
          break;
        case _ARG:
          pLdSt_op->storageClass = BrigArgSpace;
          break;
        default:
          pLdSt_op->storageClass = BrigFlatSpace;
      }
      context->token_to_scan = yylex();
      continue;
    }

    if (context->token_to_scan == _REL ||
        context->token_to_scan == _ACQ ||
        context->token_to_scan == _DEP) {

      if (pLdSt_op->memorySemantic == _ACQ && 
          context->token_to_scan == _REL) {
        pLdSt_op->memorySemantic = BrigAcquireRelease;
      } else {
        pLdSt_op->memorySemantic = context->token_to_scan;
      }

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
  if (!LdModifier(context, &ld_op, &vector_size)) {
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
      // TODO(Chuang): When the operand is a list.
      // ArrayOperandList generate the .Operand brig.
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

  if (!LdModifier(context, &st_op, &vector_size)) {
  } else {
    context->set_error(UNKNOWN_ERROR);
    return 1;
  }

  if (context->token_type == DATA_TYPE_ID) {
    st_op.type = context->token_value.data_type;
    context->token_to_scan = yylex();
    if (context->token_to_scan == '(') {  
      st_op.o_operands[0] = context->get_operand_offset();

      // TODO(Chuang): When the operand is a list.
      // ArrayOperandList generate the .Operand brig.
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

      // TODO(Chuang): When the operand is a operation.
      // MemoryOperand generate the .Operand brig.
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

  if (context->token_type == ADDRESS_SPACE_IDENTIFIER) {
    context->token_to_scan = yylex();
  }
  if (context->token_type == DATA_TYPE_ID) {
    context->token_to_scan = yylex();
    if (!Operand(context)) {
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        if (!MemoryOperand(context)) {
          if (context->token_to_scan == ';') {
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

int ImageRet(Context* context) {
  // first token is ATOMIC_IMAGE
  unsigned second_token;
  context->token_to_scan = yylex();
  second_token = context->token_to_scan;

  if (context->token_to_scan == _CAS) {
    context->token_to_scan = yylex();
  } else if (context->token_type == ATOMIC_OP) {
    context->token_to_scan =yylex();
  } else {
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }
  if (context->token_to_scan == _AR || context->token_to_scan == _REL ||
      context->token_to_scan == _ACQ ) {
    context->token_to_scan = yylex();
  }

  if (context->token_type == GEOMETRY_ID) {
    context->token_to_scan = yylex();
    if (context->token_type == DATA_TYPE_ID) {
      context->token_to_scan = yylex();
      if (!Operand(context)) {
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
          if (context->token_to_scan == '[') {
            context->token_to_scan = yylex();
            if (!AddressableOperand(context)) {
              if (context->token_to_scan == ',') {
                context->token_to_scan = yylex();
                if (context->token_to_scan == '(') {
                  if (!ArrayOperandList(context)) {
                  } else {
                    context->set_error(MISSING_CLOSING_PARENTHESIS);
                    return 1;
                  }
                } else if (!Operand(context)) {
                } else {  // Array Operand
                  context->set_error(MISSING_OPERAND);
                  return 1;
                }

                if (context->token_to_scan == ',') {
                  context->token_to_scan = yylex();
                  if (!Operand(context)) {
                    if (second_token == _CAS) {
                      if (context->token_to_scan == ',') {
                        context->token_to_scan = yylex();
                        if (!Operand(context)) {
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
  unsigned second_token;
  context->token_to_scan = yylex();

  second_token = context->token_to_scan;

  if (context->token_to_scan == _CAS) {
    context->token_to_scan = yylex();
  } else if (context->token_type == ATOMIC_OP) {
    context->token_to_scan =yylex();
  } else {
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }
  if (context->token_to_scan == _AR || context->token_to_scan == _REL ||
      context->token_to_scan == _ACQ ) {
    context->token_to_scan = yylex();
  }
  if (context->token_type == GEOMETRY_ID) {
    context->token_to_scan = yylex();
    if (context->token_type == DATA_TYPE_ID) {
      context->token_to_scan = yylex();
      if (context->token_to_scan == '[') {
        context->token_to_scan = yylex();
        if (!AddressableOperand(context)) {
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if (context->token_to_scan == '(') {
              if (!ArrayOperandList(context)) {
              } else {
                context->set_error(MISSING_CLOSING_PARENTHESIS);
                return 1;
              }
            } else if (!Operand(context)) {
            } else {  // Array Operand
              context->set_error(MISSING_OPERAND);
              return 1;
            }
            if (context->token_to_scan == ',') {
              context->token_to_scan = yylex();
              if (!Operand(context)) {
                if (second_token == _CAS) {
                  if (context->token_to_scan == ',') {
                    context->token_to_scan = yylex();
                    if (!Operand(context)) {
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
  context->token_to_scan = yylex();
  if (!CvtModifier1(context)) {
    if (context->token_type == DATA_TYPE_ID) {
      context->token_to_scan = yylex();
      if (context->token_type == DATA_TYPE_ID) {
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
          context->set_error(INVALID_OPERAND);
        }
      } else {
        context->set_error(MISSING_DATA_TYPE);
      }
    } else {
      context->set_error(MISSING_DATA_TYPE);
    }
  } else if (context->token_type == DATA_TYPE_ID) {
  context->token_to_scan = yylex();
  if (context->token_type == DATA_TYPE_ID) {
    context->token_to_scan = yylex();
    if (!Operand(context)) {
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        if (!Operand(context)) {
          if (context->token_to_scan == ';') {
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
  if (context->token_type == INSTRUCTION1_OPCODE_NODT) {
    // Instruction1OpcodeNoDT
    context->token_to_scan = yylex();
    if (!RoundingMode(context)) {  // with RoundingMode
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
    } else if (!Operand(context)) {  // without RoundingMode
      if (context->token_to_scan == ';') {
        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(MISSING_SEMICOLON);
      }
    } else {
        context->set_error(MISSING_OPERAND);
    }
  } else if (context->token_to_scan == CLOCK) {  // clock
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
  } else if (context->token_type == INSTRUCTION1_OPCODE) {
    context->token_to_scan = yylex();
    if (!RoundingMode(context)) {  // with RoundingMode
      if (context->token_type == DATA_TYPE_ID) {
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
        context->set_error(MISSING_DATA_TYPE);
      }
    } else if (context->token_type == DATA_TYPE_ID) {  // without RoundingMode
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
      context->set_error(MISSING_DATA_TYPE);
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
    if (context->token_type == ADDRESS_SPACE_IDENTIFIER) {
      context->token_to_scan = yylex();
      if (context->token_to_scan == _B1) { //datatypeId must be b1
        context->token_to_scan = yylex();
        //dest must be c register
        if (context->token_to_scan == TOKEN_CREGISTER) { 
          context->token_to_scan = yylex();
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if (!RIW_Operand(context)) {
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
        context->set_error(MISSING_DATA_TYPE);
      }
    } else {
      // should be missing ADDRESS_SPACE_IDENTIFIER
      context->set_error(UNKNOWN_ERROR);  
    }
  } else if (context->token_to_scan == STOF || // stof or ftos
             context->token_to_scan == FTOS) {
    context->token_to_scan = yylex();
    if (context->token_type == ADDRESS_SPACE_IDENTIFIER) {
      context->token_to_scan = yylex();
      if (context->token_to_scan == _U32 ||
          context->token_to_scan == _U64) { //datatypeId must be u32 or u64
        context->token_to_scan = yylex();
        //dest must be d register
        if (context->token_to_scan == TOKEN_DREGISTER) { 
          context->token_to_scan = yylex();
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if (!RIW_Operand(context)) {
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
      context->token_to_scan = yylex();
      return 0; 
    }
  } else if (context->token_to_scan == NOP) {
    if (!Instruction0(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_type == INSTRUCTION2_OPCODE_FTZ ||
             context->token_type == INSTRUCTION2_OPCODE ||
             context->token_type == INSTRUCTION2_OPCODE_NODT) {
    if (!Instruction2(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_type == INSTRUCTION3_OPCODE ||
             context->token_type == INSTRUCTION3_OPCODE_FTZ) {
    if (!Instruction3(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_type == INSTRUCTION4_OPCODE) {
    if (!Instruction4(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == PACKEDCMP ||
             context->token_to_scan == CMP) {
    if (!Cmp(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == MUL ||
             context->token_to_scan == MUL_HI ||
             context->token_to_scan == MUL24_HI ||
             context->token_to_scan == MUL24 ||
             context->token_to_scan == MAD24 ||
             context->token_to_scan == MAD24_HI) {
    if (!Mul(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == F2U4) {
    if (!Instruction5(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == MOV) {
    if (!Mov(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == SEGMENTP ||
             context->token_to_scan == FTOS ||
             context->token_to_scan == STOF) {
    if (!Segp(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == LDA) {
    if (!Lda(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == LDC) {
    if (!Ldc(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == ATOMIC ||
             context->token_to_scan == ATOMIC_CAS) {
    if (!Atom(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == RD_IMAGE) {
    if (!ImageRead(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == LD_IMAGE) {
    if (!ImageLoad(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == ST_IMAGE) {
    if (!ImageStore(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == LD) {
    if (!Ld(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == ST) {
    if (!St(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == CVT) {
    if (!Cvt(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == ATOMICNORET ||
             context->token_to_scan == ATOMICNORET_CAS) {
    if (!AtomicNoRet(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == ATOMIC_IMAGE) {
    if (!ImageRet(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == SYNC) {
    if (!Sync(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == BARRIER) {
    if (!Bar(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == SYSCALL) {
    if (!SysCall(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  }  else if (context->token_to_scan == RET) {
    if (!Ret(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_to_scan == CBR ||
             context->token_to_scan == BRN) {
    if (!Branch(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } else if (context->token_type = QUERY_OP) {
    if (!Query(context)) {
      context->token_to_scan = yylex();
      return 0;
    }
  } 
  return 1;
}


int ImageLoad(Context* context) {
  // first token is LD_IMAGE
  context->token_to_scan = yylex();
  if (context->token_to_scan == _V4) {
    context->token_to_scan = yylex();
    if (context->token_type == GEOMETRY_ID) {
      context->token_to_scan = yylex();
      if (context->token_type == DATA_TYPE_ID) {
        context->token_to_scan = yylex();
        if (context->token_type == DATA_TYPE_ID) {
          context->token_to_scan = yylex();
          if (context->token_to_scan == '(') {
            if (!ArrayOperandList(context)) {
            } else {
              context->set_error(MISSING_CLOSING_PARENTHESIS);
              return 1;
            }
          } else if (!Operand(context)) {
          } else {  // Array Operand
            context->set_error(MISSING_OPERAND);
            return 1;
          }
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if (context->token_to_scan == '[') {
              context->token_to_scan = yylex();
              if (!AddressableOperand(context)) {
                if (context->token_to_scan == ',') {
                  context->token_to_scan = yylex();
                  if (context->token_to_scan == '(') {
                    if (!ArrayOperandList(context)) {
                    } else {
                      context->set_error(MISSING_CLOSING_PARENTHESIS);
                      return 1;
                    }
                  } else if (!Operand(context)) {
                  } else {  // Array Operand
                    context->set_error(MISSING_OPERAND);
                    return 1;
                  }
                  if (context->token_to_scan == ';') {
                    context->token_to_scan = yylex();
                    return 0;
                  } else {  // ';'
                    context->set_error(MISSING_SEMICOLON);
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
  context->token_to_scan = yylex();
  if (context->token_to_scan == _V4) {
    context->token_to_scan = yylex();
    if (context->token_type == GEOMETRY_ID) {
      context->token_to_scan = yylex();
      if (context->token_type == DATA_TYPE_ID) {
        context->token_to_scan = yylex();
        if (context->token_type == DATA_TYPE_ID) {
          context->token_to_scan = yylex();
          if (context->token_to_scan == '(') {
            if (!ArrayOperandList(context)) {
            } else {
              context->set_error(MISSING_CLOSING_PARENTHESIS);
              return 1;
            }
          } else if (!Operand(context)) {
          } else {  // Array Operand
            context->set_error(MISSING_OPERAND);
            return 1;
          }
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if (context->token_to_scan == '[') {
              context->token_to_scan = yylex();
              if (!AddressableOperand(context)) {
                if (context->token_to_scan == ',') {
                  context->token_to_scan = yylex();
                  if (context->token_to_scan == '(') {
                    if (!ArrayOperandList(context)) {
                    } else {
                      context->set_error(MISSING_CLOSING_PARENTHESIS);
                      return 1;
                    }
                  } else if (!Operand(context)) {
                  } else {  // Array Operand
                    context->set_error(MISSING_OPERAND);
                    return 1;
                  }
                  if (context->token_to_scan == ';') {
                    context->token_to_scan = yylex();
                    return 0;
                  } else {  // ';'
                    context->set_error(MISSING_SEMICOLON);
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
    while (1) {
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
        return 0;
      }
    }
  } else {
    context->set_error(MISSING_SINGLE_CONSTANT);
    return 1;
  }
  return 0;
}

int ImageInit(Context *context) {
  if (FORMAT == context->token_to_scan
    // for "format" = TOKEN_PROPERTY
    || ORDER == context->token_to_scan) {        // "order" = TOKEN_PROPERTY
    context->token_to_scan = yylex();
     if ('=' == context->token_to_scan) {
       context->token_to_scan = yylex();
       if (TOKEN_PROPERTY == context->token_to_scan) {
         context->token_to_scan = yylex();
         return 0;
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
       // for tobNumeric "=" TOKEN_INTEGER_CONSTANT
    context->token_to_scan = yylex();
    if ('=' == context->token_to_scan) {
      context->token_to_scan = yylex();
      if (TOKEN_INTEGER_CONSTANT == context->token_to_scan) {
        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(MISSING_PROPERTY);
        return 1;
      }
    } else {
      context->set_error(INVALID_IMAGE_INIT);
      return 1;
    }
  } else {
    context->set_error(INVALID_IMAGE_INIT);
    return 1;
  }
}

int GlobalImageDecl(Context *context) {
	if(context->token_to_scan == GLOBAL){
		context->token_to_scan = yylex();
	  	return (GlobalImageDeclPart2(context));
	}else{
		context->set_error(MISSING_GLOBAL_IDENTIFIER);
		return 1;
	}
}

int GlobalImageDeclPart2(Context *context){	
  //First token has been scanned and verified as global. Read next token.
  
  if (_RWIMG == context->token_to_scan) {
    context->token_to_scan = yylex();
    if (TOKEN_GLOBAL_IDENTIFIER == context->token_to_scan) {
      context->token_to_scan = yylex();
      if ('[' == context->token_to_scan) {
        if (!ArrayDimensionSet(context)) {
          if ('=' == context->token_to_scan) {
            if (!ImageInitializer(context)) {
              if (';' == context->token_to_scan) {
                context->token_to_scan = yylex();
                return 0;
              } else {
                context->set_error(MISSING_SEMICOLON);
              }
            } else {
              context->set_error(INVALID_IMAGE_INIT);
            }
          } else {  // end for =
            if (';' == context->token_to_scan) {
              context->token_to_scan = yylex();
              return 0;
            } else {
              context->set_error(MISSING_SEMICOLON);
            }
          }
        }
      } else if ('=' == context->token_to_scan) {  // end for [
        if (!ImageInitializer(context)) {       // no arraydimensions
          if (';' == context->token_to_scan) {
            context->token_to_scan = yylex();
            return 0;
          } else {
            context->set_error(MISSING_SEMICOLON);
          }
        }
      } else if (';' == context->token_to_scan) {
        // no arrayDimensions and imageInitializer
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

int ImageInitializer(Context *context) {
  // first must be '='
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

int GlobalReadOnlyImageDecl(Context *context) {
	if(GLOBAL == context->token_to_scan){
		context->token_to_scan = yylex();
		return GlobalReadOnlyImageDeclPart2(context);
	}else{
		context->set_error(MISSING_GLOBAL_IDENTIFIER);
		return 1;
	}	
}

int GlobalReadOnlyImageDeclPart2(Context *context){
  //First token has been scanned and verified as global. Scan next token.		
  
  if (_ROIMG == context->token_to_scan) {
    context->token_to_scan = yylex();
    if (TOKEN_GLOBAL_IDENTIFIER == context->token_to_scan) {
      context->token_to_scan = yylex();
      if ('[' == context->token_to_scan) {
        if (!ArrayDimensionSet(context)) {
          if ('=' == context->token_to_scan) {
            if (!ImageInitializer(context)) {
              if (';' == context->token_to_scan) {
                context->token_to_scan = yylex();
                return 0;
              } else {
                context->set_error(MISSING_SEMICOLON);
              }
            } else {
            }
          } else {  // end for =
            if (';' == context->token_to_scan) {
              context->token_to_scan = yylex();
              return 0;
            } else {
              context->set_error(MISSING_SEMICOLON);
            }
          }
        }
      } else if ('=' == context->token_to_scan) {  // end for [
        if (!ImageInitializer(context)) {       // no arraydimensions
          if (';' == context->token_to_scan) {
            context->token_to_scan = yylex();
            return 0;
          } else {
            context->set_error(MISSING_SEMICOLON);
          }
        }
      } else if (';' == context->token_to_scan) {
        // no arrayDimensions and imageInitializer
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

int Ret(Context* context) {
  // first token is RET
  if(RET != context->token_to_scan)
    return 1;

  BrigOpcode32_t opcode = context->token_value.opcode ;
  uint32_t syncFlag = BrigPartialLevel ;

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
  context->token_to_scan = yylex();
  if (context->token_to_scan == _V4) {
    context->token_to_scan = yylex();
    if (context->token_type == GEOMETRY_ID) {
      context->token_to_scan = yylex();
      if (context->token_type == DATA_TYPE_ID) {
        context->token_to_scan = yylex();
        if (context->token_type == DATA_TYPE_ID) {
          context->token_to_scan = yylex();

          if (context->token_to_scan == '(') {
            if (!ArrayOperandList(context)) {
            } else {
              context->set_error(MISSING_CLOSING_PARENTHESIS);
              return 1;
            }
          } else if (!Operand(context)) {
          } else {  // Array Operand
            context->set_error(MISSING_OPERAND);
            return 1;
          }
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if (context->token_to_scan == '[') {
              context->token_to_scan = yylex();
              if (!AddressableOperand(context)) {
                if (context->token_to_scan == ',') {
                  context->token_to_scan = yylex();
                  if (context->token_to_scan == '[') {
                    context->token_to_scan = yylex();
                    if (!AddressableOperand(context)) {
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

                  if (context->token_to_scan == '(') {
                    if (!ArrayOperandList(context)) {
                    } else {
                      context->set_error(MISSING_CLOSING_PARENTHESIS);
                      return 1;
                    }
                  } else if (!Operand(context)) {
                  } else {  // Array Operand
                    context->set_error(MISSING_OPERAND);
                    return 1;
                  }
                  if (context->token_to_scan == ';') {
                    context->token_to_scan = yylex();
                    return 0;
                  } else {  // ';'
                    context->set_error(MISSING_SEMICOLON);
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
  while (1) {
    if (context->token_type == ADDRESS_SPACE_IDENTIFIER) {
      context->token_to_scan = yylex();
      continue;
    }

    if (context->token_to_scan == _AR) {
      context->token_to_scan = yylex();
      continue;
    }

    if (context->token_to_scan == _REL || context->token_to_scan == _ACQ) {
      context->token_to_scan = yylex();
      continue;
    }

    if (context->token_to_scan == _REGION) {
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
  const unsigned int first_token = context->token_to_scan;
  context->token_to_scan = yylex();
  if (first_token == ATOMICNORET) {
    if (context->token_type == ATOMIC_OP) {
      context->token_to_scan = yylex();
    } else {
      context->set_error(MISSING_OPERAND);
      return 1;
    }
  }

  if (!AtomModifiers(context)) {
    if (context->token_type == DATA_TYPE_ID) {
      context->token_to_scan = yylex();
      if (!MemoryOperand(context)) {
        if (first_token == ATOMICNORET_CAS) {
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if (!Operand(context)) {
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
          if (!Operand(context)) {
            if (context->token_to_scan == ';') {
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
  // first token is LOG
  context->token_to_scan = yylex();
  if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
    context->token_to_scan = yylex();
    if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
      context->token_to_scan = yylex();
      if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
        context->token_to_scan = yylex();
        if (context->token_to_scan == ';') {
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
int LabelList(Context* context) {
  while (context->token_to_scan == TOKEN_LABEL) {
    context->token_to_scan = yylex();
    if (context->token_to_scan == ',') {
      context->token_to_scan = yylex();
      continue;
    } else {
      // Note: the token has been updated
      return 0;
    }
  }  // While
  context->set_error(MISSING_DOUBLE_CONSTANT);
  return 1;
}
int FloatListSingle(Context* context) {
  while (context->token_to_scan == TOKEN_DOUBLE_CONSTANT) {
    context->token_to_scan = yylex();
    if (context->token_to_scan == ',') {
      context->token_to_scan = yylex();
      continue;
    } else {
      // Note: the token has been updated
      return 0;
    }
  }  // While
  context->set_error(MISSING_DOUBLE_CONSTANT);
  return 1;
}

int DecimalListSingle(Context* context) {

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
      context->token_to_scan = yylex();
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        continue;
      } else {
        // Note: the token has been updated
        return 0;
      }  // ','
    }  // integer constant
    return 0;
  }  // While
  context->set_error(MISSING_INTEGER_CONSTANT);
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


int GlobalSymbolDecl(Context* context) {
  if (!DeclPrefix(context)) {
    if (context->token_to_scan == GROUP) 
      return (GlobalGroupDecl(context));
    else if (context->token_to_scan == PRIVATE) 
      return(GlobalPrivateDecl(context));
	else {
		context->set_error(MISSING_IDENTIFIER);
		return 1;
	}
  } else{
	context->set_error(MISSING_DECLPREFIX);
	return 1;
  }
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
      context->set_error(UNKNOWN_ERROR);
      return 1;    
  }
  context->set_error(UNKNOWN_ERROR);
  return 1;
}

int SobInit(Context *context){
 if(COORD == context->token_to_scan  
    ||FILTER == context->token_to_scan  
    ||BOUNDARYU == context->token_to_scan  
    ||BOUNDARYV == context->token_to_scan  
    ||BOUNDARYW == context->token_to_scan){
    context->token_to_scan = yylex();
    if('=' == context->token_to_scan){
      context->token_to_scan = yylex();
      if(TOKEN_PROPERTY == context->token_to_scan){
        context->token_to_scan = yylex();
        return 0;
      }else{
	context->set_error(MISSING_PROPERTY);
      }
    }else{ //for '='
      context->set_error(MISSING_IDENTIFIER);
    }
  }else{
    context->set_error(MISSING_IDENTIFIER);
  }  

  return 1;
}

int SobInitializer(Context *context){
  //first must be '='
  if('=' != context->token_to_scan){
     context->set_error(MISSING_IDENTIFIER);
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
		context->set_error(MISSING_GLOBAL_IDENTIFIER);
		return 1;
	}
}	

int GlobalSamplerDeclPart2(Context *context){	
	//First token has already been verified as GLOBAL
	
	if(_SAMP == context->token_to_scan){ 
		context->token_to_scan = yylex();
		if(TOKEN_GLOBAL_IDENTIFIER == context->token_to_scan){
			context->token_to_scan = yylex();
			if('[' == context->token_to_scan){
				if(!ArrayDimensionSet(context)){
					if(!SobInitializer(context)){
						if(';' == context->token_to_scan){
							context->token_to_scan = yylex();
							return 0 ;
						}else{
							context->set_error(MISSING_SEMICOLON);
						}
					}else{//end for =
						if(';' == context->token_to_scan){
							context->token_to_scan = yylex();
							return 0 ;
						}else{
						context->set_error(MISSING_SEMICOLON);
						}
					}
				}
			}else if('=' == context->token_to_scan){//end for [
				if(!SobInitializer(context)){       //no arraydimensions
					if(';' == context->token_to_scan){
						context->token_to_scan = yylex();
						return 0 ;
					}else{
						context->set_error(MISSING_SEMICOLON);
					}
				}
			}else if(';' == context->token_to_scan){ //no arrayDimensions and imageInitializer
				context->token_to_scan = yylex();
				return 0 ;
			}else{
				context->set_error(MISSING_SEMICOLON);
			}
		}
	}else{
		context->set_error(MISSING_IDENTIFIER);
	}
	
	return 1;
}

int GlobalInitializablePart2(Context* context){
	
		if(GLOBAL == context->token_to_scan){
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
							if(context->token_type==DATA_TYPE_ID)
								return (InitializableDeclPart2(context, storage_class));
							else{	
								context->set_error(MISSING_IDENTIFIER);
								return 1;
							}
			}
		} else if(READONLY == context->token_to_scan){
			return InitializableDecl(context);
		} else{
			return 1;
		}
		return 0;
}

int GlobalInitializable(Context* context){
	if(!DeclPrefix(context)){
		return GlobalInitializablePart2(context);
	} else{
		context->set_error(MISSING_DECLPREFIX);
		return 1;
	}
}

int GlobalDecl(Context *context){
  
	if(SIGNATURE == context->token_to_scan){ //functionSignature 
		return FunctionSignature(context);
	}else if(!DeclPrefix(context)){
   		if(FUNCTION == context->token_to_scan){
			return FunctionDecl(context);
		}else if(!GlobalInitializablePart2(context)){
			return 0;
		}else if(!GlobalSymbolDecl(context)){
			return 0;
		}
	}else {
		context->set_error(MISSING_IDENTIFIER);
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

}  // namespace brig
}  // namespace hsa
