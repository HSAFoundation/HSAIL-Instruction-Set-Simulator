#include "codegen_validate.h"
#include "codegen_test.h"
 
namespace hsa {
namespace brig {
 
template <typename Tinst, typename T1, typename T2 = BrigOperandReg, typename T3 = BrigOperandReg>
class Branch_Test : public BrigCodeGenTest{
private:
  
  //Instruction in .code buffer - Pointers to brig structures
  const Tinst* RefInst;
   
  //Operands
  const BrigOperandImmed* RefWidth;
  const T1* RefSrc1;
  const T2* RefSrc2;
  const T3* RefSrc3;

public:
  
  Branch_Test(std::string& in, StringBuffer* sbuf, Tinst* ref, 
              BrigOperandImmed* width, T1* src1, T2* src2 = NULL, T3* src3 = NULL) : 
    BrigCodeGenTest(in, sbuf),
    RefInst(ref), 
    RefWidth(width),
    RefSrc1(src1),
    RefSrc2(src2),
    RefSrc3(src3) { }
    
  void validate(struct BrigSections* TestOutput){   

    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;
    uint32_t opCount = 0;
    inst_iterator getcode = TestOutput->code_begin();   
    const Tinst* getinst = (cast<Tinst>(getcode));

    validate_brig::validate(RefInst, getinst);

    const BrigOperandImmed *getwidth = reinterpret_cast <const BrigOperandImmed *> 
                                       (&(TestOutput->operands[getinst->o_operands[opCount++]]));
    validate_brig::validate(RefWidth, getwidth);

    const T1* getsrc1 = reinterpret_cast <const T1*> 
                        (&(TestOutput->operands[getinst->o_operands[opCount++]]));
    validate_brig::validateOpType<T1>(RefSrc1, refbuf, getsrc1, getbuf);

    if (RefSrc2 != NULL) {
      const T2 *getsrc2 = reinterpret_cast <const T2*> 
                         (&(TestOutput->operands[getinst->o_operands[opCount++]]));
      validate_brig::validate(RefSrc2, refbuf, getsrc2, getbuf);
    }

    if (RefSrc3 != NULL) {
      const T3 *getsrc3 = reinterpret_cast <const T3*> 
                         (&(TestOutput->operands[getinst->o_operands[opCount++]]));
      validate_brig::validate(RefSrc3, refbuf, getsrc3, getbuf);
    }
    
    while (opCount <= 4) {
      EXPECT_EQ(0, getinst->o_operands[opCount++]);       
    }       
     
  }
};

TEST(CodegenTest, Brn_Codegen){

  /********************* Common variables **********************/
  std::string in, op1, op2; 
  StringBuffer* sbuf = new StringBuffer();
  BrigOperandImmed width;
  BrigOperandLabelRef labRef;
  BrigInstBase outInst;
  BrigInstMod outMod;

  /********************* Case 1 ********************************/
  in.assign( "brn @loop;\n");

  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 0;

  labRef.size = sizeof(labRef);
  labRef.kind = BrigEOperandLabelRef;
  labRef.labeldirective = 0;


  outInst.size = sizeof(outInst);
  outInst.kind = BrigEInstBase;
  outInst.opcode = BrigBrn;
  outInst.type = Brigb32;
  outInst.packing = BrigNoPacking;
  outInst.o_operands[0] = 0;
  outInst.o_operands[1] = sizeof(width);
  outInst.o_operands[2] = 0;
  outInst.o_operands[3] = 0;
  outInst.o_operands[4] = 0;

  Branch_Test<BrigInstBase, BrigOperandLabelRef> TestCase1(in, sbuf, &outInst, &width, &labRef);
  TestCase1.Run_Test(&Branch);  
  sbuf->clear();

  /********************* Case 2 ********************************/
  in.assign( "brn_width(all)_fbar @label;\n");
  outMod.size = sizeof(outMod);
  outMod.kind = BrigEInstMod;
  outMod.opcode = BrigBrn;
  outMod.type = Brigb32;
  outMod.packing = BrigNoPacking;
  outMod.o_operands[0] = 0;
  outMod.o_operands[1] = sizeof(width);
  outMod.o_operands[2] = 0;
  outMod.o_operands[3] = 0;
  outMod.o_operands[4] = 0;
  memset(&outMod.aluModifier, 0, sizeof(outMod.aluModifier));
  outMod.aluModifier.fbar = 1;

  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 0;

  labRef.size = sizeof(labRef);
  labRef.kind = BrigEOperandLabelRef;
  labRef.labeldirective = 0;
 
  Branch_Test<BrigInstMod, BrigOperandLabelRef> TestCase2(in, sbuf, &outMod, &width, &labRef);
  TestCase2.Run_Test(&Branch);  
  sbuf->clear();

  /******************************  End of tests *****************************************/
  delete sbuf;
}
 
}  // namespace hsa 
}  // namespace brig 

