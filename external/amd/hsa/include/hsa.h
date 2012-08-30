//depot/stg/hsa/drivers/hsa/api/hsart/public/hsa.h#2 - edit change 775354 (text)
#ifndef _HSA_H_
#define _HSA_H_


// lhowes: Temporary workaround for tool chain issues
// Once headers finalized we can come up with a cleaner 
// solution if system includes are still necessary
#if !defined(AMD_AMP_HSA_INCLUDES)
#include <vector>
#include <string>
#endif // #if !defined(AMD_AMP_HSA_INCLUDES)

#include "hsailutil.h"
// lhowes: Temporary workaround for tool chain issues
// Once headers finalized we can come up with a cleaner 
// solution if system includes are still necessary
#if !defined(AMD_AMP_HSA_INCLUDES)
#include <stdint.h>
#include <assert.h>
#endif // #if !defined(AMD_AMP_HSA_INCLUDES)

#include "hsacommon.h"

namespace hsa
{

/**
 * @addtogroup HSA_PUBLIC Public HSA Device interfaces
 *
 * @{
 *
 */

#define MAX_INFO_STRING_LEN 0x40
enum HSAIL_ADDRESS_QUALIFIER{
HSAIL_ADDRESS_ERROR=0,
HSAIL_ADDRESS_GLOBAL,
HSAIL_ADDRESS_LOCAL,
HSAIL_MAX_ADDRESS_QUALIFIERS
} ;

enum HSAIL_ARG_TYPE{
HSAIL_ARGTYPE_ERROR=0,
HSAIL_ARGTYPE_POINTER,
HSAIL_ARGTYPE_VALUE,
HSAIL_ARGTYPE_IMAGE,
HSAIL_ARGMAX_ARG_TYPES
};

enum HSAIL_DATA_TYPE{
HSAIL_DATATYPE_ERROR=0,
HSAIL_DATATYPE_B1,
HSAIL_DATATYPE_B8,
HSAIL_DATATYPE_B16,
HSAIL_DATATYPE_B32,
HSAIL_DATATYPE_B64,
HSAIL_DATATYPE_S8,
HSAIL_DATATYPE_S16,
HSAIL_DATATYPE_S32,
HSAIL_DATATYPE_S64,
HSAIL_DATATYPE_U8,
HSAIL_DATATYPE_U16,
HSAIL_DATATYPE_U32,
HSAIL_DATATYPE_U64,
HSAIL_DATATYPE_F16,
HSAIL_DATATYPE_F32,
HSAIL_DATATYPE_F64,
HSAIL_DATATYPE_STRUCT,
HSAIL_DATATYPE_MAX_TYPES
};


class IQueue;
class IKernel;
class IEvent;
class IDevice;
class IProgram;
class IDispatchDescriptor;

typedef enum {
        HALT_WAVE=1,
        KILL_WAVE=2,
        RESUME_WAVE=4
} HSAWaveAction;

typedef enum {
        IGNORE_HOST=1,
        EXIT_HOST=2,
        INTERRUPT_HOST=4
} HSAHostAction;

typedef enum {
        FLUSH_ALL=1,
        FLUSH_BOTTOM_LEVEL =2,
        FLUSH_TOP_LEVEL=4
} HSACachePolicy;

typedef void (*HSATrapHandle)(void *,int);

typedef struct HSAExceptionPolicy {
        int enableException;
        HSAWaveAction waveAction;
        HSAHostAction hostAction;
        HSATrapHandle trapHandle;

        HSAExceptionPolicy()
        {
            int enableException = 1;
            waveAction = HALT_WAVE;
            hostAction = IGNORE_HOST;
            trapHandle = NULL;
        }
}HSAExceptionPolicy;

typedef enum {
        WAIT_DEPENDENCY=1,
        WAIT_ENQUEUE=2,
        WAIT_LAUNCH=4,
        WAIT_COMPLETION=8
}HSACompletionPolicy;

typedef struct LaunchAttributes {
	HSACompletionPolicy completionPolicy;
        HSAExceptionPolicy exceptionPolicy;
        HSACachePolicy cachePolicy;
        int gridX;
        int gridY;
        int gridZ;
        int groupX;
        int groupY;
        int groupZ;

        LaunchAttributes()
        {
            cachePolicy = FLUSH_ALL;
            gridX = 1;
            gridY = 1;
            gridZ = 1;
            groupX = 1;
            groupY = 1;
            groupZ = 1;

        }
}LaunchAttributes;

typedef hsa::vector<hsa::IDevice*>::const_iterator Idevice_itr;
typedef hsa::vector<hsa::IDevice*> Idevice_list;
typedef hsa::vector<hsa::IDevice*>* Idevice_list_ptr;
typedef std::string KernelId;

/**
 * @copydoc hsacore::allocateMemory
 */
DLL_PUBLIC void*
allocateMemory(
    const size_t size,
    const uint32_t memtype,
    const IDevice& dev);

/**
 * @copydoc hsacore::freeMemory
 */
DLL_PUBLIC void freeMemory(void* ptr);

/**
 * @copydoc hsacore::registerMemory
 */
DLL_PUBLIC void registerMemory(void* ptr, const size_t size,
		const uint32_t type);

/**
 * @copydoc hsacore::deregisterMemory
 */
DLL_PUBLIC void deregisterMemory(void* ptr);

/**
 * @copydoc hsacore::mapMemory
 */
DLL_PUBLIC void mapMemory(void* ptr, const size_t size);

/**
 * @copydoc hsacore::unmapMemory
 */
DLL_PUBLIC void unmapMemory(void* ptr);

/**
 * @brief Device class, public interface in the device layer.
 */
class DLL_PUBLIC IDevice
{
public:
    virtual hsa::DeviceType getType() const = 0;

    virtual unsigned int getComputeUnitsCount()=0;

    virtual uint32_t getCapabilities()=0;

    virtual std::string& getName()=0;

    virtual const hsa::vector<hsa::MemoryDescriptor*>& getMemoryDescriptors()=0;

    virtual const hsa::vector<hsa::CacheDescriptor*>& getCacheDescriptors()=0;

    virtual bool isDoublePrecision()=0;

    virtual bool isDebug()=0;

    virtual bool isDedicatedCompute()=0;

    // Get the device information in bulk.
    //virtual const Info& info()=0;

	//Temp hack to get the asicinfo
	virtual void* getASICInfo()=0;

    virtual ~IDevice(){};
};


class DLL_PUBLIC IEvent
{
public:
    virtual AMDRETCODE wait()=0;
    virtual void getDeviceTriggerInfo(uint32_t &value,uint64_t &location)=0;
    virtual ~IEvent(){};
private:
};

class DLL_PUBLIC IQueue
{
public:
    virtual hsa::AMDRETCODE dispatch(void *kinfo, void *prgmptr, unsigned int prgmsize, IEvent *eventptr)=0;
    virtual hsa::IEvent * dispatch(IKernel *k, RTKernelArg arg1, hsa::IEvent *ievp)=0;
    virtual hsa::IEvent * dispatch(IKernel * kernel, IEvent * depEvent, uint32_t numArgs, ...)=0;
    virtual hsa::IEvent *dispatch(IDispatchDescriptor * dispDescriptor, LaunchAttributes *launchAttr, IEvent * depEvent)=0;
    virtual hsa::IEvent *dispatch(IKernel * kernel, LaunchAttributes *launchAttr, IEvent * depEvent, uint32_t numArgs, ...)=0;
    virtual hsa::AMDRETCODE getScratchUserPtr(void **ptr)=0;
    virtual hsa::AMDRETCODE getScratchSize(unsigned int *scrsize)=0;
    virtual hsa::AMDRETCODE flush()=0;
    virtual IDispatchDescriptor * createDispatchDescriptor(IKernel * kernel, LaunchAttributes *launchAttr, uint32_t numArgs, ...)=0;
    virtual ~IQueue(){};
};


class DLL_PUBLIC IKernel
{
public:
    virtual ~IKernel(){};
    virtual void setArg(int index, RTKernelArg arg)=0;
    /* This needs to go - should not pass the metadata
    through an interface*/
    virtual void initArgs(char* argList)=0;

    /*! Returns the number of arguments
    * @return - The number of arguments
    * */
    virtual int getArgCount()=0;
    
    /*! Returns the offset in the parameter stack
    * @param index - index of the argument
    * @return - The offset in the parameter stack
    * */
    virtual int getArgOffset(int index)=0;
   
    /* Returns the size of the argument
    * @param index - index of the argument
    * @return - The size of the argument
    * */
    virtual int getArgSize(int index)=0;
    /*! Returns the type of the argument
    * @param index - index of the argument
    * @return - The type of the argument as a HSAIL_ARG_TYPE enum
    * */
    virtual HSAIL_ARG_TYPE getArgType(int index)=0;
	/*! Returns the Address Qualifier - global, local etc.
    * @param index - index of the argument
    * @return - The type of the argument as a HSAIL_ADDRESS_QUALIFIER enum
    * */
	virtual HSAIL_ADDRESS_QUALIFIER getArgAddrQualifier(int index)=0;
};

class IRuntimeApi;

extern "C" {

// getRuntime() function:
// When the HSA Runtime device DLL is explicitly loaded, this will be the only function called using GetProcAddress().
// All other global exported functions will be called via HsaRuntimeApi singleton instance returned by this function.
DLL_PUBLIC hsa::IRuntimeApi* getRuntime();

// typedef comes handy when GetProcAddress() returns the func pointer which needs to be cast.
typedef hsa::IRuntimeApi* (*fptr_getRuntime)();
 
}

class DLL_PUBLIC IRuntimeApi  
{
public:
	// All the exported global functions must have corresponding pure virtual public method declared in this interface class.
    virtual hsa::AMDRETCODE getDeviceCount( uint32_t* count )=0;
    virtual const hsa::vector<hsa::IDevice*>& getDevices()=0;

    virtual hsa::AMDRETCODE createDeviceQueue(hsa::IDevice *d, unsigned int size, hsa::IQueue* &q)=0;
    virtual hsa::AMDRETCODE createProgram(char *charElf, size_t elfSize, Idevice_list_ptr pDevices, IProgram* &p){
	return 1;
    };
    virtual hsa::AMDRETCODE createProgramFromELF(void *charElf, Idevice_list_ptr pDevices, IProgram* &p){
        return 1;
    }
    virtual hsa::AMDRETCODE createDeviceEvent(hsa::IDevice *d, hsa::IEvent* &e)=0;
    virtual IKernel *createKernel(IProgram * k, hsa::KernelId & kid)=0;

    /**
     * @copydoc hsa::allocateMemory
     */
    virtual void*
    allocateMemory(
        const size_t size,
        const uint32_t memtype,
        const IDevice& dev) = 0;

    /**
     * @copydoc hsa::freeMemory
     */
    virtual void
    freeMemory(void* ptr) = 0;

    /**
     * @copydoc hsa::mapMemory
     */
    virtual void
    mapMemory(void* ptr, const size_t size) = 0;

    /**
     * @copydoc hsa::unmapMemory
     */
    virtual void
    unmapMemory(void* ptr) = 0;

    virtual ~IRuntimeApi(){};
};

class DLL_PUBLIC IProgram
{
public:
    virtual hsa::IKernel * build(const char* kernelName,size_t size) = 0;
    virtual void addDevice(hsa::IDevice * device)  = 0;
    virtual hsacore::HsailKernel* getMetaData(IDevice *d, KernelId &kid) = 0;
    virtual ~IProgram(){};
};

class DLL_PUBLIC IDispatchDescriptor
{
public:
    virtual void initDispatch () = 0;
    virtual uint32_t GetCommandPkt(uint32_t * buf) = 0;
    virtual void execCommandPkt() = 0;
    virtual void waitForEndOfKernel() = 0;
    virtual ~IDispatchDescriptor(){};
};

/**
 * @}
 */
}

#endif
