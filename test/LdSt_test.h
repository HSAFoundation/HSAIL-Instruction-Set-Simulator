#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "parser.h"
#include "brig.h"
#include "error_reporter.h"
#include "context.h"
#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

using namespace validate_brig;

/*Template describes the type of the destination, reg/regv2/regv4*/
template <typename T> class Ld_Test{
private:
	std::string Input;

	BrigInstLdSt* Output;
	
	//Width operand
	BrigOperandImmed* OpWidth;
	
	//Dest Operand
	T* dest_reg;
	
	//Source operands. Only required structs will be used, the others will stay NULL.
	BrigOperandReg* oper_reg;
	BrigOperandAddress* oper_addr;
	BrigOperandIndirect* oper_indirect;
	BrigOperandCompound* oper_comp;
		
	//Symbols in .string buffer
	std::string dest_name;
	std::string src_name;
	
	//Buffer offsets
	int code_start;
	int operand_start;
	int string_start;
public:
	//TestCase outputs a BrigOperandAddress only
	Ld_Test(std::string input, std::string op1, std::string op2, int offset, BrigInstLdSt* ref, 
			BrigOperandImmed* width, T* dest, BrigOperandAddress* addr){
		Input.assign(input);
		dest_name = op1;
		src_name = op2;
		Output = ref;
		OpWidth = width;
		dest_reg = dest;
		oper_addr = addr;
		oper_reg = NULL;
		oper_indirect = NULL;
		oper_comp = NULL;
			
		code_start = operand_start = string_start = offset;
	}
			
	//Testcase output is a BrigOperandIndirect		
	Ld_Test(std::string input, std::string op1, std::string op2, int offset, BrigInstLdSt* ref, 
			BrigOperandImmed* width, T* dest, BrigOperandIndirect* indir, BrigOperandReg* reg=NULL){
		Input.assign(input);
		dest_name = op1;
		src_name = op2;
		Output = ref;
		OpWidth = width;	
		dest_reg = dest;
		oper_reg = reg;
		oper_indirect = indir;
		oper_comp = NULL;
		oper_addr = NULL;
			
		code_start = operand_start = string_start = offset;
	}
	
	//TestCase output is a BrigOperandCompound
	Ld_Test(std::string input, std::string op1, std::string op2, int offset, BrigInstLdSt* ref, 
			BrigOperandImmed* width, T* dest, BrigOperandCompound* comp, BrigOperandAddress* addr, BrigOperandReg* reg=NULL){
		Input.assign(input);
		dest_name = op1;
		src_name = op2;
		Output = ref;
		OpWidth = width;
		dest_reg = dest;
		
		oper_reg = reg;
		oper_indirect = NULL;
		oper_comp = comp;
		oper_addr = addr;
			
		code_start = operand_start = string_start = offset;			
	}
	
	void Run_Test(){
		ErrorReporter* main_reporter = ErrorReporter::get_instance();
		Context* context = Context::get_instance();
		
		context->set_error_reporter(main_reporter);
		context->clear_context();
		Lexer *lexer = new Lexer();
		lexer->set_source_string(Input);
		context->token_to_scan = lexer->get_next_token();
		int ret = Ld(context);
		EXPECT_EQ(0, ret);
		if(!ret){
			BrigInstLdSt getcode;
			context->get_code(code_start, &getcode);
			validate(Output, &getcode);
			int op_offset = operand_start;
			
			BrigOperandImmed getwidth;
			context->get_operand(op_offset, &getwidth);
			validate(OpWidth, &getwidth);
			op_offset+=sizeof(getwidth);
			
			T getdest;
			context->get_operand(op_offset, &getdest);
			validate(dest_reg, &getdest);
			op_offset+=sizeof(getdest);
			
			if(oper_indirect){
				BrigOperandIndirect getindir;
				context->get_operand(op_offset, &getindir);
				validate(oper_indirect, &getindir);
				op_offset+=sizeof(getindir);
				
				if(oper_reg){
					BrigOperandReg getreg;
					context->get_operand(op_offset, &getreg);
					validate(oper_reg, &getreg);
					op_offset+=sizeof(getreg);
				}					
			} else if(oper_comp){
				BrigOperandCompound getcomp;
				context->get_operand(op_offset, &getcomp);
				validate(oper_comp, &getcomp);
				op_offset+=sizeof(getcomp);
				
				BrigOperandAddress getaddr;
				context->get_operand(op_offset, &getaddr);
				validate(oper_addr, &getaddr);
				op_offset+=sizeof(getaddr);
				
				if(oper_reg){
					BrigOperandReg getreg;
					context->get_operand(op_offset, &getreg);
					validate(oper_reg, &getreg);
					op_offset+=sizeof(getreg);
				}				
			} else {
				BrigOperandAddress getaddr;
				context->get_operand(op_offset, &getaddr);
				validate(oper_addr, &getaddr);
				op_offset+=sizeof(getaddr);
			}
		}
		context->clear_context();
		delete lexer;
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
	sizeof(BrigOperandImmed),
	BrigEOperandImmed,
	Brigb32,
	0,
	{0}	
	};
	
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
		
	Ld_Test<BrigOperandReg> TestCase1(in, op1, op2, buffer_start, &out, &width, &dest, &addr);
	TestCase1.Run_Test();	
	


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
