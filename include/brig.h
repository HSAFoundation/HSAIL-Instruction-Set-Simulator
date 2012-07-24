#ifndef INCLUDE_BRIG_H_
#define INCLUDE_BRIG_H_
#include <stdint.h>

// PRM 20.4
typedef uint16_t BrigPacking16_t;
typedef uint16_t BrigDataType16_t;
typedef uint32_t BrigoOffset32_t;
typedef uint32_t BrigStorageClass32_t;
typedef uint32_t BrigOpcode32_t;
typedef uint32_t BrigMemorySemantic32_t;
// 20.9.9
struct BrigInstLdSt {
  uint16_t size;
  uint16_t kind;
  BrigOpcode32_t opcode;
  BrigDataType16_t type;
  BrigPacking16_t packing;
  BrigoOffset32_t o_operands[5];
  BrigStorageClass32_t storageClass;
  BrigMemorySemantic32_t memorySemantic;
  uint32_t equivClass; 
};
#endif //INCLUDE_BRIG_H_



#ifndef _SS_
#define _SS_

#define TRUE 1
#define FALSE 0


typedef unsigned char BOOL;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
//typedef long long uint64_t;
typedef int int32_t;
//Typedefs Used in Brig
//The typedefs in Brig have the C language type (including bit length) followed by the
//Brig type:
typedef uint16_t BrigPacking16_t;
typedef uint16_t BrigDataType16_t;
typedef uint32_t BrigGeom32_t;
typedef uint32_t BrigImageFormat32_t;
typedef uint32_t BrigDirectiveKinds32_t;
typedef uint32_t BrigsOffset32_t;
typedef uint32_t BrigcOffset32_t;
typedef uint32_t BrigdOffset32_t;
typedef uint32_t BrigoOffset32_t;
typedef uint16_t BrigMachine16_t;
typedef uint16_t BrigProfile16_t;
typedef uint16_t BrigSftz16_t;
typedef uint32_t BrigControlType32_t;
typedef uint16_t BrigAttribute16_t;
typedef uint32_t BrigStorageClass32_t;
typedef uint32_t BrigImageOrder32_t;
typedef uint32_t BrigOpcode32_t;
typedef uint32_t BrigAtomicOperation32_t;
typedef uint32_t BrigMemorySemantic32_t;
typedef uint32_t BrigCompareOperation32_t;

//BrigDirectiveKinds
//BrigDirectiveKinds is used to specify the kind of directive.
enum BrigDirectiveKinds {
	BrigEDirectiveFunc,
	BrigEDirectiveKernel,
	BrigEDirectiveSymbol,
	BrigEDirectiveImage,
	BrigEDirectiveSamp,
	BrigEDirectiveLabel,
	BrigEDirectiveLabelList,
	BrigEDirectiveVersion,
	BrigEDirectiveProto,
	BrigEDirectiveFile,
	BrigEDirectiveComment,
	BrigEDirectiveLoc,
	BrigEDirectiveInit,
	BrigEDirectiveLabelInit,
	BrigEDirectiveControl,
	BrigEDirectivePragma,
	BrigEDirectiveExtension,
	BrigEDirectiveArgStart,
	BrigEDirectiveArgEnd,
	BrigEDirectiveBlockStart,
	BrigEDirectiveBlockNumeric,
	BrigEDirectiveBlockString,
	BrigEDirectiveBlockEnd,
	BrigEDirectivePad
};

//BrigMachine
//BrigMachine is used to specify the type of machine model.
enum BrigMachine {
	BrigESmall, // 32-bit model (all addresses are 32 bits;
						// a pointer fits into an s register)
	BrigELarge // 64-bit model (all addresses are 64 bits;
					// a pointer fits into a d register)
};
//BrigProfile
//BrigProfile is used to specify the kind of profile.
enum BrigProfile {
	BrigEFull,
	BrigEReduced
};
//BrigSftz
//BrigSftz is used to specify the use (or non-use) of single-precision flush to zero.
enum BrigSftz {
	BrigESftz,
	BrigENosftz
};

class stringSection {
};

class BrigDirectiveBase {
    public :
        uint16_t size;
        uint16_t kind;
};

class BrigDirectiveList {
    public :
        BrigdOffset32_t offset;
        BrigDirectiveBase* thisDirective;
};

class BrigCodeBase {
    public :
        uint16_t size;
        uint16_t kind;
};
class BrigCodeList {
    public :
        BrigdOffset32_t offset;
        BrigCodeBase* thisCode;
};

class BrigOperandBase {
    public :
        uint16_t size;
        uint16_t kind;
};
class BrigOperandList {
    public :
        BrigdOffset32_t offset;
        BrigOperandBase* thisOperand;
};

class BrigDirectiveVersion : public BrigDirectiveBase {
    public:
        uint16_t size;
        uint16_t kind;
        BrigcOffset32_t c_code;
        uint16_t major;
        uint16_t minor;
        BrigMachine16_t machine;
        BrigProfile16_t profile;
        BrigSftz16_t ftz;
        uint16_t reserved;
        BrigDirectiveVersion(){};
        BrigDirectiveVersion(BrigcOffset32_t c_codeT , uint16_t majorT , uint16_t minorT ,
	                   BrigMachine16_t machineT , BrigProfile16_t profileT , 
					   BrigSftz16_t ftzT ) 
        {
	        size = 20;
        	kind = BrigEDirectiveVersion;
        	c_code = c_codeT;       
            major = majorT;  
        	minor = minorT;         
            machine = machineT;
        	profile = profileT;     
            ftz = ftzT;
        	reserved = 0;

        };
        
};

/*
BrigDirectiveVersion::BrigDirectiveVersion (BrigcOffset32_t c_code , uint16_t major , uint16_t minor ,
	                   BrigMachine16_t machine , BrigProfile16_t profile , 
					   BrigSftz16_t ftz )
{
	version->size = sizeof( BrigDirectiveVersion );
	version->kind = BrigEDirectiveVersion;
	version->c_code = c_code;       
    version->major = major;  
	version->minor = minor;         
    version->machine = machine;
	version->profile = profile;     
    version->ftz = ftz;
	version->reserved = 0;

//	WriteItIntoFile( BDVersion.size , &BDVersion );
	return TRUE; 
} 
*/
#endif
