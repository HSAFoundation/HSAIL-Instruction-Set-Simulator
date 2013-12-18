//===- newcore.cc ---------------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "newcore.h"
#include "brig_engine.h"
#include "brig_function.h"
#include "brig_llvm.h"
#include "brig_module.h"
#include "brig_reader.h"

using hsa::brig::BrigEngine;
using hsa::brig::BrigFunction;
using hsa::brig::BrigModule;
using hsa::brig::BrigProgram;
using hsa::brig::BrigReader;
using hsa::brig::GenLLVM;

extern "C" {

  static void copySection(void *&dest, uint32_t &size,
                          const llvm::StringRef &string) {
    size = string.size();
    dest = new char[size];
    memcpy(dest, string.data(), size);
  }

  HsaStatus HsaCreateBrig(const char *file, struct HsaBrig *brig) {
    BrigReader *reader = BrigReader::createBrigReader(file);
    if (!reader) return kHsaStatusInvalidArgument;
    copySection(brig->string_section, brig->string_section_byte_size,
                reader->getStrings());
    copySection(brig->directive_section, brig->directive_section_byte_size,
                reader->getDirectives());
    copySection(brig->code_section, brig->code_section_byte_size,
                reader->getCode());
    copySection(brig->operand_section, brig->operand_section_byte_size,
                reader->getOperands());
    copySection(brig->debug_section, brig->debug_section_byte_size,
                reader->getDebug());
    delete reader;
    return kHsaStatusSuccess;
  }

  HsaStatus HsaDestroyBrig(struct HsaBrig *brig) {
    delete[] (char *) brig->string_section;
    delete[] (char *) brig->directive_section;
    delete[] (char *) brig->code_section;
    delete[] (char *) brig->operand_section;
    delete[] (char *) brig->debug_section;
    return kHsaStatusSuccess;
  }

  static BrigReader *createBrigReader(const struct HsaBrig *brig) {
    llvm::StringRef dirs((char *) brig->directive_section,
                         brig->directive_section_byte_size);
    llvm::StringRef code((char *) brig->code_section,
                         brig->code_section_byte_size);
    llvm::StringRef operands((char *) brig->operand_section,
                             brig->operand_section_byte_size);
    llvm::StringRef debug((char *) brig->debug_section,
                          brig->debug_section_byte_size);
    llvm::StringRef strings((char *) brig->string_section,
                            brig->string_section_byte_size);
    return BrigReader::createBrigReader(dirs, code, operands, debug, strings);
  }

  HsaStatus HsaGetKernelOffset(const char *kernelName,
                               const struct HsaBrig *brig,
                               HsaBrigDirectiveOffset *kernel) {

    BrigReader *reader = createBrigReader(brig);
    BrigModule mod(*reader);

    const size_t len = strlen(kernelName);

    *kernel = 0;
    for (BrigFunction fun = mod.begin(), E = mod.end(); fun != E; ++fun) {
      const BrigString *name = fun.getName();
      if (len == name->byteCount - 1 &&
         !strncmp((char *) name->bytes + 1, kernelName, len))
        *kernel = fun.getOffset();
    }

    delete reader;

    return *kernel ? kHsaStatusSuccess : kHsaStatusInvalidArgument;
  }

  static HsaDevice device;

  HsaStatus HsaGetDevices(unsigned int *count, const HsaDevice **device_list) {
    *count = 1;
    *device_list = &device;
    return kHsaStatusSuccess;
  }

  struct KernelInfo {
    BrigProgram BP;
    llvm::Function *F;
    unsigned params;

    KernelInfo(BrigProgram BP, llvm::Function *F, unsigned params)
      : BP(BP), F(F), params(params) {}
  };

  HsaStatus HsaFinalizeHsail(const HsaDevice *device,
                             HsaBrig *brig,
                             HsaBrigDirectiveOffset kernel_directive,
                             const char *options,
                             HsaFinalizerContext context,
                             HsaFindSymbolInMap find_symbol_function,
                             HsaAQLKernel *kernel) {

    // Leaks info, but the API currently doesn't allow provide a method for
    // deleting finalized kernels, so there is nothing to be done for it...
    BrigReader *reader = createBrigReader(brig);
    BrigModule M(*reader);
    BrigProgram BP = GenLLVM::getLLVMModule(M);
    std::string name = BP.getFunctionName(kernel_directive);
    llvm::Function *F = BP.getFunction(name);
    unsigned params = BP.getNumParams(name);
    KernelInfo *info = new KernelInfo(BP, F, params);
    delete reader;
    kernel->kernel_object_address = (uint64_t) info;
    return kHsaStatusSuccess;
  }

  HsaStatus HsaCreateUserModeQueue(const HsaDevice *device,
                                   void *buffer, size_t buffer_size,
                                   HsaQueueType queue_type,
                                   HsaQueuePriority queue_priority,
                                   HsaQueueFraction queue_fraction,
                                   HsaQueue **queue) {

    static uint64_t queue_id = 0;
    *queue = new HsaQueue;
    (*queue)->queue_id = queue_id++;
    (*queue)->dispatch_id = 0;
    return kHsaStatusSuccess;
  }

  HsaStatus HsaWriteAQLPacket(HsaQueue *queue,
                              HsaAqlDispatchPacket *aql_packet) {
    KernelInfo *info = (KernelInfo *) aql_packet->kernel_object_address;
    BrigEngine BE(info->BP);

    uint64_t *argsPtr = (uint64_t *) aql_packet->kernel_args_address;
    std::vector<void *> argArray;
    for (unsigned i = 0; i < info->params; ++i)
      argArray.push_back(argsPtr + i);
    llvm::ArrayRef<void *> args(argArray);

    BE.launch(info->F, argArray,
              aql_packet->nd_range_size.x,
              aql_packet->workgroup_size.x,
              queue, queue->queue_id,
              aql_packet, queue->dispatch_id++);
    return kHsaStatusSuccess;
  }

  HsaStatus HsaWaitCompletion(HsaCompletionObject *completion) {
    return kHsaStatusSuccess;
  }

  HsaStatus HsaDestroyUserModeQueue (HsaQueue *queue) {
    delete queue;
    return kHsaStatusSuccess;
  }

}
