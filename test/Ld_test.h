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
    RefSrc_Addr(addr),
    RefSrc_Indir(NULL),
    RefSrc_Comp(comp) { }
   
  void validate(struct BrigSections* TestOutput){
    
    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;   
    
    inst_iterator getcode = TestOutput->code_begin();
    const BrigInstLdSt* getinst = (cast<BrigInstLdSt>(getcode));
    validate_brig::validate(RefInst, getinst);
    
   const BrigOperandImmed *getwidth = reinterpret_cast <const BrigOperandImmed *> (&(TestOutput->operands[getinst->o_operands[0]]));
    validate_brig::validate(OpWidth, getwidth);
    
    const T *getdest = reinterpret_cast <const T*> (&(TestOutput->operands[getinst->o_operands[1]]));
    validate_brig::validateOpType<T>(RefDest, refbuf, getdest, getbuf);
    
    const BrigOperandBase *getsrc_op2 = reinterpret_cast <const BrigOperandBase*> (&(TestOutput->operands[getinst->o_operands[2]]));
    
    if(getsrc_op2->kind==BrigEOperandIndirect){
      const BrigOperandIndirect *getsrc_indir = reinterpret_cast <const BrigOperandIndirect*> (getsrc_op2);
      validate_brig::validate(RefSrc_Indir, getsrc_indir); 
      
      if(getsrc_indir->reg){
        const BrigOperandReg *getsrc_reg = reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getsrc_indir->reg]));
        validate_brig::validate(RefSrc_Reg, refbuf, getsrc_reg, getbuf); 
      }             
    } else if(getsrc_op2->kind==BrigEOperandCompound){
      const BrigOperandCompound *getsrc_comp = reinterpret_cast <const BrigOperandCompound*> (getsrc_op2);
      validate_brig::validate(RefSrc_Comp, getsrc_comp); 
      
      const BrigOperandAddress *getsrc_addr = reinterpret_cast <const BrigOperandAddress*> (&(TestOutput->operands[getsrc_comp->name]));
      validate_brig::validate(RefSrc_Addr, getsrc_addr);      
      
      if(getsrc_comp->reg){
        const BrigOperandReg *getsrc_reg = reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getsrc_comp->reg]));
        validate_brig::validate(RefSrc_Reg, refbuf, getsrc_reg, getbuf); 
      }    
      
    } else {
      const BrigOperandAddress *getsrc_addr = reinterpret_cast <const BrigOperandAddress*> (&(TestOutput->operands[getinst->o_operands[2]]));
      validate_brig::validate(RefSrc_Addr, getsrc_addr);      
    }
    EXPECT_EQ(0, getinst->o_operands[3]);
    EXPECT_EQ(0, getinst->o_operands[4]);
  }
};
/*********************** Ld Test ***************************/
TEST(CodegenTest, Ld_Codegen){

/*********************Common variables**********************/
  std::string in, op1, op2; 
  StringBuffer* sbuf = new StringBuffer();

  /*****************************************************************/
  in.assign( "ld_arg_f32 $s0, [%input];\n");
  op1.assign("$s0"); op2.assign("");
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
    0  
  };
  addr1.size = sizeof(addr1);
    
  BrigInstLdSt out1 = {
    0,                     
    BrigEInstLdSt,         
    BrigLd,                
    Brigf32,               
    BrigNoPacking,         
    {0, sizeof(width1), sizeof(width1) + sizeof(dest1), 0, 0},  
    BrigArgSpace,          
    BrigRegular,           
    0                     
  };
  out1.size = sizeof(out1);
    
  Ld_Test<BrigOperandReg> TestCase1(in, sbuf, &out1, &width1, &dest1, &addr1);
  TestCase1.Run_Test(&Ld);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "ld_group_f16 $d5, [100];\n");
  op1.assign("$d5"); op2.assign("");
  sbuf->append(op1); sbuf->append(op2);
    
  BrigOperandReg dest2 = {
    0,
    BrigEOperandReg,
    Brigb64,
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
   
  BrigOperandIndirect indir2 = {
    0,
    BrigEOperandIndirect,
    0,
    Brigb64,
    0,
    100
  };
  indir2.size = sizeof(indir2);

  BrigInstLdSt out2 = {
    0,  
    BrigEInstLdSt,         
    BrigLd,                
    Brigf16,               
    BrigNoPacking,         
    {0, sizeof(width2), sizeof(width2) + sizeof(dest2), 0, 0},  
    BrigGroupSpace,        
    BrigRegular,           
    0                      
  };
  out2.size = sizeof(out2); 
 
  Ld_Test<BrigOperandReg> TestCase2(in, sbuf, &out2, &width2, &dest2, &indir2, NULL);
  TestCase2.Run_Test(&Ld);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "ld_f32 $s1, [$d0];\n");
  op1.assign("$s1"); op2.assign("$d0");
  sbuf->append(op1); sbuf->append(op2);
    
  BrigOperandReg dest3 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  dest3.size = sizeof(dest3);

  BrigOperandImmed width3 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}  
  }; 
  width3.size = sizeof(width3);
   
  BrigOperandIndirect indir3 = {
    0,
    BrigEOperandIndirect,
    sizeof(width3) + sizeof(dest3),
    Brigb64,
    0,
    0
  };
  indir3.size = sizeof(indir3);

  BrigOperandReg reg3 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0, 
    op1.size() + 1
  };
  reg3.size = sizeof(reg3);

  BrigInstLdSt out3 = {
    0,                     
    BrigEInstLdSt,         
    BrigLd,                
    Brigf32,               
    BrigNoPacking,         
    {0, sizeof(width3), sizeof(width3) + sizeof(dest3), 0, 0},  
    BrigFlatSpace,         
    BrigRegular,           
    0                      
  };
  out3.size = sizeof(out3); 
 
  Ld_Test<BrigOperandReg> TestCase3(in, sbuf, &out3, &width3, &dest3, &indir3, &reg3);
  TestCase3.Run_Test(&Ld);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "ld_global_dep_u32 $s1, [&x];\n");
  op1.assign("$s1"); op2.assign("");
  sbuf->append(op1); sbuf->append(op2);
    
  BrigOperandReg dest4 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  dest4.size = sizeof(dest4);

  BrigOperandImmed width4 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}  
  }; 
  width4.size = sizeof(width4);
   
  BrigOperandAddress addr4 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0  
  };
  addr4.size = sizeof(addr4);
 
  BrigInstLdSt out4 = {
    0,                     
    BrigEInstLdSt,         
    BrigLd,                
    Brigu32,               
    BrigNoPacking,         
    {0, sizeof(width4), sizeof(width4) + sizeof(dest4), 0, 0},  
    BrigGlobalSpace,       
    BrigDep,               
    0                      
  };
  out4.size = sizeof(out4); 
 
  Ld_Test<BrigOperandReg> TestCase4(in, sbuf, &out4, &width4, &dest4, &addr4);
  TestCase4.Run_Test(&Ld);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "ld_width(64)_global_f16 $d1, [&x];\n");
  op1.assign("$d1"); op2.assign("");
  sbuf->append(op1); sbuf->append(op2);
    
  BrigOperandReg dest5 = {
    0,
    BrigEOperandReg,
    Brigb64,
    0, 
    0
  };
  dest5.size = sizeof(dest5);

  BrigOperandImmed width5 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {64}  
  }; 
  width5.size = sizeof(width5);
   
  BrigOperandAddress addr5 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0  
  };
  addr5.size = sizeof(addr5);
 
  BrigInstLdSt out5 = {
    0,                     
    BrigEInstLdSt,         
    BrigLd,                
    Brigf16,               
    BrigNoPacking,         
    {0, sizeof(width5), sizeof(width5) + sizeof(dest5), 0, 0},  
    BrigGlobalSpace,       
    BrigRegular,               
    0                      
  };
  out5.size = sizeof(out5); 
 
  Ld_Test<BrigOperandReg> TestCase5(in, sbuf, &out5, &width5, &dest5, &addr5);
  TestCase5.Run_Test(&Ld);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "ld_global_f32 $s1, [%g][0];\n");
  op1.assign("$s1"); op2.assign("");
  sbuf->append(op1); sbuf->append(op2);
    
  BrigOperandReg dest6 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  dest6.size = sizeof(dest6);

  BrigOperandImmed width6 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}  
  }; 
  width6.size = sizeof(width6);
   
  BrigOperandAddress addr6 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0  
  };
  addr6.size = sizeof(addr6);

  BrigOperandCompound comp6 = {
    0,  
    BrigEOperandCompound,
    Brigb64,
    0,
    sizeof(width6) + sizeof(dest6) + sizeof(addr6), 
    0,
    0
  };
  comp6.size = sizeof(comp6);
 
  BrigInstLdSt out6 = {
    0,                     
    BrigEInstLdSt,         
    BrigLd,                
    Brigf32,               
    BrigNoPacking,         
    {0, sizeof(width6), sizeof(width6) + sizeof(dest6) + sizeof(addr6), 0, 0},  
    BrigGlobalSpace,       
    BrigRegular,               
    0                      
  };
  out6.size = sizeof(out6); 
 
  Ld_Test<BrigOperandReg> TestCase6(in, sbuf, &out6, &width6, &dest6, &comp6, &addr6, NULL);
  TestCase6.Run_Test(&Ld);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "ld_global_u32 $s3, [%g][$s2];\n");
  op1.assign("$s3"); op2.assign("$s2");
  sbuf->append(op1); sbuf->append(op2);
    
  BrigOperandReg dest7 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  dest7.size = sizeof(dest7);

  BrigOperandImmed width7 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}  
  }; 
  width7.size = sizeof(width7);
   
  BrigOperandAddress addr7 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0  
  };
  addr7.size = sizeof(addr7);

  BrigOperandReg reg7 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    op1.size() + 1
  };
  reg7.size = sizeof(reg7);


  BrigOperandCompound comp7 = {
    0,  
    BrigEOperandCompound,
    Brigb64,
    0,
    sizeof(width7) + sizeof(dest7), 
    sizeof(width7) + sizeof(dest7) + sizeof(addr7),
    0
  };
  comp7.size = sizeof(comp7);
 
  BrigInstLdSt out7 = {
    0,                     
    BrigEInstLdSt,         
    BrigLd,                
    Brigu32,               
    BrigNoPacking,         
    {0, sizeof(width7), sizeof(width7) + sizeof(dest7) + sizeof(addr7) + sizeof(reg7), 0, 0},  
    BrigGlobalSpace,       
    BrigRegular,               
    0                      
  };
  out7.size = sizeof(out7); 
 
  Ld_Test<BrigOperandReg> TestCase7(in, sbuf, &out7, &width7, &dest7, &comp7, &addr7, &reg7);
  TestCase7.Run_Test(&Ld);  
  sbuf->clear();

/**********************************************************************************/
  in.assign( "ld_global_u32 $s4, [%g][$s2+4];\n");
  op1.assign("$s4"); op2.assign("$s2");
  sbuf->append(op1); sbuf->append(op2);
    
  BrigOperandReg dest8 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  dest8.size = sizeof(dest8);

  BrigOperandImmed width8 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}  
  }; 
  width8.size = sizeof(width8);
   
  BrigOperandAddress addr8 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0  
  };
  addr8.size = sizeof(addr8);

  BrigOperandReg reg8 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    op1.size() + 1
  };
  reg8.size = sizeof(reg8);

  BrigOperandCompound comp8 = {
    0,  
    BrigEOperandCompound,
    Brigb64,
    0,
    sizeof(width8) + sizeof(dest8), 
    sizeof(width8) + sizeof(dest8) + sizeof(addr8),
    4
  };
  comp8.size = sizeof(comp8);
 
  BrigInstLdSt out8 = {
    0,                     
    BrigEInstLdSt,         
    BrigLd,                
    Brigu32,               
    BrigNoPacking,         
    {0, sizeof(width8), sizeof(width8) + sizeof(dest8) + sizeof(addr8) + sizeof(reg8), 0, 0},  
    BrigGlobalSpace,       
    BrigRegular,               
    0                      
  };
  out8.size = sizeof(out8); 
 
  Ld_Test<BrigOperandReg> TestCase8(in, sbuf, &out8, &width8, &dest8, &comp8, &addr8, &reg8);
  TestCase8.Run_Test(&Ld); 
  sbuf->clear();

/**********************************************************************************/
  in.assign( "ld_readonly_s32 $s1, [%tbl][12];\n");
  op1.assign("$s1"); op2.assign("");
  sbuf->append(op1); sbuf->append(op2);
    
  BrigOperandReg dest9 = {
    0,
    BrigEOperandReg,
    Brigb32,
    0, 
    0
  };
  dest9.size = sizeof(dest9);

  BrigOperandImmed width9 = {
    0,
    BrigEOperandImmed,
    Brigb32,
    0,
    {0}  
  }; 
  width9.size = sizeof(width9);
   
  BrigOperandAddress addr9 = {
    0,
    BrigEOperandAddress,
    Brigb64,
    0,
    0  
  };
  addr9.size = sizeof(addr9);

  BrigOperandCompound comp9 = {
    0,  
    BrigEOperandCompound,
    Brigb64,
    0,
    sizeof(width9) + sizeof(dest9), 
    0,
    12
  };
  comp9.size = sizeof(comp9);
 
  BrigInstLdSt out9 = {
    0,                     
    BrigEInstLdSt,         
    BrigLd,                
    Brigs32,               
    BrigNoPacking,         
    {0, sizeof(width9), sizeof(width9) + sizeof(dest9) + sizeof(addr9), 0, 0},  
    BrigReadonlySpace,       
    BrigRegular,               
    0                      
  };
  out9.size = sizeof(out9); 
 
  Ld_Test<BrigOperandReg> TestCase9(in, sbuf, &out9, &width9, &dest9, &comp9, &addr9, NULL);
  TestCase9.Run_Test(&Ld);  
  sbuf->clear();

/**********************************************************************************/
  

  /******************************  End of tests *****************************************/
  delete sbuf;

}

}
}
