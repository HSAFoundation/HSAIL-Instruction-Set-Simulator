//depot/stg/hsa/drivers/hsa/api/common/hsacommon.h#4 - edit change 783545 (text)
#ifndef _HSACOMMON_H_
#define _HSACOMMON_H_


#if !defined(AMD_AMP_HSA_INCLUDES)
#include <stdint.h>
#include <vector>
#endif // #if !defined(AMD_AMP_HSA_INCLUDES)
#include "hsacorecommon.h"

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

//////////////////////////////////////////////////////////////////
/////////////THESE MUST BE DELETED ONCE REMOVED FROM CODE/////////
#define AMDRC_SUCCESS          0	
#define AMDRC_NOT_INITIALIZED -2
#define AMDRC_NOT_IMPLEMENTED -3
#define AMDRC_NULL_ARG        -4
#define AMDRC_DEVICE_ERROR    -8
#define AMDRC_EVENT_ERROR     -9
#define AMDRC_QUEUE_ERROR     -10
#define AMD_DEVCAP_DBL_PRECISION  (1 << 0)
#define AMDFAILED(rc) (((int)(rc)) != 0)
#define AMDSUCCEDED(rc) (((int)(rc)) == 0)
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

namespace hsa
{

using hsacore::vector;
//class vector: public hsacore::vector<T> { };	
using hsacore::TEXTURE_MEMORY;
using hsacore::SYSTEM_MEMORY;
using hsacore::LOCAL_MEMORY;
using hsacore::LDS_MEMORY;
using hsacore::GDS_MEMORY;
using hsacore::SCRATCH;

/**
 * @brief Maps Hsacore Event Wait timeouts to corresponding
 * Hsa Runtime construct.
 */
typedef hsacore::HsaEventWaitTime HsaEventWaitTime;

/**
 * @brief Maps Hsacore Event Wait return codes to corresponding
 * Hsa Runtime construct.
 */
typedef hsacore::HsaEventWaitReturn HsaEventWaitReturn;

typedef hsacore::ASICType ASICType;

typedef hsacore::ASICInfo ASICInfo;

typedef hsacore::KernelArgs RTKernelArgs;

typedef hsacore::DeviceType DeviceType;
using hsacore::CPU;
using hsacore::GPU;
using hsacore::INVALID;

typedef hsacore::MemoryDescriptor MemoryDescriptor;

typedef hsacore::CacheDescriptor CacheDescriptor;

typedef hsacore::IOLinkDescriptor IOLinkDescriptor;

typedef hsacore::MemoryTypes MemoryTypes;

typedef hsacore::KernelArgs RTKernelArg;

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
} HSARTExceptionType;

class exception : public hsacore::exception {
public:
    void appendInfo(const char *fname,const char *erinfo, HSARTExceptionType et)
    {
        info_.append("<-");
        info_.append(fname);
        info_.append(":");
        info_.append(erinfo);
        excepttype_ &= et;
    }
};

#define ERR_THROW_EX(errVal, errMsg, expVal)                        \
    if (errVal){                                                    \
        exception exObj;                                            \
        exObj.appendInfo(__FUNCTION__,(errMsg),(expVal));           \
        throw (exObj);                                              \
    }

//////////////////////////////////////////////////
/////////////////TO BE DELETED////////////////////
typedef int AMDRETCODE;

}
#endif
