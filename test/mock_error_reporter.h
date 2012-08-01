/* Copyright 2012 <MulticorewareInc> */
#ifndef TEST_MOCK_ERROR_REPORTER_H_
#define TEST_MOCK_ERROR_REPORTER_H_
#include <string>
#include "gmock/gmock.h"
#include "error_reporter_interface.h"
#include "fake_error_reporter.h"

using ::testing::_;
using ::testing::Invoke;

namespace hsa {
namespace brig {
class MockErrorReporter: public ErrorReporterInterface {
  public:
    MOCK_METHOD3(report_error, void(error_t ErrorCode,
                                    unsigned int LineNo,
                                    unsigned int ColNo));
    MOCK_METHOD0(show_all_error, void());



    void DelegateToFake() {
      ON_CALL(*this, report_error(_, _, _))
        .WillByDefault(Invoke(&fake, &FakeErrorReporter::report_error));
      ON_CALL(*this, show_all_error())
        .WillByDefault(Invoke(&fake, &FakeErrorReporter::show_all_error));
    };

  private:
    FakeErrorReporter fake;
};


}  // namespace brig
}  // namespace hsa
#endif  // TEST_MOCK_ERROR_REPORTER_H_
