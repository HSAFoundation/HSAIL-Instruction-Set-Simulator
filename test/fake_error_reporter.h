/* Copyright 2012 <MulticorewareInc> */

#ifndef TEST_FAKE_ERROR_REPORTER_H_
#define TEST_FAKE_ERROR_REPORTER_H_

#include <vector>
#include <string>
#include <iostream>
#include "error_reporter_interface.h"

namespace hsa {
namespace brig {
struct ErrorInfo_t {
    ErrorReporterInterface::error_t error_code;
    unsigned int line_no;
    unsigned int col_no;
};

class FakeErrorReporter: public ErrorReporterInterface {
  public:
    void report_error(error_t ErrorCode,
                      unsigned int LineNo,
                      unsigned int ColNo) {
      struct ErrorInfo_t error = {
          ErrorCode,
          LineNo,
          ColNo
      };
      error_buffer.push_back(error);
    }

    void show_all_error() {
      std::cout << "ERROR HISTORY: " << std::endl;
      std::vector<struct ErrorInfo_t>::iterator it;
      for (it = error_buffer.begin();
           it < error_buffer.end();
           it++) {
        std::cout << "Error " << it->error_code;
        std::cout << " (" <<  it->line_no << ", " << it->col_no << ") : ";
        std::cout << translate_error(it->error_code) << std::endl;
      }

    }
  private:
    // std::vector<error_t> error_buffer;
    std::vector<struct ErrorInfo_t> error_buffer;

};
}  // namespace brig
}  // namespace hsa
#endif  // TEST_FAKE_ERROR_REPORTER_H_
