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
 * @brief Allocate system memory.
 *
 * Allocates \c size bytes of linear memory in the
 * system and returns a pointer to the allocated memory. The memory is
 * not initialized. If \c size is 0, then allocateMemory() returns NULL.
 *
 * The allocated system memory will be registered and be available to 
 * all the devices in the platform.
 *
 * The type of the allocated memory is by default pageable and cache coherent.
 *
 * @param size Requested allocation size in bytes.
 * @param alignment The alignment size in bytes for the address of resulting 
 *                  allocation. The value of this parameter needs to be a power 
 *                  of two. The default value is zero, where no particular 
 *                  alignment will be applied.
 * @exception HsaException if the input is invalid, or if the runtime is
 *            unable to allocate enough memory to perform the requested operation.
 * @return Pointer to allocated memory.
 *
 */
DLL_PUBLIC void* allocateMemory(const size_t size, const size_t alignment = 0);

/**
 * @brief Allocate system or device memory.
 *
 * Allocates \c size bytes of linear memory in the specified device or in the
 * system and returns a pointer to the allocated memory. The memory is
 * not initialized. If \c size is 0, then allocateMemory() returns NULL.
 *
 * The memory allocated is automatically registered by the affected device(s).
 *
 * If the SYSTEM_MEMORY flag is set, the runtime will try to allocate
 * system memory close to \c device. In any case, system memory will
 * be registered and be available to all the devices in the platform.
 * All the other memory types require that a valid device is specified.
 *
 * The type of memory is one of MemoryType. Further options might be
 * indicated using a mask.
 * @li If the type is SYSTEM_MEMORY, further options can be indicated by creating a
 *     mask using the values in SystemMemoryOptions. For instance, to allocate
 *     pinned, read only system memory we would specify the type as
 *     <tt>SYSTEM_MEMORY | PINNED |  READ_ONLY</tt>.
 *     If no option are specified, the runtime assumes the mask
 *     <tt>SYSTEM_MEMORY | PAGEABLE | CACHED</tt>.
 * @li If the type is LOCAL_MEMORY, further options can be indicated by creating a
 * 	   mask using the values in LocalMemoryOptions. If no options are
 * 	   specified, the runtime assumes the mask <tt>LOCAL_MEMORY | CPU_ACCESSIBLE</tt>.
 * @li All the other memory types do not support further options.
 *
 * @param size Requested allocation size in bytes.
 * @param alignment The alignment size in bytes for the address of resulting 
 *                  allocation. The value of this parameter needs to be a power 
 *                  of two. The default value is zero, where no particular 
 *                  alignment will be applied.
 * @param type Type of memory to be allocated.
 * @param dev Device where the allocated memory will be available.
 * @exception HsaException if the input is invalid, or if the runtime is
 *            unable to allocate enough memory to perform the requested operation.
 * @return Pointer to allocated memory.
 *
 * @see MemoryType
 * @see SystemMemoryOptions
 * @see LocalMemoryOptions
 */
DLL_PUBLIC void* allocateMemory(const size_t size, const size_t alignment, const uint32_t type, const Device& dev);

/**
 * @brief Frees system or device memory.
 *
 * Frees the memory space pointed to by ptr, which must have been
 * returned by a previous call to allocateMemory(). Otherwise, or if
 * freeMemory(ptr) has already been called before, undefined behavior
 * occurs. If a null pointer is passed as argument, no action occurs.The
 * memory deallocated is automatically deregistered in all the corresponding
 * devices.
 *
 * @param ptr Pointer to memory to free.
 * @exception HsaException if the the runtime is
 *            unable to deallocate the specified memory region.
 */
DLL_PUBLIC void freeMemory(void* ptr);

/**
 * @deprecated This method will probably not be part of the final API. Be aware
 * that the documentation might be incomplete or inconsistent.
 *
 * @brief Register system memory to be used by the device.
 *
 * Registers the memory space pointed to by ptr as usable by the device. The
 * memory space has to be system memory allocated by standard OS mechanisms
 * (malloc, calloc, etc.). If a pointer allocated by allocateMemory is passed
 * as argument, undefined behavior occurs. If a null pointer is passed as
 * argument, or size is 0, no action occurs.
 *
 * @param ptr Pointer to system memory.
 * @param size Requested registration size in bytes.
 * @param type Deprecated, to be removed.
 */
DLL_PUBLIC void 
registerMemory(void* ptr, const size_t size, const uint32_t type);

/**
 * @deprecated This method will probably not be part of the final API. Be aware
 * that the documentation might be incomplete or inconsistent.
 *
 * @brief Deregister system memory previously registered by the device.
 *
 * Deregisters the memory space pointed to by ptr, which must been
 * registered by a previous call to registerMemory(). Otherwise, undefined
 * behavior ocurrs. If a null pointer is passed as argument, no action
 * occurs.
 *
 * @param ptr Pointer to memory to deregister.
 */
DLL_PUBLIC void 
deregisterMemory(void* ptr);

/**
 * @brief Indicate that a memory region will be used by the device(s).
 *
 * Indicates that the memory region passed as parameter will be used by the
 * available device(s).
 * \li In the case of HSA devices, this invocation is optional if the memory
 *     type of the region is SYSTEM_MEMORY, and might speed up accesses to
 *     that memory
 *     region. If the type is LOCAL_MEMORY, it is mandatory for the device
 *     to map the region before accessing it.
 * \li In case of near-HSA devices, it is mandatory for the device to map
 *     the region before accessing it, independently of the type of the region.
 *
 * If a null pointer is passed as argument, or size is 0, no action occurs.
 *
 * @param ptr Pointer to memory.
 * @param size Requested mapping size in bytes.
 * @exception HsaException if the runtime is unable to map enough memory to
 *            perform the requested operation.
 *
 */
DLL_PUBLIC void 
mapMemory(void* ptr, const size_t size);

/**
 * @brief Indicate that a memory regions will not longer be used by the device(s).
 *
 * Unmaps the memory space pointed to by the parameter, which must been
 * mapped by a previous call to hsacore::mapMemory(). Otherwise, undefined
 * behavior ocurrs. If a null pointer is passed as argument, no action
 * occurs. Note that, in HSA devices, a SYTEM_MEMORY memory space is still
 * accessible by the device even after unmapping it.
 *
 * @param ptr Pointer to memory to unmap.
 * @exception HsaException if the runtime is unable to unmap the specified
 *            memory region.
 *
 */
DLL_PUBLIC void 
unmapMemory(void* ptr);

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
    virtual const vector<MemoryDescriptor*>& getMemoryDescriptors()=0;

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
    unsigned getDeviceCount();

    const vector<hsacore::Device*>& getDevices();

    /**
     * @copydoc hsacore::allocateMemory(const size_t size, const size_t alignment = 0)
     */
    void* 
    allocateMemory(const size_t size, const size_t alignment = 0);

     /**
     * @copydoc hsacore::allocateMemory(const size_t size, const size_t alignment, const uint32_t type, const Device& dev)
     */
    void* 
    allocateMemory(const size_t size, const size_t alignment, const uint32_t type, const Device& dev);

    /**
     * @copydoc hsacore::freeMemory
     */
    void freeMemory(void* ptr);

    /**
     * @copydoc hsacore::registerMemory
     */
    void
    registerMemory(void* ptr, const size_t size, const uint32_t type);

    /**
     * @copydoc hsacore::deregisterMemory
     */
    void
    deregisterMemory(void* ptr);

    /**
     * @copydoc hsacore::mapMemory
     */
    void
    mapMemory(void* ptr, const size_t size);

    /**
     * @copydoc hsacore::unmapMemory
     */
    void
    unmapMemory(void* ptr);

};

/*@}*/

}  // ns

#endif
