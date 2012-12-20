/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_ERROR_REPORTER_INTERFACE_H_
#define INCLUDE_ERROR_REPORTER_INTERFACE_H_

#include <string>

namespace hsa {
namespace brig {
  // Error codes
enum error_code_t {
  OK = 0,

  // missing tokens
  MISSING_INTEGER_CONSTANT,
  MISSING_SINGLE_CONSTANT,
  MISSING_DOUBLE_CONSTANT,
  MISSING_DATA_TYPE,
  MISSING_IDENTIFIER,
  MISSING_GLOBAL_IDENTIFIER,
  MISSING_LOCAL_IDENTIFIER,
  MISSING_LABEL,
  MISSING_REGISTER,

  // missing puctuations
  MISSING_COLON,
  MISSING_SEMICOLON,
  MISSING_COMMA,
  MISSING_CLOSING_BRACKET,
  MISSING_CLOSING_PARENTHESIS,
  MISSING_OPENNING_BRACKET,

  // invalid non-terminals
  INVALID_TARGET,
  INVALID_ROUNDING_MODE,
  INVALID_ALIGNMENT,
  INVALID_ARGUMENT_LIST,
  INVALID_FBAR,
  INVALID_NESTED_ARGUMENT_SCOPE,
  INVALID_INITIALIZER,
  INVALID_FUNCTION_DEFINITION,
  INVALID_CODEBLOCK,
  INVALID_OPERAND,
  INVALID_FIRST_OPERAND,
  INVALID_SECOND_OPERAND,
  INVALID_THIRD_OPERAND,
  INVALID_FOURTH_OPERAND,
  INVALID_FIFTH_OPERAND,
  INVALID_LABEL,
  INVALID_LABEL_TARGETS,
  INVALID_INSTRUCTION,
  INVALID_OPERATION,
  INVALID_IMAGE_INIT,
  INVALID_SECTION_ITEM,
  INVALID_PROGRAM,
  INVALID_WIDTH_NUMBER,
  INVALID_DATA_TYPE,
  INVALID_IDENTIFIER,
  INVALID_GLOBAL_DECL,
  INVALID_COMMENT,
  INVALID_PACKING,
  INVALID_ARG_BLOCK,
  INVALID_CALL_ARGS,
  INVALID_ARRAY_DIMENSIONS,
  INVALID_CALL_TARGETS,
  INVALID_CONTROL_TYPE,
  INVALID_SEGMENT_OPERATION,
  INVALID_MEMORY_OPERAND,
  INVALID_MODIFIER,
  // missing part
  MISSING_VERSION_STATEMENT,
  MISSING_WIDTH_INFO,
  MISSING_DECLPREFIX,
  MISSING_STRING,
  MISSING_ARGUMENT,
  MISSING_ARGUMENT_LIST,
  MISSING_OPERAND,
  MISSING_COMPARISON_TYPE,
  MISSING_PROPERTY,
  MISSING_SECTION_ITEM,
  MISSING_BLOCK_TYPE,
  MISSING_ADDRESS,
  MISSING_ADDRESSSPACE_IDENTIFIER,
  MISSING_OPERATION,
  MISSING_FUNCTION_DIRECTIVE,
  MISSING_SEGMENT,

  REPEATED_DECLARATION,

  UNKNOWN_ERROR
};
class ErrorReporterInterface {
  public:
    virtual ~ErrorReporterInterface() {}


    // report an error to error reporter
    virtual void report_error(error_code_t ErrorCode,
                              unsigned int LineNo,
                              unsigned int ColNo) = 0;

    // enable / disable showing errors to stderr
    void show_error(bool show) {
      display = show;
    }

    // get a description of error
    static std::string translate_error(error_code_t ErrorCode) {
      switch (ErrorCode) {
        case OK:
          return std::string("No error.");

        // missing tokens
        case MISSING_INTEGER_CONSTANT:
          return std::string("Missing integer constant.");
        case MISSING_SINGLE_CONSTANT:
          return std::string("Missing single constant.");
        case MISSING_DOUBLE_CONSTANT:
          return std::string("Missing double constant.");
        case MISSING_DATA_TYPE:
          return std::string("Missing data type.");
        case MISSING_IDENTIFIER:
          return std::string("Missing identifier.");
        case MISSING_GLOBAL_IDENTIFIER:
          return std::string("Missing a global identifier.");
        case MISSING_LOCAL_IDENTIFIER:
          return std::string("Missing a local identifier.");
        case MISSING_LABEL:
          return std::string("Missing label.");
        case MISSING_REGISTER:
          return std::string("A register required.");

        // missing puctuations
        case MISSING_COLON:
          return std::string("Missing a colon.");
        case MISSING_SEMICOLON:
          return std::string("Missing a semicolon.");
        case MISSING_COMMA:
          return std::string("Missing a comma.");
        case MISSING_CLOSING_BRACKET:
          return std::string("Missing a closing bracket.");
        case MISSING_CLOSING_PARENTHESIS:
          return std::string("Missing a closing parenthesis.");
        case MISSING_OPENNING_BRACKET:
          return std::string("Missing an openning bracket.");

        case INVALID_TARGET:
          return std::string("Invalid target.");
        case INVALID_ROUNDING_MODE:
          return std::string("Invalid rounding mode.");
        case INVALID_ALIGNMENT:
          return std::string("Invalid alignment.");
        case INVALID_ARGUMENT_LIST:
          return std::string("Invalid argument list.");
        case INVALID_FBAR:
          return std::string("Invalid fbar.");
        case INVALID_NESTED_ARGUMENT_SCOPE:
          return std::string("Argument scope cannot be nested.");
        case INVALID_INITIALIZER:
          return std::string("Invalid initializer.");
        case INVALID_FUNCTION_DEFINITION:
          return std::string("Invalid funcdtion definition.");
        case INVALID_CODEBLOCK:
          return std::string("Something wrong in codeblock.");
        case INVALID_OPERAND:
          return std::string("Invalid operand.");
        case INVALID_FIRST_OPERAND:
          return std::string("First operand is invalid.");
        case INVALID_SECOND_OPERAND:
          return std::string("Second operand is invalid.");
        case INVALID_THIRD_OPERAND:
          return std::string("Third operand is invalid.");
        case INVALID_FOURTH_OPERAND:
          return std::string("Fourth operand is invalid.");
        case INVALID_FIFTH_OPERAND:
          return std::string("Fifth operand is invalid.");
        case INVALID_LABEL:
          return std::string("Invalid label.");
        case INVALID_LABEL_TARGETS:
          return std::string("Invalid label targets.");
        case INVALID_INSTRUCTION:
          return std::string("Invalid instruction.");
        case INVALID_OPERATION:
          return std::string("Invalid operation.");
        case INVALID_IMAGE_INIT:
          return std::string("Invalid image initializer.");
        case INVALID_SECTION_ITEM:
          return std::string("Invalid item.");
        case INVALID_PROGRAM:
          return std::string("Invalid program.");
        case INVALID_WIDTH_NUMBER:
          return std::string("Invalid width number(1<= n <=1024 and n being a power of 2).");
        case INVALID_DATA_TYPE:
          return std::string("Invalid Data Type.");
        case INVALID_IDENTIFIER:
          return std::string("Invalid identifier.");
        case INVALID_GLOBAL_DECL:
          return std::string("Invalid global declaration.");
        case INVALID_COMMENT:
          return std::string("Invalid comment.");
        case INVALID_PACKING:
          return std::string("Invalid packing.");
        case INVALID_ARG_BLOCK:
          return std::string("Invalid Arg Block.");
        case INVALID_CALL_ARGS:
          return std::string("Invalid Call Args.");
        case INVALID_CALL_TARGETS:
          return std::string("Invalid Call Targets.");
        case INVALID_ARRAY_DIMENSIONS:
          return std::string("Invalid Array Dimensions");
        case INVALID_CONTROL_TYPE:
          return std::string("Invalid Control type.");
        case INVALID_SEGMENT_OPERATION:
          return std::string("Invalid Segment Operation: The instruction type and operand registers must match the machine model.");
        case INVALID_MODIFIER:
          return std::string("Invalid modifier.");
        case INVALID_MEMORY_OPERAND:
          return std::string("Invalid Memory Operand.");
        case MISSING_VERSION_STATEMENT:
          return std::string("Missing version statement.");
        case MISSING_WIDTH_INFO:
          return std::string("Missing width information.");
        case MISSING_DECLPREFIX:
          return std::string("Missing declPrefix." );
        case MISSING_STRING:
          return std::string("A string expected.");
        case MISSING_ARGUMENT:
          return std::string("Missing argument.");
        case MISSING_ARGUMENT_LIST:
          return std::string("Missing argument list.");
        case MISSING_OPERAND:
          return std::string("Missing operand.");
        case MISSING_COMPARISON_TYPE:
          return std::string("Missing type of comparison.");
        case MISSING_PROPERTY:
          return std::string("Missing property.");
        case MISSING_SECTION_ITEM:
          return std::string("Missing block items.");
        case MISSING_BLOCK_TYPE:
          return std::string("Missing block type.");
        case MISSING_ADDRESS:
          return std::string("Missing Address.");
        case MISSING_ADDRESSSPACE_IDENTIFIER:
          return std::string("Missing Address Space identifier.");
        case MISSING_OPERATION:
          return std::string("Missing operation");
        case MISSING_FUNCTION_DIRECTIVE:
          return std::string("Invalid argument space. No function/kernel specified");
        case MISSING_SEGMENT:
          return std::string("Missing segment information for segment operation.");
        case REPEATED_DECLARATION:
          return std::string("Repeated declaration.");

        case UNKNOWN_ERROR:
        default:
          return std::string("Unknown error.");
      }
    }

  protected:
    bool display;
    virtual unsigned int get_number_of_errors() = 0;
    virtual void show_all_error() = 0;
    virtual error_code_t get_error_at(unsigned int index) = 0;
    virtual error_code_t get_last_error() = 0;
};
}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_ERROR_REPORTER_INTERFACE_H_
