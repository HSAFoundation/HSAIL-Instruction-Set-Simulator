//===- hsailasm_wrapper.h -------------------------------------------------===//
//
//                     The HSA Simultor
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

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

  static bool assembleHSAILString(const char *source,
                                  const char *outputFile,
                                  std::string *ErrMsg = NULL);

  static bool assembleHSAILSource(const char *sourceFile,
                                  const char *outputFile,
                                  std::string *ErrMsg = NULL);
};

}  // namespace brig
}  // namespace hsa

#endif  // INCLUDE_HSAILASM_WRAPPER_H_
