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
          virtual int getSGPR() = 0;

           /*! Getter function for the number of vgpr 
            *  @return returns the number of vpgrs
           */
          
          int getVGPR();

          virtual ~HsailKernel(){}
};
/* @}*/

}
/* @}*/
#endif
