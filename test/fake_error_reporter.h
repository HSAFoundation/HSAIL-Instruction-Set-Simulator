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
    error_code_t error_code;
    unsigned int line_no;
    unsigned int col_no;
};

class FakeErrorReporter: public ErrorReporterInterface {
  public:
    FakeErrorReporter() {
      no_errors = 0;
    }
    void report_error(error_code_t ErrorCode,
                      unsigned int LineNo,
                      unsigned int ColNo) {
      struct ErrorInfo_t error = {
          ErrorCode,
          LineNo,
          ColNo
      };
      error_buffer.push_back(error);
      no_errors++;
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

    unsigned int get_number_of_errors() {
      return no_errors;
    }

    error_code_t get_error_at(unsigned int index) {
      if ((index >= 0) && (index <= no_errors)) {
        return (error_buffer[index].error_code);
      } else {
        std::cout << "Index exceeded total number of errors." << std::endl;
        return UNKNOWN_ERROR;
      }
    }

    error_code_t get_last_error() {
      return (error_buffer[no_errors-1].error_code);
    }



  private:
    std::vector<struct ErrorInfo_t> error_buffer;
    unsigned int no_errors;
};
}  // namespace brig
}  // namespace hsa
#endif  // TEST_FAKE_ERROR_REPORTER_H_
