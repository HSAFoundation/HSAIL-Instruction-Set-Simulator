/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_ERROR_REPORTER_INTERFACE_H_
#define INCLUDE_ERROR_REPORTER_INTERFACE_H_

#include <string>

namespace hsa {
namespace brig {

class ErrorReporterInterface {
  public:
    virtual ~ErrorReporterInterface() {}
    // Error codes
    enum error_t { OK = 0,
                   // missing tokens
                   MISSING_INTEGER_CONSTANT,
                   MISSING_SINGLE_CONSTANT,
                   MISSING_DOUBLE_CONSTANT,
                   MISSING_DATA_TYPE,
                   MISSING_OPERAND,
                   MISSING_IDENTIFIER,
                   MISSING_ARGUMENT,
                   MISSING_ARGUMENT_LIST,
                   MISSING_LABEL,
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
                   // missing part
                   MISSING_VERSION_STATEMENT,
                   MISSING_WIDTH_INFO,
                   MISSING_DECLPREFIX,
                   INVALID_FUNCTION_DEFINITION,
                   INVALID_CODEBLOCK,
                   UNKNOWN_ERROR
                 };

    // report an error to error reporter
    virtual void report_error(error_t ErrorCode,
                              unsigned int LineNo,
                              unsigned int ColNo) = 0;

    // enable / disable showing errors to stderr
    void show_error(bool show) {
      display = show;
    }

    // get a description of error
    static std::string translate_error(error_t ErrorCode) {
      switch (ErrorCode) {
        case OK:
          return std::string("No error.\n");
        case MISSING_INTEGER_CONSTANT:
          return std::string("Missing integer constant.\n");
        case MISSING_SINGLE_CONSTANT:
          return std::string("Missing single constant.\n");
        case MISSING_DOUBLE_CONSTANT:
          return std::string("Missing double constant.\n");
        case MISSING_DATA_TYPE:
          return std::string("Missing data type.\n");
        case MISSING_OPERAND:
          return std::string("Missing operand.\n");
        case MISSING_IDENTIFIER:
          return std::string("Missing identifier.\n");
        case MISSING_ARGUMENT:
          return std::string("Missing argument.\n");
        case MISSING_ARGUMENT_LIST:
          return std::string("Missing argument list.\n");
        case MISSING_LABEL:
          return std::string("Missing label.\n");
        case MISSING_COLON:
          return std::string("Missing a colon.\n");
        case MISSING_SEMICOLON:
          return std::string("Missing a semicolon.\n");
        case MISSING_COMMA:
          return std::string("Missing a comma.\n");
        case MISSING_CLOSING_BRACKET:
          return std::string("Missing a closing bracket.\n");
        case MISSING_CLOSING_PARENTHESIS:
          return std::string("Missing a closing parenthesis.\n");
        case MISSING_OPENNING_BRACKET:
          return std::string("Missing an openning bracket.\n");
        case INVALID_TARGET:
          return std::string("Invalid target.\n");
        case INVALID_ROUNDING_MODE:
          return std::string("Invalid rounding mode.\n");
        case INVALID_ALIGNMENT:
          return std::string("Invalid alignment.\n");
        case INVALID_ARGUMENT_LIST:
          return std::string("Invalid argument list.\n");
        case INVALID_FBAR:
          return std::string("Invalid fbar.\n");
        case INVALID_NESTED_ARGUMENT_SCOPE:
          return std::string("Argument scope cannot be nested.\n");
        case INVALID_INITIALIZER:
          return std::string("Invalid initializer.\n");
        case MISSING_VERSION_STATEMENT:
          return std::string("Missing version statement.\n");
        case MISSING_WIDTH_INFO:
          return std::string("Missing width information.\n");
        case MISSING_DECLPREFIX:
          return std::string("Missing declPrefix\n" );
        case INVALID_FUNCTION_DEFINITION:
          return std::string("Invalid funcdtion definition\n");
        case INVALID_CODEBLOCK:
          return std::string("Something wrong in codeblock\n");
      case UNKNOWN_ERROR:
        default:
          return std::string("Unknown error. \n");
      }
    }

  protected:
    bool display;
    virtual void show_all_error() = 0;
};
}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_ERROR_REPORTER_INTERFACE_H_
