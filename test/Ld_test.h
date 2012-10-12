#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

/*Template describes the type of the destination, reg/regv2/regv4*/
template <typename T> class Ld_Test : public BrigCodeGenTest{
private:
  
  const BrigInstLdSt* RefInst;
  
  //Width operand
  const BrigOperandImmed* OpWidth;
  
  //Dest Operand
  const T* RefDest;
  //Source operands. Only required structs will be used, the others will stay NULL.
  const BrigOperandReg* RefSrc_Reg;
  const BrigOperandAddress* RefSrc_Addr;
  const BrigOperandIndirect* RefSrc_Indir;
  const BrigOperandCompound* RefSrc_Comp;
  
public:
  //TestCase outputs a BrigOperandAddress only
  Ld_Test(std::string& input, StringBuffer* sbuf, BrigInstLdSt* ref, 
      BrigOperandImmed* width, T* dest, BrigOperandAddress* addr) :
    BrigCodeGenTest(input, sbuf), 
    RefInst(ref),
    OpWidth(width), 
    RefDest(dest), 
    RefSrc_Reg(NULL), 
    RefSrc_Addr(addr), 
    RefSrc_Indir(NULL), 
    RefSrc_Comp(NULL)  { }
      
  //Testcase output is a BrigOperandIndirect    
  Ld_Test(std::string& input, StringBuffer* sbuf, BrigInstLdSt* ref, 
      BrigOperandImmed* width, T* dest, BrigOperandIndirect* indir, BrigOperandReg* reg=NULL) : 
    BrigCodeGenTest(input, sbuf),
    RefInst(ref),
    OpWidth(width),
    RefDest(dest),
    RefSrc_Reg(reg),
    RefSrc_Addr(NULL),
    RefSrc_Indir(indir),
    RefSrc_Comp(NULL)  { }
  
  //TestCase output is a BrigOperandCompound
  Ld_Test(std::string& input, StringBuffer* sbuf, BrigInstLdSt* ref, 
      BrigOperandImmed* width, T* dest, BrigOperandCompound* comp, BrigOperandAddress* addr, BrigOperandReg* reg=NULL) : 
    BrigCodeGenTest(input, sbuf),
    RefInst(ref),
    OpWidth(width),
    RefDest(dest),
    RefSrc_Reg(reg),
    RefSrc_Indir(NULL),
    RefSrc_Comp(comp),
    RefSrc_Addr(addr)  { }
   
  void validate(struct BrigSections* TestOutput){
    
    const char* refbuf = reinterpret_cast<const char *>(&Refbuf->get()[0]);
    const char* getbuf = TestOutput->strings;   
    
    inst_iterator getcode = TestOutput->code_begin();
    const BrigInstLdSt* getinst = (cast<BrigInstLdSt>(getcode));
    validate_brig::validate(RefInst, getinst);
    
   const BrigOperandImmed *getwidth = reinterpret_cast <const BrigOperandImmed *> (&(TestOutput->operands[getinst->o_operands[0]]));
    validate_brig::validate(OpWidth, getwidth);
    
    const T *getdest = reinterpret_cast <const T*> (&(TestOutput->operands[getinst->o_operands[1]]));
    validate_brig::validateOpType<T>(RefDest, refbuf, getdest, getbuf);
    
    if(RefSrc_Indir){
      const BrigOperandIndirect *getsrc_indir = reinterpret_cast <const BrigOperandIndirect*> (&(TestOutput->operands[getinst->o_operands[2]]));
      validate_brig::validate(RefSrc_Indir, getsrc_indir); 
      
      if(RefSrc_Reg){
        const BrigOperandReg *getsrc_reg = reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getsrc_indir->reg]));
        validate_brig::validate(RefSrc_Reg, refbuf, getsrc_reg, getbuf); 
      }             
    } else if(RefSrc_Comp){
      const BrigOperandCompound *getsrc_comp = reinterpret_cast <const BrigOperandCompound*> (&(TestOutput->operands[getinst->o_operands[2]]));
      validate_brig::validate(RefSrc_Comp, getsrc_comp); 
      
      const BrigOperandAddress *getsrc_addr = reinterpret_cast <const BrigOperandAddress*> (&(TestOutput->operands[getsrc_comp->name]));
      validate_brig::validate(RefSrc_Addr, getsrc_addr);      
      
      if(RefSrc_Reg){
        const BrigOperandReg *getsrc_reg = reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getsrc_comp->reg]));
        validate_brig::validate(RefSrc_Reg, refbuf, getsrc_reg, getbuf); 
      }    
      
    } else {
      const BrigOperandAddress *getsrc_addr = reinterpret_cast <const BrigOperandAddress*> (&(TestOutput->operands[getinst->o_operands[2]]));
      validate_brig::validate(RefSrc_Addr, getsrc_addr);      
    }
  }
};

TEST(CodegenTest, Ld_Codegen){

/*********************Common variables**********************/
  std::string in, op1, op2; 
  StringBuffer* sbuf = new StringBuffer();

  /*****************************************************************/
  in.assign( "ld_arg_f32 $s0, [%input];\n");
  op1.assign("$s0"); op2.assign("%input");
  sbuf->append(op1); sbuf->append(op2);
    
  BrigOperandReg dest1 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  dest1.size = sizeof(dest1);  

  BrigOperandImmed width1 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}  
  }; 
  width1.size = sizeof(width1);
  
  BrigOperandAddress addr1 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0  //Offset to .directives, However, no directive in .directives. Ambiguous testing.
  };
  addr1.size = sizeof(addr1);
    
  BrigInstLdSt out1 = {
    0,  // size
    BrigEInstLdSt,         // kind
    BrigLd,                // opcode
    Brigf32,               // type
    BrigNoPacking,         // packing
    {0, sizeof(width1), sizeof(width1) + sizeof(dest1), 0, 0},  // operand[5]
    BrigArgSpace,          // storageClass
    BrigRegular,           // memorySemantic
    0                      // equivClass
  };
  out1.size = sizeof(out1);
    
  Ld_Test<BrigOperandReg> TestCase1(in, sbuf, &out1, &width1, &dest1, &addr1);
  TestCase1.Run_Test(&Ld);  
  sbuf->clear();
  
/**********************************************************************************/
  in.assign( "ld_arg_f32 $s0, [%an_output];\n");
  op1.assign("$s0"); op2.assign("%an_output");
  sbuf->append(op1); sbuf->append(op2);
    
  BrigOperandReg dest2 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  dest2.size = sizeof(dest2);
  
  BrigOperandImmed width2 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}  
  }; 
  width2.size = sizeof(width2);
  
  BrigOperandAddress addr2 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0  //Offset to .directives, However, no directive in .directives. Ambiguous testing.
  };
  addr2.size = sizeof(addr2);
    
  BrigInstLdSt out2 = {
    0,  // size
    BrigEInstLdSt,         // kind
    BrigLd,                // opcode
    Brigf32,               // type
    BrigNoPacking,         // packing
    {0, sizeof(width2), sizeof(width2) + sizeof(dest2), 0, 0},  // operand[5]
    BrigArgSpace,          // storageClass
    BrigRegular,           // memorySemantic
    0                      // equivClass
  };
  out2.size = sizeof(out2);
    
  Ld_Test<BrigOperandReg> TestCase2(in, sbuf, &out2, &width2, &dest2, &addr2);
  TestCase2.Run_Test(&Ld);  
  sbuf->clear();


}

}
}
