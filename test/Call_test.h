#include <iostream>
#include <string>
#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa {
namespace brig {

template <typename TInst, typename TSrc2> 
class Call_Test: public BrigCodeGenTest {

private:
  const TInst* RefInst;
  // Operands in .operands buffer

  // Width operand
  const BrigOperandImmed* OpWidth;

  const BrigOperandArgumentList* RefSrc1;
  // TSrc2 will become BrigOperandFunctionRef or BrigOperandReg
  const TSrc2* RefSrc2;
  const BrigOperandArgumentList* RefSrc3;
  // BrigEOperandFunctionList 
  const BrigOperandArgumentList* RefSrc4;

  const bool isSignatureFunc;

public:
  Call_Test(std::string& in, StringBuffer* sbuf, TInst* ref, BrigOperandImmed *width,
            BrigOperandArgumentList* Src1, TSrc2* Src2, BrigOperandArgumentList* Src3, 
            BrigOperandArgumentList* Src4, bool isSign):
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),
    OpWidth(width),
    RefSrc1(Src1),
    RefSrc2(Src2),
    RefSrc3(Src3),
    RefSrc4(Src4),
    isSignatureFunc(isSign) { }
 
  void validate(struct BrigSections* TestOutput) {
 
    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;   
    const BrigOperandArgumentRef* getArgRef;
    BrigOperandArgumentRef argRef = {
      0,
      BrigEOperandArgumentRef,
      0
    };
    argRef.size = sizeof(argRef);
    
    inst_iterator getcode = TestOutput->code_begin();
    const TInst* getinst = (cast<TInst>(getcode));
    validate_brig::validate(RefInst, getinst);

    if (OpWidth != NULL) {
      const BrigOperandImmed* getwidth = reinterpret_cast <const BrigOperandImmed*> 
                                         (&(TestOutput->operands[getinst->o_operands[0]]));
      validate_brig::validate(OpWidth, getwidth);
    }
    
    const BrigOperandArgumentList* getsrc1 = reinterpret_cast <const BrigOperandArgumentList*> 
                                             (&(TestOutput->operands[getinst->o_operands[1]]));
    validate_brig::validate(RefSrc1, getsrc1);

    for (uint32_t i = 0 ; i < getsrc1->elementCount ; ++i) {
      getArgRef = reinterpret_cast <const BrigOperandArgumentRef*> 
                  (&(TestOutput->operands[getsrc1->o_args[i]]));

      validate_brig::validate(&argRef, getArgRef);
    }
    
    const TSrc2* getsrc2 = reinterpret_cast <const TSrc2*> 
                           (&(TestOutput->operands[getinst->o_operands[2]]));  
    validate_brig::validateOpType<TSrc2>(RefSrc2, refbuf, getsrc2, getbuf);      

    const BrigOperandArgumentList* getsrc3 = reinterpret_cast <const BrigOperandArgumentList*> 
                                             (&(TestOutput->operands[getinst->o_operands[3]]));
    validate_brig::validate(RefSrc3, getsrc3);

    for (uint32_t i = 0 ; i < getsrc3->elementCount ; ++i) {
      getArgRef = reinterpret_cast <const BrigOperandArgumentRef*> 
                  (&(TestOutput->operands[getsrc3->o_args[i]]));

      validate_brig::validate(&argRef, getArgRef);
    }
    if (RefSrc4 != NULL) {
      const BrigOperandArgumentList* getsrc4 = reinterpret_cast <const BrigOperandArgumentList*> 
                                               (&(TestOutput->operands[getinst->o_operands[4]]));
      validate_brig::validate(RefSrc4, getsrc4);

      if (!isSignatureFunc) {
        const BrigOperandFunctionRef* getFunRef;
        BrigOperandFunctionRef funRef = {
          0,
          BrigEOperandFunctionRef,
          0
        };
        funRef.size = sizeof(funRef);
        for (uint32_t i = 0 ; i < getsrc4->elementCount ; ++i) {
          getFunRef = reinterpret_cast <const BrigOperandFunctionRef*> 
                      (&(TestOutput->operands[getsrc4->o_args[i]]));
  
          validate_brig::validate(&funRef, getFunRef);
        }
      } else {
        getArgRef = reinterpret_cast <const BrigOperandArgumentRef*> 
                    (&(TestOutput->operands[getsrc4->o_args[0]]));
  
        validate_brig::validate(&argRef, getArgRef);
        
      }
    } else {
      EXPECT_EQ(0, getinst->o_operands[4]);
    }
  }
};


TEST(CodegenTest, Call_CodeGen) {  
  
  /********************************** Common variables used by all tests******************************/
  //To keep the stack footprint low
  
  std:: string in; 
  std::string regName;
  StringBuffer* symbols;
  
  BrigInstBase outBase;
  BrigInstMod outMod;
  BrigOperandImmed width;
  BrigOperandReg reg;
  BrigOperandFunctionRef func;
  BrigOperandArgumentList inputArgs, outputArgs;
  BrigOperandArgumentList funList;
  BrigOperandArgumentList* pFunList, *pInputArgs;
  BrigOperandArgumentRef argRef;  

  BrigOperandArgumentList emptyArgs = {
    sizeof(emptyArgs),
    BrigEOperandArgumentList,
    0,
    {0}
  };

  symbols = new StringBuffer();
   
  /************************************* Test Case 1 ************************************/

  in.assign("call_width(64) $s1 (%in) [&foo, &bar];\n");
  regName.assign("$s1"); 
  symbols->append(regName);

  outBase.size = sizeof(outBase);
  outBase.kind = BrigEInstBase;
  outBase.opcode = BrigCall;
  outBase.type = Brigb32;
  outBase.packing = BrigNoPacking;
  outBase.o_operands[0] = 0;
  outBase.o_operands[1] = sizeof(width) + sizeof(reg) + sizeof(inputArgs) + sizeof(argRef);
  outBase.o_operands[2] = sizeof(width);
  outBase.o_operands[3] = sizeof(width) + sizeof(reg) + sizeof(argRef);
  outBase.o_operands[4] = outBase.o_operands[1] + sizeof(func) * 2 + sizeof(emptyArgs);

  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 64;

  inputArgs.size = sizeof(inputArgs);
  inputArgs.kind = BrigEOperandArgumentList;
  inputArgs.elementCount = 1;
  inputArgs.o_args[0] = sizeof(width) + sizeof(reg);

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.name = 0;

  unsigned char mem1[sizeof(funList) + sizeof(BrigoOffset32_t)];
  pFunList = reinterpret_cast<BrigOperandArgumentList*>(mem1);
  pFunList->size = sizeof(funList) + sizeof(BrigoOffset32_t);
  pFunList->kind = BrigEOperandFunctionList;
  pFunList->elementCount = 2;
  pFunList->o_args[0] = outBase.o_operands[1] + sizeof(emptyArgs);
  pFunList->o_args[1] = outBase.o_operands[1] + sizeof(emptyArgs) + sizeof(func);
 
  Call_Test<BrigInstBase, BrigOperandReg> 
  TestCase1(in, symbols, &outBase, &width, &emptyArgs, &reg, &inputArgs, pFunList, false);

  TestCase1.Run_Test(&Call);
  symbols->clear();

  /************************************* Test Case 2 ************************************/

  in.assign("call_width(all)_fbar $s1 (%out)(%in) &sigFunc;\n");
  regName.assign("$s1"); 
  symbols->append(regName);

  outMod.size = sizeof(outMod);
  outMod.kind = BrigEInstMod;
  outMod.opcode = BrigCall;
  outMod.type = Brigb32;
  outMod.packing = BrigNoPacking;
  outMod.o_operands[0] = 0;
  outMod.o_operands[1] = sizeof(reg) + sizeof(argRef);
  outMod.o_operands[2] = 0;
  outMod.o_operands[3] = sizeof(outputArgs) + outMod.o_operands[1] + sizeof(argRef);
  outMod.o_operands[4] = outMod.o_operands[3] + sizeof(inputArgs) + sizeof(argRef);
  memset(&outMod.aluModifier, 0, sizeof(outMod.aluModifier));
  outMod.aluModifier.fbar = 1;

  outputArgs.size = sizeof(outputArgs);
  outputArgs.kind = BrigEOperandArgumentList;
  outputArgs.elementCount = 1;
  outputArgs.o_args[0] = sizeof(reg);

  inputArgs.size = sizeof(inputArgs);
  inputArgs.kind = BrigEOperandArgumentList;
  inputArgs.elementCount = 1;
  inputArgs.o_args[0] = outMod.o_operands[1] + sizeof(outputArgs);

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.name = 0;

  funList.size = sizeof(funList);
  funList.kind = BrigEOperandFunctionList;
  funList.elementCount = 1;
  funList.o_args[0] = outMod.o_operands[3] + sizeof(inputArgs);
 
  Call_Test<BrigInstMod, BrigOperandReg> 
  TestCase2(in, symbols, &outMod, NULL, &outputArgs, &reg, &inputArgs, &funList, true);

  TestCase2.Run_Test(&Call);
  symbols->clear();

  /************************************* Test Case 3 ************************************/

  in.assign("call &foo(%out)(%in1,%in2,%in3,%in4,%in5,%in6,%in7,%in8);\n");

  outBase.size = sizeof(outBase);
  outBase.kind = BrigEInstBase;
  outBase.opcode = BrigCall;
  outBase.type = Brigb32;
  outBase.packing = BrigNoPacking;
  outBase.o_operands[0] = 0;
  outBase.o_operands[1] = sizeof(func) + sizeof(width) + sizeof(argRef);
  outBase.o_operands[2] = sizeof(width);
  outBase.o_operands[3] = outBase.o_operands[1] + sizeof(argRef) * 8 + sizeof(outputArgs);
  outBase.o_operands[4] = 0;

  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 0;

  outputArgs.size = sizeof(outputArgs);
  outputArgs.kind = BrigEOperandArgumentList;
  outputArgs.elementCount = 1;
  outputArgs.o_args[0] = sizeof(func) + sizeof(width);

  unsigned char mem3[sizeof(BrigoOffset32_t) * 7 + sizeof(inputArgs)];
  pInputArgs = reinterpret_cast<BrigOperandArgumentList*>(mem3);
  pInputArgs->size = sizeof(inputArgs) + sizeof(BrigoOffset32_t) * 7;
  pInputArgs->kind = BrigEOperandArgumentList;
  pInputArgs->elementCount = 8;
  pInputArgs->o_args[0] = outBase.o_operands[1] + sizeof(outputArgs);
  pInputArgs->o_args[1] = pInputArgs->o_args[0] + sizeof(argRef); 
  pInputArgs->o_args[2] = pInputArgs->o_args[1] + sizeof(argRef); 
  pInputArgs->o_args[3] = pInputArgs->o_args[2] + sizeof(argRef); 
  pInputArgs->o_args[4] = pInputArgs->o_args[3] + sizeof(argRef); 
  pInputArgs->o_args[5] = pInputArgs->o_args[4] + sizeof(argRef); 
  pInputArgs->o_args[6] = pInputArgs->o_args[5] + sizeof(argRef); 
  pInputArgs->o_args[7] = pInputArgs->o_args[6] + sizeof(argRef); 

  func.size = sizeof(func);
  func.kind = BrigEOperandFunctionRef;
  func.fn = 0;
 
  Call_Test<BrigInstBase, BrigOperandFunctionRef> 
  TestCase3(in, symbols, &outBase, &width, &outputArgs, &func, pInputArgs, NULL, false);

  TestCase3.Run_Test(&Call);
  symbols->clear();


  /************************************* Test Case 4 ************************************/

  in.assign("call &bar(%in);\n");

  outBase.size = sizeof(outBase);
  outBase.kind = BrigEInstBase;
  outBase.opcode = BrigCall;
  outBase.type = Brigb32;
  outBase.packing = BrigNoPacking;
  outBase.o_operands[0] = 0;
  outBase.o_operands[1] = sizeof(func) + sizeof(width) + sizeof(argRef) + sizeof(inputArgs);
  outBase.o_operands[2] = sizeof(width);
  outBase.o_operands[3] = sizeof(func) + sizeof(width) + sizeof(argRef);
  outBase.o_operands[4] = 0;

  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 0;

  inputArgs.size = sizeof(inputArgs);
  inputArgs.kind = BrigEOperandArgumentList;
  inputArgs.elementCount = 1;
  inputArgs.o_args[0] = sizeof(width) + sizeof(reg);

  func.size = sizeof(func);
  func.kind = BrigEOperandFunctionRef;
  func.fn = 0;
 
  Call_Test<BrigInstBase, BrigOperandFunctionRef> 
  TestCase4(in, symbols, &outBase, &width, &emptyArgs, &func, &inputArgs, NULL, false);

  TestCase4.Run_Test(&Call);
  symbols->clear();


  /************************************* Test Case 5 ************************************/
  in.assign("call &func;\n");

  outBase.size = sizeof(outBase);
  outBase.kind = BrigEInstBase;
  outBase.opcode = BrigCall;
  outBase.type = Brigb32;
  outBase.packing = BrigNoPacking;
  outBase.o_operands[0] = 0;
  outBase.o_operands[1] = sizeof(width) + sizeof(func);
  outBase.o_operands[2] = sizeof(width);
  outBase.o_operands[3] = sizeof(func) + sizeof(width);
  outBase.o_operands[4] = 0;

  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));
  width.bits.u = 0;

  func.size = sizeof(func);
  func.kind = BrigEOperandFunctionRef;
  func.fn = 0;
 
  Call_Test<BrigInstBase, BrigOperandFunctionRef> 
  TestCase5(in, symbols, &outBase, &width, &emptyArgs, &func, &emptyArgs, NULL, false);

  TestCase5.Run_Test(&Call);
  symbols->clear();

  delete symbols;
}
} // namespace hsa
} // namespace brig




