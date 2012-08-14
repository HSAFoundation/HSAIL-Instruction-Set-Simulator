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
    MOCK_METHOD3(report_error, void(error_code_t ErrorCode,
                                    unsigned int LineNo,
                                    unsigned int ColNo));
    MOCK_METHOD0(show_all_error, void());

    MOCK_METHOD0(get_number_of_errors, unsigned int());
    MOCK_METHOD1(get_error_at, error_code_t(unsigned int index));
    MOCK_METHOD0(get_last_error, error_code_t());

    void DelegateToFake() {
      ON_CALL(*this, report_error(_, _, _))
        .WillByDefault(Invoke(&fake, &FakeErrorReporter::report_error));
      ON_CALL(*this, show_all_error())
        .WillByDefault(Invoke(&fake, &FakeErrorReporter::show_all_error));
      ON_CALL(*this, get_number_of_errors())
        .WillByDefault(Invoke(&fake, &FakeErrorReporter::get_number_of_errors));
      ON_CALL(*this, get_error_at(_))
        .WillByDefault(Invoke(&fake, &FakeErrorReporter::get_error_at));
      ON_CALL(*this, get_last_error())
        .WillByDefault(Invoke(&fake, &FakeErrorReporter::get_last_error));
    };

  private:
    FakeErrorReporter fake;
};


}  // namespace brig
}  // namespace hsa
#endif  // TEST_MOCK_ERROR_REPORTER_H_
