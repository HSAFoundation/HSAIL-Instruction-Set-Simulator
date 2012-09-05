#ifndef _HSACORECOMMON_H_
#define _HSACORECOMMON_H_

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

#include "hsacommon.h"
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
using namespace hsacommon;

using hsacommon::vector;
using hsacommon::string;

typedef hsa::RTKernelArgs KernelArgs;

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

typedef enum
{

    /**
     * Memory option used for requesting memory with caching enabled. This
     * option is mutually exclusive with the MEMORY_OPTION_UNCACHED option. 
     */
    MEMORY_OPTION_CACHED = (1U << 12),

    /**
     * Memory option used for requesting memory with caching disabled. This
     * option is mutually exclusive with the MEMORY_OPTION_CACHED option. 
     */
    MEMORY_OPTION_UNCACHED = (1U << 13),

    /**
     * Memory option used for requesting memory capable of being paged-out to
     * disk. This option is mutually exclusive with the
     * MEMORY_OPTION_NONPAGEABLE option.
     */
    MEMORY_OPTION_PAGEABLE = (1U <<  7),

    /**
     * Memory option used for requesting memory that won't be paged-out to disk.
     * This option is mutually exclusive with the MEMORY_OPTION_PAGEABLE option. 
     */
    MEMORY_OPTION_NONPAGEABLE = (1U <<  8),

    /**
     * Memory option used for requesting memory that is write combined. 
     */
    MEMORY_OPTION_WRITE_COMBINED = (1U << 14),

    /**
     * Memory option used for declaring that the requested memory will only be
     * accessed by the host. This option is mutually exclusive with the 
     * MEMORY_OPTION_DEVICE_READ_WRITE, MEMORY_OPTION_DEVICE_READ_ONLY, and
     * MEMORY_OPTION_DEVICE_WRITE_ONLY options. 
     */
    MEMORY_OPTION_DEVICE_NO_ACCESS,

    /**
     * Memory option used for declaring that the requested memory will be both
     * read and written by the device. This option is mutually exclusive with
     * the MEMORY_OPTION_DEVICE_NO_ACCESS, MEMORY_OPTION_DEVICE_READ_ONLY, and
     * MEMORY_OPTION_DEVICE_WRITE_ONLY options. 
     */
    MEMORY_OPTION_DEVICE_READ_WRITE = (1U << 17),

    /**
     * Memory option used for declaring that the requested memory will only be
     * both read by the device. This option is mutually exclusive with the
     * MEMORY_OPTION_DEVICE_NO_ACCESS, MEMORY_OPTION_DEVICE_READ_WRITE, and
     * MEMORY_OPTION_DEVICE_WRITE_ONLY options. 
     */
    MEMORY_OPTION_DEVICE_READ_ONLY = (1U << 18),

    /**
     * Memory option used for declaring that the requested memory will only be
     * both written by the device. This option is mutually exclusive with the
     * MEMORY_OPTION_DEVICE_NO_ACCESS, MEMORY_OPTION_DEVICE_READ_WRITE, and
     * MEMORY_OPTION_DEVICE_READ_ONLY options. 
     */
    MEMORY_OPTION_DEVICE_WRITE_ONLY,

    /**
     * Memory option used for declaring that the requested memory will only be
     * accessed by the host. This option is mutually exclusive with the
     * MEMORY_OPTION_HOST_READ_WRITE, MEMORY_OPTION_HOST_READ_ONLY, and
     * MEMORY_OPTION_HOST_WRITE_ONLY options. 
     */
    MEMORY_OPTION_HOST_NO_ACCESS,

    /**
     * Memory option used for declaring that the requested memory will be both
     * read and written by the host. This option is mutually exclusive with the
     * MEMORY_OPTION_HOST_NO_ACCESS, MEMORY_OPTION_HOST_READ_ONLY, and
     * MEMORY_OPTION_HOST_WRITE_ONLY options. 
     */
    MEMORY_OPTION_HOST_READ_WRITE,

    /**
     * Memory option used for declaring that the requested memory will only be
     * both read by the host. This option is mutually exclusive with the
     * MEMORY_OPTION_HOST_NO_ACCESS, MEMORY_OPTION_HOST_READ_WRITE, and
     * MEMORY_OPTION_HOST_WRITE_ONLY options. 
     */
    MEMORY_OPTION_HOST_READ_ONLY,

    /**
     * Memory option used for declaring that the requested memory will only be
     * both written by the host. This option is mutually exclusive with the
     * MEMORY_OPTION_HOST_NO_ACCESS, MEMORY_OPTION_HOST_READ_WRITE, and
     * MEMORY_OPTION_HOST_READ_ONLY options. 
     */
    MEMORY_OPTION_HOST_WRITE_ONLY
} MemoryOption;

}
/* @}*/
#endif
