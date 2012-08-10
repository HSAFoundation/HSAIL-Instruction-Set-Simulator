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

                   MISSING_IDENTIFIER,
                   MISSING_LABEL,
                   MISSING_SREGISTER,

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
                   // missing part
                   MISSING_VERSION_STATEMENT,
                   MISSING_WIDTH_INFO,
                   MISSING_DECLPREFIX,
                   MISSING_STRING,
                   MISSING_ARGUMENT,
                   MISSING_ARGUMENT_LIST,
                   MISSING_OPERAND,

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
          return std::string("A S-register required");


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

      case UNKNOWN_ERROR:
        default:
          return std::string("Unknown error.");
      }
    }

  protected:
    bool display;
    virtual void show_all_error() = 0;
};
}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_ERROR_REPORTER_INTERFACE_H_
