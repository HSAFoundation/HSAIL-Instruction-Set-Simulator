/* Copyright 2012 <MulticorewareInc> */

#include <stdio.h>
#include <string>
#include <iostream>
#include "error_reporter.h"

namespace hsa {
namespace brig {

  ErrorReporter* ErrorReporter::er = NULL;

  ErrorReporter* ErrorReporter::get_instance(void) {
    if (er == NULL)
      er = new ErrorReporter();

    return er;
  }

  void ErrorReporter::report_error(error_code_t ErrorCode,
                                   unsigned int LineNo,
                                   unsigned int ColNo) {
    if (display && ErrorCode) {
      std::cerr << "Error " << ErrorCode;
      std::cerr << " (" <<  LineNo << ", " << ColNo << ") : ";
      std::cerr << translate_error(ErrorCode) << std::endl;
    }
  }

  ErrorReporter::ErrorReporter() {
    display = true;
  }

  ErrorReporter::~ErrorReporter() {
    delete er;
  }
}  // namespace brig
}  // namespace hsa
