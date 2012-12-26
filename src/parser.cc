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

int QueryOp(Context* context, BrigDataType16_t* pDataType, BrigOpcode32_t* pOpcode) {
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
  return QueryOp(context, &dataType, &opcode);
}

int Query(Context* context) {

  BrigOpcode32_t opcode;
  BrigDataType16_t type;
  BrigoOffset32_t OpOffset[2] = {0,0};

  if (QueryOp(context, &type, &opcode)) {
    return 1;
  }
  context->token_to_scan = yylex();
  if (context->token_value.data_type != type) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->token_to_scan = yylex();
  if (!CheckDestRegister(context->token_to_scan, type, opcode)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (Operand(context, &OpOffset[0])) {
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  if (context->token_to_scan != '[') {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  context->token_to_scan = yylex();
  if (AddressableOperand(context, &OpOffset[1], true)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstBase query_inst = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    opcode,                     // opcode
    type,               // type
    BrigNoPacking,         // packing
    {OpOffset[0], OpOffset[1], 0, 0, 0}        // o_operands[5]
  };
  context->append_code(&query_inst);
  context->token_to_scan = yylex();
  return 0;
}

int Operand(Context* context, BrigoOffset32_t* pRetOpOffset,
            BrigDataType16_t expectedType) {
  BrigoOffset32_t current_offset;
  std::string opName;
  BrigDataType16_t type;
  current_offset = context->get_operand_offset();
  if (context->token_type == REGISTER) {
    switch (context->token_to_scan) {
      case TOKEN_CREGISTER: type = Brigb1; break;
      case TOKEN_DREGISTER: type = Brigb64; break;
      case TOKEN_SREGISTER: type = Brigb32; break;
      case TOKEN_QREGISTER: type = Brigb128; break;
    }
    if (expectedType != type) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
  }

  if ((context->token_type == REGISTER) ||
      (context->token_to_scan == TOKEN_WAVESIZE)) {
    opName = context->token_value.string_val;
  } else if (context->token_type == CONSTANT ||
             context->token_to_scan == '-' ||
             context->token_to_scan == '+' ||
             context->token_type == DATA_TYPE_ID) {
    if (current_offset % 8) {
      current_offset += 8 - current_offset % 8;
    }
  }

  if (!Identifier(context)) {
    *pRetOpOffset = (current_offset == context->get_operand_offset()) ?
                      context->operand_map[opName] : current_offset;
    context->token_to_scan = yylex();
    return 0;
  } else if(!BaseOperand(context)) {
    *pRetOpOffset = (current_offset == context->get_operand_offset()) ?
                      context->operand_map[opName] : current_offset;
    context->token_to_scan = yylex();
    return 0;
  } else {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
}

int Operand(Context* context, BrigoOffset32_t* pRetOpOffset) {
  BrigoOffset32_t current_offset;
  std::string opName;
  current_offset = context->get_operand_offset();

  if ((context->token_type == REGISTER) ||
      (context->token_to_scan == TOKEN_WAVESIZE)) {
    opName = context->token_value.string_val;
  } else if (context->token_type == CONSTANT ||
             context->token_to_scan == '-' ||
             context->token_to_scan == '+' ||
             context->token_type == DATA_TYPE_ID) {
    if (current_offset % 8) {
      current_offset += 8 - current_offset % 8;
    }
  }

  if (!Identifier(context)) {
    *pRetOpOffset = (current_offset == context->get_operand_offset()) ?
      context->operand_map[opName] : current_offset;
    context->token_to_scan = yylex();
    return 0;
  } else if(!BaseOperand(context)){
    *pRetOpOffset = (current_offset == context->get_operand_offset()) ?
      context->operand_map[opName] : current_offset;
    context->token_to_scan = yylex();
    return 0;
  }else{
    context->set_error(INVALID_OPERAND);
    return 1;
  }
}

int Operand(Context* context) {
  BrigoOffset32_t retOpOffset = 0;
  return Operand(context, &retOpOffset);
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
      case TOKEN_CREGISTER: bor.type = Brigb1; break;
      case TOKEN_DREGISTER: bor.type = Brigb64; break;
      case TOKEN_SREGISTER: bor.type = Brigb32; break;
      case TOKEN_QREGISTER: bor.type = Brigb128; break;
    }
    bor.reserved = 0;
    std::string name(context->token_value.string_val);
    bor.s_name = context->add_symbol(name);
    if (!context->operand_map.count(name)) {
      context->operand_map[name] = context->get_operand_offset();
      context->append_operand(&bor);
    }
    return 0;
  }
  return 1;
}

int BaseOperand(Context* context) {

  BrigDataType16_t type;
  type = ConvertTypeToB(context->get_type());
  int sign = 1;
  if ((context->token_to_scan == '-') || (context->token_to_scan == '+')) {
    sign = (context->token_to_scan == '-') ? -1 : 1;
    context->token_to_scan = yylex();
  }

  if (context->token_to_scan == TOKEN_DOUBLE_CONSTANT) {
    BrigOperandImmed boi = {
      sizeof(boi),        // size
      BrigEOperandImmed,  // kind
      Brigb64,            // type
      0,                  // reserved
      { 0 }
    };
    boi.bits.l[0] = boi.bits.l[1] = 0;
    boi.bits.d = sign*context->token_value.double_val;
    context->append_operand(&boi);
    return 0;
  }

  if (context->token_to_scan == TOKEN_SINGLE_CONSTANT) {
    BrigOperandImmed boi = {
      sizeof(boi),        // size
      BrigEOperandImmed,  // kind
      Brigb32,            // type
      0,                  // reserved
      { 0 }
    };
    boi.type = type;
    boi.bits.l[0] = boi.bits.l[1] = 0;
    boi.bits.f = sign*context->token_value.float_val;
    context->append_operand(&boi);
    return 0;
  }

  if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
    BrigOperandImmed boi = {
      sizeof(boi),        // size
      BrigEOperandImmed,  // kind
      Brigb32,            // type
      0,                  // reserved
      { 0 }
    };

    boi.type = type;
    boi.bits.l[0] = boi.bits.l[1] = 0;
    if (boi.type == Brigb64) {
      boi.bits.l[0] = sign*context->token_value.int_val;
    } else {
      boi.bits.u = sign*context->token_value.int_val;
    }
    context->append_operand(&boi);

    return 0;
  }

  if (context->token_to_scan == TOKEN_WAVESIZE) {
    if (!context->operand_map.count("WAVESIZE")) {
      BrigOperandWaveSz waveOp = {
        sizeof(BrigOperandWaveSz),
        BrigEOperandWaveSz
      };
      context->operand_map["WAVESIZE"] = context->get_operand_offset();
      context->append_operand(&waveOp);
    }
    return 0;
  }

  if (context->token_type == DATA_TYPE_ID) {
    BrigDataType16_t subType;
    uint32_t size;
    std::vector<int64_t> decList;
    type = ConvertTypeToB(context->token_value.data_type, &subType, &size);
    context->token_to_scan = yylex();
    if (context->token_to_scan != '(') {
      context->set_error(MISSING_OPENNING_BRACKET);
      return 1;
    }
    context->token_to_scan = yylex();
    if (context->token_to_scan == TOKEN_INTEGER_CONSTANT ||
        context->token_to_scan == '-' ||
        context->token_to_scan == '+') {
      if (DecimalListSingle(context, &decList)) {
        return 1;
      }
      if (decList.size() != size) {
        context->set_error(MISSING_INTEGER_CONSTANT);
        return 1;
      }
    }
    if (context->token_to_scan == TOKEN_DOUBLE_CONSTANT) {
      std::vector<double> douList;
      if (FloatListSingle(context, &douList)) {
        return 1;
      }
      if (douList.size() != size) {
        context->set_error(MISSING_DOUBLE_CONSTANT);
        return 1;
      }
      switch (subType) {
        case Brigb32: {
          float temp;
          for (uint32_t i = 0; i < douList.size(); ++i) {
            temp = (float)douList[i];
            decList.push_back((uint64_t)(*reinterpret_cast<int32_t*>(&temp)));
          }
          break;
        }
        case Brigb64:
          for (uint32_t i = 0; i < douList.size(); ++i) {
            decList.push_back(*reinterpret_cast<int64_t*>(&douList[i]));
          }
          break;
      }
    }
    if (context->token_to_scan != ')') {
      context->set_error(MISSING_CLOSING_BRACKET);
      return 1;
    }
    BrigOperandImmed boi = {
      sizeof(boi),        // size
      BrigEOperandImmed,  // kind
      Brigb32,            // type
      0,                  // reserved
      { 0 }
    };
    uint32_t size1 = 0, size2 = 0;
    //TODO(Chuang): f16 and b1 constant implement should be implemented
    size1 = size2 = size;
    if (type == Brigb128) {
      //Note: 128bit constant is not exist except packed constant.
      //      If data type is Brigb128. Size will never be only 1.
      size1 >>= 1;
    }
    uint64_t shift = 0x0;
    //Note: If the subType is Zero, it won't be a packed constant.
    //      the type of Zero is Brigs8.
    switch (subType) {
      case Brigb8:  shift = 8;  break;
      case Brigb16: shift = 16; break;
      case Brigb32: shift = 32; break;
    }
    boi.bits.l[0] = boi.bits.l[1] = 0;
    // Not endian sensitive
    uint32_t i;
    switch (subType) {
      case Brigb8:
        for (i = 0 ; i < size1; ++i) {
          boi.bits.l[0] <<= shift;
          boi.bits.l[0] += (uint8_t)decList[i];
        }
        for (; i < size2; ++i) {
          boi.bits.l[1] <<= shift;
          boi.bits.l[1] += (uint8_t)decList[i];
        }
        break;

      case Brigb16:
        for (i = 0 ; i < size1; ++i) {
          boi.bits.l[0] <<= shift;
          boi.bits.l[0] += (uint16_t)decList[i];
        }
        for (; i < size2; ++i) {
          boi.bits.l[1] <<= shift;
          boi.bits.l[1] += (uint16_t)decList[i];
        }
        break;
      case Brigb32:
        for (i = 0 ; i < size1; ++i) {
          boi.bits.l[0] <<= shift;
          boi.bits.l[0] += (uint32_t)decList[i];
        }
        for (; i < size2; ++i) {
          boi.bits.l[1] <<= shift;
          boi.bits.l[1] += (uint32_t)decList[i];
        }
        break;
      case Brigb64:
        for (i = 0 ; i < size1; ++i) {
          boi.bits.l[0] = (uint64_t)decList[i];
        }
        for (; i < size2; ++i) {
          boi.bits.l[1] = (uint64_t)decList[i];
        }
        break;
    }
    boi.type = type;
    context->append_operand(&boi);
    return 0;
  }
  return 1;
}

int AddressableOperand(Context* context) {
  BrigoOffset32_t opOffset = 0;
  return AddressableOperand(context, &opOffset, true);
}

int AddressableOperand(Context* context, BrigoOffset32_t* pRetOpOffset,
    bool IsImageOrSampler){
  bool isIntConst = false; // check if match [name<int>]
  bool isBracket = false;  // check if include <..> in the [name ..]

  BrigdOffset32_t directive = 0;
  BrigoOffset32_t reg = 0;
  int32_t offset = 0;

  // first token must be non register
  if ((context->token_to_scan != TOKEN_GLOBAL_IDENTIFIER) &&
      (context->token_to_scan != TOKEN_LOCAL_IDENTIFIER)) {
    return 1;
  }
  std::string name(context->token_value.string_val);

  directive = context->get_symbol(name);
  if (directive == 0) {
    context->set_error(UNDEFINED_IDENTIFIER);
    return 1;
  }
  context->token_to_scan = yylex();

  if (context->token_to_scan == '<') {
    isBracket = true;

    context->token_to_scan = yylex();
    if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
      isIntConst = true ;
      offset = context->token_value.int_val;
    } else if (context->token_to_scan == TOKEN_SREGISTER) {
      std::string regName = context->token_value.string_val;
      if (Identifier(context)) {
        return 1;
      }
      reg = context->operand_map[regName];
    } else {
      context->set_error(MISSING_OPERAND);
      return 1;
    }
    context->token_to_scan = yylex();

    if ((!isIntConst) && ((context->token_to_scan == '+') ||
          (context->token_to_scan == '-'))) {
      int sign = 1;
      if (context->token_to_scan == '-') {
        sign = -1;
      }

      context->token_to_scan = yylex();
      if (context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
        context->set_error(MISSING_INTEGER_CONSTANT);
        return 1;
      }
      offset = context->token_value.int_val * sign;

      context->token_to_scan = yylex();
    }

    if (context->token_to_scan != '>') {
      context->set_error(MISSING_CLOSING_BRACKET);
      return 1;
    }
    context->token_to_scan = yylex();
  }
  // end with ]
  if (context->token_to_scan != ']') {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (!IsImageOrSampler && !isBracket) {
    BrigOperandAddress boa = {
      sizeof(boa),               // size
      BrigEOperandAddress,       // kind
      Brigb32,                   // type
      0,                         // reserved
      directive                  // directive
    };

    //TODO(Chuang): This Comparison for uint test.
    if (directive != 0) {
      if (context->get_machine() == BrigELarge) {
        BrigSymbolCommon symbol;
        char* pSym = reinterpret_cast<char*>(&symbol);
        context->get_directive_bytes(
            pSym, directive + sizeof(uint16_t) * 2, sizeof(symbol));
        switch (symbol.storageClass) {
          case BrigFlatSpace:
          case BrigGlobalSpace:
          case BrigReadonlySpace:
          case BrigKernargSpace:
            boa.type = Brigb64; break;
          default:
            boa.type = Brigb32;
        }
      }
    } else {
      if (context->get_machine() == BrigELarge) {
        boa.type = Brigb64;
      }
    }
    *pRetOpOffset = context->get_operand_offset();
    context->append_operand(&boa);
  } else if (IsImageOrSampler) {
    BrigOperandOpaque boo = {
      sizeof(BrigOperandOpaque), // size
      BrigEOperandOpaque,        // kind
      directive,                 // directive
      reg,                       // reg
      offset                     // offset
    };

    *pRetOpOffset = context->get_operand_offset();
    context->append_operand(&boo);
  } else {
    context->set_error(INVALID_OPERATION);
    return 1;
  }

  context->token_to_scan = yylex();
  return 0;
}

int ArrayOperandList(Context* context) {
  BrigoOffset32_t opOffset = 0;
  return ArrayOperandList(context, &opOffset);
}

int ArrayOperandList(Context* context, BrigoOffset32_t* pRetOpOffset) {
  unsigned int count_op = 0;
  BrigoOffset32_t offsets[4] = {0};

  // first token must be '('
  if ('(' != context->token_to_scan)
    return 1;

  while (1) {
    context->token_to_scan = yylex();
    if (count_op > 3){
        context->set_error(INVALID_OPERAND);
        return 1;
    }
    if (context->token_type != REGISTER || Operand(context, &offsets[count_op])) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
    ++count_op;
    if (context->token_to_scan == ')') {
      break;
    } else if (context->token_to_scan == ',') {
      continue;
    } else {
      context->set_error(MISSING_CLOSING_PARENTHESIS);
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
      *pRetOpOffset = offsets[0];
      break;
    }
    case 2: {
      BrigOperandRegV2 oper_regV2 = {
        sizeof(BrigOperandRegV2),// size
        BrigEOperandRegV2,     // kind
        Brigb32,               // type
        0,                     // reserved
        {0, 0}                 // regs
      };
      BrigOperandReg elements[2];
      context->get_operand_bytes(
          (char *)(&elements[0]), offsets[0], sizeof(BrigOperandReg));
      context->get_operand_bytes(
          (char *)(&elements[1]), offsets[1], sizeof(BrigOperandReg));
      if(elements[0].type != elements[1].type){
        context->set_error(INVALID_OPERAND);
        return 1;
      }
      oper_regV2.regs[0] = offsets[0];
      oper_regV2.regs[1] = offsets[1];
      oper_regV2.type = elements[0].type;

      *pRetOpOffset = context->get_operand_offset();
      context->append_operand(&oper_regV2);

      break;
    }
    case 4: {
      BrigOperandRegV4 oper_regV4 = {
        sizeof(BrigOperandRegV4),// size
        BrigEOperandRegV4,     // kind
        Brigb32,               // type
        0,                     // reserved
        {0, 0, 0, 0}           // regs
      };
      BrigOperandReg elements[4];
      context->get_operand_bytes(
          (char *)(&elements[0]), offsets[0], sizeof(BrigOperandReg));
      context->get_operand_bytes(
          (char *)(&elements[1]), offsets[1], sizeof(BrigOperandReg));
      context->get_operand_bytes(
          (char *)(&elements[2]), offsets[2], sizeof(BrigOperandReg));
      context->get_operand_bytes(
          (char *)(&elements[3]), offsets[3], sizeof(BrigOperandReg));
      if((elements[0].type != elements[1].type) ||
          (elements[0].type != elements[2].type) ||
         (elements[0].type != elements[3].type)){
        context->set_error(INVALID_OPERAND);
        return 1;
      }
      oper_regV4.regs[0] = offsets[0];
      oper_regV4.regs[1] = offsets[1];
      oper_regV4.regs[2] = offsets[2];
      oper_regV4.regs[3] = offsets[3];
      oper_regV4.type = elements[0].type;

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
  std::vector<BrigoOffset32_t> arg_offset;
  std::string funcName;

  if (context->token_to_scan != '[') {
    if (context->token_to_scan == TOKEN_GLOBAL_IDENTIFIER) {
      funcName = context->token_value.string_val;

      BrigOperandFunctionRef opFunRef = {
        0,
        BrigEOperandFunctionRef,
        0
      };
      // TODO(Chuang): judge whether the identifier has been defined.
      opFunRef.size = sizeof(opFunRef);
      opFunRef.fn = context->func_map[funcName];
      context->arg_map[funcName] = context->get_operand_offset();
      context->append_operand(&opFunRef);

      BrigOperandArgumentList funcList = {
        sizeof(BrigOperandArgumentList),
        BrigEOperandFunctionList,
        1,
        { context->arg_map[funcName] }
      };

      context->current_argList_offset = context->get_operand_offset();
      context->append_operand(&funcList);
      context->token_to_scan = yylex();
      return 0;
    } else {
      return 1;
    }
  }
  while (1) {
    context->token_to_scan = yylex();  // set context for Identifier()
    // TODO(Chuang): the name of function must be global identifiers.
    if (context->token_to_scan == TOKEN_GLOBAL_IDENTIFIER) {
      funcName = context->token_value.string_val;
    } else {
      context->set_error(MISSING_IDENTIFIER);
      return 1;
    }
    if (!Identifier(context)) {
      if (context->func_o_map.count(funcName)) {
        arg_offset.push_back(context->func_o_map[funcName]);
      } else {
        BrigOperandFunctionRef opArgRef = {
          sizeof(BrigOperandFunctionRef),
          BrigEOperandFunctionRef,
          0
        };
        // TODO(Chuang): judge whether the identifier has been defined.
        opArgRef.fn = context->func_map[funcName];
        context->func_o_map[funcName] = context->get_operand_offset();
        context->append_operand(&opArgRef);
        arg_offset.push_back(context->func_o_map[funcName]);
      }
      context->token_to_scan = yylex();
      if (context->token_to_scan == ']') {
      size_t list_size = sizeof(BrigOperandArgumentList);
      if (arg_offset.size() > 1) {
        list_size += sizeof(BrigoOffset32_t) * (arg_offset.size() - 1);
      } else if (arg_offset.size() == 0) {
        // there is one identifier at least.
        context->set_error(MISSING_IDENTIFIER);
        return 1;
      }

      char *array = new char[list_size];
      BrigOperandArgumentList &funcList =
        *reinterpret_cast<BrigOperandArgumentList *>(array);
        funcList.size = list_size;
        funcList.kind = BrigEOperandFunctionList;
        funcList.elementCount = arg_offset.size();
        for (uint32_t i = 0; i < arg_offset.size(); ++i) {
          funcList.o_args[i] = arg_offset[i];
        }

        context->current_argList_offset = context->get_operand_offset();
        context->append_operand(&funcList);

        context->token_to_scan = yylex();  // set context for following function
        delete[] array;

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
  std::vector<BrigoOffset32_t> arg_offset;

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
      if (arg_offset.size() > 1) {
        list_size += sizeof(BrigoOffset32_t) * (arg_offset.size() - 1);
      }

      char *array = new char[list_size];
      BrigOperandArgumentList &arg_list =
        *reinterpret_cast<BrigOperandArgumentList *>(array);
      arg_list.size = list_size;
      arg_list.kind = BrigEOperandArgumentList;
      arg_list.elementCount = arg_offset.size();
      for (uint32_t i = 0; i < arg_offset.size(); ++i) {
        arg_list.o_args[i] = arg_offset[i];
      }
      if (!arg_offset.size()) {
        // this is an empty Argument List.
        arg_list.o_args[0] = 0;
      }

      context->current_argList_offset = context->get_operand_offset();
      context->append_operand(&arg_list);
      context->token_to_scan = yylex();

      delete[] array;
      break;
    } else if (saved_token == TOKEN_GLOBAL_IDENTIFIER ||
               saved_token == TOKEN_LOCAL_IDENTIFIER) {

      BrigOperandArgumentRef opArgRef = {
        sizeof(BrigOperandArgumentRef),
        BrigEOperandArgumentRef,
        0
      };
      opArgRef.arg = context->get_symbol(arg_name);
      if (opArgRef.arg == 0) {
        context->set_error(UNDEFINED_IDENTIFIER);
        return 1;
      }
      context->arg_map[arg_name] = context->get_operand_offset();
      context->append_operand(&opArgRef);

      arg_offset.push_back(context->arg_map[arg_name]);
      context->token_to_scan = yylex();
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
  BrigAluModifier mod = {0, 0, 0, 0, 0, 0, 0};

  if (first_token == _FTZ) {
    mod.valid = 1;
    mod.ftz = 1;
    mod.floatOrInt = 1;
    context->token_to_scan = yylex();
  }

  if (context->token_type == FLOAT_ROUNDING) {
    if (mod.valid) {
      if (!mod.floatOrInt) {
        context->set_error(INVALID_MODIFIER);
        return 1;
      }
    } else {
      mod.valid = 1;
      mod.floatOrInt = 1;
    }
    switch (context->token_to_scan) {
      case _UP: mod.rounding = 2; break;
      case _DOWN: mod.rounding = 3; break;
      case _ZERO: mod.rounding = 1; break;
      case _NEAR: mod.rounding = 0; break;
    }
    context->token_to_scan = yylex();
  } else if (context->token_type == INT_ROUNDING) {
    if (mod.valid) {
      if (mod.floatOrInt) {
        context->set_error(INVALID_MODIFIER);
        return 1;
      }
    } else {
      mod.valid = 1;
      mod.floatOrInt = 0;
    }
    switch (first_token) {
      case _UPI: mod.rounding = 2; break;
      case _DOWNI: mod.rounding = 3; break;
      case _ZEROI: mod.rounding = 1; break;
      case _NEARI: mod.rounding = 0; break;
    }
    context->token_to_scan = yylex();
  }

  context->set_alu_modifier(mod);
  return 0;
}

int Instruction2OpcodeDT(Context* context) {

  BrigInstBase inst = {
    sizeof(BrigInstBase),
    BrigEInstBase,
    0,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };

  BrigAluModifier aluModifier = {0, 0, 0, 0, 0, 0, 0};

  inst.opcode = context->token_value.opcode;

  context->token_to_scan = yylex();
  if (RoundingMode(context)) {
    context->set_error(INVALID_MODIFIER);
    return 1;
  }
  aluModifier = context->get_alu_modifier();
  if (context->token_type == PACKING) {
    // there is packing
    inst.packing = context->token_value.packing;
    if (inst.packing != BrigPackS && inst.packing != BrigPackP) {
      context->set_error(INVALID_PACKING);
      return 1;
    }
    context->token_to_scan = yylex();
  }
  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  inst.type = context->token_value.data_type;
  context->token_to_scan = yylex();
  if (!CheckDestRegister(context->token_to_scan, inst.type, inst.opcode)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (inst.opcode == BrigMovsLo || inst.opcode == BrigMovsHi) {
    context->set_type(Brigb32);
    if (context->token_to_scan != TOKEN_SREGISTER) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
  }
  if (Operand(context, &inst.o_operands[0])) {
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  if (inst.opcode == BrigMovsLo || inst.opcode == BrigMovsHi) {
    context->set_type(Brigb64);
    if (context->token_to_scan != TOKEN_DREGISTER) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
  }
  if (Operand(context, &inst.o_operands[1])) {
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  if (aluModifier.valid) {
    BrigInstMod mod = {
      sizeof(BrigInstMod),  // size
      BrigEInstMod,         // kind
      inst.opcode,              // opcode
      inst.type,         // type
      inst.packing,        // packing
      {0, 0, 0, 0, 0},      // o_operands[5]
      {0, 0, 0, 0, 0, 0, 0}  // aluModifier
    };
    for (int i = 0 ; i < 5 ; ++i) {
      mod.o_operands[i] = inst.o_operands[i];
    }
    mod.aluModifier = aluModifier;
    context->append_code(&mod);
  } else {
    context->append_code(&inst);
  }
  context->token_to_scan = yylex();
  return 0;
}

int Instruction2OpcodeNoDT(Context* context) {

  BrigInstBase inst = {
    sizeof(BrigInstBase),
    BrigEInstBase,
    0,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };

  BrigAluModifier aluModifier = {0, 0, 0, 0, 0, 0, 0};

  inst.opcode = context->token_value.opcode;

  context->token_to_scan = yylex();
  if (RoundingMode(context)) {
    context->set_error(INVALID_MODIFIER);
    return 1;
  }
  aluModifier = context->get_alu_modifier();
  if (context->token_type != REGISTER) {
    context->set_error(INVALID_FIRST_OPERAND);
    return 1;
  }
  if (Operand(context, &inst.o_operands[0])) {
      return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  if (Operand(context, &inst.o_operands[1])) {
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  if (aluModifier.valid) {
    BrigInstMod mod = {
      sizeof(BrigInstMod),  // size
      BrigEInstMod,         // kind
      inst.opcode,              // opcode
      inst.type,         // type
      inst.packing,        // packing
      {0, 0, 0, 0, 0},      // o_operands[5]
      {0, 0, 0, 0, 0, 0, 0}  // aluModifier
    };
    for (int i = 0 ; i < 5 ; ++i) {
      mod.o_operands[i] = inst.o_operands[i];
    }
    mod.aluModifier = aluModifier;
    context->append_code(&mod);
  } else {
    context->append_code(&inst);
  }
  context->token_to_scan = yylex();
  return 0;
}

int Instruction2OpcodeFtz(Context* context) {

  /* Variables for storing Brig struct information */
  BrigDataType16_t type = Brigb32;
  BrigOpcode32_t opcode = context->token_value.opcode;
  BrigPacking16_t packing = BrigNoPacking;
  BrigoOffset32_t OpOffset0 = 0, OpOffset1 = 0;
  BrigAluModifier aluModifier = {0, 0, 0, 0, 0, 0, 0};

  /* Check for rounding mode*/
  context->token_to_scan = yylex();
  if (RoundingMode(context)) {
    context->set_error(INVALID_MODIFIER);
    return 1;
  }
  aluModifier = context->get_alu_modifier();

  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;
  context->token_to_scan = yylex();

  if (!CheckDestRegister(context->token_to_scan, type, opcode)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset0)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  if (Operand(context, &OpOffset1)) {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  if (aluModifier.valid) {
    BrigInstMod mod = {
      0,  // size
      BrigEInstMod,         // kind
      opcode,              // opcode
      type,         // type
      packing,        // packing
      {OpOffset0, OpOffset1, 0, 0, 0},      // o_operands[5]
      {0, 0, 0, 0, 0, 0, 0}  // aluModifier
    };
    mod.size = sizeof(mod);
    mod.aluModifier = aluModifier;
    context->append_code(&mod);
  } else {
    BrigInstBase inst = {
      0,
      BrigEInstBase,
      opcode,
      type,
      packing,
      {OpOffset0, OpOffset1, 0, 0, 0}
    };
    inst.size = sizeof(inst);
    context->append_code(&inst);
  }
  context->token_to_scan = yylex();
  return 0;
}

int Instruction2(Context* context) {
  switch (context->token_type) {
    case INSTRUCTION2_OPCODE:
      return Instruction2OpcodeDT(context);
    case INSTRUCTION2_OPCODE_NODT:
      return Instruction2OpcodeNoDT(context);
    case INSTRUCTION2_OPCODE_FTZ:
      return Instruction2OpcodeFtz(context);
    default:
      return 1;
  }
  return 1;
}

int Instruction3(Context* context) {
  // First token must be an Instruction3Opcode

  /* Variables for storing Brig struct information */
  BrigDataType16_t type = Brigb32;
  BrigOpcode32_t opcode = context->token_value.opcode;
  BrigPacking packing = BrigNoPacking;
  BrigoOffset32_t OpOffset0 = 0, OpOffset1 = 0, OpOffset2 = 0;
  BrigAluModifier mod = {0, 0, 0, 0, 0, 0, 0};

  /* Checking for Instruction3 statement  - no error set here*/
  if ((context->token_type != INSTRUCTION3_OPCODE)
    && (context->token_type != INSTRUCTION3_OPCODE_FTZ)) {
    return 1;
  }

  context->token_to_scan= yylex();

  /* Rounding Optional*/
  if (RoundingMode(context)) {
    context->set_error(INVALID_MODIFIER);
    return 1;
  }
  mod = context->get_alu_modifier();

  /*Packing optional*/
  if (context->token_type == PACKING) {
    packing = ((BrigPacking) context->token_value.packing);
    context->token_to_scan = yylex();
  }

  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  type = (context->token_value.data_type);
  type = (opcode == BrigClass) ? (int)Brigb1 : type;
  context->set_type(type);
  context->token_to_scan = yylex();
  if((opcode==BrigShr) || (opcode==BrigShl)) {
    packing = type >= Brigu8x4 ? BrigPackPS : packing;
  }

  if (!CheckRegister(context->token_to_scan, ConvertTypeToB(type))) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (Operand(context, &OpOffset0)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (opcode == BrigMovdHi || opcode == BrigMovdLo) {
    if (context->token_to_scan != TOKEN_DREGISTER) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
  }
  if (Operand(context, &OpOffset1)) {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (opcode == BrigMovdHi || opcode == BrigMovdLo) {
    if (context->token_to_scan != TOKEN_SREGISTER) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
  }
  if (opcode == BrigShl || opcode ==BrigShr) {
    context->set_type(Brigb32);
  }
  if (Operand(context, &OpOffset2)) {
    context->set_error(MISSING_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  if (mod.valid){
    BrigInstMod bim = {
      0,
      BrigEInstMod,
      opcode,
      type,
      packing,
      {OpOffset0, OpOffset1, OpOffset2, 0, 0},
      {0,0,0,0,0,0,0}
    };
    bim.size = sizeof(bim);
    bim.aluModifier = mod;
    context->append_code(&bim);
  } else {
    BrigInstBase bi = {
      0,
      BrigEInstBase,
      opcode,
      type,
      packing,
      {OpOffset0, OpOffset1, OpOffset2, 0, 0}
    };
    bi.size = sizeof(bi);
    context->append_code(&bi);
  }
  context->token_to_scan = yylex();
  return 0;
}

int Version(Context* context) {
  BrigDirectiveVersion bdv;
  bdv.kind = BrigEDirectiveVersion;
  bdv.size = sizeof(BrigDirectiveVersion);
  bdv.reserved = 0;
  bdv.c_code = context->get_code_offset();
  // set default values
  bdv.machine = BrigELarge;
  bdv.profile = BrigEFull;

  // first token must be version
  if (VERSION != context->token_to_scan) {
    return 1;
  }
  context->token_to_scan = yylex();

  if (TOKEN_INTEGER_CONSTANT != context->token_to_scan) {
    context->set_error(MISSING_INTEGER_CONSTANT);
    return 1;
  }
  bdv.major = context->token_value.int_val;
  if (!context->isFirstVersion) {
    if (bdv.major != context->get_major()) {
      context->set_error(INVALID_MAJOR_NUMBER);
      return 1;
    }
  } else {
    context->set_major(bdv.major);
  }

  context->token_to_scan = yylex();
  if (':' != context->token_to_scan) {
    context->set_error(MISSING_COLON);
    return 1;
  }
  context->token_to_scan = yylex();

  if (TOKEN_INTEGER_CONSTANT != context->token_to_scan) {
    context->set_error(MISSING_INTEGER_CONSTANT);
    return 1;
  }
  bdv.minor = context->token_value.int_val;
  if (!context->isFirstVersion) {
    if (bdv.minor != context->get_minor()) {
      context->set_error(INVALID_MINOR_NUMBER);
      return 1;
    }
  } else {
    context->set_minor(bdv.minor);
  }

  context->token_to_scan = yylex();

  if (':' == context->token_to_scan) {
    context->token_to_scan = yylex();
    while ( ';' != context->token_to_scan) {
      if (TARGET != context->token_type) {
        context->set_error(INVALID_TARGET);
        return 1;
      }
      switch (context->token_to_scan) {
        case _SMALL: bdv.machine = BrigESmall; break;
        case _LARGE: bdv.machine = BrigELarge; break;
        case _FULL: bdv.profile = BrigEFull; break;
        case _BASE: bdv.profile = BrigEBase; break;
      }
      context->set_machine(bdv.machine);
      context->set_profile(bdv.profile);

      context->token_to_scan = yylex();
      if ((',' != context->token_to_scan) &&
          (';' != context->token_to_scan)) {
        context->set_error(MISSING_SEMICOLON);
        return 1;
      }

      if (',' == context->token_to_scan) {
        context->token_to_scan = yylex();
      }
    }
  }
  if (';' != context->token_to_scan) {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }

  context->append_directive(&bdv);
  context->token_to_scan = yylex();
  context->set_error(OK);
  context->isFirstVersion = false;
  return 0;
}

int Alignment(Context* context) {
  // first token must be "align" keyword
  if (ALIGN != context->token_to_scan) {
    return 1;
  }

  context->token_to_scan = yylex();
  if (TOKEN_INTEGER_CONSTANT != context->token_to_scan){
    context->set_error(MISSING_INTEGER_CONSTANT);
    return 1;
  }
  if (context->token_value.int_val != 1 &&
      context->token_value.int_val != 2 &&
      context->token_value.int_val != 4 &&
      context->token_value.int_val != 8 &&
      context->token_value.int_val != 16) {
    context->set_error(INVALID_ALIGNMENT);
    return 1;
  }

  context->set_alignment(context->token_value.int_val);
  context->token_to_scan = yylex();
  return 0;
}

int DeclPrefix(Context* context){
  context->set_attribute(BrigNone);

  if ((context->token_to_scan == ALIGN)  ||
     (context->token_to_scan == EXTERN) ||
     (context->token_to_scan == STATIC) ||
     (context->token_to_scan == CONST)) {
    switch (context->token_to_scan){
      case ALIGN:
        if (Alignment(context))
          return 1;
        break;
      case CONST:
        context->set_symbol_modifier(BrigConst);
        context->token_to_scan = yylex();
        break;
      default:
        if (context->token_to_scan == EXTERN) {
          context->set_attribute(BrigExtern);
          context->token_to_scan = yylex();
        } else if (context->token_to_scan == STATIC) {
          context->set_attribute(BrigStatic);
          context->token_to_scan = yylex();
        }
    }
  } else
    return 0;

  if ((context->token_to_scan == ALIGN) ||
     (context->token_to_scan == EXTERN) ||
     (context->token_to_scan == STATIC) ||
     (context->token_to_scan == CONST)) {
    switch (context->token_to_scan) {
      case ALIGN:
        if (Alignment(context))
          return 1;
        break;
      case CONST:
        context->set_symbol_modifier(BrigConst);
        context->token_to_scan = yylex();
        break;
      default:
        if (context->token_to_scan == EXTERN) {
          context->set_attribute(BrigExtern);
          context->token_to_scan = yylex();
        } else if (context->token_to_scan == STATIC) {
          context->set_attribute(BrigStatic);
          context->token_to_scan = yylex();
        }
    }
  } else
    return 0;
  if ((context->token_to_scan == ALIGN) ||
     (context->token_to_scan == EXTERN) ||
     (context->token_to_scan == STATIC) ||
     (context->token_to_scan == CONST)) {
    switch (context->token_to_scan) {
      case ALIGN:
        if (Alignment(context))
          return 1;
        break;
      case CONST:
        context->set_symbol_modifier(BrigConst);
        context->token_to_scan = yylex();
        break;
      default:
        if (context->token_to_scan == EXTERN) {
          context->set_attribute(BrigExtern);
          context->token_to_scan = yylex();
        } else if (context->token_to_scan == STATIC) {
          context->set_attribute(BrigStatic);
          context->token_to_scan = yylex();
        }
    }
  }
  return 0;
}

int ArrayDimensionSet(Context* context) {
  uint32_t dim = 1;
  bool have_size = false;

  // first token must be '['
  if ('[' != context->token_to_scan) {
    return 1;
  }
  context->set_isArray(true);
  context->token_to_scan = yylex();

  while (1) {
    if (TOKEN_INTEGER_CONSTANT == context->token_to_scan ) {
      have_size = true;
      dim *= context->token_value.int_val;
      context->token_to_scan = yylex();
    }
    if (']' != context->token_to_scan) {
      context->set_error(MISSING_CLOSING_BRACKET);
      return 1;
    }
    context->token_to_scan = yylex();
    if ('[' == context->token_to_scan)
      context->token_to_scan = yylex();
    else
      break;
  }

  if (!have_size) {
    context->set_dim(0);
    context->set_symbol_modifier(BrigFlex);
  } else {
    context->set_dim(dim);
    context->set_symbol_modifier(BrigArray);
  }
  return 0;
}

int ArgumentDecl(Context* context) {

  BrigDataType16_t type;
  if (DeclPrefix(context)) {
    return 1;
  }

  context->set_dim(0);
  BrigStorageClass32_t storage_class = context->token_value.storage_class;

  if(context->token_to_scan != ARG){
    return 1;
  }
  context->token_to_scan = yylex();
  if ((context->token_type != DATA_TYPE_ID)&&
      (context->token_to_scan != _RWIMG) &&
      (context->token_to_scan != _SAMP) &&
      (context->token_to_scan != _ROIMG)) {

    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  type = (context->token_value.data_type);

  context->token_to_scan = yylex();
  if (context->token_to_scan != TOKEN_LOCAL_IDENTIFIER) {
    context->set_error(MISSING_IDENTIFIER);
    return 1;
  }
  // for argument, we need to set a BrigDirectiveSymbol
  // and write the corresponding string into .string section.

  std::string arg_name = context->token_value.string_val;
  if (context->local_symbol_map.count(arg_name)) {
    context->set_error(REPEATED_DECLARATION);
    return 1;
  }
  int arg_name_offset = context->add_symbol(arg_name);

  // scan for arrayDimensions
  context->token_to_scan = yylex();

  if (context->token_to_scan == '[') {
    if (ArrayDimensionSet(context)) {
      // context->token_to_scan has been set in ArrayDimensionSet()
      return 1;
    }
  }
  uint16_t naturalAlign = 0;
  switch (ConvertTypeToB(type)) {
    case Brigb1: naturalAlign = 1; break;
    case Brigb8: naturalAlign = 1; break;
    case Brigb16: naturalAlign = 2; break;
    case Brigb32: naturalAlign = 4; break;
    case Brigb64: naturalAlign = 8; break;
    case Brigb128: naturalAlign = 16; break;
    case BrigRWImg: naturalAlign = 16; break;
    case BrigROImg: naturalAlign = 16; break;
    case BrigSamp: naturalAlign = 16; break;
  }

  if (context->get_alignment() == 0) {
    context->set_alignment(naturalAlign);
  } else if (context->get_alignment() < naturalAlign) {
    context->set_error(INVALID_ALIGNMENT);
    return 1;
  }

  BrigdOffset32_t dsize = context->get_directive_offset();
  BrigDirectiveSymbol sym_decl = {
    sizeof(BrigDirectiveSymbol),        // size
    BrigEDirectiveSymbol,               // kind
    {
      context->get_code_offset(),       // c_code
      storage_class,                    // storageClass
      context->get_attribute(),         // attribute
      0,                                // reserved
      context->get_symbol_modifier(),   // symbol modifier
      context->get_dim(),               // dim
      arg_name_offset,                  // s_name
      type,                             // data type
      context->get_alignment(),         // alignment
    },
    0,                                  // d_init = 0 for arg
    0                                   // reserved
  };

  context->append_directive(&sym_decl);
  context->local_symbol_map[arg_name] = dsize;
  context->set_alignment(0);

  return 0;
}

int ArgumentListBody(Context* context) {
  int paramCount = 0;
  if(context->token_to_scan == '('){
    context->token_to_scan = yylex();
    return ArgumentListBody(context, &paramCount);
  }
  else return 1;
}

int ArgumentListBody(Context* context, int* paramCount) {
  *paramCount = 0;

  while (1) {
    if(context->token_to_scan == ')')
      return 0;
    if (ArgumentDecl(context))
      return 1;
    (*paramCount)++;
    if (context->token_to_scan == ',') {
      context->token_to_scan = yylex();
    }
  }
}

int FunctionDefinition(Context* context){
  if(!DeclPrefix(context)){
    return FunctionDefinitionPart2(context);
  } else return 1;
}

int FunctionDefinitionPart2(Context* context) {

  if (context->token_to_scan!= FUNCTION) {
    return 1;
  }

  BrigdOffset32_t bdf_offset = context->get_directive_offset();
  context->set_bdf_offset(bdf_offset);

  context->token_to_scan = yylex();
  if (context->token_to_scan != TOKEN_GLOBAL_IDENTIFIER) {
    context->set_error(MISSING_IDENTIFIER);
    return 1;
  }

  std::string func_name = context->token_value.string_val;
  BrigsOffset32_t str_offset = context->add_symbol(func_name);

  if (context->func_map.count(func_name)) {
    context->set_error(REPEATED_DECLARATION);
    return 1;
  }
  context->func_map[func_name] = bdf_offset;
  uint16_t size = sizeof(BrigDirectiveFunction);
  BrigDirectiveFunction bdf = {
    size,                      // size
    BrigEDirectiveFunction,    // kind
    context->get_code_offset(),   // c_code
    str_offset,                            // name
    0,                          // in param count
    bdf_offset + size,          // d_firstScopedDirective
    0,                          // operation count
    bdf_offset + size,          // d_nextDirective
    context->get_attribute(),  // attribute
    0,    // fbar count
    0,    // out param count
    0     // d_firstInParam
  };
  context->append_directive(&bdf);

  /*Check output argument list*/
  context->token_to_scan = yylex();
  if (context->token_to_scan != '(') {
    context->set_error(MISSING_ARGUMENT_LIST);
    return 1;
  }
  context->token_to_scan = yylex();
  int paramCount = 0;
  if(ArgumentListBody(context, &paramCount)){
    context->set_error(INVALID_ARGUMENT_LIST);
    return 1;
  }
  if (paramCount > 1) {
    context->set_error(INVALID_ARGUMENT_LIST);
    return 1;
  }
  if (context->token_to_scan != ')') {
    context->set_error(MISSING_CLOSING_PARENTHESIS);
    return 1;
  }
  if(paramCount){
    context->get_directive(bdf_offset, &bdf);
    bdf.outParamCount = paramCount;
    bdf.d_nextDirective += paramCount*sizeof(BrigDirectiveSymbol);
    bdf.d_firstScopedDirective += paramCount*sizeof(BrigDirectiveSymbol);
    unsigned char * bdf_charp = reinterpret_cast<unsigned char*>(&bdf);
    context->update_directive_bytes(bdf_charp, bdf_offset,
                                  sizeof(BrigDirectiveFunction));
  }
  context->token_to_scan = yylex();

  /* Check input argument list*/

  if (context->token_to_scan != '(') {
    context->set_error(MISSING_ARGUMENT_LIST);
    return 1;
  }
  context->token_to_scan = yylex();
  paramCount = 0;
  if(ArgumentListBody(context, &paramCount)){
    context->set_error(INVALID_ARGUMENT_LIST);
    return 1;
  }
  if (context->token_to_scan != ')') {
    context->set_error(MISSING_CLOSING_PARENTHESIS);
    return 1;
  }
  if(paramCount){
    context->get_directive(bdf_offset, &bdf);
    bdf.inParamCount = paramCount;
    bdf.d_firstInParam = bdf.d_nextDirective;
    bdf.d_nextDirective += paramCount*sizeof(BrigDirectiveSymbol);
    bdf.d_firstScopedDirective += paramCount*sizeof(BrigDirectiveSymbol);
    unsigned char * bdf_charp = reinterpret_cast<unsigned char*>(&bdf);
    context->update_directive_bytes(bdf_charp, bdf_offset,
                                  sizeof(BrigDirectiveFunction));
  }

  context->token_to_scan = yylex();
  return 0;
}

int FunctionDecl(Context *context){
  if (!DeclPrefix(context)) {
    if (!FunctionDefinitionPart2(context)){
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
  if ('{' != context->token_to_scan) {
    return 1;
  }
  BrigDirectiveScope argblock_start = {
    sizeof(BrigDirectiveScope),
    BrigEDirectiveArgStart,
    context->get_code_offset()
  };
  context->append_directive(&argblock_start);

  context->token_to_scan = yylex();
  if (ArgStatements(context)) {
    context->set_error(INVALID_ARG_BLOCK);
    return 1;
  }
  BrigDirectiveScope argblock_end = {
    sizeof(BrigDirectiveScope),
    BrigEDirectiveArgEnd,
    context->get_code_offset()
  };
  context->append_directive(&argblock_end);

  if ('}' != context->token_to_scan) {
    context->set_error(MISSING_CLOSING_BRACKET);
    return 1;
  }
  context->token_to_scan = yylex();
  context->arg_symbol_map.clear();
  return 0;
}

int CodeBlockEnd(Context* context) {
  if ('}' != context->token_to_scan)
    return 1;

  context->token_to_scan = yylex();
  if (';' != context->token_to_scan) {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  context->token_to_scan = yylex();
  context->local_symbol_map.clear();
  return 0;
}

int Codeblock(Context* context) {
  // first token should be '{'
  if ('{' != context->token_to_scan)
    return 1;

  context->token_to_scan = yylex();
  if (BodyStatements(context)) {
    context->set_error(INVALID_CODEBLOCK);
    return 1;
  }
  BrigDirectiveFunction bdf;
  context->get_directive(context->get_bdf_offset(), &bdf);
  if (bdf.kind == BrigEDirectiveFunction) {
    bdf.d_nextDirective = context->get_directive_offset();
    unsigned char * bdf_charp = reinterpret_cast<unsigned char*>(&bdf);
    context->update_directive_bytes(bdf_charp,
                             context->get_bdf_offset(),
                             sizeof(BrigDirectiveFunction));
  } else if (bdf.kind == BrigEDirectiveKernel) {
    BrigDirectiveKernel* bdk = reinterpret_cast<BrigDirectiveKernel*> (&bdf);
    bdk->d_nextDirective = context->get_directive_offset();
    unsigned char * bdf_charp = reinterpret_cast<unsigned char*>(bdk);
    context->update_directive_bytes(bdf_charp,
                               context->get_bdf_offset(),
                               sizeof(BrigDirectiveKernel));
  }
  if ('}' != context->token_to_scan) {
    context->set_error(MISSING_CLOSING_BRACKET);
    return 1;
  }
  return CodeBlockEnd(context);
}

int Functionpart2(Context *context){
  if (!Codeblock(context)){
    return 0;
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
  // first token must be VERSION
  if (VERSION != context->token_to_scan) {
    context->set_error(MISSING_VERSION_STATEMENT);
    return 1;
  }
  if (!Version(context)) {
    return TopLevelStatements(context);
  }
  return 1;
}

int OptionalWidth(Context* context) {

  uint32_t width_param = 0;
  // first token must be _WIDTH
  if (context->token_to_scan != _WIDTH)
      return 1;
  context->token_to_scan = yylex();

  if (context->token_to_scan != '(') {
    context->set_error(MISSING_WIDTH_INFO);
    return 1;
  }
  context->token_to_scan = yylex();

  if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
    width_param = context->token_value.int_val;
  } else if (context->token_to_scan == ALL){
    width_param = 0;
  } else {
    context->set_error(INVALID_WIDTH_NUMBER);
    return 1;
  }
  context->token_to_scan = yylex();

  if (context->token_to_scan != ')') {
    context->set_error(MISSING_CLOSING_PARENTHESIS);
    return 1;
  }
  BrigOperandImmed op_width = {
    sizeof(BrigOperandImmed),
    BrigEOperandImmed,
    Brigb32,
    0,
    { 0 }
  };
  op_width.size = sizeof(op_width);
  if ((width_param <= 1024) &&
      ((width_param & (width_param - 1)) == 0)) {
    op_width.bits.u  = width_param;
  } else {
    context->set_error(INVALID_WIDTH_NUMBER);
    return 1;
  }
  context->append_operand(&op_width);
  context->token_to_scan = yylex();
  return 0;
}

int BranchCbr(Context* context) {

  BrigAluModifier mod = {0, 0, 0, 0, 0, 0, 0};
  BrigoOffset32_t OpOffset[4] = {0,0,0,0};

  BrigoOffset32_t widthOffset = context->get_operand_offset();
  if (widthOffset % 8) {
    widthOffset += 8 - widthOffset % 8;
  }

  // check for optionalWidth
  if (context->token_to_scan == _WIDTH) {
    if (OptionalWidth(context)) {
      return 1;
    }
  } else {
    BrigOperandImmed op_width = {
      sizeof(BrigOperandImmed),
      BrigEOperandImmed,
      Brigb32,
      0,
      { 0 }
    };
    op_width.bits.u = 0;
    context->append_operand(&op_width);
  }

  // check for optional _fbar modifier
  if (context->token_to_scan == __FBAR) {
    mod.valid = 1;
    mod.fbar = 1;
    context->set_alu_modifier(mod);
    context->token_to_scan = yylex();
  }

  OpOffset[0] = widthOffset;
  if (context->token_to_scan != TOKEN_CREGISTER) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  // Generate $c register brig code
  if (Operand(context, &OpOffset[1])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  if (context->token_to_scan == TOKEN_LABEL) {
    std::string label_name = context->token_value.string_val;
    if (!context->label_o_map.count(label_name)) {
      BrigOperandLabelRef opLabelRef = {
        sizeof(BrigOperandLabelRef),
        BrigEOperandLabelRef,
        0
      };
      context->label_o_map[label_name] = context->get_operand_offset();
      opLabelRef.labeldirective = context->get_symbol(label_name);

      context->append_operand(&opLabelRef);
    }
    OpOffset[2] = context->label_o_map[label_name];
    context->token_to_scan = yylex();  // should be ';'
  } else if (context->token_type == REGISTER) {
    if ((context->get_machine() == BrigELarge && 
         context->token_to_scan != TOKEN_DREGISTER) ||
        (context->get_machine() == BrigESmall &&
         context->token_to_scan != TOKEN_SREGISTER)) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
    if (Operand(context, &OpOffset[2])) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }

    if (context->token_to_scan == ',') {
      context->token_to_scan = yylex();
      if (context->token_to_scan != '[') {
        context->set_error(MISSING_OPENNING_BRACKET);
        return 1;
      }
      context->token_to_scan = yylex();
      if (context->token_to_scan == TOKEN_LABEL) {
        std::string label_name = context->token_value.string_val;
        if (!context->label_o_map.count(label_name)) {
          BrigOperandLabelRef opLabelRef = {
            sizeof(BrigOperandLabelRef),
            BrigEOperandLabelRef,
            0
          };
          context->label_o_map[label_name] = context->get_operand_offset();
          opLabelRef.labeldirective = context->get_symbol(label_name);

          context->append_operand(&opLabelRef);
        }

        OpOffset[3] = context->label_o_map[label_name];

      } else if (context->token_to_scan == TOKEN_GLOBAL_IDENTIFIER ||
                 context->token_to_scan == TOKEN_LOCAL_IDENTIFIER) {
        std::string idenName(context->token_value.string_val);
        BrigOperandAddress boa = {
          sizeof(boa),            // size
          BrigEOperandAddress,    // kind
          Brigb32,                // type
          0,                      // reserved
          0                       // directive
        };

        boa.directive = context->get_symbol(idenName);
        if (boa.directive == 0) {
          context->set_error(UNDEFINED_IDENTIFIER);
          return 1;
        }
        if (context->get_machine() == BrigELarge) {
          BrigSymbolCommon symbol;
          char* pSym = reinterpret_cast<char*>(&symbol);
          context->get_directive_bytes(
              pSym, boa.directive + sizeof(uint16_t) * 2, sizeof(symbol));
          switch (symbol.storageClass) {
            case BrigFlatSpace:
            case BrigGlobalSpace:
            case BrigReadonlySpace:
            case BrigKernargSpace:
              boa.type = Brigb64; break;
            default:
              boa.type = Brigb32;
          }
        }
        OpOffset[3] = context->get_operand_offset();
        context->append_operand(&boa);
      } else {  // Identifier or Label
        context->set_error(MISSING_OPERAND);
        return 1;
      }
      context->token_to_scan = yylex();
      if (context->token_to_scan != ']') {
        context->set_error(MISSING_CLOSING_BRACKET);
        return 1;
      }
      context->token_to_scan = yylex();
    }  // ','
  } else {  // register or Label
    context->set_error(MISSING_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }

  if (mod.fbar == 1) {
    BrigInstMod cbrMod = {
      0,
      BrigEInstMod,
      BrigCbr,
      Brigb32,
      BrigNoPacking,
      {OpOffset[0], OpOffset[1], OpOffset[2], OpOffset[3], 0},
      mod
    };
    cbrMod.size = sizeof(cbrMod);
    context->append_code(&cbrMod);
  } else {
    BrigInstBase cbrInst = {
      0,
      BrigEInstBase,
      BrigCbr,
      Brigb32,
      BrigNoPacking,
      {OpOffset[0], OpOffset[1], OpOffset[2], OpOffset[3], 0}
    };

    cbrInst.size = sizeof(cbrInst);
    context->append_code(&cbrInst);
  }
  context->token_to_scan = yylex();
  return 0;

}

int BranchBrn(Context* context) {

  BrigAluModifier mod = {0, 0, 0, 0, 0, 0, 0};
  BrigoOffset32_t OpOffset[3] = {0,0,0};

  BrigoOffset32_t widthOffset = context->get_operand_offset();
  if (widthOffset % 8) {
    widthOffset += 8 - widthOffset % 8;
  }

  OpOffset[0] = widthOffset;

  // check for optionalWidth
  if (context->token_to_scan == _WIDTH) {
    if (OptionalWidth(context)) {
      return 1;
    }
  } else {
    BrigOperandImmed op_width = {
      sizeof(BrigOperandImmed),
      BrigEOperandImmed,
      Brigb32,
      0,
      { 0 }
    };
    op_width.bits.u = 0;
    context->append_operand(&op_width);
  }
  // check for optional _fbar modifier
  if (context->token_to_scan == __FBAR) {
    mod.valid = 1;
    mod.fbar = 1;
    context->set_alu_modifier(mod);
    context->token_to_scan = yylex();
  }

  if (context->token_to_scan == TOKEN_LABEL) {

    std::string label_name = context->token_value.string_val;
    if (!context->label_o_map.count(label_name)) {
      BrigOperandLabelRef opLabelRef = {
        sizeof(BrigOperandLabelRef),
        BrigEOperandLabelRef,
        0
      };
      context->label_o_map[label_name] = context->get_operand_offset();
      opLabelRef.labeldirective = context->get_symbol(label_name);

      context->append_operand(&opLabelRef);
    }
    OpOffset[1] = context->label_o_map[label_name];
    context->token_to_scan = yylex();
  } else if (!Identifier(context)) {
    // Must be an s register.
    if ((context->get_machine() == BrigELarge && 
         context->token_to_scan != TOKEN_DREGISTER) ||
        (context->get_machine() == BrigESmall &&
         context->token_to_scan != TOKEN_SREGISTER)) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
    std::string regName = context->token_value.string_val;
    OpOffset[1] = context->operand_map[regName];
    context->token_to_scan = yylex();
    if (context->token_to_scan == ',') {
      context->token_to_scan = yylex();
      if (context->token_to_scan != '[') {
        context->set_error(MISSING_OPENNING_BRACKET);
        return 1;
      }
      context->token_to_scan = yylex();
      if (context->token_to_scan == TOKEN_LABEL) {

        std::string label_name = context->token_value.string_val;
        if (!context->label_o_map.count(label_name)) {
          BrigOperandLabelRef opLabelRef = {
            sizeof(BrigOperandLabelRef),
            BrigEOperandLabelRef,
            0
          };
          context->label_o_map[label_name] = context->get_operand_offset();
          opLabelRef.labeldirective = context->get_symbol(label_name);

          context->append_operand(&opLabelRef);
        }
        OpOffset[2] = context->label_o_map[label_name];
      } else if (context->token_to_scan == TOKEN_GLOBAL_IDENTIFIER ||
                 context->token_to_scan == TOKEN_LOCAL_IDENTIFIER) {
        std::string idenName(context->token_value.string_val);
        BrigOperandAddress boa = {
          sizeof(boa),            // size
          BrigEOperandAddress,    // kind
          Brigb32,                // type
          0,                      // reserved
          0                       // directive
        };

        boa.directive = context->get_symbol(idenName);
        if (boa.directive == 0) {
          context->set_error(UNDEFINED_IDENTIFIER);
          return 1;
        }
        if (context->get_machine() == BrigELarge) {
          BrigSymbolCommon symbol;
          char* pSym = reinterpret_cast<char*>(&symbol);
          context->get_directive_bytes(
              pSym, boa.directive + sizeof(uint16_t) * 2, sizeof(symbol));
          switch (symbol.storageClass) {
            case BrigFlatSpace:
            case BrigGlobalSpace:
            case BrigReadonlySpace:
            case BrigKernargSpace:
              boa.type = Brigb64; break;
            default:
              boa.type = Brigb32;
          }
        }

        OpOffset[2] = context->get_operand_offset();
        context->append_operand(&boa);
      } else {  // Identifier or Label
        context->set_error(MISSING_OPERAND);
        return 1;
      }
      context->token_to_scan = yylex();
      if (context->token_to_scan != ']') {
        context->set_error(MISSING_CLOSING_BRACKET);
        return 1;
      }
      context->token_to_scan = yylex();
    }
  } else {  // Identifier or Label
    context->set_error(MISSING_OPERAND);
    return 1;
  }


  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }

  if (*reinterpret_cast<uint32_t*>(&mod) == 0) {
    BrigInstBase brnInst = {
      sizeof(BrigInstBase),
      BrigEInstBase,
      BrigBrn,
      Brigb32,
      BrigNoPacking,
      {OpOffset[0], OpOffset[1], OpOffset[2], 0, 0}
    };
    context->append_code(&brnInst);
  } else {
    BrigInstMod brnMod = {
      sizeof(BrigInstMod),
      BrigEInstMod,
      BrigBrn,
      Brigb32,
      BrigNoPacking,
      {OpOffset[0], OpOffset[1], OpOffset[2], 0, 0},
      mod
    };
    context->append_code(&brnMod);
  }
  context->token_to_scan = yylex();
  return 0;

}

int Branch(Context* context) {
  unsigned int op = context->token_to_scan;  // CBR or BRN

  context->token_to_scan = yylex();

  if (op == CBR) {
    return BranchCbr(context);
  } else if (op == BRN) {
    return BranchBrn(context);
  }
  return 1;
}

int Call(Context* context) {
  // first token is "call"
  BrigoOffset32_t firstOpOffset = 0;
  BrigoOffset32_t secondOpOffset = 0;

  BrigoOffset32_t OpOffset[5] = {0,0,0,0,0};
  BrigAluModifier aluModifier = {0, 0, 0, 0, 0, 0, 0};

  context->token_to_scan = yylex();
  // optional width
  BrigoOffset32_t widthOffset = context->get_operand_offset();
  bool has_width = false;
  if (widthOffset % 8) {
    widthOffset += 8 - widthOffset % 8;
  }
  OpOffset[0] = widthOffset;

  if (context->token_to_scan == _WIDTH) {
    if (OptionalWidth(context)) {
      return 1;
    }
    has_width = true;
  } else{
    BrigOperandImmed op_width = {
      sizeof(BrigOperandImmed),
      BrigEOperandImmed,
      Brigb32,
      0,
      { 0 }
    };
    op_width.bits.u = 1;
    context->append_operand(&op_width);
  }

  if (context->token_to_scan == __FBAR) {
    aluModifier.valid = 1;
    aluModifier.fbar = 1;
    context->token_to_scan = yylex();
  }

  std::string opName;
  const unsigned int firstOpToken = context->token_to_scan;;

  int* paluModifier = reinterpret_cast<int*>(&aluModifier);
  if (firstOpToken == TOKEN_GLOBAL_IDENTIFIER && (*paluModifier==0) && !has_width) {
    opName.assign(context->token_value.string_val);
    if (!context->func_o_map.count(opName)) {
      BrigOperandFunctionRef func_o_ref = {
        sizeof(BrigOperandFunctionRef),
        BrigEOperandFunctionRef,
        context->func_map[opName]
      };
      context->func_o_map[opName] = context->get_operand_offset();
      context->append_operand(&func_o_ref);
    }
    OpOffset[2] = context->func_o_map[opName];
    context->token_to_scan = yylex();
  } else if (firstOpToken == TOKEN_SREGISTER ||
             firstOpToken == TOKEN_DREGISTER) {
    if (Operand(context, &OpOffset[2])) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
  } else {
    context->set_error(MISSING_OPERAND);
    return 1;
  }

  BrigOperandArgumentList emptylist = {
    sizeof(BrigOperandArgumentList),
    BrigEOperandArgumentList,
    0,
    {0}
  };
  // check for twoCallArgs
  //operands[1] contains output args and [3] contains input args
  if (context->token_to_scan == '(') {
    if (CallArgs(context)) {
      context->set_error(INVALID_CALL_ARGS);
      return 1;
    }
    firstOpOffset = context->current_argList_offset;
    if (context->token_to_scan == '(') {
      if (CallArgs(context)) {
        context->set_error(INVALID_CALL_ARGS);
        return 1;
      }
      secondOpOffset = context->current_argList_offset;
    } else {
      secondOpOffset = firstOpOffset; //contains input args
      firstOpOffset = context->get_operand_offset(); //contains output args
      context->append_operand(&emptylist);
    }
  } else {
    firstOpOffset = context->get_operand_offset(); //Assuming both input and output args missing
    secondOpOffset = firstOpOffset;
    context->append_operand(&emptylist);
  }

  OpOffset[1] = firstOpOffset;
  BrigOperandArgumentList outputList;
  context->get_operand(firstOpOffset, &outputList);
  if (outputList.elementCount > 1) {
    context->set_error(INVALID_CALL_ARGS);
    return 1;
  }
  OpOffset[3] = secondOpOffset;

  // if there is CallTarget, the first operand token must be a s register.
  if (firstOpToken == TOKEN_SREGISTER ||
      firstOpToken == TOKEN_DREGISTER) {
    if (CallTargets(context)) {
      context->set_error(INVALID_CALL_TARGETS);
      return 1;
    }
    OpOffset[4] = context->current_argList_offset;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }

  if (aluModifier.valid) {
    BrigInstMod callMod = {
      0,
      BrigEInstMod,
      BrigCall,
      Brigb32,
      BrigNoPacking,
      {OpOffset[0], OpOffset[1], OpOffset[2], OpOffset[3], OpOffset[4]},
      aluModifier
    };
    callMod.aluModifier.valid = 1;
    callMod.size = sizeof(callMod);
    context->append_code(&callMod);
  } else {
    BrigInstBase callInst = {
      0,
      BrigEInstBase,
      BrigCall,
      Brigb32,
      BrigNoPacking,
      {OpOffset[0], OpOffset[1], OpOffset[2], OpOffset[3], OpOffset[4]}
    };
    callInst.size = sizeof(callInst);
    context->append_code(&callInst);
  }
  context->token_to_scan = yylex();
  return 0;
}

int Initializer(Context* context, BrigdOffset32_t sym_offset) {
  // first token should be '='
  if (context->token_to_scan != '=') {
    return 1;
  }
  bool hasCurlyBrackets = false;
  context->token_to_scan = yylex();
  if (context->token_to_scan == '{') {
    context->token_to_scan = yylex();
    hasCurlyBrackets = true;
  }
  BrigDirectiveSymbol bds ;
  context->get_directive(sym_offset,&bds);
  BrigDataType16_t init_type ;
  init_type = ConvertTypeToB(context->get_type());
  context->set_type(init_type);

  switch (context->token_to_scan) {
    case TOKEN_DOUBLE_CONSTANT:
      if (FloatInitializer(context, sym_offset)) {
        return 1;
      }
      break;
    case TOKEN_INTEGER_CONSTANT:
    case '-':
      if (DecimalInitializer(context, sym_offset)) {
        return 1;
      }
      break;
    case TOKEN_SINGLE_CONSTANT:
      if (SingleInitializer(context, sym_offset)) {
        return 1;
      }
      break;
    case TOKEN_LABEL:
      if (LabelList(context, sym_offset, false)) {
        return 1;
      }
      break;
    default:
      context->set_error(INVALID_INITIALIZER);
      return 1;
  }
  if (hasCurlyBrackets){
    if (context->token_to_scan != '}') {
      context->set_error(MISSING_CLOSING_BRACKET);
      return 1;
    }
    context->token_to_scan = yylex();
  }
  return 0;
}

int InitializableDecl(Context* context) {
  // first token is GLOBAL or READONLY

  return (InitializableDecl(context, &context->global_symbol_map));
}

int InitializableDecl(Context *context,
    std::map<std::string, BrigdOffset32_t> *symbol_map){
  int var_name_offset;
  if (context->token_to_scan != GLOBAL &&
      context->token_to_scan != READONLY) {
    return 1;
  }
  BrigStorageClass32_t storageClass = context->token_value.storage_class;
  context->token_to_scan = yylex();
  if (context->token_to_scan == _RWIMG ||
      context->token_to_scan == _ROIMG ||
      context->token_to_scan == _SAMP) {
    return 1;
  }

  context->set_isArray(false);
  // First token already verified as GLOBAL/READONLY
  if (context->token_type != DATA_TYPE_ID) {
    return 1;
  }
  context->set_type(context->token_value.data_type);
  context->token_to_scan = yylex();

  if (Identifier(context)) {
    context->set_error(MISSING_IDENTIFIER);
    return 1;
  }
  std::string var_name = context->token_value.string_val;
  var_name_offset = context->add_symbol(var_name);
  if (symbol_map->count(var_name)) {
    context->set_error(REPEATED_DECLARATION);
    return 1;
  }

  context->token_to_scan = yylex();
  // set default value(scalar)
  context->set_dim(0);
  if (context->token_to_scan == '[') {
    if (ArrayDimensionSet(context)) {
      return 1;
    }
  }
  uint16_t naturalAlign = 0;
  switch (ConvertTypeToB(context->get_type())) {
    case Brigb1: naturalAlign = 1; break;
    case Brigb8: naturalAlign = 1; break;
    case Brigb16: naturalAlign = 2; break;
    case Brigb32: naturalAlign = 4; break;
    case Brigb64: naturalAlign = 8; break;
    case Brigb128: naturalAlign = 16; break;
    case BrigRWImg: naturalAlign = 16; break;
    case BrigROImg: naturalAlign = 16; break;
    case BrigSamp: naturalAlign = 16; break;
  }

  if (context->get_alignment() == 0) {
    context->set_alignment(naturalAlign);
  } else if (context->get_alignment() < naturalAlign) {
    context->set_error(INVALID_ALIGNMENT);
    return 1;
  }
  if ((context->get_symbol_modifier() & BrigConst) && 
      (storageClass != BrigGlobalSpace && storageClass != BrigReadonlySpace)) {
    context->set_error(INVALID_DEFINITION);
    return 1;
  }

  BrigDirectiveSymbol sym_decl = {
    sizeof(sym_decl),                 // size
    BrigEDirectiveSymbol,             // kind
    {
      context->get_code_offset(),       // c_code
      storageClass,                     // storageClass
      context->get_attribute(),         // attribute
      0,                                // reserved
      context->get_symbol_modifier(),   // symbol modifier
      context->get_dim(),               // dim
      var_name_offset,                  // s_name
      context->get_type(),              // data type
      context->get_alignment(),         // alignment
    },
    0,                                // d_init = 0 for arg
    0                                 // reserved
  };
  context->set_alignment(0);
  symbol_map->insert(std::map<std::string, BrigdOffset32_t>::value_type(
      var_name, context->get_directive_offset()));
  BrigdOffset32_t argdecl_offset = context->get_directive_offset();
  context->append_directive(&sym_decl);

  if (context->token_to_scan == '=') {
    if (Initializer(context, argdecl_offset)) {
      context->set_error(INVALID_INITIALIZER);
      return 1;
    }
  }

  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }

  context->token_to_scan = yylex();
  return 0;
}

int UninitializableDecl(Context* context) {
  //TODO(Chuang): For test cases
  return UninitializableDecl(context, &context->global_symbol_map);
}

int UninitializableDecl(Context* context,
    std::map<std::string, BrigdOffset32_t> *symbol_map) {

  BrigStorageClass32_t storage_class;
  BrigsOffset32_t str_offset;
  // first_token is PRIVATE, GROUP or SPILL
  if (PRIVATE != context->token_to_scan &&
      GROUP != context->token_to_scan &&
      SPILL != context->token_to_scan) {
    return 1;
  }

  if (context->get_symbol_modifier() & BrigConst) {
    context->set_error(INVALID_DEFINITION);
    return 1;
  }

  storage_class = context->token_value.storage_class;
  context->token_to_scan = yylex();

  if (DATA_TYPE_ID != context->token_type) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  context->token_to_scan = yylex();

  if (Identifier(context)) {
    context->set_error(MISSING_IDENTIFIER);
    return 1;
  }

  std::string var_name = context->token_value.string_val;
  if (symbol_map->count(var_name)) {
    context->set_error(REPEATED_DECLARATION);
    return 1;
  }
  str_offset = context->add_symbol(var_name);

  context->token_to_scan = yylex();
  context->set_dim(0);

  if ('[' == context->token_to_scan) {
    if (ArrayDimensionSet(context))
      return 1;
  }

  uint16_t naturalAlign = 0;
  switch (ConvertTypeToB(context->get_type())) {
    case Brigb1: naturalAlign = 1; break;
    case Brigb8: naturalAlign = 1; break;
    case Brigb16: naturalAlign = 2; break;
    case Brigb32: naturalAlign = 4; break;
    case Brigb64: naturalAlign = 8; break;
    case Brigb128: naturalAlign = 16; break;
    case BrigRWImg: naturalAlign = 16; break;
    case BrigROImg: naturalAlign = 16; break;
    case BrigSamp: naturalAlign = 16; break;
  }

  if (context->get_alignment() == 0) {
    context->set_alignment(naturalAlign);
  } else if (context->get_alignment() < naturalAlign) {
    context->set_error(INVALID_ALIGNMENT);
    return 1;
  }
  BrigDirectiveSymbol bds = {
    sizeof(BrigDirectiveSymbol),    // size
    BrigEDirectiveSymbol ,          // kind
    {
      context->get_code_offset(),        // c_code
      storage_class,                     // storag class
      context->get_attribute() ,         // attribut
      0,                                 // reserved
      context->get_symbol_modifier(),    // symbolModifier
      context->get_dim(),                // dim
      str_offset,                        // s_name
      context->get_type(),               // type
      context->get_alignment(),          // align
    },
    0,                             // d_init
    0                              // reserved
  };

  symbol_map->insert(std::map<std::string, BrigdOffset32_t>::value_type(
      var_name, context->get_directive_offset()));
  context->append_directive(&bds);
  context->set_alignment(0);

  if (';' != context->token_to_scan) {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }

  context->token_to_scan = yylex();
  return 0;
}

int ArgUninitializableDecl(Context* context) {
  // first token is ARG
  if ( ARG != context->token_to_scan)
    return 1;

  BrigStorageClass32_t storage_class = context->token_value.storage_class;
  context->token_to_scan = yylex();

  if (DATA_TYPE_ID != context->token_type) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  context->token_to_scan = yylex();

  if (context->token_to_scan != TOKEN_LOCAL_IDENTIFIER) {
    context->set_error(MISSING_IDENTIFIER);
    return 1;
  }

  std::string var_name = context->token_value.string_val;
  if (context->arg_symbol_map.count(var_name)) {
    context->set_error(REPEATED_DECLARATION);
    return 1;
  }
  BrigsOffset32_t str_offset = context->add_symbol(var_name);

  context->token_to_scan = yylex();
  context->set_dim(0);

  if ('[' == context->token_to_scan) {
    if (ArrayDimensionSet(context))
      return 1;
  }
  uint16_t naturalAlign = 0;
  switch (ConvertTypeToB(context->get_type())) {
    case Brigb1: naturalAlign = 1; break;
    case Brigb8: naturalAlign = 1; break;
    case Brigb16: naturalAlign = 2; break;
    case Brigb32: naturalAlign = 4; break;
    case Brigb64: naturalAlign = 8; break;
    case Brigb128: naturalAlign = 16; break;
    case BrigRWImg: naturalAlign = 16; break;
    case BrigROImg: naturalAlign = 16; break;
    case BrigSamp: naturalAlign = 16; break;
  }

  if (context->get_alignment() == 0) {
    context->set_alignment(naturalAlign);
  } else if (context->get_alignment() < naturalAlign) {
    context->set_error(INVALID_ALIGNMENT);
    return 1;
  }

  BrigDirectiveSymbol bds = {
    sizeof(BrigDirectiveSymbol),    // size
    BrigEDirectiveSymbol ,          // kind
    {
      context->get_code_offset(),        // c_code
      storage_class,                     // storag class
      context->get_attribute() ,         // attribut
      0,                                 // reserved
      context->get_symbol_modifier(),    // symbolModifier
      context->get_dim(),                // dim
      str_offset,                        // s_name
      context->get_type(),               // type
      context->get_alignment(),          // align
    },
    0,                             // d_init
    0,                             // reserved
  };

  context->set_alignment(0);
  context->arg_symbol_map[var_name] = context->get_directive_offset();
  context->append_directive(&bds);

  if (';' != context->token_to_scan) {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }

  context->token_to_scan = yylex();
  return 0;
}

int FileDecl(Context* context) {
  uint32_t fileid = 0;
  BrigsOffset32_t path_offset = 0;

  // first token is _FILE "file"
  if (_FILE != context->token_to_scan)
    return 1;

  context->token_to_scan = yylex();
  if (TOKEN_INTEGER_CONSTANT != context->token_to_scan) {
    context->set_error(MISSING_INTEGER_CONSTANT);
    return 1;
  }
  fileid = context->token_value.int_val;

  context->token_to_scan = yylex();
  if (TOKEN_STRING != context->token_to_scan) {
    context->set_error(MISSING_STRING);
    return 1;
  }
  std::string path(context->token_value.string_val);
  path.assign(path, 1, path.size() - 2);
  path_offset = context->add_symbol(path);

  context->token_to_scan = yylex();
  if (';' != context->token_to_scan) {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }

  BrigDirectiveFile bdfile = {
    sizeof(BrigDirectiveFile),   //size
    BrigEDirectiveFile,          //kind
    context->get_code_offset(),  //c_code
    fileid,                      //fileid
    path_offset                  //s_filename
  };
  context->append_directive(&bdfile);

  context->token_to_scan = yylex();
  return 0;
}

int SignatureType(Context *context,
    std::vector<BrigDirectiveSignature::BrigProtoType> *types) {
  // alignment optional
  if (ALIGN == context->token_to_scan) {
    if (Alignment(context))
      return 1;
  }

  if (ARG != context->token_to_scan) {
    return 1;
  }

  context->token_to_scan = yylex();
  if (DATA_TYPE_ID != context->token_type) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);

  context->token_to_scan = yylex();
  if (TOKEN_LOCAL_IDENTIFIER == context->token_to_scan) {
    context->token_to_scan = yylex();
  }

  context->set_dim(0);
  if ('[' == context->token_to_scan )
    if (ArrayDimensionSet(context))
      return 1;

  uint16_t naturalAlign = 0;
  switch (ConvertTypeToB(context->get_type())) {
    case Brigb1: naturalAlign = 1; break;
    case Brigb8: naturalAlign = 1; break;
    case Brigb16: naturalAlign = 2; break;
    case Brigb32: naturalAlign = 4; break;
    case Brigb64: naturalAlign = 8; break;
    case Brigb128: naturalAlign = 16; break;
    case BrigRWImg: naturalAlign = 16; break;
    case BrigROImg: naturalAlign = 16; break;
    case BrigSamp: naturalAlign = 16; break;
  }
  if (context->get_alignment() == 0) {
    context->set_alignment(naturalAlign);
  } else if (context->get_alignment() < naturalAlign) {
    context->set_error(INVALID_ALIGNMENT);
    return 1;
  }

  BrigDirectiveSignature::BrigProtoType bpt = {
    context->get_type(),         // type
    context->get_alignment(),    // align
    (context->get_dim() != 0),   // hasDim
    context->get_dim()           // dim
  };
  types->push_back(bpt);
  context->set_alignment(0);

  return 0;
}

int SignatureType(Context *context) {
  std::vector<BrigDirectiveSignature::BrigProtoType> types;
  return SignatureType(context,&types);
}

int SysCall(Context* context) {
  BrigoOffset32_t  o_operands[5] = {0};

  // first token must be SYSCALL
  if (SYSCALL != context->token_to_scan)
    return 1;

  context->token_to_scan = yylex();
  context->set_type(Brigb32);

  // Note: dest: Destination. Must be a 32-bit register.
  // operand 1
  if (context->token_to_scan != TOKEN_SREGISTER) {
    context->set_error(INVALID_FIRST_OPERAND);
    return 1;
  }
  if (Operand(context,&o_operands[0])) {
    return 1;
  }
  if (context->token_to_scan != ',') {
    context-> set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  //operand 2
  if (context->token_to_scan != TOKEN_INTEGER_CONSTANT &&
      context->token_to_scan != '-') {
    context->set_error(INVALID_SECOND_OPERAND);
    return 1;
  }
  if (Operand(context, &o_operands[1])) {
    return 1;
  }
  if (context->token_to_scan != ',') {
    context-> set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  // operand 3
  if (((context->token_to_scan != TOKEN_SREGISTER) &&
       (context->token_to_scan != TOKEN_WAVESIZE) &&
       (context->token_to_scan != TOKEN_INTEGER_CONSTANT) &&
       (context->token_to_scan != '-'))) {
    context->set_error(INVALID_THIRD_OPERAND);
    return 1;
  }
  if (Operand(context, &o_operands[2])) {
    return 1;
  }
  if (context->token_to_scan != ',') {
    context-> set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  // operand 4
  if (((context->token_to_scan != TOKEN_SREGISTER) &&
       (context->token_to_scan != TOKEN_WAVESIZE) &&
       (context->token_to_scan != TOKEN_INTEGER_CONSTANT) &&
       (context->token_to_scan != '-'))) {
    context->set_error(INVALID_FOURTH_OPERAND);
    return 1;
  }
  if (Operand(context, &o_operands[3])) {
    return 1;
  }
  if (context->token_to_scan != ',') {
    context-> set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  // operand 5
  if (((context->token_to_scan != TOKEN_SREGISTER) &&
       (context->token_to_scan != TOKEN_WAVESIZE) &&
       (context->token_to_scan != TOKEN_INTEGER_CONSTANT) &&
       (context->token_to_scan != '-'))) {
    context->set_error(INVALID_FOURTH_OPERAND);
    return 1;
  }
  if (Operand(context, &o_operands[4])) {
    return 1;
  }

  if (context->token_to_scan != ';') {
    context-> set_error(MISSING_SEMICOLON);
    return 1;
  }

  BrigInstBase syscallInst = {
    sizeof(BrigInstBase),         // size
    BrigEInstBase,                // kind
    BrigSysCall,                  // opcode
    Brigb32,                      // type
    BrigNoPacking,                // packing
    {o_operands[0], o_operands[1],  // o_operands[5]
     o_operands[2], o_operands[3],
     o_operands[4]}
  };

  context->append_code(&syscallInst);
  context->token_to_scan = yylex();
  return 0;
}

int SignatureArgumentList(Context *context,
    std::vector<BrigDirectiveSignature::BrigProtoType> *types) {
  while (1) {
    if(')' == context->token_to_scan)
      return 0;

    if (SignatureType(context,types))
      return 1;

    if (',' == context->token_to_scan)
      context->token_to_scan = yylex();
  }
  return 0;
}

int SignatureArgumentList(Context *context) {
  std::vector<BrigDirectiveSignature::BrigProtoType> types;
  if ('(' == context->token_to_scan) {
    context->token_to_scan = yylex();
    return SignatureArgumentList(context,&types);
  } else {
    return 1;
  }
}

int FunctionSignature(Context *context) {
  BrigsOffset32_t s_name = 0 ;
  uint32_t inCount = 0;
  uint32_t outCount = 0;
  std::vector<BrigDirectiveSignature::BrigProtoType> types;

  // first token is SIGNATURE
  if (SIGNATURE != context->token_to_scan)
    return 1;

  context->token_to_scan = yylex();
  if (TOKEN_GLOBAL_IDENTIFIER != context->token_to_scan) {
    context->set_error(MISSING_GLOBAL_IDENTIFIER);
    return 1;
  }
  std::string name(context->token_value.string_val);
  s_name = context->add_symbol(name);

  if (context->func_map.count(name)) {
    context->set_error(REPEATED_DECLARATION);
    return 1;
  }

  // optional output
  context->token_to_scan = yylex();
  if (context->token_to_scan == '(') {
    context->token_to_scan = yylex();
    if(SignatureArgumentList(context, &types)){
      context->set_error(INVALID_ARGUMENT_LIST);
      return 1;
    }
   if (context->token_to_scan != ')') {
      context->set_error(MISSING_CLOSING_PARENTHESIS);
      return 1;
    }
    outCount = types.size();
    if (outCount > 1) {
      context->set_error(INVALID_ARGUMENT_LIST);
      return 1;
    }
    context->token_to_scan = yylex();
  }

  // optional input
  if (context->token_to_scan == '(') {
    context->token_to_scan = yylex();
    if (SignatureArgumentList(context, &types)){
      context->set_error(INVALID_ARGUMENT_LIST);
      return 1;
    }
    if (context->token_to_scan != ')') {
      context->set_error(MISSING_CLOSING_PARENTHESIS);
      return 1;
    }
    inCount = types.size() - outCount;
    context->token_to_scan = yylex();
  }

  if (';' != context->token_to_scan) {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }

  size_t arraySize = sizeof(BrigDirectiveSignature) +
         (types.size() - 1) * sizeof(BrigDirectiveSignature::BrigProtoType);
  uint8_t *array  = new uint8_t[arraySize];
  BrigDirectiveSignature *bds =
      reinterpret_cast<BrigDirectiveSignature*>(array);

  bds->size = arraySize;
  bds->kind = BrigEDirectiveSignature;
  bds->c_code = context->get_code_offset();
  bds->s_name = s_name;
  bds->outCount = outCount;
  bds->inCount = inCount;
  for (uint32_t i = 0, size = types.size(); i < size; i++)
    memmove(&bds->types[i],&types[i],sizeof(BrigDirectiveSignature::BrigProtoType));
  context->func_map[name] = context->get_directive_offset();
  context->append_directive(bds);

  delete[] reinterpret_cast<char *>(bds);
  types.clear();

  context->token_to_scan = yylex();
  return 0;
}

int Label(Context* context) {
  //TODO(Chuang): For test cases.
  return Label(context, &context->global_symbol_map);
}
int Label(Context* context, std::map<std::string, BrigdOffset32_t> *symbol_map) {
  // first must be "label"
  BrigcOffset32_t c_code = 0;
  BrigsOffset32_t s_name = 0;

  if (TOKEN_LABEL != context->token_to_scan)
    return 1;

  std::string name = context->token_value.string_val;

  context->token_to_scan = yylex();
  if (':' != context->token_to_scan) {
    context->set_error(MISSING_COLON);
    return 1;
  }
  s_name = context->add_symbol(name);
  c_code = context->get_code_offset();
  BrigDirectiveLabel labelDir = {
    sizeof(BrigDirectiveLabel),    // size
    BrigEDirectiveLabel,           // kind
    c_code,                        // c_code
    s_name                         // s_name
  };
  symbol_map->insert(std::map<std::string, BrigdOffset32_t>::value_type(
      name, context->get_directive_offset()));
  context->append_directive(&labelDir);

  if (context->label_d_map.count(name)) {
    BrigdOffset32_t dOffset = context->get_symbol(name);
    typedef std::multimap<std::string, BrigdOffset32_t>::const_iterator MapIter_t;
    std::pair<MapIter_t, MapIter_t> labRange;
    labRange = context->label_d_map.equal_range(name);

    for (MapIter_t i = labRange.first; i != labRange.second; ++i) {
      context->update_directive_bytes(
        reinterpret_cast<unsigned char*>(&dOffset),
        i->second, sizeof(BrigdOffset32_t)
      );
    }
  }

  if (context->label_o_map.count(name)) {
    BrigoOffset32_t labOpOffset = context->label_o_map[name];
    BrigOperandLabelRef labOp;
    context->get_operand(labOpOffset, &labOp);
    labOp.labeldirective = context->get_symbol(name);
    context->update_operand_bytes(
      reinterpret_cast<unsigned char*>(&labOp),
      labOpOffset, sizeof(labOp)
    );
  }

  context->token_to_scan = yylex();
  return 0;
}

int LabelTargets(Context* context) {
  std::string labelName;
  if (context->token_to_scan == TOKEN_LABEL) {
    labelName = context->token_value.string_val;
  }
  if (Label(context, &context->global_symbol_map)) {
    context->set_error(MISSING_LABEL);
    return 1;
  }
  if (context->token_to_scan == LABELTARGETS) {
    return LabelTargetsPart2(context, context->get_symbol(labelName));
  } else {
    context->set_error(INVALID_LABEL_TARGETS);
    return 1;
  }
  return 1;
}

int LabelTargetsPart2(Context* context, BrigdOffset32_t dOffset) {
  context->token_to_scan = yylex();
  if (context->token_to_scan != TOKEN_LABEL) {
    context->set_error(MISSING_LABEL);
    return 1;
  }
  if (LabelList(context, dOffset, true)) {
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  context->token_to_scan = yylex();
  return 0;
}

int Instruction4(Context* context) {

  switch(context->token_to_scan) {
    case SAD:
    case SAD2:
    case SAD4:
    case SAD4HI:
    case LERP:
    case BITALIGN:
    case BYTEALIGN:
      if (!Instruction4MultiMediaOperation(context)) {
        return 0;
      }
      return 1;
    case FMA:
      if (!Instruction4Fma(context)) {
        return 0;
      }
      return 1;
    case MAD:
      if (!Instruction4Mad(context)) {
        return 0;
      }
      return 1;
    case EXTRACT:
    case INSERT:
    case BITSELECT:
      if (!Instruction4BitOperation(context)) {
        return 0;
      }
      return 1;
    case CMOV:
      if (!Instruction4Cmov(context)) {
        return 0;
      }
      return 1;
    case SHUFFLE:
      if (!Instruction4Shuffle(context)) {
        return 0;
      }
      return 1;
  }

  return 1;
}

int Instruction4MultiMediaOperation(Context* context) {
  BrigoOffset32_t OpOffset[4] = {0, 0, 0, 0};
  BrigOpcode32_t opcode = context->token_value.opcode;
  BrigDataType16_t type = Brigb32;

  context->token_to_scan = yylex();

  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }

  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;
  context->token_to_scan = yylex();

  if (!CheckDestRegister(context->token_to_scan, type, opcode)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[0])) {
    context->set_error(INVALID_FIRST_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  // TODO(Chuang):src0, src1, src2, src3: Sources. All are b32

  if (Operand(context, &OpOffset[1])) {
    context->set_error(INVALID_SECOND_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (Operand(context, &OpOffset[2])) {
    context->set_error(INVALID_THIRD_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (Operand(context, &OpOffset[3])) {
    context->set_error(INVALID_FOURTH_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }  // ';'

  BrigInstBase mmoInst = {
    0,                     // size
    BrigEInstBase,         // kind
    opcode,                // opcode
    type,                  // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };
  mmoInst.size = sizeof(mmoInst);
  for (int i = 0 ; i < 4 ; ++i) {
    mmoInst.o_operands[i] = OpOffset[i];
  }
  context->append_code(&mmoInst);
  context->token_to_scan = yylex();
  return 0;
}

int Instruction4Fma(Context* context) {

  BrigAluModifier aluModifier = {0, 0, 0, 0, 0, 0, 0};
  BrigDataType16_t type = Brigb32;
  BrigoOffset32_t OpOffset[4] = {0, 0, 0, 0};

  context->token_to_scan = yylex();
  if (RoundingMode(context)) {
    context->set_error(INVALID_MODIFIER);
    return 1;
  }
  aluModifier = context->get_alu_modifier();
  // TODO(Chuang):Type Length: 16 (in some implementations), 32, 64
  if (context->token_to_scan != _F32 &&
      context->token_to_scan != _F64) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;
  context->token_to_scan = yylex();


  if (!CheckDestRegister(context->token_to_scan, type, BrigFma)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[0])) {
    context->set_error(INVALID_FIRST_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  // Sources. Can be a register or immediate value.
  if (context->token_type != REGISTER &&
      context->token_type != CONSTANT) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[1])) {
    context->set_error(INVALID_SECOND_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  if (context->token_type != REGISTER &&
      context->token_type != CONSTANT) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[2])) {
    context->set_error(INVALID_THIRD_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (context->token_type != REGISTER &&
      context->token_type != CONSTANT) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[3])) {
    context->set_error(INVALID_FOURTH_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }  // ';'
  if (aluModifier.valid) {
    BrigInstMod fmaMod = {
      sizeof(BrigInstMod),  // size
      BrigEInstMod,         // kind
      BrigFma,              // opcode
      type,                 // type
      BrigNoPacking,        // packing
      {0, 0, 0, 0, 0},      // o_operands[5]
      aluModifier           // aluModifier
    };
    for (int i = 0 ; i < 4 ; ++i) {
      fmaMod.o_operands[i] = OpOffset[i];
    }
    context->append_code(&fmaMod);
  }
  else {
    BrigInstBase fmaInst = {
      0,                     // size
      BrigEInstBase,         // kind
      BrigFma,               // opcode
      type,                  // type
      BrigNoPacking,         // packing
      {0, 0, 0, 0, 0}        // o_operands[5]
    };
    fmaInst.size = sizeof(fmaInst);
    for (int i = 0 ; i < 4 ; ++i) {
      fmaInst.o_operands[i] = OpOffset[i];
    }
    context->append_code(&fmaInst);
  }
  context->token_to_scan = yylex();
  return 0;
}

int Instruction4Mad(Context* context) {

  BrigAluModifier aluModifier = {0, 0, 0, 0, 0, 0, 0};
  BrigoOffset32_t OpOffset[4] = {0, 0, 0, 0};
  BrigDataType16_t type = Brigb32;

  context->token_to_scan = yylex();
  if (RoundingMode(context)) {
    context->set_error(INVALID_MODIFIER);
    return 1;
  }
  aluModifier = context->get_alu_modifier();
  // TODO(Chuang):Type f Length: 16 (in some implementations), 32, 64
  // Or Type u, s Length 32, 64
  if (context->token_to_scan != _F32 &&
      context->token_to_scan != _F64 &&
      context->token_to_scan != _U32 &&
      context->token_to_scan != _U64 &&
      context->token_to_scan != _S32 &&
      context->token_to_scan != _S64) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }

  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;
  context->token_to_scan = yylex();

  // Note: dest: Destination register.

  if (!CheckDestRegister(context->token_to_scan, type, BrigMad)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[0])) {
    context->set_error(INVALID_FIRST_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (Operand(context, &OpOffset[1])) {
    context->set_error(INVALID_SECOND_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (Operand(context, &OpOffset[2])) {
    context->set_error(INVALID_THIRD_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();


  if (Operand(context, &OpOffset[3])) {
    context->set_error(INVALID_FOURTH_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }  // ';'
  if (aluModifier.valid) {
    // Note: mad_u/s without _ftz/rounding
    if (type != Brigf32 && type != Brigf64) {
      context->set_error(INVALID_ROUNDING_MODE);
      return 1;
    }
    BrigInstMod madMod = {
      0,                    // size
      BrigEInstMod,         // kind
      BrigMad,              // opcode
      type,                 // type
      BrigNoPacking,        // packing
      {0, 0, 0, 0, 0},      // o_operands[5]
      aluModifier           // aluModifier
    };
    madMod.size = sizeof(madMod);
    for (int i = 0 ; i < 4 ; ++i) {
      madMod.o_operands[i] = OpOffset[i];
    }
    context->append_code(&madMod);
  }
  else {
    BrigInstBase madInst = {
      0,                     // size
      BrigEInstBase,         // kind
      BrigMad,               // opcode
      type,                  // type
      BrigNoPacking,         // packing
      {0, 0, 0, 0, 0}        // o_operands[5]
    };
    madInst.size = sizeof(madInst);
    for (int i = 0 ; i < 4 ; ++i) {
      madInst.o_operands[i] = OpOffset[i];
    }
    context->append_code(&madInst);
  }
  context->token_to_scan = yylex();
  return 0;
}

int Instruction4BitOperation(Context* context) {
  BrigDataType16_t type = Brigb32;
  BrigoOffset32_t OpOffset[4] = {0, 0, 0, 0};
  BrigOpcode32_t opcode = context->token_value.opcode;

  context->token_to_scan = yylex();
  // Note: Type s, u; Length 32, 64;
  // TODO(Chuang):  I think "b" should be in its type
  if (context->token_to_scan != _S32 &&
      context->token_to_scan != _S64 &&
      context->token_to_scan != _U32 &&
      context->token_to_scan != _U64 &&
      context->token_to_scan != _B32 &&
      context->token_to_scan != _B64) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;
  context->token_to_scan = yylex();

  if (!CheckDestRegister(context->token_to_scan, type, opcode)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[0])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (Operand(context, &OpOffset[1])) {
    context->set_error(INVALID_SECOND_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  // TODO(Chuang): src1, src2: type follow as the kind of opcode.
  // different opcode will have different rule of types.

  if (opcode == BrigExtract) {
    if (context->token_type == REGISTER) {
      if (context->token_to_scan != TOKEN_SREGISTER) {
	context->set_error(INVALID_THIRD_OPERAND);
	return 1;
      }
    }
  }
  if (Operand(context, &OpOffset[2])) {
    context->set_error(INVALID_THIRD_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (opcode == BrigExtract) {
    if (context->token_type == REGISTER) {
      if (context->token_to_scan != TOKEN_SREGISTER) {
	context->set_error(INVALID_FOURTH_OPERAND);
	return 1;
      }
    }
  }
  if (Operand(context, &OpOffset[3])) {
    context->set_error(INVALID_FOURTH_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }  // ';'
  BrigInstBase bsoInst = {
    0,                     // size
    BrigEInstBase,         // kind
    opcode,                // opcode
    type,                  // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };
  bsoInst.size = sizeof(bsoInst);
  for (int i = 0 ; i < 4 ; ++i) {
    bsoInst.o_operands[i] = OpOffset[i];
  }
  context->append_code(&bsoInst);
  context->token_to_scan = yylex();
  return 0;
}

int Instruction4Cmov(Context* context) {

  context->token_to_scan = yylex();
  BrigDataType16_t type = Brigb32;
  BrigoOffset32_t OpOffset[4] = {0, 0, 0, 0};
  BrigPacking16_t packing = BrigNoPacking;

  // TODO(Chuang):Type: For the regular operation: b.
  // For the packed operation: s, u, f.
  // Length: For rhe regular operation, Length can be 1, 32, 64.
  // Applies to src1, and src2. For the packed
  // operation, Length can be any packed type.

  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);

  type = context->token_value.data_type;
  packing = type >= Brigu8x4 ? (BrigPacking16_t)BrigPackPP : packing;
  context->token_to_scan = yylex();

  // Note: dest: Destination register.
  std::string opName;

  if (context->token_type != REGISTER) {
    context->set_error(INVALID_FIRST_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[0])) {
    context->set_error(INVALID_FIRST_OPERAND);
    return 1;
  }
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

  if (context->token_type != REGISTER &&
      context->token_type != CONSTANT) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (packing == BrigNoPacking) {
    context->set_type(Brigb1);
  }
  if (Operand(context ,&OpOffset[1])) {
    context->set_error(INVALID_SECOND_OPERAND);
    return 1;
  }
  if (packing == BrigNoPacking) {
    context->set_type(type);
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  if (context->token_type != REGISTER &&
      context->token_type != CONSTANT) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[2])) {
    context->set_error(INVALID_THIRD_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (context->token_type != REGISTER &&
      context->token_type != CONSTANT) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[3])) {
    context->set_error(INVALID_FOURTH_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }  // ';'
  BrigInstBase cmovInst = {
    0,                     // size
    BrigEInstBase,         // kind
    BrigCmov,              // opcode
    type,                  // type
    packing,               // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };
  cmovInst.size = sizeof(cmovInst);
  for (int i = 0 ; i < 4 ; ++i) {
    cmovInst.o_operands[i] = OpOffset[i];
  }
  context->append_code(&cmovInst);
  context->token_to_scan = yylex();
  return 0;
}

int Instruction4Shuffle(Context* context) {

  BrigoOffset32_t OpOffset[4] = {0, 0, 0, 0};
  BrigDataType16_t type = Brigb32;

  context->token_to_scan = yylex();

  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }

  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;
  context->token_to_scan = yylex();

  if (!CheckDestRegister(context->token_to_scan, type, BrigShuffle)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[0])) {
    context->set_error(INVALID_FIRST_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (context->token_type != REGISTER &&
      context->token_type != CONSTANT) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[1])) {
    context->set_error(INVALID_SECOND_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  if (context->token_type != REGISTER &&
      context->token_type != CONSTANT) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[2])) {
    context->set_error(INVALID_THIRD_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  // src2: Source. Must be a literal value used to select elements
  if (context->token_type != CONSTANT) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[3])) {
    context->set_error(INVALID_FOURTH_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }  // ';'
  BrigInstBase shuffleInst = {
    0,                     // size
    BrigEInstBase,         // kind
    BrigShuffle,           // opcode
    type,                  // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };
  shuffleInst.size = sizeof(shuffleInst);
  for (int i = 0 ; i < 4; ++i) {
    shuffleInst.o_operands[i] = OpOffset[i];
  }
  context->append_code(&shuffleInst);
  context->token_to_scan = yylex();
  return 0;
}

int KernelArgumentDecl(Context *context) {
  if (DeclPrefix(context))
    return 1;

  context->set_dim(0);
  BrigStorageClass32_t storage_class = context->token_value.storage_class;

  if (KERNARG != context->token_to_scan)
    return 1;

  context->token_to_scan = yylex();
  if ((DATA_TYPE_ID != context->token_type)&&
      (_RWIMG != context->token_to_scan) &&
      (_SAMP  != context->token_to_scan) &&
      (_ROIMG != context->token_to_scan)) {

    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);

  context->token_to_scan = yylex();
  if (TOKEN_LOCAL_IDENTIFIER != context->token_to_scan) {
    context->set_error(MISSING_IDENTIFIER);
    return 1;
  }
  // for argument, we need to set a BrigDirectiveSymbol
  // and write the corresponding string into .string section.

  std::string arg_name = context->token_value.string_val;
  if (context->local_symbol_map.count(arg_name)) {
    context->set_error(REPEATED_DECLARATION);
    return 1;
  }
  int arg_name_offset = context->add_symbol(arg_name);

  // scan for arrayDimensions
  context->token_to_scan = yylex();

  if ('[' == context->token_to_scan) {
    if (ArrayDimensionSet(context)) {
      // context->token_to_scan has been set in ArrayDimensionSet()
      return 1;
    }
  }
  uint16_t naturalAlign = 0;
  switch (ConvertTypeToB(context->get_type())) {
    case Brigb1: naturalAlign = 1; break;
    case Brigb8: naturalAlign = 1; break;
    case Brigb16: naturalAlign = 2; break;
    case Brigb32: naturalAlign = 4; break;
    case Brigb64: naturalAlign = 8; break;
    case Brigb128: naturalAlign = 16; break;
    case BrigRWImg: naturalAlign = 16; break;
    case BrigROImg: naturalAlign = 16; break;
    case BrigSamp: naturalAlign = 16; break;
  }
  if (context->get_alignment() == 0) {
    context->set_alignment(naturalAlign);
  } else if (context->get_alignment() < naturalAlign) {
    context->set_error(INVALID_ALIGNMENT);
    return 1;
  }
  BrigdOffset32_t dsize = context->get_directive_offset();
  BrigDirectiveSymbol sym_decl = {
    sizeof(BrigDirectiveSymbol),                 // size
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
  //Mapping symbol names to declarations in .dir
  context->local_symbol_map[arg_name] = dsize;
  context->set_alignment(0);

  return 0;
}

int KernelArgumentListBody(Context *context){
  uint32_t paramCount = 0;
  if ('(' != context->token_to_scan) {
    return 1;
  }
  context->token_to_scan = yylex();
  return KernelArgumentListBody(context,&paramCount);
}

int KernelArgumentListBody(Context *context,uint32_t *paramCount) {
  while (1) {
    if(')' == context->token_to_scan)
      return 0;
    if (KernelArgumentDecl(context))
      return 1;
    (*paramCount)++;
    if (',' == context->token_to_scan )
      context->token_to_scan = yylex();
  }
}

int Kernel(Context *context) {
  // first must be KERNEL
  if(KERNEL != context->token_to_scan)
    return 1;

  BrigdOffset32_t bdk_offset = context->get_directive_offset();
  context->set_bdf_offset(bdk_offset);

  context->token_to_scan = yylex();
  if (TOKEN_GLOBAL_IDENTIFIER != context->token_to_scan) {
    context->set_error(MISSING_IDENTIFIER);
    return 1;
  }

  std::string kern_name = context->token_value.string_val;
  BrigsOffset32_t str_offset = context->add_symbol(kern_name);
  if (context->func_map.count(kern_name)) {
    context->set_error(REPEATED_DECLARATION);
    return 1;
  }
  context->func_map[kern_name] = bdk_offset;

  size_t bdk_size = sizeof(BrigDirectiveKernel);
  BrigDirectiveKernel bdk = {
    bdk_size,                    // size
    BrigEDirectiveKernel,        // kind
    context->get_code_offset(),  // c_code
    str_offset,                  // s_name
    0,                           // in param count
    bdk_offset + bdk_size,       // d_firstScopedDirective
    0,                           // operation count
    bdk_offset + bdk_size,       // d_nextDirective
    context->get_attribute(),    // attribute
    0,                           // fbar count
    0,                           // out param count
    0                            // d_firstInParam
  };

  context->append_directive(&bdk);

  context->token_to_scan = yylex();
  if ('(' != context->token_to_scan) {
    context->set_error(MISSING_ARGUMENT_LIST);
    return 1;
  }
  context->token_to_scan = yylex();
  uint32_t paramCount = 0;
  if(KernelArgumentListBody(context, &paramCount)){
    context->set_error(INVALID_ARGUMENT_LIST);
    return 1;
  }
  if (')' != context->token_to_scan) {
    context->set_error(MISSING_CLOSING_PARENTHESIS);
    return 1;
  }
  if(paramCount){
    context->get_directive(bdk_offset, &bdk);
    bdk.inParamCount = paramCount;
    bdk.d_firstInParam = bdk.d_nextDirective;
    bdk.d_nextDirective += paramCount*sizeof(BrigDirectiveSymbol);
    bdk.d_firstScopedDirective += paramCount*sizeof(BrigDirectiveSymbol);
    unsigned char * bdf_charp = reinterpret_cast<unsigned char*>(&bdk);
    context->update_directive_bytes(bdf_charp, bdk_offset,
                                  sizeof(BrigDirectiveKernel));
  }

  context->token_to_scan = yylex();
  if (Codeblock(context))
    return  1;
  return 0;
}

int OperandList(Context* context) {
  if (Operand(context)) {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  while (1) {
    if (context->token_to_scan != ',') {
      context->token_to_scan = yylex();
      return 0;
    }
    context->token_to_scan = yylex();
    if (Operand(context)) {
      context->set_error(MISSING_OPERAND);
      return 1;
    }
  }
  return 1;
}

int ComparisonId(Context* context, BrigCompareOperation32_t* pCmpOperation) {
  switch(context->token_to_scan) {
    case _EQ: *pCmpOperation = BrigEq; break;
    case _NE: *pCmpOperation = BrigNe; break;
    case _LT: *pCmpOperation = BrigLt; break;
    case _LE: *pCmpOperation = BrigLe; break;
    case _GT: *pCmpOperation = BrigGt; break;
    case _GE: *pCmpOperation = BrigGe; break;
    case _EQU: *pCmpOperation = BrigEqu; break;
    case _NEU: *pCmpOperation = BrigNeu; break;
    case _LTU: *pCmpOperation = BrigLtu; break;
    case _LEU: *pCmpOperation = BrigLeu; break;
    case _GTU: *pCmpOperation = BrigGtu; break;
    case _GEU: *pCmpOperation = BrigGeu; break;
    case _NUM: *pCmpOperation = BrigNum; break;
    case _NAN: *pCmpOperation = BrigNan; break;
    case _SEQ: *pCmpOperation = BrigSeq; break;
    case _SNE: *pCmpOperation = BrigSne; break;
    case _SLT: *pCmpOperation = BrigSlt; break;
    case _SLE: *pCmpOperation = BrigSle; break;
    case _SGT: *pCmpOperation = BrigSgt; break;
    case _SGE: *pCmpOperation = BrigSge; break;
    case _SGEU: *pCmpOperation = BrigSgeu; break;
    case _SEQU: *pCmpOperation = BrigSequ; break;
    case _SNEU: *pCmpOperation = BrigSneu; break;
    case _SLTU: *pCmpOperation = BrigSltu; break;
    case _SLEU: *pCmpOperation = BrigSleu; break;
    case _SNUM: *pCmpOperation = BrigSnum; break;
    case _SNAN: *pCmpOperation = BrigSnan; break;
    case _SGTU: *pCmpOperation = BrigSgtu; break;
    default:
      context->set_error(MISSING_DECLPREFIX);
      return 1;
  }
  return 0;
}

int Cmp(Context* context) {
  // Chuang
  // first token is PACKEDCMP or CMP
  const unsigned int first_token = context->token_to_scan;
  BrigOpcode32_t opcode = BrigCmp;
  BrigDataType16_t type = Brigb32;
  BrigDataType16_t stype = Brigb32;
  BrigCompareOperation32_t comparisonOperator = 0;
  BrigoOffset32_t OpOffset[3] = {0, 0, 0};
  BrigAluModifier aluModifier = {0, 0, 0, 0, 0, 0, 0};
  context->token_to_scan = yylex();
  if (context->token_to_scan == _FTZ) {
    aluModifier.valid = 1;
    aluModifier.ftz = 1;
    aluModifier.floatOrInt = 1;
    context->token_to_scan = yylex();
  }
  if (ComparisonId(context, &comparisonOperator)) {
    context->set_error(MISSING_COMPARISON_TYPE);
    return 1;
  }
  context->token_to_scan = yylex();
  // 1, 32. Can be 16 if the implementation supports f16.
  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;
  context->token_to_scan = yylex();
  if (first_token == CMP) {
    opcode = BrigCmp;
    // 1, 32. Can be 16 if the implementation supports f16.
    if (context->token_type != DATA_TYPE_ID) {
      context->set_error(MISSING_DATA_TYPE);
      return 1;
    }
    stype = context->token_value.data_type;
    if (stype == Brigb1 && (comparisonOperator != BrigEq &&
                            comparisonOperator != BrigNe)) {
      context->set_error(INVALID_DATA_TYPE);
      return 1;
    }
    context->token_to_scan = yylex();
  } else {  // CMP
    opcode = BrigPackedCmp;
    stype = type;
  }
  // Note: Dest must be a register.
  if (context->token_type != REGISTER) {
    context->set_error(INVALID_FIRST_OPERAND);
    return 1;
  }
  if (Operand(context, &OpOffset[0])) {
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  context->set_type(stype);

  if (Operand(context, &OpOffset[1])) {
    context->set_error(INVALID_SECOND_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (Operand(context, &OpOffset[2])) {
    context->set_error(INVALID_THIRD_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstCmp cmpInst = {
    0,                  // size
    BrigEInstCmp,       // kind
    opcode,             // opcode
    type,               // type
    BrigNoPacking,      // packing
    {0, 0, 0, 0, 0},    // o_operands[5]
    aluModifier,        // aluModifier
    comparisonOperator,             // comparisonOperator
    stype,              // sourceType
    0                   // reserved
  };
  cmpInst.size = sizeof(cmpInst);
  for (int i = 0; i < 3; ++i) {
    cmpInst.o_operands[i] = OpOffset[i];
  }
  context->append_code(&cmpInst);
  context->token_to_scan = yylex();
  return 0;
}

int GlobalPrivateDecl(Context* context) {
  // first token is PRIVATE
  BrigsOffset32_t str_offset;
  context->set_dim(0);

  if (PRIVATE != context->token_to_scan)
    return 1;

  if (context->get_symbol_modifier() & BrigConst) {
    context->set_error(INVALID_IDENTIFIER);
    return 1;
  }
  context->token_to_scan = yylex();
  if (DATA_TYPE_ID != context->token_type) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }

  context->set_type(context->token_value.data_type);
  context->token_to_scan = yylex();

  if (TOKEN_GLOBAL_IDENTIFIER != context->token_to_scan){
    context->set_error(MISSING_GLOBAL_IDENTIFIER);
    return 1;
  }
  std::string symName = context->token_value.string_val;
  if (context->global_symbol_map.count(symName)) {
    context->set_error(REPEATED_DECLARATION);
    return 1;
  }
  str_offset = context->add_symbol(symName);


  context->token_to_scan = yylex();
  if ('[' == context->token_to_scan) {
    if (ArrayDimensionSet(context)) {
      return 1;
    }
  }

  if (';' != context->token_to_scan){
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  uint16_t naturalAlign = 0;
  switch (ConvertTypeToB(context->get_type())) {
    case Brigb1: naturalAlign = 1; break;
    case Brigb8: naturalAlign = 1; break;
    case Brigb16: naturalAlign = 2; break;
    case Brigb32: naturalAlign = 4; break;
    case Brigb64: naturalAlign = 8; break;
    case Brigb128: naturalAlign = 16; break;
    case BrigRWImg: naturalAlign = 16; break;
    case BrigROImg: naturalAlign = 16; break;
    case BrigSamp: naturalAlign = 16; break;
  }

  if (context->get_alignment() == 0) {
    context->set_alignment(naturalAlign);
  } else if (context->get_alignment() < naturalAlign) {
    context->set_error(INVALID_ALIGNMENT);
    return 1;
  }

  BrigDirectiveSymbol bds = {
    sizeof(BrigDirectiveSymbol),    // size
    BrigEDirectiveSymbol ,          // kind
    {
      context->get_code_offset(),        // c_code
      BrigPrivateSpace,                  // storag class
      context->get_attribute() ,         // attribut
      0,                                 // reserved
      context->get_symbol_modifier(),    // symbolModifier
      context->get_dim(),                // dim
      str_offset,                        // s_name
      context->get_type(),               // type
      context->get_alignment(),          // align
    },
    0,                             // d_init
    0,                             // reserved
  };

  context->global_symbol_map[symName] = context->get_directive_offset();
  context->append_directive(&bds);
  context->set_alignment(0);

  context->token_to_scan = yylex();
  return 0;
}

int OffsetAddressableOperandPart2(Context* context, BrigoOffset32_t addrOpOffset,
    BrigoOffset32_t* pRetOpOffset) {

  std::string operand_name;
  BrigStorageClass32_t addrSpaceType = context->get_storageClass();
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
      switch (addrSpaceType) {
        case BrigFlatSpace:
        case BrigGlobalSpace:
        case BrigReadonlySpace:
        case BrigKernargSpace:
          compound_op.type = Brigb64; break;
        default:
          compound_op.type = Brigb32;
      }
    }

    compound_op.name = addrOpOffset;
    if (context->token_type == REGISTER) {
      if ((compound_op.type == Brigb64 && 
           context->token_to_scan != TOKEN_DREGISTER) ||
          (compound_op.type == Brigb32 && 
           context->token_to_scan != TOKEN_SREGISTER)) {
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
        if (context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
          context->set_error(INVALID_SECOND_OPERAND);
          return 1;
        }
        compound_op.offset = context->token_value.int_val * imm_sign;
        context->token_to_scan = yylex();
        if (context->token_to_scan != ']') {
          context->set_error(MISSING_CLOSING_BRACKET);
          return 1;
        }
        *pRetOpOffset = context->get_operand_offset();
        context->append_operand(&compound_op);
        context->token_to_scan = yylex();
        return 0;
      } else if (context->token_to_scan == ']') {
        *pRetOpOffset = context->get_operand_offset();
        context->append_operand(&compound_op);
        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(MISSING_CLOSING_BRACKET);
        return 1;
      }
    } else if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
      compound_op.offset = context->token_value.int_val;
      context->token_to_scan = yylex();
      if (context->token_to_scan != ']') {
        context->set_error(MISSING_CLOSING_BRACKET);
        return 1;
      }
      *pRetOpOffset = context->get_operand_offset();
      context->append_operand(&compound_op);
      context->token_to_scan = yylex();
      return 0;
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
      switch (addrSpaceType) {
        case BrigFlatSpace:
        case BrigGlobalSpace:
        case BrigReadonlySpace:
        case BrigKernargSpace:
          indirect_op.type = Brigb64; break;
        default:
          indirect_op.type = Brigb32;
      }
    }
    if (context->token_type == REGISTER) {
      // The register must be an s or d register (c registers are not allowed).
      if ((indirect_op.type == Brigb64 && 
           context->token_to_scan != TOKEN_DREGISTER) ||
          (indirect_op.type == Brigb32 && 
           context->token_to_scan != TOKEN_SREGISTER)) {
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
        if (context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
          context->set_error(MISSING_INTEGER_CONSTANT);
          return 1;
        }
        indirect_op.offset = context->token_value.int_val * imm_sign;
        context->token_to_scan = yylex();
      }
      if (context->token_to_scan != ']') {
        context->set_error(MISSING_CLOSING_BRACKET);
        return 1;
      }
      *pRetOpOffset = context->get_operand_offset();
      context->append_operand(&indirect_op);
      context->token_to_scan = yylex();
      return 0;
    } else if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
      indirect_op.offset = context->token_value.int_val;
      context->token_to_scan = yylex();
      if (context->token_to_scan != ']') {
        context->set_error(MISSING_CLOSING_BRACKET);
        return 1;
      }
      *pRetOpOffset = context->get_operand_offset();
      context->append_operand(&indirect_op);
      context->token_to_scan = yylex();
      return 0;
    } else if (context->token_to_scan == ']') {
      // empty body
      context->set_error(MISSING_OPERAND);
      return 1;
    }
  }  // addrOpOffset

  return 1;
}

int OffsetAddressableOperand(Context* context) {
  BrigoOffset32_t retOpOffset;
  return OffsetAddressableOperandPart2(context, 0, &retOpOffset);
}

int MemoryOperand(Context* context) {
  BrigoOffset32_t retOpOffset;
  return MemoryOperand(context, &retOpOffset);
}

int MemoryOperand(Context* context, BrigoOffset32_t* pRetOpOffset) {
  // Chuang
  // this judge(first token == '[') is necessary in here
  if (context->token_to_scan != '[') {
    context->set_error(MISSING_OPENNING_BRACKET);
    return 1;
  }
  BrigoOffset32_t currentToOffset = 0;
  context->token_to_scan = yylex();
  // AddressableOperand

  if (!AddressableOperand(context, &currentToOffset, false)) {
    if (context->token_to_scan == '[') {
      context->token_to_scan = yylex();
      if (!OffsetAddressableOperandPart2(context, currentToOffset, 
            pRetOpOffset)) {
        // Global/Local Identifier with offsetAddressOperand.
        return 0;
      }
      return 1;
    }
    // only Global/Local Identifier without offsetAddressOperand.
    *pRetOpOffset = currentToOffset;
    return 0;
  } else if (!OffsetAddressableOperandPart2(context, 0, pRetOpOffset)) {
    return 0;
  }  // Global/Local Identifier/AddressableOperand/offsetAddressableOperand
  return 1;
}

int Instruction5(Context* context) {
  // first token is F2U4 "f2u4"
  BrigoOffset32_t OpOffset[5] = {0, 0, 0, 0, 0};
  BrigDataType16_t type = Brigb32;
  context->token_to_scan = yylex();
  // Note: must be _u32.
  if (context->token_to_scan != _U32) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;
  context->token_to_scan = yylex();

  if (!CheckDestRegister(context->token_to_scan, type, BrigF2u4)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (Operand(context, &OpOffset[0])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  // Note: Sources are f32
  if (Operand(context, &OpOffset[1])) {
    context->set_error(INVALID_SECOND_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  // Note: Sources are f32
  if (Operand(context, &OpOffset[2])) {
    context->set_error(INVALID_THIRD_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  // Note: Sources are f32
  if (Operand(context, &OpOffset[3])) {
    context->set_error(INVALID_FOURTH_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  // Note: Sources are f32

  if (Operand(context, &OpOffset[4])) {
    context->set_error(INVALID_FIFTH_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstBase f2u4Inst = {
    0,                     // size
    BrigEInstBase,         // kind
    BrigF2u4,              // opcode
    type,                  // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}       // o_operands[5]
  };
  f2u4Inst.size = sizeof(f2u4Inst);
  for (int i = 0 ; i < 5 ; ++i) {
    f2u4Inst.o_operands[i] = OpOffset[i];
  }
  context->append_code(&f2u4Inst);
  context->token_to_scan = yylex();
  return 0;
}

int Extension(Context* context) {
  // first token is EXTENSION "extension"
  if (EXTENSION != context->token_to_scan)
    return 1;

  context->token_to_scan = yylex();

  if (context->token_to_scan != TOKEN_STRING) {
    context->set_error(MISSING_STRING);
    return 1;
  }
  std::string str(context->token_value.string_val);
  str.assign(str, 1, str.size() - 2);
  context->token_to_scan = yylex();

  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigDirectiveExtension bde = {
    sizeof(BrigDirectiveExtension),
    BrigEDirectiveExtension,
    context->get_code_offset(),
    context->add_symbol(str)
  };
  context->append_directive(&bde);

  context->token_to_scan = yylex();
  return 0;
}

int Ldc(Context* context) {
  // first token is LDC "ldc"
  context->token_to_scan = yylex();

  BrigDataType16_t type;
  BrigoOffset32_t OpOffset[2] = {0,0};
  BrigMachine16_t machine = context->get_machine();

  if (context->token_to_scan != _U32 &&
      context->token_to_scan != _U64) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  if (machine == BrigELarge && context->token_to_scan != _U64) {
    context->set_error(INVALID_DATA_TYPE);
    return 1;
  }
  if (machine == BrigESmall && context->token_to_scan != _U32) {
    context->set_error(INVALID_DATA_TYPE);
    return 1;
  }

  // If the source is a function, the destination size depends on
  // the machine model. For large, the destination must be 64 bits;
  // for small, the destination must be 32 bits.
  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;
  context->token_to_scan = yylex();
  // dest: must be BrigEOperandReg.
  if ((context->token_to_scan != TOKEN_SREGISTER && type == Brigu32) ||
      (context->token_to_scan != TOKEN_DREGISTER && type == Brigu64)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (Operand(context, &OpOffset[0])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  // op[1] must be BrigEOperandLabelRef or BrigEOperandFunctionRef

  if (context->token_to_scan == TOKEN_LABEL) {
    std::string oper_name = context->token_value.string_val;
    if (!context->label_o_map.count(oper_name)) {
      BrigOperandLabelRef labRef = {
        0,                     // size
        BrigEOperandLabelRef,  // kind
        0                      // labeldirective
      };
      labRef.size = sizeof(labRef);

      labRef.labeldirective = context->get_symbol(oper_name);

      OpOffset[1] = context->get_operand_offset();
      context->label_o_map[oper_name] = context->get_operand_offset();
      context->append_operand(&labRef);

    } else {
      OpOffset[1] = context->label_o_map[oper_name];
    }
  } else if (context->token_to_scan == TOKEN_GLOBAL_IDENTIFIER) {
    std::string oper_name = context->token_value.string_val;
    if (!context->func_o_map.count(oper_name)) {
      BrigOperandFunctionRef funRef = {
        0,                        // size
        BrigEOperandFunctionRef,  // kind
        0                         // fn
      };
      funRef.size = sizeof(funRef);
      // TODO(Chuang): whether the func has been defined.
      funRef.fn = context->func_map[oper_name];
      context->func_o_map[oper_name] = context->get_operand_offset();
      OpOffset[1] = context->get_operand_offset();
      context->append_operand(&funRef);
    } else {
      OpOffset[1] = context->func_o_map[oper_name];
    }
  } else {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  context->token_to_scan = yylex();
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstBase ldc_op = {
    0,                     // size
    BrigEInstBase,         // kind
    BrigLdc,               // opcode
    type,                  // type
    BrigNoPacking,         // packing
    {OpOffset[0], OpOffset[1], 0, 0, 0}        // o_operands[5]
  };
  ldc_op.size = sizeof(ldc_op);
  context->append_code(&ldc_op);
  context->token_to_scan = yylex();
  return 0;
}

int Atom(Context* context) {

  BrigMemorySemantic32_t MemorySemantic = BrigRegular;
  BrigAtomicOperation32_t atomicOperation;
  BrigStorageClass32_t storageClass = BrigFlatSpace;
  BrigDataType16_t DataType;
  BrigoOffset32_t OpOffset[4] = {0,0,0,0};

  const unsigned int first_token = context->token_to_scan;
  if ((first_token != ATOMIC) && (first_token != ATOMIC_CAS)) {
    return 1;
  }
  context->token_to_scan = yylex();
  if ((first_token == ATOMIC) && (context->token_type == ATOMIC_OP)) {
    switch (context->token_to_scan) {
      case _AND_:  atomicOperation = BrigAtomicAnd; break;
      case _OR_:   atomicOperation = BrigAtomicOr; break;
      case _XOR_:  atomicOperation = BrigAtomicXor; break;
      case _EXCH_: atomicOperation = BrigAtomicExch; break;
      case _ADD_:  atomicOperation = BrigAtomicAdd; break;
      case _INC_:  atomicOperation = BrigAtomicInc; break;
      case _DEC_:  atomicOperation = BrigAtomicDec; break;
      case _MIN_:  atomicOperation = BrigAtomicMin; break;
      case _MAX_:  atomicOperation = BrigAtomicMax; break;
      case _SUB_:  atomicOperation = BrigAtomicSub; break;
      default:
        context->set_error(MISSING_OPERATION); return 1;
    }
    context->token_to_scan = yylex();
  } else if(first_token==ATOMIC_CAS){
    atomicOperation = BrigAtomicCas;
  } else {
    context->set_error(MISSING_OPERATION);
    return 1;
  }

  if(!AddressSpaceIdentifier(context)){
    storageClass = context->token_value.storage_class;
    context->set_storageClass(storageClass);
    context->token_to_scan = yylex();
  }

  if (Optacqreg(context, &MemorySemantic)) {
    return 1;
  }

  if (context->token_type != DATA_TYPE_ID){
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  DataType = context->token_value.data_type;
  context->token_to_scan = yylex();

  if (!CheckDestRegister(context->token_to_scan, DataType, BrigAtomic)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (Operand(context, &OpOffset[0])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (MemoryOperand(context, &OpOffset[1])) {
    context->set_error(INVALID_MEMORY_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (Operand(context, &OpOffset[2])) {
    return 1;
  }

  if (first_token == ATOMIC_CAS){
    if (context->token_to_scan != ',') {
      context->set_error(MISSING_COMMA);
      return 1;
    }
    context->token_to_scan = yylex();
    if (Operand(context, &OpOffset[3])) {
      return 1;
    }
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstAtomic atom_op = {
    sizeof(BrigInstAtomic),  // size
    BrigEInstAtomic,         // kind
    BrigAtomic,              // opcode
    DataType,                // type
    BrigNoPacking,           // packing
    {OpOffset[0], OpOffset[1], OpOffset[2], OpOffset[3], 0},
    atomicOperation,         // atomicOperation
    storageClass,            // storageClass
    MemorySemantic           // memorySemantic
  };
  context->append_code(&atom_op);
  context->token_to_scan = yylex();
  return 0;

}

int Mov(Context* context) {
  // Chuang
  // first token is MOV "mov"
  BrigDataType16_t type = Brigb32;
  BrigoOffset32_t OpOffset[2] = {0, 0};

  context->token_to_scan = yylex();
  // TODO(Chuang): When type is b128 or b64 etc,
  // check whether the operands is correct.
  if (context->token_to_scan != _B1 &&
      context->token_to_scan != _B32 &&
      context->token_to_scan != _B64 &&
      context->token_to_scan != _B128) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }

  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;
  context->token_to_scan = yylex();
  if ((context->token_to_scan != '(' && context->token_type != REGISTER) ||
      ArrayOperand(context, &OpOffset[0], type)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  if (ArrayOperand(context, &OpOffset[1], type)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstBase movInst = {
    0,                     // size
    BrigEInstBase,         // kind
    BrigMov,               // opcode
    type,                  // type
    BrigNoPacking,         // packing
    {OpOffset[0], OpOffset[1], 0, 0, 0}        // o_operands[5]
  };
  movInst.size = sizeof(movInst);
  context->append_code(&movInst);
  context->token_to_scan = yylex();
  return 0;
}

int GlobalGroupDecl(Context* context) {

  BrigsOffset32_t str_offset;
  context->set_dim(0);
  // first token is Group
  if (GROUP != context->token_to_scan)
    return 1;

  if (context->get_symbol_modifier() & BrigConst) {
    context->set_error(INVALID_IDENTIFIER);
    return 1;
  }

  context->token_to_scan = yylex();
  if (DATA_TYPE_ID != context->token_type) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }

  context->set_type(context->token_value.data_type);
  context->token_to_scan = yylex();

  if (TOKEN_GLOBAL_IDENTIFIER != context->token_to_scan){
    context->set_error(MISSING_GLOBAL_IDENTIFIER);
    return 1;
  }
  std::string symName = context->token_value.string_val;
  if (context->global_symbol_map.count(symName)) {
    context->set_error(REPEATED_DECLARATION);
    return 1;
  }
  str_offset = context->add_symbol(symName);
  context->set_dim(0);

  context->token_to_scan = yylex();
  if ('[' == context->token_to_scan) {
    if (ArrayDimensionSet(context)) {
      return 1;
    }
  }

  if (';' != context->token_to_scan){
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  uint16_t naturalAlign = 0;
  switch (ConvertTypeToB(context->get_type())) {
    case Brigb1: naturalAlign = 1; break;
    case Brigb8: naturalAlign = 1; break;
    case Brigb16: naturalAlign = 2; break;
    case Brigb32: naturalAlign = 4; break;
    case Brigb64: naturalAlign = 8; break;
    case Brigb128: naturalAlign = 16; break;
    case BrigRWImg: naturalAlign = 16; break;
    case BrigROImg: naturalAlign = 16; break;
    case BrigSamp: naturalAlign = 16; break;
  }

  if (context->get_alignment() == 0) {
    context->set_alignment(naturalAlign);
  } else if (context->get_alignment() < naturalAlign) {
    context->set_error(INVALID_ALIGNMENT);
    return 1;
  }

  BrigDirectiveSymbol bds = {
    sizeof(BrigDirectiveSymbol),    // size
    BrigEDirectiveSymbol ,          // kind
    {
      context->get_code_offset(),        // c_code
      BrigGroupSpace,                    // storag class
      context->get_attribute() ,         // attribut
      0,                                 // reserved
      context->get_symbol_modifier(),    // symbolModifier
      context->get_dim(),                // dim
      str_offset,                        // s_name
      context->get_type(),               // type
      context->get_alignment(),          // align
    },
    0,                             // d_init
    0,                             // reserved
  };
  context->global_symbol_map[symName] = context->get_directive_offset();
  context->append_directive(&bds);
  context->set_alignment(0);

  context->token_to_scan = yylex();
  return 0;
}

int MulInst(Context* context) {

  BrigAluModifier aluModifier = {0, 0, 0, 0, 0, 0, 0};
  BrigOpcode32_t opcode;
  BrigPacking16_t packing = BrigNoPacking;
  BrigDataType16_t type;
  BrigoOffset32_t OpOffset[3] = {0,0,0};

  if ((context->token_to_scan != MUL) &&
      (context->token_to_scan != MUL_HI)) {
      return 1;
  }
  opcode = context->token_value.opcode;
  context->token_to_scan = yylex();

  if (opcode == BrigMul) {
    if (RoundingMode(context)) {
      context->set_error(INVALID_MODIFIER);
      return 1;
    }
  }
  aluModifier = context->get_alu_modifier();

  if (context->token_type == PACKING) {
    packing = context->token_value.packing;
    context->token_to_scan = yylex();
  }
  size_t opCount = 0;

  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;
  context->token_to_scan = yylex();

  if (!CheckDestRegister(context->token_to_scan, type, opcode)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[opCount++])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (Operand(context, &OpOffset[opCount++])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (Operand(context, &OpOffset[opCount++])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }

  if (!aluModifier.valid) {
    BrigInstBase mulInst = {
      0,                     // size
      BrigEInstBase,         // kind
      opcode,                // opcode
      type,                  // type
      packing,               // packing
      {OpOffset[0], OpOffset[1], OpOffset[2], 0, 0}
    };
    mulInst.size = sizeof(mulInst);
    context->append_code(&mulInst);
  } else {
    BrigInstMod mulMod = {
      0,                     // size
      BrigEInstMod,          // kind
      opcode,                // opcode
      type,                  // type
      packing,               // packing
      {OpOffset[0], OpOffset[1], OpOffset[2], 0, 0},
      aluModifier
    };
    mulMod.size = sizeof(mulMod);
    context->append_code(&mulMod);
  }
  context->token_to_scan = yylex();
  return 0;
}

int Mul24Inst(Context* context) {

  BrigOpcode32_t opcode;
  BrigDataType16_t type;
  BrigoOffset32_t OpOffset[3] = {0,0,0};
  if ((context->token_to_scan != MUL24_HI) &&
      (context->token_to_scan != MUL24)) {
    return 1;
  }
  opcode = context->token_value.opcode;
  context->token_to_scan = yylex();
  size_t opCount = 0;

  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;
  context->token_to_scan = yylex();

  if (!CheckDestRegister(context->token_to_scan, type, opcode)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[opCount++])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (Operand(context, &OpOffset[opCount++])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (Operand(context, &OpOffset[opCount++])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  context->token_to_scan = yylex();

  BrigInstBase bib = {
    0,                    //size
    BrigEInstBase,        //kind
    opcode,               //opcode
    type,                 //type
    BrigNoPacking,        //packing
    {OpOffset[0],OpOffset[1],OpOffset[2],0,0}
  };
  bib.size = sizeof(bib);
  context->append_code(&bib);
  return 0;

}

int Mad24Inst(Context* context) {

  BrigOpcode32_t opcode;
  BrigDataType16_t type;
  BrigoOffset32_t OpOffset[4] = {0,0,0,0};
  if ((context->token_to_scan != MAD24_HI) &&
      (context->token_to_scan != MAD24)) {
    return 1;
  }

  opcode = context->token_value.opcode;
  context->token_to_scan = yylex();
  size_t opCount = 0;

  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;
  context->token_to_scan = yylex();

  if (!CheckDestRegister(context->token_to_scan, type, opcode)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[opCount++])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (Operand(context, &OpOffset[opCount++])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (Operand(context, &OpOffset[opCount++])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (Operand(context, &OpOffset[opCount++])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  context->token_to_scan = yylex();

  BrigInstBase bib = {
    0,                    //size
    BrigEInstBase,        //kind
    opcode,               //opcode
    type,                 //type
    BrigNoPacking,        //packing
    {OpOffset[0],OpOffset[1],OpOffset[2], OpOffset[3],0}
  };
  bib.size = sizeof(bib);
  context->append_code(&bib);
  return 0;
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
        case _V2: *pVec_size = 2; break;
        case _V4: *pVec_size = 4; break;
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

    BrigMemorySemantic32_t pmemSem;

    if(!Acq(context, &pmemSem)){
      pLdSt_op->memorySemantic = pmemSem;
      continue;
    }

    if (context->token_to_scan == _EQUIV) {
      context->token_to_scan = yylex();

      if (context->token_to_scan != '(') {
        context->set_error(MISSING_OPENNING_BRACKET);
        return 1;
      }
      context->token_to_scan = yylex();
      if (context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
        context->set_error(MISSING_INTEGER_CONSTANT);
        return 1;
      }
      pLdSt_op->equivClass = context->token_value.int_val;
      context->token_to_scan = yylex();
      if (context->token_to_scan != ')') {
        context->set_error(MISSING_CLOSING_PARENTHESIS);
        return 1;
      }
      context->token_to_scan = yylex();
      continue;
    }
    return 0;
  }

  return 1;
}

int LdModifier(Context *context) {
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
  int vector_size = 0;
  BrigDataType16_t type = Brigb32;
  BrigInstLdSt tmp;
  BrigoOffset32_t OpOffset[3] = {0, 0, 0};
  context->token_to_scan = yylex();
  BrigoOffset32_t widthOffset = context->get_operand_offset();
  if (widthOffset % 8) {
    widthOffset += 8 - widthOffset % 8;
  }
  OpOffset[0] = widthOffset;

  if (context->token_to_scan == _WIDTH) {
    if (OptionalWidth(context)) {
      return 1;
    }
  } else {
   BrigOperandImmed op_width = {
      sizeof(BrigOperandImmed),
      BrigEOperandImmed,
      Brigb32,
      0,
      { 0 }
    };
    op_width.bits.u = 1;
    context->append_operand(&op_width);
  }

  tmp.storageClass = BrigFlatSpace;
  tmp.memorySemantic = BrigRegular;
  tmp.equivClass = 0;
  if (LdModifierPart2(context, &tmp, &vector_size)) {
    return 1;
  }
  context->set_storageClass(tmp.storageClass);

  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  if (context->token_to_scan != _B128 &&
      context->token_to_scan != _U8 &&
      context->token_to_scan != _U16 &&
      context->token_to_scan != _U32 &&
      context->token_to_scan != _U64 &&
      context->token_to_scan != _S8 &&
      context->token_to_scan != _S16 &&
      context->token_to_scan != _S32 &&
      context->token_to_scan != _S64 &&
      context->token_to_scan != _F16 &&
      context->token_to_scan != _F32 &&
      context->token_to_scan != _F64) {
    context->set_error(INVALID_DATA_TYPE);
    return 1;
  }
  // Get Type value in here
  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;
  context->token_to_scan = yylex();

  if (ArrayOperand(context, &OpOffset[1])) {
    context->set_error(INVALID_FIRST_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (MemoryOperand(context, &OpOffset[2])) {
    context->set_error(INVALID_SECOND_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstLdSt ld_op = {
    0,                        // size
    BrigEInstLdSt,            // kind
    BrigLd,                   // opcode
    type,                     // type
    BrigNoPacking,            // packing
    {0, 0, 0, 0, 0},          // operand[5]
    tmp.storageClass,         // storageClass
    tmp.memorySemantic,       // memorySemantic
    tmp.equivClass            // equivClass
  };
  for (int i = 0 ; i < 3 ; ++i) {
    ld_op.o_operands[i] = OpOffset[i];
  }
  ld_op.size = sizeof(ld_op);
  context->append_code(&ld_op);
  context->token_to_scan = yylex();
  return 0;
}

int St(Context* context) {
  // first token is St
  BrigDataType16_t type = Brigb32;
  BrigInstLdSt tmp;
  BrigoOffset32_t OpOffset[2] = {0, 0};
  int vector_size = 0;
  context->token_to_scan = yylex();

  tmp.storageClass = BrigFlatSpace;
  tmp.memorySemantic  = BrigRegular;
  tmp.equivClass = 0;
  if (LdModifierPart2(context, &tmp, &vector_size)) {
    return 1;
  }
  context->set_storageClass(tmp.storageClass);

  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;
  context->token_to_scan = yylex();

  if (ArrayOperand(context, &OpOffset[0])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  if (MemoryOperand(context, &OpOffset[1])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstLdSt st_op = {
    0,                        // size
    BrigEInstLdSt,            // kind
    BrigSt,                   // opcode
    type,                     // type
    BrigNoPacking,            // packing
    {OpOffset[0], OpOffset[1], 0, 0, 0},          // operand[5]
    tmp.storageClass,         // storageClass
    tmp.memorySemantic,       // memorySemantic
    tmp.equivClass            // equivClass
  };
  st_op.size = sizeof(st_op);
  context->append_code(&st_op);
  context->token_to_scan = yylex();
  return 0;
}

int Lda(Context* context) {
  // first token is LDA
  context->token_to_scan = yylex();
  BrigDataType16_t type = Brigb32;
  BrigoOffset32_t OpOffset[2] = {0, 0};
  BrigStorageClass32_t storageClass = BrigFlatSpace;

  if (!AddressSpaceIdentifier(context)) {
    storageClass = context->token_value.storage_class;
    context->set_storageClass(storageClass);
    context->token_to_scan = yylex();
  }

  // Length: 1, 32, 64
  if (context->token_to_scan != _U32 &&
      context->token_to_scan != _U64) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }

  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;
  context->token_to_scan = yylex();

  if (!CheckDestRegister(context->token_to_scan, type, BrigLda)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[0])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  if (MemoryOperand(context, &OpOffset[1])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstMem lda_op = {
    0,                     // size
    BrigEInstMem,          // kind
    BrigLda,               // opcode
    type,                  // type
    BrigNoPacking,         // packing
    {OpOffset[0], OpOffset[1], 0, 0, 0},
    storageClass
  };
  lda_op.size = sizeof(lda_op);
  context->append_code(&lda_op);
  context->token_to_scan = yylex();
  return 0;
}

int Acq(Context* context, BrigMemorySemantic32_t* pMemSem){
  switch(context->token_to_scan){
    case _ACQ:
      *pMemSem = BrigAcquire;
      break;
    case _PART_ACQ:
      *pMemSem = BrigParAcquire;
      break;
    case _REL:
      *pMemSem = BrigRelease;
      break;
    case _PART_REL:
      *pMemSem = BrigParRelease;
      break;
    default:
      return 1;
  }
  context->token_to_scan = yylex();
  return 0;
}

int AcqRel(Context* context, BrigMemorySemantic32_t* pMemSem ){
  switch(context->token_to_scan){
    case _AR:
      *pMemSem = BrigAcquireRelease;
      break;
    case _PART_AR:
      *pMemSem = BrigParAcquireRelease;
      break;
    default:
      return 1;
  }
  context->token_to_scan = yylex();
  return 0;
}

int Optacqreg(Context* context, BrigMemorySemantic32_t* pMemSemantic) {

  /* Either acq or acqrel is allowed - not both*/
  int ret = (Acq(context, pMemSemantic));
  int ret2 = (AcqRel(context, pMemSemantic));
  int ret3 = (Acq(context, pMemSemantic));

  ret = ((ret | ret2)==0) || ((ret2 | ret3)==0) ? 1 : 0;
  return ret;
}

int AddressSpaceIdentifier(Context* context){

  switch(context->token_to_scan){
    case _READONLY:
    case _KERNARG:
    case _GLOBAL:
    case _PRIVATE:
    case _ARG:
    case _GROUP:
    case _SPILL:
      return 0;
    default:
      return 1;
  }
}

int ImageRet(Context* context) {
  // first token is ATOMIC_IMAGE
  unsigned int second_token;
  BrigDataType16_t type = Brigb32;
  BrigoOffset32_t OpOffset[5] = {0, 0, 0, 0, 0};
  BrigAtomicOperation32_t atomicOperation = 0;
  BrigMemorySemantic32_t memorySemantic = BrigRegular;
  BrigGeom32_t geom = 0;
  context->token_to_scan = yylex();
  second_token = context->token_to_scan;

  if (context->token_to_scan == _CAS) {
    atomicOperation = BrigAtomicCas;
    context->token_to_scan = yylex();
  } else if (context->token_type == ATOMIC_OP) {
    switch (context->token_to_scan) {  // without _CAS_
      case _AND_: atomicOperation = BrigAtomicAnd; break;
      case _OR_: atomicOperation = BrigAtomicOr; break;
      case _XOR_: atomicOperation = BrigAtomicXor; break;
      case _EXCH_: atomicOperation = BrigAtomicExch; break;
      case _ADD_: atomicOperation = BrigAtomicAdd; break;
      case _INC_: atomicOperation = BrigAtomicInc; break;
      case _DEC_: atomicOperation = BrigAtomicDec; break;
      case _MIN_: atomicOperation = BrigAtomicMin; break;
      case _MAX_: atomicOperation = BrigAtomicMax; break;
      case _SUB_: atomicOperation = BrigAtomicSub; break;
      default:
        context->set_error(MISSING_DECLPREFIX);
        return 1;
    }
    context->token_to_scan =yylex();
  } else {
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }
  if(Optacqreg(context, &memorySemantic))
    return 1;

  if (context->token_type != GEOMETRY_ID) {
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }
  switch (context->token_to_scan) {
    case _1D: geom = Briggeom_1d; break;
    case _2D: geom = Briggeom_2d; break;
    case _3D: geom = Briggeom_3d; break;
    case _1DB: geom = Briggeom_1db; break;
    case _1DA: geom = Briggeom_1da; break;
    case _2DA: geom = Briggeom_2da; break;
    default:
      context->set_error(MISSING_DECLPREFIX);
      return 1;
  }
  context->token_to_scan = yylex();
  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;

  context->token_to_scan = yylex();

  if (!CheckDestRegister(context->token_to_scan, type, BrigAtomicImage)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[0])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  if (context->token_to_scan != '[') {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  context->token_to_scan = yylex();
  if (AddressableOperand(context, &OpOffset[1], true)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  unsigned int curOpCount = 2;
  if (ArrayOperand(context, &OpOffset[curOpCount++])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (Operand(context, &OpOffset[curOpCount++])) {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  if (second_token == _CAS) {
    if (context->token_to_scan != ',') {
      context->set_error(MISSING_COMMA);
      return 1;
    }
    context->token_to_scan = yylex();

    if (Operand(context, &OpOffset[curOpCount++])) {
      context->set_error(MISSING_OPERAND);
      return 1;
    }
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstAtomicImage img_inst = {
    0,                      // size
    BrigEInstAtomicImage,   // kind
    BrigAtomicImage,        // opcode
    type,                   // type
    BrigNoPacking,          // packing
    {OpOffset[0], OpOffset[1], OpOffset[2], OpOffset[3], OpOffset[4]},        // o_operands[5]
    atomicOperation,        // atomicOperation
    BrigGlobalSpace,        // storageClass
    memorySemantic,         // memorySemantic
    geom                    // geom
  };
  img_inst.size = sizeof(img_inst);
  context->append_code(&img_inst);
  context->token_to_scan = yylex();
  return 0;
}

int ImageNoRet(Context* context) {
  // first token is ATOMICNORET_IMAGE
  BrigDataType16_t type = Brigb32;
  BrigoOffset32_t OpOffset[4] = {0, 0, 0, 0};
  BrigAtomicOperation32_t atomicOperation = 0;
  BrigMemorySemantic32_t memorySemantic = BrigRegular;
  BrigGeom32_t geom = 0;

  unsigned int second_token;
  context->token_to_scan = yylex();

  second_token = context->token_to_scan;

  if (context->token_to_scan == _CAS) {
    atomicOperation = BrigAtomicCas;
    context->token_to_scan = yylex();
  } else if (context->token_type == ATOMIC_OP) {
    switch (context->token_to_scan) {  // without _CAS_ , _EXCh_
      case _AND_: atomicOperation = BrigAtomicAnd; type = Brigb32; break;
      case _OR_: atomicOperation = BrigAtomicOr; type = Brigb32; break;
      case _XOR_: atomicOperation = BrigAtomicXor; type = Brigb32; break;
      case _ADD_: atomicOperation = BrigAtomicAdd; type = Brigu64; break;
      case _INC_: atomicOperation = BrigAtomicInc; type = Brigs32; break;
      case _DEC_: atomicOperation = BrigAtomicDec; type = Brigb32; break;
      case _MIN_: atomicOperation = BrigAtomicMin; type = Brigu32; break;
      case _MAX_: atomicOperation = BrigAtomicMax; type = Brigu32; break;
      case _SUB_: atomicOperation = BrigAtomicSub; type = Brigu64; break;
      default:
        context->set_error(MISSING_DECLPREFIX);
        return 1;
    }
    context->token_to_scan =yylex();
  } else {
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }

  if(Optacqreg(context, &memorySemantic))
    return 1;

  if (context->token_type != GEOMETRY_ID) {
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }
  switch (context->token_to_scan) {
    case _1D: geom = Briggeom_1d; break;
    case _2D: geom = Briggeom_2d; break;
    case _3D: geom = Briggeom_3d; break;
    case _1DB: geom = Briggeom_1db; break;
    case _1DA: geom = Briggeom_1da; break;
    case _2DA: geom = Briggeom_2da; break;
    default:
      context->set_error(MISSING_DECLPREFIX);
      return 1;
  }
  context->token_to_scan = yylex();
  // TypeLength: u32, s32, b32, u64, depending on the type of operation.
  // The add operation applies to u32, u64, and s32 types;
  // min and max apply to u32 and s32 types; inc and dec apply to s32 types;
  // and and, or, xor, and cas apply to b32 types.

  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }

  type = context->token_value.data_type;
  context->set_type(context->token_value.data_type);

  context->token_to_scan = yylex();
  if (context->token_to_scan != '[') {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  context->token_to_scan = yylex();
  if (AddressableOperand(context, &OpOffset[0], true)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  if (ArrayOperand(context, &OpOffset[1])) {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  unsigned int opCount = 2;

  if (Operand(context, &OpOffset[opCount++])) {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  if (second_token == _CAS) {
    if (context->token_to_scan != ',') {
      context->set_error(MISSING_COMMA);
      return 1;
    }
    context->token_to_scan = yylex();
    if (Operand(context, &OpOffset[opCount++])) {
      context->set_error(MISSING_OPERAND);
      return 1;
    }
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstAtomicImage imgNoRet = {
    0,
    BrigEInstAtomicImage,   // kind
    BrigAtomicNoRetImage,   // opcode
    type,                   // type
    BrigNoPacking,          // packing
    {0, 0, 0, 0, 0},        // o_operands[5]
    atomicOperation,        // atomicOperation
    BrigGlobalSpace,        // storageClass
    memorySemantic,         // memorySemantic
    geom                    // geom
  };
  imgNoRet.size = sizeof(imgNoRet);
  for (int i = 0 ; i < 4 ; ++i) {
    imgNoRet.o_operands[i] = OpOffset[i];
  }
  context->append_code(&imgNoRet);
  context->token_to_scan = yylex();
  return 0;
}

int Cvt(Context* context) {
  // first token is CVT "cvt"
  // TODO(Chuang): Extensions for Conversions
  BrigAluModifier getAluMod = {0, 0, 0, 0, 0, 0, 0};
  BrigDataType16_t type = Brigb32;
  BrigDataType16_t stype = Brigb32;
  BrigoOffset32_t OpOffset[2] = {0, 0};
  context->token_to_scan = yylex();

  if (RoundingMode(context)) {
    context->set_error(INVALID_MODIFIER);
    return 1;
  }
  getAluMod = context->get_alu_modifier();

  // destType: b, u, s, f. (For b, only b1 is supported.)
  // destLength: 1, 8, 16, 32, 64. (For 1, only b1 is supported.)

  if (context->token_to_scan != _B1 &&
      context->token_to_scan != _S8 &&
      context->token_to_scan != _S16 &&
      context->token_to_scan != _S32 &&
      context->token_to_scan != _S64 &&
      context->token_to_scan != _U8 &&
      context->token_to_scan != _U16 &&
      context->token_to_scan != _U32 &&
      context->token_to_scan != _U64 &&
      context->token_to_scan != _F16 &&
      context->token_to_scan != _F32 &&
      context->token_to_scan != _F64) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }

  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;
  context->token_to_scan = yylex();

  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  stype = context->token_value.data_type;
  context->token_to_scan = yylex();

  if (!CheckDestRegister(context->token_to_scan, type, BrigCvt)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[0])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  if (Operand(context, &OpOffset[1])) {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstCvt cvtInst = {
    0,                     // size
    BrigEInstCvt,          // kind
    BrigCvt,               // opcode
    type,                  // type
    BrigNoPacking,         // packing
    {OpOffset[0], OpOffset[1], 0, 0, 0},       // o_operands[5]
    getAluMod,             // aluModifier
    stype,                 // stype
    0                      // reserved
  };
  cvtInst.size = sizeof(cvtInst);
  context->append_code(&cvtInst);
  context->token_to_scan = yylex();
  return 0;
}

int Instruction0(Context* context) {
  // first token is NOP "nop"
  context->token_to_scan = yylex();
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstBase inst0 = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigNop,               // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };
  context->append_code(&inst0);
  context->token_to_scan = yylex();
  return 0;
}

int Instruction1OpcodeDT(Context* context) {
  BrigoOffset32_t OpOffset[2] = {0, 0};
  BrigAluModifier aluModifier = {0, 0, 0, 0, 0, 0, 0};
  BrigStorageClass32_t storageClass = BrigFlatSpace;
  BrigOpcode32_t opcode = 0;
  BrigDataType16_t type = Brigb32;

  opcode = context->token_value.opcode;
  // TODO(Chuang): fbar_wait_segment_b64
  // fbar_init_segment_b64
  // fbar_release_segment_b64
  // and whether there should be rounding in instruction1opcode.
  context->token_to_scan = yylex();

  if (opcode == BrigNullPtr) {
    if (context->token_type == ADDRESS_SPACE_IDENTIFIER) {
      storageClass = context->token_value.storage_class;
      context->token_to_scan = yylex();
    }
  }

  if (opcode != BrigFbarInit &&
      opcode != BrigFbarRelease) {
    // TODO(Chuang): whether support for rounding
    if (RoundingMode(context)) {
      context->set_error(INVALID_MODIFIER);
      return 1;
    }
    aluModifier = context->get_alu_modifier();
  }
  // TODO(Chuang): What can the type of BrigFbarInit be?
  if (context->token_to_scan != _B64 &&
      context->token_to_scan != _U32 &&
      context->token_to_scan != _U64) {
    context->set_error(INVALID_DATA_TYPE);
    return 1;
  }

  type = context->token_value.data_type;
  context->set_type(context->token_value.data_type);

  context->token_to_scan = yylex();

  if (!CheckDestRegister(context->token_to_scan, type, opcode)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (Operand(context, &OpOffset[0])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  // Note: Fbar_Release and Fbar_Init may be a instruction2.
  if (opcode == BrigFbarInit ||
      opcode == BrigFbarRelease) {
    if (context->token_to_scan == ',')  {
      context->token_to_scan = yylex();
      if (context->token_to_scan != TOKEN_DREGISTER) {
        context->set_error(MISSING_OPERAND);
        return 1;
      }
      if (Operand(context, &OpOffset[1])) {
        return 1;
      }
    }
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  if (opcode == BrigNullPtr) {
    BrigInstMem mem = {
      0,
      BrigEInstMem,
      opcode,
      type,
      BrigNoPacking,
      {OpOffset[0], 0, 0, 0, 0},
      storageClass
    };
    mem.size = sizeof(mem);
    context->append_code(&mem);
    return 0;
  }
  if (aluModifier.valid) {
    BrigInstMod mod = {
      0,
      BrigEInstMod,         // kind
      opcode,               // opcode
      type,                 // type
      BrigNoPacking,        // packing
      {OpOffset[0], OpOffset[1], 0, 0, 0},      // o_operands[5]
      aluModifier
    };
    mod.size = sizeof(mod);
    context->append_code(&mod);
  } else {
    BrigInstBase inst1_op = {
      0,
      BrigEInstBase,
      opcode,
      type,
      BrigNoPacking,
      {OpOffset[0], OpOffset[1], 0, 0, 0}
    };
    inst1_op.size = sizeof(inst1_op);
    context->append_code(&inst1_op);
  }
  context->token_to_scan = yylex();
  return 0;
}

int Instruction1OpcodeNoDT(Context* context) {
  BrigoOffset32_t OpOffset[1] = {0};
  BrigAluModifier aluModifier = {0, 0, 0, 0, 0, 0, 0};
  BrigOpcode32_t opcode = 0;

  context->set_type(Brigb32);
  opcode = context->token_value.opcode;

  context->token_to_scan = yylex();
  // TODO(Chuang): whether support for rounding
  if (RoundingMode(context)) {
    context->set_error(INVALID_MODIFIER);
    return 1;
  }
  aluModifier = context->get_alu_modifier();

  if (opcode == BrigQid) {
    if (context->token_to_scan != TOKEN_SREGISTER) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
  }

  if (context->token_to_scan != TOKEN_SREGISTER &&
      context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (Operand(context, &OpOffset[0])) {
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  if (aluModifier.valid) {
    BrigInstMod mod = {
      0,                    // size
      BrigEInstMod,         // kind
      opcode,      // opcode
      Brigb32,        // type
      BrigNoPacking,        // packing
      {OpOffset[0], 0, 0, 0, 0},      // o_operands[5]
      aluModifier           // aluModifier
    };
    mod.size = sizeof(mod);
    context->append_code(&mod);
  } else {
    BrigInstBase inst1_op = {
      0,
      BrigEInstBase,
      opcode,
      Brigb32,
      BrigNoPacking,
      {OpOffset[0], 0, 0, 0, 0}
    };
    inst1_op.size = sizeof(inst1_op);
    context->append_code(&inst1_op);
  }
  context->token_to_scan = yylex();
  return 0;
}

int Instruction1Clock(Context* context) {
  BrigoOffset32_t OpOffset[1] = {0};
  context->set_type(Brigb64);
  context->token_to_scan = yylex();
  if (context->token_to_scan != TOKEN_DREGISTER) {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  if (Operand(context, &OpOffset[0])) {
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstBase inst1_op = {
    0,
    BrigEInstBase,
    BrigClock,
    Brigb64,
    BrigNoPacking,
    {OpOffset[0], 0, 0, 0, 0}
  };
  inst1_op.size = sizeof(inst1_op);
  context->append_code(&inst1_op);
  context->token_to_scan = yylex();
  return 0;
}

int Instruction1(Context* context) {
  if (context->token_to_scan == CLOCK) {
    if (!Instruction1Clock(context)) {
      return 0;
    }
  } else if (context->token_type == INSTRUCTION1_OPCODE_NODT) {
    if (!Instruction1OpcodeNoDT(context)) {
      return 0;
    }
  } else if (context->token_type == INSTRUCTION1_OPCODE) {
    if (!Instruction1OpcodeDT(context)) {
      return 0;
    }
  }
  return 1;
}

int Segp(Context* context) {
  switch (context->token_to_scan){
    case SEGMENTP:
      return SegpPart1Segmentp(context);
    case STOF:
    case FTOS:
      return SegpPart2StoFAndFtoS(context);
  }
  return 1;
}

int SegpPart1Segmentp(Context* context) {
  context->token_to_scan = yylex();
  BrigDataType16_t type = Brigb32;
  BrigDataType16_t stype = Brigb32;
  BrigoOffset32_t OpOffset[2] = {0, 0};
  BrigStorageClass32_t storageClass =  0;

  if (context->token_type != ADDRESS_SPACE_IDENTIFIER) {
    context->set_error(MISSING_ADDRESSSPACE_IDENTIFIER);
    return 1;
  }
  storageClass = context->token_value.storage_class;

  context->token_to_scan = yylex();
  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  // datatypeId must be b1
  if (context->token_to_scan != _B1) {
    context->set_error(INVALID_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;

  context->token_to_scan = yylex();
  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  // src must be u32/64
  context->set_type(context->token_value.data_type);
  stype = context->token_value.data_type;

  if (!CheckDataType(context->get_machine(), storageClass, stype)) {
    context->set_error(INVALID_DATA_TYPE);
    return 1;
  }

  context->token_to_scan = yylex();
  if (context->token_type != REGISTER) {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  // dest must be c register
  if (context->token_to_scan != TOKEN_CREGISTER) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (Operand(context, &OpOffset[0])) {
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  if (context->token_type == REGISTER) {
    if (!CheckRegister(context->token_to_scan, stype)) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
  } else if (context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (Operand(context, &OpOffset[1])) {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstSegp segmentp_op = {
    0,                     // size
    BrigEInstSegp,          // kind
    BrigSegmentp,          // opcode
    type,                  // type
    BrigNoPacking,         // packing
    {OpOffset[0], OpOffset[1], 0, 0, 0},       // o_operands[5]
    storageClass,          // storageClass
    stype,
    0
  };
  segmentp_op.size = sizeof(segmentp_op);
  context->append_code(&segmentp_op);
  context->token_to_scan = yylex();
  return 0;
}

int SegpPart2StoFAndFtoS(Context* context) {
  unsigned int first_token = context->token_to_scan;
  BrigDataType16_t type = Brigb32;
  BrigDataType16_t stype = Brigb32;
  BrigoOffset32_t OpOffset[2] = {0, 0};
  BrigOpcode32_t opcode = 0;
  BrigStorageClass32_t storageClass = BrigFlatSpace;
  BrigStorageClass32_t destAddrType, srcAddrType;

  context->token_to_scan = yylex();
  if (context->token_type != ADDRESS_SPACE_IDENTIFIER) {
    context->set_error(MISSING_ADDRESSSPACE_IDENTIFIER);
    return 1;
  }
  storageClass = context->token_value.storage_class;

  switch (first_token) {
    case STOF:
      opcode = BrigStoF;
      destAddrType = BrigFlatSpace;
      srcAddrType = storageClass;
      break;
    case FTOS:
      opcode = BrigFtoS;
      destAddrType = storageClass;
      srcAddrType = BrigFlatSpace;
      break;
  }

  context->token_to_scan = yylex();
  type = context->token_value.data_type;

  if (!CheckDataType(context->get_machine(), destAddrType, type)) {
    context->set_error(INVALID_DATA_TYPE);
    return 1;
  }

  context->token_to_scan = yylex();
  stype = context->token_value.data_type;

  if (!CheckDataType(context->get_machine(), srcAddrType, stype)) {
    context->set_error(INVALID_DATA_TYPE);
    return 1;
  }

  context->token_to_scan = yylex();
  if (context->token_type == REGISTER) {
    if (!CheckRegister(context->token_to_scan, type)) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
  } else if (context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  context->set_type(type);
  if (Operand(context, &OpOffset[0])) {
    context->set_error(MISSING_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }

  context->token_to_scan = yylex();
  if (context->token_type == REGISTER) {
    if (!CheckRegister(context->token_to_scan, stype)) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
  } else if (context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  context->set_type(stype);
  if (Operand(context, &OpOffset[1])) {
    context->set_error(MISSING_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }

  BrigInstSegp sf_op = {
    0,                     // size
    BrigEInstSegp,         // kind
    opcode,                // opcode
    type,                  // type
    BrigNoPacking,         // packing
    {OpOffset[0], OpOffset[1], 0, 0, 0},       // o_operands[5]
    storageClass,           // storageClass
    stype,
    0
  };
  sf_op.size = sizeof(sf_op);
  context->append_code(&sf_op);
  context->token_to_scan = yylex();
  return 0;
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
  } else if (context->token_type == MUL_OP) {
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
    return Comment(context);
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
             context->token_to_scan == STATIC ||
             context->token_type == UNINITIALIZABLE_ADDRESS ||
             context->token_type == INITIALIZABLE_ADDRESS) {
    if (!DeclPrefix(context)) {
      if (!InitializableDecl(context, &context->arg_symbol_map)) {
        return 0;
      } else if (!UninitializableDecl(context, &context->arg_symbol_map)) {
        return 0;
      }
    }
  } else if (context->token_to_scan == LOC) {
    if (!Location(context)) {
      return 0;
    }
  } else if (context->token_to_scan == TOKEN_LABEL) {
    std::string labelName = context->token_value.string_val;
    if (Label(context, &context->arg_symbol_map)) {
      return 1;
    }
    if(context->token_to_scan == LABELTARGETS) {
      return LabelTargetsPart2(context, context->get_symbol(labelName));
    }
    return 0;
  } else if (!Operation(context)) {
    context->update_bdf_operation_count();
    return 0;
  }
  return 1;
}

int ArgStatement(Context* context) {

  context->initialize_statement_fields();
  if (context->token_to_scan == CALL) {
    if (!Call(context)) {
      context->update_bdf_operation_count();
      return 0;
    }
  } else if (!BodyStatementNested(context)) {
    return 0;
  } else if (context->token_to_scan == ALIGN ||
             context->token_to_scan == CONST ||
             context->token_to_scan == EXTERN ||
             context->token_to_scan == STATIC ||
             context->token_to_scan == ARG) {
    if (!DeclPrefix(context)) {
      if (!ArgUninitializableDecl(context)) {
        return 0;
      }
    }
  }
  return 1;
}

int ArgStatements(Context* context) {
  if (ArgStatement(context)) {
    return 1;
  }
  while (!ArgStatement(context)) {
    ;
  }
  return 0;
}

int Comment(Context* context) {
  if(context->token_to_scan != TOKEN_COMMENT) {
    context->set_error(INVALID_COMMENT);
    return 1;
  }
  std::string comment(context->token_value.string_val);
  int comment_offset = context->add_symbol(comment);
  BrigDirectiveComment dir_com = {
    sizeof(BrigDirectiveComment), BrigEDirectiveComment,
    context->get_code_offset(), comment_offset};
  context->append_directive(&dir_com);
  context->token_to_scan = yylex();
  return 0;
}

int BodyStatement(Context* context) {

  context->initialize_statement_fields();
  if (context->token_to_scan == TOKEN_COMMENT) {
    return Comment(context);
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
             context->token_to_scan == STATIC ||
             context->token_type == UNINITIALIZABLE_ADDRESS ||
             context->token_type == INITIALIZABLE_ADDRESS) {
    if (!DeclPrefix(context)) {
      if (!InitializableDecl(context, &context->local_symbol_map)) {
        return 0;
      } else if (!UninitializableDecl(context, &context->local_symbol_map)) {
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
    std::string labelName = context->token_value.string_val;
    if (Label(context, &context->local_symbol_map)) {
      return 1;
    }
    if (context->token_to_scan == LABELTARGETS)
      return LabelTargetsPart2(context, context->get_symbol(labelName));
    return 0;
  } else if (!Operation(context)) {
    context->update_bdf_operation_count();
    return 0;
  }
  return 1;
}

int BodyStatements(Context* context) {
  while (context->token_to_scan && (context->token_to_scan != '}')) {
    if(BodyStatement(context))
      break;
    // context->token_to_scan = yylex();
  }
  if((!context->token_to_scan) || (context->token_to_scan == '}'))
    return 0;
  return 1;
}

int ImageLoad(Context* context) {
  // first token is LD_IMAGE
  BrigDataType16_t type = Brigb32;
  BrigDataType16_t stype = Brigb32;
  BrigoOffset32_t OpOffset[3] = {0, 0, 0};
  BrigGeom32_t geom = 0;
  context->token_to_scan = yylex();
  if (context->token_to_scan != _V4) {
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }
  context->token_to_scan = yylex();
  if (context->token_type != GEOMETRY_ID) {
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }
  switch (context->token_to_scan) {
    case _1D: geom = Briggeom_1d; break;
    case _2D: geom = Briggeom_2d; break;
    case _3D: geom = Briggeom_3d; break;
    case _1DB: geom = Briggeom_1db; break;
    case _1DA: geom = Briggeom_1da; break;
    case _2DA: geom = Briggeom_2da; break;
    default:
      context->set_error(MISSING_DECLPREFIX);
      return 1;
  }
  context->token_to_scan = yylex();
  // Note: destLength: Destination length: 32 (f32, u32, or s32)
  if (context->token_to_scan != _F32 &&
      context->token_to_scan != _U32 &&
      context->token_to_scan != _S32) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  type = context->token_value.data_type;
  context->token_to_scan = yylex();
  // Note: srcLength: Source length: 32 srcType: u32.
  if (context->token_to_scan != _U32) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  stype = context->token_value.data_type;
  context->token_to_scan = yylex();
  if (ArrayOperand(context, &OpOffset[0])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  // Note: dest: Destination. Must be a vector of four s registers.
  uint16_t kind;
  context->get_operand(OpOffset[0] + sizeof(uint16_t), &kind);
  if (kind != BrigEOperandRegV4) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  // TODO(Chuang): [image]: The image. Must be a read-write or read-only image.
  if (context->token_to_scan != '[') {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  context->token_to_scan = yylex();
  if (AddressableOperand(context, &OpOffset[1], true)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  // TODO(Chuang): src: Register source for the coordinates.
  // A scalar for 1D images; a 2-element vector for 2D images;
  // a 4-element vector for 3D images, where the fourth element is ignored.
  // Each coordinate must be in an s register
  if (ArrayOperand(context, &OpOffset[2])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ';') {
     context->set_error(MISSING_SEMICOLON);
     return 1;
  }
  BrigInstImage imgLdInst = {
    0,                // size
    BrigEInstImage,   // kind
    BrigLdImage,      // opcode
    type,             // type
    BrigNoPacking,    // packing
    {0, 0, 0, 0, 0},  // o_operands[5]
    geom,             // geom
    stype,            // stype
    0                 // reserved
  };
  imgLdInst.size = sizeof(imgLdInst);
  for (int i = 0 ; i < 3 ; ++i) {
    imgLdInst.o_operands[i] = OpOffset[i];
  }
  context->append_code(&imgLdInst);
  context->token_to_scan = yylex();
  return 0;
}

int ImageStore(Context* context) {
  // first token is St_image
  BrigDataType16_t type = Brigb32;
  BrigDataType16_t stype = Brigb32;
  BrigoOffset32_t OpOffset[3] = {0, 0, 0};
  BrigGeom32_t geom = 0;
  context->token_to_scan = yylex();
  if (context->token_to_scan != _V4) {
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }
  context->token_to_scan = yylex();
  if (context->token_type != GEOMETRY_ID) {
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }
  switch (context->token_to_scan) {
    case _1D: geom = Briggeom_1d; break;
    case _2D: geom = Briggeom_2d; break;
    case _3D: geom = Briggeom_3d; break;
    case _1DB: geom = Briggeom_1db; break;
    case _1DA: geom = Briggeom_1da; break;
    case _2DA: geom = Briggeom_2da; break;
    default:
      context->set_error(MISSING_DECLPREFIX);
      return 1;
  }
  context->token_to_scan = yylex();
  // Note: destLength: Destination length: 32 (f32, u32, or s32)
  if (context->token_to_scan != _F32 &&
      context->token_to_scan != _U32 &&
      context->token_to_scan != _S32) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }

  type = context->token_value.data_type;
  context->token_to_scan = yylex();
  // Note: srcLength: Source length: 32 srcType: u32.
  if (context->token_to_scan != _U32) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  stype = context->token_value.data_type;
  context->token_to_scan = yylex();
  if (ArrayOperand(context, &OpOffset[0])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  // Note: dest: Destination. Must be a vector of four s registers.
  uint16_t kind;
  context->get_operand(OpOffset[0] + sizeof(uint16_t), &kind);
  if (kind != BrigEOperandRegV4) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  // TODO(Chuang): [image]: Destination image. A read-write image.
  if (context->token_to_scan != '[') {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  context->token_to_scan = yylex();
  if (AddressableOperand(context, &OpOffset[1], true)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  // TODO(Chuang): src: Register source for the coordinates.
  // A scalar for 1D images; a 2-element vector for 2D images;
  // a 4-element vector for 3D images, where the fourth element is ignored.
  // Each coordinate must be in an s register
  if (ArrayOperand(context, &OpOffset[2])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstImage imgStInst = {
    0,                  // size
    BrigEInstImage,     // kind
    BrigStImage,        // opcode
    type,               // type
    BrigNoPacking,      // packing
    {0, 0, 0, 0, 0},    // o_operands[5]
    geom,               // geom
    stype,              // stype
    0,                  // reserved
  };
  imgStInst.size = sizeof(imgStInst);
  for (int i = 0 ; i < 3 ; ++i ) {
    imgStInst.o_operands[i] = OpOffset[i];
  }
  context->append_code(&imgStInst);
  context->token_to_scan = yylex();
  return 0;
}

int SingleInitializer(Context* context, BrigdOffset32_t sym_offset){
  if (context->token_to_scan != TOKEN_SINGLE_CONSTANT) {
    return 1;
  }
  std::vector<float> single_list;
  if(SingleListSingle(context, &single_list)){
    return 1;
  }
  uint32_t elementCount = single_list.size();
  uint32_t n = 0;
  BrigDirectiveSymbol bds ;
  context->get_directive(sym_offset,&bds);

  n = bds.s.dim;
  if (0 == context->get_dim()) {
    if (context->get_isArray()) {
      // If array is empty [],  Update dim.
      bds.s.symbolModifier = BrigArray;
      context->set_symbol_modifier(BrigArray);
      bds.s.dim = elementCount;
      n = elementCount;
    } else {
      if (elementCount != 1) {
        context->set_error(INVALID_INITIALIZER);
        return 1;
      }
      bds.s.dim = 0;
      n = 1;
    }
  } else if (elementCount > bds.s.dim) {
    context->set_error(INVALID_INITIALIZER);
    return 1;
  }
  elementCount = n;
  context->set_dim(bds.s.dim);
  if (context->get_type() != Brigb32) {
    context->set_error(INVALID_INITIALIZER);
    return 1;
  }

  n = (n + 1) >> 1;
  size_t arraySize = sizeof(BrigDirectiveInit) + (n - 1) * sizeof(uint64_t);
  uint8_t *array = new uint8_t[arraySize];
  memset(array, 0 , sizeof(uint8_t) * arraySize);
  BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit*>(array);
  bdi->elementCount = elementCount;
  bdi->size = arraySize;
  bdi->kind = BrigEDirectiveInit;
  bdi->c_code = context->get_code_offset();
  bdi->type = context->get_type();
  bdi->reserved = 0;
  for (uint32_t i = 0; i < single_list.size(); i++ ) {
    *(float*)&bdi->initializationData.u32[i] = single_list[i];
  }
  bds.d_init = context->get_directive_offset();
  if (bds.d_init % 8) {
    bds.d_init += 8 - bds.d_init % 8;
  }
  unsigned char *bds_charp = reinterpret_cast<unsigned char*>(&bds);
  context->update_directive_bytes(bds_charp, sym_offset,
                                  sizeof(BrigDirectiveSymbol));
  context->append_directive(bdi);
  delete[] reinterpret_cast<uint8_t *>(bdi);
  bdi = NULL;

  return 0;

}

int SingleListSingle(Context * context, std::vector<float>* single_list) {

  uint32_t elementCount = 0;

  while (1) {
    elementCount++;
    single_list->push_back(context->token_value.float_val);

    context->token_to_scan = yylex();
    if (context->token_to_scan != ',') {
      break;
    }
    context->token_to_scan = yylex();
    if (context->token_to_scan != TOKEN_SINGLE_CONSTANT) {
      context->set_error(MISSING_SINGLE_CONSTANT);
      return 1;
    }
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

     if ('=' != context->token_to_scan) {
       context->set_error(INVALID_IMAGE_INIT);
       return 1;
     }
     context->token_to_scan = yylex();

     if (TOKEN_PROPERTY != context->token_to_scan) {
       context->set_error(MISSING_PROPERTY);
       return 1;
     }
     context->get_directive(&bdi);
     switch(first_token){
       case FORMAT:
         bdi.format = context->token_value.format;
         break;
       case ORDER:
         bdi.order  = context->token_value.order;
         break;
     }
  } else if (WIDTH == context->token_to_scan
          || HEIGHT == context->token_to_scan
          || DEPTH == context->token_to_scan) {
    first_token = context->token_to_scan ;

    context->token_to_scan = yylex();
    if ('=' != context->token_to_scan) {
      context->set_error(INVALID_IMAGE_INIT);
      return 1;
    }
    context->token_to_scan = yylex();

    if (TOKEN_INTEGER_CONSTANT != context->token_to_scan) {
      context->set_error(MISSING_PROPERTY);
      return 1;
    }
    context->get_directive(&bdi);
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
  }
  unsigned char *bdi_charp =
      reinterpret_cast<unsigned char *>(&bdi);

  context->update_last_directive(bdi_charp, sizeof(bdi));

  context->token_to_scan = yylex();
  return 0;
}

int GlobalImageDecl(Context *context) {
  if(context->token_to_scan != GLOBAL) {
    return 1;
  }
  context->token_to_scan = yylex();
  return (GlobalImageDeclPart2(context));
}

int GlobalImageDeclPart2(Context *context){
  // First token has been scanned and verified as global. Read next token.

  if (_RWIMG != context->token_to_scan) {
    return 1;
  }
  context->token_to_scan = yylex();

  if (TOKEN_GLOBAL_IDENTIFIER != context->token_to_scan) {
    context->set_error(MISSING_IDENTIFIER);
    return 1;
  }
  std::string var_name(context->token_value.string_val);
  int var_name_offset = context->add_symbol(var_name);
  if (context->global_symbol_map.count(var_name)) {
    context->set_error(REPEATED_DECLARATION);
    return 1;
  }

  context->token_to_scan = yylex();
  context->set_dim(0);
  if ('[' == context->token_to_scan) {
    if (ArrayDimensionSet(context)) {
      return 1;
    }
  }
  uint16_t naturalAlign = 16;

  if (context->get_alignment() == 0) {
    context->set_alignment(naturalAlign);
  } else if (context->get_alignment() < naturalAlign) {
    context->set_error(INVALID_ALIGNMENT);
    return 1;
  }

  BrigDirectiveImage bdi = {
    sizeof(BrigDirectiveImage),//size
    BrigEDirectiveImage,    //kind
    {
      context->get_code_offset(),      // c_code
      BrigGlobalSpace,                 // storag class
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
  context->global_symbol_map[var_name] = context->get_directive_offset();
  context->append_directive(&bdi);
  context->set_alignment(0);

  if ('=' == context->token_to_scan) {
    if (ImageInitializer(context)) {
      context->set_error(INVALID_IMAGE_INIT);
      return 1;
    }
  }

  if (';' != context->token_to_scan) {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  context->token_to_scan = yylex();
  return 0;

}

int GlobalReadOnlyImageDecl(Context *context) {
  if(GLOBAL != context->token_to_scan) {
    return 1;
  }
  context->token_to_scan = yylex();
  return GlobalReadOnlyImageDeclPart2(context);
}

int GlobalReadOnlyImageDeclPart2(Context *context){
  //First token has been scanned and verified as global. Scan next token.
  if (_ROIMG != context->token_to_scan) {
    return 1;
  }
  context->token_to_scan = yylex();
  if (TOKEN_GLOBAL_IDENTIFIER != context->token_to_scan) {
    context->set_error(MISSING_IDENTIFIER);
  }
  std::string var_name(context->token_value.string_val);
  int var_name_offset = context->add_symbol(var_name);
  if (context->global_symbol_map.count(var_name)) {
    context->set_error(REPEATED_DECLARATION);
    return 1;
  }

  context->token_to_scan = yylex();
  // set default value(scalar)
  context->set_dim(0);
  //context->set_symbol_modifier(BrigArray);
  if ('[' == context->token_to_scan) {
    if (ArrayDimensionSet(context)) {
      return 1;
    }
  }
  uint16_t naturalAlign = 16;

  if (context->get_alignment() == 0) {
    context->set_alignment(naturalAlign);
  } else if (context->get_alignment() < naturalAlign) {
    context->set_error(INVALID_ALIGNMENT);
    return 1;
  }
  BrigDirectiveImage bdi = {
    sizeof(BrigDirectiveImage), //size
    BrigEDirectiveImage,       //kind
    {
      context->get_code_offset(),      // c_code
      BrigGlobalSpace,                 // storag class
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
  context->global_symbol_map[var_name] = context->get_directive_offset();
  context->append_directive(&bdi);
  context->set_alignment(0);

  if ('=' == context->token_to_scan) {
    if (ImageInitializer(context)) {
      context->set_error(INVALID_IMAGE_INIT);
      return 1;
    }
  }

  if (';' != context->token_to_scan) {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  context->token_to_scan = yylex();
  return 0;
}

int ImageInitializer(Context *context) {
  // first must be '='
  if('=' != context->token_to_scan)
    return 1;

  context->token_to_scan = yylex();
  if ('{' != context->token_to_scan) {
    context->set_error(MISSING_OPENNING_BRACKET);
  }
  while (1) {
    context->token_to_scan = yylex();
    if (ImageInit(context)) {
      context->set_error(MISSING_IDENTIFIER);
      return 1;
    }
    if (',' != context->token_to_scan) {
      break;
    }
  }  // end for while

  if ('}' != context->token_to_scan) {
    context->set_error(MISSING_CLOSING_BRACKET);
    return 1;
  }
  context->token_to_scan = yylex();
  return 0;
}

int Ret(Context* context) {
  // first token is RET
  if(RET != context->token_to_scan) {
    return 1;
  }

  BrigOpcode32_t opcode = context->token_value.opcode ;

  context->token_to_scan = yylex();
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstBase op_ret = {
    sizeof(BrigInstBase),
    BrigEInstBase,
    opcode,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  // write to .code section
  context->append_code(&op_ret);

  context->token_to_scan = yylex();
  return 0;
}

int ImageRead(Context *context) {
  // first token is RD_IMAGE
  BrigDataType16_t type = Brigb32;
  BrigDataType16_t stype = Brigb32;
  BrigoOffset32_t OpOffset[4] = {0, 0, 0, 0};
  BrigGeom32_t geom = 0;

  context->token_to_scan = yylex();
  if (context->token_to_scan != _V4) {
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }
  context->token_to_scan = yylex();
  // Note: 1db is not supported
  if (context->token_type != GEOMETRY_ID) {
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }
  switch (context->token_to_scan) {
    case _1D: geom = Briggeom_1d; break;
    case _2D: geom = Briggeom_2d; break;
    case _3D: geom = Briggeom_3d; break;
    case _1DA: geom = Briggeom_1da; break;
    case _2DA: geom = Briggeom_2da; break;
    default:
      context->set_error(MISSING_DECLPREFIX);
      return 1;
  }

  context->token_to_scan = yylex();
  // Note: destLength: Destination length: 32 (f32, u32, or s32)
  if (context->token_to_scan != _F32 &&
      context->token_to_scan != _U32 &&
      context->token_to_scan != _S32) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }

  type = context->token_value.data_type;
  context->token_to_scan = yylex();
  // Note: srcLength: Source length: 32 srcType: f32 or u32.
  if (context->token_to_scan != _F32 &&
      context->token_to_scan != _U32) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  stype = context->token_value.data_type;
  context->token_to_scan = yylex();

  if (ArrayOperand(context, &OpOffset[0])) {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  // Note: dest: Destination. Must be a vector of four s registers.
  uint16_t kind;
  context->get_operand(OpOffset[0] + sizeof(uint16_t), &kind);
  if (kind != BrigEOperandRegV4) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  // TODO(Chuang): Whether the sampler("[Samp]") can be omitted.
  // rd_image used with integer coordinates has restrictions on the sampler:
  // coord must be unnormalized.
  // filter must be nearest.
  // The boundary mode must be clamp or border.

  if (context->token_to_scan != '[') {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  context->token_to_scan = yylex();
  if (AddressableOperand(context, &OpOffset[1], true)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  unsigned int opCount = 2;
  context->token_to_scan = yylex();
  if (context->token_to_scan != '[') {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  context->token_to_scan = yylex();
  if (AddressableOperand(context, &OpOffset[opCount++], true)) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  // TODO(Chuang): src: Register source for the coordinates.
  // A scalar for 1D images; a 2-element vector for 2D images;
  // a 4-element vector for 3D images, where the fourth element is ignored.
  // Each coordinate must be in an s register

  if (ArrayOperand(context, &OpOffset[opCount++])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }

  BrigInstImage imgRdInst = {
    0,                     // size
    BrigEInstImage,        // kind
    BrigRdImage,           // opcode
    type,                  // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0},       // o_operands[5]
    geom,                  // geom
    stype,                 // stype
    0                      // reserved
  };
  imgRdInst.size = sizeof(imgRdInst);
  for (int i = 0 ; i < 4 ; ++i) {
    imgRdInst.o_operands[i] = OpOffset[i];
  }
  context->append_code(&imgRdInst);
  context->token_to_scan = yylex();
  return 0;
}

int Sync(Context* context) {
  // first token is SYNC
  if(SYNC != context->token_to_scan)
    return 1;

  BrigOpcode32_t opcode = context->token_value.opcode;
  uint32_t syncFlags = BrigGroupLevel | BrigGlobalLevel;

  context->token_to_scan = yylex();
  if (context->token_to_scan == _GLOBAL) {
    syncFlags = BrigGlobalLevel;
    context->token_to_scan = yylex();
  } else if (context->token_to_scan == _GROUP) {
    syncFlags = BrigGroupLevel;
    context->token_to_scan = yylex();
  } else if(context->token_to_scan == _PARTIAL){
    syncFlags = BrigPartialLevel;
    context->token_to_scan = yylex();
  }

  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }

  BrigInstBar op_sync = {
    sizeof(BrigInstBar),
    BrigEInstBar,
    opcode ,
    Brigb32 ,
    BrigNoPacking,
    {0,0,0,0,0},
    syncFlags
  };
  context->append_code(&op_sync);
  context->token_to_scan = yylex();
  return 0;
}

int Bar(Context* context) {
  // first token is BARRIER
  if(BARRIER != context->token_to_scan)
    return 1;

  BrigOpcode32_t opcode = context->token_value.opcode;
  uint32_t syncFlags = BrigGlobalLevel | BrigGroupLevel; //default

  context->token_to_scan = yylex();
  BrigoOffset32_t offset = context->get_operand_offset();
  if (offset % 8) {
    offset += 8 - offset % 8;
  }

  if (context->token_to_scan == _WIDTH) {
    if (OptionalWidth(context)) {
      return 1;
    }
  } else {
   BrigOperandImmed op_width = {
      sizeof(BrigOperandImmed),
      BrigEOperandImmed,
      Brigb32,
      0,
      { 0 }
    };
    op_width.bits.u = 0;
    context->append_operand(&op_width);
  }

  if (context->token_to_scan == _GLOBAL) {
    syncFlags = BrigGlobalLevel;
    context->token_to_scan = yylex();
  } else if (context->token_to_scan == _GROUP) {
    syncFlags = BrigGroupLevel;
    context->token_to_scan = yylex();
  } else if(context->token_to_scan == _PARTIAL){
     syncFlags = BrigPartialLevel;
    context->token_to_scan = yylex();
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstBar op_bar = {
    sizeof(BrigInstBar),
    BrigEInstBar,
    opcode ,
    Brigb32 ,
    BrigNoPacking,
    {offset,0,0,0,0},
    syncFlags
  };
  context->append_code(&op_bar);
  context->token_to_scan = yylex();
  return 0;
}

int AtomicNoRet(Context* context) {
  // first token is ATOMICNORET or ATOMICNORET_CAS

  BrigMemorySemantic32_t MemorySemantic = BrigRegular;
  BrigAtomicOperation32_t atomicOperation = 0;
  BrigStorageClass32_t StorageClass = BrigFlatSpace;
  BrigDataType16_t DataType = Brigb32;
  BrigoOffset32_t OpOffset[3] = {0,0,0};

  const unsigned int first_token = context->token_to_scan;
  if ((first_token != ATOMICNORET) && (first_token != ATOMICNORET_CAS)) {
    return 1;
  }
  context->token_to_scan = yylex();
  if ((first_token == ATOMICNORET) && (context->token_type == ATOMIC_OP)) {
    switch (context->token_to_scan) {  // without _CAS_
      case _AND_: atomicOperation = BrigAtomicAnd; break;
      case _OR_: atomicOperation = BrigAtomicOr; break;
      case _XOR_: atomicOperation = BrigAtomicXor; break;
      case _ADD_: atomicOperation = BrigAtomicAdd; break;
      case _INC_: atomicOperation = BrigAtomicInc; break;
      case _DEC_: atomicOperation = BrigAtomicDec; break;
      case _MIN_: atomicOperation = BrigAtomicMin; break;
      case _MAX_: atomicOperation = BrigAtomicMax; break;
      case _SUB_: atomicOperation = BrigAtomicSub; break;
      default:
        context->set_error(MISSING_OPERATION);
        return 1;
    }
      context->token_to_scan = yylex();
  } else if(first_token==ATOMICNORET_CAS){
    atomicOperation = BrigAtomicCas;
  } else {
    context->set_error(MISSING_OPERATION);
    return 1;
  }

  if(!AddressSpaceIdentifier(context)){
    StorageClass = context->token_value.storage_class;
    context->set_storageClass(StorageClass);
    context->token_to_scan = yylex();
  }

  if (Optacqreg(context, &MemorySemantic)) {
    context->set_error(UNKNOWN_ERROR);
    return 1;
  }

  if (context->token_type != DATA_TYPE_ID){
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  DataType = context->token_value.data_type;
  context->token_to_scan = yylex();

  if (MemoryOperand(context, &OpOffset[0])) {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  unsigned int opCount = 1;

  if (first_token == ATOMICNORET_CAS) {
    if (context->token_to_scan != ',') {
      context->set_error(MISSING_COMMA);
      return 1;
    }
    context->token_to_scan = yylex();
    if (Operand(context, &OpOffset[opCount++])) {
      return 1;
    }
  }

  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (Operand(context, &OpOffset[opCount++])) {
    context->set_error(INVALID_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigInstAtomic aNoRetInst = {
    sizeof(BrigInstAtomic),// size
    BrigEInstAtomic,       // kind
    BrigAtomicNoRet,       // opcode
    DataType,                     // type
    BrigNoPacking,         // packing
    {OpOffset[0], OpOffset[1], OpOffset[2], 0, 0},       // o_operands[5]
    atomicOperation,         // atomicOperation
    StorageClass,       // storageClass
    MemorySemantic                      // memorySemantic
  };
  context->append_code(&aNoRetInst);
  context->token_to_scan = yylex();
  return 0;
}

int Location(Context* context) {
  // first token is LOC
  if (LOC != context->token_to_scan)
    return 1;

  context->token_to_scan = yylex();
  if (context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
    context->set_error(MISSING_INTEGER_CONSTANT);
    return 1;
  }
  uint32_t sourceFile = context->token_value.int_val;
  context->token_to_scan = yylex();
  if (context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
    context->set_error(MISSING_INTEGER_CONSTANT);
    return 1;
  }
  uint32_t sourceLine = context->token_value.int_val;
  context->token_to_scan = yylex();
  if (context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
    context->set_error(MISSING_INTEGER_CONSTANT);
    return 1;
  }
  uint32_t sourceColumn = context->token_value.int_val;
  context->token_to_scan = yylex();
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
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
}

int Control(Context* context) {
  BrigControlType32_t controlType;
  // values[0] = 1 if on,By default, memopt_on is enabled.
  uint32_t values[3] = {1,0,0};

  if (context->token_to_scan == MEMOPT_ON) {
    controlType = BrigEMemOpt;
    values[0] = 1;
  } else if (context->token_to_scan == MEMOPT_OFF) {
    controlType = BrigEMemOpt;
    values[0] = 0;
  } else if (context->token_to_scan == WORKGROUPS_PER_CU) {
    controlType = BrigEMaxGperC;
    context->token_to_scan = yylex();
    if (context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
      context->set_error(MISSING_INTEGER_CONSTANT);
      return 1;
    }
    values[0] = context->token_value.int_val;
  } else if (context->token_to_scan == ITEMS_PER_WORKGROUP) {
    controlType = BrigEMaxWIperG;
    context->token_to_scan = yylex();
    if (context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
      context->set_error(MISSING_INTEGER_CONSTANT);
      return 1;
    }
    values[0] = context->token_value.int_val;
    context->token_to_scan = yylex();
    if (context->token_to_scan != ',') {
      context->set_error(MISSING_COMMA);
      return 1;
    }
    context->token_to_scan = yylex();
    if (context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
      context->set_error(MISSING_INTEGER_CONSTANT);
      return 1;
    }
    values[1] = context->token_value.int_val;
    context->token_to_scan = yylex();
    if (context->token_to_scan != ',') {
      context->set_error(MISSING_COMMA);
      return 1;
    }
    context->token_to_scan = yylex();
    if (context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
      context->set_error(MISSING_INTEGER_CONSTANT);
      return 1;
    }
    values[2] = context->token_value.int_val;
  } else {
    context->set_error(INVALID_CONTROL_TYPE);
    return 1;
  }
  context->token_to_scan = yylex();
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  // codegen
  BrigDirectiveControl bdc = {
    sizeof(BrigDirectiveControl),   // size
    BrigEDirectiveControl,          // kind
    context->get_code_offset(),     // c_code
    controlType,                    // controlType
    {values[0],values[1],values[2]} // values
  };
  context->append_directive(&bdc);
  context->token_to_scan = yylex();
  return 0;
}

int Pragma(Context* context) {
  // first token is PRAGMA
  if(PRAGMA != context->token_to_scan)
    return 1;
  context->token_to_scan = yylex();
  if (context->token_to_scan != TOKEN_STRING) {
    context->set_error(MISSING_STRING);
    return 1;
  }
  std::string s_name = context->token_value.string_val;
  s_name.assign(s_name, 1, s_name.size() - 2);

  context->token_to_scan = yylex();
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  BrigDirectivePragma bdp = {
    sizeof(BrigDirectivePragma), // size
    BrigEDirectivePragma,        // kind
    context->get_code_offset(),  // c_code
    context->add_symbol(s_name)  // s_name
  };
  context->append_directive(&bdp);
  context->token_to_scan = yylex();
  return 0;
}

int LabelList(Context* context, BrigdOffset32_t dOffset, bool IsTargets) {
  uint32_t elementCount = 0;
  std::vector<BrigdOffset32_t> labDirOffset_list;
  std::vector<std::string> labName_list;

  while (context->token_to_scan == TOKEN_LABEL) {
    std::string label_name = context->token_value.string_val;

    labDirOffset_list.push_back(context->get_symbol(label_name));
    labName_list.push_back(label_name);

    elementCount++;
    context->token_to_scan = yylex();
    if (context->token_to_scan == ',') {
      context->token_to_scan = yylex();
      continue;
    } else {
      if (!IsTargets) {
        // update the BrigDirectiveSymbol.d_init and dim
        BrigDirectiveSymbol bds;
        context->get_directive(dOffset, &bds);

        if (bds.s.dim != 0) {
          if (elementCount > bds.s.dim) {
            context->set_error(INVALID_INITIALIZER);
            return 1;
          }
          elementCount = bds.s.dim;
        } else {
          if (context->get_isArray()) {
            bds.s.dim = elementCount;
          }
        }
        size_t arraySize = sizeof(BrigDirectiveLabelInit) +
          (elementCount - 1) * sizeof(BrigdOffset32_t);
        uint8_t *array = new uint8_t[arraySize];
        memset(array, 0, sizeof(uint8_t) * arraySize);
        BrigDirectiveLabelInit *bdli =
          reinterpret_cast<BrigDirectiveLabelInit*>(array);

        context->set_dim(bds.s.dim);
        if (0 != context->get_dim() && context->get_isArray()) {
          bds.s.symbolModifier = BrigArray;
        }

        // fill the data of BrigDirectiveLabelInit
        bdli->size = arraySize;
        bdli->kind = BrigEDirectiveLabelInit;
        bdli->c_code = context->get_code_offset();
        bdli->elementCount = elementCount;
        bdli->s_name = bds.s.s_name;

        bds.d_init = context->get_directive_offset();

        for (unsigned i = 0 ; i < labDirOffset_list.size() ; i ++){
          bdli->d_labels[i] = labDirOffset_list[i];
          // Save the address of d_label in directive section.
          if (bdli->d_labels[i] == 0) {
            context->label_d_map.insert(std::pair<std::string, BrigdOffset32_t>(
              labName_list[i], bds.d_init + arraySize - sizeof(BrigdOffset32_t) *
              (elementCount - i))
            );
          }
        }

        context->append_directive(bdli);

        unsigned char *bds_charp = reinterpret_cast<unsigned char*>(&bds);
        context->update_directive_bytes(
            bds_charp,
            dOffset,
            sizeof(BrigDirectiveSymbol)
        );

        delete[] reinterpret_cast<char *>(bdli);
      } else {
        BrigDirectiveLabelList* labelList;

        size_t arraySize = sizeof(BrigDirectiveLabelList) +
          (elementCount - 1) * sizeof(BrigdOffset32_t);
        uint8_t *array = new uint8_t[arraySize];
        memset(array, 0, sizeof(uint8_t) * arraySize);
        labelList = reinterpret_cast<BrigDirectiveLabelList*>(array);
        labelList->size = arraySize;
        labelList->kind = BrigEDirectiveLabelList;
        labelList->c_code = context->get_code_offset();
        labelList->label = dOffset;
        labelList->elementCount = elementCount;

        for (unsigned i = 0 ; i < labDirOffset_list.size() ; ++i){
          labelList->d_labels[i] = labDirOffset_list[i];
          // Save the address of d_label in directive section.
          if (labelList->d_labels[i] == 0) {
            context->label_d_map.insert(std::pair<std::string, BrigdOffset32_t>(
              labName_list[i],
              context->get_directive_offset() + arraySize -
              sizeof(BrigdOffset32_t) * (elementCount - i))
            );
          }
        }
        context->append_directive(labelList);
        delete []array;
      }
      return 0;
    }
  }  // While
  context->set_error(MISSING_LABEL);
  return 1;
}

int FloatInitializer(Context* context, BrigdOffset32_t symbol_offset){

  if(context->token_to_scan != TOKEN_DOUBLE_CONSTANT){
    return 1;
  }
  std::vector<double> float_list;
  if(FloatListSingle(context, &float_list))
    return 1;
  uint32_t elementCount = float_list.size();
  uint32_t n = 0;

  BrigDirectiveSymbol bds ;
  context->get_directive(symbol_offset, &bds);

  n = bds.s.dim;
  if (0 == context->get_dim()) {
    if (context->get_isArray()) {
      // If array is empty [],  Update dim.
      bds.s.symbolModifier = BrigArray;
      context->set_symbol_modifier(BrigArray);
      bds.s.dim = elementCount;
      n = elementCount;
    } else {
      if (elementCount != 1) {
        context->set_error(INVALID_INITIALIZER);
        return 1;
      }
      bds.s.dim = 0;
      n = 1;
    }
  } else if (elementCount > bds.s.dim) {
    context->set_error(INVALID_INITIALIZER);
    return 1;
  }
  elementCount = n;

  context->set_dim(bds.s.dim);
  if (context->get_type() != Brigb64) {
    context->set_error(INVALID_INITIALIZER);
    return 1;
  }
  size_t arraySize = sizeof(BrigDirectiveInit) + (n - 1) * sizeof(uint64_t);
  uint8_t *array = new uint8_t[arraySize];
  memset(array, 0, sizeof(uint8_t) * arraySize);

  BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit*>(array);
  bdi->elementCount = elementCount;

  bdi->size = arraySize;
  bdi->kind = BrigEDirectiveInit;
  bdi->c_code = context->get_code_offset();
  bdi->type = context->get_type();
  bdi->reserved = 0;
  for (uint32_t i = 0; i < float_list.size(); i++ ) {
    *(double*)&bdi->initializationData.u64[i] = float_list[i];
  }
  bds.d_init = context->get_directive_offset();
  if (bds.d_init % 8) {
    bds.d_init += 8 - bds.d_init % 8;
  }
  unsigned char *bds_charp = reinterpret_cast<unsigned char*>(&bds);
  context->update_directive_bytes(
    bds_charp, symbol_offset,
    sizeof(BrigDirectiveSymbol)
  );
  context->append_directive(bdi);
  delete[] reinterpret_cast<uint8_t *>(bdi);
  bdi = NULL;
  return 0;
}

int FloatListSingle(Context* context, std::vector <double>* float_list) {

  while (context->token_to_scan == TOKEN_DOUBLE_CONSTANT) {
    float_list->push_back(context->token_value.double_val);
    context->token_to_scan = yylex();
    if (context->token_to_scan != ',') {
      break;
    }
    context->token_to_scan = yylex();
    if(context->token_to_scan != TOKEN_DOUBLE_CONSTANT) {
      context->set_error(MISSING_DOUBLE_CONSTANT);
      return 1;
    }
  }
  return 0;
}

int DecimalListSingleBlock(Context* context) {

  std::vector<int64_t> decimal_list;
  if(DecimalListSingle(context, &decimal_list))
    return 1;
  uint32_t elementCount = decimal_list.size();
  uint32_t n = elementCount;
  switch (context->get_type()) {
    case Brigb1:
      context->set_error(INVALID_INITIALIZER);
      return 1;
    case Brigb8:    n = (n + 7) >> 3; break;  // (n+7)/8
    case Brigb16:   n = (n + 3) >> 2; break;  // (n+3)/4
    case Brigb32:   n = (n + 1) >> 1; break;  // (n+1)/2
    case Brigb64:   break;
  }
  size_t arraySize = sizeof(BrigBlockNumeric) + (n - 1) * sizeof(uint64_t);
  uint8_t *array = new uint8_t[arraySize];
  memset(array, 0, sizeof(uint8_t) * arraySize);
  BrigBlockNumeric *blockNumeric = reinterpret_cast<BrigBlockNumeric*>(array);

  switch(context->get_type()) {
    case Brigb8:
      for (uint32_t i = 0; i < decimal_list.size(); i++ ){
        blockNumeric->u8[i] = (uint8_t)decimal_list[i];
      }
      break;
    case Brigb16:
      for (uint32_t i = 0; i < decimal_list.size(); i++ ){
        blockNumeric->u16[i] = (uint16_t)decimal_list[i];
      }
      break;
    case Brigb32:
      for (uint32_t i = 0; i < decimal_list.size(); i++ ){
        *(int32_t*)&blockNumeric->u32[i] = (int32_t)decimal_list[i];
      }
      break;
    case Brigb64:
      // TODO(Chuang): Loss of precision
      for (uint32_t i = 0; i < decimal_list.size(); i++ ) {
        *(int64_t*)&blockNumeric->u64[i] = decimal_list[i];
      }
      break;
  }

  blockNumeric->size = arraySize;
  blockNumeric->kind = BrigEDirectiveBlockNumeric;
  blockNumeric->type = context->get_type();
  blockNumeric->elementCount = elementCount;

  context->append_directive(blockNumeric);

  delete []array;
  array = NULL;
  return 0;
}

int DecimalInitializer(Context* context, BrigdOffset32_t symbol_offset){

  std::vector<int64_t> decimal_list;
  if(DecimalListSingle(context, &decimal_list))
    return 1;
  uint32_t n = 0;
  uint32_t elementCount = decimal_list.size();

  BrigDirectiveSymbol bds ;
  context->get_directive(symbol_offset,&bds);

  n = bds.s.dim;
  if (0 == context->get_dim()) {
    if (context->get_isArray()) {
      // If array is empty [],  Update dim.
      bds.s.symbolModifier = BrigArray;
      context->set_symbol_modifier(BrigArray);
      bds.s.dim = elementCount;
      n = elementCount;
    } else {
      if (elementCount != 1) {
        context->set_error(INVALID_INITIALIZER);
        return 1;
      }
      bds.s.dim = 0;
      n = 1;
    }
  } else if (elementCount > bds.s.dim) {
    context->set_error(INVALID_INITIALIZER);
    return 1;
  }
  elementCount = n;
  context->set_dim(bds.s.dim);

  switch (context->get_type()) {
    case Brigb1:
      context->set_error(INVALID_INITIALIZER);
      return 1;
    case Brigb8:    n = (n + 7) >> 3; break;  // (n+7)/8
    case Brigb16:   n = (n + 3) >> 2; break;  // (n+3)/4
    case Brigb32:   n = (n + 1) >> 1; break;  // (n+1)/2
    case Brigb64:   break;
  }
  size_t arraySize = sizeof(BrigDirectiveInit) + (n - 1) * sizeof(uint64_t);
  uint8_t *array = new uint8_t[arraySize];
  memset(array, 0, sizeof(uint8_t) * arraySize);

  BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit*>(array);
  bdi->elementCount = elementCount;

  bdi->size = arraySize;
  bdi->kind = BrigEDirectiveInit;
  bdi->c_code = context->get_code_offset();
  bdi->type = context->get_type();
  bdi->reserved = 0;
  switch(context->get_type()) {
    case Brigb8:
      for (uint32_t i = 0; i < decimal_list.size(); i++ ){
        bdi->initializationData.u8[i] = (uint8_t)decimal_list[i];
      }
      break;
    case Brigb16:
      for (uint32_t i = 0; i < decimal_list.size(); i++ ){
        bdi->initializationData.u16[i] = (uint16_t)decimal_list[i];
      }
      break;
    case Brigb32:
      for (uint32_t i = 0; i < decimal_list.size(); i++ ){
        *(int32_t*)&bdi->initializationData.u32[i] = (int32_t)decimal_list[i];
      }
      break;
    case Brigb64:
      // TODO(Chuang): Loss of precision
      for (uint32_t i = 0; i < decimal_list.size(); i++ ) {
        *(int64_t*)&bdi->initializationData.u64[i] = decimal_list[i];
      }
      break;
  }
  bds.d_init = context->get_directive_offset();
  if (bds.d_init % 8) {
    bds.d_init += 8 - bds.d_init % 8;
  }
  unsigned char *bds_charp = reinterpret_cast<unsigned char*>(&bds);
  context->update_directive_bytes(bds_charp, symbol_offset,
                                  sizeof(BrigDirectiveSymbol));

  context->append_directive(bdi);
  delete[] reinterpret_cast<uint8_t *>(bdi);
  bdi = NULL;
  return 0;

}

int DecimalListSingle(Context* context, std::vector<int64_t>* decimal_list) {
  uint32_t elementCount = 0;
  while (1) {
    if (context->token_to_scan == '-') {
      context->token_to_scan = yylex();
      if (context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
        context->set_error(MISSING_INTEGER_CONSTANT);
        return 1;
      }
      context->token_value.int_val *= (-1);
    }
    if (context->token_to_scan == '+') {
      context->token_to_scan = yylex();
      if (context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
        context->set_error(MISSING_INTEGER_CONSTANT);
        return 1;
      }
    }

    if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
      elementCount++;
      decimal_list->push_back(context->token_value.int_val);
    }
    context->token_to_scan = yylex();
    if (context->token_to_scan != ',') {
      break;
    }
    context->token_to_scan = yylex();
  }
  return 0;
}

int Block(Context* context) {
  // first token is BLOCK
  if (BLOCK != context->token_to_scan)
    return 1;

  context->token_to_scan = yylex();
  if (context->token_to_scan != TOKEN_STRING) {
    context->set_error(MISSING_STRING);
    return 1;
  }
  std::string name = context->token_value.string_val;
  name.assign(name, 1, name.size() - 2);
  // block start
  BrigBlockStart bbs = {
    sizeof(BrigBlockStart),     // size
    BrigEDirectiveBlockStart,   // kind
    context->get_code_offset(), // c_code
    context->add_symbol(name)   // s_name
  };
  context->append_directive(&bbs);

  context->token_to_scan = yylex();
  if (context->token_to_scan == ENDBLOCK) {
    context->set_error(MISSING_SECTION_ITEM);
    return 1;
  }
  while (context->token_to_scan != ENDBLOCK) {
    if (context->token_to_scan == BLOCKSTRING) {
      context->token_to_scan = yylex();
      if (context->token_to_scan != TOKEN_STRING) {
        context->set_error(INVALID_SECTION_ITEM);
        return 1;
      }
      std::string str = context->token_value.string_val;
      str.assign(str, 1, str.size() - 2);
      // block string
      BrigBlockString bbs = {
	sizeof(BrigBlockString),
	BrigEDirectiveBlockString,
	context->add_symbol(str)
      };
      context->append_directive(&bbs);
      context->token_to_scan = yylex();
    } else if (context->token_to_scan == BLOCKNUMERIC) {
      context->token_to_scan = yylex();
      if (context->token_type != DATA_TYPE_ID) {
        context->set_error(MISSING_DATA_TYPE);
        return 1;
      }
      context->set_type(context->token_value.data_type);

      context->token_to_scan = yylex();
      if (context->token_to_scan == TOKEN_LABEL) {
        context->token_to_scan = yylex();
      } else if (context->token_to_scan == '-' ||
                 context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
        if (DecimalListSingleBlock(context)) {
          return 1;
        }
      } else {
        context->set_error(INVALID_SECTION_ITEM);
        return 1;
      }
    } else {  // Block Numeric or Block String
      context->set_error(MISSING_BLOCK_TYPE);
      return 1;
    }
    if (context->token_to_scan != ';') {
      context->set_error(MISSING_SEMICOLON);
      return 1;
    }
    context->token_to_scan = yylex();
  }  // While
  context->token_to_scan = yylex();
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
      // block end
  BrigBlockEnd bbe = {
    sizeof(BrigBlockEnd),
    BrigEDirectiveBlockEnd
  };
  context->append_directive(&bbe);

  context->token_to_scan = yylex();
  return 0;
}

int GlobalSymbolDeclpart2(Context* context) {
  if (context->token_to_scan == GROUP)
    return (GlobalGroupDecl(context));
  else if (context->token_to_scan == PRIVATE)
    return (GlobalPrivateDecl(context));
  return 1;
}

int GlobalSymbolDecl(Context* context) {
  if (DeclPrefix(context)) {
    return 1;
  }
  return GlobalSymbolDeclpart2(context);
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
  if(COORD != context->token_to_scan
    &&FILTER != context->token_to_scan
    &&BOUNDARYU != context->token_to_scan
    &&BOUNDARYV != context->token_to_scan
    &&BOUNDARYW != context->token_to_scan){
    return 1;
  }

  context->token_to_scan = yylex();
  if('=' != context->token_to_scan){
    context->set_error(MISSING_OPERATION);
    return 1;
  }
  context->token_to_scan = yylex();
  if(TOKEN_PROPERTY != context->token_to_scan){
    context->set_error(MISSING_PROPERTY);
    return 1;
  }

  BrigDirectiveSampler bds = { 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 0, 0,
                                0, 0, 0, 0, 0 };
  Context::context_error_t valid_dir = context->get_directive(&bds);
  bds.valid = 1;
  switch(first_token){
    case COORD: bds.normalized = context->token_value.normalized; break;
    case FILTER: bds.filter = context->token_value.filter; break;
    case BOUNDARYU: bds.boundaryU = context->token_value.boundary_mode; break;
    case BOUNDARYV: bds.boundaryV = context->token_value.boundary_mode; break;
    case BOUNDARYW: bds.boundaryW = context->token_value.boundary_mode; break;
  }
  unsigned char *bds_charp = reinterpret_cast<unsigned char*> (&bds);
  if(valid_dir == Context::CONTEXT_OK)
    context->update_last_directive(bds_charp, sizeof(bds));

  context->token_to_scan = yylex();
  return 0;

}

int SobInitializer(Context *context) {
  //first must be '='
  if ('=' != context->token_to_scan) {
     return 1;
  }

  context->token_to_scan = yylex();
  if ('{' != context->token_to_scan) {
    context->set_error(MISSING_OPENNING_BRACKET);
    return 1;
  }

  while(1) {
    context->token_to_scan = yylex();
    if(SobInit(context)){
      context->set_error(INVALID_INITIALIZER);
      return 1;
    }
    if (',' != context->token_to_scan)
      break ;
  }

  if ('}' != context->token_to_scan) {
    context->set_error(MISSING_CLOSING_BRACKET);
    return 1;
  }
  context->token_to_scan = yylex();
  return 0 ;

}

int GlobalSamplerDecl(Context *context) {
  if (GLOBAL != context->token_to_scan) {
    return 1;
  }
  context->token_to_scan = yylex();
  return (GlobalSamplerDeclPart2(context));
}

int GlobalSamplerDeclPart2(Context *context){
  // First token has already been verified as GLOBAL
  if (_SAMP != context->token_to_scan){
    return 1;
  }
  context->token_to_scan = yylex();
  if (TOKEN_GLOBAL_IDENTIFIER != context->token_to_scan){
     context->set_error(MISSING_IDENTIFIER);
     return 1;
  }
  std::string var_name(context->token_value.string_val);
  int var_name_offset = context->add_symbol(var_name);

  if (context->global_symbol_map.count(var_name)) {
    context->set_error(REPEATED_DECLARATION);
    return 1;
  }

  context->token_to_scan = yylex();

  context->set_dim(0);

  if ('[' == context->token_to_scan) {
    if (ArrayDimensionSet(context)) {
      context->set_error(INVALID_ARRAY_DIMENSIONS);
      return 1;
    }
  }

  uint16_t naturalAlign = 16;

  if (context->get_alignment() == 0) {
    context->set_alignment(naturalAlign);
  } else if (context->get_alignment() < naturalAlign) {
    context->set_error(INVALID_ALIGNMENT);
    return 1;
  }
  BrigDirectiveSampler bds = {
    sizeof(BrigDirectiveSampler),      //size
    BrigEDirectiveSampler,             //kind
    {
      context->get_code_offset(),      // c_code
      BrigGlobalSpace,                 // storag class
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
  context->global_symbol_map[var_name] = context->get_directive_offset();
  context->append_directive(&bds);
  context->set_alignment(0);

  if ('=' == context->token_to_scan) {
    if (SobInitializer(context)) {
      context->set_error(INVALID_IMAGE_INIT);
      return 1;
    }
  }

  if (';' != context->token_to_scan){
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  context->token_to_scan = yylex();
  return 0;
}

int GlobalInitializablePart2(Context* context) {
  if (GLOBAL == context->token_to_scan) {
    if (!InitializableDecl(context, &context->global_symbol_map)) {
      return 0;
    }

    switch(context->token_to_scan) {
      case _RWIMG:
        return(GlobalImageDeclPart2(context));
      case _ROIMG:
        return(GlobalReadOnlyImageDeclPart2(context));
      case _SAMP:
        return(GlobalSamplerDeclPart2(context));
      default:
        if (context->token_type != DATA_TYPE_ID) {
          context->set_error(MISSING_IDENTIFIER);
          return 1;
        }
    }
  } else if (READONLY == context->token_to_scan) {
    return InitializableDecl(context, &context->global_symbol_map);
  } else {
    return 1;
  }
  return 0;
}

int GlobalInitializable(Context* context){
  if (DeclPrefix(context)) {
    context->set_error(MISSING_DECLPREFIX);
    return 1;
  }
  return GlobalInitializablePart2(context);
}

int GlobalDeclpart2(Context *context){
  // the header is declPrefix
  if (FUNCTION == context->token_to_scan) {
    if (FunctionDefinitionPart2(context)) {
      return 1;
    }
    if (';' == context->token_to_scan) {
      return 0;
    }
  } else if (!GlobalInitializablePart2(context)) {
    return 0;
  } else if (!GlobalSymbolDeclpart2(context)) {
    return 0;
  }
  return 1;
}

int GlobalDecl(Context *context){
  if (SIGNATURE == context->token_to_scan) {
    // functionSignature
    return FunctionSignature(context);
  } else if (!DeclPrefix(context)) {
    return GlobalDeclpart2(context);
  }
  return 1;
}

int SequenceOfPrograms(Context *context) {
  while(VERSION == context->token_to_scan) {
    if(Program(context)) {
      context->set_error(INVALID_PROGRAM);
      return 1;
    }
  }
  return 0;
}

int PairAddressableOperand(Context* context) {
  // this judge(first token == '[') is necessary in here
  if (context->token_to_scan != '[') {
    context->set_error(MISSING_OPENNING_BRACKET);
    return 1;
  }
  int CurrentoOffset = 0;
  context->token_to_scan = yylex();
  CurrentoOffset = context->get_operand_offset();
  // AddressableOperand
  if ((context->token_to_scan != TOKEN_GLOBAL_IDENTIFIER) &&
      (context->token_to_scan != TOKEN_LOCAL_IDENTIFIER)) {
    context->set_error(MISSING_IDENTIFIER);
    return 1;
  }
  std::string name(context->token_value.string_val);
  context->token_to_scan = yylex();
  if (context->token_to_scan != ']') {
    context->set_error(MISSING_CLOSING_BRACKET);
    return 1;
  }

  BrigOperandAddress boa = {
    sizeof(boa),            // size
    BrigEOperandAddress,    // kind
    Brigb32,                // type
    0,                      // reserved
    0                       // directive
  };

  boa.directive = context->get_symbol(name);
  if (boa.directive == 0) {
    context->set_error(UNDEFINED_IDENTIFIER);
    return 1;
  }
  if (context->get_machine() == BrigELarge) {
    BrigSymbolCommon symbol;
    char* pSym = reinterpret_cast<char*>(&symbol);
    context->get_directive_bytes(
        pSym, boa.directive + sizeof(uint16_t) * 2, sizeof(symbol));
    switch (symbol.storageClass) {
      case BrigFlatSpace:
      case BrigGlobalSpace:
      case BrigReadonlySpace:
      case BrigKernargSpace:
        boa.type = Brigb64; break;
      default:
        boa.type = Brigb32;
    }
  }

  context->append_operand(&boa);
  context->token_to_scan = yylex();
  if (context->token_to_scan != '[') {
    context->set_error(MISSING_OPENNING_BRACKET);
    return 1;
  }
  BrigoOffset32_t retOpOffset;
  context->token_to_scan = yylex();
  if (OffsetAddressableOperandPart2(context, CurrentoOffset, &retOpOffset)) {
    // Global/Local Identifier with offsetAddressOperand.
    return 1;
  }
  return 0;
}

int TopLevelStatement(Context *context){

  context->initialize_statement_fields();
  if(TOKEN_COMMENT==context->token_to_scan){
    return Comment(context);
  } else if(!Directive(context)) {
    return 0 ;
  } else if(KERNEL == context->token_to_scan) {
    return Kernel(context);
  } else if(SIGNATURE == context->token_to_scan){
    return GlobalDecl(context) ;
  } else if ( (context->token_to_scan == ALIGN) ||
             (context->token_to_scan == CONST) ||
             (context->token_to_scan == EXTERN) ||
             (context->token_to_scan == STATIC) ) {
    if(DeclPrefix(context)){
      return 1;
    }
  }
  if (FUNCTION == context->token_to_scan) {
    if (FunctionDefinition(context)) {
      return 1;
    }
    if (';' == context->token_to_scan) {
      context->token_to_scan = yylex();
      context->local_symbol_map.clear();
      return 0;
    } else {
      if (Codeblock(context)){
        return 1;
      }
      return 0;
    }
  } else if (!GlobalInitializablePart2(context)){
    return 0;
  } else if (!GlobalSymbolDeclpart2(context)){
    return 0;
  }
  return 1;
}

int TopLevelStatements(Context *context){
  while (context->token_to_scan && (context->token_to_scan != VERSION)) {
    if (TopLevelStatement(context)) {
      return context->token_to_scan == VERSION ? 0 : 1;
    }
  }
  return 0;
}

int ArrayOperand(Context* context, BrigoOffset32_t* pRetOpOffset) {
  std::string op_name;
  if (context->token_to_scan == '(') {
    if (ArrayOperandList(context, pRetOpOffset)) {
      context->set_error(MISSING_CLOSING_PARENTHESIS);
      return 1;
    }
  } else {
    if (Operand(context, pRetOpOffset)) {
      context->set_error(MISSING_OPERAND);
      return 1;
    }
  }
  return 0;
}

int ArrayOperand(Context* context, BrigoOffset32_t* pRetOpOffset,
                 BrigDataType16_t expectedType) {
  std::string op_name;
  if (context->token_to_scan == '(') {
    if (ArrayOperandList(context, pRetOpOffset)) {
      context->set_error(MISSING_CLOSING_PARENTHESIS);
      return 1;
    }
  } else {
    if (Operand(context, pRetOpOffset, ConvertTypeToB(expectedType))) {
      context->set_error(MISSING_OPERAND);
      return 1;
    }
  }
  return 0;
}

int ArrayOperand(Context* context) {
  BrigoOffset32_t opOffset = 0;
  return ArrayOperand(context, &opOffset);
}

BrigDataType16_t ConvertTypeToB(BrigDataType16_t type,
    BrigDataType16_t* pSubType, uint32_t* pSize) {
  BrigDataType16_t bType = type;
  BrigDataType16_t subType = Brigb32;
  uint32_t size = 0;

  switch(type) {
    case Brigb1:      bType = Brigb1; subType = 0; size = 1; break;
    case Brigs8:
    case Brigu8:
    case Brigb8:      bType = Brigb8; subType = 0; size = 1; break;
    case Brigs16:
    case Brigu16:
    case Brigf16:
    case Brigb16:     bType = Brigb16; subType = 0; size = 1; break;
    case Brigs32:
    case Brigu32:
    case Brigf32:
    case Brigb32:     bType = Brigb32; subType = 0; size = 1; break;
    case Brigs64:
    case Brigu64:
    case Brigf64:
    case Brigb64:     bType = Brigb64; subType = 0; size = 1; break;
    case Brigb128:    bType = Brigb128; subType = 0; size = 1; break;
    case Brigu8x4:    bType = Brigb32;  subType = Brigb8;  size = 4;  break;
    case Brigs8x4:    bType = Brigb32;  subType = Brigb8;  size = 4;  break;
    case Brigu16x2:   bType = Brigb32;  subType = Brigb16; size = 2;  break;
    case Brigs16x2:   bType = Brigb32;  subType = Brigb16; size = 2;  break;
    case Brigf16x2:   bType = Brigb32;  subType = Brigb16; size = 2;  break;
    case Brigu8x8:    bType = Brigb64;  subType = Brigb8;  size = 8;  break;
    case Brigs8x8:    bType = Brigb64;  subType = Brigb8;  size = 8;  break;
    case Brigu16x4:   bType = Brigb64;  subType = Brigb16; size = 4;  break;
    case Brigs16x4:   bType = Brigb64;  subType = Brigb16; size = 4;  break;
    case Brigf16x4:   bType = Brigb64;  subType = Brigb16; size = 4;  break;
    case Brigu32x2:   bType = Brigb64;  subType = Brigb32; size = 2;  break;
    case Brigs32x2:   bType = Brigb64;  subType = Brigb32; size = 2;  break;
    case Brigf32x2:   bType = Brigb64;  subType = Brigb32; size = 2;  break;
    case Brigs8x16:   bType = Brigb128; subType = Brigb8;  size = 16; break;
    case Brigu8x16:   bType = Brigb128; subType = Brigb8;  size = 16; break;
    case Brigs16x8:   bType = Brigb128; subType = Brigb16; size = 8;  break;
    case Brigu16x8:   bType = Brigb128; subType = Brigb16; size = 8;  break;
    case Brigf16x8:   bType = Brigb128; subType = Brigb16; size = 8;  break;
    case Brigs32x4:   bType = Brigb128; subType = Brigb32; size = 4;  break;
    case Brigu32x4:   bType = Brigb128; subType = Brigb32; size = 4;  break;
    case Brigf32x4:   bType = Brigb128; subType = Brigb32; size = 4;  break;
    case Brigs64x2:   bType = Brigb128; subType = Brigb64; size = 2;  break;
    case Brigu64x2:   bType = Brigb128; subType = Brigb64; size = 2;  break;
    case Brigf64x2:   bType = Brigb128; subType = Brigb64; size = 2;  break;
  }
  if (pSubType != NULL) {
    *pSubType = subType;
    if (pSize != NULL) {
      *pSize = size;
    }
  }
  return bType;
}
bool CheckDataType(BrigMachine16_t model,BrigStorageClass32_t addressType,
    BrigDataType16_t type) {

  BrigDataType16_t btype = ConvertTypeToB(type);
  if (model == BrigESmall) {
    if (btype != Brigb32) {
      return false;
    }
  } else if (model == BrigELarge) {
    if (btype != Brigb32 && btype != Brigb64) {
      return false;
    }
    if (btype == Brigb32) {
      if (addressType != BrigGroupSpace &&
          addressType != BrigArgSpace &&
          addressType != BrigPrivateSpace &&
          addressType != BrigSpillSpace) {
        return false;
      }
    } else {
      if (addressType != BrigFlatSpace &&
          addressType != BrigGlobalSpace &&
          addressType != BrigReadonlySpace &&
          addressType != BrigKernargSpace) {
        return false;
      }
    }
  }
  return true;
}
bool CheckRegister(unsigned int token, BrigDataType16_t type) {
  BrigDataType16_t btype = ConvertTypeToB(type);
  //TODO(Chuang): How to handle F16 type.
  switch (btype) {
    case Brigb1:
      if (token != TOKEN_CREGISTER) {
        return false;
      }
      return true;
    case Brigb16:
    case Brigb32:
      if (token != TOKEN_SREGISTER) {
        return false;
      }
      return true;
    case Brigb64:
      if (token != TOKEN_DREGISTER) {
        return false;
      }
      return true;
    case Brigb128:
      if (token != TOKEN_QREGISTER) {
        return false;
      }
      return true;
    default:
      return false;
  }
  return false;
}
bool CheckDestRegister(unsigned int token, BrigDataType16_t type, BrigOpcode32_t opcode) {
  uint16_t kindOfType = 0x0;
  static const uint16_t _1Size = 0x0100;
  static const uint16_t _8Size = 0x0800;
  static const uint16_t _16Size = 0x1000;
  static const uint16_t _32Size = 0x2000;
  static const uint16_t _64Size = 0x4000;
  static const uint16_t _128Size = 0x8000;
  static const uint16_t uKind = 0x0001;
  static const uint16_t sKind = 0x0002;
  static const uint16_t fKind = 0x0004;
  static const uint16_t bKind = 0x0008;
  static const uint16_t packedKind = 0x0010;
  uint16_t regSize = 0x0;
  switch (token) {
    case TOKEN_CREGISTER: regSize = _1Size; break;
    case TOKEN_SREGISTER: regSize = _32Size; break;
    case TOKEN_DREGISTER: regSize = _64Size; break;
    case TOKEN_QREGISTER: regSize = _128Size; break;
    default: return false;
  }

  // When a destination operand is larger than the
  // operation size (for example, when an
  // add_u32 has a d (64-bit) destination register),
  // the result is extended to fill the destination
  // register. This does not apply to move operations
  // or to operations involving packed
  // data types or bit strings
  if (opcode == BrigPopCount || opcode == BrigFirstBit || opcode == BrigLastBit ||
      opcode == BrigF2u4) {
    return token == TOKEN_SREGISTER ? true : false;
  }
  if (opcode == BrigMov    || opcode == BrigMovsLo || opcode == BrigMovsHi ||
      opcode == BrigLda    || opcode == BrigMovdLo || opcode == BrigMovdHi ||
      opcode == BrigLdc    || opcode == BrigBitRev || opcode == BrigExtract ||
      opcode == BrigInsert || opcode == BrigBitSelect || opcode == BrigFirstBit ||
      opcode == BrigLastBit) {
    return CheckRegister(token, type);
  }
  switch (type) {
    case Brigb1:      kindOfType |= bKind;              kindOfType |= _1Size;  break;
    case Brigb8:      kindOfType |= bKind;              kindOfType |= _8Size;  break;
    case Brigb16:     kindOfType |= bKind;              kindOfType |= _16Size; break;
    case Brigb32:     kindOfType |= bKind;              kindOfType |= _32Size; break;
    case Brigb64:     kindOfType |= bKind;              kindOfType |= _64Size; break;
    case Brigb128:    kindOfType |= bKind;              kindOfType |= _128Size; break;
    case Brigs8:      kindOfType |= sKind;              kindOfType |= _8Size;  break;
    case Brigs16:     kindOfType |= sKind;              kindOfType |= _16Size; break;
    case Brigs32:     kindOfType |= sKind;              kindOfType |= _32Size; break;
    case Brigs8x4:    kindOfType |= sKind | packedKind; kindOfType |= _32Size; break;
    case Brigs16x2:   kindOfType |= sKind | packedKind; kindOfType |= _32Size; break;
    case Brigs64:     kindOfType |= sKind;              kindOfType |= _64Size; break;
    case Brigs8x8:    kindOfType |= sKind | packedKind; kindOfType |= _64Size; break;
    case Brigs16x4:   kindOfType |= sKind | packedKind; kindOfType |= _64Size; break;
    case Brigs32x2:   kindOfType |= sKind | packedKind; kindOfType |= _64Size; break;
    case Brigs8x16:   kindOfType |= sKind | packedKind; kindOfType |= _128Size; break;
    case Brigs16x8:   kindOfType |= sKind | packedKind; kindOfType |= _128Size; break;
    case Brigs32x4:   kindOfType |= sKind | packedKind; kindOfType |= _128Size; break;
    case Brigs64x2:   kindOfType |= sKind | packedKind; kindOfType |= _128Size; break;
    case Brigu8:      kindOfType |= uKind;              kindOfType |= _8Size;  break;
    case Brigu16:     kindOfType |= uKind;              kindOfType |= _16Size; break;
    case Brigu32:     kindOfType |= uKind;              kindOfType |= _32Size; break;
    case Brigu8x4:    kindOfType |= uKind | packedKind; kindOfType |= _32Size; break;
    case Brigu16x2:   kindOfType |= uKind | packedKind; kindOfType |= _32Size; break;
    case Brigu64:     kindOfType |= uKind;              kindOfType |= _64Size; break;
    case Brigu8x8:    kindOfType |= uKind | packedKind; kindOfType |= _64Size; break;
    case Brigu16x4:   kindOfType |= uKind | packedKind; kindOfType |= _64Size; break;
    case Brigu32x2:   kindOfType |= uKind | packedKind; kindOfType |= _64Size; break;
    case Brigu8x16:   kindOfType |= uKind | packedKind; kindOfType |= _128Size; break;
    case Brigu16x8:   kindOfType |= uKind | packedKind; kindOfType |= _128Size; break;
    case Brigu32x4:   kindOfType |= uKind | packedKind; kindOfType |= _128Size; break;
    case Brigu64x2:   kindOfType |= uKind | packedKind; kindOfType |= _128Size; break;
    case Brigf16:     kindOfType |= fKind;              kindOfType |= _16Size; break;
    case Brigf32:     kindOfType |= fKind;              kindOfType |= _32Size; break;
    case Brigf64:     kindOfType |= fKind;              kindOfType |= _64Size; break;
    case Brigf16x2:   kindOfType |= fKind | packedKind; kindOfType |= _32Size; break;
    case Brigf16x4:   kindOfType |= fKind | packedKind; kindOfType |= _64Size; break;
    case Brigf32x2:   kindOfType |= fKind | packedKind; kindOfType |= _64Size; break;
    case Brigf16x8:   kindOfType |= fKind | packedKind; kindOfType |= _128Size; break;
    case Brigf32x4:   kindOfType |= fKind | packedKind; kindOfType |= _128Size; break;
    case Brigf64x2:   kindOfType |= fKind | packedKind; kindOfType |= _128Size; break;
  }
  if (opcode == BrigShuffle || opcode == BrigUnpackLo || opcode == BrigUnpackHi) {
    if (!(kindOfType & packedKind) || (kindOfType & 0xff00) >= _128Size) {
      return false;
    }
  }

  if (kindOfType & packedKind || kindOfType & fKind) {
    return CheckRegister(token, type);
  }

  return ((kindOfType & 0xff00) > regSize ? false : true);
}

}  // namespace brig
}  // namespace hsa
