#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "brig.h"

#ifndef VALIDATE_BRIG
#define VALIDATE_BRIG

namespace validate_brig{
	
	void validate(BrigInstBase* ref, BrigInstBase* get){
		EXPECT_EQ(ref->size, get->size);
		EXPECT_EQ(ref->kind, get->kind);
		EXPECT_EQ(ref->opcode, get->opcode);
		EXPECT_EQ(ref->type, get->type);
		EXPECT_EQ(ref->packing, get->packing);
		EXPECT_EQ(ref->o_operands[0], get->o_operands[0]);
		EXPECT_EQ(ref->o_operands[1], get->o_operands[1]);
		EXPECT_EQ(ref->o_operands[2], get->o_operands[2]);
		EXPECT_EQ(ref->o_operands[3], get->o_operands[3]);
		EXPECT_EQ(ref->o_operands[4], get->o_operands[4]);
	}
	
	void validate(BrigInstMod* ref, BrigInstMod* get){
		EXPECT_EQ(ref->size, get->size);
		EXPECT_EQ(ref->kind, get->kind);
		EXPECT_EQ(ref->opcode, get->opcode);
		EXPECT_EQ(ref->type, get->type);
		EXPECT_EQ(ref->packing, get->packing);
		EXPECT_EQ(ref->o_operands[0], get->o_operands[0]);
		EXPECT_EQ(ref->o_operands[1], get->o_operands[1]);
		EXPECT_EQ(ref->o_operands[2], get->o_operands[2]);
		EXPECT_EQ(ref->o_operands[3], get->o_operands[3]);
		EXPECT_EQ(ref->o_operands[4], get->o_operands[4]);
		uint32_t* ref1 = reinterpret_cast<uint32_t *> (&(ref->aluModifier));
		uint32_t* get1 = reinterpret_cast<uint32_t *> (&(get->aluModifier));
		EXPECT_EQ(*ref1, *get1);
	}
	
	void validate(BrigOperandReg* ref, BrigOperandReg* get){
		EXPECT_EQ(ref->size, get->size);
		EXPECT_EQ(ref->kind, get->kind);
		EXPECT_EQ(ref->type, get->type);
		EXPECT_EQ(ref->reserved, get->reserved);
		EXPECT_EQ(ref->name, get->name);
	}
		
	void validate(BrigOperandImmed* ref, BrigOperandImmed* get){
		EXPECT_EQ(ref->size, get->size);
		EXPECT_EQ(ref->kind, get->kind);
		EXPECT_EQ(ref->type, get->type);
		EXPECT_EQ(ref->reserved, get->reserved);
		//EXPECT_EQ(ref->bits, get->bits);
	}
	
	void validate(BrigOperandWaveSz* ref, BrigOperandWaveSz* get){
		EXPECT_EQ(ref->size, get->size);
		EXPECT_EQ(ref->kind, get->kind);	
	}
	void validate(BrigInstLdSt* ref, BrigInstLdSt* get){
		EXPECT_EQ(ref->size, get->size);
		EXPECT_EQ(ref->kind, get->kind);
		EXPECT_EQ(ref->opcode, get->opcode);
		EXPECT_EQ(ref->type, get->type);
		EXPECT_EQ(ref->packing, get->packing);
		EXPECT_EQ(ref->o_operands[0], get->o_operands[0]);
		EXPECT_EQ(ref->o_operands[1], get->o_operands[1]);
		EXPECT_EQ(ref->o_operands[2], get->o_operands[2]);
		EXPECT_EQ(ref->o_operands[3], get->o_operands[3]);
		EXPECT_EQ(ref->o_operands[4], get->o_operands[4]);
		EXPECT_EQ(ref->storageClass, get->storageClass);
		EXPECT_EQ(ref->memorySemantic, get->memorySemantic);
		EXPECT_EQ(ref->equivClass, get->equivClass);
	}
	void validate(BrigOperandAddress* ref, BrigOperandAddress* get){
		EXPECT_EQ(ref->size, get->size);
		EXPECT_EQ(ref->kind, get->kind);
		EXPECT_EQ(ref->type, get->type);
		EXPECT_EQ(ref->reserved, get->reserved);
		EXPECT_EQ(ref->directive, get->directive);	
		
	}
	void validate(BrigOperandCompound* ref, BrigOperandCompound* get){
		EXPECT_EQ(ref->size, get->size);
		EXPECT_EQ(ref->kind, get->kind);
		EXPECT_EQ(ref->type, get->type);
		EXPECT_EQ(ref->reserved, get->reserved);
		EXPECT_EQ(ref->name, get->name);	
		EXPECT_EQ(ref->reg, get->reg);	
		EXPECT_EQ(ref->offset, get->offset);	
	}
	
	void validate(BrigOperandIndirect* ref, BrigOperandIndirect* get){
		EXPECT_EQ(ref->size, get->size);
		EXPECT_EQ(ref->kind, get->kind);
		EXPECT_EQ(ref->reg, get->reg);
		EXPECT_EQ(ref->type, get->type);
		EXPECT_EQ(ref->reserved, get->reserved);
		EXPECT_EQ(ref->offset, get->offset);	
	}
}
#endif
