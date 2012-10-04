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

#ifdef HSA_EXPERIMENTAL
#include "hsaperf.h"
#endif

namespace hsa
{
/** @defgroup HSA_EXPERIMENTAL Experimental HSA Runtime APIs
 */
/**
 * @addtogroup HSA_PUBLIC HSA Runtime APIs
 *
 * @{
 *
 */

/**
 * Define vector and string as entities from hsa namespace
 */
using namespace hsacommon;

class Runtime;
class Queue;
class Kernel;
class Event;
class Device;
class Program;
class DispatchEvent;


class TrapEvent;
class TrapHandler;


/**
 *******************************************************************************
 * @brief Event command state
 * @details The event state enumerations are used to describe the current state
 *          of the command associated with the event.  
 *******************************************************************************
 */
typedef enum {
    STATE_INITIATED,      /*!< Initial command state */
    STATE_INITIATED_HOST, /*!< Initial command state int host time*/

    STATE_BLOCKED,      /*!< The related command is blocked waiting for all
                          dependencies to be resolved. */

    STATE_SUBMITTED,      /*!< The related command is pending execution by the 
                          device.  All dependencies have been resolved. */
    STATE_SUBMITTED_HOST, /*!< The related command is pending execution by the 
                          device.  All dependencies have been resolved. host time*/

    STATE_STARTED,      /*!< Execution of the related command has been started 
                          by the device. */

    STATE_COMPLETED     /*!< Execution of the related command has been 
                          completed by the device. */
} EventState;

/**
 *******************************************************************************
 * @brief Wave actions
 * @details The wave action enumerations are used to specify the desired 
 *          behavior when a executing wave encounters an exception.
 *******************************************************************************
 */
typedef enum {
    WAVE_ACTION_HALT = 1,   /*!< The wave should be suspended in the event 
                              that an exception is encountered.  This is the 
                              default behavior.  */

    WAVE_ACTION_RESUME = 2, /*!< The wave should resume in the event that an 
                              exception is encountered.  The wave is resumed 
                              at the instruction following the exception PC.*/

    WAVE_ACTION_KILL = 3,   /*!< The wave should be killed in the event that 
                              an exception is encountered.  The wave is killed
                              at the exception instruction. If not all the 
                              wavefronts in the wave take a fault, the place 
                              in the code where the non-faulty waves get 
                              killed is non-deterministic. This is usually 
                              before the next barrier. */

    WAVE_ACTION_DEBUG = 4,  /*!< The wave should be caught by the debugger in 
                              the event that an exception is encountered. */
                              
    WAVE_ACTION_TRAP = 5,   /*!< The wave should be sent to the trap handler 
                              in the event that an exception is encountered.*/
    WAVE_ACTION_MAX
} WaveAction;

/**
 *******************************************************************************
 * @brief Host kernel exception action
 * @details The host action enumeration is used to specify the desired host 
 *          response in the event thatn a device kernel exception is 
 *          encountered.  
 *******************************************************************************
 */
typedef enum {
    HOST_ACTION_IGNORE = 1,     /*!< Ignore the kernel exception. The host 
                                  application will ignore any kernel 
                                  exceptions. */

    HOST_ACTION_EXIT = 2,       /*!< Exit the host application on a kernel 
                                  exception.  The host application will call
                                  exit() in the event of a kernel exception. */

    HOST_ACTION_NOTIFY = 4     /*!< Interrupt the host application on a kernel 
                                  exception. The host application has
                                  registered an event to be signaled in the
                                  event of a kernel exception. */
} HostAction;

/**
 *******************************************************************************
 * @brief Dispatch cache policy
 * @details The dispatch cache policy enumeration is used to specify the 
 *          level of cache flush to use upon completion of the dispatch.
 *******************************************************************************
 */
typedef enum {
    CACHE_POLICY_FLUSH_ALL = 1,             /*!< Flush all caches that can be
                                              flushed. */

    CACHE_POLICY_FLUSH_COMPUTE_UNIT = 2,    /*!< Flush the private device 
                                              compute unit caches. */

    CACHE_POLICY_FLUSH_SHARED = 4           /*!< Flush the shared device
                                              compute unit caches. */
} CachePolicy;

/**
 *******************************************************************************
 * @brief Wave broadcast level
 * @details The wave mode enumerations are used to specify the desired 
 *          broadcast level.
 *******************************************************************************
 */
typedef enum {
    WAVE_MODE_SINGLE = 0,                /*!< Send broadcast message to a 
                                           single wave */
    /* broadcast to all wavefronts of all processes is not supported for HSA 
     * user mode */
    WAVE_MODE_BROADCAST_PROCESS = 2,     /*!< Send broadcast message to all 
                                           waves in the current process */

    WAVE_MODE_BROADCAST_PROCESS_CU = 3,  /*!< Send broadcast message to all 
                                           waves within current process compute
                                           unit */
    WAVE_MODE_MAX
} WaveMode;

/**
 *******************************************************************************
 * @brief Dispatch exception policy descriptor
 * @details The dispatch exception policy descriptor is used to define the 
 *          expected exception policy in the event an exception is encountered
 *          on the associated dispatch.
 *******************************************************************************
 */
typedef struct ExceptionPolicy_ {
    uint32_t exceptionType;    /*!< Mask of exception types to handle. */

    WaveAction waveAction;     /*!< Expected wave action to take in the event
                                 of an exception.  The default wave action is
                                 to halt the wave (hsa::WAVE_ACTION_HALT) in
                                 the event of an exception during dispatch. */

    HostAction hostAction;     /*!< Expected host action to take in the event
                                 of an exception.  The default host action is
                                 to ignore any exceptions that occur during 
                                 dispatch */

    WaveMode   waveMode;       /*!< default wave mode is to single */

    ExceptionPolicy_()
    {
        waveAction = WAVE_ACTION_HALT;
        hostAction = HOST_ACTION_IGNORE;
        exceptionType = 0x0; 
        waveMode   = WAVE_MODE_SINGLE;
    }
} ExceptionPolicy;

/**
 *******************************************************************************
 * @brief Dispatch policy
 * @details The blocking policy enumerations are used to specify the expected
 *          behavior of the dispatch API when invoked. 
 *******************************************************************************
 */
typedef enum {

    BLOCKING_POLICY_NONE        = 0, /*!< The dispatch interface is 
                                       non-blocking and returns immediately. 
                                       This is the default behavior.  The user
                                       is expected to separately wait for 
                                       completion.  */

    BLOCKING_POLICY_DEPENDENCY  = 1, /*!< The dispatch interface will block 
                                       until all dependencies have succeeded 
                                       before returning from the kernel 
                                       launch. */

    BLOCKING_POLICY_ENQUEUED    = 2, /*!< The dispatch interface will block 
                                       until the kernel has been enqueued to 
                                       the device queue for execution. */

    BLOCKING_POLICY_EXECUTING   = 4, /*!< The dispatch interface will block 
                                       until the kernel dispatch command has 
                                       been processed and the kernel is 
                                       running. */

    BLOCKING_POLICY_COMPLETED   = 8  /*!< The dispatch interface will block
                                       until the kernel has completed 
                                       execution. */
} BlockingPolicy;

/** 
 *******************************************************************************
 * @brief Kernel dispatch launch attributes descriptor
 * @details The kernel dispatch launch attributes descriptor contains the 
 *          conditions for initiating the execution of a device kernel.  
 *          The attributes for launching a kernel include policies for blocking,
 *          exception handling, and cache management.  As well, the grid and
 *          group size are specified as part of the descriptor.
 *******************************************************************************
 */
typedef struct LaunchAttributes_ {

    BlockingPolicy blockingPolicy; /*!< default has none of the
                                            guarantees listed in the
                                            policy*/
    ExceptionPolicy exceptionPolicy; /*!< default is to generate
                                          exceptions*/

    CachePolicy cachePolicy;    /*!< default is to flush everything */
	
    union {
      uint32_t NDRangeSize[3];  /*!< 3 dimensional overall size of the NDRange
                                  to be processed on the dispatch */
      int grid[3];              // Legacy name - deprecated             
    };
    
    union {
      int WorkGroupSize[3];     /*!< 3 dimensional size of the work-group to 
                                  execute on each participating compute unit.  
                                  The size represents the number of work-items 
                                  concurrently executing an instance of the 
                                  kernel. */
      int group[3];             // Legacy name - deprecated
    };

    union {
      int AbsoluteOffset[3];    /*!< 3 dimensional offset to be applied to
                                  each work-item absolute ID.  */
      int groupOffsets[3];      // Legacy name - deprecated
    };

    TrapHandler *trapHandler;   /*!< trap handler object used for the
	                             current kernel dispatch */
    bool debugMode;             /*!< default is NOT executed in the debug
                                 mode */
    bool timestampEnabled;      /*!< default is false*/

    size_t groupMemorySize;      /*!< Size (in bytes) of group memory used in the kernel parameters.*/

    LaunchAttributes_()
    {
        blockingPolicy = BLOCKING_POLICY_NONE;
        cachePolicy = CACHE_POLICY_FLUSH_ALL;

        NDRangeSize[0] = 1;
        NDRangeSize[1] = 1;
        NDRangeSize[2] = 1;
        WorkGroupSize[0] = 1;
        WorkGroupSize[1] = 1;
        WorkGroupSize[2] = 1;
        AbsoluteOffset[0]=0;
        AbsoluteOffset[1]=0;
        AbsoluteOffset[2]=0;
        debugMode = false;
        trapHandler = NULL;
        timestampEnabled = false;
        groupMemorySize = 0;
    }

} LaunchAttributes;

/** 
 * @brief User event types
 * @details The user event type enumerations are used for indicating the type 
 *          of user event to create in Device::createEvent().
 */
typedef enum UserEventType_ 
{
    DEBUG_EVENT         = 1, /*!< Debug event */
    SYSTEM_CALL_EVENT   = 2, /*!< Syscall event with parameter info */
    HW_EXCEPTION_EVENT  = 3, /*!< Exception event */
    MAX_EVENTS
} UserEventType;

/** 
 * @brief Trap handler types
 * @details The trap handler type enumerations are used for indicating the type
 *          of trap handler for trap operations.
 */
typedef enum {
    DEBUG_TRAP     = 1,     /*!< Debug trap handler index */
    SYSCALL_TRAP   = 2,     /*!< Syscall trap handler index */
    EXCEPTION_TRAP = 3,     /*!< Exception trap handler index */
    MAX_TRAPS               /*!< Maximum number of trap handler types */
} TrapType; 

/** 
 * @brief Trap handler descriptor
 * @details The trap handler descriptor contains the details of a given trap 
 *          handler.
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
 *******************************************************************************
 * @brief HSA runtime interface class
 * @details The HSA runtime interface class is a container for all global HSA 
 *          interfaces.  The runtime class is used to define the singleton root 
 *          HSA object used within an HSA environment.  The runtime object may
 *          be acquired explicitly using the hsa::getRuntime() interface.
 *
 *******************************************************************************
 */
class DLL_PUBLIC Runtime  
{
public:
    /**
     ***************************************************************************
     * @brief Inquire the number of HSA devices
     * @details This HSA runtime interface is used for querying the number of
     *          HSA devices on the given system.  The number of devices returned
     *          is only indicative of device existence and does not reflect the 
     *          state of any of the devices.
     *
     * @return Number of discovered devices
     *
     ***************************************************************************
     */
    virtual uint32_t getDeviceCount()=0;

    /**
     ***************************************************************************
     * @brief Acquire a list of HSA devices
     * @details This HSA runtime interface is used for acquiring a list of HSA
     *          devices on the system.  The returned list of devices does not
     *          guarantee the availability or state of any devices.  Each device
     *          must be inquired for their state.
     *          
     * @return Vector of hsa::Device objects is returned
     *
     ***************************************************************************
     */
    virtual const hsa::vector<hsa::Device*> & getDevices()=0;

    /**
     ***************************************************************************
     * @brief Create an HSA program from memory
     * @details This HSA runtime interface is used for creating an HSA program 
     *          object from an in memory ELF binary.  The resulting 
     *          hsa::Program object is dedicated to the specified set of 
     *          devices.  All devices must be specified at creation time.
     * @note The address pointer must be a BIF 3.0 ELF binary containing HSAIL 
     *       and/or BRIG sections.
     *
     * @param address Pointer to the in memory ELF binary
     * @param size Size of the in memory ELF binary in bytes
     * @param devices Vector of device pointers for the program
     *
     * @return Pointer to the created hsa::Program object is returned
     *
     * @exception TODO
     *
     ***************************************************************************
     */
    virtual hsa::Program* createProgram(char *address, 
                                        size_t size,
                                        hsa::vector<hsa::Device *> *devices)=0;

    /**
     ***************************************************************************
     * @brief Create an HSA program from a file
     * @details This HSA runtime interface is used for creating an HSA program 
     *          object from an ELF binary file.  The resulting hsa::Program 
     *          object is dedicated to the specified set of devices.  All 
     *          devices must be specified at creation time.
     *
     *          The specified filename must include the relative path to the 
     *          file location.  The filename is specified as a NULL terminated 
     *          charcter string.
     *
     * @note The file must be a BIF 3.0 ELF binary file containing HSAIL and/or 
     *       BRIG sections.
     *
     * @param fileName Filename of the ELF file to load
     * @param devices Vector of device pointers for the program
     *
     * @return Pointer to the create hsa::Program object is returned
     *
     * @exception TODO
     *
     ***************************************************************************
     */
    virtual hsa::Program* createProgramFromFile(
                                        const char* fileName, 
                                        hsa::vector<hsa::Device *> *devices)=0;

    /**
     ***************************************************************************
     * @brief Destroy an HSA program
     * @details This HSA runtime interface is used to destroy a previously 
     *          created hsa:Program object.  The specified program object 
     *          and all of its associated resources are destoryed and/or 
     *          released.  Use of the program object after destruction shall
     *          result in an error.
     *
     ***************************************************************************
     */
    virtual void destroyProgram(hsa::Program*)=0; 

    /**
     ***************************************************************************
     * @brief Inquire the HSA runtime version
     * @details This HSA runtime interface is used to inquire the version of
     *          the HSA runtime being used.  The runtime version represents the
     *          version of the runtime and its supported interfaces.  The
     *          version string may be used for validating an applications
     *          compliance with a given runtime revision.
     *
     *          The format of the runtime version is:
     *          major.minor.patch
     *
     *          The runtime versioning follows the below guidelines:
     *          major
     *          @li Incremented only when public APIs signatures are changed, 
     *              Is NOT backward compatible with previous major versions.
     *          @li Reset the minor version to 0.
     *          @li Reset the patch version to 0.
     *          minor 
     *          @li Incremented when new feature is added, needs to be 
     *              backward compatible with previous minor versions.
     *          @li Reset the patch version to 0.
     *          patch 
     *          @li Incremented only for bug fixes, no user visible API 
     *              changes, needs to be backward and forward compatible 
     *              with previous patch versions.
     *
     * @return Reference to a string containing the runtime API version.
     *
     ***************************************************************************
     */
    virtual const hsa::string & getVersion()=0;

    /**
     ***************************************************************************
     * @brief Allocate global memory
     * @details This HSA runtime interface is used to allocate global memory 
     *          that is accessible by all HSA runtime devices.  The allocated
     *          memory is shared pageable system memory that is cache coherent
     *          and readable/writeable by both host and devices.  The memory
     *          is uninitialized.
     *
     *          The \c size argument indicates the number of bytes of linear
     *          system memory to allocate, size must be greater than zero.
     *          
     *          The \c alignment argument indicates the desired address
     *          alignment of the allocation.  The default value of this
     *          parameter is zero, which indicates that the optimal device
     *          alignment will be applied. If the value is non-zero, it needs
     *          to be a power of two and minimum of sizeof(void*). 
     *
     * @param size Allocation size in bytes.
     * @param alignment Alignment size in bytes
     *
     * @return Pointer to allocated memory.
     *
     * @exception HsaException if the input is invalid, or if the runtime is
     *            unable to allocate enough memory to perform the requested 
     *            operation.
     *
     * @see Runtime::FreeGlobalMemory()
     *
     ***************************************************************************
     */
    virtual void* 
    allocateGlobalMemory(size_t size, 
                         size_t alignment = 0) = 0;

    /**
     ***************************************************************************
     * @brief Free global memory
     * @details This HSA runtimeinterface is used to free a previously 
     *          allocated global memory region.  The specified 
     *          memory pointer must have been returned by a prior call to 
     *          hsa::Device::allocateGlobalMemory(). 
     *
     *          Calling this function with an invalid pointer will result in
     *          undefined behavior.  
     *
     * @param address Pointer to the memory to be freed.
     *
     * @see Runtime::allocateGlobalMemory()
     *
     ***************************************************************************
     */
    virtual void
    freeGlobalMemory(void* address) = 0;

    /**
     ***************************************************************************
     * @brief Default destructor.
     ***************************************************************************
     */
    virtual ~Runtime(){};
};

/**
 *******************************************************************************
 * @brief HSA runtime device interface class
 * @details The HSA runtime device class provides the a set of interfaces for 
 *          operating on an HSA device object.  Device objects represent an 
 *          actual hardware device.  The class provides a common set of 
 *          operations for querying and allocating resources on the device. 
 *          The HSA runtime device is the primary object used in constructing an
 *          HSA runtime application.
 *******************************************************************************
 */
class DLL_PUBLIC Device
{

public:

    /**
     ***************************************************************************
     * @brief Inquire the type of the device object
     * @details This HSA runtime device interface is used to inquire the type 
     *          of associated device object.
     *
     * @return The associated DeviceType enumeration is returned.
     ***************************************************************************
     */
    virtual hsa::DeviceType getType() const = 0;

    /**
     ***************************************************************************
     * @brief Compile an HSAIL kernel
     * @details This HSA program interface is used to compile a kernel with
     *          the specified kernel name for use on the associated device.  
     *          The kernel is compiled from the supplied in memory ELF data. 
     *          The result of the compilation is an hsa::Kernel object that is 
     *          specific for the given device.
     *
     *          The follow are valid compiler options to this interface:
     *          @li -optlevel 
     *
     * @note The address pointer must be a BIF 3.0 ELF binary containing HSAIL 
     *       and/or BRIG sections.
     *
     * @param program Pointer to the HSA program object containing the kernel
     *                to be compiled.
     * @param kernelName Pointer to a NULL terminated kernel name string. 
     * @param options Pointer to a NULL terminated string of compile options.
     *
     * @return Pointer to the created hsa::Kernel object is returned.
     *
     * @exception TODO
     *
     ***************************************************************************
     */
    virtual hsa::Kernel* compile(
                    hsa::Program* program,
                    const char* kernelName,
                    const char* options)=0;

     /**
     ***************************************************************************
     * @brief Inquire the name of the vendor
     * @details This HSA runtime device interface is used to inquire the name of
     *          the associated device.
     *
     * @return Reference to a string containing the name of the vendor.
     *
     ***************************************************************************
     */
    virtual const hsa::string& getVendorName() const=0;

    /**
     ***************************************************************************
     * @brief Inquire the vendor device ID
     * @details This HSA runtime device interface is used to inquire the 
     *          vendor specific ID for the associated device.
     *
     * @return Numeric device vendor ID.
     *
     ***************************************************************************
     */
    virtual uint32_t getVendorID()=0;

    /**
     ***************************************************************************
     * @brief Inquire the number of compute units on the device
     * @details This HSA runtime device interface is used to inquire the number 
     *          of compute units on the associate device.  The compute unit 
     *          count reflects the maximum number of concurrently executing 
     *          work-groups.
     *
     * @return The number of device compute units is returned.
     ***************************************************************************
     */
    virtual unsigned int getComputeUnitsCount()=0;

    ///@todo Do we need getCapabilities?
    virtual uint32_t getCapabilities()=0;

    /**
     ***************************************************************************
     * @brief Inquire the name of the device.
     * @details This HSA runtime device interface is used to inquire the 
     *          vendor specific marketing name of the assoicated device.
     *
     * @return A reference to an HSA string containing the device name is 
     *         returned.
     ***************************************************************************
     */
    virtual hsa::string& getName()=0;

    /**
     ***************************************************************************
     * @brief Acquire a list of the device memory descriptors
     * @details This HSA runtime device interface acquires a vector containing 
     *          pointers to the memory descriptors for the associated device.  
     *          Each entry in the vector points to an individual memory bank
     *          descriptor.
     *
     *          The descriptor is used for querying the attributes of memory 
     *          accessible by the associated device.  
     *
     * @return A reference to a vector of device memory descriptor pointers is 
     *         returned.
     ***************************************************************************
     */
    virtual const hsa::vector<hsa::MemoryDescriptor*>& getMemoryDescriptors()=0;



    /**
     ***************************************************************************
     * @brief Inquire device double precision support
     * @details This HSA runtime device interface is used to inquire whether a 
     *          device supports double precision flaoting point or not.  The 
     *          result of this indicates whether kernels executing on the 
     *          device can use double precision data types.  
     *          @li true - Double precision types are usable within kernels 
     *                     executing on the device.
     *          @li false - Double precision types are NOT usable within 
     *                      kernels executing on the device.
     *
     * @return Boolean indicating whether double precision types are supported.
     *
     ***************************************************************************
     */
    virtual bool isDoublePrecision()=0;

    /**
     ***************************************************************************
     * @brief Inquire whether the device is dedicated for compute
     * @details This HSA runtime device interface is used to inquire whether a 
     *          device is dedicated to performing compute operations or it
     *          is also in use for other tasks such as graphics.  The result
     *          of this does not imply non-compute tasks are currently being
     *          performed, but rather that they can be performed.
     *          @li true - The device is dedicated to compute operations.
     *          @li false - The device is NOT dedicated to compute operations.
     *
     * @return Boolean indicating whether the device is compute only.
     *
     ***************************************************************************
     */
    virtual bool isDedicatedCompute()=0;

    /**
     ***************************************************************************
     * @brief Inquire the maximum device group memory size
     * @details This HSA runtime device interface is used to inquire the 
     *          maximum amount of group memory available on the associated
     *          device.
     *
     * @return Maximum group memory size in bytes.
     *
     ***************************************************************************
     */
    virtual uint32_t getMaxGroupMemorySize()=0;

    /**
     ***************************************************************************
     * @brief Inquire the maximum device group memory size
     * @details This HSA runtime device interface is used to inquire the 
     *
     * @return Maximum size of a device queue in bytes.
     *
     ***************************************************************************
     */
    virtual uint32_t getMaxQueueSize()=0;    
	
    /**
     ***************************************************************************
     * @brief Inquire the maximum device wavefront size
     * @details This HSA runtime device interface is used to inquire the 
     *          wavefront size of the associated device.  The wavefront size
     *          represents the maximum number of work-items that are 
     *          gang-scheduled for execution on a device. 
     *
     * @return Number of work-items in a wavefront.
     *
     ***************************************************************************
     */
    virtual int getWaveFrontSize()=0;

    /**
     ***************************************************************************
     * @brief Allocate global memory
     * @details This HSA runtime device insterface is used to allocate global 
     *          memory relative to the associated device.  The global memory is
     *          accessible by both the host and the device.  The device 
     *          implementation may allocate memory with access optimal to the
     *          associated device.
     *
     *          The allocate function allows for numerous customizations to
     *
     *          Developers can determine the properties of the allocation, 
     *          such as the heap type, host/device accessibility, and 
     *          performance hint, by setting the \c heapType and \c flag 
     *          parameter.
     * 
     *          The \c heapType parameter must be specified and contain one of 
     *          the following heap types:  
     *          @li HEAP_TYPE_SYSTEM The global memory is allocated in the 
     *                               system near the calling device. 
     *          @li HEAP_TYPE_DEVICE_PUBLIC The global memory is allocated in 
     *                               host visible part of the device local 
     *                               memory.
     *
     *          The \c flag parameter must contain one of the following host 
     *          accessibility options:
     *          @li MEMORY_OPTION_HOST_READ_WRITE The host is able to read and 
     *                               write to the allocated memory.
     *          @li MEMORY_OPTION_HOST_READ_ONLY The host can only read the 
     *                               allocated memory.
     *          @li MEMORY_OPTION_HOST_WRITE_ONLY The host can only write to 
     *                               the allocated memory.
     *          @li MEMORY_OPTION_HOST_NO_ACCESS The host can not read or 
     *                               write to the allocated memory.
     *
     *          The \c flag parameter must contain one of the following device 
     *          accessibility options:
     *          @li MEMORY_OPTION_DEVICE_READ_WRITE The calling device is able 
     *                               to read and write to the allocated memory.
     *          @li MEMORY_OPTION_DEVICE_READ_ONLY The calling device can only 
     *                               read the allocated memory.
     *          @li MEMORY_OPTION_DEVICE_WRITE_ONLY The calling device can 
     *                               only write to the allcoated memory.
     *          @li MEMORY_OPTION_DEVICE_NO_ACCESS The calling device can not 
     *                               read or write to the allocated memory.
     *
     *          Additionaly, the \c flag parameter can also include the 
     *          following optional performance hints:
     *          @li MEMORY_OPTION_UNCACHED Sets the allocated memory to be 
     *                               non-cachable.  This option is mutually 
     *                               exclusive with 
     *                               MEMORY_OPTION_WRITE_COMBINED.
     *          @li MEMORY_OPTION_WRITE_COMBINED Sets the allocated memory to 
     *                               be write-combined. This option is mutually 
     *                               exclusive with MEMORY_OPTION_UNCACHED.
     *          @li MEMORY_OPTION_NONPAGEABLE Disables paging on the allocated 
     *                               memory.
     *
     * @param size Size of the allocation in bytes, must be greater than zero.
     * @param alignment The alignment size in bytes for the address of 
     *                  resulting allocation. The default value of this 
     *                  parameter is zero, which indicates that the optimal
     *                  device alignment will be applied. If the value is 
     *                  non-zero, it needs to be a power of two and minimum 
     *                  of sizeof(void*). 
     * @param heapType The backing storage type to allocate from 
     * @param flag The memory options properties of the allocated memory.
     *
     * @exception HsaException if the input is invalid, or if the runtime is
     *            unable to allocate enough memory to perform the requested 
     *            operation.
    *
     * @return Pointer to the allocated memory.
     *
     * @see HeapType
     * @see MemoryOption
     * 
     ***************************************************************************
     */
    virtual void* 
    allocateGlobalMemory(size_t size, 
                         size_t alignment = 0,
                         HeapType heapType = HEAP_TYPE_SYSTEM,
                         uint32_t flag = (MEMORY_OPTION_HOST_READ_WRITE |
                                          MEMORY_OPTION_DEVICE_READ_WRITE)) = 0;

    /**
     ***************************************************************************
     * @brief Free previously allocated global memory.
     * @details This HSA device interface is used to free a previously 
     *          allocated global memory region on the device.  The specified 
     *          memory pointer must have been returned by a prior call to 
     *          Device::allocateGlobalMemory(). 
     *
     *          Calling this function with an invalid pointer or a valid 
     *          pointer that was not allocated on the associated device will
     *          result in undefined behavior.  
     *
     * @param address Pointer to the memory to be freed.
     *
     * @see Device::allocateGlobalMemory
     * 
     ***************************************************************************
     */
    virtual void 
    freeGlobalMemory(void* address) = 0;

    /**
     ***************************************************************************
     * @brief Register memory with a device
     * @details This HSA device interface is used for registering a specified 
     *          memory region with the associated device.  This operation is 
     *          used as an optimization hint that a particular memory region
     *          will be used in association with the device. Use of this 
     *          interface is considered optional and is exclusively provided 
     *          for the purpose of performance.
     *
     *          Registration of a memory region that overlaps a previously 
     *          registered memory region is permitted. This includes 
     *          registering the same memory region multiple times.
     *
     *          Registration of memory regions allocated with HSA device or 
     *          runtime allocators is strictly disallowed and will result in
     *          an error.
     *
     * @param address Pointer to a valid system memory.
     * @param size Requested registration size in bytes, must be greater than 
     *             zero.
     * @exception HsaException if the input is invalid, or if the runtime is 
     *            unable to perform the requested operation.
     *
     * @see Device::deregisterMemory
     * 
     ***************************************************************************
     */
    virtual void 
    registerMemory(void* address, size_t size) = 0;
    
    /**
     ***************************************************************************
     * @brief Deregister memory with a device
     * @details This HSA device interface is used for deregistering a 
     *          previsously registered memory region with the associated 
     *          device.  
     *
     *          Deregisters the memory space pointed to by /c address, which 
     *          must been registered by a previous call to 
     *          Device::registerMemory().  Otherwise, undefined behavior 
     *          occurs.  A memory region that was registered multiple times 
     *          needs to be deregistered for the same amount of time.
     *
     * @param address Pointer to a memory region to deregister.
     *
     * @see Device::registerMemory
     * 
     ***************************************************************************
     */
    virtual void 
    deregisterMemory(void* address) = 0;

    /**
     ***************************************************************************
     * @brief 
     * @details Indicate that a memory region will be used by the host or 
     *        within a kernel running on the device.
     *
     * Indicates that the memory region passed as parameter will be used by the
     * available device(s).
     * \li In the case of HSA devices, this invocation is optional if the 
     *     memory type of the region is hsacommon::HEAP_TYPE_SYSTEM, and 
     *     might speed up accesses to that memory region. If the type is 
     *     hsacommon::HEAP_TYPE_DEVICE, it is mandatory for the device to map
     *     the region before accessing it.
     * \li In case of near-HSA devices, it is mandatory for the device to map
     *     the region before accessing it, independently of the type of the 
     *     region.
     *
     * @param address Pointer to valid memory region.
     * @param size Requested mapping size in bytes, must be greater than zero.
     *
     * @exception HsaException if the input is invalid or the runtime is 
     *            unable to map enough memory to perform the requested operation.
     *
     ***************************************************************************
     */
    virtual void 
    mapMemory(void* address, size_t size) = 0;
    
    /**
     ***************************************************************************
     * @brief Unmap a memory region
     * @details This HSA device interface is used to unmap the specified memory
     *          region previously mapped through a call to Device::mapMemory(). 
     *          Attempting to unmap a memory region that was not previously
     *          mapped shall result in an error.
     * 
     * @param address Pointer to a valid memory region to unmap.
     *
     ***************************************************************************
     */
    virtual void 
    unmapMemory(void* address) = 0;

    /**
     ***************************************************************************
     * @brief Create a device command queue
     * @details This HSA runtime device interface isused to create and
     *          and initialize an hsa::Queue command queue on the associated 
     *          device.  The created queue provides a channel for submitting 
     *          asynchronous command operations to the device.
     *
     *          When specified, the memory location specified by \c address 
     *          argument is used as the base of the queue ring buffer.
     *
     * @param size Size of the queue ring buffer in bytes.
     * @param address Pointer to the base of the queue ring buffer or NULL if 
     *                the queue buffer is to be allocated.
     * @param percent Percentage of compute resources to use on the associated
     *                device to be used by the queue.  The percentage value
     *                must be a multiple of 10 and be between 10 and 100.  The 
     *                default value is 100.
     * @param priority Priority of the queue wtih respect to scheduling its 
     *                 command packets relative to other queues associated 
     *                 with the device.  The priority value must be between 
     *                 -3 and 3.  The default value is 0.
     *
     * @see hsa::Queue
     *
     ***************************************************************************
     */
    virtual hsa::Queue *createQueue(uint32_t size = 0,
                                    void *address = NULL, 
                                    uint32_t percent = QUEUE_DEFAULT_PERCENT, 
                                    uint32_t priority = QUEUE_DEFAULT_PRIORITY) = 0;

    /**
     ***************************************************************************
     * @brief Acquire a list of the device cache descriptors
     * @details This HSA runtime device interface acquires a vector containing 
     *          pointers to the cache descriptors for the associated device.  
     *          Each entry in the vector points to an individual cache bank
     *          descriptor.
     *
     *          The descriptor is used for querying the attributes of the 
     *          device cache hierarchy for performance optimizations.
     *
     * @return A Reference to a vector of device cache descriptor pointers is 
     *         returned.
     ***************************************************************************
     */
    virtual const hsa::vector<hsa::CacheDescriptor*>& getCacheDescriptors()=0;

    /**
     ***************************************************************************
     * @brief Inquire whether the device is being debugge
     * @details This HSA device interface is used to inquire the maximum 
     *          frequency of the associated device.  
     *
     * @return Maximum device frequency in MHz.
     *
     ***************************************************************************
     */
    virtual bool isDebug()=0;
    
    /**
     ***************************************************************************
     * @brief Inquire the maximum device clock frequency
     * @details This HSA device interface is used to inquire the maximum 
     *          frequency of the associated device.  
     *
     * @return Maximum device frequency in MHz.
     *
     ***************************************************************************
     */
    virtual int getMaxFrequency()=0;

    

    /**
     ***************************************************************************
     * @brief Acquire a synchronized set of host/device clock counters.
     * @details This HSA device interface is used for acquiring a set of 
     *          highly correlated host and device timestamps. 
     *
     *          The frequency returned will be the max frequency reported by 
     *          the ASIC.  Additionally, until the KFD implements the wall 
     *          clock feature, this will return invalid data.
     *
     * @return DeviceClockCounterInfo object containing the timestamps 
     *         from host and device along with device frequency.
     *
     ***************************************************************************
     */
    virtual DeviceClockCounterInfo getClockCounterInfo() = 0;
    
    /**
     ***************************************************************************
     * @brief Retrieve all the trap handler information in the device
     * @param trapHandlerInfo pointer to the trap handler info buffer
     *
     ***************************************************************************
     */
    virtual void getTrapHandlerInfo(TrapHandlerInfo * trapHandlerInfo) = 0;

    /**
     ***************************************************************************
     * @brief control the wave front.
     * @param action actions to be taken on the wavefront
     * @param mode how the actions are taken, single wave, broadcast, etc.
     * @param trapID, this is used for just the action of h_trap, in which
     *  a trap ID is needed.
     * @param msgPtr, pointer to a message indicate various information. 
     *  see the KFD design for specific information.
     *
     ***************************************************************************
     */
    virtual void waveControl(uint32_t action, 
                             uint32_t mode, 
                             uint32_t trapID, 
                             void *msgPtr) = 0;

    #ifdef HSA_EXPERIMENTAL
    /**
     ***************************************************************************
     * @brief Returns performance monitoring units for a device.
     ***************************************************************************  
    */
    virtual hsaperf::Pmu * getPMU() = 0;
    #endif

   /**
     *************************************************hsa*************************
     * @brief Default destructor.
     ***************************************************************************
     */
    virtual ~Device(){};

};

/**
 *******************************************************************************
 * @brief HSA device queue interface class
 * @details The HSA device queue class provides the set of interfaces for 
 *          operating on an HSA queue object.  The queue object provides a 
 *          unique channel for enqueuing work to be processed by the device.
 *
 *          The queue object is capable of performing asynchronous or 
 *          synchronous commands for executing kernels or performing memory 
 *          transfers.
 *
 *          Commands enqueued to the queue are executed in FIFO order relative 
 *          to other commands in the queue.  The order of command completion is 
 *          implementation dependent relative to other enqueued commands. 
 *          Command ordering is provided through the use of HSA event 
 *          dependencies at the time of enqueue.
 *          
 *******************************************************************************
 */
class DLL_PUBLIC Queue
{

public:

    /**
     ***************************************************************************
     * @brief Acquire the queue device
     * @details This HSA queue interface is used to acquire a pointer to the
     *          hsa::Device associated with the queue.  The returned device 
     *          pointer corresponds to the actual device for which the queue
     *          was reated.  
     *
     * @return Pointer to the creating hsa::Device object
     *
     ***************************************************************************
     */
    virtual hsa::Device *getDevice(void) = 0;

    /**
     ***************************************************************************
     * @brief Dispatch a kernel with variable arguments
     * @details This HSA queue interface is used to enqueue dispatch of
     *          a specified HSAIL Kernel for execution.  The dispatched
     *          kernel is launched based on the provided set of launch 
     *          attributes.  The supplied set of arguments are passed to the
     *          dispatched kernel on execution.
     *
     *          An optional list of hsa::Event dependency pointers may be
     *          specified.  Execution of the dispatched event is dependent on
     *          the successful completion of the events in this list.
     *
     * @param kernel Pointer to the hsa::Kernel object to execute
     * @param launchAttr Launch attributes to apply on execution.
     * @param deps Reference to an hsa::vector containing event dependencies 
     *             of the dispatch. 
     * @param numArgs Number of arguments in the variable argument list.
     * @param ... Variable length list of arguments to be passed to the
     *            dispatched kernel
     *
     * @return Pointer to an hsa::DispatchEvent
     *
     ***************************************************************************
     */
    virtual hsa::DispatchEvent *dispatch(hsa::Kernel *kernel,
                                         hsa::LaunchAttributes launchAttr,
                                         hsa::vector<hsa::Event *> &deps,
                                         uint32_t numArgs, ...) = 0;
    
    /**
     ***************************************************************************
     * @brief Dispatch a kernel with an argument list
     * @details This HSA queue interface is used to enqueue dispatch of
     *          a specified HSAIL Kernel for execution.  The dispatched
     *          kernel is launched based on the provided set of launch 
     *          attributes.  The supplied set of arguments are passed to the
     *          dispatched kernel on execution.
     *
     *          An optional list of hsa::Event dependency pointers may be
     *          specified.  Execution of the dispatched event is dependent on
     *          the successful completion of the events in this list.
     *
     * @param kernel Pointer to the hsa::Kernel object to execute
     * @param launchAttr Launch attributes to apply on execution.
     * @param deps Reference to an hsa::vector containing event dependencies 
     *             of the dispatch. 
     * @param argList Reference to a list of arguments to be passed to the 
     *                dispatched kernel.
     *
     * @return Pointer to an hsa::DispatchEvent
     *
     ***************************************************************************
     */
    virtual hsa::DispatchEvent *dispatch(hsa::Kernel *kernel,
                                         hsa::LaunchAttributes launchAttr,
                                         hsa::vector<hsa::Event *> &deps,
                                         hsa::vector<KernelArg> &argList) = 0;
    
    /**
     ***************************************************************************
     * @brief Flush any outstanding commands on the queue
     * @details This HSA queue interface is used to flush any outstanding 
     *          commands in the associated queue.  This interface is 
     *          non-blocking.
     *
     ***************************************************************************
     */
    virtual void flush()=0;

    /**
     ***************************************************************************
     * @brief Default destructor.
     ***************************************************************************
     */
    virtual ~Queue(){};
};


/**
 *******************************************************************************
 * @brief HSA runtime program interface class
 * @details The HSA runtime program interface class provides a 
 *          set of interfaces for operating on an HSAIL program object.  The 
 *          program object is a container for an HSAIL program binary and is
 *          used for managing the devices associated with the program object.  
 *
 *          The program object is also the factory for producing kernel objects
 *          from the associated HSAIL program binary.
 *
 *******************************************************************************
 */
class DLL_PUBLIC Program
{
public:
    /*! 
     ***************************************************************************
     * @brief Compile a kernel on multiple devices
     * @details This HSA program interface is used to build a specified kernel
     *          across all devices on the associated program.  A list of device
     *          kernels is returned that corresponds to each of the devices 
     *          associated with the program.
     *
     * @param kernelName Pointer to a NULL-terminated string containing the 
     *                   kernel name. 
     * @param options Pointer to a NULL terminated string containing build
     *                options.
     *
     * @return Pointer to a list of hsa::Kernel objects.
     *
     ***************************************************************************
     */
    virtual hsa::Kernel *compileKernel(const char* kernelName,
                                       const char* options) = 0;

      /**
     ***************************************************************************
     * @brief Default destructor.
     ***************************************************************************
     */
    virtual ~Program(){};

    /*! 
     ***************************************************************************
     * @brief Associate a device with the program
     * @details This HSA program interface is used to associate an hsa::Device
     *          object with the associated program.  By associating the device
     *          with the program, the device is eligible for participating in
     *          operations that span the program devices.
     *
     ***************************************************************************
     */
    /*! @brief Add a device to the list of devices a kernel is built against.
    * @param device The device to add.
    */

    virtual void addDevice(hsa::Device * device)  = 0;
};

/**
 *******************************************************************************
 * @brief HSA executable kernel function interface class
 * @details The HSA kernel class provides the set of interfaces needed for 
 *          operating on and executing an HSA kernel function.
 *
 *******************************************************************************
 */
class DLL_PUBLIC Kernel
{
public:

    /* 
     ***************************************************************************
     * @brief Acquire a pointer to the kernel ISA
     * @details This HSA kernel interface is used to acquire a pointer to the
     *          ISA associated with the kernel. 
     *
     * @return - Pointer to the kernel ISA
     *
     ***************************************************************************
     */
   virtual void* getISA() = 0;

   /* 
    ****************************************************************************
    * @brief Inquire the size of the kernel ISA
    * @details This HSA kernel interface is used to inquire the size of the 
    *          kernel ISA associated with the kernel.
    *
    * @return - Size of the kernel ISA in bytes.
    *
    ****************************************************************************
    */
   virtual size_t getSizeOfISA() = 0;

   /* 
    ****************************************************************************
    * @brief Inquire the kernel function name
    * @details This HSA kernel inteface is used to inquire the name of the 
    *          kernel function associated with the kernel.
    *
    * @return Reference to an hsa::string containing the kernel function name.
    *
    ****************************************************************************
    */
   virtual hsa::string& getName()=0;

   
   /** 
     ****************************************************************************
     * @brief Inquire base of kernel group memory
     * @details This HSA kernel interface is used to inquire the starting address
     *          of the group memory for the assoicated kernel.
     *
     *          The returned address may be used to calculate the address of
     *          group memory arguments passed to the kernel.  It is the callers
     *          responsibility to check the upper bound of the group memory 
     *          region of associated device.  The caller must also specify the 
     *          total group memory used in the dispatch launch attribute 
     *          groupMemorySize field. 
     *
     * @return Pointer to group memory.
     *
     ****************************************************************************
     */
    virtual const void* getGroupMemoryBase() const = 0;
    
    /**
     ***************************************************************************
     * @brief Default destructor.
     ***************************************************************************
     */
    virtual ~Kernel(){};
};

/**
 *******************************************************************************
 * @brief HSA runtime event interface class
 * @details The HSA runtime event class provides the a set of interfaces for 
 *          operating on an HSA event object.  Event objects are used for 
 *          blocking on and querying the state of asynchronous operations.  
 *******************************************************************************
 */
class DLL_PUBLIC Event
{
public:
    /**
     ***************************************************************************
     * @brief Wait on the event to be signaled
     * @details This HSA event interface is used to wait on the signaling 
     *          of the associated event object.  The interface will block until
     *          one of the following occurs:
     *          @li The event is succesfully signaled.
     *          @li The specified timeout interval is exceeded.
     *          @li An error occurs during the processing of the event.
     *
     * @param timeOut Number of milliseconds to block or 0 to block indefintely.
     *
     * @return HSA status value is returned indicating the status of the wait.
     *
     ***************************************************************************
     */
    virtual hsa::Status wait(uint32_t timeOut = 0) = 0;

    /**
     ***************************************************************************
     * @brief Default destructor.
     ***************************************************************************
     */
    virtual ~Event(){};
};
 
/**
 *******************************************************************************
 * @brief HSA runtime event interface class
 * @details The HSA runtime event class provides the a set of interfaces for 
 *          operating on an HSA event object.  Event objects are used for 
 *          blocking on and querying the state of asynchronous operations.  
 * @brief HSA runtime dispatch event interface class
 * @details The HSA runtime dispatch event class is an HSA::event that is
 *          specifically associated with a dispatch queue command.  This 
 *          event variant provides interfaces related to a dispatch operation 
 *
 *          Specifically use of the dispatch event allows users to monitor the
 *          various states of a kernel dispatch.  As well, information
 *          pertaining to the dispatch may be acquired through interfaces on
 *          the object.
 *
 *******************************************************************************
 */
class DLL_PUBLIC DispatchEvent : public hsa::Event {

public:
    /**
     ***************************************************************************
     * @brief Inquire the state of the dispatch command
     * @details This HSA dispatch event interface is used to inquire the
     *          current execution state of the dispatch command associated with
     *          the event.  
     *          The state can be one of the following:
     *          @li STATE_INITIATED @copydoc STATE_INITIATED
     *          @li STATE_BLOCKED @copydoc STATE_BLOCKED
     *          @li STATE_SUBMITTED @copydoc STATE_SUBMITTED
     *          @li STATE_STARTED @copydoc STATE_STARTED
     *          @li STATE_COMPLETED @copydoc STATE_COMPLETED
     *
     * @return hsa::EventState corresponding to the current state. 
     *
     * @throws HsaException if an error occurs in runtime
     *
     ***************************************************************************
     */
    virtual hsa::EventState getState(void) = 0;

    /**
     ***************************************************************************
     * @brief Acquire the unique dispatch ID
     * @details This HSA dispatch event interface is used to acquire a unique
     *          ID corresponding to the dispatch command associated with the 
     *          event.  This unique ID may be used to identify a specific 
     *          dispatch of a shared kernel.
     *
     * @return Unique kernel dispatch ID.
     * 
     ***************************************************************************
     */
    virtual uint32_t getDispatchId(void) = 0;

    /**
     ***************************************************************************
     * @brief Acquire the dispatched kernel arguments
     * @details This HSA dispatch event interface is used to acquire a list of
     *          the kernel arguments supplied to the dispatch command 
     *          associated with this event.
     *
     * @note The argument list reference will not be the same as that
     *       supplied by the user at dispatch time.
     *
     * @return Reference to a list of kernel dispatch arguments.
     * 
     ***************************************************************************
     */
    virtual hsa::vector<KernelArg> & getKernelArguments(void) = 0;

    /**
     ***************************************************************************
     * @brief Acquire the dispatched kernel launch attributes
     * @details This HSA dispatch event interface is used to acquire the 
     *          launch attributes of the dispatch command associated with this
     *          event.  The launch attributes will match those specified at 
     *          dispatch. 
     *
     * @note The launch attribute reference will not be the same as that
     *       supplied by the user at dispatch time.
     *
     * @return Reference to an hsa::Launchattributes object
     *
     ***************************************************************************
     */
    virtual hsa::LaunchAttributes & getLaunchAttributes(void) = 0;

    /**
     ***************************************************************************
     * @brief Acquire a pointer to the dispatched ISA
     * @details This HSA dispatch event interface is used to acquire a pointer
     *          to the kernel ISA associated with the dispatch command.  In 
     *          addition the size of the dispatched kernel ISA is also 
     *          acquired.
     *
     * @param address Pointer to the dispatched kernel ISA 
     * @param size Size of the dispatched kernel ISA
     *
     * @return void
     *
     ***************************************************************************
     */
    virtual void getISA(void * &address, size_t &size) = 0;

    /**
     ***************************************************************************
     * @brief Acquire the dispatched kernel event dependencies
     * @details This HSA dispatch event interface is used to acquire the list
     *          of event dependencies of the dispatch command associated with
     *          the event.  The list of event dependencies will match those 
     *          specified at dispatch.  
     *
     * @return Reference to a list of event dependencies.
     *
     ***************************************************************************
     */
    virtual hsa::vector<hsa::Event *> & getDependencies(void) = 0;

    /**
     ***************************************************************************
     * @brief  Acquire the event state timestamp
     * @details This HSA dispatch event interfae is used to inquire the 
     *          timestamp of the specified dispatch command state associated
     *          with the event.  The interface returns a device timestamp
     *          relative to the device associated with the dispatch queue. 
     *          The returned timestamp value may be used in conjunction with
     *          the device clock frequency to determine the corresponding
     *          real time.  Querying the timestamp for a future event state
     *          will return a value of 0.
     * 
     * @return 64-bit integer timestamp or 0 if the state has not been reached. 
     *
     ***************************************************************************
     */
    virtual uint64_t getTimestamp(hsa::EventState state) = 0;
   
    /**
     ***************************************************************************
     * @brief Default destructor.
     ***************************************************************************
     */
    ~DispatchEvent() {};
}; /***    hsa::DispatchEvent    ***/




extern "C" {

/**
 *******************************************************************************
 * @brief Acquire the base HSA runtime object
 * @details This HSA global interface is used for acquiring the base HSA 
 *          singleton runtime object for simplified explicit loading.  This is
 *          the only global interface that must be acquired when explicitly 
 *          loading the HSA runtime.  All other global exported functions 
 *          are accessible via the returned HsaRuntime singleton instance 
 *          returned by this function.
 *
 * @return Pointer to the singleton HSA runtime object
 */
DLL_PUBLIC hsa::Runtime* getRuntime();

/**
 * @brief Convenience typedef for typecasting the function pointer as part of 
 *        the dynamic load.
 */
typedef hsa::Runtime* (*fptr_getRuntime)();
 
}

/*!
 *******************************************************************************
 * @brief Global HSA createProgram interface.
 * @details This global HSA interface is the same as 
 *          hsa::Runtime::createProgram()
 *******************************************************************************
 */
DLL_PUBLIC hsa::Program* createProgram(char *binary, 
                                        size_t binarySize,
                                        hsa::vector<hsa::Device *> *devices);
DLL_PUBLIC hsa::Program* createProgramFromFile(
                                const char* fileName,
                                hsa::vector<hsa::Device *> *devices);
DLL_PUBLIC void destroyProgram(hsa::Program* program);
DLL_PUBLIC uint32_t getDeviceCount();
DLL_PUBLIC const hsa::vector<hsa::Device*>& getDevices();
DLL_PUBLIC const hsa::string& getVersion();
DLL_PUBLIC void* allocateGlobalMemory(const size_t size, 
                                      const size_t alignment = 0);
DLL_PUBLIC void freeGlobalMemory(void* address);





/**
 *******************************************************************************
 * @brief HSA runtime trap event interface class
 * @details The HSA runtime trap event interface class provides a set of 
 *          interfaces for operating on an HSA trap event object.
 *******************************************************************************
 */
class DLL_PUBLIC TrapEvent : public Event 
{
public:
    /**
     ***************************************************************************
     * @brief When this event is signaled, debugger users will need
     * get various information such as scratch memory, which will be
     * available in the debugger dispatch event. To avoid the duplicate
     * of the APIs, TrapEvent returns the debugger dispach object
     * for users to get those information.
     ***************************************************************************
     */
    virtual DispatchEvent *getPayload() = 0;

    /**
     ***************************************************************************
     * @brief Default destructor.
     ***************************************************************************
     */
    virtual ~TrapEvent() {};

};

/**
 *******************************************************************************
 * @brief HSA runtime trap handler interface class
 * @details The HSA runtime trap handler interface class provides a set of 
 *          interfaces for operating on an HSA trap handler object.
 *******************************************************************************
 */
class TrapHandler {
public:
	/**
     ***************************************************************************
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
     ***************************************************************************
     */
    virtual void setTrapHandler(uint32_t trapType,
                                void *trapHandler,
                                size_t trapHandlerSizeByte) = 0;

    /**
     ***************************************************************************
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
     ***************************************************************************
     */
    virtual void setTrapHandlerBuffer(uint32_t trapType,
                                      void *trapHandlerBuffer,
                                      size_t trapHandlerBufferSizeByte) = 0;

    /**
     ***************************************************************************
     * @brief copy trap handler info from device object to be used by the 
     *        dispatch
     *
     * @param trapHandlerInfo pointer to the trap handler info to be copied from
     *
     ***************************************************************************
     */
    virtual void copyTrapHandlerFromDevice(Device *dev) = 0;
	
    /**
     ***************************************************************************
     * @brief get all the trap handlers 
     *
     * @param trapHandlerInfo pointer to the trap handler info to be copied from
     *
     ***************************************************************************
     */
    virtual void getTrapHandlerInfo(Device *dev) = 0;

    /**
     ***************************************************************************
     * @brief Default destructor.
     ***************************************************************************
     */
    virtual ~TrapHandler() {}
};

/**
 * @}
 */
}

#endif

