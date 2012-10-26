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
    void report_error(error_code_t ErrorCode,
                      unsigned int LineNo,
                      unsigned int ColNo);

    virtual ~ErrorReporter() {}
  private:
    ErrorReporter();
    static ErrorReporter* er;

    // these methods are not implemented
    unsigned int get_number_of_errors() {
      return 0;
    }
    void show_all_error() {}
    error_code_t get_error_at(unsigned int index) {
      return UNKNOWN_ERROR;
    }

    error_code_t get_last_error() {
      return UNKNOWN_ERROR;
    }
};
}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_ERROR_REPORTER_H_
