//depot/stg/hsa/drivers/hsa/api/core/common/hsacorecommon.h#9 - edit change 787886 (text)
#ifndef _HSACORECOMMON_H_
#define _HSACORECOMMON_H_

#if defined _WIN32 || defined __CYGWIN__
    #ifdef __GNUC__
        #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
        #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
    #define DLL_LOCAL
#else
    #if __GNUC__ >= 4
        #define DLL_PUBLIC __attribute__ ((visibility ("default")))
        #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
    #else
        #define DLL_PUBLIC
        #define DLL_LOCAL
    #endif
#endif


#if !defined(AMD_AMP_HSA_INCLUDES)
#include <vector>
#include <exception>
#include <string>

    #ifdef ATI_OS_WIN
        #include <cstdint>
    #endif
    
    #ifdef ATI_OS_LINUX
        #include <unistd.h>
        #include <stdexcept>
    #endif

#endif

/** 
 * @addtogroup HSACoreCommonTyps 
 * Cross-Core-Module common Types
 *  @{
 */
namespace hsacore
{
   
/** 
 * @addtogroup HSACoreCommonTyps 
 * Cross-Core-Module common Types
 *  @{
 */
  
// TODO: Replace out_of_range with an HSART exception
/**
 * @brief Class hsa::vector replaces std::vector container.
 * @tparam T Type of object
 */
template<typename T>
class vector
{
public:
    typedef T* iterator;
    typedef T* const_iterator;

    /**
     * @brief Default constructor, initializes member variables to 0.
     */
    vector() : items_(0), data_(0), capacity_(0), size_(0) { }

    /**
     * @brief Destructor, destroys vector.
     */
    ~vector()
    {
        try
        {
            clear();
            delete[] data_;
        }
        catch(...){
            delete[] data_;
        }
    }

    /**
     * @brief Copy constructor
     * @param rhs Vector to be copied to current memory
     *        location.
     */
    vector(const vector& rhs) :
        items_(0), data_(0), capacity_(0), size_(0)
    {
        if(rhs.size_ != 0){
            assign(rhs.begin(), rhs.end());
        }
    }

    /**
     * @brief Copy Operator function, assigns a copy of 
     *        rhs as content of vector object.
     * @return returns *this, vector object with copied content.
     * @param rhs Vector to be copied to current memory
     *        location.
     */
    const vector& operator=(const vector& rhs)
    {
        if(this == &rhs){
            return *this;
        }
        if(rhs.size_ != 0){
            assign(rhs.begin(), rhs.end());
        }
        else {
            clear();
        }
        return *this;
    }

    /**
     * @brief Function pushes all elements between start and 
     *        end locations onto the vector.
     * @tparam Iterator Type of iterator
     * @param start Iterator representing start of source.
     *        Must be at least a forward iterator. 
     * @param end Iterator representing end of source.
     */
    template <class Iterator> void assign(Iterator start, Iterator end)
    {
        clear();
        while(start != end){
            push_back(*start);
            start++;
        }
    }

    /**
     * @brief Function returns the number of elements in the vector.
     * @return returns the number of elements contained in the vector.
     */
    size_t size() const
    {
        return size_;
    }

    /**
     * @brief Function returns the number of items worth of 
     *        memory allocated for the vector.
     * @return returns the number of items worth of memory
     *        allocated for the vector.
     * @note Capacity will be >=the size of the vector.
     */
    size_t capacity() const
    {
        return capacity_;
    }

    /**
     * @brief Function returns true if the vector is empty.
     * @return returns true only if the vector is empty.
     */
    bool empty() const
    {
        return (size_ == 0); 
    }

    /**
     * @brief Function empties the vector and destructs each element
     *        as it is removed.
     * @note Memory is not freed by clear. Only destruction of 
     *        the vector will deallocate memory.
     * @note Capacity will be >= the size of the vector.
     */
    void clear()
    {
        while(size_ > 0){
            pop_back();
        } 
    }

    /**
     * @brief Function removes and destructs the last element of a 
     *        non-empty vector.
     * @note Capacity will not be changed.
     */
    void pop_back()
    {
        if(size_ > 0) {
            items_[size_ -1].~T();
            size_--;
        }
    }

    /*
     * @brief Function adds a new element at the end of the vector.
     *        If the vector is full, the capacity is doubled before
     *        the element is added.
     * @param item The item to be added to the end of the vector.
     */
    void push_back(const T& item)
    {        
        ptrdiff_t itemSize = reinterpret_cast<char*>(&items_[1]) - reinterpret_cast<char*>(&items_[0]);

        if(capacity_ == 0){
            capacity_ = 1;

            data_ = new char[capacity_ * itemSize];
            items_ = reinterpret_cast<T*>(data_);
        }
        if(size_ == capacity_) {
            capacity_ *= 2;
            T* prevItems( items_ ); ///< Pointer to current buffer of items to be copied to a new location
            char *prevData( data_ ); ///< Point to previous location of data_ char buffer
            data_ = new char[capacity_ * itemSize];
            items_ = reinterpret_cast<T*>(data_);
            for(size_t cpyCount=0; cpyCount < size_; cpyCount++){
                new (&items_[cpyCount]) T(prevItems[cpyCount]);
            }
            // TODO: memory leak in case of exceptions. Make RAII.
            delete[] prevData;
        }
        new(&items_[size_]) T(item);  
        size_++;
    }

    /**
     * @brief Function returns the reference to the last
     *        element in the vector.
     * @return returns the reference to the last element
     *        in the vector.
     * @throw Throw out_of_range exception when the vector is
     *        empty.
     */
    T& back()
    {
        if(size_ <= 0) {
            throw std::out_of_range("vector::back() called on empty hsa::vector.");
        }
        return (items_[size_-1]);
    }

    /**
     * @brief Constant function returns a constant reference to the
     *        last element in the vector.
     * @return returns a constant reference to the last element
     *        in the vector.
     * @throw Throw out_of_range exception when the vector is
     *        empty.
     */
    const T& back() const
    {
        if(size_ <= 0) {
            throw std::out_of_range("vector::back() called on empty hsa::vector.");
        }
        return (items_[size_-1]);
    }

    /**
     * @brief Function returns the reference to the element
     *        at index n in the vector.
     * @return returns the reference to the element at index
     *        n in the vector.
     * @param n The index of the element being returned.
     */
    T& operator[] (size_t n)
    {
        //TODO Assert for out_of_range
        return items_[n];
    }

    /**
     * @brief Constant function returns a constant reference to the
     *        element at index n in the vector.
     * @return returns a constant reference to the element at index
     *        n in the vector.
     * @param n The index of the element being returned.
     */
    const T& operator[] (size_t n) const
    {
        //TODO Assert for out_of_range
        return items_[n];
    }

    /**
     * @brief Function returns the reference to the element 
     *        at index n in the vector
     * @return returns the reference to the element at 
     *        index n in the vector
     * @throw Throw out_of_range exception when index n is 
     *        less than 0 or greater than the vector size.
     * @param n The index of the element being returned. 
     */
    T& at(size_t n) 
    {
        if(n >= size_){
            throw std::out_of_range("vector::at() called on element outside hsa::vector.");
        }
        return items_[n];
    }

    /**
     * @brief Constant function returns a constant reference to the 
     *        element at index n in the vector
     * @return returns a constant reference to the element at 
     *        index n in the vector
     * @throw Throw out_of_range exception when index n is less
     *        than 0 or greater than the vector size.
     * @param n The index of the element being returned.
     */
    const T& at(size_t n) const
    {
        if(n >= size_){
            throw std::out_of_range("vector::at() called on element outside hsa::vector.");
        }
        return items_[n];
    }

    /**
     * @brief Function returns an iterator referring to 
     *        the first element in the vector.
     * @return returns an iterator referring to the first
     *        element in the vector.
     */
    iterator begin()
    {
        return items_;
    }

    /**
     * @brief Constant function returns a constant iterator referring to 
     *        the first element in the vector.
     *  @return returns a constant iterator referring to the first
     *        element in the vector.
     */
    const_iterator begin() const
    {
        return cbegin();
    }

    /**
     * @brief Constant function returns a constant iterator referring to 
     *        the first element in the vector.
     * @return returns a constant iterator referring to the first
     *        element in the vector.
     */
    const_iterator cbegin() const
    {
        return items_;
    }

    /**
     * @brief Function returns an iterator referring to the 
     *        element following the last element in the vector.
     * @return returns an iterator referring to the element 
     *        following the last element in the vector.
     */
    iterator end()
    {
        return items_ + size_;
    }

    /**
     * @brief Constant function returns a constant iterator referring 
     *        to the element following the last element in the
     *        vector.
     * @return returns a constant iterator referring to the element 
     *        following the last element in the vector.
     */
    const_iterator end() const
    {
        return cend();
    }

    /**
     * @brief Constant function returns a constant iterator referring 
     *        to the element following the last element in the
     *        vector.
     * @return returns a constant iterator referring to the element 
     *        following the last element in the vector.
     */
    const_iterator cend() const
    {
        return items_ + size_;
    }

private:
    T*    items_; ///< Pointer to buffer of items of type T, is an alias of data_
    char* data_; ///< Pointer to buffer of items of type char
    size_t    capacity_; ///< Capacity of the vector
    size_t    size_; ///< Number of items contained in the vector
    
};//end Vector

/**
 * @brief Specifies the different timeout periods supported
 * by Hsa Runtime for event wait
 *
 * @note: The values of this enum must match corresponding
 * values defined in HsaThk layer interface.
 */
typedef enum {
    
    /*
     * Specifies Zero milliseconds timeout. The caller
     * will return immediately if the underlying event
     * is not signalled.
     */
    HSA_EVENT_TIMEOUT_IMMEDIATE = 0,

    /*
     * Specifies a blocking call to wait. The caller will
     * return only upon the signalling of the underlying
     * event object.
     */
    HSA_EVENT_TIMEOUT_INFINITE = 0xFFFFFFFF

} HsaEventWaitTime;

/**
 * @brief Specifies the different return values possible from
 * a call to Event Wait.
 */
typedef enum {
    
    /*
     * Indicates return from wait due to timeout i.e.
     * event object did not get signalled.
     */
    HSA_EVENT_WAIT_SUCCESS = 0,

    /*
     * Indicates return from wait due to event object
     * being signalled.
     */
    HSA_EVENT_WAIT_TIMEOUT = 1,

} HsaEventWaitReturn;

/**
*******************************************************************************
* union KernelArgs
*
* @brief This union provides a container for kernel argument types.
*******************************************************************************
*/
union KernelArgs
{
    void* addr;             ///< pointer to a buffer
    int32_t s32value;       ///< signed 32 bit value
    uint32_t u32value;      ///< unsigned 32 bit value
    float fvalue;           ///< float value
    double dvalue;          ///< double value
    int64_t s64value;       ///< signed 64 bit value
    uint64_t u64value;      ///< unsigned 64 bit value
};


/**
 * @brief hsacore ASICInfo class, represents ASIC specific information of a GPU device.
 */
enum ASICType 
{ 
    HSA_SI_TAHITI, 
    HSA_SI_PITCAIRN,
    HSA_CI_SKU1,
    HSA_CI_SKU2
};

class DLL_PUBLIC ASICInfo
{
public:
    /*! Getter function to check for image support.
     * @return bool indicating image support.
    */
    virtual bool isImageSupport()=0;
    /*! Getter function to check for double precision support.
     * @return bool indicating double support.
    */
    virtual bool isDoublePrecision()=0;
    /*! Getter function to check the ASIC Type.
     * @return returns an ASICType enumeration.
    */
    virtual ASICType getASICType()=0;
    virtual ~ASICInfo() {};
};

/* @}*/

/** 
 * @addtogroup HSACoreHSAILUtility 
 * @{
 */
class DLL_PUBLIC HsailKernel
{
    protected:
        char* argListInfo;
    public:
        HsailKernel(){};
	    virtual void* getMetaDataHack()=0;	        
        /*! Getter function for the ISA (Pure Virtual function) 
         *  @param ISA - The starting address of the ISA is 
         *               assigned to this pointer
         *  @param size - the size of the ISA blob is stored here
         *  @return The ISA and the size are stored in the parameters
         */

        virtual void getISA(const void* &isa,
                            size_t &size)=0;
        
        /*! Gets the list of arguments and metadata information
         * associated with each argument - global,local etc
         * @return a pointer to the list of arguments
         */
        virtual char* getArgListInfo()=0;

        /*! Gets the number of arguments
         * @return a pointer to the list of arguments
         */
        virtual int getNumArgs() = 0;

        /*! Getter function for the user elements (Pure Virtual function) 
         *  @param userElements  -  The address of the userELements in the 
         *                          shader is assigned to this pointer
         *
         *  @param noUserElements - The number of user elements is 
         *                          assigned to this parameter
         *
         *  @return -               The userELements and the count are 
         *                          stored in the parameters
         */
         virtual void getUserElements(void* userElements,
                        int& noUserELements) = 0;

         /*! Getter function for the extended user elements
          * (Pure Virtual function) 
          *  @param extUserElements   - The address of the extended 
          *                             userELements in the shader is 
          *                             assigned to this pointer
          *
          *  @param noExtUserElements - The number of extended user elements 
          *                             is assigned to this parameter
          *  @return                  - The extUserELements and the count 
          *                             are stored in the parameters
          */
          virtual void getExtUserElements(void* extUserElements,
                       int& noExtUserELements) = 0;


          /*! Getter function for the extended user elements 
           *  (Pure Virtual function) 
           *  @return returns the number of spgr elements
           */
          virtual int getSGPR() = 0;
          virtual ~HsailKernel(){}
};
/* @}*/

/**
 * @addtogroup HSACoreCommonTyps 
 * Cross-Core-Module common Types
 * @{
 */
typedef enum {

        INVALID_ARG=1,
        OUTOFBOUNDS_ARG=2,
        QUEUE_ERROR=4,
        QUEUE_INVALID=8,
        EVENT_ERROR=16,
        EVENT_INVALID=32,
        ALLOCATION_FAILURE=64,
        PROGRAM_ERROR=128,
        CORE_ERROR=256,
        MEMORY_ERROR=512,
        HSAILPROGRAM_ERROR=1024
	
} HSACoreExceptionType;



class exception:public std::exception
{

protected:
    
    int excepttype_;
    
    std::string info_; ///< place to "append" and carry along an exception

private:

    exception& operator = (const exception&);

public:
    ~exception() throw(){}

    /**
     * @brief append info to an exception, this version doesn't require type
     *
     * @param fname name of the function
     * @param info detail, probably better to keep it under 40 chars 
     */
    virtual void appendInfo(const char *fname,const char *erinfo)
    {
        info_.append("<-");
        info_.append(fname);
        info_.append(":");
        info_.append(erinfo);
    }

    /**
     * @brief append info to an exception, this version requires a type
     *
     * @param fname name of the function
     * @param info detail, probably better to keep it under 40 chars 
     * @param et exception type
     */
    void appendInfo(const char *fname,const char *erinfo, HSACoreExceptionType et)
    {
        info_.append("<-");
        info_.append(fname);
        info_.append(":");
        info_.append(erinfo);
        excepttype_ &= et;
    }

    /**
     * @brief the virtual function in std::exception implemented here
     *
     * @return string in info
     */
    const char* what() const throw()
    {
        const char * c = info_.c_str(); 
        return c;
    }

};

#define ERR_THROW_EX(errVal, errMsg, expVal)                        \
    if (errVal){                                                    \
        exception exObj;                                            \
        exObj.appendInfo(__FUNCTION__,(errMsg),(expVal));           \
        throw (exObj);                                              \
    }

#define AMD_DEVICE_TYPE_UNKNOWN                      0
#define AMD_DEVICE_TYPE_DEFAULT                      (1 << 0)
#define AMD_DEVICE_TYPE_CPU                          (1 << 1)
#define AMD_DEVICE_TYPE_GPU                          (1 << 2)
#define AMD_DEVICE_TYPE_ACCELERATOR                  (1 << 3)
#define AMD_DEVICE_TYPE_HSA                          (1 << 4)
#define AMD_DEVICE_TYPE_ALL                          0xFFFFFFFF

/**
 * @brief MemoryDescriptor class, each descriptor
 * provides interfaces to get different memory properties.
 */
class DLL_PUBLIC MemoryDescriptor
{
public:
    enum MemoryType { HOT_PLUGGABLE, NON_VOLATILE };
    enum HeapType { SYSTEM, PUBLIC, PRIVATE };
    /**
     * @brief Getter function for MemoryType
     * @return returns a MemoryType enumeration.
     */
    virtual MemoryType getMemoryType()=0 ;
    /**
     * @brief Getter function for HeapType
     * @return returns a HeapType enumeration.
     */
    virtual HeapType getHeapType()=0;
    /**
     * @brief Getter function for lower 32 bits of memory size.
     * @return returns the lower 32 bits of memory size. 
     * (example: if memory size = 4GB (2^32), getSizeLow() will 
     *  return 0 since 2^32 = 0x1 00 00 00 00 
     */
    virtual uint32_t getSizeLow()=0;
    /**
     * @brief Getter function for higher 32 bits of memory size.
     * @return returns the higher 32 bits of memory size. 
     * (example: if memory size = 4GB (2^32), getSizeHigh() will 
     *  return 1 since 2^32 = 0x1 00 00 00 00 
     */
    virtual uint32_t getSizeHigh()=0;
    /**
     * @brief Getter function for memory size.
     * @return returns 64 bit integer representing memory size. 
     */
    virtual uint64_t getSize()=0;
    /**
     * @brief Getter function for memory width (the number of parallel bits to
     *        the memory interface)
     * @return returns memory width.
     */
    virtual uint32_t getWidth()=0;
    /**
     * @brief Getter function for maximum memory clock which allows for
     *        computation of available bandwidth.
     * @return returns max memory clock.
     */
    virtual uint32_t getMaxMemoryClock()=0;
    virtual ~MemoryDescriptor() {};
};

/**
 * @brief CacheDescriptor class, each descriptor
 * provides interfaces to get different cache properties.
 */
class DLL_PUBLIC CacheDescriptor
{
public:
    enum CacheType { DATA=1, INSTRUCTION, CPU, SIMD };
     /**
     * @brief Getter function for ProcessorId
     * @return returns a processor number
     */
    virtual uint32_t    getProcessorId()=0;
    /**
     * @brief Getter function to query the cache level.
     * @return returns a number (1,2,3.. etc) representing the
     *         L1,L2,L3... cache levels.
     */
    virtual uint32_t    getCacheLevel()=0;
    /**
     * @brief Getter function for cache size
     * @return returns the size of the cache.
     */
    virtual uint32_t    getCacheSize()=0;
    /**
     * @brief Getter function for cache line size
     * @return returns the size of the cache line in bytes.
     */
    virtual uint32_t    getCacheLineSize()=0;
    /**
     * @brief Getter function for cache lines per tag.
     * @return returns the number of cache lines per tag.
     */
    virtual uint32_t    getCacheLinesPerTag()=0;
    /**
     * @brief Getter function for cache associativity.
     * @return returns the associativity of the cache.
     */
    virtual uint32_t    getCacheAssociativity()=0;
    /**
     * @brief Getter function for cache latency.
     * @return returns the cache latency in nano seconds.
     */
    virtual uint32_t    getCacheLatency()=0;
    /**
     * @brief Getter function for cache type
     * @return returns a CacheType enumeration
     */
    virtual CacheType    getCacheType()=0;
    virtual uint32_t    getSiblingMap()=0;
    virtual ~CacheDescriptor() {};
};

/**
 * @brief IOLinkDescriptor class, each descriptor
 * provides interfaces to get different iolink properties.
 */
class DLL_PUBLIC IOLinkDescriptor
{
public:
    enum IOLinkType { UNDEFINED=0, HYPERTRANSPORT, PCIEXPRESS, AMBA, OTHER };
    /**
     * @brief Getter function for IO Link Type.
     * @return returns a IoLinkType enumeration
     */
    virtual IOLinkType getIoLinkType()=0;
    /**
     * @brief Getter function for IO Bus interface Major version.
     * @return returns the major version of IO bus interface.
     */
    virtual uint32_t getMajorVersion()=0;
    /**
     * @brief Getter function for IO Bus interface Minor version.
     * @return returns the minor version of IO bus interface.
     */
    virtual uint32_t getMinorVersion()=0;
    /**
     * @brief Getter function for source node of an IOLink.
     * @return returns the source node Id of an IOLink.
     */
    virtual uint32_t getFromNode()=0;
    /**
     * @brief Getter function for destination node of an IOLink.
     * @return returns the destination node Id of an IOLink.
     */
    virtual uint32_t getToNode()=0;
     /**
     * @brief Getter function for weight associated with a link.
     * @return returns the weight associated to an IOLink.
     */
    virtual uint32_t getWeight()=0;
     /**
     * @brief Getter function for minimum transfer time.
     * @return returns the minimum cost of time to transfer in nano seconds.
     */
    virtual uint32_t getMinimumLatency()=0;
    /**
     * @brief Getter function for maximum transfer time.
     * @return returns the maximum cost of time to transfer in nano seconds.
     */
    virtual uint32_t getMaximumLatency()=0;
    /**
     * @brief Getter function for minimum IO interface bandwidth.
     * @return returns the minimum interface bandwidth in MB/s
     */
    virtual uint32_t getMinimumBandwidth()=0;
    /**
     * @brief Getter function for maximum IO interface bandwidth.
     * @return returns the maximum interface bandwidth in MB/s
     */
    virtual uint32_t getMaximumBandwidth()=0;
    /**
     * @brief Getter function for recommendedTxfrSize to achieve peak bandwidth.
     * @return returns the recommended transfer size in bytes.
     */
    virtual uint32_t getRecommendedTransferSize()=0;
    virtual ~IOLinkDescriptor() {} ;
};

enum MemoryTypes
{
    SYSTEM_MEMORY =  (1U << 0),
    LOCAL_MEMORY =   (1U << 1),
    LDS_MEMORY   =   (1U << 2),
    GDS_MEMORY   =   (1U << 3),
    TEXTURE_MEMORY = (1U << 4),
    SCRATCH =        (1U << 5)
};

enum SystemMemoryOptions
{
    //page properties
    PAGEABLE =       (1U <<  7),
    PINNED =         (1U <<  8),
    // cache properties
    CACHED =         (1U << 12),
    NON_CACHED =     (1U << 13),
    WRITE_COMBINED = (1U << 14),
    // access properties
    READ_WRITE =     (1U << 17),
    READ_ONLY =      (1U << 18),
};

enum DeviceType 
{ 
    CPU=AMD_DEVICE_TYPE_CPU, 
    GPU=AMD_DEVICE_TYPE_GPU, 
    INVALID=AMD_DEVICE_TYPE_UNKNOWN 
};


}
/* @}*/
#endif
