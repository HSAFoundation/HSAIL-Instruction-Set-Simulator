//===- brig_validate.cc ---------------------------------------------------===//
//
//                     The HSA Simultor
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//


#include "brig_reader.h"
#include "brig_module.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>

int main(int argc, char **argv) {

  if(argc < 2) {
    std::cerr << argv[0] << ": Missing input!\n";
    return 1;
  }

  hsa::brig::BrigReader *reader =
    hsa::brig::BrigReader::createBrigReader(argv[1]);
  hsa::brig::BrigModule mod(*reader, &llvm::errs());
  if(!mod.isValid())
    std::cerr << argv[0] << ": Input is invalid!\n";

  return 0;
}
