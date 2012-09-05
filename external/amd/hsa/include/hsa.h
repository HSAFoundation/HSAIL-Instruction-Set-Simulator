//depot/stg/hsa/drivers/hsa/api/hsart/public/hsa.h#27 - edit change 812680 (text)
#ifndef _HSA_H_
#define _HSA_H_


// lhowes: Temporary workaround for tool chain issues
// Once headers finalized we can come up with a cleaner 
// solution if system includes are still necessary
#if !defined(AMD_AMP_HSA_INCLUDES)
#include <vector>
#include <string>
#endif // #if !defined(AMD_AMP_HSA_INCLUDES)

//#include "hsailutil.h"
// lhowes: Temporary workaround for tool chain issues
// Once headers finalized we can come up with a cleaner 
// solution if system includes are still necessary
#if !defined(AMD_AMP_HSA_INCLUDES)
#include <stdint.h>
#include <assert.h>
#endif // #if !defined(AMD_AMP_HSA_INCLUDES)

#include "hsacommon.h"
#include "hsacorecommon.h"


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
class Program;
class DispatchDescriptor;

/**
 * @ingroup Dispatch, Debugger
 * The wave action lets the user chose what happens to a wave when an
 * exception occurs, there are three choices
 */
typedef enum {
    WAVE_ACTION_HALT  =1, /*!< The wave is not killed just suspended. This is the
                               default behavior. Halt the wave on an exception. */
    WAVE_ACTION_RESUME=2, /*!< Resume the wave on an exception The PC is
                               incremented to the next instruction and execution is
                               resumed. This choice is for advanced users. */
    WAVE_ACTION_KILL=3,   /*!< Kill the wave on an exception The wave is killed at
                               the point of exception. If not all the wavefronts in the
                               wave take a fault, the place in the code where the
                               non-faulty waves get killed is non-deterministic. This is
                               usually before the next barrier. */
    WAVE_ACTION_DEBUG=4,  /*!< Cause the wave into the debug mode */
    WAVE_ACTION_TRAP=5,   /*!< Cause the wave to take a trap */
    WAVE_ACTION_MAX
} WaveAction;

/**
 * @ingroup message used by the KFD wave control
 * This structure indicates the various information used
 * by the wave control function. Currnetly, only the
 * WaveID and SIMD item is used.
 */
typedef struct _DbgWaveMsgAMDGen1
{
    union
    {
        struct
        {
            unsigned int  UserData     : 8; // User data
            unsigned int  Reserved1    : 6; // This field is reserved, should be 0
            unsigned int  WaveId       : 4; // Wave id
            unsigned int  SIMD         : 2; // SIMD id
            unsigned int  HSACU        : 4; // Compute unit
            unsigned int  ShaderArray  : 1; // Shader array
            unsigned int  ShaderEngine : 1; // Shader engine
            unsigned int  MessageType  : 2; // see HSA_DBG_WAVEMSG_TYPE
            unsigned int  Reserved2    : 4; // This field is reserved, should be 0
        } ui32;
        uint32_t      Value;
    } ui32;
    uint32_t          Reserved3;

} DbgWaveMsgAMDGen1;
    
typedef struct _DbgWaveMessage
{
    void*                 MemoryVA;         // ptr to associated host-accessible data
    union
    {
        DbgWaveMsgAMDGen1  WaveMsgInfoGen1;
        //for future HW specific "HsaDbgWaveMsgAMDGen2"
    };
} DbgWaveMessage;

/**
 * @ingroup Dispatch, Debugger
 * The wave mode lets the user chose how many waves the message affects
 */
typedef enum {
    WAVEMODE_SINGLE=0,                /*!< send command to a single wave */
    /* broadcast to all wavefronts of all processes is not supported for HSA user mode */
    WAVEMODE_BROADCAST_PROCESS=2,     /*!< send to waves within current process */
    WAVEMODE_BROADCAST_PROCESS_CU=3,  /*!< send to waves within current process */
    MAX_WAVEMODE
} HSAWaveMode;

/**
 * @ingroup Dispatch
 * This is to chose what should happen to the application program, there are
 * three choices
 */
typedef enum {
    HOST_ACTION_IGNORE = 1, /*!< Ignore the kernel exception. The host
                    application will ignore any kernel exceptions. */
    HOST_ACTION_EXIT = 2, /*!< Exit the host application on a kernel exception.
                    The host application will call exit() in the event of a
                    kernel exception. */
    HOST_ACTION_NOTIFY = 4 /*!< Interrupt the host application on a kernel
                    exception. The host application has registered an event to be
                    signaled in the event of a kernel exception. */
} HostAction;

/**
 * @ingroup Dispatch, Debugger
 * This is to chose what should happen to the application program in terms of
 * the data it uses. There are three choices
 * */
typedef enum {
    CACHE_POLICY_FLUSH_ALL=1,          /*!< Flush all caches that can be flushed. */
    CACHE_POLICY_FLUSH_COMPUTE_UNIT=2, /*!< Flush the private device compute
                                            unit caches. */
    CACHE_POLICY_FLUSH_SHARED=4        /*!< Flush the shared device compute unit
                                            caches. */
} CachePolicy;


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
    uint32_t exceptionType;    /*!< exception types enabled */
    WaveAction waveAction;     /*!< default wave action is to halt it */
    HostAction hostAction;     /*!< default host action is to ignore the
                                 event when it occurs it but inform the
                                 host when it does wait on this kernel */
    HSATrapHandle trapHandle;  /*!< if host action is to interrupt, this
                                 function gets executed */

    HSAExceptionPolicy()
    {
        enableException = 0;
        exceptionType = 0x0; 
        waveAction = WAVE_ACTION_HALT;
        hostAction = HOST_ACTION_IGNORE;
        trapHandle = NULL;
    }
}HSAExceptionPolicy;

/**
 * @ingroup Dispatch
 * completion policy defined the following
 */
typedef enum {
    BLOCKING_POLICY_NONE, /*!< The dispatch interface returns immediately. */
    BLOCKING_POLICY_DEPENDENCY=1, /*!< The dispatch interface will block until
                    all dependencies have succeeded before returning from the
                    kernel launch. */
    BLOCKING_POLICY_ENQUEUED=2, /*!< The dispatch interface will block until the
                    kernel has been enqueued to the device queue for
                    execution. */
    BLOCKING_POLICY_EXECUTING=4, /*!< The dispatch interface will block until
                    the kernel dispatch command has been processed and the
                    kernel is running. */
    BLOCKING_POLICY_COMPLETED=8  /*!< The dispatch interface will block until
                    the kernel has completed executing. */
} BlockingPolicy;

/** 
 * index of each type of trap handler 
 */
typedef enum {
    RUNTIME_TRAP   = 0, /*!< level-1, runtime trap handler index */
    DEBUG_TRAP     = 1, /*!< level-2, debug trap handler index */
    SYSCALL_TRAP   = 2, /*!< level-2, syscall trap handler index */
    EXCEPTION_TRAP = 3, /*!< level-2, exception trap handler index */
    MAX_TRAP_NUM        /*!< the number of trap handlers */
} TrapType; 


/** 
 * trap handler and the trap handler buffer info 
 */
typedef struct TrapHandlerInfo_ {
    void *trapHandler;
    void *trapHandlerBuffer;
    size_t trapHandlerSize;
    size_t trapHandlerBufferSize;

    TrapHandlerInfo_()
    {
        trapHandler = NULL;
        trapHandlerBuffer = NULL;
        trapHandlerSize = 0;
        trapHandlerBufferSize = 0;
    }
} TrapHandlerInfo;

/** 
 * attributes of a launch include, exception policy, launch policy and cache
 * policy 
 */
typedef struct LaunchAttributes {
    BlockingPolicy blockingPolicy; /*!< default has none of the
                                            guarantees listed in the
                                            policy*/
    HSAExceptionPolicy exceptionPolicy; /*!< default is to generate
                                          exceptions*/

    void *trapHandler;/*!< trap handler used for GPU exeception and HW debugger */
    
    void *trapHandlerBuffer;/*!< trap handler buffer, accessed by trap handler */
    
    size_t trapHandlerBufferSizeByte; /*!< trap handler buffer size */

    CachePolicy cachePolicy; /*!< default is to flush everything */

    int gridX; /*!< default is 1*/
    int gridY; /*!< default is 1*/
    int gridZ; /*!< default is 1*/
    int groupX; /*!< default is 1*/
    int groupY; /*!< default is 1*/
    int groupZ; /*!< default is 1*/
    int groupOffsets[3];

    LaunchAttributes()
    {
        blockingPolicy = BLOCKING_POLICY_NONE;
        cachePolicy = CACHE_POLICY_FLUSH_ALL;
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
flushDeviceCaches(hsa::Device *dev, CachePolicy cachePolicy);

/**
 * @brief Device class, public interface in the device layer.
 */
class DLL_PUBLIC Device
{
public:
    virtual hsa::DeviceType getType() const = 0;

    virtual hsa::Kernel* compile(
                    hsa::Program* prog,
                    const char* kernelName,
                    const char* options)=0;

    virtual unsigned int getComputeUnitsCount()=0;

    virtual uint32_t getCapabilities()=0;

    virtual string& getName()=0;

    virtual const hsa::vector<hsa::MemoryDescriptor*>& getMemoryDescriptors()=0;

    virtual const hsa::vector<hsa::CacheDescriptor*>& getCacheDescriptors()=0;

    virtual bool isDoublePrecision()=0;

    virtual bool isDebug()=0;

    virtual bool isDedicatedCompute()=0;

    virtual uint32_t getMaxGroupMemorySize()=0;

    // Get the device information in bulk.
    //virtual const Info& info()=0;

    //Temp hack to get the asicinfo
    virtual void* getASICInfo()=0;

    /**
     * @brief Set up trap handler in the current device
     *
     * @param trapHandler pointer to the trap handler, this address needs
     * to be 256-byte aligned.
     *
     * @param trapHandlerSizeByte size of the trap handler in bytes
     *
     * @param trapType different types of trap handler, currently, support runtime,  
     *  debugger, exception, system call.
     */
    virtual void setupTrapHandler(void *trapHandler, size_t trapHandlerSizeByte, uint32_t trapType) = 0;

	/**
     * @brief Get the wavefront size
     *
     * @return returns the size of wavefront.
     */
    virtual int getWaveFrontSize()=0;

    /**
     * @brief Set up trap handler buffer in the current device
     *
     * @param trapHandlerBuffer pointer to the trap handler buffer, this 
     * address needs to be 256-byte aligned.
     *
     * @param trapHandlerBufferSizeByte size of the trap handler buffer in bytes
     *
     * @param trapType different types of trap handler, currently, support runtime,  
     *  debugger, exception, system call.
     */
    virtual void setupTrapHandlerBuffer(void *trapHandlerBuffer, size_t trapHandlerBufferSizeByte, uint32_t trapType) = 0;

    /**
     * @brief Set up trap handler and buffer in the current device
     *
     * @param trapHandler pointer to the trap handler, this address needs
     * to be 256-byte aligned.
     *
     * @param trapHandlerSizeByte size of the trap handler in bytes
     *
     * @param trapHandlerBuffer pointer to the trap handler buffer, this 
     * address needs to be 256-byte aligned.
     *
     * @param trapHandlerBufferSizeByte size of the trap handler buffer in bytes
     *
     * @param trapType different types of trap handler, currently, support runtime,  
     *  debugger, exception, system call.
     */
    virtual void setupTrapHandlerAndBuffer(void *trapHandler, size_t trapHandlerSizebyte, 
                                           void *trapHandlerBuffer, size_t trapHandlerBufferSizeByte, uint32_t trapType) = 0;

    /**
     * @brief retrieve all the trap handler information in the device
     *
     * @param trapHandlerInfo pointer to the trap handler info buffer
     */
    virtual void getTrapHandlerInfo(TrapHandlerInfo * trapHandlerInfo) = 0;

    /**
     * @brief Each device has a list to store all the queues created on
     *  the device. When a queue is created, this function is called
     *  to add the queue to the list.
     *
     * @param queue pointer to the queue created on the device
     */
    virtual void addQueueToList(hsa::Queue *queue) = 0;

    /**
     * @brief Each dispatch has a dispatch ID, this function is to
     *  get the dispatch descriptor based on the dispatch ID.
     *
     * @param dispatchID dispatch ID
     *
     * @return DispatchDescriptor return the pointer to the dispatch descriptor
     */
    virtual DispatchDescriptor * findDispatchDescriptor(uint32_t dispatchID) = 0;

    /**
     * @brief Invalidate all cache on the GPU.
     *
     * @param dev pointer to device
     *
     * @param cachePolicy indicate how the cache should be invalidated.
     */
    virtual void flushCaches(hsa::Device *dev, CachePolicy cachePolicy) = 0;

    /**
     * @brief control the wave front.
     *
     * @param action actions to be taken on the wavefront
     *
     * @param mode how the actions are taken, single wave, broadcast, etc.
     *
     * @param trapID, this is used for just the action of h_trap, in which
     *  a trap ID is needed.
     * 
     * @param msgPtr, pointer to a message indicate various information. 
     *  see the KFD design for specific information.
     */
    virtual void waveControl(uint32_t action, 
                             uint32_t mode, 
                             uint32_t trapID, 
                             void *msgPtr) = 0;

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
    virtual void getTriggerInfo(uint32_t &krnlValue, uint64_t &krnlAddr) = 0;

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

    /**
     * @brief Returns the compute device with which the Queue object
     * is associated with.
     *
     * @note: The Api will throw an exception if the Queue object is
     * invalid i.e. it has been destroyed.
     *
     * @return hsa::Device * pointer to the compute device
     */
    virtual hsa::Device *getDevice(void) = 0;

    virtual hsa::Event *dispatch(Kernel * kernel, 
                        hsa::Event * depEvent, 
                        uint32_t numArgs, ...)=0;

    virtual hsa::Event *dispatch(DispatchDescriptor * dispDescriptor, 
                        LaunchAttributes *launchAttr, 
                        hsa::Event * depEvent)=0;
    
    virtual hsa::Event *dispatch(Kernel * kernel, 
                        LaunchAttributes *launchAttr, 
                        hsa::Event * depEvent, 
                        uint32_t numArgs, 
                        ...)=0;
    
    virtual hsa::Event *dispatch(Kernel* kernel, 
                        LaunchAttributes *launchAttr,
                        hsa::Event* depEvent, 
                        hsacore::vector<RTKernelArgs>& args)=0;
    
    virtual void flush()=0;

    /**
     * @brief Set up trap handler in the current device
     *
     * @param trapHandler pointer to the trap handler, this address needs
     * to be 256-byte aligned.
     *
     * @param trapHandlerSizeByte size of the trap handler in bytes
     *
     * @param trapType different types of trap handler, currently, support runtime,  
     *  debugger, exception, system call.
     */
    virtual void setupTrapHandler(void *pTrapHandler, size_t trapHandlerSizeByte, uint32_t trapType) = 0;

    /**
     * @brief Set up trap handler buffer in the current device
     *
     * @param trapHandlerBuffer pointer to the trap handler buffer, this 
     * address needs to be 256-byte aligned.
     *
     * @param trapHandlerBufferSizeByte size of the trap handler buffer in bytes
     *
     * @param trapType different types of trap handler, currently, support runtime,  
     *  debugger, exception, system call.
     */
    virtual void setupTrapHandlerBuffer(void *pTrapHandlerBuffer, size_t trapHandlerBufferSizeByte, uint32_t trapType) = 0;

    /**
     * @brief create a dispatch descriptor
     *
     * @param kernel pointer to the kernel to be dispatched
     *
     * @param launchAttr pointer to a structure containing various kernel execution info
     *
     * @param numArgs number of arguments for the kernel
     *
     * @return DispatchDescriptor return the pointer to the dispatch descriptor
     */
    virtual hsa::DispatchDescriptor * createDispatchDescriptor(hsa::Kernel * kernel, LaunchAttributes *launchAttr, uint32_t numArgs, ...)=0;
    
    /**
     * @brief Each dispatch has a dispatch ID, this function is to
     *  get the dispatch descriptor based on the dispatch ID.
     *
     * @param dispatchID dispatch ID
     *
     * @return DispatchDescriptor return the pointer to the dispatch descriptor
     */
    virtual hsa::DispatchDescriptor * findDispatchDescriptor(uint32_t dispatchID) = 0;

};


class DLL_PUBLIC Kernel
{
public:
    
    /*! @brief destructor
    */

    virtual ~Kernel(){};

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
    virtual hsa::Program* createProgram(char *charElf, 
                                        size_t elfSize,
                                        Device_list_ptr pDevices)=0;

    virtual hsa::Program* createProgramFromFile(const char* fileName,
        Device_list_ptr pDevices)=0;
    virtual void destroyProgram(hsa::Program*)=0;

 

    /**
     * @brief Get the runtime API version
     *
     *
     * @return runtime API version.
     */
    virtual const string getVersion()=0;

    virtual hsa::Event* createDeviceEvent(hsa::Device *d)=0;
//    virtual Kernel *createKernel(Program * k, hsa::KernelId & kid)=0;

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

class DLL_PUBLIC Program
{
public:

    /*! @brief Builds and returns a kernel for the list of devices owned 
    * @param kernelName the name of the kernel to build 
    * @param size length of kernel name 
    * @return returns a built kernel for execution
    */

    virtual hsa::Kernel *compileKernel(const char* kernelName,
                               const char* options) = 0;
    /*! @brief Add a device to the list of devices a kernel is built against.
    * @param device The device to add.
    */

    virtual void addDevice(hsa::Device * device)  = 0;

    /*! @brief Get debug information for the particular device
    * @param d The device 
    * @param kid kernel name
    * @return The pointer to the debug info - Might have to be void*
    */

    /*! @brief Return a pointer to the elf
    *  @return elf pointer
    */

    virtual char* getElf()=0;

    /*! @brief Returns size of the elf
    *  @return The size of the elf
    */

    virtual size_t getElfSize()=0;

    /*! @brief Returns the base address for all
     *  globals associated with this memory
     *  @return The size of the elf
     */

    virtual void* getGlobalBase()=0;
    
    /*! @brief standard destructor */

    virtual ~Program(){};

};

/**
 * @ingroup Debugger, Dispatch
 */
class DLL_PUBLIC DispatchDescriptor
{

public:

    /**
     * @brief Destructor for the kernel dispatch info descriptor.
     * The implmentation will release all resources, memory
     * devices, queues, etc that have been acquired in support
     * of kernel dispatch.
     */
    virtual ~DispatchDescriptor() {};

    /**
     * @brief Returns the unique Id of HsaRt kernel dispatch.
     *
     * @return uint32_t Id of kernel dispatch.
     */
    virtual uint32_t getDispatchID(void) = 0;

    /**
     * @brief Initializes the various entities, memory, command
     * writer, etc that are needed to launch and execute a HsaRt
     * kernel.
     */
    virtual bool initDispatch (void) = 0;

    /**
     * @brief Executes the various steps to submit and execute a
     * HsaRt kernel object. It is necessary that the descriptor
     * be initialized prior to this call.
     *
     * @return bool true if kernel is submitted to compute engine
     * successfully, false otherwise.
     */
    virtual bool launchDispatch(void) = 0;

    /**
     * @brief Returns a boolean flag to indicate if the kernel has
     * completed or not.
     *
     * @return bool true if the kernel has completed, false otherwise.
     */
    virtual bool hasKrnlCompleted(void) = 0;
	
    /**
     * @brief Blocking api that waits for the kernel to complete.
     */
    virtual void waitForEndOfKernel(void) = 0;
    
    /**
     * @brief Returns address and size of Kernel Launch packet. This is
     * provided as convenience to allow users to execute this packet
     * directly.
     *
     * @note: It is possible that the memory block associated with
     * launch packet could be released. Also users could modify the
     * packet with invalid instructions.
     *
     * @param launchAddr output parameter updated with address of kernel
     * launch packet.
     *
     * @return uint32_t size of kernel launch packet in 32-bit word size.
     */ 
    virtual uint32_t getLaunchAddr(uint32_t *launchAddr) = 0;
    
    /**
     * @brief Returns the memory block used by HsaRt kernel for temporary
     * computation results. Debugger uses this info to support the querying
     * of kernel parameters.
     *
     * @note: In Debug mode the memory used by kernel for various entities
     * should be of type SYSTEM_MEMORY since it should be accessible to
     * both Cpu and Gpu.
     *
     * @param hiAddr the upper 32 bits of the scratch memory block
     *
     * @param lowAddr the lower 32 bits of the scratch memory block
     *
     * @return void
     */
    virtual void getScratchAddress(uint32_t &hiAddr, uint32_t &lowAddr) = 0;
    
    /**
     * @brief The debugger needs to know the scratch memory descriptor
     * used for debugger trap handler. A memory description consists
     * of four registers, which are:
     *  
     *      SQ_BUF_RSRC_WORD0,
     *      SQ_BUF_RSRC_WORD1,
     *      SQ_BUF_RSRC_WORD2,
     *      SQ_BUF_RSRC_WORD3
     *
     * @note: In Debug mode the memory used by kernel for various entities
     * should be of type SYSTEM_MEMORY since it should be accessible to
     * both Cpu and Gpu.
     *
     * @param scratchDescr pointer to the four components of scratch memory
     * descriptor
     *
     * @return void
     */
    virtual void getScratchMemoryDescriptor(uint32_t *scratchDescr) = 0;
    
    /**
     * @brief Set up trap handler in the current device
     *
     * @param trapHandler pointer to the trap handler, this address needs
     * to be 256-byte aligned.
     *
     * @param trapHandlerSizeByte size of the trap handler in bytes
     *
     * @param trapType different types of trap handler, currently, support runtime,  
     *  debugger, exception, system call.
     */
    virtual void setupTrapHandler(void *trapHandler, size_t trapHandlerSizeByte, uint32_t trapType) = 0;
    
    /**
     * @brief Set up trap handler buffer in the current device
     *
     * @param trapHandlerBuffer pointer to the trap handler buffer, this 
     * address needs to be 256-byte aligned.
     *
     * @param trapHandlerBufferSizeByte size of the trap handler buffer in bytes
     *
     * @param trapType different types of trap handler, currently, support runtime,  
     *  debugger, exception, system call.
     */
    virtual void setupTrapHandlerBuffer(void *trapHandlerBuffer, size_t trapHandlerBufferSizeByte, uint32_t trapType) = 0;

    /**
     * @brief retrieve all the trap handler information in the device
     *
     * @param trapHandlerInfo pointer to the trap handler info buffer
     */
    virtual void setupTrapHandlerInfo(TrapHandlerInfo *pTrapHandlerInfo) = 0;

    /**
     * @brief retrieve the ISA pointer and size that are dispatched to the kernel
     *
     * @param ptr pointer to the ISA in the dispatch descriptor
     *
     * @param size size of the ISA
     */
    virtual void getISA(void * &ptr, size_t &size) = 0;

	/**
     * @brief, allows the query of the index of the SGPR containing the scratch 
     * the scratch bufffer offset for each wave
     */
    virtual uint32_t getScratchBufferWaveOffsetSgprIndex() = 0;

    /**
     * @brief, DXX will reserve 4 consecutive VGPRs for use by the trap handler
     * for debugger shaders. The ELF section will consist of a 32-bit unsigned
     * interger in little endian oder specifying the index of the first VGPR
     */
    virtual uint32_t getTrapReservedVgprIndex() = 0;

    /**
     * @brief returns the pointer to ELF so debugger or other tools that
     * received asynchronous notification about this dispatch can do
     * deep-dives
     */
    virtual void * getBoundShaderBinary() = 0;

    /**
     * @brief set the kernel to execute in the debug mode
     */
    virtual void enableDebugMode() = 0;

    /**
     * @brief disable the debug mode
     */
    virtual void disableDebugMode() = 0;

    /**
     * @brief enable the exception types
     *
     * @param excp_en types of exception to be enabled
     */
    virtual void enableExceptionTypes(uint32_t excp_en) = 0;

    /**
     * @brief control the wave front.
     *
     * @param action actions to be taken on the wavefront
     *
     * @param mode how the actions are taken, single wave, broadcast, etc.
     *
     * @param trapID, this is used for just the action of h_trap, in which
     *  a trap ID is needed.
     * 
     * @param msgPtr, pointer to a message indicate various information. 
     *  see the KFD design for specific information.
     */
    virtual void waveControl(uint32_t action, 
                             uint32_t mode, 
                             uint32_t trapID, 
                             void *msgPtr) = 0;
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
     * @brief Updates output parameters witht Hsa Kernel Module - HKM
     * provided values. These values are needed to trigger an interrupt
     * via End Of Pipe (EOP) packet.
     *
     * @param eventID event ID to be written to a specific location.
     *
     * @param eventAddr address where the eventID is written to.
     *
     * @return void
     */
    virtual void getTriggerInfo(uint32_t &eventID, uint64_t &eventAddr) = 0;

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


/* @ brief Creates a Program object from an ELF
 * @param charElf - Pointer to an ELF
 * @param elfSize - size of the ELF
 * @param pDevices - pointer to a list of devices
*/

DLL_PUBLIC hsa::Program* createProgram(char *charElf, 
                                        size_t elfSize,
                                        Device_list_ptr pDevices);

/* @ brief Creates a Program object from a File
 * @param fileName - Name of the file
 * @param pDevices - pointer to a list of devices
*/

DLL_PUBLIC hsa::Program* createProgramFromFile(
                                const char* fileName,
                                Device_list_ptr pDevices);

/* @ brief Destroys a a program
 * @param prog pointer to the program
 * @param pDevices - pointer to a list of devices
*/

DLL_PUBLIC void destroyProgram(hsa::Program* prog);


/**
 * @}
 */
}

#endif
