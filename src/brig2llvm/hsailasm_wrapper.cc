
#include "hsailasm_wrapper.h"
#include "stdio.h"
namespace hsa {
  namespace brig {

int HsailAsm::assembleHSAILSource( const char* source_file,
                                   const char* output_file,
                                   std::string* ErrMsg) {

  const char* args[5];
  args[0] = "hsailasm";
  args[1] = source_file;
  args[2] = "-o";
  args[3] = output_file; 

  llvm::sys::Path program_path(std::string("./hsailasm"));

  if (program_path.isEmpty()) {
    if (ErrMsg)
      ErrMsg->assign("Cannot find program");
    return -1;
  }

//  printf("Path = %s\n", program_path.c_str());
  int res = llvm::sys::Program::ExecuteAndWait(program_path,
                                            args,
                                            NULL,
                                            NULL,
                                            0,
                                            0,
                                            ErrMsg
                                            );
  return res;
}
  } // namespace brig
}  // namespace hsa
