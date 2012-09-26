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
		EXPECT_EQ(Output.o_operands[2], get.o_operands[2]);
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
	
	in.assign( "add_u64 $d1, WAVESIZE, $d3;\n");
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
		{8, 24, 24 + sizeof(BrigOperandImmed), 0, 0}
	};
	TestCase_Instr3Opcode[3].init(in, out3);

	in.assign( "div_s32 $s1, 100, 10;\n");
	BrigInstBase out4 = {
		32,
		BrigEInstBase, 
		BrigDiv, 
		Brigs32,
		BrigNoPacking,
		{8, 24, 24 + sizeof(BrigOperandImmed), 0, 0}
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
		{8, 24, 48, 0, 0}
	};
	TestCase_Instr3Opcode[7].init(in, out7);
	
	in.assign( "shl_u32 $s1, $s1, 2;\n");
	BrigInstBase out8 = {
		32,
		BrigEInstBase, 
		BrigShl, 
		Brigu32,
		BrigNoPacking,
		{8, 8, 24, 0, 0}
	};
	TestCase_Instr3Opcode[8].init(in, out8);
	
	
}

class Inst2TestCase {
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
  static unsigned int numCases;
};
unsigned int Inst2TestCase::numCases = 0;
Inst2TestCase testInst2[100];

void Init_Instruction2TestCases(){

  std::string in; 
	
  in.assign( "abs_s32 $s1, $s2;\n");
  BrigInstBase out1 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out1);

  in.assign( "abs_s64 $d1, $d2;\n");
  BrigInstBase out2 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out2);

  in.assign( "abs_f32 $s1, $s2;\n");
  BrigInstBase out3 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigf32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out3);

  in.assign( "abs_f64 $d1,$d2;\n");
  BrigInstBase out4 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigf64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out4);

  in.assign( "abs_p_s8x4 $s1, $s2;\n");
  BrigInstBase out5 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs8x4,
    BrigPackP,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out5);

  in.assign( "abs_p_f32x2 $d1, $d1;\n");
  BrigInstBase out6 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigf32x2,
    BrigPackP,
    {8, 8, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out6);  

  in.assign( "abs_s_s8x4 $s1, $s2;\n");
  BrigInstBase out7 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs8x4,
    BrigPackS,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out7);  
  
  in.assign( "abs_p_s8x8 $d1, $d2;\n");
  BrigInstBase out8 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs8x8,
    BrigPackP,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out8);  

  in.assign( "abs_s_s8x8 $d1, $d2;\n");
  BrigInstBase out9 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs8x8,
    BrigPackS,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out9);  

  in.assign( "abs_p_s16x2 $s1, $s2;\n");
  BrigInstBase out10 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs16x2,
    BrigPackP,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out10);  

  in.assign( "abs_s_s16x2 $s1, $s2;\n");
  BrigInstBase out11 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs16x2,
    BrigPackS,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out11);  


  in.assign( "neg_s32 $s1, 100;\n");
  BrigInstBase out12 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigs32,
    BrigNoPacking,
    {8, 24, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out12);  

  in.assign( "neg_s64 $d1, $d2;\n");
  BrigInstBase out13 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigs64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out13);  

  in.assign( "neg_f32 $s3,1.0f;\n");
  BrigInstBase out14 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigf32,
    BrigNoPacking,
    {8, 24, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out14); 

  in.assign( "neg_f64 $d3,1.0;\n");
  BrigInstBase out15 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigf64,
    BrigNoPacking,
    {8, 24, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out15); 

  in.assign( "neg_p_f16x2 $s1, $s2;\n");
  BrigInstBase out16 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigf16x2,
    BrigPackP,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out16); 

  in.assign( "neg_s_u8x4 $s1, $s2;\n");
  BrigInstBase out17 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigu8x4,
    BrigPackS,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out17); 

  in.assign( "neg_p_s32x2 $d1, $d2;\n");
  BrigInstBase out18 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNeg, 
    Brigs32x2,
    BrigPackP,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out18); 

  in.assign( "not_b1 $c1, $c2;\n");
  BrigInstBase out19 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNot, 
    Brigb1,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out19); 

  in.assign( "not_b32 $s0, $s2;\n");
  BrigInstBase out20 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNot, 
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out20); 

  in.assign( "not_b64 $d0, $d1;\n");
  BrigInstBase out21 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigNot, 
    Brigb64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out21); 

  in.assign( "popcount_b32 $s1, $s2;\n");
  BrigInstBase out22 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigPopcount, 
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out22); 

  in.assign( "popcount_b64 $s1, $d2;\n");
  BrigInstBase out23 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigPopcount, 
    Brigb64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out23); 

  in.assign( "bitrev_s32 $s1, $s2;\n");
  BrigInstBase out24 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigBitRev, 
    Brigs32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out24); 
 
  in.assign( "bitrev_u64 $d1, 0x234;\n");
  BrigInstBase out25 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigBitRev, 
    Brigu64,
    BrigNoPacking,
    {8, 24, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out25); 
  
  in.assign( "firstbit_s32 $s0, $s0;\n");
  BrigInstBase out26 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFirstbit, 
    Brigs32,
    BrigNoPacking,
    {8, 8, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out26); 

  in.assign( "firstbit_u64 $s0, $d6;\n");
  BrigInstBase out27 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFirstbit, 
    Brigu64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out27);

  in.assign( "lastbit_u32 $s0, $s0;\n");
  BrigInstBase out28 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigLastbit, 
    Brigu32,
    BrigNoPacking,
    {8, 8, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out28);

  in.assign( "lastbit_s64 $s0, $d6;\n");
  BrigInstBase out29 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigLastbit, 
    Brigs64,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out29);

  in.assign( "movs_lo_b32 $s1, $d1;\n");
  BrigInstBase out30 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigMovsLo, 
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out30);

  in.assign( "movs_hi_b32 $s1, $d1;\n");
  BrigInstBase out31 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigMovsHi, 
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out31);

  in.assign( "fract_f32 $s0, 3.2f;\n");
  BrigInstBase out32 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFract, 
    Brigf32,
    BrigNoPacking,
    {8, 24, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out32);

  in.assign( "fcos_f32 $s1, $s0;\n");
  BrigInstBase out33 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFcos, 
    Brigf32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out33);

  in.assign( "fsin_f32 $s1, $s0;\n");
  BrigInstBase out34 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFsin, 
    Brigf32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out34);

  in.assign( "flog2_f32 $s1, $s0;\n");
  BrigInstBase out35 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFlog2, 
    Brigf32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out35);

  in.assign( "fexp2_f32 $s1, $s0;\n");
  BrigInstBase out36 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFexp2, 
    Brigf32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out36);

  in.assign( "frsqrt_f32 $s1, $s0;\n");
  BrigInstBase out37 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFrsqrt, 
    Brigf32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out37);

  in.assign( "frcp_f32 $s1, $s0;\n");
  BrigInstBase out38 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigFrcp, 
    Brigf32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out38);

  in.assign( "sqrt_f64 $d1, 1.21;\n");
  BrigInstBase out39 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigSqrt, 
    Brigf64,
    BrigNoPacking,
    {8, 24, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out39);

  in.assign( "unpack3 $s1, $s2;\n");
  BrigInstBase out40 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigUnpack3, 
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out40);

  in.assign( "unpack2 $s1, $s2;\n");
  BrigInstBase out41 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigUnpack2, 
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out41);

  in.assign( "unpack1 $s1, $s2;\n");
  BrigInstBase out42 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigUnpack1, 
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out42);

  in.assign( "unpack0 $s1, $s2;\n");
  BrigInstBase out43 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigUnpack0, 
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out43);

  in.assign( "workitemaid $s1, $s2;\n");
  BrigInstBase out44 = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigWorkItemAId, 
    Brigb32,
    BrigNoPacking,
    {8, 20, 0, 0, 0}
  };
  testInst2[Inst2TestCase::numCases++].init(in, out44);
}




