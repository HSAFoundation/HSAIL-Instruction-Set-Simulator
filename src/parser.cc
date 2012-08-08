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
  yylex();
  if (context->token_type == DATA_TYPE_ID) {
    // next token should be an Operand
    context->token_to_scan = yylex();  // set token for Operand()
    if (!Operand(context)) {
      // next should be a comma
      if (context->token_to_scan == ',') {
        // then finally an addressable Operand
        context->token_to_scan = yylex();  // set token for addressableOperand()
        if (!AddressableOperand(context)) {
          if (context->token_to_scan == ';') {
            context->token_to_scan = yylex();  // set token for next function
            return 0;
          } else {
            context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
          }
        } else {
          context->set_error(ErrorReporterInterface::MISSING_COMMA);
        }
      } else {
        context->set_error(ErrorReporterInterface::MISSING_COMMA);
      }
    } else {
      context->set_error(ErrorReporterInterface::MISSING_OPERAND);
    }
  } else {
    context->set_error(ErrorReporterInterface::MISSING_DATA_TYPE);
  }
  return 1;
}

int Operand(Context* context) {
  unsigned int first_token = context->token_to_scan;
  if (!Identifier(context)) {  // an Identifier
    if (context->token_type == REGISTER) {
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
      std::string name(context->token_value.string_val);


      bor.name = context->add_symbol(name);

      if (!context->operand_map.count(name)) {
        context->operand_map[name] = context->get_operand_offset();
        context->append_operand(&bor);
      }
      name.clear();
    }
    // free memory
    free(context->token_value.string_val);
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
    return 0;
  }
  return 1;
}

int BaseOperand(Context* context) {
  int next;
  if (context->token_to_scan == TOKEN_DOUBLE_CONSTANT) {
    BrigOperandImmed boi = {
      sizeof(boi),        // size
      BrigEOperandImmed,  // kind
      Brigb64,            // type
      0                   // reserved
    };
    boi.bits.d = context->token_value.double_val;
    context->append_operand(&boi);

    return 0;
  } else if (context->token_to_scan == TOKEN_SINGLE_CONSTANT) {
    BrigOperandImmed boi = {
      sizeof(boi),        // size
      BrigEOperandImmed,  // kind
      Brigb32,            // type
      0                   // reserved
    };
    boi.bits.f = context->token_value.float_val;
    context->append_operand(&boi);
    return 0;
  } else if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
    BrigOperandImmed boi = {
      sizeof(boi),        // size
      BrigEOperandImmed,  // kind
      Brigb32,            // type
      0                   // reserved
    };
    // TODO(Huy): check context for operation type and decide the type
    boi.bits.u = context->token_value.int_val;
    context->append_operand(&boi);

    return 0;
  } else if (context->token_to_scan == TOKEN_WAVESIZE) {
    return 0;  // currently not supported
  } else if (context->token_to_scan == '-') {
    if (yylex() == TOKEN_INTEGER_CONSTANT) {
      BrigOperandImmed boi = {
      sizeof(boi),        // size
      BrigEOperandImmed,  // kind
      Brigb32,            // type
      0                   // reserved
      };
      // TODO(Huy): check context for operation type and decide the type
      boi.bits.u = -context->token_value.int_val;
      context->append_operand(&boi);

     return 0;
    }
  } else if (context->token_type == DATA_TYPE_ID) {
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
        boi.bits.u = -context->token_value.int_val;
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
              boi.bits.u = context->token_value.int_val;
              context->append_operand(&boi);

              next = yylex();
              if (next == ')') {
                return 0;
              } else if (next != ',') {
                context->set_error(ErrorReporterInterface::MISSING_COMMA);
                return 1;
              }
            } else {
              context->set_error(ErrorReporterInterface::
                                    MISSING_INTEGER_CONSTANT);
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
        boi.bits.d = context->token_value.double_val;
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
              boi.bits.d = context->token_value.double_val;
              context->append_operand(&boi);
              next = yylex();
              if (next == ')') {
                return 0;
              } else if (next != ',') {
                context->set_error(ErrorReporterInterface::MISSING_COMMA);
                return 1;
              }
            } else {
              context->set_error(ErrorReporterInterface::
                                  MISSING_DOUBLE_CONSTANT);
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

int AddressableOperand(Context* context) {
  int next;

  if (context->token_to_scan == '[') {
    // next should be a non register
    next = yylex();
    if ((next == TOKEN_GLOBAL_IDENTIFIER) ||
        (next == TOKEN_LOCAL_IDENTIFIER)) {
      std::string name(context->token_value.string_val);
      // free memory
      free(context->token_value.string_val);

      next = yylex();
      if (next == ']') {
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
      } else if (next == '<') {
        next = yylex();
        if (next == TOKEN_INTEGER_CONSTANT) {
          if (yylex() == '>') {
            if (yylex() == ']') {
              context->token_to_scan = yylex();
              return 0;
            } else {
              context->set_error(ErrorReporterInterface::
                                  MISSING_CLOSING_BRACKET);
            }
          } else {
            context->set_error(ErrorReporterInterface::MISSING_CLOSING_BRACKET);
          }
        } else if (context->token_type == REGISTER) {
          next = yylex();
          if (next == '>') {
            if (yylex() == ']') {
              context->token_to_scan = yylex();
              return 0;
            } else {
              context->set_error(ErrorReporterInterface::
                                  MISSING_CLOSING_BRACKET);
            }
          } else if ((next == '+') || (next == '-')) {
            if (yylex() == TOKEN_INTEGER_CONSTANT) {
              if (yylex() == '>') {
                if (yylex() == ']') {
                  context->token_to_scan = yylex();
                  return 0;
                } else {
                  context->set_error(ErrorReporterInterface::
                                        MISSING_CLOSING_BRACKET);
                }
              } else {
                context->set_error(ErrorReporterInterface::
                                      MISSING_CLOSING_BRACKET);
              }
            } else {
              context->set_error(ErrorReporterInterface::
                                  MISSING_INTEGER_CONSTANT);
            }
          } else {
            context->set_error(ErrorReporterInterface::MISSING_CLOSING_BRACKET);
          }
        }
      }
    } else {
      context->set_error(ErrorReporterInterface::MISSING_IDENTIFIER);
    }
  }
  return 1;
}

int ArrayOperandList(Context* context) {
  // assumed first_token is '('
  int next;
  while (1) {
    next = yylex();
    context->token_to_scan = next;  // set context for Identifier()
    if (!Identifier(context)) {
      // need to free string
      free(context->token_value.string_val);
      next = yylex();
      if (next == ')') {
        // set context for following functions
        context->token_to_scan = yylex();
        return 0;
      } else if (next == ',') {
      } else {
        context->set_error(ErrorReporterInterface::MISSING_CLOSING_PARENTHESIS);
        return 1;
      }
    } else {
      context->set_error(ErrorReporterInterface::MISSING_IDENTIFIER);
      return 1;
    }
  }
}

int CallTargets(Context* context) {
  // assumed first_token is '['

  int next;
  while (1) {
    next = yylex();
    context->token_to_scan = next;  // set context for Identifier()
    if (!Identifier(context)) {
      // need to free string
      free(context->token_value.string_val);
      next = yylex();
      if (next == ']') {
        context->token_to_scan = yylex();  // set context for following function
        return 0;
      } else if (next == ',') {
      } else {
        context->set_error(ErrorReporterInterface::MISSING_CLOSING_BRACKET);
        return 1;
      }
    } else {
      context->set_error(ErrorReporterInterface::MISSING_IDENTIFIER);
      return 1;
    }
  }
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
      // string is freed in Operand()
    }

    if (context->token_to_scan == ')') {

      size_t listSize = sizeof(BrigOperandArgumentList);
      if(n_elements > 1)
        listSize += sizeof(BrigoOffset32_t) * (n_elements - 1);

      char array[listSize];
      BrigOperandArgumentList &arg_list =
        *reinterpret_cast<BrigOperandArgumentList *>(array);
      arg_list.size = listSize;
      arg_list.kind = BrigEOperandArgumentList;
      arg_list.elementCount = n_elements;
      for(uint32_t i = 0; i < n_elements; ++i) {
        arg_list.o_args[i] = arg_offset[i];
      }
      context->current_argList_offset = context->get_operand_offset();
      context->append_operand<BrigOperandArgumentList>(&arg_list);
      context->token_to_scan = yylex();

      return 0;
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
      context->set_error(ErrorReporterInterface::MISSING_CLOSING_PARENTHESIS);
      return 1;
    }
  }
}

int RoundingMode(Context* context) {
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
        yylex();
      }

      // now we must have a dataTypeId
      if (context->token_type == DATA_TYPE_ID) {
        // check the operands
        inst_op.type = context->token_value.data_type;

        context->token_to_scan = yylex();  // set context for Operand()
        std::string oper_str = context->token_value.string_val;
        // string_val will be freed in Operand()

        if (!Operand(context)) {
          inst_op.o_operands[0] = context->operand_map[oper_str];
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();  // set context for Operand()
            oper_str = context->token_value.string_val;
            // string_val will be freed in Operand()

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
                context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
              }

            } else {
              context->set_error(ErrorReporterInterface::MISSING_OPERAND);
            }

          } else {
            context->set_error(ErrorReporterInterface::MISSING_COMMA);
          }
        } else {
          context->set_error(ErrorReporterInterface::MISSING_OPERAND);
        }
      } else {
        context->set_error(ErrorReporterInterface::MISSING_DATA_TYPE);
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

        context->token_to_scan = yylex();  // set context for Operand()
        std::string oper_str = context->token_value.string_val;
        // string_val will be freed in Operand()

        if (!Operand(context)) {
          inst_op.o_operands[0] = context->operand_map[oper_str];
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();  // set context for Operand()
            oper_str = context->token_value.string_val;
            // string_val will be freed in Operand()
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
                context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
              }

            } else {
              context->set_error(ErrorReporterInterface::MISSING_OPERAND);
            }

          } else {
            context->set_error(ErrorReporterInterface::MISSING_COMMA);
          }
        } else {
          context->set_error(ErrorReporterInterface::MISSING_OPERAND);
        }
      } else {
        context->set_error(ErrorReporterInterface::MISSING_DATA_TYPE);
      }
      return 1;
    }
  } else if (context->token_type == INSTRUCTION2_OPCODE_NODT) {
    context->token_to_scan = yylex();  // set context for RoundingMode
    if (!RoundingMode(context)) {
    }

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
            context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
          }
        } else {
          context->set_error(ErrorReporterInterface::MISSING_OPERAND);
        }
      } else {
        context->set_error(ErrorReporterInterface::MISSING_COMMA);
      }
    } else {
      context->set_error(ErrorReporterInterface::MISSING_OPERAND);
    }
    return 1;
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
        // string_val will be freed in Operand()

        if (!Operand(context)) {
          inst_op.o_operands[0] = context->operand_map[oper_str];
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();  // set context for Operand()
            oper_str = context->token_value.string_val;
            // string_val will be freed in Operand()
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
                context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
              }
            } else {
              context->set_error(ErrorReporterInterface::MISSING_OPERAND);
            }
          } else {
            context->set_error(ErrorReporterInterface::MISSING_COMMA);
          }
        } else {
          context->set_error(ErrorReporterInterface::MISSING_OPERAND);
        }
      } else {
        context->set_error(ErrorReporterInterface::MISSING_DATA_TYPE);
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
        // string_val will be freed in Operand()

        if (!Operand(context)) {
          inst_op.o_operands[0] = context->operand_map[oper_str];
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();  // set context for Operand()
            oper_str = context->token_value.string_val;
            // string_val will be freed in Operand()

            if (!Operand(context)) {
              inst_op.o_operands[1] = context->operand_map[oper_str];
              if (context->token_to_scan == ';') {
                context->append_code<BrigInstBase>(&inst_op);
                // if the rule is valid, just write to the .code section,
                // may need to edit others, worry that later.
                context->token_to_scan = yylex();  // set context for later
                return 0;
              } else {
                context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
              }
            } else {
              context->set_error(ErrorReporterInterface::MISSING_OPERAND);
            }
          } else {
            context->set_error(ErrorReporterInterface::MISSING_COMMA);
          }
        } else {
          context->set_error(ErrorReporterInterface::MISSING_OPERAND);
        }
      } else {
        context->set_error(ErrorReporterInterface::MISSING_DATA_TYPE);
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
      // string_val will be freed in Operand()
      if (!Operand(context)) {
        inst_op.o_operands[0] = context->operand_map[oper_str];
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
          oper_str = context->token_value.string_val;
          // string_val will be freed in Operand()
          if (!Operand(context)) {
            inst_op.o_operands[1] = context->operand_map[oper_str];
            if (context->token_to_scan == ',') {
              context->token_to_scan = yylex();
              oper_str = context->token_value.string_val;
              // string_val will be freed in Operand()
              if (!Operand(context)) {
                inst_op.o_operands[2] = context->operand_map[oper_str];
                if (context->token_to_scan == ';') {
                  context->append_code<BrigInstBase>(&inst_op);
                  context->token_to_scan = yylex();
                  return 0;
                } else {
                  context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
                }
              } else {  // 3rd operand
                context->set_error(ErrorReporterInterface::MISSING_OPERAND);
              }
            } else {  // 2nd comma
              context->set_error(ErrorReporterInterface::MISSING_COMMA);
            }
          } else {  // 2nd operand
            context->set_error(ErrorReporterInterface::MISSING_OPERAND);
          }
        } else {  // 1st comma
          context->set_error(ErrorReporterInterface::MISSING_COMMA);
        }
      } else {  // 1st operand
        context->set_error(ErrorReporterInterface::MISSING_OPERAND);
      }
    } else {  // DATA_TYPE_ID
      context->set_error(ErrorReporterInterface::MISSING_DATA_TYPE);
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
      yylex();
    }
    // now we must have a dataTypeId
    if (context->token_type == DATA_TYPE_ID) {
      // check the operands
      inst_op.type = context->token_value.data_type;

      context->token_to_scan = yylex();
      std::string oper_str = context->token_value.string_val;
      // string_val will be freed in Operand()

      if (!Operand(context)) {
        inst_op.o_operands[0] = context->operand_map[oper_str];
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
          oper_str = context->token_value.string_val;
          // string_val will be freed in Operand()

          if (!Operand(context)) {
            inst_op.o_operands[1] = context->operand_map[oper_str];
            if (context->token_to_scan == ',') {
              context->token_to_scan = yylex();
              oper_str = context->token_value.string_val;
              // string_val will be freed in Operand()
              if (!Operand(context)) {
                inst_op.o_operands[2] = context->operand_map[oper_str];
                if (context->token_to_scan == ';') {
                  context->append_code<BrigInstBase>(&inst_op);
                  context->token_to_scan = yylex();
                  return 0;
                } else {
                  context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
                }
              } else {  // 3rd operand
                context->set_error(ErrorReporterInterface::MISSING_OPERAND);
              }
            } else {  // 2nd comma
              context->set_error(ErrorReporterInterface::MISSING_COMMA);
            }
          } else {  // 2nd operand
            context->set_error(ErrorReporterInterface::MISSING_OPERAND);
          }
        } else {  // 1st comma
          context->set_error(ErrorReporterInterface::MISSING_COMMA);
        }
      } else {  // 1st operand
        context->set_error(ErrorReporterInterface::MISSING_OPERAND);
      }
    } else {  // DATA_TYPE_ID
      context->set_error(ErrorReporterInterface::MISSING_DATA_TYPE);
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
  if (yylex() == TOKEN_INTEGER_CONSTANT) {
    bdv.major = context->token_value.int_val;
    if (yylex() == ':') {
      // check for minor
      if (yylex() == TOKEN_INTEGER_CONSTANT) {
        bdv.minor = context->token_value.int_val;
        int next = yylex();

        if (next == ';') {
        } else if (next == ':') {
          // check for target
          next = yylex();
          while (next != ';') {
            if (context->token_type == TARGET) {
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

              // update context
              context->set_machine(bdv.machine);
              context->set_profile(bdv.profile);
              context->set_ftz(bdv.ftz);

              next = yylex();
              if (next == ',') {
                next = yylex();      // next target
              } else {
                if (next != ';') {
                  context->set_error(
                    ErrorReporterInterface::MISSING_SEMICOLON);
                  return 1;
                }
              }
            } else {
              context->set_error(
                ErrorReporterInterface::INVALID_TARGET);
              return 1;
            }
          }
        } else {
          context->set_error(
            ErrorReporterInterface::MISSING_SEMICOLON);
          return 1;
        }
        context->append_directive(&bdv);
        context->token_to_scan = yylex();
        context->set_error(ErrorReporterInterface::OK);
        return 0;
      } else {
      context->set_error(
        ErrorReporterInterface::MISSING_INTEGER_CONSTANT);
      }
    } else {
    context->set_error(ErrorReporterInterface::MISSING_COLON);
    }
  } else {
    context->set_error(ErrorReporterInterface::MISSING_INTEGER_CONSTANT);
    return 1;
  }
};

int Alignment(Context* context) {
  // first token must be "align" keyword
  if (yylex() == TOKEN_INTEGER_CONSTANT) {
    context->set_alignment(context->token_value.int_val);
    context->token_to_scan = yylex();
    return 0;
  } else {
    context->set_error(ErrorReporterInterface::MISSING_INTEGER_CONSTANT);
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

        if ((context->token_to_scan == EXTERN)||(context->token_to_scan == STATIC)) {
          if (context->token_to_scan == EXTERN)
            context->set_attribute(BrigExtern);
          else
            context->set_attribute(BrigStatic);

          // alignment const externOrStatic
          context->token_to_scan = yylex();
        }
      } else if ((context->token_to_scan == EXTERN)||(context->token_to_scan == STATIC)) {
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
      context->set_error(ErrorReporterInterface::INVALID_ALIGNMENT);
    }
  } else if (context->token_to_scan == CONST) {
    // first is const
    context->set_symbol_modifier(BrigConst);
    context->token_to_scan = yylex();

    if (context->token_to_scan == ALIGN) {
      if (!Alignment(context)) {
        // const alignment
       if ((context->token_to_scan == EXTERN)||(context->token_to_scan == STATIC)) {
         // const alignment externOrStatic
         if (context->token_to_scan == EXTERN)
            context->set_attribute(BrigExtern);
          else
            context->set_attribute(BrigStatic);

          context->token_to_scan = yylex();
        }
      } else {
        context->set_error(ErrorReporterInterface::INVALID_ALIGNMENT);
      }
    } else if ((context->token_to_scan == EXTERN)||(context->token_to_scan == STATIC)) {
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
          context->set_error(ErrorReporterInterface::INVALID_ALIGNMENT);
          return 1;
        }
      }
    } else {  // const does not stand alone
      return 1;
    }
  } else if ((context->token_to_scan == EXTERN)||(context->token_to_scan == STATIC)) {
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
        context->set_error(ErrorReporterInterface::INVALID_ALIGNMENT);
      }
    } else if (context->token_to_scan == CONST) {
      // externOrStatic const
      context->set_symbol_modifier(BrigConst);

      context->token_to_scan = yylex();

      if (context->token_to_scan == ALIGN) {
        if (!Alignment(context)) {
        } else {
          context->set_error(ErrorReporterInterface::INVALID_ALIGNMENT);
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

  if (yylex() == '(') {
    if (yylex() == TOKEN_INTEGER_CONSTANT) {
      context->set_fbar(context->token_value.int_val);
      if (yylex() == ')') {
        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(ErrorReporterInterface::MISSING_CLOSING_PARENTHESIS);
      }
    } else {
      context->set_error(ErrorReporterInterface::MISSING_INTEGER_CONSTANT);
    }
  }
  return 1;
}

int ArrayDimensionSet(Context* context) {
  // first token must be '['

  unsigned int next_token = yylex();

  while (1) {
    if (next_token == ']') {
      next_token = yylex();  // check if there is more item
      if (next_token == '[') {  // more item
        next_token = yylex();
      } else {  // no more item
        context->token_to_scan = next_token;
        return 0;
      }
    } else if (next_token == TOKEN_INTEGER_CONSTANT) {
      next_token = yylex();  // scan next
    } else {
      context->set_error(ErrorReporterInterface:: MISSING_CLOSING_BRACKET);
      return 1;
    }
  }
}

int ArgumentDecl(Context* context) {
  unsigned int next;

  if (!DeclPrefix(context)) {
    next = yylex();  // skip over "arg" in context->token_to_scan
    if ((context->token_type == DATA_TYPE_ID)||
        (next == _RWIMG) ||
        (next == _SAMP) ||
        (next == _ROIMG)) {
      context->set_type(context->token_value.data_type);
      next = yylex();
      if (next == TOKEN_LOCAL_IDENTIFIER) {
        // should have a meaning for DATA_TYPE_ID.
        // for argument, we need to set a BrigDirectiveSymbol
        // and write the corresponding string into .string section.


        std::string arg_name = context->token_value.string_val;
        // free memory
        free(context->token_value.string_val);


        int arg_name_offset = context->add_symbol(arg_name);
        // scan for arrayDimensions
        next = yylex();
        if (next == '[') {
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
          context->get_code_offset(),       // c_code
          BrigArgSpace,                     // storageClass
          context->get_attribute(),         // attribute
          0,                                // reserved
          context->get_symbol_modifier(),   // symbol modifier
          0,                                // dim
          arg_name_offset,                  // s_name
          context->token_value.data_type,                        // data type
          context->get_alignment(),         // alignment
          0,                                // d_init = 0 for arg
          0                                 // reserved
          };
          // append the DirectiveSymbol to .directive section.
          context->append_directive_symbol(&sym_decl);
          context->symbol_map[arg_name] = dsize;

          // update the current DirectiveFunction.
          // 1. update the directive offset.
          BrigDirectiveFunction bdf;
          context->get_directive(context->current_bdf_offset, &bdf);
          if (bdf.d_firstScopedDirective == bdf.d_nextDirective) {
            bdf.d_nextDirective += 36;
            bdf.d_firstScopedDirective = bdf.d_nextDirective;
          } else {
            bdf.d_nextDirective += 36;
          }
          // std::cout << bdf.d_nextDirective << std::endl;
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

          context->get_directive<BrigDirectiveFunction>(
                    context->current_bdf_offset,
                    &bdf);
          // std::cout << bdf.size << std::endl;


          context->token_to_scan = next;
          return 0;
        }
      } else {
        context->set_error(ErrorReporterInterface:: MISSING_IDENTIFIER);
      }
    } else {
      context->set_error(ErrorReporterInterface:: MISSING_DATA_TYPE);
    }
  }
  context->set_error(ErrorReporterInterface::MISSING_DECLPREFIX);
  return 1;
}

int ArgumentListBody(Context* context) {
  while (1) {
    if (!ArgumentDecl(context)) {
      if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
      } else {
        return 0;  // context was set in ArgumentDecl
      }
    } else {
      context->set_error(ErrorReporterInterface:: MISSING_ARGUMENT);
      return 1;
    }
  }
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

      if (yylex() == TOKEN_GLOBAL_IDENTIFIER) {
        // should have meaning of Global_Identifier,
        // and check if there is existing global identifier
        // if there is, just use the current string,
        // if not write into string.

        std::string func_name = context->token_value.string_val;
        // free memory
        free(context->token_value.string_val);


        BrigsOffset32_t check_result = context->add_symbol(func_name);

        // add the func_name to the func_map.
        context->func_map[func_name] = context->current_bdf_offset;

        bdf.s_name = check_result;
        context->append_directive(&bdf);

        /* Debug */
        // BrigDirectiveFunction get;
        // context->get_d<BrigDirectiveFunction>(bdf_offset, &get);
        // std::cout << get.s_name << std::endl;

        // check return argument list
        if (yylex() == '(') {
          context->set_arg_output(true);
          context->token_to_scan = yylex();

          if (context->token_to_scan == ')') {   // empty argument list body
            context->token_to_scan = yylex();
          } else if (!ArgumentListBody(context)) {
            if (context->token_to_scan == ')') {
              context->token_to_scan = yylex();
            } else {
              context->set_error(ErrorReporterInterface::
                                    MISSING_CLOSING_PARENTHESIS);
              return 1;
            }
          } else {
            context->set_error(ErrorReporterInterface:: INVALID_ARGUMENT_LIST);
            return 1;
          }
        } else {
          context->set_error(ErrorReporterInterface:: MISSING_ARGUMENT_LIST);
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
              context->set_error(ErrorReporterInterface::
                                    MISSING_CLOSING_PARENTHESIS);
              return 1;
            }
          } else {
            context->set_error(ErrorReporterInterface:: INVALID_ARGUMENT_LIST);
            return 1;
          }
        } else {
          context->set_error(ErrorReporterInterface:: MISSING_ARGUMENT_LIST);
          return 1;
        }
        // check for optional FBar
        if (context->token_to_scan == _FBAR) {
          if (!FBar(context)) {
            // context->token_to_scan = yylex();
            return 0;
          } else {
            context->set_error(ErrorReporterInterface:: INVALID_FBAR);
          }
        } else {
          return 0;
        }
      } else {
        context->set_error(ErrorReporterInterface:: MISSING_IDENTIFIER);
      }
    }
  }
  return 1;
}

int FunctionDecl(Context* context) {
  if (!DeclPrefix(context)) {
    if (context->token_to_scan == FUNCTION) {
      if (yylex() == TOKEN_GLOBAL_IDENTIFIER) {

        // free string
        free(context->token_value.string_val);
        // check return argument list
        if (yylex() == '(') {
          context->token_to_scan = yylex();

          if (context->token_to_scan == ')') {   // empty argument list body
              context->token_to_scan = yylex();
          } else if (!ArgumentListBody(context)) {
            if (context->token_to_scan == ')') {
                context->token_to_scan = yylex();
            } else {
              context->set_error(ErrorReporterInterface::
                                    MISSING_CLOSING_PARENTHESIS);
              return 1;
            }
          } else {
            context->set_error(ErrorReporterInterface:: INVALID_ARGUMENT_LIST);
            return 1;
          }
        } else {
          context->set_error(ErrorReporterInterface:: MISSING_ARGUMENT_LIST);
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
              context->set_error(ErrorReporterInterface::
                                    MISSING_CLOSING_PARENTHESIS);
              return 1;
            }
          } else {
            context->set_error(ErrorReporterInterface:: INVALID_ARGUMENT_LIST);
            return 1;
          }
        } else {
          context->set_error(ErrorReporterInterface:: MISSING_ARGUMENT_LIST);
          return 1;
        }


        // check for optional FBar
        if (context->token_to_scan == _FBAR) {
          if (!FBar(context)) {
          } else {
            context->set_error(ErrorReporterInterface:: INVALID_FBAR);
          }
        }
        if (context->token_to_scan == ';') {
          context->token_to_scan = yylex();
          return 0;
        } else {
          context->set_error(ErrorReporterInterface:: MISSING_SEMICOLON);
        }
      } else {
        context->set_error(ErrorReporterInterface:: MISSING_IDENTIFIER);
      }
    }
  } else {
    context->set_error(ErrorReporterInterface::MISSING_DECLPREFIX);
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
    } else if ((context->token_type == INSTRUCTION3_OPCODE) ||
               (context->token_type == INSTRUCTION3_OPCODE_FTZ)) {
      // Instruction 3 Operation
      if (!Instruction3(context)) {
        // update the operationCount.
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
    } else if (context->token_type == QUERY_OP) {  // Query Operation
      if (!Query(context)) {
      } else {
        return 1;
      }
    } else if (context->token_to_scan == RET) {  // ret operation
      if (yylex() == ';') {
        BrigcOffset32_t csize = context->get_code_offset();
        BrigInstBase op_ret = {
          32,
          BrigEInstBase,
          BrigRet,
          Brigb32,
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
        context->token_to_scan = yylex();
      } else {
        context->set_error(ErrorReporterInterface:: MISSING_SEMICOLON);
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

      // free string
      free(context->token_value.string_val);
      if (context->token_to_scan == ':') {
        context->token_to_scan = yylex();
      } else {
        context->set_error(ErrorReporterInterface:: MISSING_COLON);
        return 1;
      }
    } else if (context->token_to_scan == CALL) {  // call (only inside argblock
      if (!Call(context)) {
        // update the operationCount.
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
      context->set_error(ErrorReporterInterface::
                          INVALID_NESTED_ARGUMENT_SCOPE);
      return 1;
    } else if (context->token_to_scan == '}') {
      // add BrigDirectiveScope
      BrigDirectiveScope argblock_start = {
        8,
        BrigEDirectiveArgEnd,
        context->get_code_offset()
      };
      context->append_directive<BrigDirectiveScope>(&argblock_start);

      // update the d_nextDirective.
      BrigDirectiveFunction bdf;
      context->get_directive<BrigDirectiveFunction>(
                                        context->current_bdf_offset, &bdf);
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
    } else if (context->token_type == QUERY_OP) {  // Query Operation
      if (!Query(context)) {
      } else {
        return 1;
      }
    } else if (context->token_to_scan == RET) {  // ret operation
      if (yylex() == ';') {
      BrigcOffset32_t csize = context->get_code_offset();
      BrigInstBase op_ret = {
        32,
        BrigEInstBase,
        BrigRet,
        Brigb32,
        BrigNoPacking,
        {0, 0, 0, 0, 0}
        };
      // write to .code section
      context->append_code<BrigInstBase>(&op_ret);
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
        context->set_error(ErrorReporterInterface:: MISSING_SEMICOLON);
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
      // free memory
      free(context->token_value.string_val);


      BrigDirectiveLabel label_directive = {
        12,
        BrigEDirectiveLabel,
        context->get_code_offset(),
        context->add_symbol(label_name)
      };

      BrigdOffset32_t label_directive_offset = context->get_directive_offset();
      context->append_directive<BrigDirectiveLabel>(&label_directive);

      // add to the .operand section
      BrigoOffset32_t label_operand_offset = context->get_operand_offset();
      BrigOperandLabelRef label_operand = {
      8,
      BrigEOperandLabelRef,
      label_directive_offset
      };

      context->append_operand<BrigOperandLabelRef>(&label_operand);
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

      if (yylex() == ':') {
        context->token_to_scan = yylex();
      } else {
        context->set_error(ErrorReporterInterface:: MISSING_COLON);
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
        context->set_error(ErrorReporterInterface:: MISSING_SEMICOLON);
      }
    } else {
      context->set_error(ErrorReporterInterface::INVALID_CODEBLOCK);
    }

  } else {
    context->set_error(ErrorReporterInterface::INVALID_FUNCTION_DEFINITION);
  }
  return 1;
}

int Program(Context* context) {
  int result;

  if (context->token_to_scan == VERSION) {
    if (!Version(context)) {
      // parse topLevelStatement
      while (context->token_to_scan && (context->token_to_scan != VERSION)) {
        if ( (context->token_to_scan == ALIGN) ||
             (context->token_to_scan == CONST) ||
             (context->token_to_scan == EXTERN) ||
             (context->token_to_scan == STATIC) ) {
          result = DeclPrefix(context);
          if (result)
            return 1;
        }

        // Found "function" keyword ------------------------
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


          // look at next token
          if (yylex() == TOKEN_GLOBAL_IDENTIFIER) {
            std::string func_name = context->token_value.string_val;
            // free memory
            free(context->token_value.string_val);


            BrigsOffset32_t check_result = context->add_symbol(func_name);

            // add the func_name to the func_map.
            context->func_map[func_name] = context->current_bdf_offset;

            bdf.s_name = check_result;
            context->append_directive(&bdf);

            // check return argument list
            if (yylex() == '(') {
                context->set_arg_output(true);
                context->token_to_scan = yylex();

              if (context->token_to_scan == ')') {   // empty argument list body
                context->token_to_scan = yylex();
              } else if (!ArgumentListBody(context)) {
                if (context->token_to_scan == ')') {
                  context->token_to_scan = yylex();
                } else {
                  context->set_error(ErrorReporterInterface::
                                        MISSING_CLOSING_PARENTHESIS);
                  return 1;  // missing closing )
                }
              } else {
                context->set_error(ErrorReporterInterface::
                                      INVALID_ARGUMENT_LIST);
                return 1;
              }
            } else {  // missing '('
              context->set_error(ErrorReporterInterface::
                                  MISSING_ARGUMENT_LIST);
              return 1;
            }          // if found '(' - returnArgList

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
                  context->set_error(ErrorReporterInterface::
                                        MISSING_CLOSING_PARENTHESIS);
                  return 1;  // missing closing )
                }
              } else {
                context->set_error(ErrorReporterInterface::
                                      INVALID_ARGUMENT_LIST);
                return 1;
              }
            } else {  // missing '('
              context->set_error(ErrorReporterInterface::
                                  MISSING_ARGUMENT_LIST);
              return 1;
            }            // if found '(' - argList

            // check for optional FBar
            if (context->token_to_scan == _FBAR) {
              if (!FBar(context)) {
              } else {
                context->set_error(ErrorReporterInterface:: INVALID_FBAR);
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
                  context->set_error(ErrorReporterInterface::
                                      MISSING_SEMICOLON);
                  return 1;
                }
              } else {
                return 1;
              }
            } else {
              context->set_error(ErrorReporterInterface:: MISSING_SEMICOLON);
            }
          }       // if found TOKEN_GLOBAL_ID
        } else if (context->token_type == INITIALIZABLE_ADDRESS) {
          // global initializable
          // this is an initializable declaration
          if (!InitializableDecl(context)) {
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
    context->set_error(ErrorReporterInterface:: MISSING_VERSION_STATEMENT);
  }
  return 1;
}

int OptionalWidth(Context* context) {
  // first token must be _WIDTH

  unsigned int next_token = yylex();
  if (next_token == '(') {
    next_token = yylex();
    if (next_token == ALL) {
      next_token = yylex();
    } else if (next_token == TOKEN_INTEGER_CONSTANT) {
      next_token = yylex();
    } else {
      context->set_error(ErrorReporterInterface:: MISSING_WIDTH_INFO);
      return 1;
    }
    if (next_token == ')') {
      context->token_to_scan = yylex();
      return 0;
    } else {
      context->set_error(ErrorReporterInterface:: MISSING_CLOSING_PARENTHESIS);
    }
  } else {
    context->set_error(ErrorReporterInterface:: MISSING_WIDTH_INFO);
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

  BrigdOffset32_t current_offset = context->get_directive_offset();
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


    std::string operand_name = context->token_value.string_val;
    // string_val will be freed in Operand()

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
          // free memory
          free(context->token_value.string_val);


          if (context->label_o_map.count(label_name)) {
            inst_op.o_operands[2] = context->label_o_map[label_name];
          } else {
            context->label_c_map.insert(make_pair(
                                          label_name,
                                          context->get_code_offset()+20));
          }

          context->token_to_scan = yylex();  // should be ';'
        } else if (!Identifier(context)) {
          // need to free string
          free(context->token_value.string_val);
          context->token_to_scan = yylex();  // should be ';'
        } else if (!Operand(context)) {
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();

            if (context->token_to_scan == TOKEN_LABEL) {  // LABEL
                // free memory
                free(context->token_value.string_val);
                context->token_to_scan = yylex();  // should be ';'
            } else if (context->token_to_scan == '[') {
              context->token_to_scan = yylex();

              if (!Identifier(context)) {
                // need to free string
                free(context->token_value.string_val);
                context->token_to_scan = yylex();  // should be ']'
              } else if (context->token_to_scan == TOKEN_LABEL) {
                // free memory
                free(context->token_value.string_val);
                context->token_to_scan = yylex();

                while (context->token_to_scan != ']') {
                  if (context->token_to_scan == ',') {
                    if (yylex() == TOKEN_LABEL) {
                      // free memory
                      free(context->token_value.string_val);
                      context->token_to_scan = yylex();  // scan next;
                    } else {
                      context->set_error(ErrorReporterInterface::
                                          MISSING_OPERAND);
                      return 1;
                    }
                  } else {
                    context->set_error(ErrorReporterInterface::MISSING_COMMA);
                    return 1;
                  }
                }   // while
              } else {
                context->set_error(ErrorReporterInterface::MISSING_OPERAND);
              }
              // current token should be ']'
              context->token_to_scan = yylex();  // should be ';'
            } else {
              context->set_error(ErrorReporterInterface::MISSING_OPERAND);
            }
          }  else {  // yylex() = ','
            context->set_error(ErrorReporterInterface::MISSING_COMMA);
          }
        } else {
          context->set_error(ErrorReporterInterface::MISSING_OPERAND);
          return 1;
        }
        if (context->token_to_scan == ';') {
          context->append_code<BrigInstBase>(&inst_op);
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
          context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
        }
      } else {  // yylex = ','
        context->set_error(ErrorReporterInterface::MISSING_COMMA);
      }
    } else {  // first operand
      context->set_error(ErrorReporterInterface::MISSING_OPERAND);
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
      // free memory
      free(context->token_value.string_val);


      if (context->label_o_map.count(label_name)) {
        inst_op.o_operands[1] = context->label_o_map[label_name];
      } else {
        context->label_c_map.insert(make_pair(
                                      label_name,
                                      context->get_code_offset()+16));
      }
      context->token_to_scan = yylex();
      if (context->token_to_scan == ';') {
        context->append_code<BrigInstBar>(&inst_op);
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
        context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
      }
    } else if (!Identifier(context)) {
        // need to free string
        free(context->token_value.string_val);
        context->token_to_scan = yylex();

      if (context->token_to_scan == ';') {
        context->token_to_scan = yylex();
        return 0;
      } else if (context->token_to_scan == ',') {
        if (yylex() == '[') {
          context->token_to_scan = yylex();

        if (context->token_to_scan == TOKEN_LABEL) {
          // free memory
          free(context->token_value.string_val);
          context->token_to_scan = yylex();    // should be ']'
        } else if (!Identifier(context)) {
          // need to free string
          free(context->token_value.string_val);
          context->token_to_scan = yylex();    // should be ']'
          }
        }

        if (context->token_to_scan == ']') {
        } else {
          context->set_error(ErrorReporterInterface::MISSING_CLOSING_BRACKET);
        }

        context->token_to_scan = yylex();
        if (context->token_to_scan == ';') {
          context->token_to_scan = yylex();
          return 0;
        } else {
          context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
        }
      } else {
        context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
      }
    } else {
      context->set_error(ErrorReporterInterface::MISSING_OPERAND);
    }
    return 1;
  }
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
    // string_val will be freed in Operand()
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
        context->append_operand<BrigOperandFunctionRef>(&func_o_ref);
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
      context->append_code<BrigInstBase>(&call_op);
      context->token_to_scan = yylex();
      return 0;
    } else if (context->token_to_scan == '[') {
      if (!CallTargets(context)) {
        if (context->token_to_scan == ';') {
            context->token_to_scan = yylex();
           return 0;
        } else {
          context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
        }
      }
      return 1;
    } else {
      context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
    }
  } else {
    context->set_error(ErrorReporterInterface::MISSING_OPERAND);
  }
  return 1;
}

int Initializer(Context* context) {
  // first token should be '='
  context->token_to_scan = yylex();
  if (context->token_to_scan == TOKEN_LABEL) {
    // free memory
    free(context->token_value.string_val);
    context->set_error(ErrorReporterInterface::INVALID_INITIALIZER);
    return 1;
  } else if (context->token_to_scan == '{') {
    context->token_to_scan = yylex();
  }

  // check type of initializer
  if (context->token_to_scan == TOKEN_LABEL) {
    // free memory
    free(context->token_value.string_val);
    // label initializer
    while (1) {
      context->token_to_scan = yylex();
      if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
        if (context->token_to_scan == TOKEN_LABEL) {
          // free memory
          free(context->token_value.string_val);
          continue;
        } else {
          context->set_error(ErrorReporterInterface::MISSING_LABEL);
          return 1;
        }
      } else {
        break;
      }
    }  // while(1)

  } else if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
    // decimal initializer
    while (1) {
      context->token_to_scan = yylex();
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
          continue;
        } else {
          context->set_error(ErrorReporterInterface::MISSING_INTEGER_CONSTANT);
          return 1;
        }
      } else {
        break;
      }
    }  // while(1)
  } else if (context->token_to_scan == TOKEN_SINGLE_CONSTANT) {
    // single initializer
    while (1) {
      context->token_to_scan = yylex();
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        if (context->token_to_scan == TOKEN_SINGLE_CONSTANT) {
          continue;
        } else {
          context->set_error(ErrorReporterInterface::MISSING_SINGLE_CONSTANT);
          return 1;
        }
      } else {
        break;
      }
    }  // while(1)
  } else if (context->token_to_scan == TOKEN_DOUBLE_CONSTANT) {
    // double initializer
    while (1) {
      context->token_to_scan = yylex();
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        if (context->token_to_scan == TOKEN_DOUBLE_CONSTANT) {
          continue;
        } else {
          context->set_error(ErrorReporterInterface::MISSING_DOUBLE_CONSTANT);
          return 1;
        }
      } else {
        break;
      }
    }  // while(1)
  } else {
    context->set_error(ErrorReporterInterface::INVALID_INITIALIZER);
    return 1;
  }
  if (context->token_to_scan == '}') {
    context->token_to_scan = yylex();
  }
  return 0;
}

int InitializableDecl(Context* context) {
  // first_token is READONLY or GLOBAL
  context->token_to_scan = yylex();

  if (context->token_type == DATA_TYPE_ID) {
    context->token_to_scan = yylex();

    if (!Identifier(context)) {
      // need to free string
      free(context->token_value.string_val);

      // scan for arrayDimensions
      context->token_to_scan = yylex();
      if (context->token_to_scan == '[') {
        if (!ArrayDimensionSet(context)) {
        }
      }

      if (!Initializer(context)) {
        if (context->token_to_scan == ';') {
            context->token_to_scan = yylex();
          return 0;
        } else {
          context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
        }
      }
    } else {
      context->set_error(ErrorReporterInterface::MISSING_IDENTIFIER);
    }
  } else {
    context->set_error(ErrorReporterInterface::MISSING_DATA_TYPE);
  }
  return 1;
};

int UninitializableDecl(Context* context) {
  // first_token is PRIVATE, GROUP or SPILL

  context->token_to_scan = yylex();
  if (context->token_type == DATA_TYPE_ID) {
    context->token_to_scan = yylex();

    if (!Identifier(context)) {
      // need to free string
      free(context->token_value.string_val);

      // scan for arrayDimensions
      context->token_to_scan = yylex();
      if (context->token_to_scan == '[') {
        if (!ArrayDimensionSet(context)) {
        }
      }

      if (context->token_to_scan == ';') {
        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
      }
    } else {
      context->set_error(ErrorReporterInterface::MISSING_IDENTIFIER);
    }
  } else {
    context->set_error(ErrorReporterInterface::MISSING_DATA_TYPE);
  }
  return 1;
}

int ArgUninitializableDecl(Context* context) {
  // first token is ARG
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
      // free memory
      free(context->token_value.string_val);


      BrigDirectiveSymbol arg_decl = {
        sizeof(arg_decl),                 // size
        BrigEDirectiveSymbol,             // kind
        context->get_code_offset(),       // c_code
        BrigArgSpace,                     // storageClass
        context->get_attribute(),         // attribute
        0,                                // reserved
        context->get_symbol_modifier(),   // symbol modifier
        0,                                // dim
        context->add_symbol(arg_name),    // s_name
        data_type,   // data_type
        context->get_alignment(),         // alignment
        0,                                // d_init = 0 for arg
        0                                 // reserved
      };

      BrigOperandArgumentRef arg_ref = {
        8,
        BrigEOperandArgumentRef,
        context->get_directive_offset()
      };

      context->symbol_map[arg_name]= context->get_operand_offset();
      context->append_directive_symbol(&arg_decl);
      // add the operand to the map.
      context->arg_map[arg_name] = context->get_operand_offset()          ;
      context->append_operand<BrigOperandArgumentRef>(&arg_ref);

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
        context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
      }
    } else {
      context->set_error(ErrorReporterInterface::MISSING_IDENTIFIER);
    }
  } else {
    context->set_error(ErrorReporterInterface::MISSING_DATA_TYPE);
  }
  return 1;
}

int FileDecl(Context* context) {
  // first token is _FILE "file"
  context->token_to_scan = yylex();

  if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
    context->token_to_scan = yylex();

    if (context->token_to_scan == TOKEN_STRING) {
      // free string
      free(context->token_value.string_val);
      context->token_to_scan = yylex();

      if (context->token_to_scan == ';') {
        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
      }
    }
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
  // to keep a copy since calling Operand will update the token
  unsigned int current_token = context->token_to_scan;
  if (!Operand(context) &&
      current_token == TOKEN_SREGISTER &&
      context->token_to_scan == ',') {
    context->token_to_scan = yylex();
    // to keep a copy since calling Operand will update the token
    current_token = context->token_to_scan;
    if (!BaseOperand(context) &&
        current_token == TOKEN_INTEGER_CONSTANT &&
        yylex() == ',') {
      context->token_to_scan = yylex();
      current_token = context->token_to_scan;
      if ((!Operand(context)) &&
          ((current_token == TOKEN_SREGISTER) ||
           (current_token == TOKEN_WAVESIZE) ||
           (current_token == TOKEN_INTEGER_CONSTANT)) &&
           (context->token_to_scan == ',')) {
        context->token_to_scan = yylex();
        current_token = context->token_to_scan;
        if ((!Operand(context)) &&
            ((current_token == TOKEN_SREGISTER) ||
             (current_token == TOKEN_WAVESIZE) ||
             (current_token == TOKEN_INTEGER_CONSTANT)) &&
             (context->token_to_scan == ',')) {
          context->token_to_scan = yylex();
          current_token = context->token_to_scan;

          if ((!Operand(context)) &&
              ((current_token == TOKEN_SREGISTER) ||
               (current_token == TOKEN_WAVESIZE) ||
               (current_token == TOKEN_INTEGER_CONSTANT))) {
            if (context->token_to_scan == ';') {
              context->token_to_scan = yylex();
              return 0;
            }  // ';'
          }  // 5 operand
        }  // 4 operand
      }  // 3 operand
    }  // 2 base operand
  }  // 1 operand
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
      context->set_error(ErrorReporterInterface::MISSING_ARGUMENT_LIST);
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
    // free memory
    free(context->token_value.string_val);
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
          context->set_error(ErrorReporterInterface::
                              MISSING_CLOSING_PARENTHESIS);
      }
    } else {
      context->set_error(ErrorReporterInterface::UNKNOWN_ERROR);
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
          context->set_error(ErrorReporterInterface::
                              MISSING_CLOSING_PARENTHESIS);
      }
    } else {
      context->set_error(ErrorReporterInterface::UNKNOWN_ERROR);
      return 1;
    }

    if (_FBAR == context->token_to_scan) {
      if (!FBar(context)) {
      } else {
        context->set_error(ErrorReporterInterface::INVALID_FBAR);
        return 1;
      }
    }

    if (';' == context->token_to_scan) {  // end with ;
      context->token_to_scan = yylex();
      return 0;
    } else {
      context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
      return 1;
    }
  }
}

int Label(Context* context) {
  if (context->token_to_scan == TOKEN_LABEL) {
    // free memory
    free(context->token_value.string_val);

    if (yylex() == ':') {
      context->token_to_scan = yylex();
      return 0;
    }
  }

  return 1;
}

int LabelTargets(Context* context) {
  if (!Label(context)) {
    if (context->token_to_scan == LABELTARGETS) {
      while (1) {
        if (yylex() == TOKEN_LABEL) {
          // should get string_val here and free it
          free(context->token_value.string_val);


          context->token_to_scan = yylex();
          if (context->token_to_scan == ',') {
            continue;
          } else if (context->token_to_scan == ';') {
            context->token_to_scan = yylex();
            return 0;
          } else {
            // there is an illegal character behind TOKEN_LABEL
            return 1;
          }
        } else {
          // it isn't TOKEN_LABEL behind LABELTARGETS or ','
          return 1;
        }
      }
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
      if (!Operand(context) && context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        if (!Operand(context) && context->token_to_scan == ',') {
          context->token_to_scan = yylex();
          if (!Operand(context) && context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if (!Operand(context) && context->token_to_scan == ';') {
              context->token_to_scan = yylex();
              return 0;
            }  // 4 operand
          }  // 3 operand
        }  // 2 operand
      }  // 1 operand
    }  // DATA_TYPE_ID
  }  // INSTRUCTION4_OPCODE
  return 1;
}

int Extension(Context* context) {
  // first token is EXTENSION "extension"
  context->token_to_scan = yylex();

  if (context->token_to_scan == TOKEN_STRING) {
    context->token_to_scan = yylex();

    // free string
    free(context->token_value.string_val);
    if (context->token_to_scan == ';') {
      context->token_to_scan = yylex();
      return 0;
    } else {   // missing ";"
			context->set_error(ErrorReporterInterface::MISSING_SEMICOLON);
    }
  }
  return 1;
}
}  // namespace brig
}  // namespace hsa
