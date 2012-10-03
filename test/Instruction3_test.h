#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "parser.h"
#include "brig.h"
#include "error_reporter.h"
#include "context.h"
#include "codegen_validate.h"

namespace hsa{
namespace brig{

extern ErrorReporter* main_reporter;
extern Context* context;

template <class T, class T1, class T2, class T3> class Instruction3Opcode_Test{
public:
	std::string Input;
	//Instruction in .code buffer
	T Output;
	//Operands in .operands buffer
	T1 dest;
	T2 src1;
	T3 src2;
	//Symbols in .string buffer
	std::string dest_name;
	std::string src1_name;
	std::string src2_name;
	
	//Buffer offsets
	int code_start;
	int operand_start;
	int string_start;

	validate_brig<char> Instr3_validate;
	
	void assign(std::string in, T ref, T1 Dest, T2 Src1, T3 Src2, 
				std::string op1, std::string op2, std::string op3, int offset){
		Input.assign(in);
		Output = ref;
		dest = Dest;
		src1 = Src1;
		src2 = Src2;
		dest_name = op1;
		src1_name = op2;
		src2_name = op3;
		code_start = offset; 
		operand_start = offset;
	}
		
	void Run_Test(){
		context->set_error_reporter(main_reporter);
		context->clear_context();
		Lexer *lexer = new Lexer();
		lexer->set_source_string(Input);
		context->token_to_scan = lexer->get_next_token();
		int ret = Instruction3(context);
		EXPECT_EQ(0, ret);
		if(!ret){
			T getcode;
			context->get_code(code_start, &getcode);
			Instr3_validate.validate(Output, getcode);
			
			T1 getreg1;
			int op_offset = operand_start;
			context->get_operand(op_offset, &getreg1);
			Instr3_validate.validate(dest, getreg1);
			op_offset+=sizeof(getreg1);
			
			T2 getreg2;
			context->get_operand(op_offset, &getreg2);
			Instr3_validate.validate(src1, getreg2);
			op_offset+=sizeof(getreg2);
			
			T3 getreg3;
			context->get_operand(op_offset, &getreg3);
			Instr3_validate.validate(src2, getreg3);
			op_offset+=sizeof(getreg3);
		}
		context->clear_context();
		delete lexer;
	}
};
  
/*
Steps for Unit Test Generation
1. Declare the HSAIL string, and sub-strings, if any
2. Declare all Brig structures that have been created by the HSAIL string and enter correct values.
3. Create a class with the appropriate type
4. Assign the appropriate reference values,  and call Run_Test(). 
*/
  
TEST(CodegenTest, Instruction3Op_CodeGen){
	
	std:: string in; 
	in.assign( "add_pp_sat_u16x2 $s1, $s0, $s3; \n");
	std::string op1, op2, op3; op1.assign("$s1"); op2.assign("$s0"); op3.assign("$s3");
		
	int buffer_start = 8;		//All buffers(.code, .directive, .string) begin at an offset of 8 bytes
	int size_reg = sizeof(BrigOperandReg);
	BrigInstBase Out = {
		32,
		BrigEInstBase, 
		BrigAdd, 
		Brigu16x2,
		BrigPackPPsat,
		{buffer_start, buffer_start + size_reg, buffer_start + 2*size_reg, 0, 0}
	};
	BrigOperandReg reg1 = {
		sizeof(BrigOperandReg),
		BrigEOperandReg,
		Brigb32,
		0,
		buffer_start //Offset to string table	
	};
	BrigOperandReg reg2, reg3;
	reg2 = reg3 = reg1;
	reg2.name = buffer_start + op1.size()+1; reg3.name = buffer_start + op1.size()+1 + op2.size()+1;
	
	Instruction3Opcode_Test<BrigInstBase, BrigOperandReg, BrigOperandReg, BrigOperandReg> TestCase1;
	TestCase1.assign(in, Out, reg1, reg2, reg3, op1, op2, op3, buffer_start);
	TestCase1.Run_Test();
	
	/***************************Add more Unit Tests************************************/
	
/*	
	in.assign( "add_s64 $d1, $d2, $d3;\n");
	BrigInstBase out1 = {
		32,
		BrigEInstBase, 
		BrigAdd, 
		Brigs64,
		BrigNoPacking,
		{8, 20, 32, 0, 0}
	};
	init(in, out1, numTestCases++);		
	
	in.assign( "add_u64 $d1, WAVESIZE, $d3;\n");
	BrigInstBase out2 = {
		32,
		BrigEInstBase, 
		BrigAdd, 
		Brigu64,
		BrigNoPacking,
		{8, 20, 24, 0, 0}
	};
	init(in, out2, numTestCases++);

	in.assign( "add_ps_sat_s16x4 $d1, 0x40, WAVESIZE;\n");
	BrigInstBase out3 = {
		32,
		BrigEInstBase, 
		BrigAdd, 
		Brigs16x4,
		BrigPackPSsat,
		{8, 24, 24 + sizeof(BrigOperandImmed), 0, 0}
	};
	init(in, out3, numTestCases++);

	in.assign( "div_s32 $s1, 100, 10;\n");
	BrigInstBase out4 = {
		32,
		BrigEInstBase, 
		BrigDiv, 
		Brigs32,
		BrigNoPacking,
		{8, 24, 24 + sizeof(BrigOperandImmed), 0, 0}
	};
	init(in, out4, numTestCases++);

	in.assign( "div_u64 $d1, $d3, 0x233412349456;\n");
	BrigInstBase out5 = {
		32,
		BrigEInstBase, 
		BrigDiv, 
		Brigu64,
		BrigNoPacking,
		{8, 20, 32, 0, 0}
	};
	init(in, out5, numTestCases++);
	
	in.assign( "rem_s64 $d1, $d3, WAVESIZE;\n");
	BrigInstBase out6 = {
		32,
		BrigEInstBase, 
		BrigRem, 
		Brigs64,
		BrigNoPacking,
		{8, 20, 32, 0, 0}
	};
	init(in, out6, numTestCases++); 
	
	in.assign( "rem_u64 $d1, 0x040, 0x233412349456;\n");
	BrigInstBase out7 = {
		32,
		BrigEInstBase, 
		BrigRem, 
		Brigu64,
		BrigNoPacking,
		{8, 24, 48, 0, 0}
	};
	init(in, out7, numTestCases++);
	
	in.assign( "shl_u32 $s1, $s1, 2;\n");
	BrigInstBase out8 = {
		32,
		BrigEInstBase, 
		BrigShl, 
		Brigu32,
		BrigNoPacking,
		{8, 8, 24, 0, 0}
	};
	init(in, out8, numTestCases++);
	
	in.assign("class_f32 $c1, $s2, 0x10;");
	BrigInstBase out9 = {
		32,
		BrigEInstBase, 
		BrigClass, 
		Brigb1,
		BrigNoPacking,
		{8, 20, 32, 0, 0}
	};
	init(in, out9, numTestCases++);
	
	in.assign("class_f64 $c0, $d1, $s1;");
	BrigInstBase out10 = {
		32,
		BrigEInstBase, 
		BrigClass, 
		Brigb1,
		BrigNoPacking,
		{8, 20, 32, 0, 0}
	};
	init(in, out10, numTestCases++);
*/	
  }
} //namespace hsa
} //namespace brig




