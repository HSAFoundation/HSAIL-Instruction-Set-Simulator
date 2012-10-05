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

template <typename T, typename T1, typename T2> class Instruction2_Test{
private:
	std::string Input;
	//Instruction in .code buffer - Pointers to brig structures
	T* Output;
	//Operands in .operands buffer
	T1* dest;
	T2* src1;
	
	//Symbols in .string buffer
	std::string dest_name;
	std::string src1_name;
		
	//Buffer offsets
	int code_start;
	int operand_start;
	int string_start;

public:
	Instruction2_Test(){
		Output = NULL;
		dest = NULL;
		src1 = NULL;
		code_start = 0;
		operand_start = 0;
		string_start = 0;
		
	}
	Instruction2_Test(std::string in, T* ref, T1* Dest, T2* Src1, 
				std::string op1, std::string op2, int offset){
		Input.assign(in);
		Output = ref;
		dest = Dest;
		src1 = Src1;
		dest_name = op1;
		src1_name = op2;
		code_start = offset; 
		operand_start = offset;
		string_start = offset;
	}
		
	void Run_Test(){
		ErrorReporter* main_reporter = ErrorReporter::get_instance();
		Context* context = Context::get_instance();
		
		context->set_error_reporter(main_reporter);
		context->clear_context();
		Lexer *lexer = new Lexer();
		lexer->set_source_string(Input);
		context->token_to_scan = lexer->get_next_token();
		int ret = Instruction2(context);
		EXPECT_EQ(0, ret);
		if(!ret){
			T getcode;
			context->get_code(code_start, &getcode);
			validate(Output, &getcode);
			
			T1 getreg1;
			int op_offset = operand_start;
			context->get_operand(op_offset, &getreg1);
			validate(dest, &getreg1);
			op_offset+=sizeof(getreg1);
			
			T2 getreg2;
			context->get_operand(op_offset, &getreg2);
			validate(src1, &getreg2);
			op_offset+=sizeof(getreg2);
		}
		context->clear_context();
		delete lexer;
	}
};
  
TEST(CodegenTest, Instruction2_CodeGen){

  std::string in; 
  in.assign( "abs_s32 $s1, $s2;\n");
  std::string op1, op2; op1.assign("$s1"); op2.assign("$s2"); 
  int buffer_start = BUFFER_OFFSET;
  int size_reg = sizeof(BrigOperandReg);
  BrigInstBase out = {
    sizeof(BrigInstBase),
    BrigEInstBase, 
    BrigAbs, 
    Brigs32,
    BrigNoPacking,
    {buffer_start, buffer_start + size_reg, 0, 0, 0}
  };
  BrigOperandReg reg1 = {
		sizeof(BrigOperandReg),
		BrigEOperandReg,
		Brigb32,
		0,
		buffer_start //Offset to string table	
	};
	BrigOperandReg reg2 = reg1; reg2.name = buffer_start + op1.size() + 1;
	Instruction2_Test<BrigInstBase, BrigOperandReg, BrigOperandReg> TestCase1(in, &out, &reg1, &reg2, op1, op2, buffer_start);
	TestCase1.Run_Test();
			
     /***********************Add mOre tests***********************************/
/* 

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
*/
}
}
}
