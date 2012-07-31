/* Copyright 2012 <MulticorewareInc> */

#ifndef TEST_FAKE_ERROR_REPORTER_H_
#define TEST_FAKE_ERROR_REPORTER_H_

#include <vector>
#include <string>
#include "error_reporter_interface.h"

namespace hsa {
namespace brig {

class FakeErrorReporter: public ErrorReporterInterface {
  public:
    void report_error(error_t ErrorCode, unsigned int LineNo) {
      error_buffer.push_back(ErrorCode);
    }

    void show_all_error() {
      std::vector<error_t>::iterator it;
      for (it = error_buffer.begin();
           it < error_buffer.end();
           it++)
        printf("Error history: %d \n", *it);
    }
  private:
    std::vector<error_t> error_buffer;
};
}  // namespace brig
}  // namespace hsa
#endif  // TEST_FAKE_ERROR_REPORTER_H_
