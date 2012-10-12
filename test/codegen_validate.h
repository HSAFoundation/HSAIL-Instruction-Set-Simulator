#ifndef VALIDATE_BRIG
#define VALIDATE_BRIG

#include <iostream>
#include <string>
#include "gtest/gtest.h"

namespace hsa{
namespace brig{
namespace validate_brig{

void validate(const BrigInstBase* ref, const BrigInstBase* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->packing, get->packing); 
}

void validate(const BrigInstMod* ref, const BrigInstMod* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->packing, get->packing);
  const uint32_t* ref1 = reinterpret_cast<const uint32_t* > (&(ref->aluModifier));
  const uint32_t* get1 = reinterpret_cast<const uint32_t* > (&(get->aluModifier));
  EXPECT_EQ(*ref1, *get1);
}

void validate(const BrigOperandReg* ref, const char* refstr, const BrigOperandReg* get, const char* getstr){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  EXPECT_STREQ(&refstr[ref->name], &getstr[get->name]);
}

void validate(const BrigOperandImmed* ref, const char* refstr, const BrigOperandImmed* get, const char* getstr){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  EXPECT_EQ(ref->bits.l[0], get->bits.l[0]);
  EXPECT_EQ(ref->bits.l[1], get->bits.l[1]);
}

void validate(const BrigOperandWaveSz* ref, const char* refstr, const BrigOperandWaveSz* get, const char* getstr){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);  
}
void validate(const BrigInstLdSt* ref, const BrigInstLdSt* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->packing, get->packing);
  /*EXPECT_EQ(ref->o_operands[0], get->o_operands[0]);
  EXPECT_EQ(ref->o_operands[1], get->o_operands[1]);
  EXPECT_EQ(ref->o_operands[2], get->o_operands[2]);
  EXPECT_EQ(ref->o_operands[3], get->o_operands[3]);
  EXPECT_EQ(ref->o_operands[4], get->o_operands[4]);*/
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
  //EXPECT_EQ(ref->name, get->name);  
  //EXPECT_EQ(ref->reg, get->reg);  
  EXPECT_EQ(ref->offset, get->offset);  
}

void validate(const BrigOperandIndirect* ref, const BrigOperandIndirect* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->reg, get->reg);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  //EXPECT_EQ(ref->offset, get->offset);  
}

void validate(const BrigDirectiveFunction* ref, const BrigDirectiveFunction* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->c_code, get->c_code);
  EXPECT_EQ(ref->s_name, get->s_name);
  EXPECT_EQ(ref->inParamCount, get->inParamCount);
  EXPECT_EQ(ref->d_firstScopedDirective, get->d_firstScopedDirective);
  EXPECT_EQ(ref->operationCount, get->operationCount);
  EXPECT_EQ(ref->d_nextDirective, get->d_nextDirective);
  EXPECT_EQ(ref->attribute, get->attribute);
  EXPECT_EQ(ref->fbarCount, get->fbarCount);
  EXPECT_EQ(ref->outParamCount, get->outParamCount);
  EXPECT_EQ(ref->d_firstInParam, get->d_firstInParam);
}

void validate(const BrigDirectiveSymbol* ref, const BrigDirectiveSymbol* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->s.c_code, get->s.c_code);
  EXPECT_EQ(ref->s.storageClass, get->s.storageClass);
  EXPECT_EQ(ref->s.attribute, get->s.attribute);
  EXPECT_EQ(ref->s.reserved, get->s.reserved);
  EXPECT_EQ(ref->s.symbolModifier, get->s.symbolModifier);
  EXPECT_EQ(ref->s.dim, get->s.dim);
  EXPECT_EQ(ref->s.s_name, get->s.s_name);
  EXPECT_EQ(ref->s.type, get->s.type);
  EXPECT_EQ(ref->s.align, get->s.align);
  EXPECT_EQ(ref->d_init, get->d_init);
  EXPECT_EQ(ref->reserved, get->reserved);  
}

#if 0
void validate(const BrigDirectiveSignature* ref, const BrigDirectiveSignature* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->c_code, get->c_code);
  EXPECT_EQ(ref->s_name, get->s_name);
  EXPECT_EQ(ref->fbarCount, get->fbarCount);
  EXPECT_EQ(ref->reserved, get->reserved);
  EXPECT_EQ(ref->outCount, get->outCount);
  EXPECT_EQ(ref->inCount, get->inCount);
}

#endif

}//namespace validate_brig
}
}
#endif
