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
  MISSING_INTEGER_CONSTANT = 1,
  MISSING_SINGLE_CONSTANT = 2,
  MISSING_DOUBLE_CONSTANT = 3,
  MISSING_DATA_TYPE = 4,

  MISSING_IDENTIFIER = 5,
  MISSING_LABEL = 6,
  MISSING_SREGISTER = 7,

  // missing puctuations
  MISSING_COLON = 8,
  MISSING_SEMICOLON = 9,
  MISSING_COMMA = 10,
  MISSING_CLOSING_BRACKET = 11,
  MISSING_CLOSING_PARENTHESIS = 12,
  MISSING_OPENNING_BRACKET = 13,

  // invalid non-terminals
  INVALID_TARGET = 14,
  INVALID_ROUNDING_MODE = 15,
  INVALID_ALIGNMENT = 16,
  INVALID_ARGUMENT_LIST = 17,
  INVALID_FBAR = 18,
  INVALID_NESTED_ARGUMENT_SCOPE = 19,
  INVALID_INITIALIZER = 20,
  INVALID_FUNCTION_DEFINITION = 21,
  INVALID_CODEBLOCK = 22,
  INVALID_OPERAND = 23,
  INVALID_FIRST_OPERAND = 24,
  INVALID_SECOND_OPERAND = 25,
  INVALID_THIRD_OPERAND = 26,
  INVALID_FOURTH_OPERAND = 27,
  INVALID_FIFTH_OPERAND = 28,

  // missing part
  MISSING_VERSION_STATEMENT = 29,
  MISSING_WIDTH_INFO = 30,
  MISSING_DECLPREFIX = 31,
  MISSING_STRING = 32,
  MISSING_ARGUMENT = 33,
  MISSING_ARGUMENT_LIST = 34,
  MISSING_OPERAND = 35,
  MISSING_COMPARISON_TYPE = 36,

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
        case MISSING_STRING:
          return std::string("A string expected.");
        case MISSING_LABEL:
          return std::string("Missing label.");
        case MISSING_SREGISTER:
          return std::string("A S-register required.");


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



        case MISSING_VERSION_STATEMENT:
          return std::string("Missing version statement.");
        case MISSING_WIDTH_INFO:
          return std::string("Missing width information.");
        case MISSING_DECLPREFIX:
          return std::string("Missing declPrefix." );
        case MISSING_ARGUMENT:
          return std::string("Missing argument.");
        case MISSING_ARGUMENT_LIST:
          return std::string("Missing argument list.");
        case MISSING_OPERAND:
          return std::string("Missing operand.");
        case MISSING_COMPARISON_TYPE:
          return std::string("Missing type of comparison.");

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
