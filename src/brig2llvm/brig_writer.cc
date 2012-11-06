#include "brig_writer.h"

#include "llvm/Object/ELF.h"
#include "llvm/Support/raw_ostream.h"

namespace hsa {
namespace brig {

const char BrigWriter::Sections[] =
  "\0"
  ".directives\0"
  ".code\0"
  ".operands\0"
  ".debug\0"
  ".strings\0"
  ".shstrtab";

const llvm::StringRef BrigWriter::shstrtab(Sections, sizeof(Sections));

bool BrigWriter::write(const char *filename,
                       const llvm::StringRef &directives,
                       const llvm::StringRef &code,
                       const llvm::StringRef &operands,
                       const llvm::StringRef &debug,
                       const llvm::StringRef &strings) {
  std::string errMsg;
  llvm::raw_fd_ostream out(filename, errMsg, llvm::raw_fd_ostream::F_Binary);
  if(!errMsg.empty())
    return false;

  bool success = write(out, directives, code, operands, debug, strings);

  out.close();

  return !out.has_error() && success;
}

bool BrigWriter::write(llvm::raw_fd_ostream &out,
                       const llvm::StringRef &directives,
                       const llvm::StringRef &code,
                       const llvm::StringRef &operands,
                       const llvm::StringRef &debug,
                       const llvm::StringRef &strings) {

  out.seek(sizeof(llvm::ELF::Elf32_Ehdr));

  size_t dirOffset      = writeSection(out, directives);
  size_t codeOffset     = writeSection(out, code);
  size_t operandsOffset = writeSection(out, operands);
  size_t debugOffset    = writeSection(out, debug);
  size_t stringsOffset  = writeSection(out, strings);
  size_t shstrtabOffset = writeSection(out, shstrtab);

  align(out);
  uint32_t secHeaderOffset = (uint32_t) out.tell();
  for(size_t i = 0; i < sizeof(llvm::ELF::Elf32_Shdr); ++i) out.write(0);

  writeSecHeader(out, ".directives",
                 llvm::ELF::SHT_PROGBITS, dirOffset, directives.size());
  writeSecHeader(out, ".code",
                 llvm::ELF::SHT_PROGBITS, codeOffset, code.size());
  writeSecHeader(out, ".operands",
                 llvm::ELF::SHT_PROGBITS, operandsOffset, operands.size());
  writeSecHeader(out, ".debug",
                 llvm::ELF::SHT_PROGBITS, debugOffset, debug.size());
  writeSecHeader(out, ".strings",
                 llvm::ELF::SHT_PROGBITS, stringsOffset, strings.size());
  writeSecHeader(out, ".shstrtab",
                 llvm::ELF::SHT_STRTAB, shstrtabOffset, shstrtab.size());

  out.seek(0);

  llvm::ELF::Elf32_Ehdr header = {
    { llvm::ELF::ElfMagic[0],
      llvm::ELF::ElfMagic[1], llvm::ELF::ElfMagic[2], llvm::ELF::ElfMagic[3],
      llvm::ELF::ELFCLASS32,
      llvm::ELF::ELFDATA2LSB,
      llvm::ELF::EV_CURRENT,
      llvm::ELF::ELFOSABI_NONE,
      llvm::ELF::EI_PAD
    },
    llvm::ELF::ET_NONE,
    llvm::ELF::EM_NONE,
    llvm::ELF::EV_CURRENT,
    0, // e_entry
    0, // e_phoff
    secHeaderOffset, // e_shoff
    0, // e_flags
    sizeof(header), // e_ehsize
    0, // e_phentsize
    0, // e_phnum
    sizeof(llvm::ELF::Elf32_Shdr), // e_shentsize
    NumSecEntries, // e_shnum
    NumSecEntries - 1 // e_shstrndx
  };
  out.write(reinterpret_cast<const char *>(&header), sizeof(header));

  return !out.has_error();
}

void BrigWriter::align(llvm::raw_fd_ostream &out) {
  // HSA PRM 19.1.1: All sections must start on a memory address that is a
  // multiple of 16. This allows Brig structures to be naturally aligned.
  while(!out.tell() % 16) out.write(0);
}

size_t BrigWriter::writeSection(llvm::raw_fd_ostream &out,
                                const llvm::StringRef &section) {
  align(out);
  size_t offset = out.tell();
  out.write(section.data(), section.size());
  return offset;
}

void BrigWriter::writeSecHeader(llvm::raw_fd_ostream &out,
                                const char *name,
                                uint32_t sh_type,
                                size_t offset,
                                size_t size) {
  llvm::ELF::Elf32_Shdr secHeader = {
    (uint32_t) shstrtab.find(name),
    sh_type, // sh_type
    0, // sh_flags
    0, // sh_addr
    offset,
    size,
    0, // sh_link
    0, // sh_info
    sizeof(uint8_t), // sh_addralign
    0  // sh_entsize
  };
  out.write(reinterpret_cast<const char *>(&secHeader), sizeof(secHeader));
}

} // namespace brig
} // namespace hsa
