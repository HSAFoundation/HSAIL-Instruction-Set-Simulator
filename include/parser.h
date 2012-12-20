// Copyright 2012 MulticoreWare Inc.

#ifndef INCLUDE_PARSER_H_
#define INCLUDE_PARSER_H_

#include <string>
#include "context.h"
namespace hsa {
namespace brig {

int Query(Context* context);
int QueryOp(Context* context);
int QueryOp(Context* context, BrigDataType16_t* pDataType, BrigOpcode32_t* pOpcode);


int Operand(Context* context);
int Operand(Context* context, BrigoOffset32_t* pRetOpOffset);
int Operand(Context* context, BrigoOffset32_t* pRetOpOffset,
            BrigDataType16_t expectedType);

int Identifier(Context* context);
int BaseOperand(Context* context);
int AddressableOperand(Context* context);
int ArrayOperandList(Context* context);
int RoundingMode(Context* context);
int FunctionSignature(Context* context);
int Instruction2(Context* context);
int Instruction3(Context* context);
int Version(Context* context);
int Alignment(Context* context);
int SignatureArgumentList(Context *context);
int SignatureArgumentList(Context *context, std::vector<BrigDirectiveSignature::BrigProtoType> *types);
int Label(Context* context);
int LabelTargets(Context* context);
int Instruction4(Context* context);
int DeclPrefix(Context* context);
int FBar(Context* context);
int ArrayDimensionSet(Context* context);
int ArgumentDecl(Context* context);
int ArgumentListBody(Context* context);
int ArgumentListBody(Context* context, int* paramCount);

int FunctionDefinition(Context* context);
int FunctionDecl(Context* context);
int Codeblock(Context* context);
int Function(Context* context);
int OptionalWidth(Context* context);
int Program(Context* context);
int Branch(Context* context);
int CallTargets(Context* context);
int CallArgs(Context* context);
int Call(Context* context);
int Initializer(Context* context, BrigdOffset32_t sym_offset);
int InitializableDecl(Context* context);
int InitializableDeclPart2(Context* context, BrigStorageClass32_t storage_class);
int UninitializableDecl(Context* context);
int ArgUninitializableDecl(Context* context);
int VectorToken(Context *context);
int SignatureType(Context *context);
int SignatureType(Context *context, std::vector<BrigDirectiveSignature::BrigProtoType> *types);
int FileDecl(Context* context);
int SysCall(Context* context);
int ArgBlock(Context* context);
int Extension(Context* context);
int KernelArgumentDecl(Context *context);
int KernelArgumentListBody(Context *context);
int KernelArgumentListBody(Context *context,uint32_t *paramCount);
int Kernel(Context *context);
int OperandList(Context* context);
int Cmp(Context* context);
int GlobalPrivateDecl(Context* context);
int OffsetAddressableOperand(Context* context, BrigoOffset32_t addrOpOffset);
int MemoryOperand(Context* context);
int Ldc(Context* context);
int Instruction5(Context* context);
int CvtModifier1(Context* context);
int Mov(Context* context);
int Atom(Context* context);
int Instruction0(Context* context);
int Instruction1(Context* context);
int Operation(Context* context);
int Segp(Context* context);
int SegpPart1Segmentp(Context* context);
int SegpPart2StoFAndFtoS(Context* context);

int BodyStatementNested(Context* context);
int ArgStatement(Context* context);
int ArgStatements(Context* context);
int BodyStatement(Context* context);
int BodyStatements(Context* context);
int GlobalGroupDecl(Context* context);

/* MUL */
int Mul(Context* context);
int MulInst(Context* context);
int Mul24Inst(Context* context);
int Mad24Inst(Context* context);
int Ld(Context* context);
int LdModifier(Context* context, BrigInstLdSt* pLdSt_op, int* pVec_size);
int St(Context* context);
int Lda(Context* context);
int ImageRet(Context* context);
int ImageNoRet(Context* context);
int Cvt(Context* context);
int ImageLoad(Context* context);
int ImageStore(Context* context);
int GlobalImageDecl(Context *context);
int GlobalImageDeclPart2(Context *context);
int ImageInitializer(Context *context);
int ImageInit(Context *context);
int GlobalReadOnlyImageDecl(Context *context);
int GlobalReadOnlyImageDeclPart2(Context *context);
int Ret(Context *context);
int ImageRead(Context *context);
int Sync(Context *context);
int Bar(Context *context);
int AtomicNoRet(Context *context);

int Location(Context* context);
int Control(Context* context);
int Pragma(Context* context);
int Block(Context* context);

int DecimalListSingleBLock(Context* context);
int DecimalInitializer(Context* context, BrigdOffset32_t sym_offset);
int DecimalListSingle(Context* context, std::vector<int64_t>* decimal_list);
int FloatInitializer(Context* context, BrigdOffset32_t sym_offset);
int FloatListSingle(Context* context, std::vector<double>* float_list);
int SingleInitializer(Context *context, BrigdOffset32_t sym_offset);
int SingleListSingle(Context* context, std::vector<float>* single_list);
int LabelList(Context* context, BrigdOffset32_t dOffset, bool IsTargets);

int GlobalSymbolDecl(Context* context);
int Directive(Context* context);
int SobInit(Context *context);
int SobInitializer(Context *context);
int GlobalSamplerDecl(Context *context);
int GlobalSamplerDeclPart2(Context *context);
int GlobalDecl(Context *context);
int GlobalInitializablePart2(Context* context);
int GlobalInitializable(Context* context);

int SequenceOfPrograms(Context *context);
int PairAddressableOperand(Context *context);
int OffsetAddressableOperand(Context* context);
int LdModifier(Context* context);

int TopLevelStatement(Context *context);
int TopLevelStatements(Context *context);

int ArrayOperand(Context* context);

int CodeBlockEnd(Context* context);
int Comment(Context* context);

//Declarations of Helper Functions 
int GlobalImageDeclPart2(Context *context);
int GlobalReadOnlyImageDeclPart2(Context *context);
int GlobalInitializablePart2(Context* context);
int GlobalSamplerDeclPart2(Context *context);
int InitializableDecl(Context* context, BrigStorageClass32_t storage_class);
int OffsetAddressableOperandPart2(Context* context, BrigoOffset32_t addrOpOffset, BrigoOffset32_t* pRetOpOffset);
int LdModifierPart2(Context* context, BrigInstLdSt* pLdSt_op, int* pVec_size);
int AddressSpaceIdentifier(Context* context);
int Optacqreg(Context* context, BrigMemorySemantic32_t* memSemantic);
int AcqRel(Context* context, BrigMemorySemantic32_t* memSemantic);
int Acq(Context* context, BrigMemorySemantic32_t* memSemantic);
int ArrayOperandList(Context* context, BrigoOffset32_t* pRetOpOffset);

int ArrayOperand(Context* context, BrigoOffset32_t* pOperand);
int ArrayOperand(Context* context, BrigoOffset32_t* pOperand, BrigDataType16_t expectedType);

int AddressableOperand(Context* context, BrigoOffset32_t* pRetOpOffset, bool IsImageOrSampler);
int ComparisonIdPart2(Context* context, BrigCompareOperation32_t* pCmpOperation);

int Instruction4MultiMediaOperation(Context* context);
int Instruction4Fma(Context* context);
int Instruction4Mad(Context* context);
int Instruction4BitOperation(Context* context);
int Instruction4Cmov(Context* context);
int Instruction4Shuffle(Context* context);
int FunctionDefinitionPart2(Context* context);
int MemoryOperand(Context* context, BrigoOffset32_t* pRetOpOffset);

int LabelTargetsPart2(Context* context, BrigdOffset32_t dOffset);

int Instruction1OpcodeDT(Context* context);
int Instruction1OpcodeNoDT(Context* context);
int Instruction1Clock(Context* context);

int Instruction2OpcodeDT(Context* context);
int Instruction2OpcodeNoDT(Context* context);
int Instruction2OpcodeFtz(Context* context);

int BranchCbr(Context* context);
int BranchBrn(Context* context);

BrigDataType16_t ConvertTypeToB(BrigDataType16_t type, BrigDataType16_t* pSubType = NULL,
                                uint32_t* pSize = NULL);
bool CheckDataType(BrigMachine16_t model, BrigStorageClass32_t addressType, 
    BrigDataType16_t type);
bool CheckRegister(unsigned int token, BrigDataType16_t type); 

bool CheckDestRegister(unsigned int token, BrigDataType16_t type, BrigOpcode32_t opcode); 

}  // namespace brig
}  // namespace hsa
#endif  // INCLUDE_PARSER_H_

