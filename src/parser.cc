/* Copyright 2012 <MulticorewareInc> */

#include "parser.h"
#include <string>
#include <map>
#include "tokens.h"
#include "lexer.h"
#include "error_reporter_interface.h"


// variables returned by lexer
extern int int_val;
extern char* string_val;
extern float float_val;
extern double double_val;
extern TerminalType token_type;
extern BrigDataType16_t data_type;
extern BrigPacking16_t  packing;
extern BrigOpcode32_t   opcode;
extern int yycolno;


namespace hsa {
namespace brig {
int Query(unsigned int QueryOp, Context* context) {
  // next token should be a dataTypeId
  ErrorReporterInterface* rpt = context->get_error_reporter();
  int next = yylex();
  if (token_type == DATA_TYPE_ID) {
    // next token should be an Operand
    if (!Operand(yylex(), context)) {
      // next should be a comma
      if (yylex() == ',') {
        // then finally an addressable Operand
        if (!AddressableOperand(yylex(), context)) {
          if (yylex() == ';')
            return 0;
          else
            rpt->report_error(ErrorReporterInterface::MISSING_SEMICOLON,
                              yylineno,
                              yycolno);
        } else {
          rpt->report_error(ErrorReporterInterface::MISSING_COMMA,
                            yylineno,
                            yycolno);
        }
      } else {
        rpt->report_error(ErrorReporterInterface::MISSING_COMMA,
                          yylineno,
                          yycolno);
      }
    } else {
      rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                        yylineno,
                        yycolno);
    }
  } else {
    rpt->report_error(ErrorReporterInterface::MISSING_DATA_TYPE,
                      yylineno,
                      yycolno);
  }
  return 1;
}

int Operand(unsigned int first_token, Context* context) {
  if (!Identifier(first_token, context)) {  // an Identifier
    if (token_type == REGISTER) {
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

      if (!context->operand_map.count(name)) {
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
  } else if (token_type == REGISTER) {
    return 0;
  }
  return 1;
}

int BaseOperand(unsigned int first_token, Context* context) {
  int next;
  ErrorReporterInterface* rpt = context->get_error_reporter();
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
  } else if (token_type == DATA_TYPE_ID) {
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
              if (next == ')') {
                return 0;
              } else if (next != ',') {
                rpt->report_error(ErrorReporterInterface::MISSING_COMMA,
                                  yylineno,
                                  yycolno);
                return 1;
              }
            } else {
              rpt->report_error(ErrorReporterInterface::
                                  MISSING_INTEGER_CONSTANT,
                                yylineno,
                                yycolno);
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
              if (next == ')') {
                return 0;
              } else if (next != ',') {
                rpt->report_error(ErrorReporterInterface::MISSING_COMMA,
                                  yylineno,
                                  yycolno);
                return 1;
              }
            } else {
              rpt->report_error(ErrorReporterInterface::MISSING_DOUBLE_CONSTANT,
                                yylineno,
                                yycolno);
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
  ErrorReporterInterface* rpt = context->get_error_reporter();

  if (first_token == '[') {
    // next should be a non register
    next = yylex();
    if ((next == TOKEN_GLOBAL_IDENTIFIER) ||
        (next == TOKEN_LOCAL_IDENTIFIER)) {
      next = yylex();
      std::string name(string_val);
      if (next == ']') {
        BrigOperandAddress boa = {
          sizeof(boa),            // size
          BrigEOperandAddress,    // kind
          Brigb32,                // type
          0,                      // reserved
          0,                      // directive
          0
          };
  // TODO(Huy): Fill in directive field with the offset to BrigDirectiveSymbol
        if (context->get_machine() == BrigELarge)
          boa.type = Brigb64;

        context->append_operand(&boa);

        return 0;
      } else if (next == '<') {
        next = yylex();
        if (next == TOKEN_INTEGER_CONSTANT) {
          if (yylex() == '>') {
            if (yylex() == ']')
              return 0;
            else
              rpt->report_error(ErrorReporterInterface::MISSING_CLOSING_BRACKET,
                              yylineno,
                              yycolno);
          } else {
            rpt->report_error(ErrorReporterInterface::MISSING_CLOSING_BRACKET,
                              yylineno,
                              yycolno);
          }
        } else if (token_type == REGISTER) {
          next = yylex();
          if (next == '>') {
            if (yylex() == ']')
              return 0;
          } else if ((next == '+') || (next == '-')) {
            if (yylex() == TOKEN_INTEGER_CONSTANT) {
              if (yylex() == '>') {
                if (yylex() == ']')
                  return 0;
                else
                  rpt->report_error(ErrorReporterInterface::
                                        MISSING_CLOSING_BRACKET,
                                    yylineno,
                                    yycolno);
              } else {
                rpt->report_error(ErrorReporterInterface::
                                      MISSING_CLOSING_BRACKET,
                                  yylineno,
                                  yycolno);
              }
            } else {
              rpt->report_error(ErrorReporterInterface::
                                  MISSING_INTEGER_CONSTANT,
                                yylineno,
                                yycolno);
            }
          } else {
            rpt->report_error(ErrorReporterInterface::MISSING_CLOSING_BRACKET,
                              yylineno,
                              yycolno);
          }
        }
      }
    } else {
      rpt->report_error(ErrorReporterInterface::MISSING_IDENTIFIER,
                        yylineno,
                        yycolno);
    }
  }
  return 1;
}

int ArrayOperandList(unsigned int first_token, Context* context) {
  // assumed first_token is '('
  ErrorReporterInterface* rpt = context->get_error_reporter();
  int next;
  while (1) {
    next = yylex();
    if (!Identifier(next, context)) {
      next = yylex();
      if (next == ')') {
        return 0;
      } else if (next == ',') {
      } else {
        rpt->report_error(ErrorReporterInterface::MISSING_CLOSING_PARENTHESIS,
                          yylineno,
                          yycolno);
        return 1;
      }
    } else {
      rpt->report_error(ErrorReporterInterface::MISSING_IDENTIFIER,
                        yylineno,
                        yycolno);
      return 1;
    }
  }
}

int CallTargets(unsigned int first_token, Context* context) {
  // assumed first_token is '['
  ErrorReporterInterface* rpt = context->get_error_reporter();
  int next;
  while (1) {
    next = yylex();
    if (!Identifier(next, context)) {
      next = yylex();
      if (next == ']')
        return 0;
      else if (next == ',') {
      } else {
        rpt->report_error(ErrorReporterInterface::MISSING_CLOSING_BRACKET,
                          yylineno,
                          yycolno);
        return 1;
      }
    } else {
      rpt->report_error(ErrorReporterInterface::MISSING_IDENTIFIER,
                        yylineno,
                        yycolno);
      return 1;
    }
  }
}

int CallArgs(unsigned int first_token, Context* context) {
  // assumed first_token is '('
  ErrorReporterInterface* rpt = context->get_error_reporter();
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
        rpt->report_error(ErrorReporterInterface::MISSING_CLOSING_PARENTHESIS,
                          yylineno,
                          yycolno);
        return 1;
      }
    } else {
      rpt->report_error(ErrorReporterInterface::MISSING_CLOSING_PARENTHESIS,
                        yylineno,
                        yycolno);
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

  // get current alu modifier from context
  BrigAluModifier mod = context->get_alu_modifier();

  if (first_token == _FTZ) {
    mod.ftz = 1;
    next = yylex();
    *last_token = next;

    if (token_type == FLOAT_ROUNDING) {
      // next is floatRounding
      mod.floatOrInt = 1;
      switch (next) {
        case _UP:
          mod.rounding = 2;
        case _DOWN:
          mod.rounding = 3;
        case _ZERO:
          mod.rounding = 1;
        case _NEAR:
          mod.rounding = 0;
      }
    } else {
      *is_ftz = true;
    }

    context->set_alu_modifier(mod);
    return 0;
  } else if (token_type == INT_ROUNDING) {
    mod.floatOrInt = 0;
    switch (first_token) {
      case _UPI:
        mod.rounding = 2;
      case _DOWNI:
        mod.rounding = 3;
      case _ZEROI:
        mod.rounding = 1;
      case _NEARI:
        mod.rounding = 0;
    }
    context->set_alu_modifier(mod);
    return 0;
  } else if (token_type == FLOAT_ROUNDING) {
    mod.floatOrInt = 1;
    switch (first_token) {
      case _UP:
        mod.rounding = 2;
      case _DOWN:
        mod.rounding = 3;
      case _ZERO:
        mod.rounding = 1;
      case _NEAR:
        mod.rounding = 0;
    }

    context->set_alu_modifier(mod);
    return 0;
  } else {
    return 1;
  }
}

int Instruction2(unsigned int first_token, Context* context) {
  // First token must be an Instruction2Opcode
  unsigned int next;
  // to get last token returned by RoundingMode in case _ftz
  unsigned int temp = 0;
  bool is_ftz = false;
  ErrorReporterInterface* rpt = context->get_error_reporter();


  if (token_type == INSTRUCTION2_OPCODE) {
    next = yylex();
    if (!RoundingMode(next, &is_ftz, &temp, context)) {
      // need to use BrigInstMod
      // there is a rounding mode specified
      if (is_ftz)
        // need to check the token returned by rounding mode
        next = temp;
      else
        next = yylex();

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
      if (token_type == PACKING) {
        // there is packing
        inst_op.packing = packing;
        next = yylex();
      }

      // now we must have a dataTypeId
      if (token_type == DATA_TYPE_ID) {
        // check the operands
        inst_op.type = data_type;
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
                context->append_code(&inst_op);
                // if the rule is valid, just write to the .code section,
                // may need to edit others, worry that later.
                return 0;
              } else {
                rpt->report_error(ErrorReporterInterface::MISSING_SEMICOLON,
                                  yylineno,
                                  yycolno);
              }

            } else {
              rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                                yylineno,
                                yycolno);
            }

          } else {
            rpt->report_error(ErrorReporterInterface::MISSING_COMMA,
                              yylineno,
                              yycolno);
          }
        } else {
          rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                            yylineno,
                            yycolno);
        }
      } else {
        rpt->report_error(ErrorReporterInterface::MISSING_DATA_TYPE,
                          yylineno,
                          yycolno);
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
      if (token_type == PACKING) {
        // there is packing
        inst_op.packing = packing;
        next = yylex();
      }

      // now we must have a dataTypeId
      if (token_type == DATA_TYPE_ID) {
        // check the operands
        inst_op.type = data_type;
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
                context->append_code(&inst_op);
                // if the rule is valid, just write to the .code section,
                // may need to edit others, worry that later.
                return 0;
              } else {
                rpt->report_error(ErrorReporterInterface::MISSING_SEMICOLON,
                                  yylineno,
                                  yycolno);
              }

            } else {
              rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                                yylineno,
                                yycolno);
            }

          } else {
            rpt->report_error(ErrorReporterInterface::MISSING_COMMA,
                              yylineno,
                              yycolno);
          }
        } else {
          rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                            yylineno,
                            yycolno);
        }
      } else {
        rpt->report_error(ErrorReporterInterface::MISSING_DATA_TYPE,
                          yylineno,
                          yycolno);
      }
      return 1;
    }
  } else if (token_type == INSTRUCTION2_OPCODE_NODT) {
    next = yylex();
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
          } else {
            rpt->report_error(ErrorReporterInterface::MISSING_SEMICOLON,
                              yylineno,
                              yycolno);
          }
        } else {
          rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                            yylineno,
                            yycolno);
        }
      } else {
        rpt->report_error(ErrorReporterInterface::MISSING_COMMA,
                          yylineno,
                          yycolno);
      }
    } else {
      rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                        yylineno,
                        yycolno);
    }
    return 1;
  } else if (token_type == INSTRUCTION2_OPCODE_FTZ) {
    next = yylex();
    // Optional FTZ
    if (next == _FTZ) {  // use BrigInstMod
      // has a _ftz
      BrigAluModifier bam;
      bam.ftz = 1;
      context->set_alu_modifier(bam);

      next = yylex();
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
      if (token_type == DATA_TYPE_ID) {
        // check the operands
        inst_op.type = data_type;
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
                context->append_code(&inst_op);
                // if the rule is valid, just write to the .code section,
                // may need to edit others, worry that later.
                return 0;
              } else {
                rpt->report_error(ErrorReporterInterface::MISSING_SEMICOLON,
                                  yylineno,
                                  yycolno);
              }
            } else {
              rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                                yylineno,
                                yycolno);
            }
          } else {
            rpt->report_error(ErrorReporterInterface::MISSING_COMMA,
                              yylineno,
                              yycolno);
          }
        } else {
          rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                            yylineno,
                            yycolno);
        }
      } else {
        rpt->report_error(ErrorReporterInterface::MISSING_DATA_TYPE,
                          yylineno,
                          yycolno);
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
      if (token_type == DATA_TYPE_ID) {
        // check the operands
        inst_op.type = data_type;
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
              } else {
                rpt->report_error(ErrorReporterInterface::MISSING_SEMICOLON,
                                  yylineno,
                                  yycolno);
              }
            } else {
              rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                                yylineno,
                                yycolno);
            }
          } else {
            rpt->report_error(ErrorReporterInterface::MISSING_COMMA,
                              yylineno,
                              yycolno);
          }
        } else {
          rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                            yylineno,
                            yycolno);
        }
      } else {
        rpt->report_error(ErrorReporterInterface::MISSING_DATA_TYPE,
                          yylineno,
                          yycolno);
      }
      return 1;
    }
  } else {
    return 1;
  }
}

int Instruction3(unsigned int first_token, Context* context) {
  ErrorReporterInterface* rpt = context->get_error_reporter();
  // First token must be an Instruction3Opcode
  unsigned int next;
  // to get last token returned by RoundingMode in case _ftz
  unsigned int temp = 0;
  bool is_ftz = false;

  // default value.
  BrigInstBase inst_op = {
    32,
    BrigEInstBase,
    opcode,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };

  if (token_type == INSTRUCTION3_OPCODE) {
    next = yylex();
    if (!RoundingMode(next, &is_ftz, &temp, context)) {
      // there is a rounding mode specified
      if (is_ftz)
        // need to check the token returned by rounding mode
        next = temp;
      else
        next = yylex();
    }

    // check whether there is a Packing
    if (token_type == PACKING) {
      // there is packing
      inst_op.packing = packing;
      next = yylex();
    }

    // now we must have a dataTypeId
    if (token_type == DATA_TYPE_ID) {
      // check the operands
      inst_op.type = data_type;
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
                if (yylex() == ';') {
                  context->append_code<BrigInstBase>(&inst_op);
                  return 0;
                } else {
                  rpt->report_error(ErrorReporterInterface::MISSING_SEMICOLON,
                                    yylineno,
                                    yycolno);
                }
              } else {  // 3rd operand
                rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                                  yylineno,
                                  yycolno);
              }
            } else {  // 2nd comma
              rpt->report_error(ErrorReporterInterface::MISSING_COMMA,
                                yylineno,
                                yycolno);
            }
          } else {  // 2nd operand
            rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                              yylineno,
                              yycolno);
          }
        } else {  // 1st comma
          rpt->report_error(ErrorReporterInterface::MISSING_COMMA,
                            yylineno,
                            yycolno);
        }
      } else {  // 1st operand
        rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                          yylineno,
                          yycolno);
      }
    } else {  // DATA_TYPE_ID
      rpt->report_error(ErrorReporterInterface::MISSING_DATA_TYPE,
                        yylineno,
                        yycolno);
    }
    return 1;
  } else if (token_type == INSTRUCTION3_OPCODE_FTZ) {
    next = yylex();
    // Optional FTZ
    if (next == _FTZ) {
      // has a _ftz
      next = yylex();
    }

    // check whether there is a Packing
    if (token_type == PACKING)
      // there is packing
      inst_op.packing = packing;
      next = yylex();

    // now we must have a dataTypeId
    if (token_type == DATA_TYPE_ID) {
      // check the operands
      inst_op.type = data_type;
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
                if (yylex() == ';') {
                  context->append_code<BrigInstBase>(&inst_op);
                  return 0;
                } else {
                  rpt->report_error(ErrorReporterInterface::MISSING_SEMICOLON,
                                    yylineno,
                                    yycolno);
                }
              } else {  // 3rd operand
                rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                                  yylineno,
                                  yycolno);
              }
            } else {  // 2nd comma
              rpt->report_error(ErrorReporterInterface::MISSING_COMMA,
                                yylineno,
                                yycolno);
            }
          } else {  // 2nd operand
            rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                              yylineno,
                              yycolno);
          }
        } else {  // 1st comma
          rpt->report_error(ErrorReporterInterface::MISSING_COMMA,
                            yylineno,
                            yycolno);
        }
      } else {  // 1st operand
        rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                          yylineno,
                          yycolno);
      }
    } else {  // DATA_TYPE_ID
      rpt->report_error(ErrorReporterInterface::MISSING_DATA_TYPE,
                        yylineno,
                        yycolno);
    }
    return 1;
  } else {
    return 1;
  }
}

int Version(unsigned int first_token, Context* context) {
  // first token must be version keyword

  if (context == NULL) {
    printf("INVALID CONTEXT\n");
    return 1;
  }

  ErrorReporterInterface* rpt = context->get_error_reporter();

  BrigDirectiveVersion bdv;
  bdv.kind = BrigEDirectiveVersion;
  bdv.size = 20;
  bdv.reserved = 0;

  // set default values
  bdv.machine = BrigELarge;
  bdv.profile = BrigEFull;
  bdv.ftz = BrigENosftz;
  if (yylex() == TOKEN_INTEGER_CONSTANT) {
    bdv.major = int_val;
    if (yylex() == ':') {
      // check for minor
      if (yylex() == TOKEN_INTEGER_CONSTANT) {
        bdv.minor = int_val;
        int next = yylex();
        if (next == ';') {
        } else if (next == ':') {
          // check for target
          next = yylex();
          while (next != ';') {
            if (token_type == TARGET) {
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
                  rpt->report_error(
                    ErrorReporterInterface::MISSING_SEMICOLON,
                    yylineno,
                    yycolno);
                  return 1;
                }
              }
            } else {
              rpt->report_error(
                ErrorReporterInterface::INVALID_TARGET,
                yylineno,
                yycolno);
              return 1;
            }
          }
        } else {
          rpt->report_error(
            ErrorReporterInterface::MISSING_SEMICOLON,
            yylineno,
            yycolno);
          return 1;
        }
        context->append_directive(&bdv);
        rpt->report_error(ErrorReporterInterface::OK, yylineno, yycolno);
        return 0;
      }
      rpt->report_error(
        ErrorReporterInterface::MISSING_INTEGER_CONSTANT,
        yylineno,
        yycolno);
    }
    rpt->report_error(ErrorReporterInterface::MISSING_COLON, yylineno, yycolno);
  }
  rpt->report_error(ErrorReporterInterface::MISSING_INTEGER_CONSTANT,
                             yylineno,
                             yycolno);
  return 1;
};

int Alignment(unsigned int first_token, Context* context) {
  // first token must be "align" keyword
  ErrorReporterInterface* rpt = context->get_error_reporter();
  if (yylex() == TOKEN_INTEGER_CONSTANT) {
    context->set_alignment(int_val);
    return 0;
  } else {
    rpt->report_error(ErrorReporterInterface::MISSING_INTEGER_CONSTANT,
                               yylineno,
                               yycolno);
    return 1;
  }
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
  ErrorReporterInterface* rpt = context->get_error_reporter();
  if (first_token == ALIGN) {
    if (!Alignment(first_token, context)) {
      next_token = yylex();  // need to go to next token
      *last_token = next_token;
      // first is alignment
      if (next_token == CONST) {
        context->set_symbol_modifier(BrigConst);
        // alignment const
        next_token = yylex();
        *last_token = next_token;

        if ((next_token == EXTERN)||(next_token == STATIC)) {
          if (next_token == EXTERN)
            context->set_attribute(BrigExtern);
          else
            context->set_attribute(BrigStatic);

          // alignment const externOrStatic
          *recheck_last_token = false;
        } else {
          // alignment const
          *recheck_last_token = true;
        }
      } else if ((next_token == EXTERN)||(next_token == STATIC)) {
        // alignment externOrStatic
        if (next_token == EXTERN)
          context->set_attribute(BrigExtern);
        else
          context->set_attribute(BrigStatic);
        next_token = yylex();
        *last_token = next_token;

        if (next_token == CONST) {
          // alignmnet externOrStatic const
          context->set_symbol_modifier(BrigConst);
        } else {
          // alignment externOrStatic
          *recheck_last_token = true;
        }
      } else {
        // alignment
        *recheck_last_token = true;
      }
    } else {
      rpt->report_error(ErrorReporterInterface::INVALID_ALIGNMENT,
                                 yylineno,
                                 yycolno);
    }
  } else if (first_token == CONST) {
    // first is const
    context->set_symbol_modifier(BrigConst);
    next_token = yylex();
    *last_token = next_token;
    if (next_token == ALIGN) {
      if (!Alignment(next_token, context)) {
        // const alignment
        next_token = yylex();
        *last_token = next_token;

        if ((next_token == EXTERN)||(next_token == STATIC)) {
          if (next_token == EXTERN)
            context->set_attribute(BrigExtern);
          else
            context->set_attribute(BrigStatic);
          // const alignment externOrStatic
        } else {
          // const alignment
          *recheck_last_token = true;
        }
      } else {
        rpt->report_error(ErrorReporterInterface::INVALID_ALIGNMENT,
                                   yylineno,
                                   yycolno);
      }
    } else if ((next_token == EXTERN)||(next_token == STATIC)) {
      // const externOrStatic
      if (next_token == EXTERN)
        context->set_attribute(BrigExtern);
      else
        context->set_attribute(BrigStatic);
      next_token = yylex();
      *last_token = next_token;

      if (next_token == ALIGN) {
        if (!Alignment(next_token, context)) {
          // const externOrStatic alignment
        } else {
          rpt->report_error(ErrorReporterInterface::INVALID_ALIGNMENT,
                                     yylineno,
                                     yycolno);
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
    if (next_token == EXTERN)
      context->set_attribute(BrigExtern);
    else
      context->set_attribute(BrigStatic);
    next_token = yylex();
    *last_token = next_token;
    if (next_token == ALIGN) {
      if (!Alignment(next_token, context)) {
        // externOrStatic alignment
        next_token = yylex();
        *last_token = next_token;

        if (next_token == CONST) {
          // externOrStatic alignment const
          context->set_symbol_modifier(BrigConst);
        } else {
          // externOrStatic alignment
          *recheck_last_token = true;
        }
      } else {
        rpt->report_error(ErrorReporterInterface::INVALID_ALIGNMENT,
                                   yylineno,
                                   yycolno);
      }
    } else if (next_token == CONST) {
      // externOrStatic const
      context->set_symbol_modifier(BrigConst);
      next_token = yylex();
      *last_token = next_token;

      if (next_token == ALIGN) {
        if (!Alignment(next_token, context)) {
          *last_token = next_token;
        } else {
          rpt->report_error(ErrorReporterInterface::INVALID_ALIGNMENT,
                                     yylineno,
                                     yycolno);
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
  ErrorReporterInterface* rpt = context->get_error_reporter();
  if (yylex() == '(') {
    if (yylex() == TOKEN_INTEGER_CONSTANT) {
      context->set_fbar(int_val);
      if (yylex() == ')')
        return 0;
      else
        rpt->report_error(ErrorReporterInterface::MISSING_CLOSING_PARENTHESIS,
                                   yylineno,
                                   yycolno);
    } else {
      rpt->report_error(ErrorReporterInterface::MISSING_INTEGER_CONSTANT,
                                 yylineno,
                                 yycolno);
    }
  }
  return 1;
}


int ArrayDimensionSet(unsigned int first_token,
                      bool* rescan_last_token,
                      unsigned int* last_token,
                      Context* context) {
  // first token must be '['
  ErrorReporterInterface* rpt = context->get_error_reporter();
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
      rpt->report_error(ErrorReporterInterface:: MISSING_CLOSING_BRACKET,
                                 yylineno,
                                 yycolno);
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
  ErrorReporterInterface* rpt = context->get_error_reporter();
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
    if ((token_type == DATA_TYPE_ID)||
        (next == _RWIMG) ||
        (next == _SAMP) ||
        (next == _ROIMG)) {
      context->set_type(data_type);
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
          context->get_code_offset(),       // c_code
          BrigArgSpace,                     // storageClass
          context->get_attribute(),         // attribute
          0,                                // reserved
          context->get_symbol_modifier(),   // symbol modifier
          0,                                // dim
          arg_name_offset,                  // s_name
          data_type,                        // data_type
          context->get_alignment(),         // alignment
          0,                                // d_init = 0 for arg
          0                                 // reserved
          };
          // append the DirectiveSymbol to .directive section.
          context->append_directive_symbol(&sym_decl);

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


          *last_token = next;
          *rescan_last_token = true;
          return 0;
        }
      } else {
        rpt->report_error(ErrorReporterInterface:: MISSING_IDENTIFIER,
                          yylineno,
                          yycolno);
      }
    } else {
      rpt->report_error(ErrorReporterInterface:: MISSING_DATA_TYPE,
                                 yylineno,
                                 yycolno);
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
  ErrorReporterInterface* rpt = context->get_error_reporter();
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
      rpt->report_error(ErrorReporterInterface:: MISSING_ARGUMENT,
                        yylineno,
                        yycolno);
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
  ErrorReporterInterface* rpt = context->get_error_reporter();
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

        std::string func_name = string_val;
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
          token_to_scan = yylex();

          if (token_to_scan == ')') {   // empty argument list body
            token_to_scan = yylex();
          } else if (!ArgumentListBody(token_to_scan,
                                       &rescan,
                                       &token_to_scan,
                                       context)) {
            if (!rescan)
              token_to_scan = yylex();

            if (token_to_scan == ')') {
              token_to_scan = yylex();
            } else {
              rpt->report_error(ErrorReporterInterface::
                                    MISSING_CLOSING_PARENTHESIS,
                                yylineno,
                                yycolno);
              return 1;
            }
          } else {
            rpt->report_error(ErrorReporterInterface:: INVALID_ARGUMENT_LIST,
                              yylineno,
                              yycolno);
            return 1;
          }
        } else {
          rpt->report_error(ErrorReporterInterface:: MISSING_ARGUMENT_LIST,
                            yylineno,
                            yycolno);
          return 1;
        }
        // check argument list
        if (token_to_scan == '(') {
          context->set_arg_output(false);
          token_to_scan = yylex();

          if (token_to_scan == ')') {   // empty argument list body
            token_to_scan = yylex();
          } else if (!ArgumentListBody(token_to_scan,
                                       &rescan,
                                       &token_to_scan,
                                       context)) {
            if (!rescan)
              token_to_scan = yylex();
            if (token_to_scan == ')') {
              token_to_scan = yylex();
            } else {
              rpt->report_error(ErrorReporterInterface::
                                    MISSING_CLOSING_PARENTHESIS,
                                yylineno,
                                yycolno);
              return 1;
            }
          } else {
            rpt->report_error(ErrorReporterInterface:: INVALID_ARGUMENT_LIST,
                              yylineno,
                              yycolno);
            return 1;
          }
        } else {
          rpt->report_error(ErrorReporterInterface:: MISSING_ARGUMENT_LIST,
                            yylineno,
                            yycolno);
          return 1;
        }
        // check for optional FBar
        if (token_to_scan == _FBAR) {
          if (!FBar(token_to_scan, context)) {
            return 0;
          } else {
            rpt->report_error(ErrorReporterInterface:: INVALID_FBAR,
                              yylineno,
                              yycolno);
          }
        } else {
          *rescan_last_token = true;
          *last_token = token_to_scan;
          return 0;
        }
      } else {
        rpt->report_error(ErrorReporterInterface:: MISSING_IDENTIFIER,
                          yylineno,
                          yycolno);
      }
    }
  }
  return 1;
}

int FunctionDecl(unsigned int first_token, Context* context) {
  unsigned int token_to_scan;
  bool rescan;
  ErrorReporterInterface* rpt = context->get_error_reporter();
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

            if (token_to_scan == ')') {
              token_to_scan = yylex();
            } else {
              rpt->report_error(ErrorReporterInterface::
                                    MISSING_CLOSING_PARENTHESIS,
                                yylineno,
                                yycolno);
              return 1;
            }
          } else {
            rpt->report_error(ErrorReporterInterface:: INVALID_ARGUMENT_LIST,
                              yylineno,
                              yycolno);
            return 1;
          }
        } else {
          rpt->report_error(ErrorReporterInterface:: MISSING_ARGUMENT_LIST,
                            yylineno,
                            yycolno);
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
            if (token_to_scan == ')') {
              token_to_scan = yylex();
            } else {
              rpt->report_error(ErrorReporterInterface::
                                    MISSING_CLOSING_PARENTHESIS,
                                yylineno,
                                yycolno);
              return 1;
            }
          } else {
            rpt->report_error(ErrorReporterInterface:: INVALID_ARGUMENT_LIST,
                              yylineno,
                              yycolno);
            return 1;
          }
        } else {
          rpt->report_error(ErrorReporterInterface:: MISSING_ARGUMENT_LIST,
                            yylineno,
                            yycolno);
          return 1;
        }


        // check for optional FBar
        if (token_to_scan == _FBAR) {
          if (!FBar(token_to_scan, context)) {
            token_to_scan = yylex();
          } else {
            rpt->report_error(ErrorReporterInterface:: INVALID_FBAR,
                              yylineno,
                              yycolno);
          }
        }
        if (token_to_scan == ';')
          return 0;
        else
          rpt->report_error(ErrorReporterInterface:: MISSING_SEMICOLON,
                            yylineno,
                            yycolno);
      } else {
        rpt->report_error(ErrorReporterInterface:: MISSING_IDENTIFIER,
                          yylineno,
                          yycolno);
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
  // add BrigDirectiveScope
  BrigDirectiveScope argblock_start = {
    8,
    BrigEDirectiveArgStart,
    context->get_code_offset()
  };

  // needed for update d_firstScopedDirective
  BrigdOffset32_t arg_offset = context->get_directive_offset();
  context->append_directive<BrigDirectiveScope>(&argblock_start);

  // update the d_nextDirective.
  BrigDirectiveFunction bdf;
  context->get_directive<BrigDirectiveFunction>(
                                    context->current_bdf_offset, &bdf);
  if (bdf.d_firstScopedDirective == bdf.d_nextDirective)
    // check if the firstScopedDirective is modified before.
    bdf.d_firstScopedDirective = arg_offset;
  unsigned char * bdf_charp = reinterpret_cast<unsigned char*>(&bdf);
  context->update_directive_bytes(bdf_charp,
                                  context->current_bdf_offset,
                                  bdf.size);

  ErrorReporterInterface* rpt = context->get_error_reporter();
  while (1) {
    next_token = yylex();
    if ((token_type == INSTRUCTION2_OPCODE) ||
        (token_type == INSTRUCTION2_OPCODE_NODT) ||
        (token_type == INSTRUCTION2_OPCODE_FTZ)) {
      // Instruction 2 Operation
      if (!Instruction2(next_token, context)) {
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
    } else if ((token_type == INSTRUCTION3_OPCODE) ||
               (token_type == INSTRUCTION3_OPCODE_FTZ)) {
      // Instruction 3 Operation
      if (!Instruction3(next_token, context)) {
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
    } else if (token_type == QUERY_OP) {  // Query Operation
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
      } else {
        rpt->report_error(ErrorReporterInterface:: MISSING_SEMICOLON,
                          yylineno,
                          yycolno);
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
      // context->lable_o_map[label_name] = context->get_operand_offset();
      // set the lable_map
      // add operand in .operand;
      // check if any entry with the same key in label_c_map;
      // update the corresponding instructions.
      //
      // if no correlated instructions, then later instructions
      // can directly use the label.
      if (yylex() == ':') {
      } else {
        rpt->report_error(ErrorReporterInterface:: MISSING_COLON,
                          yylineno,
                          yycolno);
        return 1;
      }
    } else if (next_token == CALL) {  // call (only inside argblock
      if (!Call(next_token, context)) {
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
    } else if ((next_token == ALIGN) ||
               (next_token == CONST) ||
               (next_token == STATIC) ||
               (next_token == EXTERN)) {
      if (!DeclPrefix(first_token, &rescan, &last_token, context)) {
        if (!rescan)
          next_token = yylex();
        else
          next_token = last_token;

        if (token_type == INITIALIZABLE_ADDRESS) {
          // initializable decl
          if (!InitializableDecl(next_token, context)) {
          }
        } else if (token_type == UNINITIALIZABLE_ADDRESS) {
          // uninitializable decl
          if (!UninitializableDecl(next_token, context)) {
          }
        } else if (next_token == ARG) {
          // arg uninitializable decl
          if (!ArgUninitializableDecl(next_token, context)) {
          }
        }
      }
    } else if (token_type == INITIALIZABLE_ADDRESS) {
      if (!InitializableDecl(next_token, context)) {
      } else {
        return 1;
      }
    } else if (token_type == UNINITIALIZABLE_ADDRESS) {
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
      rpt->report_error(ErrorReporterInterface:: INVALID_NESTED_ARGUMENT_SCOPE,
                        yylineno,
                        yycolno);
      return 1;
    } else if (next_token == '}') {
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
  ErrorReporterInterface* rpt = context->get_error_reporter();
  while (1) {
    next_token = yylex();
    if ((token_type == INSTRUCTION2_OPCODE) ||
        (token_type == INSTRUCTION2_OPCODE_NODT) ||
        (token_type == INSTRUCTION2_OPCODE_FTZ)) {
      // Instruction 2 Operation
      if (!Instruction2(next_token, context)) {
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
    } else if ((token_type == INSTRUCTION3_OPCODE) ||
               (token_type == INSTRUCTION3_OPCODE_FTZ)) {
      // Instruction 3 Operation
      if (!Instruction3(next_token, context)) {
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
    } else if (token_type == QUERY_OP) {  // Query Operation
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

      } else {
        rpt->report_error(ErrorReporterInterface:: MISSING_SEMICOLON,
                          yylineno,
                          yycolno);
        return 1;
      }
    } else if ((next_token == BRN) ||
               (next_token == CBR)) {
      if (!Branch(next_token, context)) {
      } else {
        return 1;
      }
    } else if (next_token == TOKEN_LABEL) {  // label
      // add to the .directive section
      std::string label_name = string_val;
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
      context->get_directive<BrigDirectiveFunction>(
                  context->current_bdf_offset, &bdf);
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
      } else {
        rpt->report_error(ErrorReporterInterface:: MISSING_COLON,
                          yylineno,
                          yycolno);
        return 1;
      }
    } else if (next_token == '{') {  // argument scope -> inner codeblock
      if (!ArgBlock(next_token, context)) {
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

        if (token_type == INITIALIZABLE_ADDRESS) {
          // initializable decl
          if (!InitializableDecl(next_token, context)) {
          } else {
            return 1;
          }
        } else if (token_type == UNINITIALIZABLE_ADDRESS) {
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
    } else if (token_type == INITIALIZABLE_ADDRESS) {
      if (!InitializableDecl(next_token, context)) {
      } else {
        return 1;
      }
    } else if (token_type == UNINITIALIZABLE_ADDRESS) {
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
  ErrorReporterInterface* rpt = context->get_error_reporter();
  if (!FunctionDefinition(first_token, &rescan, &last_token, context)) {
    if (!rescan)
      last_token = yylex();
    if (!Codeblock(last_token, context)) {
      if (yylex() == ';') {
        return 0;
      } else {
        rpt->report_error(ErrorReporterInterface:: MISSING_SEMICOLON,
                          yylineno,
                          yycolno);
      }
    }
  }
  return 1;
}

int Program(unsigned int first_token, Context* context) {
  int result;
  unsigned int last_token;
  bool rescan;
  ErrorReporterInterface* rpt = context->get_error_reporter();
  if (first_token == VERSION) {
    if (!Version(first_token, context)) {
      // parse topLevelStatement
      first_token = yylex();
      while (first_token && (first_token != VERSION)) {
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

                if (first_token == ')') {
                  first_token = yylex();
                } else {
                  rpt->report_error(ErrorReporterInterface::
                                        MISSING_CLOSING_PARENTHESIS,
                                    yylineno,
                                    yycolno);
                  return 1;  // missing closing )
                }
              } else {
                rpt->report_error(ErrorReporterInterface::
                                      INVALID_ARGUMENT_LIST,
                                  yylineno,
                                  yycolno);
                return 1;
              }
            } else {  // missing '('
              rpt->report_error(ErrorReporterInterface:: MISSING_ARGUMENT_LIST,
                                yylineno,
                                yycolno);
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
                if (first_token == ')') {
                  first_token = yylex();
                } else {
                  rpt->report_error(ErrorReporterInterface::
                                        MISSING_CLOSING_PARENTHESIS,
                                    yylineno,
                                    yycolno);
                  return 1;  // missing closing )
                }
              } else {
                rpt->report_error(ErrorReporterInterface::
                                      INVALID_ARGUMENT_LIST,
                                  yylineno,
                                  yycolno);
                return 1;
              }
            } else {  // missing '('
              rpt->report_error(ErrorReporterInterface:: MISSING_ARGUMENT_LIST,
                                yylineno,
                                yycolno);
              return 1;
            }            // if found '(' - argList

            // check for optional FBar
            if (first_token == _FBAR) {
              if (!FBar(first_token, context)) {
                first_token = yylex();
              } else {
                rpt->report_error(ErrorReporterInterface:: INVALID_FBAR,
                                  yylineno,
                                  yycolno);
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
                  rpt->report_error(ErrorReporterInterface:: MISSING_SEMICOLON,
                                    yylineno,
                                    yycolno);
                  return 1;
                }
              } else {
                return 1;
              }
            } else {
              rpt->report_error(ErrorReporterInterface:: MISSING_SEMICOLON,
                                yylineno,
                                yycolno);
            }
          }       // if found TOKEN_GLOBAL_ID
        } else if (token_type == INITIALIZABLE_ADDRESS) {
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
    rpt->report_error(ErrorReporterInterface:: MISSING_VERSION_STATEMENT,
                      yylineno,
                      yycolno);
  }
  return 1;
}

int OptionalWidth(unsigned int first_token, Context* context) {
  // first token must be _WIDTH
  ErrorReporterInterface* rpt = context->get_error_reporter();
  unsigned int next_token = yylex();
  if (next_token == '(') {
    next_token = yylex();
    if (next_token == ALL) {
      next_token = yylex();
    } else if (next_token == TOKEN_INTEGER_CONSTANT) {
      next_token = yylex();
    } else {
      rpt->report_error(ErrorReporterInterface:: MISSING_WIDTH_INFO,
                        yylineno,
                        yycolno);
      return 1;
    }
    if (next_token == ')') {
      return 0;
    } else {
      rpt->report_error(ErrorReporterInterface:: MISSING_CLOSING_PARENTHESIS,
                        yylineno,
                        yycolno);
    }
  } else {
    rpt->report_error(ErrorReporterInterface:: MISSING_WIDTH_INFO,
                      yylineno,
                      yycolno);
  }
  return 1;
}

int Branch(unsigned int first_token, Context* context) {
  unsigned int op = first_token;  // CBR or BRN
  unsigned int current_token;
  ErrorReporterInterface* rpt = context->get_error_reporter();
  BrigAluModifier mod = context->get_alu_modifier();

  current_token = yylex();
  // check for optionalWidth
  if (current_token == _WIDTH) {
    if (!OptionalWidth(current_token, context)) {
    } else {
      return 1;
    }
    current_token = yylex();
  }

  // check for optional _fbar modifier
  if (current_token == __FBAR) {
    mod.fbar = 1;
    context->set_alu_modifier(mod);
    current_token = yylex();
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

    std::string operand_name = string_val;
    if (!Operand(current_token, context)) {
      inst_op.o_operands[1] = context->operand_map[operand_name];
      if (yylex() == ',') {
        current_token = yylex();
        if (current_token == TOKEN_LABEL) {
          // if the next operand is label, which is the case in example4
          // 1. check if the label is already defined,
          // 2. if defined, just set it up
          // 3. if not, add it to the multimap
          std::string label_name = string_val;
          if (context->label_o_map.count(label_name)) {
            inst_op.o_operands[2] = context->label_o_map[label_name];
          } else {
            context->label_c_map.insert(make_pair(
                                          label_name,
                                          context->get_code_offset()+20));
          }

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
                    if (yylex() == TOKEN_LABEL) {
                      current_token = yylex();  // scan next;
                    } else {
                      rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                                        yylineno,
                                        yycolno);
                      return 1;
                    }
                  } else {
                    rpt->report_error(ErrorReporterInterface::MISSING_COMMA,
                                      yylineno,
                                      yycolno);
                    return 1;
                  }
                }   // while
              } else {
                rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                              yylineno,
                              yycolno);
              }
              // current token should be ']'
              current_token = yylex();  // should be ';'
            } else {
              rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                            yylineno,
                            yycolno);
            }
          }  else {  // yylex() = ','
            rpt->report_error(ErrorReporterInterface::MISSING_COMMA,
                              yylineno,
                              yycolno);
          }
        } else {
          rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                            yylineno,
                            yycolno);
          return 1;
        }
        if (current_token == ';') {
          context->append_code<BrigInstBase>(&inst_op);
          // update the operationCount.
          BrigDirectiveFunction bdf;
          context->get_directive<BrigDirectiveFunction>(
                                            context->current_bdf_offset,
                                            &bdf);
          bdf.operationCount++;
          unsigned char * bdf_charp = reinterpret_cast<unsigned char*>(&bdf);
          context->update_directive_bytes(bdf_charp,
                                          context->current_bdf_offset,
                                          bdf.size);

          return 0;
        } else {
          rpt->report_error(ErrorReporterInterface::MISSING_SEMICOLON,
                            yylineno,
                            yycolno);
        }
      } else {  // yylex = ','
        rpt->report_error(ErrorReporterInterface::MISSING_COMMA,
                          yylineno,
                          yycolno);
      }
    } else {  // first operand
      rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                        yylineno,
                        yycolno);
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

    if (current_token == TOKEN_LABEL) {
      // if the next operand is label, which is the case in example4
      // 1. check if the label is already defined,
      // 2. if defined, just set it up
      // 3. if not, add it to the multimap
      std::string label_name = string_val;
      if (context->label_o_map.count(label_name)) {
        inst_op.o_operands[1] = context->label_o_map[label_name];
      } else {
        context->label_c_map.insert(make_pair(
                                      label_name,
                                      context->get_code_offset()+16));
      }
      current_token = yylex();
      if (current_token == ';') {
        context->append_code<BrigInstBar>(&inst_op);
          // update the operationCount.
          BrigDirectiveFunction bdf;
          context->get_directive<BrigDirectiveFunction>(
                                            context->current_bdf_offset,
                                            &bdf);
          bdf.operationCount++;
          unsigned char * bdf_charp = reinterpret_cast<unsigned char*>(&bdf);
          context->update_directive_bytes(bdf_charp,
                                          context->current_bdf_offset,
                                          bdf.size);
        return 0;
      } else {
        rpt->report_error(ErrorReporterInterface::MISSING_SEMICOLON,
                          yylineno,
                          yycolno);
      }
    } else if (!Identifier(current_token, context)) {
      current_token = yylex();

      if (current_token == ';') {
        return 0;
      } else if (current_token == ',') {
        if (yylex() == '[') {
          current_token = yylex();
          if (current_token == TOKEN_LABEL) {
            current_token = yylex();    // should be ']'
          } else if (!Identifier(current_token, context)) {
            current_token = yylex();    // should be ']'
          }
        }

        if (current_token == ']') {
        } else {
          rpt->report_error(ErrorReporterInterface::MISSING_CLOSING_BRACKET,
                            yylineno,
                            yycolno);
        }

        current_token = yylex();
        if (current_token == ';') {
          return 0;
        } else {
          rpt->report_error(ErrorReporterInterface::MISSING_SEMICOLON,
                            yylineno,
                            yycolno);
        }
      } else {
        rpt->report_error(ErrorReporterInterface::MISSING_SEMICOLON,
                          yylineno,
                          yycolno);
      }
    } else {
      rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                        yylineno,
                        yycolno);
    }
    return 1;
  }
}

int Call(unsigned int first_token, Context* context) {
  // first token is "call"
  unsigned int next = yylex();
  ErrorReporterInterface* rpt = context->get_error_reporter();
  // optional width
  if (next == _WIDTH) {
    if (!OptionalWidth(next, context)) {
      next = yylex();
    } else {
      return 1;
    }
  }


  if (!Operand(next, context)) {
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
    if (next == TOKEN_GLOBAL_IDENTIFIER) {
      std::string func_name = string_val;
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
      context->append_code<BrigInstBase>(&call_op);
      return 0;
    } else if (next == '[') {
      if (!CallTargets(next, context)) {
        if (yylex() == ';') {
           return 0;
        } else {
          rpt->report_error(ErrorReporterInterface::MISSING_SEMICOLON,
                            yylineno,
                            yycolno);
        }
      }
      return 1;
    } else {
      rpt->report_error(ErrorReporterInterface::MISSING_SEMICOLON,
                        yylineno,
                        yycolno);
    }
  } else {
    rpt->report_error(ErrorReporterInterface::MISSING_OPERAND,
                      yylineno,
                      yycolno);
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
  ErrorReporterInterface* rpt = context->get_error_reporter();
  if (next == TOKEN_LABEL) {
    rpt->report_error(ErrorReporterInterface::INVALID_INITIALIZER,
                      yylineno,
                      yycolno);
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
          rpt->report_error(ErrorReporterInterface::MISSING_LABEL,
                            yylineno,
                            yycolno);
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
          rpt->report_error(ErrorReporterInterface::MISSING_INTEGER_CONSTANT,
                            yylineno,
                            yycolno);
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
          rpt->report_error(ErrorReporterInterface::MISSING_SINGLE_CONSTANT,
                            yylineno,
                            yycolno);
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
          rpt->report_error(ErrorReporterInterface::MISSING_DOUBLE_CONSTANT,
                            yylineno,
                            yycolno);
          return 1;
        }
      } else {
        *last_token = next;
        *rescan = true;
        break;
      }
    }  // while(1)
  } else {
    rpt->report_error(ErrorReporterInterface::INVALID_INITIALIZER,
                      yylineno,
                      yycolno);
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
  ErrorReporterInterface* rpt = context->get_error_reporter();
  if (token_type == DATA_TYPE_ID) {
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

        if (next == ';') {
          return 0;
        } else {
          rpt->report_error(ErrorReporterInterface::MISSING_SEMICOLON,
                            yylineno,
                            yycolno);
        }
      }
    } else {
      rpt->report_error(ErrorReporterInterface::MISSING_IDENTIFIER,
                        yylineno,
                        yycolno);
    }
  } else {
    rpt->report_error(ErrorReporterInterface::MISSING_DATA_TYPE,
                      yylineno,
                      yycolno);
  }
  return 1;
};

int UninitializableDecl(unsigned int first_token, Context* context) {
  // first_token is PRIVATE, GROUP or SPILL
  bool rescan;
  unsigned int last_token;
  unsigned int next;
  ErrorReporterInterface* rpt = context->get_error_reporter();
  next = yylex();
  if (token_type == DATA_TYPE_ID) {
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

      if (next == ';') {
        return 0;
      } else {
        rpt->report_error(ErrorReporterInterface::MISSING_SEMICOLON,
                          yylineno,
                          yycolno);
      }
    } else {
      rpt->report_error(ErrorReporterInterface::MISSING_IDENTIFIER,
                        yylineno,
                        yycolno);
    }
  } else {
    rpt->report_error(ErrorReporterInterface::MISSING_DATA_TYPE,
                      yylineno,
                      yycolno);
  }
  return 1;
}

int ArgUninitializableDecl(unsigned int first_token, Context* context) {
  // first token is ARG
  bool rescan;
  unsigned int last_token;
  unsigned int next = yylex();
  ErrorReporterInterface* rpt = context->get_error_reporter();
  if (token_type == DATA_TYPE_ID) {
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
      if (next == ';') {
        return 0;
      } else {
        rpt->report_error(ErrorReporterInterface::MISSING_SEMICOLON,
                          yylineno,
                          yycolno);
      }
    } else {
      rpt->report_error(ErrorReporterInterface::MISSING_IDENTIFIER,
                        yylineno,
                        yycolno);
    }
  } else {
    rpt->report_error(ErrorReporterInterface::MISSING_DATA_TYPE,
                      yylineno,
                      yycolno);
  }
  return 1;
}

int FileDecl(unsigned int first_token, Context* context) {
  // first token is _FILE "file"
  unsigned int next_token = yylex();

  if (next_token == TOKEN_INTEGER_CONSTANT) {
    next_token = yylex();

    if (next_token == TOKEN_STRING) {
      next_token = yylex();

      if (next_token == ';') {
        return 0;
      } else {
       // "Missing ';' at the end of statement\n"
      }
    }
  }
  return 1;
}

int VectorToken(unsigned int first_token , Context *context) {
  if ( _V2 == first_token || _V4 == first_token)
    return 0;
  else
    return 1;
}

int SignatureType(unsigned int first_token , Context *context) {
  // first token is ARG
  unsigned int last_token;
  unsigned int next = yylex();

  if (DATA_TYPE_ID == token_type) {
    return 0;
  } else if (_ROIMG == next
           || _RWIMG == next
           || _SAMP == next ) {
    return 0;
  }
  return 1;
}

int SysCall(unsigned int first_token, Context* context) {
  // frist token is _SYSCALL "syscall"
  unsigned int token_next;
  token_next = yylex();
  if (!Operand(token_next, context) &&
      token_next == TOKEN_SREGISTER &&
      yylex() == ',') {
    token_next = yylex();

    if (!BaseOperand(token_next, context) &&
        token_next == TOKEN_INTEGER_CONSTANT &&
        yylex() == ',') {
      token_next = yylex();

      if (!Operand(token_next, context) &&
         (token_next == TOKEN_SREGISTER ||
          token_next == TOKEN_WAVESIZE ||
          token_next == TOKEN_INTEGER_CONSTANT) &&
         yylex() == ',') {
        token_next = yylex();

        if (!Operand(token_next, context) &&
           (token_next == TOKEN_SREGISTER ||
            token_next == TOKEN_WAVESIZE ||
            token_next == TOKEN_INTEGER_CONSTANT) &&
           yylex() == ',') {
          token_next = yylex();

          if (!Operand(token_next, context) &&
             (token_next == TOKEN_SREGISTER ||
              token_next == TOKEN_WAVESIZE ||
              token_next == TOKEN_INTEGER_CONSTANT)) {
            if (yylex() == ';') {
              return 0;
            }  // ';'
          }  // 5 operand
        }  // 4 operand
      }  // 3 operand
    }  // 2 base operand
  }  // 1 operand
  return 1;
}

}  // namespace brig
}  // namespace hsa
