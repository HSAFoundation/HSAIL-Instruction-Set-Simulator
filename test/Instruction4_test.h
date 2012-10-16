#include <iostream>
#include <string>
#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

template <typename T, typename T1, typename T2, typename T3, typename T4> class Instruction4Opcode_Test : public BrigCodeGenTest{
private:
  
  //Instruction in .code buffer
  const T* RefInst;
  //Operands in .operands buffer
  const T1* RefDest;
  const T2* RefSrc1;
  const T3* RefSrc2;
  const T4* RefSrc3;
     
public:
  Instruction4Opcode_Test(std::string& in, StringBuffer* sbuf, T* ref, T1* Dest, T2* Src1, T3* Src2, T4* Src3) : 
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),
    RefDest(Dest),
    RefSrc1(Src1),
    RefSrc2(Src2), 
    RefSrc3(Src3)  { }
 
  void validate(struct BrigSections* TestOutput){
  
    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;   
    
    inst_iterator getcode = TestOutput->code_begin();
    const T* getinst = (cast<T>(getcode));
    validate_brig::validate(RefInst, getinst);
    
    const T1 *getdest = reinterpret_cast <const T1*> (&(TestOutput->operands[getinst->o_operands[0]]));
    validate_brig::validateOpType<T1>(RefDest, refbuf, getdest, getbuf);
        
    const T2 *getsrc1 = reinterpret_cast <const T2*> (&(TestOutput->operands[getinst->o_operands[1]]));
    validate_brig::validateOpType<T2>(RefSrc1, refbuf, getsrc1, getbuf);
    
    const T3 *getsrc2 = reinterpret_cast <const T3*> (&(TestOutput->operands[getinst->o_operands[2]]));
    validate_brig::validateOpType<T3>(RefSrc2, refbuf, getsrc2, getbuf);
    
    const T4 *getsrc3 = reinterpret_cast <const T4*> (&(TestOutput->operands[getinst->o_operands[3]]));
    validate_brig::validateOpType<T4>(RefSrc3, refbuf, getsrc3, getbuf);
    
    EXPECT_EQ(0, getinst->o_operands[4]);       
  }
 };
}
} 