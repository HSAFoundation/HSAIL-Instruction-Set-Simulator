#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

template <typename T> class Instruction1_Test : public BrigCodeGenTest{
private:
 
  //Instruction in .code buffer - Pointers to brig structures
  const BrigInstBase* RefInst;
  //Operands in .operands buffer
  const T* RefDest;
    
public:
 
  Instruction1_Test(std::string& in, StringBuffer *sbuf, BrigInstBase* ref, T* Dest) : 
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),
    RefDest(Dest)  { }
    
  void validate(struct BrigSections* TestOutput){
  
    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;   
    
    inst_iterator getcode = TestOutput->code_begin();
    const BrigInstBase* getinst = (cast<BrigInstBase>(getcode));
    validate_brig::validate(RefInst, getinst);
    
    const T *getdest = reinterpret_cast <const T*> (&(TestOutput->operands[getinst->o_operands[0]]));
    validate_brig::validateOpType<T>(RefDest, refbuf, getdest, getbuf);
      
    EXPECT_EQ(0, getinst->o_operands[1]);    
    EXPECT_EQ(0, getinst->o_operands[2]);    
    EXPECT_EQ(0, getinst->o_operands[3]);
    EXPECT_EQ(0, getinst->o_operands[4]);       
  }
};

}
}