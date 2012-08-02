/* Copyright 2012 <MulticorewareInc> */

#include <stdio.h>
#include <string>
#include "error_reporter.h"

namespace hsa {
namespace brig {

  ErrorReporter* ErrorReporter::er = NULL;

  ErrorReporter* ErrorReporter::get_instance(void) {
    if (er == NULL)
      er = new ErrorReporter();

    return er;
  };

  void ErrorReporter::report_error(error_t ErrorCode,
                                   unsigned int LineNo,
                                   unsigned int ColNo) {
    if (display && ErrorCode)
      fprintf(stderr,
              "Line %d, Column %d : Error %d : %s \n",
              LineNo,
              ColNo,
              ErrorCode,
              translate_error(ErrorCode).c_str());
  };

  std::string ErrorReporter::translate_error(error_t ErrorCode) {
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
        return std::string("Mission width information.\n");
    case UNKNOWN_ERROR:
      default:
        return std::string("Unknown error. \n");
    }
  };

  ErrorReporter::ErrorReporter() {
    display = true;
  }

  ErrorReporter::~ErrorReporter() {
    delete er;
  }
}  // namespace brig
}  // namespace hsa
