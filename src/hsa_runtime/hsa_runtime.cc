//===- hsa_runtime.cc -----------------------------------------------------===//
//
//                     The HSA Simulator
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "hsa.h"

#include "brig_engine.h"
#include "brig_llvm.h"
#include "brig_module.h"
#include "brig_reader.h"

#include "llvm/IR/Module.h"

#include <cassert>
#include <cstdarg>

namespace hsa {

typedef vector<Device *> DeviceList;

class SimKernel : public Kernel {
 public:

  SimKernel(llvm::Function *F) : F_(F) {}

  virtual void *allocateGroupMemory(size_t, size_t) {
    return NULL;
  }

  virtual void freeGroupMemory(const void *) {}

  virtual void *getISA() {
    return NULL;
  }

  virtual const void *getGroupMemoryBase() const {
    return NULL;
  }

  virtual size_t getSizeOfISA() {
    return 0;
  }

  virtual hsacommon::string &getName() {
    assert(false && "Unimplemented");
    static hsacommon::string name;
    return name;
  }

  llvm::Function *F_;
};

class SimProgram : public Program {
 public:

  SimProgram(hsa::brig::BrigModule &mod) :
    BP_(hsa::brig::GenLLVM::getLLVMModule(mod)) {}

  virtual Kernel *compileKernel(const char *kernelName, const char *) {
    llvm::Function *fun = BP_->getFunction(kernelName + 1);
    return fun ? new SimKernel(fun) : NULL;
  }

  virtual void addDevice(Device *device) {}
  virtual char *getElf() { return NULL; }
  virtual size_t getElfSize() { return 0; }
  virtual void *getGlobalBase() {
    assert(false && "Unimplemented");
    return NULL;
  }

  virtual ~SimProgram() {}

 private:
  hsa::brig::BrigProgram BP_;
};

class SimQueue : public Queue {

 public:

  SimQueue(Device *device) : device_(device) {}

  virtual Device *getDevice() {
    return device_;
  }

  virtual DispatchEvent* dispatch(Kernel *kernel,
                                  LaunchAttributes attrs,
                                  hsacommon::vector<Event *> &events,
                                  uint32_t count, ...) {

    hsacommon::vector<KernelArg> kernArgs;

    va_list ap;
    va_start(ap, count);
    for (unsigned i = 0; i < count; ++i) {
      kernArgs.push_back(va_arg(ap, KernelArg));
    }

    return dispatch(kernel, attrs, events, kernArgs);
  }

  virtual DispatchEvent* dispatch(Kernel *kernel,
                                  LaunchAttributes attrs,
                                  hsacommon::vector<Event *> &,
                                  hsacommon::vector<KernelArg> &kernArgs) {

    SimKernel *sk = reinterpret_cast<SimKernel *>(kernel);

    std::vector<void *> args;
    for (unsigned i = 0; i < kernArgs.size(); ++i)
      args.push_back(&kernArgs[i]);

    llvm::Function *fun = sk->F_;
    llvm::Module *mod = fun->getParent();
    hsa::brig::BrigEngine BE(mod);
    uint32_t blockNum = attrs.grid[0] * attrs.grid[1] * attrs.grid[2];
    uint32_t threadNum = attrs.group[0] * attrs.group[1] * attrs.group[2];
    BE.launch(fun, args, blockNum, threadNum);
    return NULL;
  }

  virtual void flush() {}

 private:
  Device *device_;
};


class SimDevice : public Device {
  virtual DeviceType getType() const {
    return DEVICE_TYPE_GPU;
  }

  virtual Kernel *compile(Program *,
                          const char *,
                          const char *) {
    assert(false && "Unimplemented");
    return NULL;
  }

  virtual const hsacommon::string &getVendorName() const {
    assert(false && "Unimplemented");
    static hsacommon::string vendorName;
    return vendorName;
  }

  virtual uint32_t getVendorID() {
    assert(false && "Unimplemented");
    return 0;
  }

  virtual unsigned int getComputeUnitsCount() {
    assert(false && "Unimplemented");
    return 0;
  }

  virtual uint32_t getCapabilities() {
    assert(false && "Unimplemented");
    return 0;
  }

  virtual hsacommon::string &getName() {
    assert(false && "Unimplemented");
    static hsacommon::string name;
    return name;
  }

  typedef hsacommon::vector<hsacommon::MemoryDescriptor *> MemDescriptorList;
  virtual const MemDescriptorList &getMemoryDescriptors() {
    assert(false && "Unimplemented");
    static const MemDescriptorList mdl;
    return mdl;
  }

  typedef hsacommon::vector<hsacommon::CacheDescriptor *> CachDescriptorList;
  virtual const CachDescriptorList &getCacheDescriptors() {
    assert(false && "Unimplemented");
    static const CachDescriptorList cdl;
    return cdl;
  }

  virtual bool isDoublePrecision() {
    assert(false && "Unimplemented");
    return false;
  }

  virtual bool isDebug() {
    assert(false && "Unimplemented");
    return false;
  }

  virtual bool isDedicatedCompute() {
    assert(false && "Unimplemented");
    return false;
  }

  virtual uint32_t getMaxGroupMemorySize() {
    assert(false && "Unimplemented");
    return false;
  }

  virtual uint32_t getMaxQueueSize() {
    assert(false && "Unimplemented");
    return false;
  }

  virtual int getWaveFrontSize() {
    assert(false && "Unimplemented");
    return 0;
  }

  virtual void *allocateGlobalMemory(size_t, size_t, hsacommon::HeapType,
                                     uint32_t) {
    assert(false && "Unimplemented");
    return NULL;
  }

  virtual void freeGlobalMemory(void *) {
    assert(false && "Unimplemented");
  }

  virtual void registerMemory(void *, size_t) {
    assert(false && "Unimplemented");
  }

  virtual void deregisterMemory(void *) {
    assert(false && "Unimplemented");
  }

  virtual void mapMemory(void *, size_t) {
    assert(false && "Unimplemented");
  }

  virtual void unmapMemory(void *) {
    assert(false && "Unimplemented");
  }


  virtual hsacommon::DeviceClockCounterInfo getClockCounterInfo() {
    assert(false && "Unimplemented");
    return hsacommon::DeviceClockCounterInfo();
  }

  virtual int getMaxFrequency() {
    return 0;
  }

  virtual Event *createEvent() {
    return NULL;
  }

  virtual Queue *createQueue(uint32_t) {
    return new SimQueue(this);
  }
};

class SimRuntimeApi : public RuntimeApi {
 public:

  SimRuntimeApi() {
    if (!devices.size())
      devices.push_back(new SimDevice());
  }

  ~SimRuntimeApi() {
    for (unsigned i = 0; i < devices.size(); ++i)
      delete devices[i];
  }

  virtual uint32_t getDeviceCount() { return 1U; }

  virtual const DeviceList &getDevices() { return devices; }

  virtual Program *createProgram(char *elf, size_t elfSize, DeviceList *) {
    llvm::OwningPtr<hsa::brig::BrigReader> reader
      (hsa::brig::BrigReader::createBrigReader(elf, elfSize));
    if (!reader) return NULL;

    hsa::brig::BrigModule brigMod(*reader, &llvm::errs());
    if (!brigMod.isValid()) return NULL;

    return new SimProgram(brigMod);
  }

  virtual Program *createProgramFromFile(const char *filename, DeviceList *) {
    hsa::brig::BrigReader *reader =
      hsa::brig::BrigReader::createBrigReader(filename);
    if (!reader) return NULL;

    hsa::brig::BrigModule brigMod(*reader, &llvm::errs());
    if (!brigMod.isValid()) return NULL;

    return new SimProgram(brigMod);
  }

  virtual void destroyProgram(Program *) {}

  virtual const string &getVersion() { return version; }

  virtual Event *createDeviceEvent(Device *) {
    return NULL;
  }

  virtual void *allocateGlobalMemory(size_t size, size_t align) {
    void *memptr;
    if (posix_memalign(&memptr, std::max(sizeof(void *), align), size))
      return NULL;
    return memptr;
  }

  virtual void freeGlobalMemory(void *ptr) {
    free(ptr);
  }

 private:
  static DeviceList devices;
  static string version;
};

DeviceList SimRuntimeApi::devices;
string SimRuntimeApi::version;

RuntimeApi *getRuntime() {
  return new SimRuntimeApi();
}

}  // namespace hsa
