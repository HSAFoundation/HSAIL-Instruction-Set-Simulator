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

  void ErrorReporter::report_error(error_t ErrorCode, unsigned int LineNo) {
    if (display)
      fprintf(stderr,
              "%d : Error %d : %s \n",
              LineNo, ErrorCode,
              translate_error(ErrorCode).c_str());
  };

  std::string ErrorReporter::translate_error(error_t ErrorCode) {
    switch (ErrorCode) {
      case OK:
        return std::string("No error.\n");

      default:
        return std::string("Unknown error. \n");
    }
  };

  ErrorReporter::ErrorReporter() {
    display = false;
  }

  ErrorReporter::~ErrorReporter() {
    delete er;
  }
}  // namespace brig
}  // namespace hsa
