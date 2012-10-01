#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "hsa.h"

#include "brig_engine.h"
#include "brig_llvm.h"
#include "brig_module.h"
#include "brig_reader.h"

#include "llvm/Module.h"

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
  }

  llvm::Function *F_;
};

class SimProgram : public Program {
 public:

  SimProgram(llvm::Module *mod) : mod_(mod) {}

  virtual Kernel *compileKernel(const char *kernelName, const char *) {
    llvm::Function *fun = mod_->getFunction(kernelName + 1);
    return fun ? new SimKernel(fun) : NULL;
  }

  virtual void addDevice(Device *device) {}
  virtual char *getElf() { return NULL; }
  virtual size_t getElfSize() { return 0; }
  virtual void *getGlobalBase() {
    assert(false && "Unimplemented");
  }

  virtual ~SimProgram() {}

 private:
  llvm::Module *mod_;
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
    for(unsigned i = 0; i < count; ++i) {
      kernArgs.push_back(va_arg(ap, KernelArg));
    }

    return dispatch(kernel, attrs, events, kernArgs);
  }

  virtual DispatchEvent* dispatch(Kernel *kernel,
                                  LaunchAttributes,
                                  hsacommon::vector<Event *> &,
                                  hsacommon::vector<KernelArg> &kernArgs) {

    SimKernel *sk = reinterpret_cast<SimKernel *>(kernel);

    std::vector<void *> args;
    for(unsigned i = 0; i < kernArgs.size(); ++i)
      args.push_back(&kernArgs[i]);

    llvm::Function *fun = sk->F_;
    llvm::Module *mod = fun->getParent();
    hsa::brig::launchBrig(mod, fun, args);

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
  }

  virtual const hsacommon::string &getVendorName() const {
    assert(false && "Unimplemented");
  }

  virtual uint32_t getVendorID() {
    assert(false && "Unimplemented");
  }

  virtual unsigned int getComputeUnitsCount() {
    assert(false && "Unimplemented");
  }

  virtual uint32_t getCapabilities() {
    assert(false && "Unimplemented");
  }

  virtual hsacommon::string &getName() {
    assert(false && "Unimplemented");
  }

  typedef hsacommon::vector<hsacommon::MemoryDescriptor *> MemDescriptorList;
  virtual const MemDescriptorList &getMemoryDescriptors() {
    assert(false && "Unimplemented");
  }

  typedef hsacommon::vector<hsacommon::CacheDescriptor *> CachDescriptorList;
  virtual const CachDescriptorList &getCacheDescriptors() {
    assert(false && "Unimplemented");
  }

  virtual bool isDoublePrecision() {
    assert(false && "Unimplemented");
  }

  virtual bool isDebug() {
    assert(false && "Unimplemented");
  }

  virtual bool isDedicatedCompute() {
    assert(false && "Unimplemented");
  }

  virtual uint32_t getMaxGroupMemorySize() {
    assert(false && "Unimplemented");
  }

  virtual uint32_t getMaxQueueSize() {
    assert(false && "Unimplemented");
  }

  virtual int getWaveFrontSize() {
    assert(false && "Unimplemented");
  }

  virtual void *allocateGlobalMemory(size_t, size_t, hsacommon::HeapType, uint32_t) {
    assert(false && "Unimplemented");
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
    if(!devices.size())
      devices.push_back(new SimDevice());
  }

  virtual uint32_t getDeviceCount() { return 1U; }

  virtual const DeviceList &getDevices() { return devices; }

  virtual Program *createProgram(char *elf, size_t elfSize, DeviceList *) {
    hsa::brig::BrigReader *reader =
      hsa::brig::BrigReader::createBrigReader(elf, elfSize);
    if(!reader) return NULL;

    hsa::brig::BrigModule brigMod(*reader, &llvm::errs());
    if(!brigMod.isValid()) return NULL;

    llvm::Module *M = hsa::brig::GenLLVM::getLLVMModule(brigMod);
    if(!M) return NULL;

    return new SimProgram(M);
  }

  virtual Program *createProgramFromFile(const char *filename, DeviceList *) {
    hsa::brig::BrigReader *reader =
      hsa::brig::BrigReader::createBrigReader(filename);
    if(!reader) return NULL;

    hsa::brig::BrigModule brigMod(*reader, &llvm::errs());
    if(!brigMod.isValid()) return NULL;

    llvm::Module *M = hsa::brig::GenLLVM::getLLVMModule(brigMod);
    if(!M) return NULL;

    return new SimProgram(M);
  }

  virtual void destroyProgram(Program *) {}

  virtual const string &getVersion() { return version; }

  virtual Event *createDeviceEvent(Device *) {
    return NULL;
  }

  virtual void *allocateGlobalMemory(size_t size, size_t align) {
    void *memptr;
    posix_memalign(&memptr, std::max(sizeof(void *), align), size);
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

} // namespace hsa
