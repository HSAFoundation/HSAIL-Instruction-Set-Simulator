//===- brig_reader.cc -----------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "brig_reader.h"

#include "llvm/Object/ELF.h"
#include "llvm/Object/ObjectFile.h"
#include "llvm/Support/MemoryBuffer.h"

namespace hsa {
namespace brig {

BrigReader::~BrigReader() { delete objFile_; }

BrigReader *BrigReader::createBrigReader(const char *filename) {
  llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> file =
    llvm::MemoryBuffer::getFile(filename);
  if (file.getError()) return NULL;
  llvm::ErrorOr<llvm::object::ObjectFile *> objFile =
    llvm::object::ObjectFile::createELFObjectFile(*file);

  BrigReader *reader = createBrigReader(*objFile);
  if (!reader) delete *objFile;
  return reader;
}

BrigReader *BrigReader::createBrigReader(const char *buffer, size_t size) {
  llvm::StringRef bufRef(buffer, size);
  std::unique_ptr<llvm::MemoryBuffer> file(
    llvm::MemoryBuffer::getMemBuffer(bufRef, "", false));
  llvm::ErrorOr<llvm::object::ObjectFile *> objFile =
    llvm::object::ObjectFile::createELFObjectFile(file);

  BrigReader *reader = createBrigReader(*objFile);
  if (!reader) delete *objFile;
  return reader;
}

BrigReader *BrigReader::createBrigReader(llvm::object::ObjectFile *objFile) {
  llvm::StringRef directives;
  llvm::StringRef code;
  llvm::StringRef operands;
  llvm::StringRef debug;
  llvm::StringRef strings;

  typedef llvm::object::section_iterator SecIt;
  const SecIt E = objFile->section_end();
  for (SecIt it = objFile->section_begin(); it != E; ++it) {
    llvm::StringRef name;
    it->getName(name);

    if (name == ".brig_directives" || name == ".directives") {
      if (it->getContents(directives)) return NULL;
    } else if (name == ".brig_code" || name == ".code") {
      if (it->getContents(code)) return NULL;
    } else if (name == ".brig_operands" || name == ".operands") {
      if (it->getContents(operands)) return NULL;
    } else if (name == ".brig_debug" || name == ".debug") {
      if (it->getContents(debug)) return NULL;
    } else if (name == ".brig_strtab" || name == ".strtab" ||
              name == ".strings") {
      if (it->getContents(strings)) return NULL;
    }
  }

  if (!directives.size()) return NULL;
  if (!code.size()) return NULL;
  if (!operands.size()) return NULL;
  if (!strings.size()) return NULL;

  return new BrigReader(objFile, directives, code, operands, debug, strings);
}

}  // namespace brig
}  // namespace hsa
