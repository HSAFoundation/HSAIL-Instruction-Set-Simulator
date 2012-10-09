#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

/*Template describes the type of the destination, reg/regv2/regv4 or immediate value*/
template <typename T> class St_Test : public BrigCodeGenTest{
private:
  
  //Symbols in .string buffer
  const std::string dest_name;
  const std::string src_name;
    
  const BrigInstLdSt* Output;
   
  //Dest Operand
  const T* src_reg_immed; 
  //Source operands. Only required structs will be used, the others will stay NULL.
  const BrigOperandReg* oper_reg;
  const BrigOperandAddress* oper_addr;
  const BrigOperandIndirect* oper_indirect;
  const BrigOperandCompound* oper_comp;
  
public:
  //TestCase outputs a BrigOperandAddress only
  St_Test(std::string& input, std::string& op1, std::string& op2, BrigInstLdSt* ref, 
      T* src, BrigOperandAddress* addr) :
    BrigCodeGenTest(input), 
    dest_name(op1), 
    src_name(op2), 
    Output(ref),
    src_reg_immed(src), 
    oper_reg(NULL), 
    oper_addr(addr), 
    oper_indirect(NULL), 
    oper_comp(NULL)  { }
      
  //Testcase output is a BrigOperandIndirect    
  St_Test(std::string& input, std::string& op1, std::string& op2, BrigInstLdSt* ref, 
      T* src, BrigOperandIndirect* indir, BrigOperandReg* reg=NULL) : 
    BrigCodeGenTest(input),
    dest_name(op1),
    src_name(op2),
    Output(ref),
    src_reg_immed(src),
    oper_reg(reg),
    oper_addr(NULL),
    oper_indirect(indir),
    oper_comp(NULL)  { }
  
  //TestCase output is a BrigOperandCompound
  St_Test(std::string& input, std::string& op1, std::string& op2, BrigInstLdSt* ref, 
     T* src, BrigOperandCompound* comp, BrigOperandAddress* addr, BrigOperandReg* reg=NULL) : 
    BrigCodeGenTest(input),
    dest_name(op1),
    src_name(op2),
    Output(ref),
    src_reg_immed(src),
    oper_reg(reg),
    oper_addr(addr),  
    oper_indirect(NULL),
    oper_comp(comp)   { }
   
  void validate(Context* context){
    
    BrigInstLdSt getcode;
    context->get_code(code_start, &getcode);
    validate_brig::validate(Output, &getcode);
    int op_offset = operand_start;
        
    T getsrc;
    context->get_operand(op_offset, &getsrc);
    validate_brig::validate(src_reg_immed, &getsrc);
    op_offset+=sizeof(getsrc);
    
    if(oper_indirect){
      if(oper_reg){
        BrigOperandReg getreg;
        context->get_operand(op_offset, &getreg);
        validate_brig::validate(oper_reg, &getreg);
        op_offset+=sizeof(getreg);
      }   
      BrigOperandIndirect getindir;
      context->get_operand(op_offset, &getindir);
      validate_brig::validate(oper_indirect, &getindir);
      op_offset+=sizeof(getindir);      
             
    } else if(oper_comp){
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
      BrigOperandCompound getcomp;
      context->get_operand(op_offset, &getcomp);
      validate_brig::validate(oper_comp, &getcomp);
      op_offset+=sizeof(getcomp);      
      
    } else {
      BrigOperandAddress getaddr;
      context->get_operand(op_offset, &getaddr);
      validate_brig::validate(oper_addr, &getaddr);
      op_offset+=sizeof(getaddr);
    }
  }
};

TEST(CodegenTest, St_Codegen){

  std::string in, op1, op2, op3; 
  in.assign( "st_arg_f32 $s0, [%output][$s2-4];\n");
  op1.assign("$s0"); op2.assign("%output"); op3.assign("$s2-4");
  int buffer_start = BUFFER_OFFSET;
  
  //Reference to $s0
  BrigOperandReg src = {
  sizeof(BrigOperandReg),
  BrigEOperandReg,
  Brigb32,
  0, 
  buffer_start  //Offset to string table
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
  buffer_start + op1.size()+1, 
  };
  
  BrigOperandCompound comp = {
  sizeof(BrigOperandCompound),
  BrigEOperandCompound,
  Brigb64,
  0,
  buffer_start + sizeof(src),//byte offset to operand addr
  buffer_start + sizeof(src) + sizeof(dest1), //to operand reg
  -4  //Offset in the included test case
  };  
   
  BrigInstLdSt out = {
    sizeof(BrigInstLdSt),                // size
    BrigEInstLdSt,     // kind
    BrigSt,            // opcode
    Brigf32,           // type
    BrigNoPacking,     // packing
    {buffer_start, buffer_start + sizeof(src) + sizeof(dest1) + sizeof(dest2), 0, 0, 0},  // operand[5]
    BrigArgSpace,      // storageClass
    BrigRegular,       // memorySemantic
    0                  // equivClass
  };
    
  St_Test<BrigOperandReg> TestCase1(in, op1, op2, &out, &src, &comp, &dest1, &dest2);
  TestCase1.Run_Test(&St);   


/********************************Add More Unit tests **********************************/

}
}
}
