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
    static std::string translate_error(error_t ErrorCode);

  protected:
    bool display;
    virtual void show_all_error() = 0;
};
}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_ERROR_REPORTER_INTERFACE_H_
