#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "brig.h"

template<class T> class validate_brig{
public:
	void validate(BrigInstBase Ref, BrigInstBase get){
		EXPECT_EQ(Ref.size, get.size);
		EXPECT_EQ(Ref.kind, get.kind);
		EXPECT_EQ(Ref.opcode, get.opcode);
		EXPECT_EQ(Ref.type, get.type);
		EXPECT_EQ(Ref.packing, get.packing);
		EXPECT_EQ(Ref.o_operands[0], get.o_operands[0]);
		EXPECT_EQ(Ref.o_operands[1], get.o_operands[1]);
		EXPECT_EQ(Ref.o_operands[2], get.o_operands[2]);
		EXPECT_EQ(Ref.o_operands[3], get.o_operands[3]);
		EXPECT_EQ(Ref.o_operands[4], get.o_operands[4]);
	}
	
	void validate(BrigInstMod Ref, BrigInstMod get){
		EXPECT_EQ(Ref.size, get.size);
		EXPECT_EQ(Ref.kind, get.kind);
		EXPECT_EQ(Ref.opcode, get.opcode);
		EXPECT_EQ(Ref.type, get.type);
		EXPECT_EQ(Ref.packing, get.packing);
		EXPECT_EQ(Ref.o_operands[0], get.o_operands[0]);
		EXPECT_EQ(Ref.o_operands[1], get.o_operands[1]);
		EXPECT_EQ(Ref.o_operands[2], get.o_operands[2]);
		EXPECT_EQ(Ref.o_operands[3], get.o_operands[3]);
		EXPECT_EQ(Ref.o_operands[4], get.o_operands[4]);
		EXPECT_EQ(Ref.aluModifier, get.aluModifier);
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
	
	void validate(BrigOperandWaveSz ref, BrigOperandWaveSz get){
		EXPECT_EQ(ref.size, get.size);
		EXPECT_EQ(ref.kind, get.kind);	
	}
};

