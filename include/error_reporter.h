/* Copyright 2012 <MulticorewareInc> */

#ifndef INCLUDE_ERROR_REPORTER_H_
#define INCLUDE_ERROR_REPORTER_H_

#include <string>
#include "error_reporter_interface.h"

namespace hsa {
namespace brig {

class ErrorReporter: public ErrorReporterInterface {
  public:
    // get an instance of error reporter
    static ErrorReporter* get_instance(void);

    // report an error to error reporter
    void report_error(error_t ErrorCode, unsigned int LineNo);

    // get a description of error
    static std::string translate_error(error_t ErrorCode);

  private:
    ErrorReporter();
    ~ErrorReporter();
    static ErrorReporter* er;
};
}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_ERROR_REPORTER_H_
