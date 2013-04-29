#include "brig_reader.h"
#include "brig_writer.h"
#include "error_reporter.h"
#include "parser_wrapper.h"

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_ostream.h"

static void check(llvm::error_code ec,
           const char *program,
           const char *msg) {
  if(!ec) return;
  llvm::errs() << program << ": " << msg << ": " << ec.message() << ".\n";
  exit(-1);
}

static const char *getInputFile(int argc, char **argv) {
  for(int i = 1; i < argc; ++i) {
    if(!strcmp(argv[i], "-o")) {
      ++i;
    } else {
      return argv[i];
    }
  }
  return NULL;
}

static const char *getOutputFile(int argc, char **argv) {
  for(int i = 1; i < argc - 1; ++i) {
    if(!strcmp(argv[i], "-o"))
      return argv[i + 1];
  }
  return NULL;
}

int main(int argc, char **argv) {

  const char *inputFile = getInputFile(argc, argv);
  if(!inputFile) {
    llvm::errs() << argv[0] << ": No input files.\n";
    exit(-1);
  }

  llvm::OwningPtr<llvm::MemoryBuffer> file;
  llvm::error_code readEC = llvm::MemoryBuffer::getFile(inputFile, file);
  check(readEC, argv[0], "error reading file");

  hsa::brig::Parser parser(file->getBufferStart());
  hsa::brig::Context *context = parser.get_context();
  context->set_error_reporter(hsa::brig::ErrorReporter::get_instance());
  int res = parser.parse();
  if(res) {
    llvm::errs() << argv[0] << ": Compilation halted due to parsing errors.\n";
    exit(-1);
  }

  BrigcOffset32_t codeSize = context->get_code_offset();
  BrigdOffset32_t dirSize  = context->get_directive_offset();
  BrigoOffset32_t operSize = context->get_operand_offset();
  BrigsOffset32_t strSize  = context->get_string_offset();

  llvm::OwningArrayPtr<char> code(new char[codeSize]);
  llvm::OwningArrayPtr<char> directives(new char[dirSize]);
  llvm::OwningArrayPtr<char> operands(new char[operSize]);
  llvm::OwningArrayPtr<char> strings(new char[strSize]);

  context->get_code_bytes(code.get(), 0, codeSize);
  context->get_directive_bytes(directives.get(), 0, dirSize);
  context->get_operand_bytes(operands.get(), 0, operSize);
  context->get_string_bytes(strings.get(), 0, strSize);

  const char *outputFile = getOutputFile(argc, argv);
  if(!outputFile) {
    llvm::errs() << argv[0] << ": No output file.\n";
    exit(-1);
  }

  std::string errMsg;
  llvm::raw_fd_ostream out(outputFile, errMsg, llvm::raw_fd_ostream::F_Binary);

  bool result =
    hsa::brig::BrigWriter::write(out,
                                 llvm::StringRef(directives.get(), dirSize),
                                 llvm::StringRef(code.get(), codeSize),
                                 llvm::StringRef(operands.get(), operSize),
                                 llvm::StringRef("", 0),
                                 llvm::StringRef(strings.get(), strSize));
  if(!result) {
    llvm::errs() << argv[0] << ": Error writing ELF file: " << errMsg << "\n";
    exit(-1);
  }
  out.close();

  return 0;
}
