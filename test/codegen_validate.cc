#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "codegen_validate.h"

namespace hsa{
namespace brig{
namespace validate_brig{

/**************************** Instructions *********************************************/

void validate(const BrigInstBase* ref, const BrigInstBase* get, 
      BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->packing, get->packing);    
  for(int i=0; i<5; i++){
    if((ref->o_operands[i]!=0) && (get->o_operands[i]!=0)){
      oper_iterator refoper(RefOutput->operands + ref->o_operands[i]);
      oper_iterator getoper(GetOutput->operands + get->o_operands[i]);
      ASSERT_EQ(refoper->size, getoper->size);
      ASSERT_EQ(refoper->kind, getoper->kind);
      switch(getoper->kind){
        caseOperBrig(OperandReg);
        caseOperBrig(OperandImmed);
        caseOperBrig(OperandWaveSz);
        default:
          printf("Offset to invalid operand");
          exit(1);
      }
    }
  }
}

void validate(const BrigInstMod* ref, const BrigInstMod* get, 
      BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->packing, get->packing);
  for(int i=0; i<5; i++){
    if((ref->o_operands[i]!=0) && (get->o_operands[i]!=0)){
      oper_iterator refoper(RefOutput->operands + ref->o_operands[i]);
      oper_iterator getoper(GetOutput->operands + get->o_operands[i]);
      ASSERT_EQ(refoper->size, getoper->size);
      ASSERT_EQ(refoper->kind, getoper->kind);
      switch(getoper->kind){
        caseOperBrig(OperandReg);
        caseOperBrig(OperandImmed);
        caseOperBrig(OperandWaveSz);
        default:
          printf("Offset to invalid operand");
          exit(1);
      }
    }
  }
  const uint32_t* ref1 = reinterpret_cast<const uint32_t* > (&(ref->aluModifier));
  const uint32_t* get1 = reinterpret_cast<const uint32_t* > (&(get->aluModifier));
  EXPECT_EQ(*ref1, *get1);
}

void validate(const BrigInstCvt* ref, const BrigInstCvt* get, 
      BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->packing, get->packing);
  for(int i=0; i<5; i++){
    if((ref->o_operands[i]!=0) && (get->o_operands[i]!=0)){
      oper_iterator refoper(RefOutput->operands + ref->o_operands[i]);
      oper_iterator getoper(GetOutput->operands + get->o_operands[i]);
      ASSERT_EQ(refoper->size, getoper->size);
      ASSERT_EQ(refoper->kind, getoper->kind);
      switch(getoper->kind){
        caseOperBrig(OperandReg);
        caseOperBrig(OperandImmed);
        caseOperBrig(OperandWaveSz);
        default:
          printf("Offset to invalid operand");
          exit(1);
      }
    }
  }
  const uint32_t* ref1 = reinterpret_cast<const uint32_t* > (&(ref->aluModifier));
  const uint32_t* get1 = reinterpret_cast<const uint32_t* > (&(get->aluModifier));
  EXPECT_EQ(*ref1, *get1);
  EXPECT_EQ(ref->stype, get->stype);
  EXPECT_EQ(ref->reserved, get->reserved);
}

void validate(const BrigInstRead* ref, const BrigInstRead*get, 
      BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->stype, get->stype);
  EXPECT_EQ(ref->packing, get->packing);
  for(int i=0; i<5; i++){
    if((ref->o_operands[i]!=0) && (get->o_operands[i]!=0)){
      oper_iterator refoper(RefOutput->operands + ref->o_operands[i]);
      oper_iterator getoper(GetOutput->operands + get->o_operands[i]);
      ASSERT_EQ(refoper->size, getoper->size);
      ASSERT_EQ(refoper->kind, getoper->kind);
      switch(getoper->kind){
        caseOperBrig(OperandReg);
        caseOperBrig(OperandImmed);
        caseOperBrig(OperandWaveSz);
        default:
          printf("Offset to invalid operand");
          exit(1);
      }
    }
  }
  EXPECT_EQ(ref->geom, get->geom);
  EXPECT_EQ(ref->reserved, get->reserved);      
}      

void validate(const BrigInstBar* ref, const BrigInstBar* get, 
      BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->packing, get->packing);
  for(int i=0; i<5; i++){
    if((ref->o_operands[i]!=0) && (get->o_operands[i]!=0)){
      oper_iterator refoper(RefOutput->operands + ref->o_operands[i]);
      oper_iterator getoper(GetOutput->operands + get->o_operands[i]);
      ASSERT_EQ(refoper->size, getoper->size);
      ASSERT_EQ(refoper->kind, getoper->kind);
      switch(getoper->kind){
        caseOperBrig(OperandReg);
        caseOperBrig(OperandImmed);
        caseOperBrig(OperandWaveSz);
        default:
          printf("Offset to invalid operand");
          exit(1);
      }
    }
  }
  EXPECT_EQ(ref->syncFlags, get->syncFlags);
}

void validate(const BrigInstLdSt* ref, const BrigInstLdSt* get, 
      BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->packing, get->packing);
  for(int i=0; i<5; i++){
    if((ref->o_operands[i]!=0) && (get->o_operands[i]!=0)){
      oper_iterator refoper(RefOutput->operands + ref->o_operands[i]);
      oper_iterator getoper(GetOutput->operands + get->o_operands[i]);
      ASSERT_EQ(refoper->size, getoper->size);
      ASSERT_EQ(refoper->kind, getoper->kind);
      switch(getoper->kind){
        caseOperBrig(OperandReg);
        caseOperBrig(OperandImmed);
        caseOperBrig(OperandWaveSz);
        default:
          printf("Offset to invalid operand");
          exit(1);
      }
    }
  }
  EXPECT_EQ(ref->storageClass, get->storageClass);
  EXPECT_EQ(ref->memorySemantic, get->memorySemantic);
  EXPECT_EQ(ref->equivClass, get->equivClass);
}

void validate(const BrigInstCmp* ref, const BrigInstCmp* get, 
      BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->packing, get->packing);
  for(int i=0; i<5; i++){
    if((ref->o_operands[i]!=0) && (get->o_operands[i]!=0)){
      oper_iterator refoper(RefOutput->operands + ref->o_operands[i]);
      oper_iterator getoper(GetOutput->operands + get->o_operands[i]);
      ASSERT_EQ(refoper->size, getoper->size);
      ASSERT_EQ(refoper->kind, getoper->kind);
      switch(getoper->kind){
        caseOperBrig(OperandReg);
        caseOperBrig(OperandImmed);
        caseOperBrig(OperandWaveSz);
        default:
          printf("Offset to invalid operand");
          exit(1);
      }
    }
  }
  const uint32_t* ref1 = reinterpret_cast<const uint32_t* > (&(ref->aluModifier));
  const uint32_t* get1 = reinterpret_cast<const uint32_t* > (&(get->aluModifier));
  EXPECT_EQ(*ref1, *get1);
  EXPECT_EQ(ref->comparisonOperator, get->comparisonOperator);
  EXPECT_EQ(ref->sourceType, get->sourceType);
  EXPECT_EQ(ref->reserved, get->reserved);

}

void validate(const BrigInstMem* ref, const BrigInstMem* get, 
      BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->packing, get->packing);
  for(int i=0; i<5; i++){
    if((ref->o_operands[i]!=0) && (get->o_operands[i]!=0)){
      oper_iterator refoper(RefOutput->operands + ref->o_operands[i]);
      oper_iterator getoper(GetOutput->operands + get->o_operands[i]);
      ASSERT_EQ(refoper->size, getoper->size);
      ASSERT_EQ(refoper->kind, getoper->kind);
      switch(getoper->kind){
        caseOperBrig(OperandReg);
        caseOperBrig(OperandImmed);
        caseOperBrig(OperandWaveSz);
        default:
          printf("Offset to invalid operand");
          exit(1);
      }
    }
  }
  EXPECT_EQ(ref->storageClass, get->storageClass);
}

void validate(const BrigInstAtomic* ref, const BrigInstAtomic* get, 
      BrigSections* RefOutput, BrigSections* GetOutput){

  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->packing, get->packing);
  for(int i=0; i<5; i++){
    if((ref->o_operands[i]!=0) && (get->o_operands[i]!=0)){
      oper_iterator refoper(RefOutput->operands + ref->o_operands[i]);
      oper_iterator getoper(GetOutput->operands + get->o_operands[i]);
      ASSERT_EQ(refoper->size, getoper->size);
      ASSERT_EQ(refoper->kind, getoper->kind);
      switch(getoper->kind){
        caseOperBrig(OperandReg);
        caseOperBrig(OperandImmed);
        caseOperBrig(OperandWaveSz);
        default:
          printf("Offset to invalid operand");
          exit(1);
      }
    }
  }
  EXPECT_EQ(ref->atomicOperation, get->atomicOperation);
  EXPECT_EQ(ref->storageClass, get->storageClass);
  EXPECT_EQ(ref->memorySemantic, get->memorySemantic);
}

void validate(const BrigInstAtomicImage* ref, const BrigInstAtomicImage* get, 
      BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->packing, get->packing);
  for(int i=0; i<5; i++){
    if((ref->o_operands[i]!=0) && (get->o_operands[i]!=0)){
      oper_iterator refoper(RefOutput->operands + ref->o_operands[i]);
      oper_iterator getoper(GetOutput->operands + get->o_operands[i]);
      ASSERT_EQ(refoper->size, getoper->size);
      ASSERT_EQ(refoper->kind, getoper->kind);
      switch(getoper->kind){
        caseOperBrig(OperandReg);
        caseOperBrig(OperandImmed);
        caseOperBrig(OperandOpaque);
        caseOperBrig(OperandWaveSz);
        caseOperBrig(OperandRegV4);
        caseOperBrig(OperandRegV2);
        default:
          printf("Offset to invalid operand");
          exit(1);
      }
    }
  }
  EXPECT_EQ(ref->atomicOperation, get->atomicOperation);
  EXPECT_EQ(ref->storageClass, get->storageClass);
  EXPECT_EQ(ref->memorySemantic, get->memorySemantic);
  EXPECT_EQ(ref->geom, get->geom);

}

void validate(const BrigInstImage* ref, const BrigInstImage* get, 
      BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->opcode, get->opcode);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->stype, get->stype);
  EXPECT_EQ(ref->packing, get->packing);
  EXPECT_EQ(ref->reserved, get->reserved);      
  for(int i=0; i<5; i++){
    if((ref->o_operands[i]!=0) && (get->o_operands[i]!=0)){
      oper_iterator refoper(RefOutput->operands + ref->o_operands[i]);
      oper_iterator getoper(GetOutput->operands + get->o_operands[i]);
      ASSERT_EQ(refoper->size, getoper->size);
      ASSERT_EQ(refoper->kind, getoper->kind);
      switch(getoper->kind){
        caseOperBrig(OperandReg);
        caseOperBrig(OperandImmed);
        caseOperBrig(OperandWaveSz);
        default:
          printf("Offset to invalid operand");
          exit(1);
      }
    }
  }
  EXPECT_EQ(ref->geom, get->geom);  
}

/********************************************* Operands ***********************************************************/

void validate(const BrigOperandBase* ref, const BrigOperandBase* get, 
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
}

void validate(const BrigOperandReg* ref, const BrigOperandReg* get, 
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  EXPECT_STREQ(&(RefOutput->strings[ref->s_name]), &(GetOutput->strings[get->s_name]));
}

void validate(const BrigOperandImmed* ref, const BrigOperandImmed* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  EXPECT_EQ(ref->bits.l[0], get->bits.l[0]);
  EXPECT_EQ(ref->bits.l[1], get->bits.l[1]);
}

void validate(const BrigOperandRegV2* ref, const BrigOperandRegV2* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  for(int i=0; i<2; i++){
    oper_iterator refoper(RefOutput->operands + ref->regs[i]);
    oper_iterator getoper(GetOutput->operands + get->regs[i]);
    switch(getoper->kind){
      caseOperBrig(OperandReg);
      default:
        printf("Offset to invalid operand");
        exit(1);
    }
  }
}

void validate(const BrigOperandRegV4* ref, const BrigOperandRegV4* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  for(int i=0; i<4; i++){
    oper_iterator refoper(RefOutput->operands + ref->regs[i]);
    oper_iterator getoper(GetOutput->operands + get->regs[i]);
    switch(getoper->kind){
      caseOperBrig(OperandReg);
      default:
        printf("Offset to invalid operand");
        exit(1);
    }
  }
}

void validate(const BrigOperandAddress* ref, const BrigOperandAddress* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  dir_iterator refdir(RefOutput->directives + ref->directive);
  dir_iterator getdir(GetOutput->directives + get->directive);
  switch(getdir->kind){
    caseDirBrig(DirectiveSymbol);
    default:
      printf("Offset to invalid directive");
      exit(1);
  }
}

void validate(const BrigOperandLabelRef* ref, const BrigOperandLabelRef* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  dir_iterator refdir(RefOutput->directives + ref->labeldirective);
  dir_iterator getdir(GetOutput->directives + get->labeldirective);
  switch(getdir->kind){
    caseDirBrig(DirectiveLabel);
    caseDirBrig(DirectiveLabelList);
    default:
      printf("Offset to invalid directive");
      exit(1);
  }
}

void validate(const BrigOperandIndirect* ref, const BrigOperandIndirect* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  if((ref->reg!=0) && (get->reg!=0)){
    oper_iterator refoper(RefOutput->operands + ref->reg);
    oper_iterator getoper(GetOutput->operands + get->reg);
    if(getoper->kind == BrigEOperandReg)
      validate_brig::validate((cast<BrigOperandReg>(refoper)), (cast<BrigOperandReg>(getoper)),
      RefOutput, GetOutput); 
    else{
      printf("Offset to invalid operand");
      exit(1);
    }
  }
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  EXPECT_EQ(ref->offset, get->offset);
}

void validate(const BrigOperandCompound* ref, const BrigOperandCompound* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  oper_iterator refoper(RefOutput->operands + ref->name);
  oper_iterator getoper(GetOutput->operands + get->name);
  if(getoper->kind == BrigEOperandAddress)
    validate_brig::validate((cast<BrigOperandAddress>(refoper)), (cast<BrigOperandAddress>(getoper)),
      RefOutput, GetOutput); 
  else{
    printf("Offset to invalid operand");
    exit(1);
  }
  if((ref->reg!=0) && (get->reg!=0)){
    oper_iterator refoper1(RefOutput->operands + ref->reg);
    oper_iterator getoper1(GetOutput->operands + get->reg);
    if(getoper1->kind == BrigEOperandReg)
      validate_brig::validate((cast<BrigOperandReg>(refoper1)), (cast<BrigOperandReg>(getoper1)),
      RefOutput, GetOutput); 
    else{
      printf("Offset to invalid operand");
      exit(1);
    }
  }
  EXPECT_EQ(ref->offset, get->offset);
}

void validate(const BrigOperandArgumentList* ref, const BrigOperandArgumentList* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->elementCount, get->elementCount);
  if(get->kind==BrigEOperandArgumentList){
    for(unsigned int i=0; i<ref->elementCount; i++){
      oper_iterator refoper(RefOutput->operands + ref->o_args[i]);
      oper_iterator getoper(GetOutput->operands + get->o_args[i]);
      if(getoper->kind == BrigEOperandArgumentRef)
        validate_brig::validate((cast<BrigOperandArgumentRef>(refoper)), (cast<BrigOperandArgumentRef>(getoper)),
        RefOutput, GetOutput); 
      else{
        printf("Offset to invalid operand");
        exit(1);
      }
    }
  } else if(get->kind==BrigEOperandFunctionList){
    for(unsigned int i=0; i<ref->elementCount; i++){
      oper_iterator refoper(RefOutput->operands + ref->o_args[i]);
      oper_iterator getoper(GetOutput->operands + get->o_args[i]);
      switch(getoper->kind){
      caseOperBrig(OperandFunctionRef);
      default:
        printf("Offset to invalid operand");
        exit(1);
      }
    }
  }
}

void validate(const BrigOperandArgumentRef* ref, const BrigOperandArgumentRef* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  dir_iterator refdir(RefOutput->directives + ref->arg);
  dir_iterator getdir(GetOutput->directives + get->arg);
  switch(getdir->kind){
    caseDirBrig(DirectiveSymbol);
    caseDirBrig(DirectiveImage);
    caseDirBrig(DirectiveSampler);
    default:
      printf("Offset to invalid directive");
      exit(1);
  }
}

void validate(const BrigOperandWaveSz* ref, const BrigOperandWaveSz* get, 
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
}

void validate(const BrigOperandFunctionRef* ref, const BrigOperandFunctionRef* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  dir_iterator refdir(RefOutput->directives + ref->fn);
  dir_iterator getdir(GetOutput->directives + get->fn);
  switch(getdir->kind){
    caseDirBrig(DirectiveFunction);
    caseDirBrig(DirectiveSignature);
    default:
      printf("Offset to invalid directive");
      exit(1);
  }
}

void validate(const BrigOperandOpaque* ref, const BrigOperandOpaque* get,
    BrigSections* RefOutput, BrigSections* GetOutput){

  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  dir_iterator refdir(RefOutput->directives + ref->directive);
  dir_iterator getdir(GetOutput->directives + get->directive);
  if (refdir != NULL) {
    switch(getdir->kind){
      caseDirBrig(DirectiveSymbol);
      default:
        printf("Offset to invalid directive");
        exit(1);
    }
  }
  if((ref->reg!=0) && (get->reg!=0)){
    oper_iterator refoper(RefOutput->operands + ref->reg);
    oper_iterator getoper(GetOutput->operands + get->reg);
    if(getoper->kind == BrigEOperandReg)
      validate_brig::validate((cast<BrigOperandReg>(refoper)), (cast<BrigOperandReg>(getoper)),
      RefOutput, GetOutput); 
    else{
      printf("Offset to invalid operand");
      exit(1);
    }
  }
    EXPECT_EQ(ref->offset, get->offset);
}


/********************************************** Directives ********************************************/
void validate_CODE_OFFSET(BrigcOffset32_t refoffset, BrigcOffset32_t getoffset, BrigSections* RefOutput, BrigSections* GetOutput){
  inst_iterator refinst(RefOutput->code + refoffset); 
  inst_iterator getinst(GetOutput->code + getoffset); 
  if(refinst==NULL || getinst==NULL) {
    return;
  }
  switch(getinst->kind){ 
    caseInstBrig(InstBase); 
    caseInstBrig(InstMod) ;
    caseInstBrig(InstCvt) ;
    caseInstBrig(InstRead);
    caseInstBrig(InstBar) ;
    caseInstBrig(InstLdSt);
    caseInstBrig(InstCmp);
    caseInstBrig(InstMem);
    caseInstBrig(InstAtomic);
    caseInstBrig(InstAtomicImage);
    caseInstBrig(InstImage);
    default:
        printf("Invalid instruction\n");
        exit(1);
  }
}  
  
void validate_DIR_OFFSET(BrigdOffset32_t refoffset, BrigdOffset32_t getoffset, BrigSections* RefOutput, BrigSections* GetOutput){
  dir_iterator refdir(RefOutput->directives + refoffset);
  dir_iterator getdir(GetOutput->directives + getoffset);
  if (refdir == NULL || getdir == NULL) {
    return ;
  }
  switch(getdir->kind){
    caseDirBrig(DirectivePad);        
        caseDirBrig(DirectiveFunction) ;        
        caseDirBrig(DirectiveKernel);        
        caseDirBrig(DirectiveSymbol);        
        caseDirBrig(DirectiveImage) ;        
        caseDirBrig(DirectiveSampler);        
        caseDirBrig(DirectiveLabel) ;       
        caseDirBrig(DirectiveLabelList) ;        
        caseDirBrig(DirectiveVersion);         
        caseDirBrig(DirectiveSignature);          
        caseDirBrig(DirectiveFile) ;        
        caseDirBrig(DirectiveComment);        
        caseDirBrig(DirectiveLoc);        
        caseDirBrig(DirectiveInit);        
        caseDirBrig(DirectiveLabelInit);        
        caseDirBrig(DirectiveControl);        
        caseDirBrig(DirectivePragma);       
        caseDirBrig(DirectiveExtension);        
        case(BrigEDirectiveArgStart):        
        case(BrigEDirectiveArgEnd): /*validate_brig::validate((reinterpret_cast<BrigDirectiveScope>(refdir)), (reinterpret_cast<BrigDirectiveScope>(getdir)), 
                                      RefOutput, GetOutput);*/
                                    break;
        caseDirBrig(DirectiveBlockStart);        
        caseDirBrig(DirectiveBlockNumeric);       
        caseDirBrig(DirectiveBlockString);        
        caseDirBrig(DirectiveBlockEnd);
        default:
            printf("Invalid instruction\n");
            exit(1);
  }
}

void validate(const BrigDirectiveBase* ref, const BrigDirectiveBase* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
}    

void validate(const BrigDirectiveComment* ref, const BrigDirectiveComment* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  validate_CODE_OFFSET(ref->c_code, get->c_code, RefOutput, GetOutput);
  EXPECT_STREQ(&(RefOutput->strings[ref->s_name]), &(GetOutput->strings[get->s_name]));
}    

void validate(const BrigDirectiveControl* ref, const BrigDirectiveControl* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  validate_CODE_OFFSET(ref->c_code, get->c_code, RefOutput, GetOutput);
  EXPECT_EQ(ref->controlType, get->controlType);
  EXPECT_EQ(ref->values[0], get->values[0]);
  EXPECT_EQ(ref->values[1], get->values[1]);
  EXPECT_EQ(ref->values[2], get->values[2]);
}

void validate(const BrigDirectiveExtension* ref, const BrigDirectiveExtension* get, 
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  validate_CODE_OFFSET(ref->c_code, get->c_code, RefOutput, GetOutput);
  EXPECT_STREQ(&(RefOutput->strings[ref->s_name]), &(GetOutput->strings[get->s_name]));
}

void validate(const BrigDirectiveFile* ref, const BrigDirectiveFile* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  validate_CODE_OFFSET(ref->c_code, get->c_code, RefOutput, GetOutput);
  EXPECT_EQ(ref->fileid, get->fileid);
  EXPECT_STREQ(&(RefOutput->strings[ref->s_filename]), &(GetOutput->strings[get->s_filename]));
}

void validate(const BrigDirectiveFunction* ref, const BrigDirectiveFunction* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  validate_CODE_OFFSET(ref->c_code, get->c_code, RefOutput, GetOutput);
  EXPECT_STREQ(&(RefOutput->strings[ref->s_name]), &(GetOutput->strings[get->s_name]));
  EXPECT_EQ(ref->inParamCount, get->inParamCount);
  validate_DIR_OFFSET(ref->d_firstScopedDirective, get->d_firstScopedDirective, RefOutput, GetOutput);
  EXPECT_EQ(ref->operationCount, get->operationCount);
  validate_DIR_OFFSET(ref->d_nextDirective, get->d_nextDirective, RefOutput, GetOutput);
  EXPECT_EQ(ref->attribute, get->attribute);
  EXPECT_EQ(ref->reserved, get->reserved);
  EXPECT_EQ(ref->outParamCount, get->outParamCount);
  if((ref->d_firstInParam!=0) && (get->d_firstInParam !=0)){
    validate_DIR_OFFSET(ref->d_firstInParam, get->d_firstInParam, RefOutput, GetOutput);
  }    
}

void validate(const BrigDirectiveImage* ref, const BrigDirectiveImage* get, 
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  validate_CODE_OFFSET(ref->s.c_code, get->s.c_code, RefOutput, GetOutput);
  EXPECT_EQ(ref->s.storageClass, get->s.storageClass);
  EXPECT_EQ(ref->s.attribute, get->s.attribute);
  EXPECT_EQ(ref->s.reserved, get->s.reserved);
  EXPECT_EQ(ref->s.symbolModifier, get->s.symbolModifier);
  EXPECT_EQ(ref->s.dim, get->s.dim);
  EXPECT_STREQ(&(RefOutput->strings[ref->s.s_name]), &(GetOutput->strings[get->s.s_name]));
  EXPECT_EQ(ref->s.type, get->s.type);
  EXPECT_EQ(ref->s.align, get->s.align);
  EXPECT_EQ(ref->width, get->width);
  EXPECT_EQ(ref->height, get->height);
  EXPECT_EQ(ref->depth, get->depth);
  EXPECT_EQ(ref->array, get->array);
  EXPECT_EQ(ref->order, get->order);
  EXPECT_EQ(ref->format, get->format);
}

void validate(const BrigDirectiveInit* ref, const BrigDirectiveInit* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  validate_CODE_OFFSET(ref->c_code, get->c_code, RefOutput, GetOutput);
  EXPECT_EQ(ref->elementCount, get->elementCount);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->reserved, get->reserved);
  int no_init_bytes = ref->size - sizeof(BrigDirectiveInit) + sizeof(uint64_t);
  for(int i=0; i < no_init_bytes; i++)
    EXPECT_EQ(ref->initializationData.u8[i], get->initializationData.u8[i]);
}

void validate(const BrigDirectiveLabel* ref, const BrigDirectiveLabel* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
    
}

void validate(const BrigDirectiveLabelList* ref, const BrigDirectiveLabelList* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
    
}

void validate(const BrigDirectiveLabelInit* ref, const BrigDirectiveLabelInit* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
    
}

void validate(const BrigDirectiveKernel* ref, const BrigDirectiveKernel* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  validate_CODE_OFFSET(ref->c_code, get->c_code, RefOutput, GetOutput);
  EXPECT_STREQ(&(RefOutput->strings[ref->s_name]), &(GetOutput->strings[get->s_name]));
  EXPECT_EQ(ref->inParamCount, get->inParamCount);
  validate_DIR_OFFSET(ref->d_firstScopedDirective, get->d_firstScopedDirective, RefOutput, GetOutput);
  EXPECT_EQ(ref->operationCount, get->operationCount);
  validate_DIR_OFFSET(ref->d_nextDirective, get->d_nextDirective, RefOutput, GetOutput);
  EXPECT_EQ(ref->attribute, get->attribute);
  EXPECT_EQ(ref->reserved, get->reserved);
  EXPECT_EQ(ref->outParamCount, get->outParamCount);
  if((ref->d_firstInParam!=0) && (get->d_firstInParam !=0)){
    validate_DIR_OFFSET(ref->d_firstInParam, get->d_firstInParam, RefOutput, GetOutput);
  }    
}

void validate(const BrigDirectiveLoc* ref, const BrigDirectiveLoc* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  validate_CODE_OFFSET(ref->c_code, get->c_code, RefOutput, GetOutput);
  EXPECT_EQ(ref->sourceFile, get->sourceFile);
  EXPECT_EQ(ref->sourceLine, get->sourceLine);
  EXPECT_EQ(ref->sourceColumn, get->sourceColumn);
}

void validate(const  BrigDirectivePad* ref, const BrigDirectivePad* get, 
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
}

void validate(const BrigDirectivePragma* ref, const BrigDirectivePragma* get, 
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  validate_CODE_OFFSET(ref->c_code, get->c_code, RefOutput, GetOutput);
  EXPECT_STREQ(&(RefOutput->strings[ref->s_name]), &(GetOutput->strings[get->s_name]));
}

void validate(const BrigDirectiveSignature* ref, const BrigDirectiveSignature* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  validate_CODE_OFFSET(ref->c_code, get->c_code, RefOutput, GetOutput);
  EXPECT_STREQ(&(RefOutput->strings[ref->s_name]), &(GetOutput->strings[get->s_name]));
  EXPECT_EQ(ref->outCount, get->outCount);
  EXPECT_EQ(ref->inCount, get->inCount);
}

void validate(const BrigDirectiveSampler* ref, const BrigDirectiveSampler* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  validate_CODE_OFFSET(ref->s.c_code, get->s.c_code, RefOutput, GetOutput);
  EXPECT_EQ(ref->s.storageClass, get->s.storageClass);
  EXPECT_EQ(ref->s.attribute, get->s.attribute);
  EXPECT_EQ(ref->s.reserved, get->s.reserved);
  EXPECT_EQ(ref->s.symbolModifier, get->s.symbolModifier);
  EXPECT_EQ(ref->s.dim, get->s.dim);
  EXPECT_STREQ(&(RefOutput->strings[ref->s.s_name]), &(GetOutput->strings[get->s.s_name]));
  EXPECT_EQ(ref->s.type, get->s.type);
  EXPECT_EQ(ref->s.align, get->s.align);
  EXPECT_EQ(ref->valid, get->valid);
  EXPECT_EQ(ref->normalized, get->normalized);
  EXPECT_EQ(ref->filter, get->filter);
  EXPECT_EQ(ref->boundaryU, get->boundaryU);
  EXPECT_EQ(ref->boundaryV, get->boundaryV);
  EXPECT_EQ(ref->boundaryW, get->boundaryW);
  EXPECT_EQ(ref->reserved, get->reserved);
}

void validate(const BrigDirectiveScope* ref, const BrigDirectiveScope* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
    
}    

void validate(const BrigDirectiveSymbol* ref, const BrigDirectiveSymbol* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  validate_CODE_OFFSET(ref->s.c_code, get->s.c_code, RefOutput, GetOutput);
  EXPECT_EQ(ref->s.storageClass, get->s.storageClass);
  EXPECT_EQ(ref->s.attribute, get->s.attribute);
  EXPECT_EQ(ref->s.reserved, get->s.reserved);
  EXPECT_EQ(ref->s.symbolModifier, get->s.symbolModifier);
  EXPECT_EQ(ref->s.dim, get->s.dim);
  EXPECT_STREQ(&(RefOutput->strings[ref->s.s_name]), &(GetOutput->strings[get->s.s_name]));
  EXPECT_EQ(ref->s.type, get->s.type);
  EXPECT_EQ(ref->s.align, get->s.align);
  if(ref->d_init!=0 && get->d_init!=0){
    validate_DIR_OFFSET(ref->d_init, get->d_init, RefOutput, GetOutput);
  }    
  EXPECT_EQ(ref->reserved, get->reserved);
}

void validate(const BrigDirectiveVersion* ref, const BrigDirectiveVersion* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  validate_CODE_OFFSET(ref->c_code, get->c_code, RefOutput, GetOutput);
  EXPECT_EQ(ref->major, get->major);
  EXPECT_EQ(ref->minor, get->minor); 
  EXPECT_EQ(ref->machine, get->machine);
  EXPECT_EQ(ref->profile, get->profile);
  EXPECT_EQ(ref->ftz, get->ftz);
  EXPECT_EQ(ref->reserved, get->reserved);
}

void validate(const BrigBlockStart* ref, const BrigBlockStart* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  validate_CODE_OFFSET(ref->c_code, get->c_code, RefOutput, GetOutput);  
  EXPECT_STREQ(&(RefOutput->strings[ref->s_name]), &(GetOutput->strings[get->s_name]));
}

void validate(const BrigBlockString* ref, const BrigBlockString* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_STREQ(&(RefOutput->strings[ref->s_name]), &(GetOutput->strings[get->s_name]));
}    

void validate(const BrigBlockNumeric* ref, const BrigBlockNumeric* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);
  EXPECT_EQ(ref->type, get->type);
  EXPECT_EQ(ref->elementCount, get->elementCount);  
  EXPECT_EQ(ref->u64, get->u64);
}

void validate(const BrigBlockEnd* ref, const BrigBlockEnd* get,
    BrigSections* RefOutput, BrigSections* GetOutput){
  EXPECT_EQ(ref->size, get->size);
  EXPECT_EQ(ref->kind, get->kind);  
}

}//validate_brig
}//brig
}//hsa    
