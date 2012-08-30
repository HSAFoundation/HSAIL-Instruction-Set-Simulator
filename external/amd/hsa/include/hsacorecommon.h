//depot/stg/hsa/drivers/hsa/api/core/common/hsacorecommon.h#3 - edit change 775106 (text)
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
#include <cstdint>
#endif

/** 
 * @addtogroup HSACoreCommonTyps 
 * Cross-Core-Module common Types
 *  @{
 */
namespace hsacore
{

template<typename T>
class vector : public std::vector<T>
{

};


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
    virtual bool isImageSupport()=0;
    virtual bool isDoublePrecision()=0;
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
public:
    ~exception() throw(){}

    /**
     * @brief append info to an exception, this version doesn't require type
     *
     * @param fname name of the function
     * @param info detail, probably better to keep it under 40 chars 
     */
    void appendInfo(const char *fname,const char *erinfo)
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

private:
    exception& operator = (const exception&);
    std::string info_; ///< place to "append" and carry along an exception
    int excepttype_;
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

class DLL_PUBLIC MemoryDescriptor
{
public:
	enum MemoryType { HOT_PLUGGABLE, NON_VOLATILE };
	enum HeapType { SYSTEM, PUBLIC, PRIVATE };
	virtual MemoryType	 getMemoryType()=0 ;
	virtual HeapType	 getHeapType()=0;
	virtual uint32_t getSizeLow()=0;
	virtual uint32_t getSizeHigh()=0;
	virtual uint64_t	 getSize()=0;
	virtual uint32_t getWidth()=0;
	virtual uint32_t getMaxMemoryClock()=0;
    virtual ~MemoryDescriptor() {};
};

class DLL_PUBLIC CacheDescriptor
{
public:
	enum CacheType { DATA=1, INSTRUCTION, CPU, SIMD };
	virtual uint32_t    getProcessorId()=0;
	virtual uint32_t    getCacheLevel()=0;
    virtual uint32_t    getCacheSize()=0;
    virtual uint32_t    getCacheLineSize()=0;
    virtual uint32_t    getCacheLinesPerTag()=0;
    virtual uint32_t    getCacheAssociativity()=0;
    virtual uint32_t    getCacheLatency()=0;
    virtual CacheType    getCacheType()=0;
    virtual uint32_t    getSiblingMap()=0;
    virtual ~CacheDescriptor() {};
};

class DLL_PUBLIC IOLinkDescriptor
{
public:
	enum IOLinkType { UNDEFINED=0, HYPERTRANSPORT, PCIEXPRESS, AMBA, OTHER };
	virtual IOLinkType getIoLinkType()=0;
    virtual uint32_t getMajorVersion()=0;
    virtual uint32_t getMinorVersion()=0;
	virtual uint32_t getFromNode()=0;
    virtual uint32_t getToNode()=0;
	virtual uint32_t getWeight()=0;
	virtual uint32_t getMinimumLatency()=0;
    virtual uint32_t getMaximumLatency()=0;
    virtual uint32_t getMinimumBandwidth()=0;
    virtual uint32_t getMaximumBandwidth()=0;
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
