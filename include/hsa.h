//===- hsa.h --------------------------------------------------------------===//
//
//                     The HSA Simultor
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

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

namespace hsa
{

/**
 * Define vector and string as entities from hsa namespace
 */
using hsacommon::vector;
using hsacommon::string;
using hsacommon::Status;

/**
 * @addtogroup HSA_PUBLIC Public HSA Device interfaces
 *
 * @{
 *
 */

class Queue;
class Kernel;
class Event;
class TrapEvent;
class Device;
class Program;

/**
 * @brief Specifies the various states a Hsa Runtime
 * task will go through in execution.
 */
typedef enum {
    STATE_INITITIATED,   // Command initiated (time of API call)
    STATE_BLOCKED,       // Command blocked on dependencies
    STATE_SUBMITTED,     // Command passed to hardware
    STATE_STARTED,       // Command processing started
    STATE_COMPLETED      // Command processing complete
} EventState;


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
 * @ingroup Dispatch, Debugger
 * The wave mode lets the user chose how many waves the message affects
 */
typedef enum {
    WAVE_MODE_SINGLE=0,                /*!< send command to a single wave */
    /* broadcast to all wavefronts of all processes is not supported for HSA user mode */
    WAVE_MODE_BROADCAST_PROCESS=2,     /*!< send to waves within current process */
    WAVE_MODE_BROADCAST_PROCESS_CU=3,  /*!< send to waves within current process */
    MAX_WAVE_MODE
} WaveMode;

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
 * the exception policy is comprised of wave action, host action and the
 * location of trap handle
 */
typedef struct ExceptionPolicy_ {
    uint32_t exceptionMask;       /*!< default is to enable them */
    uint32_t exceptionType;    /*!< exception types enabled */
    WaveAction waveAction;     /*!< default wave action is to halt it */
    WaveMode   waveMode;       /*!< default wave mode is to single */
    HostAction hostAction;     /*!< default host action is to ignore the
                                 event when it occurs it but inform the
                                 host when it does wait on this kernel */

    ExceptionPolicy_()
    {
        exceptionMask = 0;
        exceptionType = 0x0;
        waveAction = WAVE_ACTION_HALT;
        waveMode   = WAVE_MODE_SINGLE;
        hostAction = HOST_ACTION_IGNORE;
    }
} ExceptionPolicy;

/**
 * @ingroup Event
 * the timeinfo is always given in absolute device time with the exception of the queueCPUTime.
 */
typedef struct _EventTimeInfo
{
    uint64_t * hostTime;
    uint64_t * queuedTime;
    uint64_t * submittedTime;
    uint64_t * runningTime;
    uint64_t * completeTime;

    _EventTimeInfo()
    {
        hostTime = NULL;
        queuedTime = NULL;
        submittedTime = NULL;
        runningTime = NULL;
        completeTime = NULL;
    }

} EventTimeInfo;

/**
 * @ingroup Dispatch
 * completion policy defined the following
 */
typedef enum {

    BLOCKING_POLICY_NONE, /*!< The dispatch interface returns immediately. */

    #ifdef HSA_EXPERIMENTAL
    BLOCKING_POLICY_DEPENDENCY=1, /*!< The dispatch interface will block until
                    all dependencies have succeeded before returning from the
                    kernel launch. */
    BLOCKING_POLICY_ENQUEUED=2, /*!< The dispatch interface will block until the
                    kernel has been enqueued to the device queue for
                    execution. */
    BLOCKING_POLICY_EXECUTING=4, /*!< The dispatch interface will block until
                    the kernel dispatch command has been processed and the
                    kernel is running. */
    #endif
    BLOCKING_POLICY_COMPLETED=8  /*!< The dispatch interface will block until
                    the kernel has completed executing. */
} BlockingPolicy;

/**
 * @brief enum for the event type to be utilized in one of the constructors
 */
typedef enum USER_EVENT_TYPE_
{
    HW_EXCEPTION_EVENT         = 3, ///< Exception event
    SYSTEM_CALL_EVENT          = 4, ///< Syscall event with parameter info
    DEBUG_EVENT                = 5, ///< Debug event
    //...
    MAX_EVENTS
} USER_EVENT_TYPE;

/**
 * index of each type of trap handler
 */
typedef enum {
    DEBUG_TRAP     = 1, /*!< level-2, debug trap handler index */
    SYSCALL_TRAP   = 2, /*!< level-2, syscall trap handler index */
    EXCEPTION_TRAP = 3, /*!< level-2, exception trap handler index */
    MAX_TRAPS           /*!< the number of trap handlers */
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
 * @brief trap handler class, maintains all trap handlers used
 * in the dispatch
 */
class TrapHandler {

public:

    /**
     * @brief default descructor of the trap handler class
     */
    virtual ~TrapHandler() {}

	/**
     * @brief Set up trap handler, When this function is called,
     * trap handler will internally read the runtime trap handler.
     *
     * @param trapType different types of trap handler, currently,
     * support runtime, debugger, exception, system call.
     *
     * @param trapHandler pointer to the trap handler, this address needs
     * to be 256-byte aligned.
     *
     * @param trapHandlerSizeByte size of the trap handler in bytes
     *
     * @return void
     */
    virtual void setTrapHandler(uint32_t trapType,
                                void *trapHandler,
                                size_t trapHandlerSizeByte) = 0;

    /**
     * @brief Set up trap handler buffer in the current device
     *
     * @param trapType different types of trap handler, currently,
     * support runtime,
     *
     * @param trapHandlerBuffer pointer to the trap handler buffer,
     * this address needs to be 256-byte aligned.
     *
     * @param trapHandlerBufferSizeByte size of the trap handler
     * buffer in bytes debugger, exception, system call.
     *
     * @return void
     */
    virtual void setTrapHandlerBuffer(uint32_t trapType,
                                      void *trapHandlerBuffer,
                                      size_t trapHandlerBufferSizeByte) = 0;

    /**
     * @brief copy trap handler info from device object to be used by the dispatch
     *
     * @param trapHandlerInfo pointer to the trap handler info to be copied from
     *
     * @return void
     */
    virtual void copyTrapHandlerFromDevice(Device *dev) = 0;

    /**
     * @brief get all the trap handlers
     *
     * @param trapHandlerInfo pointer to the trap handler info to be copied from
     *
     * @return void
     */
    virtual void getTrapHandlerInfo(Device *dev) = 0;
};

/**
 * attributes of a launch include, exception policy, launch policy and cache
 * policy
 */
typedef struct LaunchAttributes_ {

    BlockingPolicy blockingPolicy; /*!< default has none of the
                                            guarantees listed in the
                                            policy*/
    ExceptionPolicy exceptionPolicy; /*!< default is to generate
                                          exceptions*/

    CachePolicy cachePolicy;    /*!< default is to flush everything */

    TrapHandler *trapHandler;   /*!< trap handler object used for the
	                             current kernel dispatch */
    bool debugMode;             /*!< default is NOT executed in the debug
                                 mode */

    int grid[3];                /*!< default is 1*/
    int group[3];               /*!< default is 1*/
    int groupOffsets[3];        /*!< default is 0*/
    bool timestampEnabled;      /*!< default is false*/

    /*!<
     * @brief Size (in bytes) of group memory used in the kernel parameters.
     */
    size_t groupMemorySize;

    LaunchAttributes_()
    {
        grid[0] = 1;
        grid[1] = 1;
        grid[2] = 1;
        group[0] = 1;
        group[1] = 1;
        group[2] = 1;
        groupOffsets[0]=0;
        groupOffsets[1]=0;
        groupOffsets[2]=0;

        debugMode = false;
        trapHandler = NULL;
        timestampEnabled = false;
        cachePolicy = CACHE_POLICY_FLUSH_ALL;
        blockingPolicy = BLOCKING_POLICY_NONE;

        groupMemorySize = 0;
    }

} LaunchAttributes;


/**
 * @brief Allocate global memory that is shared by all devices
 *        in the platform.
 *
 * Allocates \c size bytes of linear memory and returns a pointer to
 * the allocated memory. The memory is not initialized.
 *
 * The allocated memory by default will be a system memory that is pageable,
 * cache coherent and readable / writeable by both host and devices.
 *
 * @param size Requested allocation size in bytes. This parameter needs to be larger
     *             than zero.
 * @param alignment The alignment size in bytes for the address of resulting
 *                  allocation. The default value of this parameter is zero,
 *                  where no particular alignment will be applied. If the value
 *                  is not zero, it needs to be a power of two and minimum of
 *                  sizeof(void*).
 * @exception HsaException if the input is invalid, or if the runtime is
 *            unable to allocate enough memory to perform the requested
 *            operation.
 * @return Pointer to allocated memory.
 *
 */
DLL_PUBLIC void*
allocateGlobalMemory(size_t size,
                     size_t alignment = 0);

/**
 * @brief Frees global memory pointed to by \c ptr, which must have been
 *        returned by a previous call to \c hsa::allocateGlobalMemory(). Otherwise,
 *        or if this function has already been called before, undefined
 *        behavior may occur.
 *
 * @param ptr Pointer to the memory to be freed.
 */
DLL_PUBLIC void
freeGlobalMemory(void* ptr);

/**
 * @brief Device class, public interface in the device layer.
 */
class DLL_PUBLIC Device
{

public:

    virtual hsa::DeviceType getType() const = 0;

    /**
     * @brief Compiles a kernel down to ISA and returns a hsa::kernel
     *
     * @param prog - program object
     *
     * @param kernelName - name of the kernel to compiler
     *
     * @param options- compiler options to compiler for
     */
    virtual hsa::Kernel* compile(
                    hsa::Program* prog,
                    const char* kernelName,
                    const char* options)=0;

     /**
     * @brief Returns the name of the vendor
     * @return returns the name of the vendor as a hsa::string
     */

    virtual const hsa::string& getVendorName() const=0;

    /**
     * @brief Returns the ID of the vendor's device
     * @return returns the name of the vendor's device ID as a unsigned int
     */
    virtual uint32_t getVendorID()=0;

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

   	/**
     * @brief get the max user-mode queue size possible
     *
     * @return returns max queue size in bytes
     */
    virtual uint32_t getMaxQueueSize()=0;

	/**
     * @brief Get the wavefront size
     *
     * @return returns the size of wavefront.
     */
    virtual int getWaveFrontSize()=0;

    #ifdef HSA_EXPERIMENTAL
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
    virtual void setTrapHandler(void *trapHandler, size_t trapHandlerSizeByte, uint32_t trapType) = 0;

    /**
     * @brief Set up trap handler buffer in the current device
     *
     * @param trapType different types of trap handler, currently,
     * support runtime, debugger, exception, system call.
     *
     * @param trapHandlerBuffer pointer to the trap handler buffer,
     * this address needs to be 256-byte aligned.
     *
     * @param trapHandlerBufferSizeByte size of the trap handler
     * buffer in bytes
     *
     * @return void
     */
    virtual void setTrapHandlerBuffer(uint32_t trapType,
                                      void *trapHandlerBuffer,
                                      size_t trapHandlerBufferSizeByte) = 0;

    /**
     * @brief Retrieve all the trap handler information in the device
     *
     * @param trapHandlerInfo pointer to the trap handler info buffer
     */
    virtual void getTrapHandlerInfo(TrapHandlerInfo * trapHandlerInfo) = 0;

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

    /**
     * @brief Invalidate all cache on the device.
     *
     * @param dev pointer to device
     *
     * @param cachePolicy indicate how the cache should be invalidated.
     */
    virtual void flushCaches(CachePolicy cachePolicy) = 0;

    /**
     * @ingroup Debugger
     * @brief APIs for creating a user trap event
     *
     * @param dev pointer to the HSA runtime device
     *
     * @param eventType user event type, can be
     *  HW_EXCEPTION_EVENT         = 3, ///< Exception event
     *  SYSTEM_CALL_EVENT          = 4, ///< Syscall event with parameter info
     *  DEBUG_EVENT                = 5, ///< Debug event
     *
     * @param manualReset if this parameter is true the function creates a
     * manual-reset event object; if this parameter is false the function creates
     * an auto-reset event object.
     *
     * @param isSignaled if this parameter is true, the initial state of the event
     * is signaled, otherwise is nonsignaled.
     */
    TrapEvent * createTrapEvent(USER_EVENT_TYPE eventType,
                                   bool manualReset, bool state) = 0;

    #endif      /***   HSA_EXPERIMENTAL   ***/


    /**
     * @brief Allocates global memory with specific properties. The allocated
     *        memory is shared by the host and the calling device.
     *
     * Developers can determine the properties of the allocation, such as the
     * heap type, host/device accessibility, and performance hint, by setting
     * the \c heapType and \c flag parameter.
     *
     * The \c heapType parameter must contain one of the following heap types.
     * @li HEAP_TYPE_SYSTEM The global memory is allocated in the system
     *                      near the calling device.
     * @li HEAP_TYPE_DEVICE_PUBLIC The global memory is allocated in host visible
     *                             part of the device local memory.
     *
     * The \c flag parameter must contain one of the following host accessibility options.
     * @li MEMORY_OPTION_HOST_READ_WRITE The host is able to read and write to
     *     the allocated memory.
     * @li MEMORY_OPTION_HOST_READ_ONLY The host can only read
     *                                  the allocated memory.
     * @li MEMORY_OPTION_HOST_WRITE_ONLY The host can only write to
     *                                   the allocated memory.
     * @li MEMORY_OPTION_HOST_NO_ACCESS The host can not read or write to
     *     the allocated memory.
     *
     * The \c flag parameter must contain one of the following
     * device accessibility options.
     * @li MEMORY_OPTION_DEVICE_READ_WRITE The calling device is able to
     *     read and write to the allocated memory.
     * @li MEMORY_OPTION_DEVICE_READ_ONLY The calling device can only read
     *                                    the allocated memory.
     * @li MEMORY_OPTION_DEVICE_WRITE_ONLY The calling device can only write
     *                                     to the allcoated memory.
     * @li MEMORY_OPTION_DEVICE_NO_ACCESS The calling device can not
     *                                    read or write to the allocated memory.
     *
     * Additionaly, a \c flag parameter can be combined with the following
     * performance hints.
     * Note that these options are not mandatory.
     * @li MEMORY_OPTION_UNCACHED Sets the allocated memory to be non-cachable.
     *                            This option is mutually exclusive with
     *                            MEMORY_OPTION_WRITE_COMBINED.
     * @li MEMORY_OPTION_WRITE_COMBINED Sets the allocated memory to be
     *                                  write-combined. This option is mutually
     *                                  exclusive with MEMORY_OPTION_UNCACHED.
     * @li MEMORY_OPTION_NONPAGEABLE Disables paging on the allocated memory.
     *
     * @param size Size of the allocation in bytes. This parameter needs to be larger
     *             than zero.
     * @param alignment The alignment size in bytes for the address of resulting
     *                  allocation. The default value of this parameter is zero,
     *                  where no particular alignment will be applied. If the value
     *                  is not zero, it needs to be a power of two and minimum of
     *                  sizeof(void*).
     * @param heapType The backing storage of the allocated global memory.
     * @param flag The properties of the allocated memory.
     * @exception HsaException if the input is invalid, or if the runtime is
     *            unable to allocate enough memory to perform the requested
     *            operation.
     * @return Pointer to the allocated memory.
     */
    virtual void*
    allocateGlobalMemory(size_t size,
                         size_t alignment = 0,
                         HeapType heapType = HEAP_TYPE_SYSTEM,
                         uint32_t flag = (MEMORY_OPTION_HOST_READ_WRITE
                                          | MEMORY_OPTION_DEVICE_READ_WRITE)) = 0;

    /**
     * @brief Frees a global memory region, which must have been
     *        returned by a previous call to \c hsa::Device::allocateGlobalMemory().
     *        Otherwise, or if this function has already been called before,
     *        undefined behavior may occur.
     *
     * @param ptr Pointer to the memory to be freed.
     */
    virtual void
    freeGlobalMemory(void* ptr) = 0;

    /**
     * @brief Gives hint to the runtime that a particular memory region
     *        will be used by the calling device. This routine is optional
     *        and is exclusively provided for the purpose of performance.
     *
     * Registration to a memory region, which overlapped with a previously
     * registered memory is allowed. This includes registering the same memory
     * region multiple times.
     *
     * Memory region allocated with hsa::allocateGlobalMemory or
     * hsa::Device::allocateGlobalMemory is automatically registered.
     * Registering this kind of memory region will not generate an error and
     * simply be ignored.
     *
     * @param ptr Pointer to a valid system memory.
     * @param size Requested registration size in bytes. This parameter needs to
     *             be larger than zero.
     * @exception HsaException if the input is invalid, or if the runtime is
     *            unable to perform the requested operation.
     */
    virtual void
    registerMemory(void* ptr, size_t size) = 0;

    /**
     * @brief Deregister system memory previously registered by the device.
     *
     * Deregisters the memory space pointed to by ptr, which must been
     * registered by a previous call to hsa::Device::registerMemory().
     * Otherwise, undefined behavior ocurrs. A memory region that was registered
     * multiple times needs to be deregistered for the same amount of time.
     *
     * @param ptr Pointer to memory to deregister.
     */
    virtual void
    deregisterMemory(void* ptr) = 0;

    /**
     * @brief Indicates that a global memory region will be used by the
     *        host or the calling device.
     *
     * This method has several purposes depending on
     * the location of the memory.
     * \li In the case of global memory located in the system, this function is optional
     *     and may speed up the access.
     * \li As for global memory located in the device local memory, a call to this
     *     function is mandatory to make the memory region accessible by the host or the
     *     calling device. Otherwise, undefined behavior may occurs.
     *
     * @param ptr Pointer to a valid memory.
     * @param size Requested mapping size in bytes. This parameter needs to be larger
     *             than zero.
     * @exception HsaException if the input is invalid or the runtime is unable to map
     *            enough memory to perform the requested operation.
     *
     */
    virtual void
    mapMemory(void* ptr, size_t size) = 0;

    /**
     * @brief Indicates that a memory regions will not longer be used
     *        by the host or calling device.
     *
     * The memory region to be unmapped must have been
     * mapped by a previous call to hsa::Device::mapMemory(). Otherwise, undefined
     * behavior ocurrs.
     *
     * @param ptr Pointer to a valid memory to unmap.
     *
     */
    virtual void
    unmapMemory(void* ptr) = 0;

    /**
    * @brief Returns a structure of highly correlated host and device times
    *        as well as the frequency of the device for conversion purposes.
    *
    * The frequency returned will be the max frequency reported by the ASIC.
    * Additionally, until the KFD implements the wall clock feature, this will
    * return invalid data.
    * @return DeviceClockCounterInfo containing the clock stamps from host
    *         and device along with device frequency.
    */
    virtual DeviceClockCounterInfo getClockCounterInfo() = 0;

    /**
    * @brief Returns the max frequency as reported by kfd for the
    *        approriate device type.
    *
    * @return max frequency as integer
    */
    virtual int getMaxFrequency()=0;

    /**
     * @brief Returns the corresponding event object
     *
     * @return hsa::Event * pointer to the event object.
     *
     * @note: Since the createDeviceEvent is removed, we
     * need a way for the clients to get one.
     */
    virtual hsa::Event * createEvent(void) = 0;

    /**
     * @brief Returns the corresponding Queue object
     *
     * @param queSize size of Queue in terms of WORD size.
     *
     * @return hsa::Queue * pointer to the Queue object.
     *
     * @note: Since the createDeviceQueue is removed, we
     * need a way for the clients to get one.
     */
    virtual hsa::Queue * createQueue(uint32_t queSize) = 0;

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
     * @brief Waits on the event object until it is signalled. The wait
     * is for indefinite period.
     *
     * @return hsa::Status return value indicating success or
     * failure
     */
    virtual hsa::Status wait(void) = 0;

    /**
     * @brief Waits on the event object until it is signalled or timeout
     * occurs. The wait is specified in terms of milliseconds. The literal
     * 0xFFFFFFFF is specified for indefinite period.
     *
     * @param timeOut in milliseconds, 0 means non-blocking wait while
     * 0xFFFFFFFF means blocking wait.
     *
     * @return hsa::Status return value indicating success or timeout.
     */
    virtual hsa::Status wait(uint32_t timeOut) = 0;

};

/**
 * @brief Creates a Dispatch Event that allows users to monitor the
 * completion of a kernel dispatch. It supports monitoring by allowing
 * users to wait on the dispatch event object.
 *
 * In addition, DispatchEvent provides an Api for Hsa Runtime clients
 * to obtain the Dispatch related information.
 *
 * Hsa Runtime clients should note that Dispatch Events are Read only
 * objects. They do not provide Api to set event state - reset(),
 * setEventState().
 */
class DLL_PUBLIC DispatchEvent : public hsa::Event {

public:

    /**
     * @brief Destructor. Releases all resources used in support of
     * its functionality, especially memory used to execute a kernel.
     *
     * @note: For now the memory allocated is not freed since multiple
     * readers exist for the event state, HsaRt, HsaRt Clients.
     */
    ~DispatchEvent() {};

    /**
     * @brief Returns the status of Dispatch Event. Four states are
     * defined for a Dispatch - Initiated, Submitted, Running and
     * Complete.
     *
     * @return hsa::EventState One of the states specified for
     * the HsaRt Tasks.
     *
     * @throws HsaException if an error occurs in runtime
     */
    virtual hsa::EventState getState(void) = 0;

    /**
     * @brief Retrieves the time when Dispatch event has transitioned
     * to input state.
     *
     * @return uint64_t time structure containing wallclock time
     * in Gpu domain. Will be initialized to zero if input state
     * is invalid or event has not  yet reached input state.
     */
    virtual uint64_t getTimestamp(hsa::EventState state) = 0;

    /**
     * @brief Returns the unique Id of HsaRt kernel dispatch.
     *
     * @return uint32_t Id of kernel dispatch.
     */
    virtual uint32_t getDispatchId(void) = 0;

    /**
     * @brief Retrieves the ISA pointer and size that are dispatched
     * to the kernel
     *
     * @param ptr pointer to the ISA in the dispatch descriptor
     *
     * @param size size of the ISA
     *
     * @return void
     */
    virtual void getISA(void * &ptr, size_t &size) = 0;

    /**
     * @brief Retrieves the input and output parameters of the dispatch
     * associated with this event.
     *
     * @return the list of input/output parameters
     */
    virtual hsa::vector<KernelArg> & getKernelArguments(void) = 0;

    /**
     * @brief Retrieves the policy configuration used to launch
     * the dispatch associated with this event.
     *
     * @return hsa::Launchattributes instance of the policy choices
     * used in launching this dispatch.
     */
    virtual hsa::LaunchAttributes & getLaunchAttributes(void) = 0;

    /**
     * @brief Retrieves the list of dependent events used in launching
     * the dispatch associated with this event.
     *
     * @return the list of dependent events used in launching this dispatch.
     */
    virtual hsa::vector<hsa::Event *> & getDependencies(void) = 0;

};              /***    hsa::DispatchEvent    ***/


/**
 * @brief the user event is used for syscall, exception processing,
 * and debugger.
 */
class DLL_PUBLIC TrapEvent : public Event
{

public:

    /**
     * @brief Default destructor of TrapEvent interface.
     */
    virtual ~TrapEvent() {};

    /**
     * @brief When this event is signaled, debugger users will need
     * get various information such as scratch memory, which will be
     * available in the debugger dispatch event. To avoid the duplicate
     * of the APIs, TrapEvent returns the debugger dispach object
     * for users to get those information.
     */
    virtual DispatchEvent *getPayload() = 0;
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

    /**
     * @brief Submits a compiled Hsail Kernel for execution. HsaRt is guided
     * in this process by various attributes of kernel launch, including any
     * dependent events. Any input parameters needed for the kernel are passed
     * in as a variadic list.
     *
     * @param kernel pointer to a Compiled Hsail kernel object
     *
     * @param launchAttr a set of policies to apply in executing kernel.
     *
     * @param depEvents list of dependent events on which this kernel must
     * wait prior to starting the execution of kernel.
     *
     * @param numArgs number of input parameters that follow.
     *
     * @return hsa::DispatchEvent * pointer to an event object that could be
     * queried with regards to dispatch status, including getting handles to
     * compiled code and various other parameters.
     */
    virtual hsa::DispatchEvent *dispatch(hsa::Kernel *kernel,
                                         hsa::LaunchAttributes launchAttr,
                                         hsa::vector<hsa::Event *> &depEvents,
                                         uint32_t numArgs, ...) = 0;

    /**
     * @brief Submits a compiled Hsail Kernel for execution. HsaRt is guided
     * in this process by various attributes of kernel launch, including any
     * dependent events. Any input parameters needed for the kernel are passed
     * in as a variadic list.
     *
     * @param kernel pointer to a Compiled Hsail kernel object
     *
     * @param launchAttr a set of policies to apply in executing kernel.
     *
     * @param depEvents list of dependent events on which this kernel must
     * wait prior to starting the execution of kernel.
     *
     * @param krnlArgs list of input/output parameters
     *
     * @return hsa::DispatchEvent * pointer to an event object that could be
     * queried with regards to dispatch status, including getting handles to
     * compiled code and various other parameters.
     */
    virtual hsa::DispatchEvent *dispatch(hsa::Kernel *kernel,
                                         hsa::LaunchAttributes launchAttr,
                                         hsa::vector<hsa::Event *> &depEvents,
                                         hsa::vector<KernelArg> &krnlArgs) = 0;

    virtual void flush()=0;

};


class DLL_PUBLIC Kernel
{
public:

    /*! @brief destructor
    */

    virtual ~Kernel(){};

    /* @brief Returns a pointer to the ISA.
    * return - returns a void pointer to the location of ISA
    */
   virtual void* getISA() = 0;

   /* @brief Returns the size of ISA
    * return - returns the size of of ISA
    */
   virtual size_t getSizeOfISA() = 0;

   /* @brief Returns the name of the kernel
   * @return the name of the string
   */

   virtual hsa::string& getName()=0;

   /** @brief Returns the starting address of the group memory
     *        in flat memory model.
     *
     *        The developer can use this address to calculate
     *        the address of each group memory parameter in the kernel.
     *        To avoid unexpected behavior, the developer needs
     *        to check the upper bound of the group memory region of
     *        associated device.
     *
     * @return Pointer to group memory.
     */
    virtual const void* getGroupMemoryBase() const = 0;
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
    virtual const hsa::vector<hsa::Device*> & getDevices()=0;

    /* @ brief Creates a Program object from an ELF
    * @param charElf - Pointer to an ELF
    * @param elfSize - size of the ELF
    * @param pDevices - pointer to a list of devices
    */
    virtual hsa::Program* createProgram(char *charElf,
                                        size_t elfSize,
                                        hsa::vector<hsa::Device *> *devList)=0;

    /* @ brief Creates a Program object from a File
    * @param fileName - Name of the file
    * @param pDevices - pointer to a list of devices
    */
    virtual hsa::Program* createProgramFromFile(const char* fileName,
                                                hsa::vector<hsa::Device *> *devList)=0;

    /* @ brief Destroys a a program
    * @param prog pointer to the program
    * @param pDevices - pointer to a list of devices
    */
    virtual void destroyProgram(hsa::Program*)=0;

    /**
     * @brief Get the runtime API version
     *
     *
     * @return runtime API version.
     */
    virtual const hsa::string & getVersion()=0;

    /**
     * @copydoc hsa::allocateGlobalMemory(size_t,size_t)
     */
    virtual void*
    allocateGlobalMemory(size_t size,
                         size_t alignment = 0) = 0;

    /**
     * @copydoc hsa::freeGlobalMemory(void*)
     */
    virtual void
    freeGlobalMemory(void* ptr) = 0;

    virtual ~RuntimeApi(){};
};

class DLL_PUBLIC Program
{
public:
    /* This is experimental */
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

    /*! @brief standard destructor */

    virtual ~Program(){};

};

/* @ brief Creates a Program object from an ELF
 * @param charElf - Pointer to an ELF
 * @param elfSize - size of the ELF
 * @param devList - pointer to a list of devices
*/

DLL_PUBLIC hsa::Program* createProgram(char *charElf,
                                        size_t elfSize,
                                        hsa::vector<hsa::Device *> *devList);

/* @ brief Creates a Program object from a File
 * @param fileName - Name of the file
 * @param devList - pointer to a list of devices
*/

DLL_PUBLIC hsa::Program* createProgramFromFile(
                                const char* fileName,
                                hsa::vector<hsa::Device *> *devList);

/* @ brief Destroys a a program
 * @param prog pointer to the program
 * @param pDevices - pointer to a list of devices
*/

DLL_PUBLIC void destroyProgram(hsa::Program* prog);

/**
 * @brief get the count of devices on the platform
 *
 * @return the count of the number of devices in the platform.
 */
DLL_PUBLIC uint32_t getDeviceCount();

/**
 * @brief get a list of devices available on the platform.
 *
 * @return Returns the list of all Devices
 */
DLL_PUBLIC const hsa::vector<hsa::Device*>& getDevices();

/**
 * @brief get the version of the RuntimeApi.
 *
 * Rules for incrementing the major, minor, patch versions:
 * a) major:
 *       i) incremented only when public APIs signatures
 *          are changed, IS NOT backward compatible with
 *          previous major versions.
 *      ii) Reset the minor version to 0.
 *     iii) Reset the patch version to 0.
 * b) minor:
 *       i) incremented when new feature is added, needs
 *          to be backward compatible with previous minor
 *          versions.
 *      ii) Reset the patch version to 0.
 * c) patch:
 *       i) incremented only for bug fixes, no user visible
 *          API changes, needs to be backward and forward
 *          compatible with previous patch versions.
 * @return string containing the version number in the
 *         form major.minor.patch
 */
DLL_PUBLIC const hsa::string& getVersion();

/**
 * @}
 */
}

#endif
