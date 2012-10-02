#include <iostream>
#include "gtest/gtest.h"
#include "tokens.h"
#include "lexer.h"
#include "parser.h"
#include "brig.h"
#include "error_reporter.h"
#include "context.h"
#include "parser_wrapper.h"

namespace hsa{
namespace brig{

extern ErrorReporter* main_reporter;
extern Context* context;

template <class T1, class T2, class T3> class Instruction3Opcode_Test{
public:
	std::string Input;
	BrigInstBase Output;
	T1 dest;
	T2 src1;
	T3 src2;
	int code_offset;
	
	void validate(BrigInstBase get){
		EXPECT_EQ(Output.size, get.size);
		EXPECT_EQ(Output.kind, get.kind);
		EXPECT_EQ(Output.opcode, get.opcode);
		EXPECT_EQ(Output.type, get.type);
		EXPECT_EQ(Output.packing, get.packing);
		EXPECT_EQ(Output.o_operands[0], get.o_operands[0]);
		EXPECT_EQ(Output.o_operands[1], get.o_operands[1]);
		EXPECT_EQ(Output.o_operands[2], get.o_operands[2]);
		EXPECT_EQ(Output.o_operands[3], get.o_operands[3]);
		EXPECT_EQ(Output.o_operands[4], get.o_operands[4]);
	}
	
	void validate(BrigOperandReg ref, BrigOperandReg get){
		EXPECT_EQ(ref.size, get.size);
		EXPECT_EQ(ref.kind, get.kind);
		EXPECT_EQ(ref.type, get.type);
		EXPECT_EQ(ref.reserved, get.reserved);
		EXPECT_EQ(ref.name, get.name);
	}
	
	void validate(BrigOperandImmed ref, BrigOperandImmed get){
		EXPECT_EQ(ref.size, get.size);
		EXPECT_EQ(ref.kind, get.kind);
		EXPECT_EQ(ref.type, get.type);
		EXPECT_EQ(ref.reserved, get.reserved);
		EXPECT_EQ(ref.bits, get.bits);
	}
	
	void assign(std::string in, BrigInstBase ref, T1 Dest, T2 Src1, T3 Src2){
		Input.assign(in);
		Output = ref;
		dest = Dest;
		src1 = Src1;
		src2 = Src2;
	}
		
	void Run_Test(Context* context){
		context->set_error_reporter(main_reporter);
		context->clear_context();
		Lexer *lexer = new Lexer();
		lexer->set_source_string(Input);
		context->token_to_scan = lexer->get_next_token();
		int ret = Instruction3(context);
		EXPECT_EQ(0, ret);
		if(!ret){
			BrigInstBase getcode;
			context->get_code(8, &getcode);
			this->validate(getcode);
			BrigOperandReg getreg;
			context->get_operand(8, &getreg);
			this->validate(dest, getreg);
			context->get_operand(20, &getreg);
			this->validate(src1, getreg);
			context->get_operand(32, &getreg);
			this->validate(src2, getreg);
		}
		context->clear_context();
		delete lexer;
	}
};
  
TEST(CodegenTest, Instruction3Op_CodeGen){

	Instruction3Opcode_Test<BrigOperandReg, BrigOperandReg, BrigOperandReg> TestCase1;
	std:: string in; 
	in.assign( "add_pp_sat_u16x2 $s1, $s0, $s3; \n");
	BrigInstBase Out = {
		32,
		BrigEInstBase, 
		BrigAdd, 
		Brigu16x2,
		BrigPackPPsat,
		{8, 20, 32, 0, 0}
	};
	BrigOperandReg reg1 = {
		12,
		BrigEOperandReg,
		Brigb32,
		0,
		8 //Offset to string table	
	};
	BrigOperandReg reg2, reg3;
	reg2 = reg3 = reg1;
	reg2.name = 12; reg3.name = 16;
	
	TestCase1.assign(in, Out, reg1, reg2, reg3);
	TestCase1.Run_Test(context);
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




