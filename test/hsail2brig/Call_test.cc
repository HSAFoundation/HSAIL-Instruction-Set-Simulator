#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa {
namespace brig {

template <typename TInst = BrigInstBase,
          typename TSrc2 = BrigOperandReg, 
          typename TFuncList = BrigOperandFunctionRef>
class Call_Test: public BrigCodeGenTest {

private:
  const TInst* RefInst;

  const BrigOperandImmed* OpWidth;

  const BrigOperandArgumentList* RefSrc1;
  const TSrc2* RefSrc2;
  const BrigOperandArgumentList* RefSrc3;
  const BrigOperandArgumentList* RefSrc4;

  const BrigOperandArgumentRef* OutArgsList;
  const BrigOperandArgumentRef* InArgsList;
  const TFuncList* FuncList;
  const std::vector<std::string> *SymbolNameList;

public:
  Call_Test(std::string& in, std::vector<std::string> *nameList = NULL):
    BrigCodeGenTest(in), SymbolNameList(nameList) {}

  Call_Test(std::string& in, StringBuffer* sbuf, TInst* ref, BrigOperandImmed *width,
            BrigOperandArgumentList* Src1, TSrc2* Src2, BrigOperandArgumentList* Src3,
            BrigOperandArgumentList* Src4,  BrigOperandArgumentRef* outList = NULL,
            BrigOperandArgumentRef* inList = NULL, TFuncList* funcList = NULL, 
            std::vector<std::string> *nameList = NULL):
    BrigCodeGenTest(in, sbuf),
    RefInst(ref),         OpWidth(width),
    RefSrc1(Src1),        RefSrc2(Src2),
    RefSrc3(Src3),        RefSrc4(Src4),
    OutArgsList(outList), InArgsList(inList),
    FuncList(funcList),   SymbolNameList(nameList) { }

  void Run_Test(int (*Rule)(Context*)) {
    Buffer* code = new Buffer();
    Buffer* oper = new Buffer();
    Buffer* dir = new Buffer();
    code->append(RefInst);
    oper->append(OpWidth);

    oper->append(RefSrc2);

    if (RefSrc1->elementCount == 0) {
      for (uint32_t i = 0 ; i < RefSrc3->elementCount ; ++i) {
        oper->append(&InArgsList[i]);
      }
      oper->append(RefSrc3);
      oper->append(RefSrc1);
    } else {
      for (uint32_t i = 0 ; i < RefSrc1->elementCount ; ++i) {
        oper->append(&OutArgsList[i]);
      }      
      oper->append(RefSrc1);
      for (uint32_t i = 0 ; i < RefSrc3->elementCount ; ++i) {
        oper->append(&InArgsList[i]);
      }
      oper->append(RefSrc3);
    }

    if (RefSrc4 != NULL) {
      for (uint32_t i = 0 ; i < RefSrc4->elementCount ; ++i) {
        oper->append(&FuncList[i]);
      }
      oper->append(RefSrc4);
    }

    struct BrigSections RefOutput(reinterpret_cast<const char *>(&RefStr->get()[0]), 
      reinterpret_cast<const char *>(&dir->get()[0]),
      reinterpret_cast<const char *>(&code->get()[0]), 
      reinterpret_cast<const char *>(&oper->get()[0]), NULL, 
      RefStr->size(), 0, code->size(), oper->size(), 0);    
    
    Parse_Validate(Rule, &RefOutput, SymbolNameList);
    delete code;
    delete oper;
    delete dir;
  }
  void Run_Test(int (*Rule)(Context*), error_code_t refError){
    False_Validate(Rule, refError, SymbolNameList);
  }
};


TEST(CodegenTest, Call_CodeGen) {

  std::string in;
  std::string regName;
  std::vector<std::string> symbolNameList;
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

  /********************** Test Case 1 ****************************/
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

  BrigOperandArgumentRef* inputArgsList1 = 
    new BrigOperandArgumentRef[inputArgs.elementCount * sizeof(argRef)];
 
  for (uint32_t i = 0 ; i < inputArgs.elementCount ; ++i) {
    inputArgsList1[i].size = sizeof(inputArgsList1[i]);
    inputArgsList1[i].kind = BrigEOperandArgumentRef;
    inputArgsList1[i].arg = 0;
  }

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.s_name = 0;

  unsigned char mem1[sizeof(funList) + sizeof(BrigoOffset32_t)];
  pFunList = reinterpret_cast<BrigOperandArgumentList*>(mem1);
  pFunList->size = sizeof(funList) + sizeof(BrigoOffset32_t);
  pFunList->kind = BrigEOperandFunctionList;
  pFunList->elementCount = 2;
  pFunList->o_args[0] = outBase.o_operands[1] + sizeof(emptyArgs);
  pFunList->o_args[1] = outBase.o_operands[1] + sizeof(emptyArgs) + sizeof(func);

  BrigOperandFunctionRef* funcList1 = 
    new BrigOperandFunctionRef[pFunList->elementCount * sizeof(func)];
  for (uint32_t i = 0 ; i < pFunList->elementCount ; ++i) {
    funcList1[i].size = sizeof(funcList1[i]);
    funcList1[i].kind = BrigEOperandFunctionRef;
    funcList1[i].fn = 0;
  }

  symbolNameList.clear();
  symbolNameList.push_back("%in");
  Call_Test<BrigInstBase, BrigOperandReg, BrigOperandFunctionRef>
  TestCase1(in, symbols, &outBase, &width, &emptyArgs, &reg, 
      &inputArgs, pFunList, NULL, inputArgsList1, funcList1, &symbolNameList);

  TestCase1.Run_Test(&Call);

  delete []inputArgsList1;  inputArgsList1 = NULL;
  delete []funcList1;  funcList1 = NULL;
  symbols->clear();

  /********************** Test Case 2 ************************/
  in.assign("call_width(all)_fbar $s1 (%out)(%in) &sigFunc;\n");
  regName.assign("$s1");
  symbols->append(regName);

  outMod.size = sizeof(outMod);
  outMod.kind = BrigEInstMod;
  outMod.opcode = BrigCall;
  outMod.type = Brigb32;
  outMod.packing = BrigNoPacking;
  outMod.o_operands[0] = 0;
  outMod.o_operands[1] = sizeof(width) + sizeof(reg) + sizeof(argRef);
  outMod.o_operands[2] = sizeof(width);
  outMod.o_operands[3] = sizeof(outputArgs) + outMod.o_operands[1] + sizeof(argRef);
  outMod.o_operands[4] = outMod.o_operands[3] + sizeof(inputArgs) + sizeof(argRef);
  memset(&outMod.aluModifier, 0, sizeof(outMod.aluModifier));
  outMod.aluModifier.valid = 1;
  outMod.aluModifier.fbar = 1;

  width.size = sizeof(width);
  width.kind = BrigEOperandImmed;
  width.type = Brigb32;
  width.reserved = 0;
  memset(&width.bits, 0, sizeof(width.bits));

  outputArgs.size = sizeof(outputArgs);
  outputArgs.kind = BrigEOperandArgumentList;
  outputArgs.elementCount = 1;
  outputArgs.o_args[0] = sizeof(reg) + sizeof(width);

  BrigOperandArgumentRef* outputArgsList2 = 
    new BrigOperandArgumentRef[outputArgs.elementCount * sizeof(argRef)];
  for (uint32_t i = 0 ; i < inputArgs.elementCount ; ++i) {
    outputArgsList2[i].size = sizeof(outputArgsList2[i]);
    outputArgsList2[i].kind = BrigEOperandArgumentRef;
    outputArgsList2[i].arg = 0;
  }

  inputArgs.size = sizeof(inputArgs);
  inputArgs.kind = BrigEOperandArgumentList;
  inputArgs.elementCount = 1;
  inputArgs.o_args[0] = outMod.o_operands[1] + sizeof(outputArgs);

  BrigOperandArgumentRef* inputArgsList2 = 
    new BrigOperandArgumentRef[inputArgs.elementCount * sizeof(argRef)];
  for (uint32_t i = 0 ; i < inputArgs.elementCount ; ++i) {
    inputArgsList2[i].size = sizeof(inputArgsList2[i]);
    inputArgsList2[i].kind = BrigEOperandArgumentRef;
    inputArgsList2[i].arg = 0;
  }

  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb32;
  reg.reserved = 0;
  reg.s_name = 0;

  funList.size = sizeof(funList);
  funList.kind = BrigEOperandFunctionList;
  funList.elementCount = 1;
  funList.o_args[0] = outMod.o_operands[3] + sizeof(inputArgs);

  BrigOperandFunctionRef* funcList2 = 
    new BrigOperandFunctionRef[funList.elementCount * sizeof(func)];
  for (uint32_t i = 0 ; i < funList.elementCount ; ++i) {
    funcList2[i].size = sizeof(funcList2[i]);
    funcList2[i].kind = BrigEOperandFunctionRef;
    funcList2[i].fn = 0;
  }

  symbolNameList.clear();
  symbolNameList.push_back("%out");
  symbolNameList.push_back("%in");
  Call_Test<BrigInstMod, BrigOperandReg, BrigOperandFunctionRef>
  TestCase2(in, symbols, &outMod, &width, &outputArgs, &reg, 
      &inputArgs, &funList, outputArgsList2, inputArgsList2, funcList2, &symbolNameList);
  TestCase2.Run_Test(&Call);

  delete []outputArgsList2;  outputArgsList2 = NULL;
  delete []inputArgsList2;  inputArgsList2 = NULL;
  delete []funcList2;  funcList2 = NULL;
  symbols->clear();

  /********************** Test Case 3 ****************************/
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
  width.bits.u = 1;

  outputArgs.size = sizeof(outputArgs);
  outputArgs.kind = BrigEOperandArgumentList;
  outputArgs.elementCount = 1;
  outputArgs.o_args[0] = sizeof(func) + sizeof(width);

  BrigOperandArgumentRef* outputArgsList3 = 
    new BrigOperandArgumentRef[outputArgs.elementCount * sizeof(argRef)];
  for (uint32_t i = 0 ; i < inputArgs.elementCount ; ++i) {
    outputArgsList3[i].size = sizeof(outputArgsList3[i]);
    outputArgsList3[i].kind = BrigEOperandArgumentRef;
    outputArgsList3[i].arg = 0;
  }

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

  BrigOperandArgumentRef* inputArgsList3 = 
    new BrigOperandArgumentRef[pInputArgs->elementCount * sizeof(argRef)];
  for (uint32_t i = 0 ; i < pInputArgs->elementCount ; ++i) {
    inputArgsList3[i].size = sizeof(inputArgsList3[i]);
    inputArgsList3[i].kind = BrigEOperandArgumentRef;
    inputArgsList3[i].arg = 0;
  }

  func.size = sizeof(func);
  func.kind = BrigEOperandFunctionRef;
  func.fn = 0;

  symbolNameList.clear();
  symbolNameList.push_back("%out");
  symbolNameList.push_back("%in1");
  symbolNameList.push_back("%in2");
  symbolNameList.push_back("%in3");
  symbolNameList.push_back("%in4");
  symbolNameList.push_back("%in5");
  symbolNameList.push_back("%in6");
  symbolNameList.push_back("%in7");
  symbolNameList.push_back("%in8");
  Call_Test<BrigInstBase, BrigOperandFunctionRef>
  TestCase3(in, symbols, &outBase, &width, &outputArgs, &func, pInputArgs, NULL, 
      outputArgsList3, inputArgsList3, NULL, &symbolNameList);
  TestCase3.Run_Test(&Call);

  delete []outputArgsList3;  outputArgsList3 = NULL;
  delete []inputArgsList3;  inputArgsList3 = NULL;
  symbols->clear();


  /********************** Test Case 4 **************************/

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
  width.bits.u = 1;

  inputArgs.size = sizeof(inputArgs);
  inputArgs.kind = BrigEOperandArgumentList;
  inputArgs.elementCount = 1;
  inputArgs.o_args[0] = sizeof(width) + sizeof(func);

  BrigOperandArgumentRef* inputArgsList4 = 
    new BrigOperandArgumentRef[inputArgs.elementCount * sizeof(argRef)];
  for (uint32_t i = 0 ; i < inputArgs.elementCount ; ++i) {
    inputArgsList4[i].size = sizeof(inputArgsList4[i]);
    inputArgsList4[i].kind = BrigEOperandArgumentRef;
    inputArgsList4[i].arg = 0;
  }

  func.size = sizeof(func);
  func.kind = BrigEOperandFunctionRef;
  func.fn = 0;

  symbolNameList.clear();
  symbolNameList.push_back("%in");
  Call_Test<BrigInstBase, BrigOperandFunctionRef>
  TestCase4(in, symbols, &outBase, &width, &emptyArgs, &func, &inputArgs, 
      NULL, NULL, inputArgsList4, NULL, &symbolNameList);
  TestCase4.Run_Test(&Call);

  delete []inputArgsList4;  inputArgsList4 = NULL;
  symbols->clear();

  /********************** Test Case 5 ****************************/
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
  width.bits.u = 1;

  func.size = sizeof(func);
  func.kind = BrigEOperandFunctionRef;
  func.fn = 0;

  Call_Test<BrigInstBase, BrigOperandFunctionRef>
  TestCase5(in, symbols, &outBase, &width, &emptyArgs, &func, &emptyArgs, NULL);
  TestCase5.Run_Test(&Call);

  symbols->clear();
  /************************ Test Case 6 **********************/
  in.assign("call $d1 (%in) [&foo, &bar];\n");
  regName.assign("$d1");
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
  width.bits.u = 1;

  inputArgs.size = sizeof(inputArgs);
  inputArgs.kind = BrigEOperandArgumentList;
  inputArgs.elementCount = 1;
  inputArgs.o_args[0] = sizeof(width) + sizeof(reg);

  BrigOperandArgumentRef* inputArgsList6 = 
    new BrigOperandArgumentRef[inputArgs.elementCount * sizeof(argRef)];
  for (uint32_t i = 0 ; i < inputArgs.elementCount ; ++i) {
    inputArgsList6[i].size = sizeof(inputArgsList6[i]);
    inputArgsList6[i].kind = BrigEOperandArgumentRef;
    inputArgsList6[i].arg = 0;
  }


  reg.size = sizeof(reg);
  reg.kind = BrigEOperandReg;
  reg.type = Brigb64;
  reg.reserved = 0;
  reg.s_name = 0;

  unsigned char mem6[sizeof(funList) + sizeof(BrigoOffset32_t)];
  pFunList = reinterpret_cast<BrigOperandArgumentList*>(mem6);
  pFunList->size = sizeof(funList) + sizeof(BrigoOffset32_t);
  pFunList->kind = BrigEOperandFunctionList;
  pFunList->elementCount = 2;
  pFunList->o_args[0] = outBase.o_operands[1] + sizeof(emptyArgs);
  pFunList->o_args[1] = outBase.o_operands[1] + sizeof(emptyArgs) + sizeof(func);

  BrigOperandFunctionRef* funcList6 = 
    new BrigOperandFunctionRef[pFunList->elementCount * sizeof(func)];
  for (uint32_t i = 0 ; i < pFunList->elementCount ; ++i) {
    funcList6[i].size = sizeof(funcList6[i]);
    funcList6[i].kind = BrigEOperandFunctionRef;
    funcList6[i].fn = 0;
  }

  symbolNameList.clear();
  symbolNameList.push_back("%in");
  Call_Test<BrigInstBase, BrigOperandReg, BrigOperandFunctionRef>
  TestCase6(in, symbols, &outBase, &width, &emptyArgs, &reg, &inputArgs, 
      pFunList, NULL, inputArgsList6, funcList6, &symbolNameList);

  TestCase6.Run_Test(&Call);

  delete []inputArgsList1;  inputArgsList1 = NULL;
  delete []funcList1;  funcList1 = NULL;
  symbols->clear();

  delete symbols;
}
TEST(ErrorReportTest, Call) {  
  std::vector<std::string> symNameList;
  std::string input = "call &bar(%in)\n";
  symNameList.clear();
  symNameList.push_back("%in");
  Call_Test<> TestCase1(input, &symNameList);
  TestCase1.Run_Test(&Call, MISSING_SEMICOLON);

  input.assign("call_width(64) (%in) [&foo, &bar];\n");
  symNameList.clear();
  symNameList.push_back("%in");
  Call_Test<> TestCase2(input, &symNameList);
  TestCase2.Run_Test(&Call, MISSING_OPERAND);

  input.assign("call &foo(%out)(%in1,%in2;\n");
  symNameList.clear();
  symNameList.push_back("%out");
  symNameList.push_back("%in1");
  symNameList.push_back("%in2");
  Call_Test<> TestCase3(input);
  TestCase3.Run_Test(&Call, INVALID_CALL_ARGS);
}
} // namespace hsa
} // namespace brig




