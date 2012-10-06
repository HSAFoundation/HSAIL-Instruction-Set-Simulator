#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

/*Template describes the type of the destination, reg/regv2/regv4*/
template <typename T> class Ld_Test : public BrigCodeGenTest{
private:
  
  //Symbols in .string buffer
  const std::string dest_name;
  const std::string src_name;
    
  const BrigInstLdSt* Output;
  
  //Width operand
  const BrigOperandImmed* OpWidth;
  
  //Dest Operand
  const T* dest_reg;
  //Source operands. Only required structs will be used, the others will stay NULL.
  const BrigOperandReg* oper_reg;
  const BrigOperandAddress* oper_addr;
  const BrigOperandIndirect* oper_indirect;
  const BrigOperandCompound* oper_comp;
  
public:
  //TestCase outputs a BrigOperandAddress only
  Ld_Test(std::string& input, std::string& op1, std::string& op2, BrigInstLdSt* ref, 
      BrigOperandImmed* width, T* dest, BrigOperandAddress* addr) :
    BrigCodeGenTest(input), 
    dest_name(op1), 
    src_name(op2), 
    Output(ref),
    OpWidth(width), 
    dest_reg(dest), 
    oper_reg(NULL), 
    oper_addr(addr), 
    oper_indirect(NULL), 
    oper_comp(NULL)  { }
      
  //Testcase output is a BrigOperandIndirect    
  Ld_Test(std::string& input, std::string& op1, std::string& op2, BrigInstLdSt* ref, 
      BrigOperandImmed* width, T* dest, BrigOperandIndirect* indir, BrigOperandReg* reg=NULL) : 
    BrigCodeGenTest(input),
    dest_name(op1),
    src_name(op2),
    Output(ref),
    OpWidth(width),
    dest_reg(dest),
    oper_reg(reg),
    oper_addr(NULL),
    oper_indirect(indir),
    oper_comp(NULL)  { }
  
  //TestCase output is a BrigOperandCompound
  Ld_Test(std::string& input, std::string& op1, std::string& op2, BrigInstLdSt* ref, 
      BrigOperandImmed* width, T* dest, BrigOperandCompound* comp, BrigOperandAddress* addr, BrigOperandReg* reg=NULL) : 
    BrigCodeGenTest(input),
    dest_name(op1),
    src_name(op2),
    Output(ref),
    OpWidth(width),
    dest_reg(dest),
    oper_reg(reg),
    oper_indirect(NULL),
    oper_comp(comp),
    oper_addr(addr)  { }
   
  void validate(Context* context){
    BrigInstLdSt getcode;
    context->get_code(code_start, &getcode);
    validate_brig::validate(Output, &getcode);
    int op_offset = operand_start;
    
    BrigOperandImmed getwidth;
    context->get_operand(op_offset, &getwidth);
    validate_brig::validate(OpWidth, &getwidth);
    op_offset+=sizeof(getwidth);
    
    T getdest;
    context->get_operand(op_offset, &getdest);
    validate_brig::validate(dest_reg, &getdest);
    op_offset+=sizeof(getdest);
    
    if(oper_indirect){
      BrigOperandIndirect getindir;
      context->get_operand(op_offset, &getindir);
      validate_brig::validate(oper_indirect, &getindir);
      op_offset+=sizeof(getindir);
      
      if(oper_reg){
        BrigOperandReg getreg;
        context->get_operand(op_offset, &getreg);
        validate_brig::validate(oper_reg, &getreg);
        op_offset+=sizeof(getreg);
      }          
    } else if(oper_comp){
      BrigOperandCompound getcomp;
      context->get_operand(op_offset, &getcomp);
      validate_brig::validate(oper_comp, &getcomp);
      op_offset+=sizeof(getcomp);
      
      BrigOperandAddress getaddr;
      context->get_operand(op_offset, &getaddr);
      validate_brig::validate(oper_addr, &getaddr);
      op_offset+=sizeof(getaddr);
      
      if(oper_reg){
        BrigOperandReg getreg;
        context->get_operand(op_offset, &getreg);
        validate_brig::validate(oper_reg, &getreg);
        op_offset+=sizeof(getreg);
      }        
    } else {
      BrigOperandAddress getaddr;
      context->get_operand(op_offset, &getaddr);
      validate_brig::validate(oper_addr, &getaddr);
      op_offset+=sizeof(getaddr);
    }
  }
};

TEST(CodegenTest, Ld_Codegen){

  std::string in, op1, op2; 
  in.assign( "ld_arg_f32 $s0, [%input];\n");
  op1.assign("$s0"); op2.assign("[%input]");
  int buffer_start = BUFFER_OFFSET;
  
  BrigOperandReg dest = {
  sizeof(BrigOperandReg),
  BrigEOperandReg,
  Brigb32,
  0, 
  buffer_start  //Offset to string table
  };
  
  BrigOperandImmed width = {
  0,
  BrigEOperandImmed,
  Brigb32,
  0,
  {0}  
  }; 
  width.size = sizeof(width);
  
  BrigOperandAddress addr = {
  sizeof(BrigOperandAddress),
  BrigEOperandAddress,
  Brigb64,
  0,
  0  //Offset to .directives, However, no directive in .directives. Ambiguous testing.
  };
    
  BrigInstLdSt out = {
    sizeof(BrigInstLdSt),                // size
    BrigEInstLdSt,     // kind
    BrigLd,            // opcode
    Brigf32,           // type
    BrigNoPacking,     // packing
    {buffer_start, buffer_start + sizeof(width), buffer_start + sizeof(width) + sizeof(dest), 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
    
  Ld_Test<BrigOperandReg> TestCase1(in, op1, op2, &out, &width, &dest, &addr);
  TestCase1.Run_Test(&Ld);  
  


/********************************Add More Unit tests **********************************/
/*  
  in.assign("st_arg_f32 $s0, [%output][$s2-4];\n");
  BrigInstLdSt out1 = {
    44,                // size
    BrigEInstLdSt,     // kind
    BrigSt,            // opcode
    Brigf32,           // type
    BrigNoPacking,     // packing
    {8, 44, 0, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
  
    //Ref to $s0
    BrigOperandReg reg2 = {
  sizeof(BrigOperandReg),
  BrigEOperandReg,
  Brigb32,
  0,
  16
  };
    
  //Ref to $s2
  BrigOperandReg reg3 = {
  sizeof(BrigOperandReg),
  BrigEOperandReg,
  Brigb32,
  0,
  20 
  };
  
  //Ref to Compound Addr
  BrigOperandCompound comp = {
  sizeof(BrigOperandCompound),
  BrigEOperandCompound,
  Brigb64,
  0,
  20,//byte offset to operand addr
  32, //to operand reg
  -4  
  };
  
  TestCase_LdSt[1].init(in, out1, reg2, addr, &reg3, &comp);  
  */

}
}
}
