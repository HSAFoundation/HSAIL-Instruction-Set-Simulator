/* Copyright 2013 <MulticorewareInc> */
#ifndef INCLUDE_HSAILASM_WRAPPER_H_
#define INCLUDE_HSAILASM_WRAPPER_H_

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Program.h"
#include "llvm/Support/PathV1.h"

#include <string>

namespace llvm {
  class StringRef;
  class Path;
  class Program;
}


namespace hsa {
  namespace brig {
    
class HsailAsm {
  public:
    static int assembleHSAILSource(const char* source_file,
                                   const char* output_file,
                                   std::string* ErrMsg = NULL);
};
  }  // namespace brig
}  // namespace hsa

#endif  // INCLUDE_HSAILASM_WRAPPER_H_
