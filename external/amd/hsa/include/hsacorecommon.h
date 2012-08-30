//depot/stg/hsa/drivers/hsa/api/core/common/hsacorecommon.h#2 - edit change 773172 (text)
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
	
} HsaExceptionType;


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
	
} HsaExceptionType;



class HsaException:public std::exception
{
public:
    ~HsaException() throw(){}

    /**
     * @brief append info to an exception, this version doesn't require type
     *
     * @param fname name of the function
     * @param info detail, probably better to keep it under 40 chars 
     */
    void appendInfo(const char *fname,const char *info);

    /**
     * @brief append info to an exception, this version requires a type
     *
     * @param fname name of the function
     * @param info detail, probably better to keep it under 40 chars 
     * @param et exception type
     */
    void appendInfo(const char *fname,const char *info, HsaExceptionType et);

    /**
     * @brief the virtual function in std::exception implemented here
     *
     * @return string in info
     */
    const char* what() const throw();

private:
    //HsaException(const HsaException&);     
    HsaException& operator = (const HsaException&);
    //HsaExceptionType ty;
    std::string info_; ///< place to "append" and carry along an exception
    int excepttype_;
};

void 
HsaException::appendInfo(const char *fname, const char *erinfo)
{
    info_.append("<-");
    info_.append(fname);
    info_.append(":");
    info_.append(erinfo);
}

void
HsaException::appendInfo(const char *fname, const char *erinfo, HsaExceptionType et)
{
    info_.append("<-");
    info_.append(fname);
    info_.append(":");
    info_.append(erinfo);
    excepttype_ &= et;
}

const char* 
HsaException::what() const throw()
{
    const char * c = info_.c_str(); 
    return c;
}
}
#endif
