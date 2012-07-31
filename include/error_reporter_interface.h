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
                   MISSING_INTEGER_CONSTANT,
                   MISSING_COLON,
                   MISSING_SEMICOLON,
                   INVALID_TARGET
                 };

    // report an error to error reporter
    virtual void report_error(error_t ErrorCode, unsigned int LineNo) = 0;

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
