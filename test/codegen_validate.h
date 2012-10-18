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
  //ignore o_operands
}

void validate(const BrigInstMod* ref, const BrigInstMod* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->packing, get->packing);
  //ignore o_operands
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

void validate(const BrigOperandRegV2* ref, const BrigOperandRegV2* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);  
  //ignore regs
}

void validate(const BrigOperandRegV4* ref, const BrigOperandRegV4* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);  
  //ignore regs
}

void validate(const BrigOperandImmed* ref, const BrigOperandImmed* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  EXPECT_EQ(ref->bits.l[0], get->bits.l[0]);
  EXPECT_EQ(ref->bits.l[1], get->bits.l[1]);
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
  //ignore o_operands
  EXPECT_EQ(ref->storageClass, get->storageClass);
  EXPECT_EQ(ref->memorySemantic, get->memorySemantic);
  EXPECT_EQ(ref->equivClass, get->equivClass);
}

void validate(const BrigOperandAddress* ref, const BrigOperandAddress* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  //ignore directive
}

void validate(const BrigOperandCompound* ref, const BrigOperandCompound* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  //ignore name and reg
  EXPECT_EQ(ref->offset, get->offset);  
}

void validate(const BrigOperandIndirect* ref, const BrigOperandIndirect* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  //ignore reg
  EXPECT_EQ(ref->reserved, get->reserved);
  EXPECT_EQ(ref->offset, get->offset);  
}

void validate(const BrigDirectiveFunction* ref, const char* refbuf, const BrigDirectiveFunction* get, const char* getbuf){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  //EXPECT_EQ(ref->c_code, get->c_code);
  EXPECT_STREQ(&refbuf[ref->s_name], &getbuf[get->s_name]);
  EXPECT_EQ(ref->inParamCount, get->inParamCount);
  //EXPECT_EQ(ref->d_firstScopedDirective, get->d_firstScopedDirective);
  EXPECT_EQ(ref->operationCount, get->operationCount);
  //EXPECT_EQ(ref->d_nextDirective, get->d_nextDirective);
  EXPECT_EQ(ref->attribute, get->attribute);
  EXPECT_EQ(ref->fbarCount, get->fbarCount);
  EXPECT_EQ(ref->outParamCount, get->outParamCount);
  //EXPECT_EQ(ref->d_firstInParam, get->d_firstInParam);
}

void validate(const BrigDirectiveSymbol* ref, const char* refstr, const BrigDirectiveSymbol* get, const char* getstr){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  //EXPECT_EQ(ref->s.c_code, get->s.c_code);
  EXPECT_EQ(ref->s.storageClass, get->s.storageClass);
  EXPECT_EQ(ref->s.attribute, get->s.attribute);
  EXPECT_EQ(ref->s.reserved, get->s.reserved);
  EXPECT_EQ(ref->s.symbolModifier, get->s.symbolModifier);
  EXPECT_EQ(ref->s.dim, get->s.dim);
  EXPECT_STREQ(&refstr[ref->s.s_name], &getstr[get->s.s_name]);
  EXPECT_EQ(ref->s.type, get->s.type);
  EXPECT_EQ(ref->s.align, get->s.align);
  //EXPECT_EQ(ref->d_init, get->d_init);
  EXPECT_EQ(ref->reserved, get->reserved);  
}

template <typename T> void validateOpType(const T* ref, const char* refstr, const T* get, const char* getstr){
  validate(ref, get);
}

template <> void validateOpType <BrigOperandReg>(const BrigOperandReg* ref, const char* refstr, const BrigOperandReg* get, const char* getstr){
  validate(ref, refstr, get, getstr);
}

void validate(const BrigDirectiveSignature* ref, const char* refbuf, const BrigDirectiveSignature* get, const char* getbuf){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  //EXPECT_EQ(ref->c_code, get->c_code);
  EXPECT_STREQ(&refbuf[ref->s_name], &getbuf[get->s_name]);
  EXPECT_EQ(ref->fbarCount, get->fbarCount);
  EXPECT_EQ(ref->reserved, get->reserved);
  EXPECT_EQ(ref->outCount, get->outCount);
  EXPECT_EQ(ref->inCount, get->inCount);
}

void validate(const BrigDirectiveInit* ref, const BrigDirectiveInit* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  //EXPECT_EQ(ref->c_code, get->c_code);
  EXPECT_EQ(ref->elementCount, get->elementCount);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  int no_init_bytes = ref->size - sizeof(BrigDirectiveInit) + sizeof(uint64_t);
  for(int i=0; i < no_init_bytes; i++)
    EXPECT_EQ(ref->initializationData.u8[i], get->initializationData.u8[i]);
}

void validate(const BrigInstMem* ref, const BrigInstMem* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->packing, get->packing);
  //ignore operands
  EXPECT_EQ(ref->storageClass, get->storageClass);
}

void validate(const BrigInstCvt* ref, const BrigInstCvt* get){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->packing, get->packing);
  //ignore o_operands
  const uint32_t* ref1 = reinterpret_cast<const uint32_t* > (&(ref->aluModifier));
  const uint32_t* get1 = reinterpret_cast<const uint32_t* > (&(get->aluModifier));
  EXPECT_EQ(*ref1, *get1);
  EXPECT_EQ(ref->stype, get->stype);
  EXPECT_EQ(ref->reserved, get->reserved); 
}

void validate(const BrigDirectiveImage* ref, const char* refstr, const BrigDirectiveImage* get, const char* getstr){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  //EXPECT_EQ(ref->s.c_code, get->s.c_code);
  EXPECT_EQ(ref->s.storageClass, get->s.storageClass);
  EXPECT_EQ(ref->s.attribute, get->s.attribute);
  EXPECT_EQ(ref->s.reserved, get->s.reserved);
  EXPECT_EQ(ref->s.symbolModifier, get->s.symbolModifier);
  EXPECT_EQ(ref->s.dim, get->s.dim);
  EXPECT_STREQ(&refstr[ref->s.s_name], &getstr[get->s.s_name]);
  EXPECT_EQ(ref->s.type, get->s.type);
  EXPECT_EQ(ref->s.align, get->s.align);
  EXPECT_EQ(ref->width, get->width);  
  EXPECT_EQ(ref->height, get->height);  
  EXPECT_EQ(ref->depth, get->depth);
  EXPECT_EQ(ref->array, get->array);    
  EXPECT_EQ(ref->order, get->order);  
  EXPECT_EQ(ref->format, get->format);  
}

void validate(const BrigDirectiveSampler* ref, const char* refstr, const BrigDirectiveSampler* get, const char* getstr){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  //EXPECT_EQ(ref->s.c_code, get->s.c_code);
  EXPECT_EQ(ref->s.storageClass, get->s.storageClass);
  EXPECT_EQ(ref->s.attribute, get->s.attribute);
  EXPECT_EQ(ref->s.reserved, get->s.reserved);
  EXPECT_EQ(ref->s.symbolModifier, get->s.symbolModifier);
  EXPECT_EQ(ref->s.dim, get->s.dim);
  EXPECT_STREQ(&refstr[ref->s.s_name], &getstr[get->s.s_name]);
  EXPECT_EQ(ref->s.type, get->s.type);
  EXPECT_EQ(ref->s.align, get->s.align);
  EXPECT_EQ(ref->valid, get->valid);  
  EXPECT_EQ(ref->normalized, get->normalized);  
  EXPECT_EQ(ref->filter, get->filter);
  EXPECT_EQ(ref->boundaryU, get->boundaryU);    
  EXPECT_EQ(ref->boundaryV, get->boundaryV);  
  EXPECT_EQ(ref->boundaryW, get->boundaryW);  
  EXPECT_EQ(ref->reserved1, get->reserved1);  
}

void validate(const BrigOperandLabelRef* ref, const BrigOperandLabelRef* get){
  
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  //EXPECT_EQ(ref->labeldirective, get->labeldirective);
}

void validate(const BrigOperandFunctionRef* ref, const BrigOperandFunctionRef* get){
  
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  //EXPECT_EQ(ref->fn, get->fn);
}

void validate(const BrigInstCmp* ref, const BrigInstCmp* get) {
  
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->packing, get->packing);
  // ignore o_operands
  const uint32_t* ref1 = reinterpret_cast<const uint32_t* > (&(ref->aluModifier));
  const uint32_t* get1 = reinterpret_cast<const uint32_t* > (&(get->aluModifier));
  EXPECT_EQ(*ref1, *get1);
  EXPECT_EQ(ref->comparisonOperator, get->comparisonOperator);
  EXPECT_EQ(ref->sourceType, get->sourceType);
  EXPECT_EQ(ref->reserved, get->reserved);

}

void validate(const BrigOperandArgumentList* ref, const BrigOperandArgumentList* get){
  
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->elementCount, get->elementCount);
  //ignore o_operands
}

void validate(const BrigOperandArgumentRef* ref, const BrigOperandArgumentRef* get){
  
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  //ignore o_operands
}

void validate(const BrigInstAtomic* ref, const BrigInstAtomic* get) {
  
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->packing, get->packing);
  // ignore o_operands
  EXPECT_EQ(ref->atomicOperation, get->atomicOperation);
  EXPECT_EQ(ref->storageClass, get->storageClass);
  EXPECT_EQ(ref->memorySemantic, get->memorySemantic);
}

}//namespace validate_brig
}
}
#endif

