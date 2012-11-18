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
  if (context->token_to_scan != TOKEN_SREGISTER) {
    context->set_error(MISSING_OPERAND);
    return 1;
  }
  std::string opName = context->token_value.string_val;
  if (Identifier(context)) {
    return 1;
  }
  OpOffset[0] = context->operand_map[opName];
  context->token_to_scan = yylex();
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

int Operand(Context* context, BrigoOffset32_t* pRetOpOffset) {
  BrigoOffset32_t current_offset;
  std::string opName;
  current_offset = context->get_operand_offset();

  if ((context->token_type == REGISTER) || (context->token_to_scan == TOKEN_WAVESIZE)) {
    opName = context->token_value.string_val;
  } else if (context->token_type == CONSTANT) {
    current_offset += current_offset & 0x7;
  }

  if (!Identifier(context)) {
    *pRetOpOffset = (current_offset == context->get_operand_offset()) ? context->operand_map[opName] : current_offset;
    context->token_to_scan = yylex();
    return 0;
  } else if(!BaseOperand(context)){
    *pRetOpOffset = (current_offset == context->get_operand_offset()) ? context->operand_map[opName] : current_offset;
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
  
  BrigDataType16_t type = context->get_type();
  switch(type){
    case Brigb1: type = Brigb1; break; 
    case Brigs8:
    case Brigu8:
    case Brigb8: type = Brigb8; break;
    case Brigs16:
    case Brigu16:
    case Brigf16:
    case Brigb16: type = Brigb16; break;
    case Brigs32:
    case Brigu32:
    case Brigf32:
    case Brigb32: 
    case Brigu8x4:
    case Brigs8x4:
    case Brigu16x2:
    case Brigs16x2:
    case Brigf16x2: type = Brigb32; break;
    case Brigs64:
    case Brigu64:
    case Brigf64:
    case Brigb64: 
    case Brigu8x8:
    case Brigs8x8:
    case Brigu16x4:
    case Brigs16x4:
    case Brigf16x4:
    case Brigu32x2:
    case Brigs32x2:
    case Brigf32x2: type = Brigb64; break;
    case Brigb128:
    case Brigs8x16:
    case Brigu8x16:
    case Brigs16x8:
    case Brigu16x8:
    case Brigf16x8:
    case Brigs32x4:
    case Brigu32x4:
    case Brigf32x4:
    case Brigs64x2:
    case Brigu64x2:
    case Brigf64x2: type = Brigb128; break;    
  }
  
  if (context->token_to_scan == TOKEN_DOUBLE_CONSTANT) {
    BrigOperandImmed boi = {
      sizeof(boi),        // size
      BrigEOperandImmed,  // kind
      Brigb64,            // type
      0,                  // reserved
      { 0 }
    };
    boi.type = type;
    boi.bits.l[0] = boi.bits.l[1] = 0;
    boi.bits.d = context->token_value.double_val;
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
    boi.bits.f = context->token_value.float_val;
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
    boi.bits.u = context->token_value.int_val;
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

  if (context->token_to_scan == '-') {
    context->token_to_scan = yylex();
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
      boi.bits.u = -context->token_value.int_val;
      context->append_operand(&boi);

     return 0;
    }
  }
  
//TODO: Deepthi - This part needs to be tested - is there any instruction which uses datatypeid list as an operand  
#if 0
  if (context->token_type == DATA_TYPE_ID) {
    context->token_to_scan = yylex();
    if (context->token_to_scan == '(') {
      context->token_to_scan = yylex();
      if(!DecimalListSingle(context)){
        context->token_to_scan = yylex();
        return 0;
      }  
      else if(!FloatListSingle(context)){
        context->token_to_scan = yylex();
        return 0;
      }        
    } else
      return 1;
  }
#endif  
  return 1;
}

int AddressableOperand(Context* context) {
  BrigoOffset32_t opOffset = 0;
  return AddressableOperand(context, &opOffset, true);
}

int AddressableOperand(Context* context, BrigoOffset32_t* pRetOpOffset, bool IsImageOrSampler){
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
  /*if (!context->symbol_map.count(name)) {
      context->set_error(MISSING_OPERAND);
       return 1;
    }*/
  directive = context->symbol_map[name];
  context->token_to_scan = yylex();

  // check <..>
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

    if (context->get_machine() == BrigELarge) {
      boa.type = Brigb64;
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
  std::string iden_name;

  // first token must be '('
  if ('(' != context->token_to_scan) 
    return 1;

  while (1) {
    context->token_to_scan = yylex();
    if (count_op > 3){
        context->set_error(INVALID_OPERAND);
        return 1;
    }
    iden_name = context->token_value.string_val;
    if (Identifier(context)) {
      context->set_error(MISSING_IDENTIFIER);
      return 1;
    }    
    offsets[count_op] = context->operand_map[iden_name];
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
      context->get_operand_bytes((char *)(&elements[0]), offsets[0], sizeof(BrigOperandReg));
      context->get_operand_bytes((char *)(&elements[1]), offsets[1], sizeof(BrigOperandReg));
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
      context->get_operand_bytes((char *)(&elements[0]), offsets[0], sizeof(BrigOperandReg));
      context->get_operand_bytes((char *)(&elements[1]), offsets[1], sizeof(BrigOperandReg));
      context->get_operand_bytes((char *)(&elements[2]), offsets[2], sizeof(BrigOperandReg));
      context->get_operand_bytes((char *)(&elements[3]), offsets[3], sizeof(BrigOperandReg));
      if((elements[0].type != elements[1].type) && (elements[3].type!=elements[4].type) 
        && (elements[0].type != elements[3].type)){
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
  // TODO(Chuang): the max size is 256
  BrigoOffset32_t arg_offset[256] = {0};
  int nElements = 0;
  std::string funcName;

  if (context->token_to_scan != '[') {
    if (context->token_to_scan == TOKEN_GLOBAL_IDENTIFIER) {
      funcName = context->token_value.string_val;

      BrigOperandArgumentRef opArgRef = {
        sizeof(BrigOperandArgumentRef),
        BrigEOperandArgumentRef,
        0
      };
      // TODO(Chuang): judge whether the identifier has been defined.
      // and which Map will the offset of directive about signature func be saved into?
      opArgRef.arg = context->func_map[funcName];
      context->arg_map[funcName] = context->get_operand_offset();
      context->append_operand(&opArgRef);

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
        arg_offset[nElements++] = context->func_o_map[funcName];
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
        arg_offset[nElements++] = context->func_o_map[funcName];
      }
      context->token_to_scan = yylex();
      if (context->token_to_scan == ']') {
      size_t list_size = sizeof(BrigOperandArgumentList);
      if (nElements > 1) {
        list_size += sizeof(BrigoOffset32_t) * (nElements - 1);
      } else if (nElements == 0) {
        // there is one identifier at least.
        context->set_error(MISSING_IDENTIFIER);
        return 1;
      }

      char *array = new char[list_size];
      BrigOperandArgumentList &funcList =
        *reinterpret_cast<BrigOperandArgumentList *>(array);
        funcList.size = list_size;
        funcList.kind = BrigEOperandFunctionList;
        funcList.elementCount = nElements;
        for (int32_t i = 0; i < nElements; ++i) {
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
      if (n_elements > 1) {
        list_size += sizeof(BrigoOffset32_t) * (n_elements - 1);
      }

      char *array = new char[list_size];
      BrigOperandArgumentList &arg_list =
        *reinterpret_cast<BrigOperandArgumentList *>(array);
      arg_list.size = list_size;
      arg_list.kind = BrigEOperandArgumentList;
      arg_list.elementCount = n_elements;
      for (int32_t i = 0; i < n_elements; ++i) {
        arg_list.o_args[i] = arg_offset[i];
      }
      if (!n_elements) {
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
      // TODO(Chuang): judge whether the identifier has been defined.
      opArgRef.arg = context->symbol_map[arg_name];
      context->arg_map[arg_name] = context->get_operand_offset();
      context->append_operand(&opArgRef);

      arg_offset[n_elements] = context->arg_map[arg_name];
      n_elements++;
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
  BrigAluModifier mod = {0, 0, 0, 0, 0, 0, 0};//context->get_alu_modifier();

  if (first_token == _FTZ) {
    mod.valid = 1;
    mod.ftz = 1;
    mod.floatOrInt = 1;
    context->token_to_scan = yylex();
  }

  if (context->token_type == FLOAT_ROUNDING) {
      // next is floatRounding
    mod.valid = 1;
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
    context->set_alu_modifier(mod);
    return 0;
  } else if (context->token_type == INT_ROUNDING) {
    mod.valid = 1;
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
  }
  if(mod.ftz){
    context->set_alu_modifier(mod);
    return 0;
  }
  return 1;
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
  if (!RoundingMode(context)) {
    aluModifier = context->get_alu_modifier();
  }
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
    context->set_error(INVALID_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  inst.type = context->token_value.data_type;
  context->token_to_scan = yylex();
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
  int* aluValue = reinterpret_cast<int*>(&aluModifier);
  if (*aluValue != 0) {
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
  if (!RoundingMode(context)) {
    aluModifier = context->get_alu_modifier();
  }
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
  int* aluValue = reinterpret_cast<int*>(&aluModifier);
  if (*aluValue != 0) {
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
  if (!RoundingMode(context)) {
      aluModifier = context->get_alu_modifier();
  }

  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(INVALID_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);
  type = context->token_value.data_type;
  context->token_to_scan = yylex();

  if (context->token_type != REGISTER) {
    context->set_error(INVALID_FIRST_OPERAND);
    return 1;
  }
  if (Operand(context, &OpOffset0)) {
    context->set_error(INVALID_FIRST_OPERAND);
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
  int* aluValue = reinterpret_cast<int*>(&aluModifier);
  if (*aluValue != 0) {
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
  bool has_modifier = false;
  BrigDataType16_t type = Brigb32;
  BrigOpcode32_t opcode = context->token_value.opcode;
  BrigPacking packing = BrigNoPacking;
  BrigoOffset32_t OpOffset0 = 0, OpOffset1 = 0, OpOffset2 = 0;

  /* Checking for Instruction3 statement  - no error set here*/
  if ((context->token_type != INSTRUCTION3_OPCODE)
    && (context->token_type != INSTRUCTION3_OPCODE_FTZ)) {
    return 1;
  }

  context->token_to_scan= yylex();

  /* Rounding Optional*/
  has_modifier = (!RoundingMode(context));

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
  if((opcode==BrigShr) || (opcode==BrigShl))
    packing = type >= Brigu8x4 ? BrigPackPS : packing;
    
  if (Operand(context, &OpOffset0)) {
    context->set_error(MISSING_OPERAND);
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
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (Operand(context, &OpOffset2)) {
    context->set_error(MISSING_OPERAND);
    return 1;
  }

  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  if (has_modifier){
    BrigAluModifier mod = context->get_alu_modifier();
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
  }else {
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
  bdv.ftz = BrigENosftz;

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
  context->token_to_scan = yylex();

  if (':' == context->token_to_scan) {
    context->token_to_scan = yylex();
    while ( ';' != context->token_to_scan) {
      if (TARGET != context->token_type) {
        context->set_error(INVALID_TARGET);
        return 1;
      }
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
  return 0;   
}

int Alignment(Context* context) {
  // first token must be "align" keyword
  if ( ALIGN != context->token_to_scan)
    return 1;

  context->token_to_scan = yylex();
  if (TOKEN_INTEGER_CONSTANT != context->token_to_scan){
    context->set_error(MISSING_INTEGER_CONSTANT);
    return 1;
  }

  context->set_alignment(context->token_value.int_val);
  context->token_to_scan = yylex();
  return 0;
}

int DeclPrefix(Context* context){
  context->set_attribute(BrigNone);
  context->init_symbol_modifier();

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
        if (context->token_to_scan==EXTERN)
          context->set_attribute(BrigExtern);
        else if (context->token_to_scan == STATIC)
          context->set_attribute(BrigStatic);
        context->token_to_scan = yylex();
        break;
    }
  } else 
    return 0;

  if ((context->token_to_scan == ALIGN) ||
     (context->token_to_scan == EXTERN) ||
     (context->token_to_scan == STATIC) ||
     (context->token_to_scan==CONST)) {
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
        if (context->token_to_scan == EXTERN)
          context->set_attribute(BrigExtern);
        else if (context->token_to_scan == STATIC)
          context->set_attribute(BrigStatic);
        context->token_to_scan = yylex();
        break;
    }
  } else
    return 0;
  if ((context->token_to_scan == ALIGN) ||
     (context->token_to_scan == EXTERN) ||
     (context->token_to_scan == STATIC) ||
     (context->token_to_scan==CONST)) {
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
        if (context->token_to_scan==EXTERN)
          context->set_attribute(BrigExtern);
        else if (context->token_to_scan == STATIC)
          context->set_attribute(BrigStatic);
      context->token_to_scan = yylex();
      break;
    }
  }
  return 0;
}

int ArrayDimensionSet(Context* context) {
  uint32_t dim = 1;
  bool have_size = false;
  context->set_isArray(true);

  // first token must be '['
  if ('[' != context->token_to_scan) {
    return 1;
  }
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
  
  if (!have_size){
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
  context->init_symbol_modifier();
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
  int arg_name_offset = context->add_symbol(arg_name);

  // scan for arrayDimensions
  context->token_to_scan = yylex();

  if (context->token_to_scan == '[') {
    if (ArrayDimensionSet(context)) {
      // context->token_to_scan has been set in ArrayDimensionSet()
      return 1;
    }
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
      type,              // data type
      context->get_alignment(),         // alignment
    },
    0,                                // d_init = 0 for arg
    0                                 // reserved
  };
  
  // append the DirectiveSymbol to .directive section.
  context->append_directive(&sym_decl);
  //Mapping symbol names to declarations in .dir
  context->symbol_map[arg_name] = dsize;
        
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
  if ((width_param <= 1024) && ((width_param & 0x01) == 0))
    op_width.bits.u  = width_param ;
  context->append_operand(&op_width);
  context->token_to_scan = yylex();
  return 0;
}

int BranchCbr(Context* context) {

  BrigAluModifier mod = {0, 0, 0, 0, 0, 0, 0};
  BrigoOffset32_t OpOffset[4] = {0,0,0,0};

  BrigoOffset32_t widthOffset = context->get_operand_offset();
  widthOffset += widthOffset & 0x7;

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
      if (context->symbol_map.count(label_name)) {
        opLabelRef.labeldirective = context->symbol_map[label_name];
      }
      context->append_operand(&opLabelRef);
    }
    OpOffset[2] = context->label_o_map[label_name];
    context->token_to_scan = yylex();  // should be ';'
  } else if (context->token_to_scan == TOKEN_SREGISTER) {
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
          if (context->symbol_map.count(label_name)) {
            opLabelRef.labeldirective = context->symbol_map[label_name];
          }
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

        boa.directive = context->symbol_map[idenName];
  
        if (context->get_machine() == BrigELarge) {
          boa.type = Brigb64;
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
  } else {  // S register or Label
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
  widthOffset += widthOffset & 0x7;

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
      if (context->symbol_map.count(label_name)) {
        opLabelRef.labeldirective = context->symbol_map[label_name];
      }
      context->append_operand(&opLabelRef);
    }
    OpOffset[1] = context->label_o_map[label_name];
    context->token_to_scan = yylex();
  } else if (!Identifier(context)) {
    // Must be an s register.
    if (context->token_to_scan != TOKEN_SREGISTER) {
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
          if (context->symbol_map.count(label_name)) {
            opLabelRef.labeldirective = context->symbol_map[label_name];
          }
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

        boa.directive = context->symbol_map[idenName];
  
        if (context->get_machine() == BrigELarge) {
          boa.type = Brigb64;
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
  widthOffset += widthOffset & 0x7;
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
    op_width.bits.u = 0;
    context->append_operand(&op_width);
  }
  
  if (context->token_to_scan == __FBAR) {
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
  } else if (firstOpToken == TOKEN_SREGISTER) {
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
  OpOffset[3] = secondOpOffset;

  // if there is CallTarget, the first operand token must be a s register.
  if (firstOpToken == TOKEN_SREGISTER) {
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
  
  if (*paluModifier!=0) {
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
    default:
      context->set_error(INVALID_INITIALIZER);
      return 1;
  }
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
      if (LabelList(context, sym_offset)) {
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
  if ((context->token_to_scan != GLOBAL)
     && (context->token_to_scan != READONLY))
    return 1;

  BrigStorageClass32_t storage_class = context->token_value.storage_class;
  context->token_to_scan = yylex();

  return (InitializableDecl(context, storage_class));

}

int InitializableDecl(Context *context, BrigStorageClass32_t storage_class){  
  int var_name_offset;
  context->init_symbol_modifier();
  context->set_isArray(false);
  //First token already verified as GLOBAL/READONLY
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

  context->token_to_scan = yylex();
  // set default value(scalar)
  context->set_dim(0);
  if (context->token_to_scan == '[') {
    if (ArrayDimensionSet(context)) {
      return 1;
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
  context->symbol_map[var_name] = context->get_directive_offset();
  BrigdOffset32_t argdecl_offset = context->get_directive_offset();
  context->append_directive(&sym_decl);

  if(context->token_to_scan == '='){
    if (Initializer(context, argdecl_offset)) {
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

  BrigStorageClass32_t storage_class;
  BrigsOffset32_t str_offset;
  // first_token is PRIVATE, GROUP or SPILL
  if ((PRIVATE != context->token_to_scan)
     &&(GROUP != context->token_to_scan)
     &&(SPILL != context->token_to_scan))
    return 1;

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
  str_offset = context->add_symbol(var_name);

  context->token_to_scan = yylex();
  context->set_dim(0);
  context->init_symbol_modifier();

  if ('[' == context->token_to_scan) {
    if (ArrayDimensionSet(context))
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

  context->symbol_map[var_name] = context->get_directive_offset();
  context->append_directive(&bds);
 
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

  if (Identifier(context)) {
    context->set_error(MISSING_IDENTIFIER);
    return 1;
  }

  std::string var_name = context->token_value.string_val;
  BrigsOffset32_t str_offset = context->add_symbol(var_name);

  context->token_to_scan = yylex();
  context->set_dim(0);
  context->init_symbol_modifier();

  if ('[' == context->token_to_scan) {
    if (ArrayDimensionSet(context))
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

  context->symbol_map[var_name] = context->get_directive_offset();
  context->append_directive(&bds);
/*
  BrigOperandArgumentRef boar = {
    sizeof(BrigOperandArgumentRef),
    BrigEOperandArgumentRef,
    context->get_directive_offset()
  };

  context->arg_map[var_name] = context->get_operand_offset();
  context->append_operand(&boar);
*/
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

int SignatureType(Context *context, std::vector<BrigDirectiveSignature::BrigProtoType> *types) {
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

  BrigDirectiveSignature::BrigProtoType bpt = {
    context->get_type(),         // type
    context->get_alignment(),    // align
    (context->get_dim() != 0),   // hasDim
    context->get_dim()           // dim
  };
  types->push_back(bpt);

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
  if (context->token_to_scan != TOKEN_INTEGER_CONSTANT) {
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
       (context->token_to_scan != TOKEN_INTEGER_CONSTANT))) {
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
       (context->token_to_scan != TOKEN_INTEGER_CONSTANT))) {
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
       (context->token_to_scan != TOKEN_INTEGER_CONSTANT))) {
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

int SignatureArgumentList(Context *context, std::vector<BrigDirectiveSignature::BrigProtoType> *types) {
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
  context->append_directive(bds);

  delete[] reinterpret_cast<char *>(bds);
  types.clear();

  context->token_to_scan = yylex();
  return 0;
}

int Label(Context* context) {
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

  if (context->symbol_map.count(name)) {
   // TODO if count isnot zeor,
   // it's wrong program,emit error report
   // context->set_error(INVALID_LABEL);
   // return 1;
    context->token_to_scan = yylex();
    return 0;
  }
  c_code = context->get_code_offset();

  // dont need to check if name exist in symbol buffer 
  s_name = context->get_string_offset();
  context->add_symbol(name);

  if (context->label_o_map.count(name)) {
    BrigoOffset32_t ope_offset = context->label_o_map[name];
    BrigdOffset32_t lab_dir_offset = context->get_directive_offset();
    unsigned char* lab_d_Offset = reinterpret_cast<unsigned char*>(&lab_dir_offset);
    context->update_operand_bytes(lab_d_Offset,
                         ope_offset + sizeof(uint16_t) * 2,
                         sizeof(BrigdOffset32_t));
  }
  BrigDirectiveLabel label_directive = {
    sizeof(BrigDirectiveLabel),    // size
    BrigEDirectiveLabel,           // kind
    c_code,                        // c_code
    s_name                         // s_name
  };

  context->symbol_map[name] = context->get_directive_offset();
  context->append_directive(&label_directive);

  context->token_to_scan = yylex();
  return 0;
}

int LabelTargets(Context* context) {
  if (Label(context)) {
    context->set_error(MISSING_LABEL);
    return 1;
  }
  if (context->token_to_scan == LABELTARGETS) {
    return LabelTargetsPart2(context);
  } else {
    context->set_error(INVALID_LABEL_TARGETS);
    return 1;
  }
}

int LabelTargetsPart2(Context* context){
  while (1) {
    context->token_to_scan = yylex();
    if (context->token_to_scan != TOKEN_LABEL) {
      context->set_error(MISSING_LABEL);
      return 1;
    }

    context->token_to_scan = yylex();
    if (context->token_to_scan == ',') {
      continue;
    } else if (context->token_to_scan == ';') {
      context->token_to_scan = yylex();
      return 0;
    } else {
      context->set_error(MISSING_SEMICOLON);
      return 1;
    }
  }
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
      mmoInst.opcode = BrigSad4Hi;
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

    context->set_type(context->token_value.data_type);
    mmoInst.type = context->token_value.data_type;
    context->token_to_scan = yylex();

    // TODO(Chuang): dest:  The destination is an f32.

    if (context->token_type != REGISTER) {
      context->set_error(INVALID_FIRST_OPERAND);
      return 1;
    }

    if (!Operand(context, &mmoInst.o_operands[0])) {
      if (context->token_to_scan != ',') {
        context->set_error(MISSING_COMMA);
        return 1;
      }
      context->token_to_scan = yylex();
      // TODO(Chuang):src0, src1, src2, src3: Sources. All are b32

      if (!Operand(context, &mmoInst.o_operands[1])) {
        if (context->token_to_scan != ',') {
          context->set_error(MISSING_COMMA);
          return 1;
        }
        context->token_to_scan = yylex();

        if (!Operand(context, &mmoInst.o_operands[2])) {
          if (context->token_to_scan != ',') {
            context->set_error(MISSING_COMMA);
            return 1;
          }
          context->token_to_scan = yylex();

          if (!Operand(context, &mmoInst.o_operands[3])) {
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

int Instruction4Fma(Context* context) {
  BrigInstBase fmaInst = {
    sizeof(BrigInstBase),   // size
    BrigEInstBase,         // kind
    BrigFma,               // opcode
    0,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}       // o_operands[5]
  };

  BrigAluModifier aluModifier = {0, 0, 0, 0, 0, 0, 0};

  context->token_to_scan = yylex();
  if (!RoundingMode(context)) {
    aluModifier = context->get_alu_modifier();
  }
  // TODO(Chuang):Type Length: 16 (in some implementations), 32, 64
  if (context->token_to_scan == _F32 ||
      context->token_to_scan == _F64) {
    context->set_type(context->token_value.data_type);
    fmaInst.type = context->token_value.data_type;
    context->token_to_scan = yylex();

    // Note: dest: Destination register.

    if (context->token_type != REGISTER) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }

    if (!Operand(context, &fmaInst.o_operands[0])) {
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

      if (!Operand(context, &fmaInst.o_operands[1])) {
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

        if (!Operand(context, &fmaInst.o_operands[2])) {
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

          if (!Operand(context, &fmaInst.o_operands[3])) {

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
                  {0, 0, 0, 0, 0, 0, 0}  // aluModifier
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

int Instruction4Mad(Context* context) {
  BrigInstBase madInst = {
    sizeof(BrigInstBase),   // size
    BrigEInstBase,         // kind
    BrigMad,               // opcode
    0,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}       // o_operands[5]
  };

  BrigAluModifier aluModifier = {0, 0, 0, 0, 0, 0, 0};
  context->token_to_scan = yylex();
  if (!RoundingMode(context)) {
    aluModifier = context->get_alu_modifier();
  }
  if (context->token_to_scan == _F32 ||
      context->token_to_scan == _F64 ||
      context->token_to_scan == _U32 ||
      context->token_to_scan == _U64 ||
      context->token_to_scan == _S32 ||
      context->token_to_scan == _S64) {

    context->set_type(context->token_value.data_type);
    madInst.type = context->token_value.data_type;
    context->token_to_scan = yylex();

    if (context->token_type != REGISTER) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
    if (Operand(context, &madInst.o_operands[0])) {
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
    if (Operand(context, &madInst.o_operands[1])) {
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
    if (Operand(context, &madInst.o_operands[2])) {
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
    if (Operand(context, &madInst.o_operands[3])) {
      return 1;
    }
    if (context->token_to_scan != ';') {
      context->set_error(MISSING_SEMICOLON);
      return 1;
    }
    int* pCmp = reinterpret_cast<int*>(&aluModifier);
    if (*pCmp != 0) {      
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
        {0, 0, 0, 0, 0, 0, 0}  // aluModifier
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
  
  } else {  // DATA_TYPE_ID
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }  
}

int Instruction4BitOperation(Context* context) {
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
      bsoInst.opcode = BrigBitSelect;
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
    context->set_type(context->token_value.data_type);
    bsoInst.type = context->token_value.data_type;
    context->token_to_scan = yylex();

    if (context->token_type != REGISTER) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
    if (Operand(context, &bsoInst.o_operands[0])) {
      return 1;
    }
    if (context->token_to_scan != ',') {
      context->set_error(MISSING_COMMA);
      return 1;
    }
    context->token_to_scan = yylex();
    if (Operand(context, &bsoInst.o_operands[1])) {
      return 1;
    }
    if (context->token_to_scan != ',') {
      context->set_error(MISSING_COMMA);
      return 1;
    }
    context->token_to_scan = yylex();
    if (Operand(context, &bsoInst.o_operands[2])) {
      return 1;
    }
    if (context->token_to_scan != ',') {
        context->set_error(MISSING_COMMA);
        return 1;
    }
    context->token_to_scan = yylex();
    if (Operand(context, &bsoInst.o_operands[3])) {
      return 1;
    }
    if (context->token_to_scan != ';') {
      context->set_error(MISSING_SEMICOLON);
      return 1;
    }
    context->append_code(&bsoInst);
    context->token_to_scan = yylex();
    return 0;
  } else {  // DATA_TYPE_ID
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  } 
}

int Instruction4Cmov(Context* context) {
  BrigInstBase cmovInst = {
    sizeof(BrigInstBase),                    // size
    BrigEInstBase,         // kind
    BrigCmov,               // opcode
    0,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}       // o_operands[5]
  };

  context->token_to_scan = yylex();

  if (context->token_type != DATA_TYPE_ID) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  context->set_type(context->token_value.data_type);  
  cmovInst.type = context->token_value.data_type;
  BrigPacking packing = cmovInst.type >= Brigu8x4 ? BrigPackPP : BrigNoPacking;
  cmovInst.packing = packing;
  context->token_to_scan = yylex();

  std::string opName;
  if (context->token_type != REGISTER) {
    context->set_error(INVALID_FIRST_OPERAND);
    return 1;
  }
  if (Operand(context, &cmovInst.o_operands[0])) {
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
  if (Operand(context ,&cmovInst.o_operands[1])) {
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
  if (Operand(context, &cmovInst.o_operands[2])) {
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
  if (Operand(context, &cmovInst.o_operands[3])) {
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  context->append_code(&cmovInst);
  context->token_to_scan = yylex();
  return 0;
}

int Instruction4Shuffle(Context* context) {
  BrigInstBase shuffleInst = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigShuffle,           // opcode
    0,                     // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };

  context->token_to_scan = yylex();

  if (context->token_to_scan == _U8X4 ||
      context->token_to_scan == _S8X4 ||
      context->token_to_scan == _U8X8 ||
      context->token_to_scan == _S8X8 ||
      context->token_to_scan == _U16X2 ||
      context->token_to_scan == _S16X2 ||
      context->token_to_scan == _F16X2 ||
      context->token_to_scan == _U16X4 ||
      context->token_to_scan == _S16X4 ||
      context->token_to_scan == _F16X4 ||
      context->token_to_scan == _U32X2 ||
      context->token_to_scan == _S32X2 ||
      context->token_to_scan == _F32X2) {

    context->set_type(context->token_value.data_type);
    shuffleInst.type = context->token_value.data_type;
    context->token_to_scan = yylex();

    // Note: dest: Destination register.
    std::string opName;
    if (context->token_type != REGISTER) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
    if (Operand(context, &shuffleInst.o_operands[0])) {
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
    if (Operand(context, &shuffleInst.o_operands[1])) {
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
    if (Operand(context, &shuffleInst.o_operands[2])) {
      return 1;
    }
    if (context->token_to_scan != ',') {
        context->set_error(MISSING_COMMA);
        return 1;
    }
    context->token_to_scan = yylex();
    if (context->token_type != CONSTANT) {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
    if (Operand(context, &shuffleInst.o_operands[3])) {
      return 1;
    }
    if (context->token_to_scan != ';') {
      context->set_error(MISSING_SEMICOLON);
      return 1;
    }
    context->append_code(&shuffleInst);
    context->token_to_scan = yylex();
    return 0;
  } else{
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
}

int KernelArgumentDecl(Context *context) {
  if (DeclPrefix(context))
    return 1;

  context->set_dim(0);
  context->init_symbol_modifier();
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
  int arg_name_offset = context->add_symbol(arg_name);

  // scan for arrayDimensions
  context->token_to_scan = yylex();

  if ('[' == context->token_to_scan) {
    if (ArrayDimensionSet(context)) {
      // context->token_to_scan has been set in ArrayDimensionSet()
      return 1;
    }
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
  context->symbol_map[arg_name] = dsize;
        
  return 0;
}

int KernelArgumentListBody(Context *context){
  uint32_t paramCount = 0;
  if ('(' == context->token_to_scan){
    context->token_to_scan = yylex();
    return KernelArgumentListBody(context,&paramCount);
  } else { 
    return 1;
  }
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

int ComparisonId(Context* context, BrigCompareOperation32_t* pCmpOperation) {
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
    sizeof(BrigInstCmp),// size
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
  if (!ComparisonId(context, &cmpInst.comparisonOperator)) {
    context->token_to_scan = yylex();
    // 1, 32. Can be 16 if the implementation supports f16.
    if (context->token_type == DATA_TYPE_ID) {
      context->set_type(context->token_value.data_type);
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
        cmpInst.sourceType = cmpInst.type;
      }
      std::string opName;
      // Note: Dest must be a register.
      if (context->token_type == REGISTER) {
        if (Operand(context, &cmpInst.o_operands[0])) {
          return 1;
        }
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
          
          context->set_type(cmpInst.sourceType);
          
          if (!Operand(context, &cmpInst.o_operands[1])) {
            if (context->token_to_scan == ',') {
              context->token_to_scan = yylex();

              if (!Operand(context, &cmpInst.o_operands[2])) {
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
  BrigsOffset32_t str_offset;
  context->set_dim(0);
  context->init_symbol_modifier();
  
  if (PRIVATE != context->token_to_scan)
    return 1;

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
  
  str_offset = context->add_symbol(context->token_value.string_val);
  

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
  context->append_directive(&bds);

  context->token_to_scan = yylex();
  return 0;
} 

int OffsetAddressableOperandPart2(Context* context, BrigoOffset32_t addrOpOffset,
                                  BrigoOffset32_t* pRetOpOffset) {

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
                *pRetOpOffset = context->get_operand_offset();
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
          if (context->token_to_scan == ']') {
            *pRetOpOffset = context->get_operand_offset();
            context->append_operand(&compound_op);
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
          *pRetOpOffset = context->get_operand_offset();
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
          *pRetOpOffset = context->get_operand_offset();
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
  if (context->token_to_scan == '[') {
    BrigoOffset32_t currentToOffset = 0;
    context->token_to_scan = yylex();
    // AddressableOperand
    if (!AddressableOperand(context, &currentToOffset, false)) {
      if (context->token_to_scan == '[') {
        context->token_to_scan = yylex();
        if (!OffsetAddressableOperandPart2(context, currentToOffset, pRetOpOffset)) {
          // Global/Local Identifier with offsetAddressOperand.
          return 0;
        }
      }
      // only Global/Local Identifier without offsetAddressOperand.
      *pRetOpOffset = currentToOffset;
      return 0;
    } else if (!OffsetAddressableOperandPart2(context, 0, pRetOpOffset)) {
      return 0;
    }  // Global/Local Identifier/AddressableOperand/offsetAddressableOperand
  }  // '['
  return 1;
}

int Instruction5(Context* context) {
  // first token is F2U4 "f2u4"

  BrigInstBase f2u4Inst = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigF2u4,               // opcode
    Brigu32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}       // o_operands[5]
  };
  context->token_to_scan = yylex();
  // Note: must be _u32.
  if (context->token_to_scan == _U32) {
    context->set_type(context->token_value.data_type);
    context->token_to_scan = yylex();
    // Note: dest: The destination is an f32.

    if (!Operand(context, &f2u4Inst.o_operands[0])) {

      if (context->token_to_scan != ',') {
        context->set_error(MISSING_COMMA);
        return 1;
      }
      context->token_to_scan = yylex();

      // Note: Sources. All are b32
      if (!Operand(context, &f2u4Inst.o_operands[1])) {
        if (context->token_to_scan != ',') {
          context->set_error(MISSING_COMMA);
          return 1;
        }
        context->token_to_scan = yylex();

        // Note: Sources. All are b32
        if (!Operand(context, &f2u4Inst.o_operands[2])) {
          if (context->token_to_scan != ',') {
            context->set_error(MISSING_COMMA);
            return 1;
          }
          context->token_to_scan = yylex();

          // Note: Sources. All are b32
          if (!Operand(context, &f2u4Inst.o_operands[3])) {
            if (context->token_to_scan != ',') {
              context->set_error(MISSING_COMMA);
              return 1;
            }
            context->token_to_scan = yylex();
            // Note: Sources. All are b32

            if (!Operand(context, &f2u4Inst.o_operands[4])) {
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
  if (EXTENSION != context->token_to_scan)
    return 1;

  context->token_to_scan = yylex();

  if (context->token_to_scan == TOKEN_STRING) {
    std::string str(context->token_value.string_val);
    str.assign(str, 1, str.size() - 2);
    context->token_to_scan = yylex();

    if (context->token_to_scan == ';') {
      BrigDirectiveExtension bde = {
        sizeof(BrigDirectiveExtension),
        BrigEDirectiveExtension,
        context->get_code_offset(),
        context->add_symbol(str)
      };
      context->append_directive(&bde);

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
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigLdc,               // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };

  if (context->token_to_scan == _B32 ||
      (context->token_to_scan == _B64 &&
       context->get_machine() == BrigELarge)) {

    // If the source is a function, the destination size depends on
    // the machine model. For large, the destination must be 64 bits;
    // for small, the destination must be 32 bits. For
    // more information
    context->set_type(context->token_value.data_type);
    ldc_op.type = context->token_value.data_type;
    context->token_to_scan = yylex();
    // dest: must be BrigEOperandReg.
    if (context->token_to_scan == TOKEN_SREGISTER ||
        (context->token_to_scan == TOKEN_DREGISTER &&
         ldc_op.type == Brigb64)) {
    } else {
      context->set_error(INVALID_FIRST_OPERAND);
      return 1;
    }
    if (!Operand(context, &ldc_op.o_operands[0])) {
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        // op[1] must be BrigEOperandLabelRef or BrigEOperandFunctionRef

        if (context->token_to_scan == TOKEN_LABEL && ldc_op.type != Brigb64) {
          std::string oper_name = context->token_value.string_val;
          if (!context->label_o_map.count(oper_name)) {
            BrigOperandLabelRef labRef = {
              sizeof(BrigOperandLabelRef), // size
              BrigEOperandLabelRef,  // kind
              0                      // labeldirective
            };

            if (context->symbol_map.count(oper_name)) {
              labRef.labeldirective = context->symbol_map[oper_name];
            }
            ldc_op.o_operands[1] = context->get_operand_offset();
            context->label_o_map[oper_name] = context->get_operand_offset();
            context->append_operand(&labRef);

          } else {
            ldc_op.o_operands[1] = context->label_o_map[oper_name];
          }
        } else if (context->token_to_scan == TOKEN_GLOBAL_IDENTIFIER) {
          std::string oper_name = context->token_value.string_val;
          if (!context->func_o_map.count(oper_name)) {
            BrigOperandFunctionRef funRef = {
              sizeof(BrigOperandFunctionRef),   // size
              BrigEOperandFunctionRef,  // kind
              0                         // fn
            };
            // TODO(Chuang): whether the func has been defined.
            funRef.fn = context->func_map[oper_name];
            context->func_o_map[oper_name] = context->get_operand_offset();
            ldc_op.o_operands[1] = context->get_operand_offset();
            context->append_operand(&funRef);
          } else {
            ldc_op.o_operands[1] = context->func_o_map[oper_name];
          }
        } else {
          context->set_error(INVALID_SECOND_OPERAND);
          return 1;
        }
        context->token_to_scan = yylex();
        if (context->token_to_scan == ';') {
          context->append_code(&ldc_op);
          context->token_to_scan = yylex();
          return 0;
        } else {  // ';'
          context->set_error(MISSING_SEMICOLON);
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

  BrigMemorySemantic32_t MemorySemantic = BrigRegular;
  BrigAtomicOperation32_t atomicOperation;
  BrigStorageClass32_t StorageClass = BrigFlatSpace;
  BrigDataType16_t DataType;
  BrigoOffset32_t OpOffset[4] = {0,0,0,0};

  const unsigned int first_token = context->token_to_scan;
  if ((first_token != ATOMIC) && (first_token != ATOMIC_CAS)) {
    return 1;
  }
  context->token_to_scan = yylex();
  if ((first_token == ATOMIC) && (context->token_type == ATOMIC_OP)) {
    switch (context->token_to_scan) {
      case _AND_:
        atomicOperation = BrigAtomicAnd;
        break;
      case _OR_:
        atomicOperation = BrigAtomicOr;
        break;
      case _XOR_:
        atomicOperation = BrigAtomicXor;
        break;
      case _EXCH_:
        atomicOperation = BrigAtomicExch;
        break;
      case _ADD_:
        atomicOperation = BrigAtomicAdd;
        break;
      case _INC_:
        atomicOperation = BrigAtomicInc;
        break;
      case _DEC_:
        atomicOperation = BrigAtomicDec;
        break;
      case _MIN_:
        atomicOperation = BrigAtomicMin;
        break;
      case _MAX_:
        atomicOperation = BrigAtomicMax;
        break;
      case _SUB_:
        atomicOperation = BrigAtomicSub;
        break;
      default:
        context->set_error(MISSING_OPERATION);
        return 1;
    }
    context->token_to_scan = yylex();
  } else if(first_token==ATOMIC_CAS){
    atomicOperation = BrigAtomicCas;
  } else {
    context->set_error(MISSING_OPERATION);
    return 1;
  }

  if(context->token_type==ADDRESS_SPACE_IDENTIFIER){
    StorageClass = context->token_value.storage_class;
    context->token_to_scan = yylex();
  }

  if (AtomModifiers(context, &MemorySemantic)) {
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

  if (Operand(context, &OpOffset[0])) {
    return 1;
  }

  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();

  if (MemoryOperand(context, &OpOffset[1])) {
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
    sizeof(BrigInstAtomic),// size
    BrigEInstAtomic,       // kind
    BrigAtomic,       // opcode
    DataType,                     // type
    BrigNoPacking,         // packing
    {OpOffset[0], OpOffset[1], OpOffset[2], OpOffset[3], 0},       // o_operands[5]
    atomicOperation,         // atomicOperation
    StorageClass,       // storageClass
    MemorySemantic                      // memorySemantic
  };
  context->append_code(&atom_op);
  context->token_to_scan = yylex();
  return 0;

}

int Mov(Context* context) {
  
  BrigInstBase movInst = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigMov,               // opcode
    0,                     // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };

  context->token_to_scan = yylex();
  // TODO(Chuang): When type is b128 or b64 etc, check whether the operands is correct.
  if (context->token_to_scan != _B1 &&
      context->token_to_scan != _B32 &&
      context->token_to_scan != _B64 &&
      context->token_to_scan != _B128) {
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }    

  context->set_type(context->token_value.data_type);  
  movInst.type = context->token_value.data_type;
  context->token_to_scan = yylex();
  if (ArrayOperand(context, &movInst.o_operands[0])) {
    return 1;
  }
  if (context->token_to_scan != ',') {
    context->set_error(MISSING_COMMA);
    return 1;
  }
  context->token_to_scan = yylex();
  if (ArrayOperand(context, &movInst.o_operands[1])) {
    return 1;
  }
  if (context->token_to_scan != ';') {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
  context->append_code(&movInst);
  context->token_to_scan = yylex();
  return 0;
}

int GlobalGroupDecl(Context* context) {
  
  BrigsOffset32_t str_offset;
  context->set_dim(0);
  context->init_symbol_modifier();
  // first token is Group
  if (GROUP != context->token_to_scan)
    return 1;

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
  
  str_offset = context->add_symbol(context->token_value.string_val);
  context->set_dim(0);
  context->init_symbol_modifier();

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
  context->append_directive(&bds);

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
    if (!RoundingMode(context)) {
      aluModifier = context->get_alu_modifier();
    }
  }

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

  if (REGISTER != context->token_type) {
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

  if (*(reinterpret_cast<int*>(&aluModifier)) == 0) {
    BrigInstBase mulInst = {
      0,                     // size
      BrigEInstBase,         // kind
      opcode,                     // opcode
      type,               // type
      packing,         // packing
      {OpOffset[0], OpOffset[1], OpOffset[2], 0, 0}        // o_operands
    };
    mulInst.size = sizeof(mulInst);
    context->append_code(&mulInst);
  } else {
    BrigInstMod mulMod = {
      0,                     // size
      BrigEInstMod,         // kind
      opcode,                     // opcode
      type,               // type
      packing,         // packing
      {OpOffset[0], OpOffset[1], OpOffset[2], 0, 0},        // o_operands
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

  if (REGISTER != context->token_type){
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
      0, //size
      BrigEInstBase,        //kind
      opcode,                    //opcode
      type,              //type
      BrigNoPacking,        //packing
      {OpOffset[0],OpOffset[1],OpOffset[2],0,0}           //o_operands[5]
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

  if (REGISTER != context->token_type){
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
      0, //size
      BrigEInstBase,        //kind
      opcode,                    //opcode
      type,              //type
      BrigNoPacking,        //packing
      {OpOffset[0],OpOffset[1],OpOffset[2], OpOffset[3],0}           //o_operands[5]
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
  BrigoOffset32_t widthOffset = context->get_operand_offset();
  widthOffset += widthOffset & 0x7;
  ld_op.o_operands[0] = widthOffset;

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


  if (LdModifierPart2(context, &ld_op, &vector_size)) {
    return 1;
  }

  if (context->token_type == DATA_TYPE_ID) {
    // Get Type value in here
    context->set_type(context->token_value.data_type);
    ld_op.type = context->token_value.data_type;
    context->token_to_scan = yylex();
    if (!ArrayOperand(context, &ld_op.o_operands[1])) {
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();

        if (!MemoryOperand(context, &ld_op.o_operands[2])) {
          if (context->token_to_scan == ';') {
            context->append_code(&ld_op);
            context->token_to_scan = yylex();
            return 0;
          } else {  // ';'
            context->set_error(MISSING_SEMICOLON);
            return 1;
          }
        } else {  // Memory Operand
          context->set_error(INVALID_FIRST_OPERAND);
          return 1;
        }
      } else {  // ','
        context->set_error(MISSING_COMMA);
        return 1;
      }
    } else {
      context->set_error(INVALID_SECOND_OPERAND);
      return 1;
    }
  } else {  // Data Type
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
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

  if (LdModifierPart2(context, &st_op, &vector_size)) {
    return 1;
  }

  if (context->token_type == DATA_TYPE_ID) {
    context->set_type(context->token_value.data_type);
    st_op.type = context->token_value.data_type;
    context->token_to_scan = yylex();
    if (!ArrayOperand(context, &st_op.o_operands[0])) {
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        if (!MemoryOperand(context, &st_op.o_operands[1])) {
          if (context->token_to_scan == ';') {
            context->token_to_scan = yylex();
            context->append_code(&st_op);
            return 0;
          } else {  // ';'
            context->set_error(MISSING_SEMICOLON);
            return 1;
          }
        } else {  // Memory Operand
          context->set_error(INVALID_SECOND_OPERAND);
          return 1;
        }
      } else {  // ','
        context->set_error(MISSING_COMMA);
        return 1;
      }
    } else {
      context->set_error(INVALID_FIRST_OPERAND);
      return 1;
    }
  } else {  // Data Type
    context->set_error(MISSING_DATA_TYPE);
    return 1;
  }
  return 1;
}

int Lda(Context* context) {
  // first token is LDA
  context->token_to_scan = yylex();

  BrigInstBase lda_op = {
    sizeof(BrigInstBase),   // size
    BrigEInstBase,          // kind
    BrigLda,               // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}       // o_operands[5]
  };

  // Note: lda_uLength I think 'b' is also allowed.
  // Length: 1, 32, 64
  if (context->token_to_scan == _U32 ||
      context->token_to_scan == _U64 ||
      context->token_to_scan == _B32 ||
      context->token_to_scan == _B64) {

    context->set_type(context->token_value.data_type);  
    lda_op.type = context->token_value.data_type;
    context->token_to_scan = yylex();

    // dest: must be BrigEOperandReg.
    if ((context->token_to_scan == TOKEN_SREGISTER &&
         (lda_op.type == Brigu32 || lda_op.type == Brigb32)) ||
        (context->token_to_scan == TOKEN_DREGISTER &&
         (lda_op.type == Brigu64 || lda_op.type == Brigb64))) {
    } else {
      context->set_error(INVALID_OPERAND);
      return 1;
    }
    if (!Operand(context, &lda_op.o_operands[0])) {
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        if (!MemoryOperand(context, &lda_op.o_operands[1])) {
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
  return 1;
}

int Optacqreg(Context* context, BrigMemorySemantic32_t* pMemSemantic) {
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
  return Optacqreg(context, &temp);
}

int ImageRet(Context* context) {
  // first token is ATOMIC_IMAGE
  unsigned int second_token;
  BrigInstAtomicImage img_inst = {
    sizeof(BrigInstAtomicImage),// size
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
  if (!Optacqreg(context, &img_inst.memorySemantic)) {
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
      context->set_type(context->token_value.data_type);
      img_inst.type = context->token_value.data_type;

      context->token_to_scan = yylex();

      if (!Operand(context, &img_inst.o_operands[0])) {
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();
          if (context->token_to_scan == '[') {
            context->token_to_scan = yylex();
            if (!AddressableOperand(context, &img_inst.o_operands[1], true)) {
              if (context->token_to_scan == ',') {
                context->token_to_scan = yylex();
                unsigned int curOpCount = 2;
                if (!ArrayOperand(context, &img_inst.o_operands[curOpCount++])) {
                  if (context->token_to_scan == ',') {
                    context->token_to_scan = yylex();

                    if (!Operand(context, &img_inst.o_operands[curOpCount++])) {
                      if (second_token == _CAS) {
                        if (context->token_to_scan == ',') {
                          context->token_to_scan = yylex();

                          if (!Operand(context, &img_inst.o_operands[curOpCount++])) {
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
    sizeof(BrigInstAtomicImage),// size
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
    switch (context->token_to_scan) {  // without _CAS_ , _EXCh_
      case _AND_:
        imgNoRet.atomicOperation = BrigAtomicAnd;
        imgNoRet.type = Brigb32;
        break;
      case _OR_:
        imgNoRet.atomicOperation = BrigAtomicOr;
        imgNoRet.type = Brigb32;
        break;
      case _XOR_:
        imgNoRet.atomicOperation = BrigAtomicXor;
        imgNoRet.type = Brigb32;
        break;
    //   case _EXCH_:
    //    imgNoRet.atomicOperation = BrigAtomicExch;
    //   break;
      case _ADD_:
        imgNoRet.atomicOperation = BrigAtomicAdd;
        imgNoRet.type = Brigu64;
        break;
      case _INC_:
        imgNoRet.atomicOperation = BrigAtomicInc;
        imgNoRet.type = Brigs32;
        break;
      case _DEC_:
        imgNoRet.atomicOperation = BrigAtomicDec;
        imgNoRet.type = Brigb32;
        break;
      case _MIN_:
        imgNoRet.atomicOperation = BrigAtomicMin;
        imgNoRet.type = Brigu32;
        break;
      case _MAX_:
        imgNoRet.atomicOperation = BrigAtomicMax;
        imgNoRet.type = Brigu32;
        break;
      case _SUB_:
        imgNoRet.atomicOperation = BrigAtomicSub;
        imgNoRet.type = Brigu64;
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
  if (!Optacqreg(context, &imgNoRet.memorySemantic)) {
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
    // TypeLength: u32, s32, b32, u64, depending on the type of operation.
    // The add operation applies to u32, u64, and s32 types;
    // min and max apply to u32 and s32 types; inc and dec apply to s32 types;
    // and and, or, xor, and cas apply to b32 types.
    /*
    BrigDataType16_t type = context->token_value.data_type;
    if (type == imgNoRet.type ||
        ((imgNoRet.atomicOperation == BrigAtomicSub ||
          imgNoRet.atomicOperation == BrigAtomicAdd) &&
         (type == Brigu32 || type == Brigs32)) ||
        ((imgNoRet.atomicOperation == BrigAtomicMin ||
          imgNoRet.atomicOperation == BrigAtomicMax) &&
         type == Brigs32)) {
      imgNoRet.type = type;
    } else {
      context->set_error(MISSING_DATA_TYPE);
      return 1;
    }*/

    if (context->token_type == DATA_TYPE_ID) {

      std::string op_name;
      imgNoRet.type = context->token_value.data_type;
      context->set_type(context->token_value.data_type);

      context->token_to_scan = yylex();
      if (context->token_to_scan == '[') {
        context->token_to_scan = yylex();
        if (!AddressableOperand(context, &imgNoRet.o_operands[0], true)) {
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if (!ArrayOperand(context, &imgNoRet.o_operands[1])) {
              if (context->token_to_scan == ',') {
                context->token_to_scan = yylex();
                unsigned int opCount = 2;

                if (!Operand(context, &imgNoRet.o_operands[opCount++])) {
                  if (second_token == _CAS) {
                    if (context->token_to_scan == ',') {
                      context->token_to_scan = yylex();
                      if (context->valid_string) {
                        op_name = context->token_value.string_val;
                      }
                      if (!Operand(context, &imgNoRet.o_operands[opCount++])) {
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
    sizeof(BrigInstCvt),   // size
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

  if (!RoundingMode(context)) {
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

    context->set_type(context->token_value.data_type);  
    cvtInst.type = context->token_value.data_type;
    context->token_to_scan = yylex();

    if (context->token_type == DATA_TYPE_ID) {
      cvtInst.stype = context->token_value.data_type;
      context->token_to_scan = yylex();
      // Note: dest: Destination register.
      if (context->token_type != REGISTER) {
        context->set_error(INVALID_OPERAND);
        return 1;
      }
      if (!Operand(context, &cvtInst.o_operands[0])) {
        if (context->token_to_scan == ',') {
          context->token_to_scan = yylex();

          if (!Operand(context, &cvtInst.o_operands[1])) {

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
  BrigInstBase inst0 = {
    sizeof(BrigInstBase),  // size
    BrigEInstBase,         // kind
    BrigNop,               // opcode
    Brigb32,               // type
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0}        // o_operands[5]
  };
  context->token_to_scan = yylex();
  if (context->token_to_scan == ';') {
    context->append_code(&inst0);
    context->token_to_scan = yylex();
    return 0;
  } else {
    context->set_error(MISSING_SEMICOLON);
  }
  return 1;
}

int Instruction1OpcodeDT(Context* context) {
  BrigInstBase inst1_op = {
    sizeof(inst1_op),
    BrigEInstBase,
    0,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  BrigAluModifier aluModifier = {0, 0, 0, 0, 0, 0, 0};

  inst1_op.opcode = context->token_value.opcode;
  // TODO(Chuang): fbar_wait_segment_b64
  // fbar_init_segment_b64
  // fbar_release_segment_b64
  // and whether there should be rounding in instruction1opcode.
  context->token_to_scan = yylex();

  if (inst1_op.opcode != BrigFbarInit &&
      inst1_op.opcode != BrigFbarRelease) {
    // TODO(Chuang): whether support for rounding
    if (!RoundingMode(context)) {
      aluModifier = context->get_alu_modifier();
    }
  }
  // TODO(Chuang): What can the type of BrigFbarInit be?
  if ((context->token_to_scan == _B64 &&
       inst1_op.opcode != BrigCountUp) ||
      ((inst1_op.opcode == BrigCountUp ||
        inst1_op.opcode == BrigFbarInit) &&
       context->token_to_scan == _U32)) {

    inst1_op.type = context->token_value.data_type;
    context->set_type(context->token_value.data_type);
    
    context->token_to_scan = yylex();
    if ((context->token_to_scan == TOKEN_DREGISTER &&
         inst1_op.opcode != BrigCountUp) ||
        (context->token_to_scan == TOKEN_SREGISTER &&
         (inst1_op.opcode == BrigCountUp ||
          inst1_op.opcode == BrigFbarInit))) {
      if (Operand(context, &inst1_op.o_operands[0])) {
        return 1;
      }
      // Note: Fbar_Release and Fbar_Init may be a instruction2.
      if (inst1_op.opcode == BrigFbarInit ||
          inst1_op.opcode == BrigFbarRelease) {
        if (context->token_to_scan == ',')  {
          context->token_to_scan = yylex();
          if (context->token_to_scan == TOKEN_DREGISTER) {
            if (Operand(context, &inst1_op.o_operands[1])) {
              return 1;
            }
          } else {  // Second Operand
            context->set_error(MISSING_OPERAND);
            return 1;
          }
        }
      }
      if (context->token_to_scan == ';') {
        int* aluValue = reinterpret_cast<int *>(&aluModifier);
        if (*aluValue != 0) {
          BrigInstMod mod = {
            sizeof(BrigInstMod),  // size
            BrigEInstMod,         // kind
            inst1_op.opcode,              // opcode
            inst1_op.type,         // type
            inst1_op.packing,        // packing
            {0, 0, 0, 0, 0},      // o_operands[5]
            {0, 0, 0, 0, 0, 0, 0}  // aluModifier
          };
          for (int i = 0 ; i < 5 ; ++i) {
            mod.o_operands[i] = inst1_op.o_operands[i];
          }
          mod.aluModifier = aluModifier;
          context->append_code(&mod);
        } else {
          context->append_code(&inst1_op);
        }
        context->token_to_scan = yylex();
        return 0;
      } else {
        context->set_error(MISSING_SEMICOLON);
      }
    } else {  // First Operand
      context->set_error(MISSING_OPERAND);
    }
  } else {
    context->set_error(INVALID_DATA_TYPE);
  }

  return 1;
}

int Instruction1OpcodeNoDT(Context* context) {
  BrigInstBase inst1_op = {
    sizeof(inst1_op),
    BrigEInstBase,
    0,
    Brigb32,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  BrigAluModifier aluModifier = {0, 0, 0, 0, 0, 0, 0};
  context->set_type(Brigb32);
  inst1_op.opcode = context->token_value.opcode;
  
  context->token_to_scan = yylex();
  // TODO(Chuang): whether support for rounding
  if (!RoundingMode(context)) {
    aluModifier = context->get_alu_modifier();
  }

  if ((context->token_to_scan == TOKEN_SREGISTER &&
       inst1_op.opcode != BrigDebugTrap) ||
      (inst1_op.opcode == BrigDebugTrap &&
       context->token_to_scan == TOKEN_INTEGER_CONSTANT)) {
    if (Operand(context, &inst1_op.o_operands[0])) {
      return 1;
    }
    if (context->token_to_scan == ';') {
      int* aluValue = reinterpret_cast<int*>(&aluModifier);
      if (*aluValue != 0) {
        BrigInstMod mod = {
          sizeof(BrigInstMod),  // size
          BrigEInstMod,         // kind
          inst1_op.opcode,              // opcode
          inst1_op.type,         // type
          BrigNoPacking,        // packing
          {0, 0, 0, 0, 0},      // o_operands[5]
          {0, 0, 0, 0, 0, 0, 0}  // aluModifier
        };
        for (int i = 0 ; i < 5 ; ++i) {
          mod.o_operands[i] = inst1_op.o_operands[i];
        }
        mod.aluModifier = aluModifier;
        context->append_code(&mod);
      } else {
        context->append_code(&inst1_op);
      }
      context->token_to_scan = yylex();
      return 0;
    } else {
      context->set_error(MISSING_SEMICOLON);
    }
  } else {
    context->set_error(MISSING_OPERAND);
  }
  return 0;
}

int Instruction1Clock(Context* context) {
  BrigInstBase inst1_op = {
    sizeof(inst1_op),
    BrigEInstBase,
    BrigClock,
    Brigb64,
    BrigNoPacking,
    {0, 0, 0, 0, 0}
  };
  context->set_type(Brigb64);
  context->token_to_scan = yylex();
  if (context->token_to_scan == TOKEN_DREGISTER) {
    if (Operand(context, &inst1_op.o_operands[0])) {
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
  return 1;
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
  if (context->token_to_scan == SEGMENTP) { //segmentp
    context->token_to_scan = yylex();

    BrigInstMem segmentp_op = {
      sizeof(BrigInstMem),   // size
      BrigEInstMem,          // kind
      BrigSegmentp,          // opcode
      Brigb32,               // type
      BrigNoPacking,         // packing
      {0, 0, 0, 0, 0},       // o_operands[5]
      BrigFlatSpace          // storageClass
    };

    if (context->token_type == ADDRESS_SPACE_IDENTIFIER) {
      segmentp_op.storageClass = context->token_value.storage_class;

      context->token_to_scan = yylex();
      if (context->token_to_scan == _B1) { //datatypeId must be b1
        context->set_type(context->token_value.data_type);
        segmentp_op.type = context->token_value.data_type;
        context->token_to_scan = yylex();
        //dest must be c register
        if (context->token_to_scan == TOKEN_CREGISTER) {
          if (Operand(context, &segmentp_op.o_operands[0])) {
            return 1;
          }
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if(context->token_type == REGISTER ) {
              if (Operand(context, &segmentp_op.o_operands[1])) {
                return 1;
              }
              if (context->token_to_scan == ';') {
                context->append_code(&segmentp_op);

                context->token_to_scan = yylex();
                return 0;
              } else {
                context->set_error(MISSING_SEMICOLON);
              }
            } else if (context->token_type == CONSTANT ||
                       context->token_to_scan == TOKEN_WAVESIZE) {
              if (Operand(context, &segmentp_op.o_operands[1])) {
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
      context->set_error(MISSING_IDENTIFIER);
    }
  } else if (context->token_to_scan == STOF || // stof or ftos
             context->token_to_scan == FTOS) {

    BrigInstMem sf_op = {
      sizeof(BrigInstMem),   // size
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
      sf_op.storageClass = context->token_value.storage_class;

      context->token_to_scan = yylex();
      if (context->token_to_scan == _U32 ||
          context->token_to_scan == _U64) { //datatypeId must be u32 or u64
        context->set_type(context->token_value.data_type);
        sf_op.type = context->token_value.data_type;
        context->token_to_scan = yylex();
        //dest must be d register
        if (context->token_to_scan == TOKEN_DREGISTER) {
          if (Operand(context, &sf_op.o_operands[0])) {
            return 1;
          }
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if(context->token_type == REGISTER ) {
              if (Operand(context, &sf_op.o_operands[1])) {
                return 1;
              }
              if (context->token_to_scan == ';') {
                context->append_code(&sf_op);
                context->token_to_scan = yylex();
                return 0;
              } else {
                context->set_error(MISSING_SEMICOLON);
              }
            } else if (context->token_type == CONSTANT ||
                       context->token_to_scan == TOKEN_WAVESIZE) {
              if (Operand(context, &sf_op.o_operands[1])) {
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
      context->set_error(MISSING_IDENTIFIER);
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
    if (!Label(context)) {
      if(context->token_to_scan == LABELTARGETS) {
        return LabelTargetsPart2(context);
      }
      return 0;
    } else {
      return 1;
    }
  } else if (!Operation(context)) {
    context->update_bdf_operation_count();
    return 0;
  }

  return 1;
}

int ArgStatement(Context* context) {
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
  if (!ArgStatement(context)) {
    while (!ArgStatement(context)) {
      ;
    }
    return 0;
  }
  return 1;
}

int Comment(Context* context){

    if(context->token_to_scan == TOKEN_COMMENT){
    std::string comment(context->token_value.string_val);
    int comment_offset = context->add_symbol(comment);
    BrigDirectiveComment dir_com = { sizeof(BrigDirectiveComment), BrigEDirectiveComment, context->get_code_offset(), comment_offset};
    context->append_directive(&dir_com);
    context->token_to_scan = yylex();
    return 0;
    } else {
        context->set_error(INVALID_COMMENT);
        return 1;
    }
}

int BodyStatement(Context* context) {

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
      if (!Label(context)) {
		if(context->token_to_scan == LABELTARGETS)
			return LabelTargetsPart2(context);
		return 0;
	  }else
		return 1;
  } else if (!Operation(context)) {
    context->update_bdf_operation_count();
    return 0;
  }

  return 1;
}

int BodyStatements(Context* context) {
  if (!BodyStatement(context)) {
    while (context->token_to_scan && (context->token_to_scan != '}')){
      if(BodyStatement(context))
		break;
	  //context->token_to_scan = yylex();
	}
	if((!context->token_to_scan) || (context->token_to_scan == '}'))
		return 0;
	else
		return 1;
  } else{
    context->set_error(INVALID_CODEBLOCK); //Codeblock should have atleast one bodyStatement
    return 1;
  }
 }

int ImageLoad(Context* context) {
  // first token is LD_IMAGE
  BrigInstImage imgLdInst = {
    sizeof(BrigInstImage), // size
    BrigEInstImage,        // kind
    BrigLdImage,           // opcode
    0,                 // type
    0,                 // stype
    BrigNoPacking,         // packing
    0,                      // reserved
    {0, 0, 0, 0, 0},   // o_operands[5]
    0                 // geom
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
          context->set_type(context->token_value.data_type);
          imgLdInst.stype = context->token_value.data_type;
          context->token_to_scan = yylex();
          if (!ArrayOperand(context, &imgLdInst.o_operands[0])) {
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
                if (!AddressableOperand(context, &imgLdInst.o_operands[1], true)) {
                  if (context->token_to_scan == ',') {
                    context->token_to_scan = yylex();
                    // TODO(Chuang): src: Register source for the coordinates.
                    // A scalar for 1D images; a 2-element vector for 2D images;
                    // a 4-element vector for 3D images, where the fourth element is ignored.
                    // Each coordinate must be in an s register

                    if (!ArrayOperand(context, &imgLdInst.o_operands[2])) {
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
    sizeof(BrigInstImage), // size
    BrigEInstImage,        // kind
    BrigStImage,           // opcode
    0,                 // type
    0,                 // stype
    BrigNoPacking,         // packing
    0,                      // reserved
    {0, 0, 0, 0, 0},   // o_operands[5]
    0                 // geom
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
          context->set_type(context->token_value.data_type);
          imgStInst.stype = context->token_value.data_type;
          context->token_to_scan = yylex();
          if (!ArrayOperand(context, &imgStInst.o_operands[0])) {
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
                if (!AddressableOperand(context, &imgStInst.o_operands[1], true)) {
                  if (context->token_to_scan == ',') {
                    context->token_to_scan = yylex();
                    // TODO(Chuang): src: Register source for the coordinates.
                    // A scalar for 1D images; a 2-element vector for 2D images;
                    // a 4-element vector for 3D images, where the fourth element is ignored.
                    // Each coordinate must be in an s register
                    if (!ArrayOperand(context, &imgStInst.o_operands[2])) {
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
  if (0 == context->get_dim()) {
    if (context->get_isArray()) {
      bds.s.symbolModifier = BrigArray;
      context->set_symbol_modifier(BrigArray);
    }
    bds.s.dim = elementCount;
  } else if (elementCount > bds.s.dim) {
    context->set_error(INVALID_INITIALIZER);
    return 1;
  }
  n = bds.s.dim;
  context->set_dim(bds.s.dim);
  
  switch (context->get_type()) {
    case Brigb1:
      context->set_error(INVALID_INITIALIZER);
      return 1;
    case Brigb8:    n = (n + 7) >> 3; break;
    case Brigb16:   n = (n + 3) >> 2; break;
    case Brigb32:   n = (n + 1) >> 1; break;
    case Brigb64:   break;
  }
  size_t arraySize = sizeof(BrigDirectiveInit) + (n - 1) * sizeof(uint64_t);
  uint8_t *array = new uint8_t[arraySize];
  memset(array, 0 , sizeof(uint8_t) * arraySize);
  BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit*>(array);
  bdi->elementCount = bds.s.dim;
  bdi->size = arraySize;
  bdi->kind = BrigEDirectiveInit;
  bdi->c_code = 0;
  bdi->type = context->get_type();
  bdi->reserved = 0;
  switch (context->get_type()) {
    case Brigb8:
      for (uint32_t i = 0; i < single_list.size(); i++ ) {
        *(float*)&bdi->initializationData.u8[i] = single_list[i];
      }
      break;
    case Brigb16:
      for (uint32_t i = 0; i < single_list.size(); i++ ) {
        *(float*)&bdi->initializationData.u16[i] = single_list[i];
      }
      break;
    case Brigb32:
      for (uint32_t i = 0; i < single_list.size(); i++ ) {
        *(float*)&bdi->initializationData.u32[i] = single_list[i];
      }
      break;
    case Brigb64:
      // TODO(Chuang): Loss of precision
      for (uint32_t i = 0; i < single_list.size(); i++ ) {
        *(double*)&bdi->initializationData.u64[i] = (double)single_list[i];
      }
      break;
  }
  bds.d_init = context->get_directive_offset();
  bds.d_init += bds.d_init & 0x7;
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
    if (context->token_to_scan == ',') {
      context->token_to_scan = yylex();
      if (context->token_to_scan == TOKEN_SINGLE_CONSTANT) {
        continue;
      } else {
        context->set_error(MISSING_SINGLE_CONSTANT);
        return 1;
      }
    } else 
      break;
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
         context->get_directive(&bdi);
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

  context->update_last_directive(bdi_charp, sizeof(bdi));

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

  context->token_to_scan = yylex();
  context->set_dim(0);
  if ('[' == context->token_to_scan) {
    if (ArrayDimensionSet(context)) {
      return 1;
    }
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
  context->append_directive(&bdi);

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
  if(GLOBAL == context->token_to_scan){
    context->token_to_scan = yylex();

    return GlobalReadOnlyImageDeclPart2(context);
  }else{
    return 1;
  }
}

int GlobalReadOnlyImageDeclPart2(Context *context){
  //First token has been scanned and verified as global. Scan next token.

 if (_ROIMG == context->token_to_scan) {
    context->token_to_scan = yylex();
    if (TOKEN_GLOBAL_IDENTIFIER == context->token_to_scan) {
      std::string var_name(context->token_value.string_val);
      int var_name_offset = context->add_symbol(var_name);

      context->token_to_scan = yylex();
      // set default value(scalar)
      context->set_dim(0);
      //context->set_symbol_modifier(BrigArray);
      if ('[' == context->token_to_scan) {
        if (!ArrayDimensionSet(context)) {
        } else {
          return 1;
        }
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
  if(RET != context->token_to_scan) {
    return 1;
  }

  BrigOpcode32_t opcode = context->token_value.opcode ;

  context->token_to_scan = yylex();
  if (context->token_to_scan == ';') {
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
  } else {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }

}

int ImageRead(Context *context) {
  // first token is RD_IMAGE
  BrigInstRead imgRdInst = {
    sizeof(BrigInstRead),  // size
    BrigEInstRead,         // kind
    BrigRdImage,           // opcode
    0,                     // type
    0,                     // stype
    BrigNoPacking,         // packing
    {0, 0, 0, 0, 0},       // o_operands[5]
    0,                     // geom
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
          context->set_type(context->token_value.data_type);
          imgRdInst.stype = context->token_value.data_type;
          context->token_to_scan = yylex();

          if (!ArrayOperand(context, &imgRdInst.o_operands[0])) {
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
                if (!AddressableOperand(context, &imgRdInst.o_operands[1], true)) {
                  if (context->token_to_scan == ',') {
                    unsigned int opCount = 2;
                    context->token_to_scan = yylex();
                    if (context->token_to_scan == '[') {
                      context->token_to_scan = yylex();
                      if (!AddressableOperand(context, &imgRdInst.o_operands[opCount++], true)) {
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

                    if (!ArrayOperand(context, &imgRdInst.o_operands[opCount])) {

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
  return 1;
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
  offset += offset & 0x7;

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
    }
    if (context->token_to_scan == ';') {
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
    } else {
      context->set_error(MISSING_SEMICOLON);
      return 1;
    }
}

int AtomModifiers(Context* context, BrigMemorySemantic32_t *pMemorySemantic) {

  if (context->token_to_scan == _AR) {
    *pMemorySemantic = BrigAcquireRelease;
    context->token_to_scan = yylex();
  } else if (context->token_to_scan == _ACQ) {
    *pMemorySemantic = BrigAcquire;
    context->token_to_scan = yylex();
  } else if (context->token_to_scan == _PART_AR) {
    *pMemorySemantic = BrigParAcquireRelease;
    context->token_to_scan = yylex();
  }

  if(context->token_to_scan == _REGION){
    //TODO: What parameter needs to be set if the token is _REGION?
     context->token_to_scan = yylex();
  }
  return 0;
}

int AtomicNoRet(Context* context) {
  // first token is ATOMICNORET or ATOMICNORET_CAS

  BrigMemorySemantic32_t MemorySemantic = BrigRegular;
  BrigAtomicOperation32_t atomicOperation;
  BrigStorageClass32_t StorageClass = BrigFlatSpace;
  BrigDataType16_t DataType;
  BrigoOffset32_t OpOffset[3] = {0,0,0};

  const unsigned int first_token = context->token_to_scan;
  if ((first_token != ATOMICNORET) && (first_token != ATOMICNORET_CAS)) {
    return 1;
  }
  context->token_to_scan = yylex();
  if ((first_token == ATOMICNORET) && (context->token_type == ATOMIC_OP)) {
    switch (context->token_to_scan) {  // without _CAS_
      case _AND_:
        atomicOperation = BrigAtomicAnd;
        break;
      case _OR_:
        atomicOperation = BrigAtomicOr;
        break;
      case _XOR_:
        atomicOperation = BrigAtomicXor;
        break;
      case _EXCH_:
        atomicOperation = BrigAtomicExch;
        break;
      case _ADD_:
        atomicOperation = BrigAtomicAdd;
        break;
      case _INC_:
        atomicOperation = BrigAtomicInc;
        break;
      case _DEC_:
        atomicOperation = BrigAtomicDec;
        break;
      case _MIN_:
        atomicOperation = BrigAtomicMin;
        break;
      case _MAX_:
        atomicOperation = BrigAtomicMax;
        break;
      case _SUB_:
        atomicOperation = BrigAtomicSub;
        break;
      default:
        context->set_error(MISSING_OPERATION);
        return 1;
    }
      context->token_to_scan = yylex();
  } else if(first_token==ATOMICNORET_CAS){
    atomicOperation = BrigAtomicCas;
  } else{
      context->set_error(MISSING_OPERATION);
      return 1;
  }

  if(context->token_type == ADDRESS_SPACE_IDENTIFIER){
    StorageClass = context->token_value.storage_class;
    context->token_to_scan = yylex();
  }

  if (AtomModifiers(context, &MemorySemantic)) {
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
    if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
      values[0] = context->token_value.int_val; //TODO
    } else {  // Integer Constant
      context->set_error(MISSING_INTEGER_CONSTANT);
      return 1;
    }
  } else if (context->token_to_scan == ITEMS_PER_WORKGROUP) {
    controlType = BrigEMaxWIperG;
    context->token_to_scan = yylex();
    if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
      values[0] = context->token_value.int_val;
      context->token_to_scan = yylex();
      if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
          values[1] = context->token_value.int_val;
          context->token_to_scan = yylex();
          if (context->token_to_scan == ',') {
            context->token_to_scan = yylex();
            if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
              values[2] = context->token_value.int_val;
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
    return 1;
  }
  context->token_to_scan = yylex();
  if (context->token_to_scan == ';') {
    // codegen
    BrigDirectiveControl bdc = {
      sizeof(BrigDirectiveControl),   // size
      BrigEDirectiveControl,          // kind
      context->get_code_offset(),     // c_code
      controlType,                    // controlType
      {values[0],values[1],values[2]} //values
    };
    context->append_directive(&bdc);

    context->token_to_scan = yylex();
    return 0;
  } else {
    context->set_error(MISSING_SEMICOLON);
    return 1;
  }
}

int Pragma(Context* context) {
  // first token is PRAGMA
  if(PRAGMA != context->token_to_scan)
    return 1;

  context->token_to_scan = yylex();
  if (context->token_to_scan == TOKEN_STRING) {
    std::string s_name = context->token_value.string_val;
    s_name.assign(s_name, 1, s_name.size() - 2); 

    context->token_to_scan = yylex();
    if (context->token_to_scan == ';') {
      BrigDirectivePragma bdp = {
        sizeof(BrigDirectivePragma), // size
        BrigEDirectivePragma,        // kind
        context->get_code_offset(),  // c_code
        context->add_symbol(s_name)  // s_name
      };
      context->append_directive(&bdp);

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

int LabelList(Context* context, BrigdOffset32_t bds_offset) {
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
      context->get_directive(bds_offset,&bds);
      bds.d_init = context->get_directive_offset();
      bds.d_init += bds.d_init & 0x7;
      // TODO(Chuang):  handle to an empty list.
      if (bds.s.dim != 0) {
        if (elementCount > bds.s.dim) {
          context->set_error(INVALID_INITIALIZER);
          return 1;
        }
      } else {
        bds.s.dim = elementCount;
      }
      context->set_dim(bds.s.dim);
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
      delete[] reinterpret_cast<char *>(bdli);

      return 0;
    }
  }  // While

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

  if (0 == context->get_dim()) {
    if (context->get_isArray()) {
      // If array is empty [],  Update dim.
      bds.s.symbolModifier = BrigArray;
      context->set_symbol_modifier(BrigArray);
    }
    bds.s.dim = elementCount;
  } else if (elementCount > bds.s.dim) {
    context->set_error(INVALID_INITIALIZER);
    return 1;
  }
  n = bds.s.dim;
  context->set_dim(bds.s.dim);
  switch (context->get_type()) {
    case Brigb1:
      context->set_error(INVALID_INITIALIZER);
      return 1;
    case Brigb8:    n = (n + 7) >> 3; break;
    case Brigb16:   n = (n + 3) >> 2; break;
    case Brigb32:   n = (n + 1) >> 1; break;
    case Brigb64:   break;
  }
  size_t arraySize = sizeof(BrigDirectiveInit) + (n - 1) * sizeof(uint64_t);
  uint8_t *array = new uint8_t[arraySize];
  memset(array, 0, sizeof(uint8_t) * arraySize);

  BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit*>(array);
  bdi->elementCount = bds.s.dim;

  bdi->size = arraySize;
  bdi->kind = BrigEDirectiveInit;
  bdi->c_code = 0;
  bdi->type = context->get_type();
  bdi->reserved = 0;
  switch(context->get_type()) {
    case Brigb8:
      for (uint32_t i = 0; i < float_list.size(); i++ ) {
        *(double*)&bdi->initializationData.u8[i] = float_list[i];
      }
      break;
    case Brigb16:
      for (uint32_t i = 0; i < float_list.size(); i++ ) {
        *(double*)&bdi->initializationData.u16[i] = float_list[i];
      }
      break;
    case Brigb32:
      for (uint32_t i = 0; i < float_list.size(); i++ ) {
        *(double*)&bdi->initializationData.u32[i] = float_list[i];
      }
      break;
    case Brigb64:
      // TODO(Chuang): Loss of precision
      for (uint32_t i = 0; i < float_list.size(); i++ ) {
        *(double*)&bdi->initializationData.u64[i] = float_list[i];
      }
      break;
  }
  bds.d_init = context->get_directive_offset();
  bds.d_init += bds.d_init & 0x7;
  unsigned char *bds_charp = reinterpret_cast<unsigned char*>(&bds);
  context->update_directive_bytes(bds_charp, symbol_offset,
                                  sizeof(BrigDirectiveSymbol));
  context->append_directive(bdi);
  delete[] reinterpret_cast<uint8_t *>(bdi);
  bdi = NULL;
  return 0;
}

int FloatListSingle(Context* context, std::vector <double>* float_list) {
    
  while (context->token_to_scan == TOKEN_DOUBLE_CONSTANT) {
    float_list->push_back(context->token_value.double_val);
    context->token_to_scan = yylex();

    if (context->token_to_scan == ',') {
      context->token_to_scan = yylex();
      if(context->token_to_scan != TOKEN_DOUBLE_CONSTANT){
        context->set_error(MISSING_DOUBLE_CONSTANT);
        return 1;
      } else
        continue;
    } else 
      break;
  }
  return 0;  
}

int DecimalListSingleBlock(Context* context) {
  
  std::vector<int32_t> decimal_list;
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
        *(int32_t*)&blockNumeric->u32[i] = decimal_list[i];
      }
      break;
    case Brigb64:
      // TODO(Chuang): Loss of precision
      for (uint32_t i = 0; i < decimal_list.size(); i++ ) {
        *(int32_t*)&blockNumeric->u64[i] = decimal_list[i];
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

  std::vector<int32_t> decimal_list;
  if(DecimalListSingle(context, &decimal_list))
    return 1;
  uint32_t n = 0;
  uint32_t elementCount = decimal_list.size();
  
  BrigDirectiveSymbol bds ;
  context->get_directive(symbol_offset,&bds);

  if (0 == context->get_dim()) {
    if (context->get_isArray()) {
      // If array is empty [],  Update dim.
      bds.s.symbolModifier = BrigArray;
      context->set_symbol_modifier(BrigArray);
    }
    bds.s.dim = elementCount;
  } else if (elementCount > bds.s.dim) {
    context->set_error(INVALID_INITIALIZER);
    return 1;
  }
  n = bds.s.dim;
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
  bdi->elementCount = bds.s.dim;

  bdi->size = arraySize;
  bdi->kind = BrigEDirectiveInit;
  bdi->c_code = 0;
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
        *(int32_t*)&bdi->initializationData.u32[i] = decimal_list[i];
      }
      break;
    case Brigb64:
      // TODO(Chuang): Loss of precision
      for (uint32_t i = 0; i < decimal_list.size(); i++ ) {
        *(int32_t*)&bdi->initializationData.u64[i] = decimal_list[i];
      }
      break;
  }
  bds.d_init = context->get_directive_offset();
  bds.d_init += bds.d_init & 0x7;
  unsigned char *bds_charp = reinterpret_cast<unsigned char*>(&bds);
  context->update_directive_bytes(bds_charp, symbol_offset,
                                  sizeof(BrigDirectiveSymbol));

  context->append_directive(bdi);
  delete[] reinterpret_cast<uint8_t *>(bdi);
  bdi = NULL;
  return 0;  

}

int DecimalListSingle(Context* context, std::vector<int32_t>* decimal_list) {
  
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
    
    if (context->token_to_scan == TOKEN_INTEGER_CONSTANT) {
      elementCount++;
      decimal_list->push_back(context->token_value.int_val);
    }
    context->token_to_scan = yylex();
    if (context->token_to_scan == ',') {
        context->token_to_scan = yylex();
        continue;
    } else 
      break;
  }
  return 0;
}

int Block(Context* context) {
  // first token is BLOCK
  if (BLOCK != context->token_to_scan)
    return 1;

  context->token_to_scan = yylex();
  if (context->token_to_scan == TOKEN_STRING) {
    std::string name = context->token_value.string_val;
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
        if (context->token_to_scan == TOKEN_STRING) {
          std::string str = context->token_value.string_val;
          // block string
          BrigBlockString bbs = {
            sizeof(BrigBlockString),
            BrigEDirectiveBlockString,
            context->add_symbol(str)
          };
          context->append_directive(&bbs);

          context->token_to_scan = yylex();
        } else {  // String
          context->set_error(INVALID_SECTION_ITEM);
          return 1;
        }
      } else if (context->token_to_scan == BLOCKNUMERIC) {
        
        context->token_to_scan = yylex();
        if (context->token_type == DATA_TYPE_ID) {
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
      // block end
      BrigBlockEnd bbe = {
        sizeof(BrigBlockEnd),
        BrigEDirectiveBlockEnd
      };
      context->append_directive(&bbe);

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
  unsigned char *bds_charp = reinterpret_cast<unsigned char*> (&bds);
  if(valid_dir == Context::CONTEXT_OK)      
    context->update_last_directive(bds_charp, sizeof(bds));

  context->token_to_scan = yylex();
  return 0;
   
}

int SobInitializer(Context *context){
  //first must be '='
  if('=' != context->token_to_scan){
     return 1;
  }

  context->token_to_scan = yylex();
  if('{' != context->token_to_scan){
    context->set_error(MISSING_OPENNING_BRACKET);
    return 1;
  }
  
  while(1){
      context->token_to_scan = yylex();
      if(SobInit(context)){
        context->set_error(INVALID_INITIALIZER);
        return 1;
      }
      if(',' != context->token_to_scan)
        break ;      
  }

  if('}' != context->token_to_scan){
    context->set_error(MISSING_CLOSING_BRACKET);
    return 1;
  }
  context->token_to_scan = yylex();
  return 0 ;
  
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
  context->token_to_scan = yylex();

  context->set_dim(0);

  if ('[' == context->token_to_scan) {
    if (ArrayDimensionSet(context)) {
      context->set_error(INVALID_ARRAY_DIMENSIONS);
      return 1;
    }
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
  context->append_directive(&bds);

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
          return (InitializableDecl(context, storage_class));
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
    if(!FunctionDefinitionPart2(context)){
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
          0                       // directive
        };

        boa.directive = context->symbol_map[name];

        if (context->get_machine() == BrigELarge) {
          boa.type = Brigb64;
        }
        context->append_operand(&boa);
        context->token_to_scan = yylex();
        if (context->token_to_scan == '[') {
          BrigoOffset32_t retOpOffset;
          context->token_to_scan = yylex();
          if (!OffsetAddressableOperandPart2(context, CurrentoOffset, &retOpOffset)) {
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

  if(TOKEN_COMMENT==context->token_to_scan){
    return Comment(context);
  }
  else if(!Directive(context)) {
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
  return 1;
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

int ArrayOperand(Context* context, BrigoOffset32_t* pRetOpOffset) {
  std::string op_name;
  if (context->token_to_scan == '(') {
    if (!ArrayOperandList(context ,pRetOpOffset)) {
      return 0;
    } else {
      context->set_error(MISSING_CLOSING_PARENTHESIS);
      return 1;
    }
  } else {

    if (!Operand(context, pRetOpOffset)) {
      return 0;

    } else {
      context->set_error(MISSING_OPERAND);
      return 1;
    }
  }
  return 1;
}

int ArrayOperand(Context* context) {
  BrigoOffset32_t opOffset = 0;
  return ArrayOperand(context, &opOffset);
}

}  // namespace brig
}  // namespace hsa
