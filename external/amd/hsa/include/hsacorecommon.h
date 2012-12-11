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

enum ASICFamily
{
    ASIC_FAMILY_SI,
    ASIC_FAMILY_CI
};

/**
 * @brief hsacommon ASICInfo class, represents ASIC specific information of a GPU device.
 */
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
    /*! Getter function to check the ASIC Family.
     * @return returns an ASICFamily enumeration.
    */
    virtual ASICFamily getASICFamily()=0;
    /*! Getter function to get the max queue size in bytes.
     * @return returns maximum queue size that can be created in bytes.
    */
    virtual uint32_t getMaxQueueSize()=0;
    virtual ~ASICInfo() {};
};

// TODO: Replace out_of_range with an HSART exception
using namespace hsacommon;

//using hsacommon::vector;
//using hsacommon::string;

//typedef hsa::KernelArg KernelArg;

/* @}*/

/** 
 * @addtogroup HSACoreHSAILUtility 
 * @{
 */
class DLL_PUBLIC HsailKernel
{
 
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
         virtual void getUserElements(void* &userElements,
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
          virtual void getExtUserElements(void* &extUserElements,
                       int& noExtUserELements) = 0;


          /*! Getter function for the extended user elements 
           *  (Pure Virtual function) 
           *  @return returns the number of spgr elements
           */
          virtual int getNoSGPRs() = 0;

           /*! Getter function for the number of vgpr 
            *  @return returns the number of vpgrs
           */
          virtual int getNoVGPRs() = 0;

          /* @brief - Reserves VGPRS for the debugger
          *   @param noOfRegisters - Number of registers to reserve
          *   @returns - 1 on success or 0 on overflow
          */
          virtual bool reserveVGPRs(int noOfRegisters) = 0;


          /* @brief - Returns the first index of the registers 
          *  reserved for the debugger
          *  return - Returns the index of type uint32_t
          */
          virtual int getTrapReservedVGPRIndex() = 0;

          /* @brief - Returns the index of the register that 
          * contains the Buffer Wave offset
          * return - Returns the index of type uint32_t
          * returns -1 if value is scratch buffer is not set
          */
          virtual int getScratchBufferWaveOffsetSGPRIndex() = 0;

          /* @brief - Returns the index of the registers that 
          * contains work group ID, and the number of registers
          * that are actually used to hold the info
          */
          virtual int getWorkGroupSGPRIndex(bool en[3]) = 0;

		  /* @brief - Returns a pointer to the DWARF info (BRIG to ISA)
          * return - Returns the index of type uint32_t
          */
          virtual void* getDebugISASourceInfo() = 0;

          /* @brief - Returns a pointer to the DWARF info 
          * (Higher level language to BRIG)
          * return - Returns the index of type uint32_t
          */
          virtual void* getDebugBRIGSourceInfo() = 0;

          /* 
           * @brief - Returns the size (in bytes) of group memory statically 
           *          allocated by compiler.
           * @return Size (in bytes) of compiler's group memory allocation.
           */
          virtual size_t 
          getSizeStaticGroupMemory() const = 0;

          virtual ~HsailKernel(){};
};
/* @}*/

}
/* @}*/
#endif
