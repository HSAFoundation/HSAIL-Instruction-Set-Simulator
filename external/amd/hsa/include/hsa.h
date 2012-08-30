//depot/stg/hsa/drivers/hsa/api/hsart/public/hsa.h#15 - edit change 794751 (text)
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


class Queue;
class Kernel;
class Event;
class DebuggerEvent;
class Device;
class _Program;
class DispatchDescriptor;

/**
 * @ingroup Dispatch, Debugger
 * The wave action lets the user chose what happens to a wave when an
 * exception occurs, there are three choices
 */
typedef enum {
    HALT_WAVE=1, /*!< Halt the wave, usually this is what happens, 
                   wave is not killed, just halted */
    KILL_WAVE=2, /*!< Kill the wave, wave is killed at the point of
                   exception, if not all the wavefronts in the wave take a
                   fault, the place in the code where the non-faulty waves
                   get killed is not deterministic. It usually happens
                   before the next barrier */
    RESUME_WAVE=4 /*!< increment the PC to the next instruction and
                    continue running, this choice is for advanced users */
} HSAWaveAction;


/**
 * @ingroup Dispatch
 * This is to chose what should happen to the application program, there are
 * three choices
 */
typedef enum {
    IGNORE_HOST = 1,   /*!< the application doesn't want to know about the
                        exception */
    EXIT_HOST = 2,     /*!< the application wants runtime to call exit() */
    INTERRUPT_HOST = 4 /*!< the host is expecing it be interrupted, host has
                        registered a call back for this via events */
} HSAHostAction;

/**
 * @ingroup Dispatch, Debugger
 * This is to chose what should happen to the application program in terms of
 * the data it uses. There are three choices
 * */
typedef enum {
    FLUSH_ALL=1,         /*!< flush all the caches the runtime can flush*/
    FLUSH_BOTTOM_LEVEL=2,/*!< flush flushable caches closer to the processing
                            units */
    FLUSH_TOP_LEVEL=4    /*!< flush top-level cache */
} HSACachePolicy;


/**
 * @ingroup Dispatch, Debugger
 * the function pointer for registering handlers associated with events in HSA
 */
typedef void (*HSATrapHandle)(void *,int);

/**
 * @ingroup Dispatch, Debugger
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
                                 function gets executed */

    HSAExceptionPolicy()
    {
        int enableException = 1;
        waveAction = HALT_WAVE;
        hostAction = IGNORE_HOST;
        trapHandle = NULL;
    }
}HSAExceptionPolicy;

/**
 * @ingroup Dispatch
 * completion policy defined the following
 */
typedef enum {
    WAIT_DEPENDENCY=1, /*!< return from launch only after waiting for all
                          dependencies */
    WAIT_ENQUEUE=2,    /*!< return from launch only after enqueue is intiated */
    WAIT_LAUNCH=4,     /*!< return from launch after the kernel launch is
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

    void *trapHandler;/*!< trap handler used for GPU exeception and HW debugger */
    
    void *trapHandlerBuffer;/*!< trap handler buffer, accessed by trap handler */
    
    size_t trapHandlerBufferSizeByte; /*!< trap handler buffer size */

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

typedef hsa::vector<hsa::Device*>::const_iterator Device_itr;
typedef hsa::vector<hsa::Device*> Device_list;
typedef hsa::vector<hsa::Device*>* Device_list_ptr;
typedef std::string KernelId;

/**
 * @copydoc hsacore::allocateMemory(const size_t size, const size_t alignment = 0)
 * @ingroup Memory
 */
DLL_PUBLIC void* allocateMemory(const size_t size, const size_t alignment = 0);

/**
 * @copydoc hsacore::allocateMemory(const size_t size, const size_t alignment, const uint32_t type, const Device& dev)
 */
DLL_PUBLIC void* allocateMemory(const size_t size, const size_t alignment, const uint32_t type, const Device& dev);

/**
 * @ingroup Memory
 * @copydoc hsacore::freeMemory
 */
DLL_PUBLIC void freeMemory(void* ptr);

/**
 * @ingroup Memory
 * @copydoc hsacore::registerMemory
 */
DLL_PUBLIC void registerMemory(
                void* ptr, 
                const size_t size,
                const uint32_t type);

/**
 * @ingroup Memory
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
 * @ingroup Debugger
 * @brief flush device caches 
 * The device cache may be flushed at any time by the debugger. 
 * The HSA runtime create a private high-priority queue that it uses to send
 * commands down to the device. Cache fluses done by this API use this private
 * queue.
 *
 * @param dev the HSA device
 * @param HSACachePolicy indicates the cache policy, which is users choice
 *
 */
DLL_PUBLIC void
flushDeviceCaches(hsa::Device *dev, HSACachePolicy cachePolicy);

/**
 * @brief Device class, public interface in the device layer.
 */
class DLL_PUBLIC Device
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
    virtual void setupTrapHandler(void *trapHandler, size_t trapHandlerSizeByte) = 0;

    //setup trap handler buffer
    virtual void setupTrapHandlerBuffer(void *trapHandlerBuffer, size_t trapHandlerBufferSizeByte) = 0;

    //setup trap handler buffer
    virtual void setupTrapHandlerAndBuffer(void *trapHandler, size_t trapHandlerSizebyte, 
                                           void *trapHandlerBuffer, size_t trapHandlerBufferSizeByte) = 0;

    //get trap handler and its size
    virtual void getTrapHandler(void* &trapHandler, size_t &trapHandlerSizeByte) = 0;

    //get the trap handler buffer and its size
    virtual void getTrapHandlerBuffer(void* &trapHandlerBuffer, size_t &trapHandlerBufferSizeByte) = 0;

    //add queue to list
    virtual void addQueueToList(hsa::Queue *queue) = 0;

    //find dispatch descriptor from dispatch ID
    virtual DispatchDescriptor * findDispatchDescriptor(uint32_t dispatchID) = 0;

    virtual ~Device(){};
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


class DLL_PUBLIC Queue
{

public:

    /**
     * @brief Default destructor of Queue interface.
     */
    virtual ~Queue(){};

    virtual hsa::Event *dispatch(Kernel * kernel, hsa::Event * depEvent, uint32_t numArgs, ...)=0;

    virtual hsa::Event *dispatch(DispatchDescriptor * dispDescriptor, LaunchAttributes *launchAttr, hsa::Event * depEvent)=0;
    
    virtual hsa::Event *dispatch(Kernel * kernel, LaunchAttributes *launchAttr, hsa::Event * depEvent, uint32_t numArgs, ...)=0;
    
    virtual hsa::Event *dispatch(Kernel* kernel, LaunchAttributes *launchAttr, hsa::Event* depEvent, hsacore::vector<RTKernelArgs>& args)=0;
    
    virtual void flush()=0;

    virtual void setupTrapHandler(void *pTrapHandler, size_t trapHandlerSizeByte) = 0;

    virtual void setupTrapHandlerBuffer(void *pTrapHandlerBuffer, size_t trapHandlerBufferSizeByte) = 0;
    
    virtual hsa::DispatchDescriptor * createDispatchDescriptor(hsa::Kernel * kernel, LaunchAttributes *launchAttr, uint32_t numArgs, ...)=0;
    
    virtual hsa::DispatchDescriptor * findDispatchDescriptor(uint32_t dispatchID) = 0;

};


class DLL_PUBLIC Kernel
{
public:
    virtual ~Kernel(){};
    virtual void setArg(int index, RTKernelArgs arg)=0;

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

class RuntimeApi;

extern "C" {

// getRuntime() function:
// When the HSA Runtime device DLL is explicitly loaded, this will be the only function called using GetProcAddress().
// All other global exported functions will be called via HsaRuntimeApi singleton instance returned by this function.
DLL_PUBLIC hsa::RuntimeApi* getRuntime();

// typedef comes handy when GetProcAddress() returns the func pointer which needs to be cast.
typedef hsa::RuntimeApi* (*fptr_getRuntime)();
 
}

class DLL_PUBLIC RuntimeApi  
{
public:
    // All the exported global functions must have corresponding pure virtual public method declared in this interface class.
    virtual uint32_t getDeviceCount()=0;
    virtual const hsa::vector<hsa::Device*>& getDevices()=0;
    virtual hsa::Queue* createDeviceQueue(hsa::Device *d, unsigned int size)=0;
    virtual _Program* createProgram(char *charElf, size_t elfSize, Device_list_ptr pDevices)=0;
    virtual _Program* createProgramFromELF(void *charElf, Device_list_ptr pDevices)=0;
    virtual hsa::Event* createDeviceEvent(hsa::Device *d)=0;
    virtual Kernel *createKernel(_Program * k, hsa::KernelId & kid)=0;

    /**
     * @copydoc hsa::allocateMemory(const size_t size, const size_t alignment = 0)
     */
    virtual void* 
    allocateMemory(const size_t size, const size_t alignment = 0) = 0;

    /**
     * @copydoc hsa::allocateMemory(const size_t size, const size_t alignment, const uint32_t type, const Device& dev)
     */
    virtual void* 
    allocateMemory(const size_t size, const size_t alignment, const uint32_t type, const hsa::Device& dev) = 0;

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

    virtual ~RuntimeApi(){};
};

class DLL_PUBLIC _Program
{
public:

    /*! Builds and returns a kernel for the list of devices owned 
    * @param kernelName the name of the kernel to build 
    * @param size length of kernel name 
    * @return returns a built kernel for execution
    */

    virtual hsa::Kernel * build(const char* kernelName,size_t size) = 0;
    /*! Add a device to the list of devices a kernel is built against.
    * @param device The device to add.
    */

    virtual void addDevice(hsa::Device * device)  = 0;
    /*! Get metadata associated with a kernel 
    * @param d The device the kernel will run on 
    * @param kid the KernelID identiying the kernel
    */

    virtual hsacore::HsailKernel* getMetaData(Device *d, KernelId &kid) = 0;
    /*! standard destructor */
    virtual ~_Program(){};
};

/**
* @ingroup Debugger, Dispatch
*/
class DLL_PUBLIC DispatchDescriptor
{
public:
    /*! Allocates the memory for dispatch  */
    virtual void initDispatch () = 0;
    
    /*! Gives the user the command packet built by the command writer */
    virtual uint32_t GetCommand(uint32_t * buf) = 0;
    
    /*! Hand off the command to the core queue for execution */
    virtual void execCommand() = 0;
    
    /*! Wait for notification that the kernel has executed */
    virtual void waitForEndOfKernel() = 0;
	
    /*! Get the dispatch ID */
    virtual uint32_t getDispatchID() = 0;
    
    /*! Deallocates memory as necessary */
    virtual ~DispatchDescriptor(){};
    
    /*! setup trap handler*/
    virtual void setupTrapHandler(void *trapHandler, size_t trapHandlerSizeByte) = 0;
    
    /*! associate buffer with trap handler */
    virtual void setupTrapHandlerBuffer(void *trapHandlerBuffer, size_t trapHandlerBufferSizeByte) = 0;

    /**
     * @brief function to get the location of the current ISA as used in the
     * dispatch
     * This function flushes the device caches before returning and hence is
     * not a simple getter. The debugger has the ability to query the
     * descriptor of a dispatch from any debug event to lead it to the actual
     * dispatch in the runtime that caused the debug event. Once debugger has
     * this dispatch descriptor, it may query the pointer and size of the ISA,
     * modify the ISA if need be and return the control to the user. The SIZE
     * of the ISA or the relative location of non-nop and nop instructions
     * MUST NOT BE CHANGED! The debugger is only allowed to replace NOPs with
     * S_TRAP. SIMM16 must have 11111111 in relevent bits (0:7). 
     * @param ptr the location of the ISA
     * @param size the size of the ISA
     */
    virtual void getISA(void * &ptr, uint32_t &size) = 0;
    
    /**
     * @brief the debugger needs to know the scratch address that was given to
     * this kernel for this dispatch. 
     * When in debug model _ONLY_ SYSTEM_MEMORY may be used for any dispatch
     * resources. This API points debugger to the starting location of the
     * scratch used for this dispatch.  
     * @param addr_hi the high 32 bits of the address 
     * @param addr_lo the low 32 bits of the address
     */
    virtual void getScratchAddress(uint32_t &addr_hi, uint32_t &addr_lo) = 0;
    
    /**
     * @brief, allows the query of the scratch offset within the scratch space
     * allocated for each kernel by each thread
     * @param waveid ID of the wave
     * @param groupID the groupID (global)
     * @param threadID the thread ID within a group
     */
    virtual uint32_t getScratchWaveOffset(uint32_t waveID, uint32_t groupID, uint32_t threadID) = 0;
    
    /**
     * @brief returns the pointer to ELF so debugger or other tools that
     * received asynchronous notification about this dispatch can do
     * deep-dives
     */
    virtual void * getELF() = 0;
    
    /**
     * @brief API to give the user an ability to control the wave
     *
     * The entire dispatch (entire "grid") can be controlled via WaveAction
     * choices
     * @param waveID the ID of a wave
     * @param waveAct the response
     */
    virtual void waveControl(uint32_t waveID,HSAWaveAction waveAct) = 0;

};

/**
 * @ingroup Debugger
 * the debugger event class, that internally extends from the common runtime
 * event
 */
class DLL_PUBLIC DebuggerEvent
{
public:
    /**
     * @brief Default destructor of Event interface.
     */
    virtual ~DebuggerEvent(){};

    /**
     * @brief wait for an event to be generated by debug activity
     *
     * @param time in milliseconds, 0 for time means blocking wait
     *
     * @return HsaEventWaitReturn return value indicating success or timeout.
     */
    virtual HsaEventWaitReturn wait() = 0;
    
    virtual HsaEventWaitReturn wait(uint32_t timeOut) = 0;
	
    /**
     * @brief explicitly set the event, forcefully. Useful in graceful exits
     *
     * This function sets the event without consideration to if an event is
     * pending, if there is a waiter, or if debugger is running
     *
     * @throws a derivative of std::exception if event is not valid
     */
    virtual void set() = 0;
	
    /**
     * @brief explicitly set the event, forcefully. Useful in graceful
     * recovery
     *
     * This function resets the event without consideration to if an event is
     * pending, if there is a waiter, or if debugger is running
     *
     * @throws a derivative of std::exception if event is not valid
     */
    virtual void reset() = 0;
    
    /**
     * @brief query the current state of the event
     *
     * @throws exception if event is not valid
     */
    virtual void queryState() = 0;
     
    /**
     * @brief get the actual ID of the wave that caused this event in the
     * first place
     *
     * @there may be scenarious where this cannot be obtained. One example is
     * when the wave gets killed
     */ 
    virtual uint32_t getActiveWaveID() = 0;

    /**
     * @brief get the dispatch descriptor (a runtime structure) of the
     * dispatch that caused the creation of the wave that caused this event to
     * occur
     *
     * this allows for a lot of info to be gathered about the dispatch. All
     * the operations that can be performed via a dispatch descriptor may be
     * performed. Resources used by runtime for a dispatch are not freed until
     * a dispatch descriptor for that dispatch stays active. Hence info about
     * argument buffers, global variables, etc., may also be obtained
     *
     */ 
    virtual DispatchDescriptor * getDispatchDescriptor() = 0;
    
    /*
     * @brief API to get the PC at which debug activity occured
     *
     * THIS API will be modified to return array of PCs and a corresponding
     * array of group IDs of the wavefronts to which these PCs belong to
     */
    virtual void getPC(uint32_t waveID, uint32_t &pc_hi, uint32_t &pc_lo) = 0;

};

/**
 * @ingroup Debugger
 * @brief APIs for creating debug event
 *
 * @param dev pointer to the HSA runtime device
 *
 * @param manualReset if this parameter is true the function creates a
 * manual-reset event object; if this parameter is false the function creates
 * an auto-reset event object.  
 *
 * @param isSignaled if this parameter is true, the initial state of the event
 * is signaled, otherwise is nonsignaled.
 */
DLL_PUBLIC hsa::DebuggerEvent * 
createDebuggerEvent(
                hsa::Device *dev, 
                bool manualReset, 
                bool state
                );

/**
 * @ingroup Debugger
 * @brief APIs for creating debug event
 *
 * This function is a work around to create a debugger event when the KFD
 * event mechanism does not work. In this approach, the user provides a buffer
 * that can be accessed by the trap hander, which will write a specific value
 * to a specific location in the buffer in the trap handler. Then the event
 * wait function can poll the specific location to check whether trap handler
 * is executed to the location where the write buffer instruction is executed
 * (With the KFD mechanism, the s_sendmsg instruction is executed instead of
 * writing a value to the buffer.).
 *
 * @param dev pointer to the HSA runtime device
 *
 * @param userBuffer  pointer to the trap handler buffer to mock the event
 * mechanism. This buffer should be the trap handler buffer that can be
 * accessed by the trap handler.
 * 
 * @param manualReset if this parameter is true the function creates a
 * manual-reset event object; if this parameter is false the function creates
 * an auto-reset event object.  
 *
 * @param isSignaled if this parameter is true, the initial state of the event
 * is signaled, otherwise is nonsignaled.
 * 
 *
 */
DLL_PUBLIC hsa::DebuggerEvent * 
createDebuggerEvent(
                hsa::Device *dev, 
                void *userBuffer, 
                uint32_t writeBackLoc,
                uint32_t writeBackValue,
                bool manualReset, 
                bool state
                );

/**
 * @}
 */
}

#endif
