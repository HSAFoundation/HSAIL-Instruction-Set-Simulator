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
    void report_error(error_t ErrorCode,
                      unsigned int LineNo,
                      unsigned int ColNo);

  private:
    ErrorReporter();
    ~ErrorReporter();
    static ErrorReporter* er;
    void show_all_error() {}
};
}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_ERROR_REPORTER_H_
