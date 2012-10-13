#ifndef _HSACORE_H_
#define _HSACORE_H_

#if defined(__WIN32)
#pragma comment( lib, "hsacore" )
#elif defined(__WIN64)
#pragma comment( lib, "hsacore64" )
#endif // #if defined(__WIN32)


#if !defined(AMD_AMP_HSA_INCLUDES)
#include <vector>
#include <iostream>
#endif // #if !defined(AMD_AMP_HSA_INCLUDES)
#include "hsacorecommon.h"

namespace hsacore
{

/**
 * Define vector and string as entities from hsa namespace
 */
using hsacommon::vector;
using hsacommon::string;
using hsacommon::Status;

/**
 * @addtogroup HSACoreRuntime 
 * Core Runtime Interace Documentation
 * @{
 */

/**
 * @brief enum for the event type to be utilized in one of the constructors
 */
typedef enum _EVENTID
{
    USER_GEN_DEVICE_INTERRUPT  = 0, ///< user-mode generated interrupt
    NODE_CHANGE_EVENT          = 1, ///< "H-NUMA" node change
    DEVICE_STATE_CHANGE_EVENT  = 2, ///< device state change ( start/stop )
    HW_EXCEPTION_EVENT         = 3, ///< GPU shader exception event
    SYSTEM_EVENT               = 4, ///< GPU SYSCALL with parameter info
    DEBUG_EVENT                = 5, ///< debugger event
    PROFILE_EVENT              = 6, ///< GPU signal for profiling
    /*
     * Specifies type as Gpu Memory Write event.
     */
    HSA_MEM_WRITE_EVENT = 7,
    //...
    MAX_EVENTS
} EVENTID;



class Device;
class Queue;
class Event;


///////////////////////////////////////////////////////////////////////////////
///Exported Global Functions
///////////////////////////////////////////////////////////////////////////////

// Add all the global function which are exported from this DLL to the
// HsaCoreApi class.

DLL_PUBLIC unsigned int getDeviceCount();
DLL_PUBLIC const vector<hsacore::Device*>& getDevices();

/**
 * @brief factory API for abstracting the creation of an event class
 *
 * @param queObj queue object whose compute device is used for events.
 *
 * @param usrHandle reference to the data provided by user. It is upto the
 * user to provide valid user data. The class does not impose any
 * constraints nor does it perform any checks
 *
 * @param autoReset if the event object should be reset automatically or
 * manually. True indicates auto reset, false indicates manual reset. 
 *
 * @param initState initial state of event if it is SET or not SET.
 *
 * @return a pointer to a new Event class object
 */
DLL_PUBLIC Event* createCoreEvent(Queue*  queObj,
                                  void *usrHandle,
                                  bool autoReset, bool initState);

/**
 * @brief factory API for abstracting the creation of an event class
 *
 * @param devId Id of the compute device.
 *
 * @param usrHandle reference to the data provided by user. It is upto
 * the user to provide valid user data. The class does not impose any
 * constraints.
 *
 * @param autoReset if the event object should be reset automatically
 * or manually. True indicates manual reset, false otherwise.
 *
 * @param initState initial state of event if it is SET or not SET.
 *
 * @return a pointer to a new Event class object
 */
DLL_PUBLIC Event* createCoreEvent(uint32_t devId,
                                  void *usrHandle,
                                  bool autoReset, bool initState);

/**
 *
 * @brief factory API for abstracting the creation of an event class
 *
 * @param eventId for now the class throws and exception event ID is not 
 * recognized (! (NODE_CHANGE | DEVICE_STATE_CHANGE) )
 *
 * @param autoReset if the event object should be reset automatically
 * or manually. True indicates manual reset, false otherwise.
 *
 * @param initState initial state of event if it is SET or not SET.
 *
 * @throws HsaExeption if input parameters are illegal or invalid.
 *
 * @return a pointer to a new Event class object
 */
DLL_PUBLIC Event* createCoreEvent(EVENTID eventId, bool autoReset, bool initState);

/**
 *
 * @brief factory API for abstracting the creation of an event class
 *
 * @param device pointer to core device 
 *
 * @param eventId for now the class throws and exception event ID is not 
 * recognized (! (NODE_CHANGE | DEVICE_STATE_CHANGE) )
 *
 * @param autoReset if the event object should be reset automatically
 * or manually. True indicates manual reset, false otherwise.
 *
 * @param initState initial state of event if it is SET or not SET.
 *
 * @throws HsaExeption if input parameters are illegal or invalid.
 *
 * @return a pointer to a new Event class object
 */
DLL_PUBLIC Event* createCoreEvent(Device *dev, EVENTID eventId, bool autoReset, bool initState);

/**
 * @brief eventObj destruction API provided, primarily for uniformity. delete e
 * also accomplishes the same thing.
 *
 * @param eventObj the event to be destroyed.
 *
 * @throws HsaExeption if input parameters are illegal or invalid.
 *
 */
DLL_PUBLIC void destroyCoreEvent(Event *eventObj);

/**
 * @deprecated
 * @brief Allocates global memory that is accessible by all AMD devices 
 *        in the platform. 
 *
 * Allocates \c size bytes of linear memory and returns a pointer to 
 * the allocated memory. The memory is not initialized. If \c size 
 * is 0, then this function returns NULL.
 *
 * The allocated memory by default will be system memory that is pageable, 
 * cache coherent and readable / writeable by both host and devices. 
 *
 * @param size Requested allocation size in bytes.
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
allocateGlobalMemory(const size_t size, 
                     const size_t alignment = 0);
      
/**
 * @deprecated
 * @brief Frees a global memory region pointed to by \c ptr, which must have been
 *        returned by a previous call to \c hsacore::allocateGlobalMemory(). Otherwise, 
 *        or if \c freeGlobalMemory(ptr) has already been called before, undefined 
 *        behavior may occur. If a null pointer is passed as argument, no 
 *        action occurs.
 *
 * @param ptr Pointer to the memory to be freed.
 * @exception HsaException if the the runtime is
 *            unable to deallocate the specified memory region.
 */
DLL_PUBLIC void 
freeGlobalMemory(void* ptr);

///////////////////////////////////////////////////////////////////////////////
//Exported Queue interface class
///////////////////////////////////////////////////////////////////////////////

/**
 * @brief the packet type for generation of packet
 */
enum PacketType{
    ISAKERNEL=1,
    CLOCK,
    NULL_EVENT,
    SYNC,
    INDIRECT,
    DMA,
    EOS
};

/**
 * Public interface of HSA Core Queue objects. The Queue
 * interface allows users to get a queue object and submit
 * via this various forms of command packets (PM4).
 *
 * The interface provides two modes of access - Raw and
 * Synchronized. Users choosing Raw mode API should be aware
 * that implementation won't force synchronized access to the
 * underlying queue object. Synchronized access API's are provided
 * to support multi-threaded scenarios.
 */
class DLL_PUBLIC Queue
{
	
public:

    /**
     * Specifies the default device used in building a Hsa Queue.
     */
    static const uint32_t DEFAULT_DEV_IDX = 0;

    /**
     * Specifies the default scheduling rank of a Hsa Queue.
     */
    static const int32_t DEFAULT_SCHED_RANK = 0;

    /**
     * Specifies the default priority of a Hsa Queue.
     */
    static const uint32_t DEFAULT_PRIORITY = 0;

    /**
     * Specifies the default percent of SIMD's used by a Hsa Queue.
     */
    static const uint32_t DEFAULT_PRCNT_SIMD = 100;

    /**
     * API to determine the maximum number of queues that can actually
     * be created for this platform, including all devices.
     *
     * @return the number of queues allowed by the platform including
     * all devices.
     *
     * @note will always return 2 for now.
     */ 
    virtual uint32_t getMaxQueues(void) = 0;

    /**
     * Default destructor
     */
	virtual ~Queue(){};

    /**
     * @brief API to determine if Queue object is valid for access.
     *
     * @note: Accessing Queue api once it has been marked as
     * invalid will lead to exception.
     *
     * @return bool true if it is valid, false otherwise.
     */
    virtual bool isValid() = 0;

    /**
     * @brief API to configure the Queue object.
     *
     * @note: This api is a temporary hack and would go away.
     *
     * @return uint32_t value of virtual memory id used to map
     * Queue buffer in the kernel.
     */
    virtual uint32_t getVmId(void) = 0;

    /**
     * Returns the address at which a new command packet could be written
     * into the queue buffer. This api is provided for users who wish to
     * access Queue buffer in a RAW form. Accessing Queue in this fashion
     * assumes User is knowledgeable and responsible about the correct use
     * of Queue object. For example users should know the end of queue buffer
     * and not try to write past this.
     *
     * @note: Users should write Command (PM4) packets such that they are in
     * multiples of a word size (sizeof(uint32_t), else it will lead to runtime
     * errors.
     *
     * @return uint32_t * address into the queue buffer at which a new Command
     * (PM4) packet should be written.
     */
    virtual uint32_t * getWriteAddr(void) = 0;

    /**
     * Updates the memory mapped Write Register of compute device to indicate
     * availability of a new PM4 packet for execution. The implementation will
     * check if the input parameter is invalid/illegal i.e. causes the address
     * to go past end of queue buffer or is less than current address.
     *
     * @note: Implementation will throw an exception if input parameter is
     * determined to be invalid/illegal.
     *
     * @param pktSize number of words (sizeof(uint32_t) by which current
     * address of execution front end must be updated. Users must specify
     * the number of words used to write into queue buffer and not number
     * of bytes.
     *
     * @return void
     */
    virtual void setWriteAddr(uint32_t pktSize) = 0;

    /**
     * Acquires the address into queue buffer where a new command packet
     * of specified size could be written. The address that is returned
     * is guaranteed to be unique even in a multi-threaded access scenario.
     * The API throws an exception in case not enough space is available in
     * queue buffer or that the requested size is invalid.
     *
     * @note: Implementation will throw an exception if input parameter is
     * determined to be invalid/illegal.
     *
     * @param pktSize number of words (sizeof(uint32_t) used by the Command
     * (PM4) packet. Users must specify the number of words occupied by PM4
     * packet and not number of bytes.
     *
     * @return uint32_t * pointer into the queue buffer where a PM4 packet
     * of specified size could be written.
     */
    virtual uint32_t * acquireWriteAddr(uint32_t pktSize) = 0;

    /**
     * Updates the Write Register of compute device to the end of PM4 packet
     * written into queue buffer. An exception will be thrown if either of
     * the input parameters are invalid/illegal. The update to Write Register
     * will be safe under multi-threaded usage scenario. Furthermore, updates
     * to Write Register are blocking until all prior updates are completed
     * i.e. if two threads T1 & T2 were to call release, then updates by T2
     * will block until T1 has completed its update (assumes T1 acquired the
     * write address first).
     *
     * @param writeAddr pointer into the queue buffer where a PM4 packet was
     * written.
     *
     * @param pktSize number of words in PM4 packet. Size is defined in terms
     * of number of sizeof(uint32_t) words and not in terms of bytes.
     *
     * @return void
     */
    virtual void releaseWriteAddr(uint32_t *writeAddr, uint32_t pktSize) = 0;

    /**
     * This convenience API does three things: Acquires an address into queue
     * buffer, Writes the PM4 packet into buffer and then updates Write
     * Register of compute device to execute the PM4 packet. An exception
     * will be thrown if input parameters are invalid/illegal.
     *
     * @param pktBuffer pointer to the buffer containing PM4 command data.
     *
     * @param pktSize size of the command packet in terms of number of words.
     *
     * @return void
     */
    virtual void execCommand(uint32_t *pktBuffer, uint32_t pktSize) = 0;

    /**
     * @brief Indicates if the Queue has executed all command packets or some
     * are still pending execution.
     *
     * An exception will be thrown in case of: illegal access (queue object
     * is invalid) or an error has occured in the runtime.
     *
     * @return bool true if all packets have been executed, false otherwise.
     */
    virtual bool isEmpty(void) = 0;

    /**
     * @brief Waits upon the Queue object until all outstanding command packets
     * have been executed.
     *
     * API to wait until all outstanding command packets have been executed.
     * An exception will be thrown in case of: illegal access (queue object
     * is invalid) or an error has occured in the runtime.
     *
     * @return void
     */
    virtual void waitEmpty(void) = 0;

    /**
     * @brief Alters the parameters of Queue object that affect its behaviour.
     *
     * Modifies a queue object with regards to its properties: queue buffer
     * address, queue buffer size, its percent use and scheduling priority.
     * Execution of all outstanding PM4 packets of queue must be completed
     * prior to altering the queue's properties.
     *
     * In case any one of the input parameters is invalid/illegal, the old
     * properties will continue to be used.
     *
     * @note: This API is not available until support for user supplied
     * queue buffers is in place.
     *
     * @param buffAddr reference to the new buffer to use for queue.
     *
     * @param buffSize size of the new buffer in terms of words sizeof(uint32_t).
     *
     * @param prcntSimd Percentage of SIMD units on compute device that are
     * allowed for use by a queue object. Users can choose the default value
     * of 100 percent or something else between [10, 100] in increments of 10.
     *
     * @param schedPrior Priority of queue object in scheduling its command
     * packets. Users can choose to provide a default rank.
     *
     * @return void
     */
    virtual void alter(uint32_t *buffAddr, uint32_t buffSize,
                       uint32_t prcntSimd, int32_t schedPrior) = 0;

    /**
     * @brief Destroys a Queue object such that it becomes invalid for
     * further usage.
     *
     * Marks the queue object as invalid and uncouples its link with
     * the underlying compute device's control block. An exception is
     * thrown if queue object has already been marked as invalid or
     * an error occurs while uncoupling it from the underlying device.
     *
     * @note: What happens to command packets that have not yet been
     * executed. Will the call block until all packets have been
     * executed or will it decouple it right away.
     *
     * @return void
     */
    virtual void destroy(void) = 0;

    /**
     * @brief Detaches the command packets of Queue from run list of
     * associated device.
     *
     * Detaches the queue and its command packets from the run list
     * of attached compute device. An exception is thrown if queue
     * object has already been marked as invalid or an error occurs
     * while detaching it from the underlying device.
     *
     * @note: What happens to command packets that have not yet been
     * executed. Will the call block until all packets have been
     * executed or will it detach it right away.
     *
     * @return void 
     */
    virtual void detach(void) = 0;

    /**
     * @brief Reengages the command packets of Queue object with the
     * run list of associated device.
     *
     * Reattaches the queue and its command packets to the run list
     * of attached compute device. An exception is thrown if queue
     * object has already been marked as invalid or an error occurs
     * while reattaching it to the underlying device.
     *
     * @return void
     */
    virtual void reattach(void) = 0;

};

/**
 * @brief API to release a HSA Core queue object. All resources
 * held by the queue object will be released and queue marked as
 * invalid. Use of queue objects once destroyed is illegal.
 *
 * @note: Currently users cannot find out if an error has
 * occured while executing this API. This may be changed
 * to throw an exception.
 *
 * @param queObj reference to queue object being destroyed.
 *
 * @return void
 */
void destroyUserModeQueue(Queue *queObj);

/**
 * API to create a User Mode Queue object. Input parameters specify
 * properties of queue being created.
 *
 * @param buffAddr handle of the buffer for use by queue object.
 *
 * @param buffSize number of words (sizeof(uint32_t)) in queue buffer.
 * Do not confuse it as number of bytes.
 *
 * @param prcntSimd Percentage of SIMD units on compute device that are
 * allowed for use by a queue object. Users can choose the default value
 * of 100 percent or something else between [10, 100] in increments of 10.
 *
 * @param schedPrior Priority of queue object in scheduling its command
 * packets. Users can choose to provide a default rank.
 *
 * @return Queue * handle to a queue object, NULL in case of error.
 */
Queue * createUserModeQueue(uint32_t *&buffAddr, uint32_t &buffSize,
                            uint32_t prcntSimd = hsacore::Queue::DEFAULT_PRCNT_SIMD,
                            int32_t schedPrior = hsacore::Queue::DEFAULT_SCHED_RANK);

/**
 * API to create a User Mode Queue object. Input parameters specify
 * properties of queue being created.
 *
 * @param devIdx index of the device in device list to which the queue
 * object is mapped.
 *
 * @param buffAddr handle of the buffer for use by queue object.
 *
 * @param buffSize number of words (sizeof(uint32_t)) in queue buffer.
 * Do not confuse it as number of bytes.
 *
 * @param prcntSimd Percentage of SIMD units on compute device that are
 * allowed for use by a queue object. Users can choose the default value
 * of 100 percent or something else between [10, 100] in increments of 10.
 *
 * @param schedPrior Priority of queue object in scheduling its command
 * packets. Users can choose to provide a default rank.
 *
 * @return Queue * handle to a queue object, NULL in case of error.
 */
Queue * createUserModeQueue(uint32_t devIdx,
                            uint32_t *&buffAddr, uint32_t &buffSize,
                            uint32_t prcntSimd = hsacore::Queue::DEFAULT_PRCNT_SIMD,
                            int32_t schedPrior = hsacore::Queue::DEFAULT_SCHED_RANK);

/**
 * API to create a User Mode Queue object.
 *
 * @param devObj reference to a compute device to which the queue
 * object is mapped.
 *
 * @param buffAddr handle of the buffer for use by queue object.
 *
 * @param buffSize number of words (sizeof(uint32_t)) in queue buffer.
 *
 * @param prcntSimd Percentage of SIMD units on compute device that are
 * allowed for use by a queue object. Users can choose the default value
 * of 100 percent or something else between [10, 100] in increments of 10.
 *
 * @param schedPrior Priority of queue object in scheduling its command
 * packets. Users can choose to provide a default rank.
 *
 * @return Queue * handle to a queue object, NULL in case of error.
 */
Queue * createUserModeQueue(Device *devObj,
                            uint32_t *&buffAddr, uint32_t &buffSize,
                            uint32_t prcntSimd = hsacore::Queue::DEFAULT_PRCNT_SIMD,
                            int32_t schedPrior = hsacore::Queue::DEFAULT_SCHED_RANK);

///////////////////////////////////////////////////////////////////////////////
///Exported Device interface class
///////////////////////////////////////////////////////////////////////////////
/**
 * @brief hsacore Device class, public interface for hsacoredevice.
 */
class DLL_PUBLIC Device
{

public:

    /**
     * @brief Gets the type of device.
     *
     * @return returns a DeviceType enum.
     *   enum DeviceType { 
     *       CPU, 
     *       GPU,
     *       INVALID
     *   };
     */
    virtual DeviceType getType()=0;
    
    /**
     * @brief Get a unique identifier for this device.
     * @return unique id.
     */
    virtual uint32_t getDeviceId() const =0;
    
    /**
     * @brief Get the number of Compute units in a GPU
     *
     * @return returns the number of compute units
     */
    virtual unsigned int getNumComputeUnits()=0;
    
    /**
     * @brief Get the memory descriptors associated with the node containing the device
     * MemoryDescriptor is a node property, so the device class uses the numanode reference to 
     * retrieve the memory descriptor.
     *
     * @return reference to a vector of memory descriptor pointers.
     */
    virtual const vector<MemoryDescriptor*>& getMemoryDescriptors() const = 0;


       /**
     * @brief Get the cache descriptors associated with the device. Cache descriptor is the
     * interface exposed to the user to access the HsaCacheProperties structures given by the KFD.
     *
     * @return reference to a vector of cache descriptor pointers.
     */
    virtual const vector<CacheDescriptor*>& getCacheDescriptors()=0;


    /**
     * @brief API to check if double precision is supported on the device.
     *
     * @return returns true if double precision is supported by the device.
     */
    virtual bool isDoublePrecision()=0;

      /**
  * @brief API to check if device is being used for anything else
     * such as driving a monitor.
     *
     * @return returns true if no external display is attached to the device.
     */
    virtual bool isDedicatedCompute()=0;

    /**
     * @brief Get the size of the LDS in KB
     *
     * @return returns the size of the local data store in KB.
     */
    virtual uint32_t getMaxGroupMemorySize()=0;

      /**
     * @brief Get the wavefront size
     *
     * @return returns the size of wavefront.
     */
    virtual int getWaveFrontSize()=0;

     /**
     * @brief Get the Max number of waves that can be launched per SIMD.
     *
     * @return returns the max number of waves per SIMD.
     */
    virtual int getMaxWavesPerSIMD()=0;

    /**
     * @brief Get the ASIC Info associated with the core device
     * The ASIC Info class will contain asic specific settings like double precision support,
     * image support etc.
     *
     * @return returns a pointer to the ASICInfo class.
     */
    virtual ASICInfo* getASICInfo()=0;

    /**
    * @brief Get the max frequency of the gpu device
    * 
    * @return max frequency as integer
    */
    virtual int getMaxClockRateOfFCompute()=0;
    
    /**
    * @brief Get the max frequency of the cpu device
    * 
    * @return max frequency as integer
    */
    virtual int getMaxClockRateOfCCompute()=0;
               
    /**
     * API to determine the maximum number of queues that can actually
     * be created for this device.  
     *
     * @return the number of queues allowed per queried device.
     *
     * @note will always return 1 for now.
     */ 
    virtual uint32_t getMaxQueues(void) = 0;

    /**
     * API to release a HSA Core queue object. All resources held
     * by the queue object will be released and queue marked as
     * invalid. Use of queue objects once destroyed is illegal.
     *
     * @note: Currently users cannot find out if an error has
     * occured while executing this API. This may be changed
     * to throw an exception.
     *
     * @param queObj reference to queue object being destroyed.
     *
     * @return void
     */
    virtual void destroyUserModeQueue(Queue *queObj) = 0;

    /**
     * API to create a User Mode Queue object. Input parameters specify
     * properties of queue being created.
     *
     * @param buffAddr handle of the buffer for use by queue object.
     *
     * @param buffSize number of words (sizeof(uint32_t)) in queue buffer.
     * Do not confuse it as number of bytes.
     *
     * @param prcntSimd Percentage of SIMD units on compute device that are
     * allowed for use by a queue object. Users can choose the default value
     * of 100 percent or something else between [10, 100] in increments of 10.
     *
     * @param schedPrior Priority of queue object in scheduling its command
     * packets. Users can choose to provide a default rank.
     *
     * @return Queue * handle to a queue object, NULL in case of error.
     */
    virtual Queue * createUserModeQueue(uint32_t *&buffAddr, uint32_t &buffSize,
                                        uint32_t prcntSimd = hsacore::Queue::DEFAULT_PRCNT_SIMD,
                                        int32_t schedPrior = hsacore::Queue::DEFAULT_SCHED_RANK) = 0;

    /**
     * API to create a User Mode Queue object. Input parameters specify
     * properties of queue being created.
     *
     * @param devIdx index of the device in device list to which the queue
     * object is mapped.
     *
     * @param buffAddr handle of the buffer for use by queue object.
     *
     * @param buffSize number of words (sizeof(uint32_t)) in queue buffer.
     * Do not confuse it as number of bytes.
     *
     * @param prcntSimd Percentage of SIMD units on compute device that are
     * allowed for use by a queue object. Users can choose the default value
     * of 100 percent or something else between [10, 100] in increments of 10.
     *
     * @param schedPrior Priority of queue object in scheduling its command
     * packets. Users can choose to provide a default rank.
     *
     * @return Queue * handle to a queue object, NULL in case of error.
     */
    virtual Queue * createUserModeQueue(uint32_t devIdx,
                                        uint32_t *&buffAddr, uint32_t &buffSize,
                                        uint32_t prcntSimd = hsacore::Queue::DEFAULT_PRCNT_SIMD,
                                        int32_t schedPrior = hsacore::Queue::DEFAULT_SCHED_RANK) = 0;

    /**
     * API to create a User Mode Queue object.
     *
     * @param devObj reference to a compute device to which the queue
     * object is mapped.
     *
     * @param buffAddr handle of the buffer for use by queue object.
     *
     * @param buffSize number of words (sizeof(uint32_t)) in queue buffer.
     *
     * @param prcntSimd Percentage of SIMD units on compute device that are
     * allowed for use by a queue object. Users can choose the default value
     * of 100 percent or something else between [10, 100] in increments of 10.
     *
     * @param schedPrior Priority of queue object in scheduling its command
     * packets. Users can choose to provide a default rank.
     *
     * @return Queue * handle to a queue object, NULL in case of error.
     */
    virtual Queue * createUserModeQueue(Device *devObj,
                                        uint32_t *&buffAddr, uint32_t &buffSize,
                                        uint32_t prcntSimd = hsacore::Queue::DEFAULT_PRCNT_SIMD,
                                        int32_t schedPrior = hsacore::Queue::DEFAULT_SCHED_RANK) = 0;

    /**
     * @brief Allocates global memory with specific properties. The allocated 
     *        memory is accessible by the host and the calling device.
     *
     * Developers can determine the properties of the allocation, such as the 
     * heap type, host/device accessibility, and performance hint, by setting 
     * the \c heapType and \c flag parameter.
     * 
     * The \c heapType parameter must contain one of the following heap types. 
     * @li HEAP_TYPE_SYSTEM The global memory is allocated in the system 
     *                        near the calling device. 
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
     * Additionaly, the \c flag parameter can be combined with 
     * the following performance hints.
     * Note that these options are not mandatory.
     * @li MEMORY_OPTION_UNCACHED Sets the allocated memory to be non-cachable. 
     *                            This option is mutually exclusive with 
     *                            MEMORY_OPTION_WRITE_COMBINED.
     * @li MEMORY_OPTION_WRITE_COMBINED Sets the allocated memory to be 
     *                                  write-combined. This option is mutually 
     *                                  exclusive with MEMORY_OPTION_UNCACHED.
     * @li MEMORY_OPTION_NONPAGEABLE Disables paging on the allocated memory.
     *
     * @param size Size of the allocation in bytes.
     * @param alignment The alignment size in bytes for the address of resulting 
     *                  allocation. If the value is zero, no particular alignment 
     *                  will be applied. If the value is not zero, it needs to be 
     *                  a power of two and minimum of sizeof(void*). 
     * @param heapType The backing storage of the allocated global memory.
     * @param flag The type and propertird of the allocated memory.
     * @param dev The device argument for localizing the memory allocation.
     * @exception HsaException if the input is invalid, or if the runtime is
     *            unable to allocate enough memory to perform the requested 
     *            operation.
     * @return Pointer to the allocated memory.
     *
     * @see MemoryType
     * @see MemoryOption
     */
    virtual void* 
    allocateGlobalMemory(const size_t size, 
                         const size_t alignment, 
                         const HeapType heapType,
                         const uint32_t flag) = 0;

    /**
     * @brief Frees global memory, which must have been returned by a previous 
     *        call to \c hsacore::Device::allocateGlobalMemory(). Otherwise, 
     *        or if \c freeGlobalMemory(ptr) has already been called before, 
     *        undefined behavior may occur. If a null pointer is passed as 
     *        argument, no action occurs.
     *
     * @param ptr Pointer to the memory to be freed.
     * @exception HsaException if the the runtime is
     *            unable to deallocate the specified memory region.
     */
    virtual void 
    freeGlobalMemory(void* ptr) = 0;

    /**
     * @brief Gives hint to the runtime that a particular memory region
     *        will be used by the calling device. This routine is optional 
     *        and is exclusively provided for the purpose of performance.
     *
     * Registration to a memory region, which overlaps a previously registered
     * memory is allowed. This includes registering the same memory 
     * region multiple times.
     *
     * Memory region allocated with hsacore::allocateGlobalMemory or 
     * hsacore::Device::allocateGlobalMemory is automatically registered.
     *
     * @param ptr Pointer to a valid system memory.
     * @param size Requested registration size in bytes.
     */
    virtual void 
    registerMemory(void* ptr, const size_t size) = 0;

    /**
     * @brief Deregister system memory previously registered by the device.
     *
     * Deregisters the memory space pointed to by ptr, which must been
     * registered by a previous call to hsacore::Device::registerMemory(). 
     * Otherwise, undefined behavior ocurrs. A memory region that was registered 
     * multiple times needs to be deregistered for the same amount of time.
     * If a null pointer is passed as argument, no action occurs.
     *
     * Memory region deallocated with hsacore::freeGlobalMemory or 
     * hsacore::Device::freeGlobalMemory is automatically deregistered.
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
     * If a null pointer is passed as argument, or size is 0, no action occurs.
     *
     * @param ptr Pointer to a valid memory.
     * @param size Requested mapping size in bytes.
     * @exception HsaException if the runtime is unable to map enough memory to
     *            perform the requested operation.
     *
     */
    virtual void 
    mapMemory(void* ptr, const size_t size) = 0;

    /**
     * @brief Indicates that a memory regions will not longer be used 
     *        by the host or calling device.
     *
     * The memory region to be unmapped must have been
     * mapped by a previous call to hsacore::Device::mapMemory(). Otherwise, undefined
     * behavior ocurrs. If a null pointer is passed as argument, no action
     * occurs. 
     *
     * @param ptr Pointer to a valid memory to unmap.
     * @exception HsaException if the runtime is unable to unmap the specified
     *            memory region.
     *
     */
    virtual void 
    unmapMemory(void* ptr) = 0;

    /**
     * API to control the wave execution on this device.
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
     * @return a hardware specific PMU structure.
     */
    virtual void waveControl(uint32_t       Operand,
                        uint32_t       Mode,
                        uint32_t       TrapId,
                        void*          msgPtr) = 0;

    virtual ~Device(){};

    /**
    * @brief Returns a structure of highly correlated host and device times
    *        as well as the frequency of the device for conversion purposes.
    *
    * The frequency returned will be the max frequency reported by the ASIC.
    * Additionally, until the KFD implements the wall clock feature, this will
    * return invalid data.
    *
    * @return DeviceClockCounterInfo containing the clock stamps from host 
    *         and device along with device frequency.
    */
    virtual DeviceClockCounterInfo getClockCounterInfo() = 0;    
};


/**
 * @brief Event interface of Hsacore namespace. Objects which implement this
 * interface allow users to synchronize over kernel execution.
 *
 */
class DLL_PUBLIC Event {

public:

    /**
     * @brief destructor. Calls the KMD function to destroy the event
     */
    virtual ~Event(){};

    /**
     * @brief Waits on the event object until it is signalled or timeout
     * occurs. The wait is specified in terms of milliseconds. The literal
     * 0x00 is specified for indefinite period.
     *
     * @param timeOut in milliseconds, 0 means blocking wait while values
     * greater than zero specify the time to wait.
     *
     * @return hsacore::Status return value indicating success or timeout.
     */
    virtual hsacore::Status wait(uint32_t timeOut = 0x00) = 0;

    /**
     * @brief Returns the status of event i.e. it is signaled or not. If the
     * event object was created with autoReset policy this will reset the
     * event.
     *
     * @return bool true if event object is signaled, false otherwise.
     *
     * @throws HsaException if an error occurs in runtime
     */
    virtual bool getEventState(void) = 0;

    /**
     * @bried Signals the event object.
     *
     * If a thread is waiting on this event object it will unblock
     * that thread.
     *
     * @return bool true if successful in signalling, false otherwise.
     */
    virtual bool setEventState(void) = 0;

    /**
     * @brief Resets the event data if the object is valid. A request to reset
     * is invalid if the event object has been invalid
     *
     * @note: is reset valid when event is created with autoreset flag?
     *
     */
    virtual void reset(void) = 0;

    /**
     * @brief Returns the current value of user handle
     *
     * @return the current value of user handle
     */
    virtual void * getUsrHandle(void) = 0;

    /**
     * @brief set a new value for the user handle
     *
     * @return success if set, users burden to ensure MT races are dealt with
     */
    virtual void setUsrHandle(void *usrHandle) = 0;

    /**
     * @brief Returns information that is to be used in End Of Pipe packet (EOP)
     * construction as the 5th and 3rd words respectivley, counting starts @ 1.
     *
     * @param evntValue value to be passed to End Of Pipe (EOP) as the
     * 5th word.
     *
     * @param evntAddress address to be passed to End Of Pipe (EOP) as the
     * 3rd word.
     *
     * @throws HsaException when either of the arguments is null
     */
    virtual void getTriggerInfo(uint32_t &eventValue, uint64_t &eventAddress) = 0;

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
                                  uint32_t eventCnt, hsacore::Event **eventList) = 0;
};



///////////////////////////////////////////////////////////////////////////////
//API accessor exports, to be used when DLL is loaded explicitly.  When the DLL
//is loaded explicitly, the above API can be accesses via getRuntime() exported
//function and the IRuntimeApi interface class.
///////////////////////////////////////////////////////////////////////////////
class HsaCoreApi;

// When the DLL is explicitly loaded this will be the only function called
// using GetProcAddress().  All other global functions will be called via
// IHsaCoreApi singleton instance returned by this function.
extern "C" {
DLL_PUBLIC HsaCoreApi& getRuntime();

// typedef comes handy when GetProcAddress() returns the func pointer which
// needs to be cast.
typedef HsaCoreApi& (*fptr_getRuntime)();
}

/**
 * @brief Core API accessor.
 */
class DLL_PUBLIC HsaCoreApi
{
// The only purpose of this class is to give access to the exported global
// functions of the DLL when it(DLL) is explicitly loaded.

public:
    // For each function in exported global functions above (hsacore.h) add
    // the same function here as a public method of this(IHsaCoreApi)
    // interface class.
    virtual unsigned getDeviceCount() = 0;

    virtual const vector<hsacore::Device*>& getDevices() = 0;

    /**
     * @copydoc hsacore::allocateGlobalMemory(size_t,size_t)
     */
    virtual void* 
    allocateGlobalMemory(const size_t size,
                         const size_t alignment = 0) = 0;

    /**
     * @copydoc hsacore::freeGlobalMemory(void*)
     */
    virtual void 
    freeGlobalMemory(void* ptr) = 0;

    virtual ~HsaCoreApi(){};
};

/*@}*/

}  // ns

#endif
