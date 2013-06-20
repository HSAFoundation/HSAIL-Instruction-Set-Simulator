//===- hsailasm_wrapper.cc ------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "hsailasm_wrapper.h"

#include "llvm/ADT/OwningPtr.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/system_error.h"

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
  llvm::error_code ec =
    llvm::sys::fs::unique_file("temp-%%%%.hsail", result_fd, sourceFile);
  close(result_fd);
  check(!ec, ec.message());

  std::string outErrMsg;
  llvm::raw_fd_ostream out(sourceFile.c_str(), outErrMsg,
                           llvm::raw_fd_ostream::F_Binary);
  check(!outErrMsg.size(), outErrMsg);

  out << sourceCode;
  out.close();
  check(!out.has_error(), "Error writing HSAIL");

  bool result =
    assembleHSAILSource(sourceFile.c_str(), outputFile, errMsg, enableDebug);

  bool existed;
  llvm::sys::fs::remove(sourceFile.c_str(), existed);

  return result && existed;
}

bool HsailAsm::assembleHSAILSource(const char *sourceFile,
                                   const char *outputFile,
                                   std::string *errMsg,
                                   bool enableDebug) {

  const char *args[] = { "hsailasm", sourceFile, "-o", outputFile,
                         enableDebug ? "-g" : NULL, NULL };

  llvm::sys::Path programPath(XSTR(BIN_PATH) "/hsailasm");

  check(programPath.isRegularFile(), "Cannot find hsailasm");
  check(programPath.canExecute(), "Cannot execute hsailasm");

  int result_fd;
  llvm::SmallString<128> resultPath;
  llvm::error_code ec =
    llvm::sys::fs::unique_file("hsailasm-%%%%.log", result_fd, resultPath);
  close(result_fd);
  check(!ec, ec.message());

  llvm::sys::Path errFile(resultPath);

  const llvm::sys::Path *redirects[] = { &errFile, &errFile, 0 };

  int result = llvm::sys::Program::ExecuteAndWait(programPath,
                                                  args,
                                                  NULL,
                                                  redirects,
                                                  0,
                                                  0,
                                                  errMsg);

  if (errMsg && errMsg->size() > 0) {
    errFile.eraseFromDisk();
    return false;
  }

  if (result) {
    if (errMsg) {
      llvm::OwningPtr<llvm::MemoryBuffer> result;
      llvm::error_code error =
        llvm::MemoryBuffer::getFile(errFile.c_str(), result);
      *errMsg = error == llvm::error_code::success() ?
        std::string(result->getBuffer()) :
        error.message();
    }
    errFile.eraseFromDisk();
    return false;
  }

  llvm::sys::Path output(outputFile);
  check(output.isRegularFile(), "Missing output");

  errFile.eraseFromDisk();

  return true;
}

}  // namespace brig
}  // namespace hsa
