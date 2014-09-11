//===- hsailasm_wrapper.cc ------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "hsailasm_wrapper.h"

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/Program.h"
#include "llvm/Support/raw_ostream.h"

#include <unistd.h>

#define STR(X) #X
#define XSTR(X) STR(X)

namespace hsa {
namespace brig {

#define check(TEST,MESSAGE) do {                \
    if (!TEST) {                                \
      if (errMsg) *errMsg = MESSAGE;            \
      return false;                             \
    }                                           \
  } while (0)

bool HsailAsm::assembleHSAILString(const char *sourceCode,
                                   const char *outputFile,
                                   std::string *errMsg,
                                   bool enableDebug) {

  int result_fd;
  llvm::SmallString<128> sourceFile;
  {
    std::error_code ec =
      llvm::sys::fs::createUniqueFile("temp-%%%%.hsail", result_fd, sourceFile);
    close(result_fd);
    check(!ec, ec.message());
  }

  std::string outErrMsg;
  llvm::raw_fd_ostream out(sourceFile.c_str(), outErrMsg,
                           llvm::sys::fs::F_Text);
  check(!outErrMsg.size(), outErrMsg);

  out << sourceCode;
  out.close();
  check(!out.has_error(), "Error writing HSAIL");

  bool result =
    assembleHSAILSource(sourceFile.c_str(), outputFile, errMsg, enableDebug);

  std::error_code ec = llvm::sys::fs::remove(sourceFile.c_str(), false);
  return result && !ec;
}

bool HsailAsm::assembleHSAILSource(const char *sourceFile,
                                   const char *outputFile,
                                   std::string *errMsg,
                                   bool enableDebug) {

  const char *args[] = { "hsailasm", sourceFile, "-o", outputFile,
                         enableDebug ? "-g" : NULL, NULL };

  llvm::StringRef programPath(XSTR(BIN_PATH) "/hsailasm");

  check(llvm::sys::fs::is_regular_file(programPath), "Cannot find hsailasm");
  check(llvm::sys::fs::can_execute(programPath), "Cannot execute hsailasm");

  int result_fd;
  llvm::SmallString<128> resultPath;
  std::error_code ec =
    llvm::sys::fs::createUniqueFile("hsailasm-%%%%.log", result_fd, resultPath);
  close(result_fd);
  check(!ec, ec.message());

  llvm::StringRef errFile(resultPath);

  const llvm::StringRef *redirects[] = { &errFile, &errFile, 0 };

  int result = llvm::sys::ExecuteAndWait(programPath,
                                         args,
                                         NULL,
                                         redirects,
                                         0,
                                         0,
                                         errMsg);
  if (errMsg && errMsg->size() > 0) {
    llvm::sys::fs::remove(errFile);
    return false;
  }

  if (result) {
    if (errMsg) {
      llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> result =
        llvm::MemoryBuffer::getFile(errFile);
      *errMsg = !result.getError() ?
        std::string((*result)->getBuffer()) :
        result.getError().message();
    }
    llvm::sys::fs::remove(errFile);
    return false;
  }

  llvm::StringRef output(outputFile);
  check(llvm::sys::fs::is_regular_file(output), "Missing output");

  llvm::sys::fs::remove(errFile);

  return true;
}

}  // namespace brig
}  // namespace hsa
