//depot/stg/hsa/drivers/hsa/api/hsart/public/hsa.h#11 - edit change 793355 (text)
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
class Event;
class DebuggerEvent;
class IDevice;
class IProgram;
class IDispatchDescriptor;

/**
 * The wave action lets the user chose what happens to a wave when an
 * exception occurs, there are three choices
 */
typedef enum {
	HALT_WAVE=1, /*!< Halt the wave, usually this is what happens, wave is
		       not killed, just halted */
	KILL_WAVE=2, /*!< Kill the wave, wave is killed at the point of
		       exception, if not all the wavefronts in the wave take a
		       fault, the place in the code where the non-faulty waves
		       get killed is not deterministic. It usually happens
		       before the next barrier */
	RESUME_WAVE=4 /*!< increment the PC to the next instruction and
			continue running, this choice is for advanced users */
} HSAWaveAction;


/**
 * This is to chose what should happen to the application program, there are
 * three choices
 */
typedef enum {
	IGNORE_HOST=1, /*!< the application doesn't want to know about the
			 exception */
        EXIT_HOST=2,   /*!< the application wants runtime to call exit() */
	INTERRUPT_HOST=4 /*!< the host is expecing it be interrupted, host has
			   registered a call back for this via events */
} HSAHostAction;

/**
 * This is to chose what should happen to the application program in terms of
 * the data it uses. There are three choices
 * */
typedef enum {
        FLUSH_ALL=1, /*!< flush all the caches the runtime can flush*/
	FLUSH_BOTTOM_LEVEL =2,/*!< flush flushable caches closer to the vector
				arrays*/
        FLUSH_TOP_LEVEL=4  /*!< flush top-level GPU cache */
} HSACachePolicy;


/**
 * the function pointer for registering handlers associated with events in HSA
 */
typedef void (*HSATrapHandle)(void *,int);

/**
 * the exception policy is comprised of wave action, host action and the
 * location of trap handle
 */
typedef struct HSAExceptionPolicy {
    int enableException;       /*!< default is to enable them */
    HSAWaveAction waveAction;  /*!< default wave action is to halt it */
	HSAHostAction hostAction;  /*!< default host action is to ignore the
				     event when it occurs it but inform the
				     host when it does wait on this kernel */
	HSATrapHandle trapHandle;  /*!< if host action is to interrupt, this
				     trap handle gets executed */

    HSAExceptionPolicy()
    {
        int enableException = 1;
        waveAction = HALT_WAVE;
        hostAction = IGNORE_HOST;
        trapHandle = NULL;
    }
}HSAExceptionPolicy;

/**
 * completion policy defined the following
 */
typedef enum {
	WAIT_DEPENDENCY=1,  /*!< return from launch only after waiting for all
			      dependencies */
        WAIT_ENQUEUE=2,  /*!< return from launch only after enqueue is intiated */
	WAIT_LAUNCH=4,  /*!< return from launch after the kernel launch is
			  actually copied on to the queue*/
	WAIT_COMPLETION=8  /*!< return after the kernel is completed execution,
			     event is still returned if this kernel takes an
			     exception */
}HSACompletionPolicy;

/** 
 * attributes of a launch include, exception policy, launch policy and cache
 * policy 
 */
typedef struct LaunchAttributes {
	HSACompletionPolicy completionPolicy; /*!< default has none of the
						guarantees listed in the
						policy*/
	HSAExceptionPolicy exceptionPolicy; /*!< default is to generate
					      exceptions*/
	
	/* trap handler used for GPU exeception and HW debugger */
	void *trapHandler;  
	/* trap handler buffer, accessed by trap handler */
	void *trapHandlerBuffer;
	/* trap handler buffer size */
	void *trapHandlerBufferSize;

	HSACachePolicy cachePolicy; /*!< default is to flush everything */
        int gridX; /*!< default is 1*/
        int gridY; /*!< default is 1*/
        int gridZ; /*!< default is 1*/
        int groupX; /*!< default is 1*/
        int groupY; /*!< default is 1*/
        int groupZ; /*!< default is 1*/
        int groupOffsets[3];

        LaunchAttributes()
        {
            cachePolicy = FLUSH_ALL;
            gridX = 1;
            gridY = 1;
            gridZ = 1;
            groupX = 1;
            groupY = 1;
            groupZ = 1;
            groupOffsets[0]=0;
            groupOffsets[1]=0;
            groupOffsets[2]=0;
        }
}LaunchAttributes;

typedef hsa::vector<hsa::IDevice*>::const_iterator Idevice_itr;
typedef hsa::vector<hsa::IDevice*> Idevice_list;
typedef hsa::vector<hsa::IDevice*>* Idevice_list_ptr;
typedef std::string KernelId;

/**
 * @copydoc hsacore::allocateMemory(const size_t size, const size_t alignment = 0)
 */
DLL_PUBLIC void* allocateMemory(const size_t size, const size_t alignment = 0);

/**
 * @copydoc hsacore::allocateMemory(const size_t size, const size_t alignment, const uint32_t type, const IDevice& dev)
 */
DLL_PUBLIC void* allocateMemory(const size_t size, const size_t alignment, const uint32_t type, const IDevice& dev);

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
 * @brief APIs for creating debug event
 */
DLL_PUBLIC hsa::DebuggerEvent * 
createDebuggerEvent(
	hsa::IDevice *dev, 
	bool manulRest, 
	bool state
	);

DLL_PUBLIC hsa::DebuggerEvent * 
createDebuggerEvent(
	hsa::IDevice *dev, 
    void *userBuffer, 
    bool manulRest, 
    bool state
	);

/**
 * @brief installs the trap handler
 */
DLL_PUBLIC hsa::AMDRETCODE 
setupDbgTrapHandler(
	hsa::IDevice * dev, 
	void *         trapHandler,
	size_t         trapHandlerSizeByte
	);

/**
 * @brief installs the trap handler buffer
 */
DLL_PUBLIC hsa::AMDRETCODE 
setupDbgTrapHandlerBuffer(
	hsa::IDevice * dev, 
	void *   trapHandlerBuffer,
	size_t   trapHandlerBufferSizeByte
	);

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

	//setup trap handler
	virtual hsa::AMDRETCODE setupTrapHandler(void *trapHandler, size_t trapHandlerSizeByte) = 0;

	//setup trap handler buffer
	virtual hsa::AMDRETCODE setupTrapHandlerBuffer(void *trapHandlerBuffer, size_t trapHandlerBufferSizeByte) = 0;

	//get trap handler and its size
	virtual hsa::AMDRETCODE getTrapHandler(void* &trapHandler, size_t &trapHandlerSizeByte) = 0;

	//get the trap handler buffer and its size
	virtual hsa::AMDRETCODE getTrapHandlerBuffer(void* &trapHandlerBuffer, size_t &trapHandlerBufferSizeByte) = 0;

	//add queue to the list
	virtual hsa::AMDRETCODE addQueueToList(hsa::IQueue *queue) = 0;

	//get dispatch descriptor from dispatch ID
    virtual IDispatchDescriptor * getDispatchDescriptor(uint32_t dispatchID) = 0;

    virtual ~IDevice(){};
};

class DLL_PUBLIC Event
{

public:

    /**
     * @brief Default destructor of Event interface.
     */
    virtual ~Event(){};

    /**
     * @brief Ensures that the event is generated atleast once after this
     * call was invoked
     *
     * @param timeOut in milliseconds, 0 means non-blocking wait while
     * 0xFFFFFFFF means blocking wait.
     *
     * @return HsaEventWaitReturn return value indicating success or timeout.
     */
    virtual HsaEventWaitReturn wait(uint32_t timeOut) = 0;

    /**
     * @brief Updates output parameters witht Hsa Kernel Module - HKM
     * provided values. These values are needed to trigger an interrupt
     * via End Of Pipe (EOP) packet.
     *
     * @param krnlValue Kernel value used by HKM to map an EOP based
     * interrupt to a specific event object.
     *
     * @param krnlAddr Kernel address used by HKM to map an EOP based
     * interrupt to a specific event object.
     *
     * @return void
     */
    virtual void getDeviceTriggerInfo(uint32_t &krnlValue, uint64_t &krnlAddr) = 0;

    /**
     * @brief Waits on the list of input events. The method will return when
     * one or all of the events in the list has been signalled or the timeout
     * specified for wait has expired.
     *
     * @param waitOnAll boolean specifies if the call should wait on all events
     * in the dependent list to be signalled or any one of them is sufficient.
     *
     * @param timeOut in milliseconds, 0 means non-blocking wait while 0xFFFFFFFF
     * means a blocking wait.
     *
     * @param eventCnt number of events in the events list on which this call
     * would suspend.
     *
     * @param eventList list of events to wait on.
     *
     * @param waitOnAll specifies if the wait should wait on all dependent events.
     * TRUE means wait on all events, FALSE means call will return when any one
     * of the events is signalled.
     *
     * @throws HsaException when either of the arguments is invalid or an errors
     * occurs in the runtime.
     *
     * @return uint32_t specifies if the kFD call has succeeded.
     */
    virtual uint32_t waitOnEvents(bool waitOnAll, uint32_t timeOut,
                                  uint32_t eventCnt, hsa::Event **eventList) = 0;

};

class DLL_PUBLIC DebuggerEvent
{
public:
    /**
     * @brief Default destructor of Event interface.
     */
    virtual ~DebuggerEvent(){};

    /**
     * @brief Ensures that the event is generated atleast once after this
     * call was invoked
     *
     * @param time in milliseconds, 0 for time means blocking wait
     *
     * @return HsaEventWaitReturn return value indicating success or timeout.
     */
    virtual HsaEventWaitReturn wait() = 0;
    virtual HsaEventWaitReturn wait(uint32_t timeOut) = 0;
	virtual hsa::AMDRETCODE set() = 0;
	virtual hsa::AMDRETCODE reset() = 0;
	virtual hsa::AMDRETCODE queryState() = 0;
};

class DLL_PUBLIC IQueue
{

public:

    /**
     * @brief Default destructor of Queue interface.
     */
    virtual ~IQueue(){};

    virtual hsa::AMDRETCODE dispatch(void *kinfo, void *prgmptr, unsigned int prgmsize, hsa::Event *eventptr)=0;
    virtual hsa::Event * dispatch(IKernel *k, RTKernelArg arg1, hsa::Event *ievp)=0;
    virtual hsa::Event * dispatch(IKernel * kernel, hsa::Event * depEvent, uint32_t numArgs, ...)=0;
    virtual hsa::Event *dispatch(IDispatchDescriptor * dispDescriptor, LaunchAttributes *launchAttr, hsa::Event * depEvent)=0;
    virtual hsa::Event *dispatch(IKernel * kernel, LaunchAttributes *launchAttr, hsa::Event * depEvent, uint32_t numArgs, ...)=0;
    virtual hsa::Event *dispatch(IKernel* kernel, LaunchAttributes *launchAttr, hsa::Event* depEvent, hsacore::vector<RTKernelArg>& args)=0;
    virtual hsa::AMDRETCODE getScratchUserPtr(void **ptr)=0;
    virtual hsa::AMDRETCODE getScratchSize(unsigned int *scrsize)=0;
    virtual hsa::AMDRETCODE flush()=0;
    virtual IDispatchDescriptor * createDispatchDescriptor(IKernel * kernel, LaunchAttributes *launchAttr, uint32_t numArgs, ...)=0;
    virtual IDispatchDescriptor * getDispatchDescriptor(uint32_t dispatchID) = 0;

};


class DLL_PUBLIC IKernel
{
public:
    virtual ~IKernel(){};
    virtual void setArg(int index, RTKernelArg arg)=0;

    /* This needs to go - should not pass the metadata
    through an interface*/
    //virtual void initArgs(char* argList)=0;

    /*! Returns the number of arguments
    * @return - The number of arguments
    * */
    //virtual int getArgCount()=0;
    
    /*! Returns the offset in the parameter stack
    * @param index - index of the argument
    * @return - The offset in the parameter stack
    * */
    //virtual int getArgOffset(int index)=0;
   
    /* Returns the size of the argument
    * @param index - index of the argument
    * @return - The size of the argument
    * */
    //virtual int getArgSize(int index)=0;
    /*! Returns the type of the argument
    * @param index - index of the argument
    * @return - The type of the argument as a HSAIL_ARG_TYPE enum
    * */
    //virtual HSAIL_ARG_TYPE getArgType(int index)=0;
	/*! Returns the Address Qualifier - global, local etc.
    * @param index - index of the argument
    * @return - The type of the argument as a HSAIL_ADDRESS_QUALIFIER enum
    * */
	//virtual HSAIL_ADDRESS_QUALIFIER getArgAddrQualifier(int index)=0;
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
    virtual hsa::AMDRETCODE createDeviceEvent(hsa::IDevice *d, hsa::Event* &e)=0;
    virtual IKernel *createKernel(IProgram * k, hsa::KernelId & kid)=0;

    /**
     * @copydoc hsa::allocateMemory(const size_t size, const size_t alignment = 0)
     */
    virtual void* 
    allocateMemory(const size_t size, const size_t alignment = 0) = 0;

    /**
     * @copydoc hsa::allocateMemory(const size_t size, const size_t alignment, const uint32_t type, const IDevice& dev)
     */
    virtual void* 
    allocateMemory(const size_t size, const size_t alignment, const uint32_t type, const IDevice& dev) = 0;

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

    /*! Builds and returns a kernel for the list of devices owned 
    * @param kernelName the name of the kernel to build 
    * @param size length of kernel name 
    * @return returns a built kernel for execution
    */
    virtual hsa::IKernel * build(const char* kernelName,size_t size) = 0;
    /*! Add a device to the list of devices a kernel is built against.
    * @param device The device to add.
    */
    virtual void addDevice(hsa::IDevice * device)  = 0;
    /*! Get metadata associated with a kernel 
    * @param d The device the kernel will run on 
    * @param kid the KernelID identiying the kernel
    */
    virtual hsacore::HsailKernel* getMetaData(IDevice *d, KernelId &kid) = 0;
    /*! standard destructor */
    virtual ~IProgram(){};
};

class DLL_PUBLIC IDispatchDescriptor
{
public:
    /*! Allocates the memory for dispatch  */
    virtual void initDispatch () = 0;
    /*! Gives the user the command packet built by the command writer */
    virtual uint32_t GetCommandPkt(uint32_t * buf) = 0;
    /*! Hand off the command to the core queue for execution */
    virtual void execCommandPkt() = 0;
    /*! Wait for notification that the kernel has executed */
    virtual void waitForEndOfKernel() = 0;
	/*! Get the dispatch ID */
	virtual uint32_t getDispatchID() = 0;
    /*! Deallocates memory as necessary */
    virtual ~IDispatchDescriptor(){};
    /*! Deallocates memory as necessary */
	virtual void setupTrapHandler(void *trapHandler, size_t trapHandlerSizeByte) = 0;
    /*! Deallocates memory as necessary */
	virtual void setupTrapHandlerBuffer(void *trapHandlerBuffer, size_t trapHandlerBufferSizeByte) = 0;

};

/**
 * @}
 */
}

#endif
