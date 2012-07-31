/* Copyright 2012 <MulticorewareInc> */
#ifndef INCLUDE_MOCK_ERROR_REPORTER_H_
#define INCLUDE_MOCK_ERROR_REPORTER_H_
#include <string>
#include "gmock/gmock.h"
#include "error_reporter_interface.h"

namespace hsa {
namespace brig {
class MockErrorReporter: public ErrorReporterInterface {
  public:
    MOCK_METHOD2(report_error, void(error_t ErrorCode, unsigned int LineNo));
};


}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_MOCK_ERROR_REPORTER_H_
