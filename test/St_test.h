#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

/*Template describes the type of the destination, reg/regv2/regv4 or immediate value*/
template <typename T> class St_Test : public BrigCodeGenTest{
private:
     
  const BrigInstLdSt* RefInst;
   
  //source Operand - reg or immed
  const T* RefSrc; 
  //destination operands. Only required structs will be used, the others will stay NULL.
  const BrigOperandReg* RefDest_Reg;
  const BrigOperandAddress* RefDest_Addr;
  const BrigOperandIndirect* RefDest_Indir;
  const BrigOperandCompound* RefDest_Comp;
  
public:
  //TestCase outputs a BrigOperandAddress only
  St_Test(std::string& input, StringBuffer* sbuf, BrigInstLdSt* ref, 
      T* src, BrigOperandAddress* addr) :
    BrigCodeGenTest(input, sbuf), 
    RefInst(ref),
    RefSrc(src), 
    RefDest_Reg(NULL), 
    RefDest_Addr(addr), 
    RefDest_Indir(NULL), 
    RefDest_Comp(NULL)  { }
      
  //Testcase output is a BrigOperandIndirect    
  St_Test(std::string& input, StringBuffer* sbuf, BrigInstLdSt* ref, 
      T* src, BrigOperandIndirect* indir, BrigOperandReg* reg=NULL) : 
    BrigCodeGenTest(input, sbuf),
    RefInst(ref),
    RefSrc(src),
    RefDest_Reg(reg),
    RefDest_Addr(NULL),
    RefDest_Indir(indir),
    RefDest_Comp(NULL)  { }
  
  //TestCase output is a BrigOperandCompound
  St_Test(std::string& input, StringBuffer* sbuf, BrigInstLdSt* ref, 
     T* src, BrigOperandCompound* comp, BrigOperandAddress* addr, BrigOperandReg* reg=NULL) : 
    BrigCodeGenTest(input, sbuf),
    RefInst(ref),
    RefSrc(src),
    RefDest_Reg(reg),
    RefDest_Addr(addr),  
    RefDest_Indir(NULL),
    RefDest_Comp(comp)   { }
   
  void validate(struct BrigSections* TestOutput){
    
    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;   
    
    inst_iterator getcode = TestOutput->code_begin();
    const BrigInstLdSt* getinst = (cast<BrigInstLdSt>(getcode));
    validate_brig::validate(RefInst, getinst);
        
    const T *getsrc = reinterpret_cast <const T*> (&(TestOutput->operands[getinst->o_operands[0]]));
    validate_brig::validateOpType<T>(RefSrc, refbuf, getsrc, getbuf);
    
    if(RefDest_Indir){
      const BrigOperandIndirect *getdest_indir = reinterpret_cast <const BrigOperandIndirect*> (&(TestOutput->operands[getinst->o_operands[1]]));
      validate_brig::validate(RefDest_Indir, getdest_indir); 
      
      if(RefDest_Reg){
        const BrigOperandReg *getdest_reg = reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getdest_indir->reg]));
        validate_brig::validate(RefDest_Reg, refbuf, getdest_reg, getbuf);
      }         
             
    } else if(RefDest_Comp){
      
      const BrigOperandCompound *getdest_comp = reinterpret_cast <const BrigOperandCompound*> (&(TestOutput->operands[getinst->o_operands[1]]));
      validate_brig::validate(RefDest_Comp, getdest_comp); 
            
      const BrigOperandAddress *getdest_addr = reinterpret_cast <const BrigOperandAddress*> (&(TestOutput->operands[getdest_comp->name]));
      validate_brig::validate(RefDest_Addr, getdest_addr);         
      
      if(RefDest_Reg){
        const BrigOperandReg *getdest_reg = reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getdest_comp->reg]));
        validate_brig::validate(RefDest_Reg, refbuf, getdest_reg, getbuf); 
      } 
            
    } else {
      const BrigOperandAddress *getdest_addr = reinterpret_cast <const BrigOperandAddress*> (&(TestOutput->operands[getinst->o_operands[1]]));
      validate_brig::validate(RefDest_Addr, getdest_addr);
    }
  }
};

TEST(CodegenTest, St_Codegen){

  /***********************************Common variables used by all tests**************************************/
  std::string in, op1, op2, op3; 
  StringBuffer* sbuf = new StringBuffer();
  
  /*********************************** Test Case 1************************************************************/
  
  in.assign( "st_arg_f32 $s0, [%output][$s2-4];\n");
  op1.assign("$s0"); op2.assign("%output"); op3.assign("$s2");
  sbuf->append(op1); sbuf->append(op2); sbuf->append(op3);
  
  //Reference to $s0
  BrigOperandReg src = {
  sizeof(BrigOperandReg),
  BrigEOperandReg,
  Brigb32,
  0, 
  0  //Offset to string table
  };
  
  //ref to %output - Not added in symbol table; it is assumed that it was added during variable decl
  BrigOperandAddress dest1 = {
  sizeof(BrigOperandAddress),
  BrigEOperandAddress,
  Brigb64,
  0,
  0  //Offset to .directives, However, no directive in .directives. Ambiguous testing.
  };
  
  //Ref to $s2
  BrigOperandReg dest2 = {
  sizeof(BrigOperandReg),
  BrigEOperandReg,
  Brigb32,
  0,
  op1.size() + op2.size() +2, 
  };
  
  BrigOperandCompound comp = {
  sizeof(BrigOperandCompound),
  BrigEOperandCompound,
  Brigb64,
  0,
  sizeof(src),//byte offset to operand addr
  sizeof(src) + sizeof(dest1), //to operand reg
  -4  //Offset in the included test case
  };  
   
  BrigInstLdSt out = {
    sizeof(BrigInstLdSt),                // size
    BrigEInstLdSt,     // kind
    BrigSt,            // opcode
    Brigf32,           // type
    BrigNoPacking,     // packing
    {0, sizeof(src) + sizeof(dest1) + sizeof(dest2), 0, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
    
  St_Test<BrigOperandReg> TestCase1(in, sbuf, &out, &src, &comp, &dest1, &dest2);
  TestCase1.Run_Test(&St);   
  sbuf->clear();
  
/********************************Add More Unit tests **********************************/


/************************************ End of tests*******************************************/
  delete sbuf;
}

}
}
