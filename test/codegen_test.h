#include <iostream>
#include "gtest/gtest.h"
#include "tokens.h"
#include "lexer.h"
#include "parser.h"
#include "brig.h"
#include "context.h"

class Instruction3Opcode{
public:
	std::string Input;
	BrigInstBase Output;
	void validate(BrigInstBase get){
		EXPECT_EQ(Output.opcode, get.opcode);
		EXPECT_EQ(Output.packing, get.packing);
		EXPECT_EQ(Output.type, get.type);
		EXPECT_EQ(Output.o_operands[0], get.o_operands[0]);
		EXPECT_EQ(Output.o_operands[1], get.o_operands[1]);
	}
	void init(std::string input, BrigInstBase ref){
		(this->Input).assign(input);
		this->Output = ref;
	}
};

Instruction3Opcode TestCase_Instr3Opcode[20]; 

void Init_Instruction3TestCases(){

	std::string in; 
	in.assign( "add_pp_sat_u16x2 $s1, $s0, $s3; \n");
	BrigInstBase out = {
		32,
		BrigEInstBase, 
		BrigAdd, 
		Brigu16x2,
		BrigPackPPsat,
		{8, 20, 32, 0, 0}
	};	
	TestCase_Instr3Opcode[0].init(in, out);		
	
	in.assign( "add_s64 $d1, $d2, $d3;\n");
	BrigInstBase out1 = {
		32,
		BrigEInstBase, 
		BrigAdd, 
		Brigs64,
		BrigNoPacking,
		{8, 20, 32, 0, 0}
	};
	TestCase_Instr3Opcode[1].init(in, out1);		
	
	in.assign( "add_u64 $d1, WAVESIZE, WAVESIZE;\n");
	BrigInstBase out2 = {
		32,
		BrigEInstBase, 
		BrigAdd, 
		Brigu64,
		BrigNoPacking,
		{8, 20, 24, 0, 0}
	};
	TestCase_Instr3Opcode[2].init(in, out2);

	in.assign( "add_ps_sat_s16x4 $d1, 0x40, WAVESIZE;\n");
	BrigInstBase out3 = {
		32,
		BrigEInstBase, 
		BrigAdd, 
		Brigs16x4,
		BrigPackPSsat,
		{8, 24, 36, 0, 0}
	};
	TestCase_Instr3Opcode[3].init(in, out3);

	in.assign( "div_s32 $s1, 100, 10;\n");
	BrigInstBase out4 = {
		32,
		BrigEInstBase, 
		BrigDiv, 
		Brigs32,
		BrigNoPacking,
		{8, 24, 40, 0, 0}
	};
	TestCase_Instr3Opcode[4].init(in, out4);

	in.assign( "div_u64 $d1, $d3, 0x233412349456;\n");
	BrigInstBase out5 = {
		32,
		BrigEInstBase, 
		BrigDiv, 
		Brigu64,
		BrigNoPacking,
		{8, 20, 32, 0, 0}
	};
	TestCase_Instr3Opcode[5].init(in, out5);	
	
	in.assign( "rem_s64 $d1, $d3, WAVESIZE;\n");
	BrigInstBase out6 = {
		32,
		BrigEInstBase, 
		BrigRem, 
		Brigs64,
		BrigNoPacking,
		{8, 20, 32, 0, 0}
	};
	TestCase_Instr3Opcode[6].init(in, out6);
	
	in.assign( "rem_u64 $d1, 0x040, 0x233412349456;\n");
	BrigInstBase out7 = {
		32,
		BrigEInstBase, 
		BrigRem, 
		Brigu64,
		BrigNoPacking,
		{8, 24, 36, 0, 0}
	};
	TestCase_Instr3Opcode[7].init(in, out7);
	
	in.assign( "shl_u32 $s1, $s1, 2;\n");
	BrigInstBase out8 = {
		32,
		BrigEInstBase, 
		BrigShl, 
		Brigu32,
		BrigNoPacking,
		{8, 8, 20, 0, 0}
	};
	TestCase_Instr3Opcode[8].init(in, out8);
	
	
}