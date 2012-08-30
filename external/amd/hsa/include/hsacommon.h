//depot/stg/hsa/drivers/hsa/api/common/hsacommon.h#1 - add change 772988 (text)
#ifndef _HSACOMMON_H_
#define _HSACOMMON_H_


#if !defined(AMD_AMP_HSA_INCLUDES)
#include <stdint.h>
#include <vector>
#endif // #if !defined(AMD_AMP_HSA_INCLUDES)

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

namespace hsa
{

typedef hsacore::vector vector;

typedef hsacore::ASICType ASICType;

typedef hsacore::ASICInfo ASICInfo;

typedef hsacore::KernelArgs RTKernelArgs;

}
#endif
