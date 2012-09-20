#ifndef BRIG_WRITER_H
#define BRIG_WRITER_H

#include "llvm/Support/raw_ostream.h"

namespace hsa {
namespace brig {

class BrigWriter {

 public:
  static bool write(const char *filename,
                    const llvm::StringRef &directives,
                    const llvm::StringRef &code,
                    const llvm::StringRef &operands,
                    const llvm::StringRef &debug,
                    const llvm::StringRef &strings);

  static bool write(llvm::raw_fd_ostream &out,
                    const llvm::StringRef &directives,
                    const llvm::StringRef &code,
                    const llvm::StringRef &operands,
                    const llvm::StringRef &debug,
                    const llvm::StringRef &strings);

 private:

  static const unsigned NumSecEntries = 6;
  static const char Sections[];
  static const llvm::StringRef shstrtab;

  BrigWriter() {}

  static void align(llvm::raw_fd_ostream &out);

  static size_t writeSection(llvm::raw_fd_ostream &out,
                             const llvm::StringRef &section);

  static void writeSecHeader(llvm::raw_fd_ostream &out,
                             const char *name,
                             uint32_t sh_type,
                             size_t offset,
                             size_t size);
};

} // namespace brig
} // namespace hsa

#endif // BRIG_WRITER_H
