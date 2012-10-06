#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "brig.h"

#ifndef VALIDATE_BRIG
#define VALIDATE_BRIG

namespace validate_brig{

void validate(const BrigInstBase* ref, const BrigInstBase* get){
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

void validate(const BrigOperandReg* ref, const BrigOperandReg* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  EXPECT_EQ(ref->name, get->name);
}

void validate(const BrigOperandImmed* ref, const BrigOperandImmed* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  // Question: validating uint and float, and double and uint64. No information in type. Check!!  
  switch(ref->type){
    case Brigb1: 
      EXPECT_EQ(ref->bits.c, get->bits.c); break;
    case Brigb8: 
      EXPECT_EQ(ref->bits.c, get->bits.c); break;
    case Brigb16: 
      EXPECT_EQ(ref->bits.h, get->bits.h); break;
    case Brigb32: 
      EXPECT_EQ(ref->bits.u, get->bits.u); break;
    case Brigb64:
      EXPECT_EQ(ref->bits.l[0], get->bits.l[0]);
      EXPECT_EQ(ref->bits.l[1], get->bits.l[1]); 
      break;
  }  
}

void validate(const BrigOperandWaveSz* ref, const BrigOperandWaveSz* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);  
}
void validate(const BrigInstLdSt* ref, const BrigInstLdSt* get){
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
void validate(const BrigOperandAddress* ref, const BrigOperandAddress* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  EXPECT_EQ(ref->directive, get->directive);  

}
void validate(const BrigOperandCompound* ref, const BrigOperandCompound* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  EXPECT_EQ(ref->name, get->name);  
  EXPECT_EQ(ref->reg, get->reg);  
  EXPECT_EQ(ref->offset, get->offset);  
}

void validate(const BrigOperandIndirect* ref, const BrigOperandIndirect* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->reg, get->reg);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  EXPECT_EQ(ref->offset, get->offset);  
}

}//namespace validate_brig

#endif
