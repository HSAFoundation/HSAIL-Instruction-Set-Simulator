#include "llvm/ADT/OwningPtr.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/system_error.h"

#include "hsailasm_wrapper.h"

#define STR(X) #X
#define XSTR(X) STR(X)

namespace hsa {
namespace brig {

bool HsailAsm::assembleHSAILSource(const char *sourceFile,
                                  const char *outputFile,
                                  std::string *errMsg) {

  const char *args[5];
  args[0] = "hsailasm";
  args[1] = sourceFile;
  args[2] = "-o";
  args[3] = outputFile;
  args[4] = NULL;

  llvm::sys::Path programPath(XSTR(BIN_PATH) "/hsailasm");

  if(!programPath.isRegularFile()) {
    if(errMsg)
      *errMsg = "Cannot find hsailasm";
    return false;
  }

  if(!programPath.canExecute()) {
    if(errMsg)
      *errMsg = "Cannot execute hsailasm";
    return false;
  }

  llvm::sys::Path errFile("hsailasm.log");
  if(errFile.createTemporaryFileOnDisk(true, errMsg))
    return false;

  const llvm::sys::Path *redirects[] = { &errFile, &errFile, 0 };

  int result = llvm::sys::Program::ExecuteAndWait(programPath,
                                                  args,
                                                  NULL,
                                                  redirects,
                                                  0,
                                                  0,
                                                  errMsg);

  if(errMsg && errMsg->size() > 0)
    return false;

  if(result) {
    if(errMsg) {
      llvm::OwningPtr<llvm::MemoryBuffer> result;
      llvm::error_code error =
        llvm::MemoryBuffer::getFile(errFile.c_str(), result);
      *errMsg = error == llvm::error_code::success() ?
        std::string(result->getBuffer()) :
        error.message();
    }
    return false;
  }

  llvm::sys::Path output(outputFile);
  if(!output.isRegularFile()) {
    if(errMsg)
      *errMsg = "Missing output";
    return false;
  }

  return true;
}

} // namespace brig
} // namespace hsa
