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
    const BrigOperandBase *getdest_op1 = reinterpret_cast <const BrigOperandBase*> (&(TestOutput->operands[getinst->o_operands[1]]));
    
    if(getdest_op1->kind==BrigEOperandIndirect){
      const BrigOperandIndirect *getdest_indir = reinterpret_cast <const BrigOperandIndirect*> (getdest_op1);
      validate_brig::validate(RefDest_Indir, getdest_indir); 
      
      if(getdest_indir->reg){
        const BrigOperandReg *getdest_reg = reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getdest_indir->reg]));
        validate_brig::validate(RefDest_Reg, refbuf, getdest_reg, getbuf);
      }         
             
    } else if(getdest_op1->kind==BrigEOperandCompound){
      
      const BrigOperandCompound *getdest_comp = reinterpret_cast <const BrigOperandCompound*> (getdest_op1);
      validate_brig::validate(RefDest_Comp, getdest_comp); 
            
      const BrigOperandAddress *getdest_addr = reinterpret_cast <const BrigOperandAddress*> (&(TestOutput->operands[getdest_comp->name]));
      validate_brig::validate(RefDest_Addr, getdest_addr);         
      
      if(getdest_comp->reg){
        const BrigOperandReg *getdest_reg = reinterpret_cast <const BrigOperandReg*> (&(TestOutput->operands[getdest_comp->reg]));
        validate_brig::validate(RefDest_Reg, refbuf, getdest_reg, getbuf); 
      } 
            
    } else {
      const BrigOperandAddress *getdest_addr = reinterpret_cast <const BrigOperandAddress*> (&(TestOutput->operands[getinst->o_operands[1]]));
      validate_brig::validate(RefDest_Addr, getdest_addr);
    }
    EXPECT_EQ(0, getinst->o_operands[2]);
    EXPECT_EQ(0, getinst->o_operands[3]);
    EXPECT_EQ(0, getinst->o_operands[4]);
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
  0,
  BrigEOperandReg,
  Brigb32,
  0, 
  0  //Offset to string table
  };
  src.size = sizeof(src);

  //ref to %output - Not added in symbol table; it is assumed that it was added during variable decl
  BrigOperandAddress dest1 = {
  0,
  BrigEOperandAddress,
  Brigb64,
  0,
  0  //Offset to .directives, However, no directive in .directives. Ambiguous testing.
  };
  dest1.size = sizeof(dest1);
  
  //Ref to $s2
  BrigOperandReg dest2 = {
  0,
  BrigEOperandReg,
  Brigb32,
  0,
  op1.size() + op2.size() +2, 
  };
  dest2.size = sizeof(dest2);

  BrigOperandCompound comp = {
  0,
  BrigEOperandCompound,
  Brigb64,
  0,
  sizeof(src),//byte offset to operand addr
  sizeof(src) + sizeof(dest1), //to operand reg
  -4  //Offset in the included test case
  };  
  comp.size = sizeof(comp);

  BrigInstLdSt out1 = {
    0,                // size
    BrigEInstLdSt,     // kind
    BrigSt,            // opcode
    Brigf32,           // type
    BrigNoPacking,     // packing
    {0, sizeof(src) + sizeof(dest1) + sizeof(dest2), 0, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
  out1.size = sizeof( out1);

  St_Test<BrigOperandReg> TestCase1(in, sbuf, &out1, &src, &comp, &dest1, &dest2);
  TestCase1.Run_Test(&St);   
  sbuf->clear();
  
  /*********************************** Test Case 2************************************************************/
  
  in.assign( "st_arg_f32 $s0, [$s2-4];\n");
  op1.assign("$s0");  op2.assign("$s2");
  sbuf->append(op1); sbuf->append(op2); 
 
  //Reference to $s0
  src.size = sizeof(src);
  src.kind = BrigEOperandReg;
  src.type = Brigb32;
  src.reserved = 0;
  src.name = 0;
  
  //Ref to $s2
  dest2.size = sizeof(dest2);
  dest2.kind = BrigEOperandReg;
  dest2.type = Brigb32;
  dest2.reserved = 0;
  dest2.name = op1.size() + 1;
  
  BrigOperandIndirect indirect = {
  0,
  BrigEOperandIndirect,
  sizeof(src) + sizeof(dest2), //to operand reg
  Brigb64,
  0,  
  -4  //Offset in the included test case
  };  
  indirect.size = sizeof(indirect);

  BrigInstLdSt out2 = {
    0,                // size
    BrigEInstLdSt,     // kind
    BrigSt,            // opcode
    Brigf32,           // type
    BrigNoPacking,     // packing
    {0, sizeof(src) + sizeof(dest2) , 0, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
  out2.size = sizeof(out2);

  St_Test<BrigOperandReg> TestCase2(in, sbuf, &out2, &src, &indirect, &dest2);
  TestCase2.Run_Test(&St);   
  sbuf->clear();

  /*********************************** Test Case 3************************************************************/
  
  in.assign( "st_arg_f32 $s0, [%output];\n");
  op1.assign("$s0"); op2.assign("%output");
  sbuf->append(op1);sbuf->append(op2);
  
  //Reference to $s0
  src.size = sizeof(src);
  src.kind = BrigEOperandReg;
  src.type = Brigb32;
  src.reserved = 0;
  src.name = 0;
  
  //ref to %output - Not added in symbol table; it is assumed that it was added during variable decl 
  dest1.size = sizeof(dest1);
  dest1.kind = BrigEOperandAddress;
  dest1.type = Brigb64;
  dest1.reserved = 0;
  dest1.directive = 0;//Offset to .directives, However, no directive in .directives. Ambiguous testing.
   
  BrigInstLdSt out3 = {
    0,                // size
    BrigEInstLdSt,     // kind
    BrigSt,            // opcode
    Brigf32,           // type
    BrigNoPacking,     // packing
    {0, sizeof(src) + sizeof(dest1) , 0, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
  out3.size = sizeof(out3);

  St_Test<BrigOperandReg> TestCase3(in, sbuf, &out3, &src, &dest1);
  TestCase3.Run_Test(&St);   
  sbuf->clear();

  /*********************************** Test Case 4************************************************************/
  
  in.assign( "st_arg_f32 0x1234, [%output];\n");
  op1.assign("%output");
  sbuf->append(op1);
  
  //for immed
  BrigOperandImmed src1 = {
    0, //size
    BrigEOperandImmed, //kind
    Brigb32,                      //type
    0,                                 //reserved
    {0x1234}
  };
  src1.size = sizeof(src1);

  //ref to %output - Not added in symbol table; it is assumed that it was added during variable decl 
  dest1.size = sizeof(dest1);
  dest1.kind = BrigEOperandAddress;
  dest1.type = Brigb64;
  dest1.reserved = 0;
  dest1.directive = 0;//Offset to .directives, However, no directive in .directives. Ambiguous testing.
   

  BrigInstLdSt out4 = {
    0,                // size
    BrigEInstLdSt,     // kind

    BrigSt,            // opcode
    Brigf32,           // type
    BrigNoPacking,     // packing

    {0, sizeof(src1) + sizeof(dest1) , 0, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic

    0                  // equivClass
  };
  out4.size = sizeof(out4);

  St_Test<BrigOperandImmed> TestCase4(in, sbuf, &out4, &src1, &dest1);
  TestCase4.Run_Test(&St);   
  sbuf->clear();

  /*********************************** Test Case 5************************************************************/
  
  in.assign( "st_arg_f64 3.1415l, [$s2-4];\n");
  op1.assign("$s2");  
  sbuf->append(op1); 
 
  //Reference to immed
  src1.size = sizeof(src1);
  src1.kind = BrigEOperandImmed;
  src1.type = Brigb64;
  src1.reserved = 0;
  src1.bits.d = 3.1415l;
  
  //Ref to $s2
  dest2.size = sizeof(dest2);
  dest2.kind = BrigEOperandReg;
  dest2.type = Brigb32;
  dest2.reserved = 0;
  dest2.name = 0;
  
  indirect.size = sizeof(indirect);
  indirect.kind = BrigEOperandIndirect;
  indirect.reg = sizeof(src1) + sizeof(dest2);
  indirect.type = Brigb64;
  indirect.reserved = 0;
  indirect.offset = -4 ; 
 
  BrigInstLdSt out5 = {
    0,                // size
    BrigEInstLdSt,     // kind
    BrigSt,            // opcode
    Brigf64,           // type
    BrigNoPacking,     // packing
    {0, sizeof(src1) + sizeof(dest2) , 0, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
  out5.size = sizeof(out5);

  St_Test<BrigOperandImmed> TestCase5(in, sbuf, &out5, &src1, &indirect, &dest2);
  TestCase5.Run_Test(&St);   
  sbuf->clear();

 /*********************************** Test Case 6************************************************************/
  
  in.assign( "st_arg_f64 3.1415l, [%output][$s2-4];\n");
  op1.assign("%output");  op2.assign("$s2"); 
  sbuf->append(op1);  sbuf->append(op2); 
 
  //Reference to immed
  src1.size = sizeof(src1);
  src1.kind = BrigEOperandImmed;
  src1.type = Brigb64;
  src1.reserved = 0;
  src1.bits.d = 3.1415l;
  
  //ref to %output - Not added in symbol table; it is assumed that it was added during variable decl 
  dest1.size = sizeof(dest1);
  dest1.kind = BrigEOperandAddress;
  dest1.type = Brigb64;
  dest1.reserved = 0;
  dest1.directive = 0;//Offset to .directives, However, no directive in .directives. Ambiguous testing.

  //Ref to $s2
  dest2.size = sizeof(dest2);
  dest2.kind = BrigEOperandReg;
  dest2.type = Brigb32;
  dest2.reserved = 0;
  dest2.name = op1.size() + 1;
  
  // ref to comp
  comp.size = sizeof(comp);
  comp.kind = BrigEOperandCompound;
  comp.type = Brigb64;
  comp.reserved = 0;
  comp.name = sizeof(src1);
  comp.reg = sizeof(src1) + sizeof(dest1);
  comp.offset = -4 ; 

  BrigInstLdSt out6 = {
    0,                // size
    BrigEInstLdSt,     // kind
    BrigSt,            // opcode
    Brigf64,           // type
    BrigNoPacking,     // packing
    {0, sizeof(src1) + sizeof(dest1) + sizeof(dest2), 0, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
  out6.size = sizeof( out6);


  St_Test<BrigOperandImmed> TestCase6(in, sbuf, &out6, &src1, &comp, &dest1, &dest2);
  TestCase6.Run_Test(&St);   
  sbuf->clear();

 /*********************************** Test Case 6************************************************************/
  
  in.assign( "st_arg_f64 3.1415l, [%output][$s2-4];\n");
  op1.assign("%output");  op2.assign("$s2"); 
  sbuf->append(op1);  sbuf->append(op2); 
 
  //Reference to immed
  src1.size = sizeof(src1);
  src1.kind = BrigEOperandImmed;
  src1.type = Brigb64;
  src1.reserved = 0;
  src1.bits.d = 3.1415l;
  
  //ref to %output - Not added in symbol table; it is assumed that it was added during variable decl 
  dest1.size = sizeof(dest1);
  dest1.kind = BrigEOperandAddress;
  dest1.type = Brigb64;
  dest1.reserved = 0;
  dest1.directive = 0;//Offset to .directives, However, no directive in .directives. Ambiguous testing.

  //Ref to $s2
  dest2.size = sizeof(dest2);
  dest2.kind = BrigEOperandReg;
  dest2.type = Brigb32;
  dest2.reserved = 0;
  dest2.name = op1.size() + 1;
  
  // ref to comp
  comp.size = sizeof(comp);
  comp.kind = BrigEOperandCompound;
  comp.type = Brigb64;
  comp.reserved = 0;
  comp.name = sizeof(src1);
  comp.reg = sizeof(src1) + sizeof(dest1);
  comp.offset = -4 ; 

  BrigInstLdSt out6 = {
    0,                // size
    BrigEInstLdSt,     // kind
    BrigSt,            // opcode
    Brigf64,           // type
    BrigNoPacking,     // packing
    {0, sizeof(src1) + sizeof(dest1) + sizeof(dest2), 0, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
  out6.size = sizeof( out6);


  St_Test<BrigOperandImmed> TestCase6(in, sbuf, &out6, &src1, &comp, &dest1, &dest2);
  TestCase6.Run_Test(&St);   
  sbuf->clear();

/************************************ End of tests*******************************************/
  delete sbuf;
}

}
}
