#include <float.h>
#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa{
namespace brig{

//T = BrigDirectivefunction/BrigDirectiveSymbol/BrigDirectiveSignature

template <typename T=BrigDirectiveSymbol,
	  typename TInit=BrigDirectiveInit>
class GlobalDecl_Test: public BrigCodeGenTest{
private:
  T* RefDir;

  BrigDirectiveSymbol* RefArgsList;
  BrigDirectiveLabel* RefLabList;
  uint32_t labListSize;
  TInit* RefInit;

public:
  GlobalDecl_Test(std::string& in):
    BrigCodeGenTest(in) {}

  GlobalDecl_Test(std::string& in, StringBuffer* sbuf,  T* out, TInit* d_init=NULL, 
                  BrigDirectiveLabel* labList=NULL, uint32_t size=0):
    BrigCodeGenTest(in, sbuf),
    RefDir(out),
    RefArgsList(NULL),
    RefLabList(labList),
    labListSize(size),
    RefInit(d_init)   { }

  GlobalDecl_Test(std::string& in, StringBuffer* sbuf, T* out, BrigDirectiveSymbol* arglist) :
    BrigCodeGenTest(in, sbuf),
    RefDir(out),
    RefArgsList(arglist),
    RefLabList(NULL),
    labListSize(0),
    RefInit(NULL) { }

  void Run_Test(int (*Rule)(Context*)){
    Buffer* dir = new Buffer();

    dir->append(RefDir);
    if (RefArgsList)
      dir->append(RefArgsList);
    if (RefLabList != NULL) {
      for (uint32_t i = 0 ; i < labListSize ; ++i ) {
	dir->append(&RefLabList[i]);
      }
    }
    if (RefInit)
      dir->append(RefInit);

    struct BrigSections RefOutput(reinterpret_cast<const char *>(&RefStr->get()[0]),
      reinterpret_cast<const char *>(&dir->get()[0]), NULL, NULL, NULL,
      RefStr->size(), dir->size(), (size_t)0, (size_t)0, (size_t)0);

    Parse_Validate(Rule, &RefOutput);
    delete dir;
  }
  void Run_Test(int (*Rule)(Context*), error_code_t refError){
    False_Validate(Rule, refError);
  }
};

TEST(CodegenTest, GlobalSymbolDecl_Codegen){

  std::string in, name;
  StringBuffer *sbuf = new StringBuffer();

  in.assign("align 1 static group_s8 &tmp[2];\n");
  name.assign("&tmp"); sbuf->append(name);

  BrigDirectiveSymbol ref = {

    0,                       // size
    BrigEDirectiveSymbol ,    // kind
    {
      0,                      // c_code
      BrigGroupSpace,         // storag class
      BrigStatic,             // attribute
      0,                      // reserved
      BrigArray,              // symbolModifier
      2,                      // dim
      0,                      // s_name
      Brigs8,                // type
      1,                      // align
    },
    0,                        // d_init
    0,                        // reserved
  };
  ref.size = sizeof(ref);
  GlobalDecl_Test<BrigDirectiveSymbol> TestCase1(in, sbuf, &ref);
  TestCase1.Run_Test(&GlobalDecl);
  sbuf->clear();
  /*********************************Add more test cases ********************************/


  /********************************* Case 2 Group ********************************/
  in.assign("align 4 group_f32 &x2[4][1024];\n");
  name.assign("&x2"); sbuf->append(name);

  ref.size = sizeof(ref);
  ref.kind = BrigEDirectiveSymbol;
  ref.s.c_code = 0;
  ref.s.storageClass = BrigGroupSpace;
  ref.s.attribute = BrigNone;
  ref.s.reserved = 0;
  ref.s.symbolModifier = BrigArray;
  ref.s.dim = 4096;
  ref.s.s_name = 0;
  ref.s.type = Brigf32;
  ref.s.align = 4;
  ref.d_init = 0;
  ref.reserved = 0;
  GlobalDecl_Test<BrigDirectiveSymbol> TestCase2(in, sbuf, &ref);
  TestCase2.Run_Test(&GlobalDecl);
  sbuf->clear();

  /********************************* Case 3 Group ********************************/
  in.assign("group_u32 &x3;\n");
  name.assign("&x3"); sbuf->append(name);

  ref.size = sizeof(ref);
  ref.kind = BrigEDirectiveSymbol;
  ref.s.c_code = 0;
  ref.s.storageClass = BrigGroupSpace;
  ref.s.attribute = BrigNone;
  ref.s.reserved = 0;
  ref.s.symbolModifier = 0;
  ref.s.dim = 0;
  ref.s.s_name = 0;
  ref.s.type = Brigu32;
  ref.s.align = 1;
  ref.d_init = 0;
  ref.reserved = 0;
  GlobalDecl_Test<BrigDirectiveSymbol> TestCase3(in, sbuf, &ref);
  TestCase3.Run_Test(&GlobalDecl);
  sbuf->clear();
  
  in.assign("const align 8 extern group_u32 &x3;\n");
  name.assign("&x3"); sbuf->append(name);

  ref.size = sizeof(ref);
  ref.kind = BrigEDirectiveSymbol;
  ref.s.c_code = 0;
  ref.s.storageClass = BrigGroupSpace;
  ref.s.attribute = BrigExtern;
  ref.s.reserved = 0;
  ref.s.symbolModifier = BrigConst;
  ref.s.dim = 0;
  ref.s.s_name = 0;
  ref.s.type = Brigu32;
  ref.s.align = 8;
  ref.d_init = 0;
  ref.reserved = 0;
  GlobalDecl_Test<BrigDirectiveSymbol> TestCase4(in, sbuf, &ref);
  TestCase4.Run_Test(&GlobalDecl);
  sbuf->clear();

  /********************************* Case 4 Group ********************************/
  in.assign("group_b8 &x4[];\n");
  name.assign("&x4"); sbuf->append(name);

  ref.size = sizeof(ref);
  ref.kind = BrigEDirectiveSymbol;
  ref.s.c_code = 0;
  ref.s.storageClass = BrigGroupSpace;
  ref.s.attribute = BrigNone;
  ref.s.reserved = 0;
  ref.s.symbolModifier = BrigFlex;
  ref.s.dim = 0;
  ref.s.s_name = 0;
  ref.s.type = Brigb8;
  ref.s.align = 1;
  ref.d_init = 0;
  ref.reserved = 0;
  GlobalDecl_Test<BrigDirectiveSymbol> TestCase5(in, sbuf, &ref);
  TestCase5.Run_Test(&GlobalDecl);
  sbuf->clear();

  /********************************* Case 5 Private ********************************/
  in.assign("private_b8 &y5[8];\n");
  name.assign("&y5"); sbuf->append(name);

  ref.size = sizeof(ref);
  ref.kind = BrigEDirectiveSymbol;
  ref.s.c_code = 0;
  ref.s.storageClass = BrigPrivateSpace;
  ref.s.attribute = BrigNone;
  ref.s.reserved = 0;
  ref.s.symbolModifier = BrigArray;
  ref.s.dim = 8;
  ref.s.s_name = 0;
  ref.s.type = Brigb8;
  ref.s.align = 1;
  ref.d_init = 0;
  ref.reserved = 0;
  GlobalDecl_Test<BrigDirectiveSymbol> TestCase6(in, sbuf, &ref);
  TestCase6.Run_Test(&GlobalDecl);
  sbuf->clear();

  /********************************* Case 6 Private ********************************/
  in.assign("static private_b64 &y5[8][256];\n");
  name.assign("&y5"); sbuf->append(name);

  ref.size = sizeof(ref);
  ref.kind = BrigEDirectiveSymbol;
  ref.s.c_code = 0;
  ref.s.storageClass = BrigPrivateSpace;
  ref.s.attribute = BrigStatic;
  ref.s.reserved = 0;
  ref.s.symbolModifier = BrigArray;
  ref.s.dim = 2048;
  ref.s.s_name = 0;
  ref.s.type = Brigb64;
  ref.s.align = 1;
  ref.d_init = 0;
  ref.reserved = 0;
  GlobalDecl_Test<BrigDirectiveSymbol> TestCase7(in, sbuf, &ref);
  TestCase7.Run_Test(&GlobalDecl);
  sbuf->clear();

  in.assign("const private_b64 &y5[8][256];\n");
  name.assign("&y5"); sbuf->append(name);

  ref.size = sizeof(ref);
  ref.kind = BrigEDirectiveSymbol;
  ref.s.c_code = 0;
  ref.s.storageClass = BrigPrivateSpace;
  ref.s.attribute = BrigNone;
  ref.s.reserved = 0;
  ref.s.symbolModifier = BrigArray | BrigConst;
  ref.s.dim = 2048;
  ref.s.s_name = 0;
  ref.s.type = Brigb64;
  ref.s.align = 1;
  ref.d_init = 0;
  ref.reserved = 0;
  GlobalDecl_Test<BrigDirectiveSymbol> TestCase8(in, sbuf, &ref);
  TestCase8.Run_Test(&GlobalDecl);
  sbuf->clear();
  

  /******************************************** End of tests ********************************/
  delete sbuf;
}


TEST(CodegenTest, FunctionDecl_Codegen){

  std::string in, name;
  StringBuffer *sbuf = new StringBuffer();

  in.assign("function &callee()(); \n");
  name.assign("&callee");
  sbuf->append(name);
  BrigDirectiveFunction ref = {
    sizeof(BrigDirectiveFunction),                       // size
    BrigEDirectiveFunction,   // kind
    0,                       // c_code
    0,                        // s_name
    0,                        // inParamCount
    sizeof(BrigDirectiveFunction),                      // d_firstScopedDirective
    0,                        // operationCount
    sizeof(BrigDirectiveFunction),                      // d_nextDirective
    BrigNone,
    0,
    0,                        // outParamCount
    0,
  };
  GlobalDecl_Test<BrigDirectiveFunction> TestCase1(in, sbuf, &ref);
  TestCase1.Run_Test(&GlobalDecl);
  sbuf->clear();
  /***************************************************Test case 2********************************/

  std::string arg1, arg2;
  in.assign("extern function &callee(arg_u32 %val1)(arg_u16 %val2);\n ");
  name.assign("&callee"); arg1.assign("%val1"); arg2.assign("%val2");
  sbuf->append(name); sbuf->append(arg1); sbuf->append(arg2);
  int dir_size = sizeof(BrigDirectiveFunction) + sizeof(BrigDirectiveSymbol)*2;
  BrigdOffset32_t firstInParam = sizeof(BrigDirectiveFunction) + sizeof(BrigDirectiveSymbol)* 1;
  BrigDirectiveFunction ref2 = {
    sizeof(BrigDirectiveFunction),                       // size
    BrigEDirectiveFunction,   // kind
    0,                       // c_code
    0,                        // s_name
    1,                        // inParamCount
    dir_size,                      // d_firstScopedDirective
    0,                        // operationCount
    dir_size,                      // d_nextDirective
    BrigExtern,
    0,
    1,                        // outParamCount
    firstInParam
  };
  BrigDirectiveSymbol args[2] = {
    //args[0]
    {0,
    BrigEDirectiveSymbol,
    { 0,
      BrigArgSpace,
      BrigNone,
      0,
      0,
      0,
      name.size()+1,
      Brigu32,
      1,
    },
    0,
    0
    },
    //args[1]
    {0,
    BrigEDirectiveSymbol,
    { 0,
      BrigArgSpace,
      BrigNone,
      0,
      0,
      0,
      name.size()+arg1.size()+2,
      Brigu16,
      1,
    },
    0,
    0
    }
  };
  args[0].size = sizeof(args[0]);
  args[1].size = sizeof(args[1]);

  GlobalDecl_Test<BrigDirectiveFunction> TestCase2(in, sbuf, &ref2, args);
  TestCase2.Run_Test(&GlobalDecl);
  sbuf->clear();
  /*********************************   Add more unit tests ****************************************/

  /********************************* Case 3 ****************************************/
  in.assign("extern function &foo ()();\n");
  name.assign("&foo");  sbuf->append(name);
  dir_size = sizeof(BrigDirectiveFunction);
  firstInParam = sizeof(BrigDirectiveFunction);
  BrigDirectiveFunction ref3 = {
    0,                                // size
    BrigEDirectiveFunction,           // kind
    0,                                // c_code
    0,                                // s_name
    0,                                // inParamCount
    dir_size,                         // d_firstScopedDirective
    0,                                // operationCount
    dir_size,                         // d_nextDirective
    BrigExtern,                       // attribute
    0,                                // fbarCount
    0,                                // outParamCount
    0                                 // d_firstInParam
  };
  ref3.size = sizeof(ref3);

  GlobalDecl_Test<BrigDirectiveFunction> TestCase3(in, sbuf, &ref3);
  TestCase3.Run_Test(&GlobalDecl);
  sbuf->clear();

  /********************************* Case 4 ****************************************/
  in.assign("function &foo4 (arg_u32 %x[64])(arg_u32 %z1, arg_b8 %map[64][64] ,");
  in.append("arg_b32 %value[64], arg_b1 %jud1, arg_b1 %jud2);\n");
  std::string argsName[6];

  name.assign("&foo4");             sbuf->append(name);
  argsName[0].assign("%x");         sbuf->append(argsName[0]);
  argsName[1].assign("%z1");        sbuf->append(argsName[1]);
  argsName[2].assign("%map");       sbuf->append(argsName[2]);
  argsName[3].assign("%value");     sbuf->append(argsName[3]);
  argsName[4].assign("%jud1");      sbuf->append(argsName[4]);
  argsName[5].assign("%jud2");      sbuf->append(argsName[5]);

  BrigDirectiveSymbol args4[6];  // One output arg, Five input args
  // args[0] is the output arg.
  BrigDirectiveFunction ref4 = {
    0,                                // size
    BrigEDirectiveFunction,           // kind
    0,                                // c_code
    0,                                // s_name
    5,                                // inParamCount
    0,                                // d_firstScopedDirective
    0,                                // operationCount
    0,                                // d_nextDirective
    BrigNone,                         // attribute
    0,                                // reserved
    1,                                // outParamCount
    0                                 // d_firstInParam
  };
  ref4.size = sizeof(ref4);
  ref4.d_firstScopedDirective = sizeof(ref4) + sizeof(args4[0]) + sizeof(args4[1])
                                             + sizeof(args4[2]) + sizeof(args4[3])
                                             + sizeof(args4[4]) + sizeof(args4[5]);
  ref4.d_nextDirective = ref4.d_firstScopedDirective;
  ref4.d_firstInParam = sizeof(ref4) + sizeof(args4[0]);
  /*********** args init ************/
  args4[0].size = sizeof(args4[0]);
  args4[0].kind = BrigEDirectiveSymbol;
  args4[0].s.c_code = 0;
  args4[0].s.storageClass = BrigArgSpace;
  args4[0].s.attribute = BrigNone;
  args4[0].s.reserved = 0;
  args4[0].s.symbolModifier = BrigArray;
  args4[0].s.dim = 64;
  args4[0].s.s_name = name.size() + 1;
  args4[0].s.type = Brigu32;
  args4[0].s.align = 1;
  args4[0].d_init = 0;
  args4[0].reserved = 0;

  args4[1].size = sizeof(args4[1]);
  args4[1].kind = BrigEDirectiveSymbol;
  args4[1].s.c_code = 0;
  args4[1].s.storageClass = BrigArgSpace;
  args4[1].s.attribute = BrigNone;
  args4[1].s.reserved = 0;
  args4[1].s.symbolModifier = 0;
  args4[1].s.dim = 0;
  args4[1].s.s_name = name.size() + argsName[0].size() + 2;
  args4[1].s.type = Brigu32;
  args4[1].s.align = 1;
  args4[1].d_init = 0;
  args4[1].reserved = 0;

  args4[2].size = sizeof(args4[2]);
  args4[2].kind = BrigEDirectiveSymbol;
  args4[2].s.c_code = 0;
  args4[2].s.storageClass = BrigArgSpace;
  args4[2].s.attribute = BrigNone;
  args4[2].s.reserved = 0;
  args4[2].s.symbolModifier = BrigArray;
  args4[2].s.dim = 4096;
  args4[2].s.s_name = args4[1].s.s_name + argsName[1].size() + 1;
  args4[2].s.type = Brigb8;
  args4[2].s.align = 1;
  args4[2].d_init = 0;
  args4[2].reserved = 0;

  args4[3].size = sizeof(args4[3]);
  args4[3].kind = BrigEDirectiveSymbol;
  args4[3].s.c_code = 0;
  args4[3].s.storageClass = BrigArgSpace;
  args4[3].s.attribute = BrigNone;
  args4[3].s.reserved = 0;
  args4[3].s.symbolModifier = BrigArray;
  args4[3].s.dim = 64;
  args4[3].s.s_name = args4[2].s.s_name + argsName[2].size() + 1;
  args4[3].s.type = Brigb32;
  args4[3].s.align = 1;
  args4[3].d_init = 0;
  args4[3].reserved = 0;

  args4[4].size = sizeof(args4[4]);
  args4[4].kind = BrigEDirectiveSymbol;
  args4[4].s.c_code = 0;
  args4[4].s.storageClass = BrigArgSpace;
  args4[4].s.attribute = BrigNone;
  args4[4].s.reserved = 0;
  args4[4].s.symbolModifier = 0;
  args4[4].s.dim = 0;
  args4[4].s.s_name = args4[3].s.s_name + argsName[3].size() + 1;
  args4[4].s.type = Brigb1;
  args4[4].s.align = 1;
  args4[4].d_init = 0;
  args4[4].reserved = 0;

  args4[5].size = sizeof(args4[5]);
  args4[5].kind = BrigEDirectiveSymbol;
  args4[5].s.c_code = 0;
  args4[5].s.storageClass = BrigArgSpace;
  args4[5].s.attribute = BrigNone;
  args4[5].s.reserved = 0;
  args4[5].s.symbolModifier = 0;
  args4[5].s.dim = 0;
  args4[5].s.s_name = args4[4].s.s_name + argsName[4].size() + 1;
  args4[5].s.type = Brigb1;
  args4[5].s.align = 1;
  args4[5].d_init = 0;
  args4[5].reserved = 0;

  GlobalDecl_Test<BrigDirectiveFunction> TestCase4(in, sbuf, &ref4, args4);
  TestCase4.Run_Test(&GlobalDecl);

  sbuf->clear();

  /************************************End all tests *****************************************/
  delete sbuf;
}

TEST(CodegenTest, functionSignature_Codegen){

  std::string in, name;
  StringBuffer *sbuf = new StringBuffer();

  /********************************* Case 1 ****************************************/
  in.assign("signature &bar_t (arg_u32 ) (align 8 arg_f32, arg_f32 %x[10]);\n");
  name.assign("&bar_t");  sbuf->append(name);

  char* pMem1 = new char[sizeof(BrigDirectiveSignature) +
                sizeof(BrigDirectiveSignature::BrigProtoType) * 2];

  BrigDirectiveSignature* pRef1 = reinterpret_cast<BrigDirectiveSignature*>(pMem1);

  pRef1->size = sizeof(BrigDirectiveSignature) + sizeof(BrigDirectiveSignature::BrigProtoType) * 2;
  pRef1->kind = BrigEDirectiveSignature;  // kind
  pRef1->c_code = 0;                      // c_code
  pRef1->s_name = 0;                      // s_name
  pRef1->outCount = 1;                    // outCount
  pRef1->inCount = 2;                     // inCount
  pRef1->types[0].type = Brigu32;         // type
  pRef1->types[0].align = 1;              // align
  pRef1->types[0].hasDim = 0;             // hasDim
  pRef1->types[0].dim = 0;                // dim

  pRef1->types[1].type = Brigf32;         // type
  pRef1->types[1].align = 8;              // align
  pRef1->types[1].hasDim = 0;             // hasDim
  pRef1->types[1].dim = 0;                // dim

  pRef1->types[2].type = Brigf32;         // type
  pRef1->types[2].align = 8;              // align
  pRef1->types[2].hasDim = 1;             // hasDim
  pRef1->types[2].dim = 10;               // dim

  GlobalDecl_Test<BrigDirectiveSignature> TestCase1(in, sbuf, pRef1);
  TestCase1.Run_Test(&GlobalDecl);
  delete []pMem1;  pMem1 = NULL; pRef1 = NULL;
  sbuf->clear();

  /********************************* Case 2 ****************************************/
  in.assign("signature &fun_t (arg_u32) (arg_u32, arg_u32);\n");
  name.assign("&fun_t");  sbuf->append(name);

  char* pMem2 = new char[sizeof(BrigDirectiveSignature) + sizeof(BrigDirectiveSignature::BrigProtoType) * 2];

  BrigDirectiveSignature* pRef2 = reinterpret_cast<BrigDirectiveSignature*>(pMem2);

  pRef2->size = sizeof(BrigDirectiveSignature) + sizeof(BrigDirectiveSignature::BrigProtoType) * 2;
  pRef2->kind = BrigEDirectiveSignature;  // kind
  pRef2->c_code = 0;                      // c_code
  pRef2->s_name = 0;                      // s_name
  pRef2->outCount = 1;                    // outCount
  pRef2->inCount = 2;                     // inCount
  pRef2->types[0].type = Brigu32;         // type
  pRef2->types[0].align = 1;              // align
  pRef2->types[0].hasDim = 0;             // hasDim
  pRef2->types[0].dim = 0;                // dim

  pRef2->types[1].type = Brigu32;         // type
  pRef2->types[1].align = 1;              // align
  pRef2->types[1].hasDim = 0;             // hasDim
  pRef2->types[1].dim = 0;                // dim

  pRef2->types[2].type = Brigu32;         // type
  pRef2->types[2].align = 1;              // align
  pRef2->types[2].hasDim = 0;             // hasDim
  pRef2->types[2].dim = 0;                // dim

  GlobalDecl_Test<BrigDirectiveSignature> TestCase2(in, sbuf, pRef2);
  TestCase2.Run_Test(&GlobalDecl);
  delete []pMem2;  pMem2 = NULL;  pRef2 = NULL;
  sbuf->clear();

  /************************************End all tests *****************************************/
  delete sbuf;
}

TEST(CodegenTest, InitializableDecl_Codegen){

  std::string in, name;
  StringBuffer* sbuf = new StringBuffer();

  in.assign("global_b8 &x[9] = { 1,2,3,4,5,6,7,8,9 }; ");
  name.assign("&x");
  sbuf->append(name);

  BrigDirectiveSymbol ref = {
  0,                          // size
  BrigEDirectiveSymbol,       // kind
  {
    0,                        // c_code
    BrigGlobalSpace,          // storag class
    BrigNone,                 // attribut
    0,                        // reserved
    BrigArray,                // symbolModifier
    9,                        // dim
    0,                        // s_name
    Brigb8,                   // type
    1,                        // align
  },
  0,                          // d_init
  0,                          // reserved
  };
  ref.size = sizeof(ref);
  ref.d_init = sizeof(ref);
  BrigDirectiveInit* bdi = (BrigDirectiveInit*) (malloc(sizeof(BrigDirectiveInit) + sizeof(uint64_t)));

  bdi->size = sizeof(BrigDirectiveInit) + sizeof(uint64_t);   // size
  bdi->kind = BrigEDirectiveInit;                             // kind
  bdi->c_code = 0;                                            // c_code
  bdi->elementCount = 9;                                      // elementCount
  bdi->type = Brigb8;                                         // type
  bdi->reserved = 0;                                          // reserved
  bdi->initializationData.u8[0] = 1;                          // initializationData
  bdi->initializationData.u8[1] = 2;
  bdi->initializationData.u8[2] = 3;
  bdi->initializationData.u8[3] = 4;
  bdi->initializationData.u8[4] = 5;
  bdi->initializationData.u8[5] = 6;
  bdi->initializationData.u8[6] = 7;
  bdi->initializationData.u8[7] = 8;

  bdi->initializationData.u8[8] = 9;                          // initializationData
  bdi->initializationData.u8[9] = 0;
  bdi->initializationData.u8[10] = 0;
  bdi->initializationData.u8[11] = 0;
  bdi->initializationData.u8[12] = 0;
  bdi->initializationData.u8[13] = 0;
  bdi->initializationData.u8[14] = 0;
  bdi->initializationData.u8[15] = 0;

  GlobalDecl_Test<BrigDirectiveSymbol> TestCase1(in, sbuf, &ref, bdi);
  TestCase1.Run_Test(&GlobalDecl);
  free(bdi);
  sbuf->clear();
  /******************************************************* Add more tests *******************************************/
  /************************************************ Case 2 ***************************************/
  in.assign("global_f32 &x[9] = { 1.1f,2.2f,3.3f,4.4f,5.5f,6.6f,7.7f,8.8f,9.9f };");
  name.assign("&x"); sbuf->append(name);

  BrigDirectiveSymbol ref2 = {
  0,                        // size
  BrigEDirectiveSymbol,     // kind
  {
    0,                      // c_code
    BrigGlobalSpace,        // storage class
    BrigNone,               // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    9,                      // dim
    0,                      // s_name
    Brigf32,                // type
    1,                      // align
  },
  0,                        // d_init
  0,                        // reserved
  };
  ref2.size = sizeof(ref2);
  ref2.d_init = sizeof(ref2);

  BrigDirectiveInit* bdi2 = (BrigDirectiveInit*) (malloc(sizeof(BrigDirectiveInit) + sizeof(uint64_t) * 4));

  bdi2->size = sizeof(BrigDirectiveInit) + sizeof(uint64_t) * 4;  // size
  bdi2->kind = BrigEDirectiveInit;                                // kind
  bdi2->c_code = 0;                                               // c_code
  bdi2->elementCount = 9;                                         // elementCount
  bdi2->type = Brigb32;                                           // type
  bdi2->reserved = 0;                                             // reserved

  *(float*)&bdi2->initializationData.u32[0] = 1.1f;               // initializationData

  *(float*)&bdi2->initializationData.u32[1] = 2.2f;
  *(float*)&bdi2->initializationData.u32[2] = 3.3f;
  *(float*)&bdi2->initializationData.u32[3] = 4.4f;
  *(float*)&bdi2->initializationData.u32[4] = 5.5f;

  *(float*)&bdi2->initializationData.u32[5] = 6.6f;
  *(float*)&bdi2->initializationData.u32[6] = 7.7f;
  *(float*)&bdi2->initializationData.u32[7] = 8.8f;

  *(float*)&bdi2->initializationData.u32[8] = 9.9f;               // initializationData
  bdi2->initializationData.u32[9] = 0;

  GlobalDecl_Test<BrigDirectiveSymbol> TestCase2(in, sbuf, &ref2, bdi2);
  TestCase2.Run_Test(&GlobalDecl);
  free(bdi2);  bdi2 = NULL;
  sbuf->clear();

  /************************************************ Case 3 ***************************************/
  in.assign("global_f64 &x[9] = { 1.1l,2.2l,3.3l,4.4l,5.5l,6.6l,7.7l,8.8l,9.9l };");
  name.assign("&x"); sbuf->append(name);

  BrigDirectiveSymbol ref3 = {
  0,                        // size
  BrigEDirectiveSymbol,     // kind
  {
    0,                      // c_code
    BrigGlobalSpace,        // storage class
    BrigNone,               // attribute
    0,                      // reserved
    BrigArray,              // symbolModifier
    9,                      // dim
    0,                      // s_name
    Brigf64,                // type
    1,                      // align
  },
  0,                        // d_init
  0,                        // reserved
  };
  ref3.size = sizeof(ref3);
  ref3.d_init = sizeof(ref3);

  BrigDirectiveInit* bdi3 = (BrigDirectiveInit*) (malloc(sizeof(BrigDirectiveInit) + sizeof(uint64_t) * 8));

  bdi3->size = sizeof(BrigDirectiveInit) + sizeof(uint64_t) * 8;   // size
  bdi3->kind = BrigEDirectiveInit;                                 // kind
  bdi3->c_code = 0;                                                // c_code
  bdi3->elementCount = 9;                                          // elementCount
  bdi3->type = Brigb64;                                            // type
  bdi3->reserved = 0;                                              // reserved

  *(double*)&bdi3->initializationData.u64[0] = 1.1l;               // initializationData

  *(double*)&bdi3->initializationData.u64[1] = 2.2l;
  *(double*)&bdi3->initializationData.u64[2] = 3.3l;
  *(double*)&bdi3->initializationData.u64[3] = 4.4l;
  *(double*)&bdi3->initializationData.u64[4] = 5.5l;

  *(double*)&bdi3->initializationData.u64[5] = 6.6l;
  *(double*)&bdi3->initializationData.u64[6] = 7.7l;
  *(double*)&bdi3->initializationData.u64[7] = 8.8l;

  *(double*)&bdi3->initializationData.u64[8] = 9.9l;               // initializationData

  GlobalDecl_Test<BrigDirectiveSymbol> TestCase3(in, sbuf, &ref3, bdi3);
  TestCase3.Run_Test(&GlobalDecl);
  free(bdi3);  bdi3 = NULL;
  sbuf->clear();

  /************************************************ Case 4 ***************************************/
  in.assign("global_u32 &x0 = {3.0f};\n");
  name.assign("&x0"); sbuf->append(name);

  BrigDirectiveSymbol ref4 = {
  0,                           // size
  BrigEDirectiveSymbol ,       // kind
  {
    0,                         // c_code
    BrigGlobalSpace,           // storage class
    BrigNone ,                 // attribute
    0,                         // reserved
    0,                         // symbolModifier
    0,                         // dim
    0,                         // s_name
    Brigu32,                   // type
    1                          // align
  },
  0,                           // d_init
  0                            // reserved
  };

  ref4.size = sizeof(ref4);
  ref4.d_init = sizeof(ref4);

  BrigDirectiveInit* bdi4 = (BrigDirectiveInit*) (malloc(sizeof(BrigDirectiveInit)));

  bdi4->size = sizeof(BrigDirectiveInit);                          // size
  bdi4->kind = BrigEDirectiveInit;                                 // kind
  bdi4->c_code = 0;                                                // c_code
  bdi4->elementCount = 1;                                          // elementCount
  bdi4->type = Brigb32;                                            // type
  bdi4->reserved = 0;                                              // reserved

  bdi4->initializationData.u32[0] = 0x40400000;                    // initializationData
  bdi4->initializationData.u32[1] = 0;

  GlobalDecl_Test<BrigDirectiveSymbol> TestCase4(in, sbuf, &ref4, bdi4);
  TestCase4.Run_Test(&GlobalDecl);
  free(bdi4);  bdi4 = NULL;
  sbuf->clear();

  /************************************************ Case 5 ***************************************/
  in.assign("readonly_b8 &x1[] = {1, 3, 0xFF1, 0, -2};\n");
  name.assign("&x1"); sbuf->append(name);

  BrigDirectiveSymbol ref5 = {
  0,                           // size
  BrigEDirectiveSymbol ,       // kind
  {
    0,                         // c_code
    BrigReadonlySpace,         // storage class
    BrigNone ,                 // attribute
    0,                         // reserved
    BrigArray,                 // symbolModifier
    5,                         // dim
    0,                         // s_name
    Brigb8,                    // type
    1                          // align
  },
  0,                           // d_init
  0                            // reserved
  };

  ref5.size = sizeof(ref5);
  ref5.d_init = sizeof(ref5);

  BrigDirectiveInit* bdi5 = (BrigDirectiveInit*) (malloc(sizeof(BrigDirectiveInit)));

  bdi5->size = sizeof(BrigDirectiveInit);                          // size
  bdi5->kind = BrigEDirectiveInit;                                 // kind
  bdi5->c_code = 0;                                                // c_code
  bdi5->elementCount = 5;                                          // elementCount
  bdi5->type = Brigb8;                                             // type
  bdi5->reserved = 0;                                              // reserved

  bdi5->initializationData.u8[0] = 1;    // initializationData
  bdi5->initializationData.u8[1] = 3;
  bdi5->initializationData.u8[2] = 0xF1;
  bdi5->initializationData.u8[3] = 0;
  bdi5->initializationData.u8[4] = 0xFE;
  bdi5->initializationData.u8[5] = 0;
  bdi5->initializationData.u8[6] = 0;
  bdi5->initializationData.u8[7] = 0;

  GlobalDecl_Test<BrigDirectiveSymbol> TestCase5(in, sbuf, &ref5, bdi5);
  TestCase5.Run_Test(&GlobalDecl);
  free(bdi5);  bdi5 = NULL;
  sbuf->clear();

  /************************************************ Case 6 ***************************************/
  in.assign("global_b16 &x2[17] = {0xF7F7, 3, 0xFFFF1, 0, -3, 321};\n");
  name.assign("&x2"); sbuf->append(name);

  BrigDirectiveSymbol ref6 = {
  0,                           // size
  BrigEDirectiveSymbol ,       // kind
  {
    0,                         // c_code
    BrigGlobalSpace,           // storage class
    BrigNone ,                 // attribute
    0,                         // reserved
    BrigArray,                 // symbolModifier
    17,                        // dim
    0,                         // s_name
    Brigb16,                   // type
    1                          // align
  },
  0,                           // d_init
  0                            // reserved
  };

  ref6.size = sizeof(ref6);
  ref6.d_init = sizeof(ref6);

  BrigDirectiveInit* bdi6 = (BrigDirectiveInit*) (malloc(sizeof(BrigDirectiveInit) + sizeof(uint64_t) * 4));

  bdi6->size = sizeof(BrigDirectiveInit) + sizeof(uint64_t) * 4;   // size
  bdi6->kind = BrigEDirectiveInit;                                 // kind
  bdi6->c_code = 0;                                                // c_code
  bdi6->elementCount = 17;                                         // elementCount
  bdi6->type = Brigb16;                                            // type
  bdi6->reserved = 0;                                              // reserved

  bdi6->initializationData.u16[0] = 0xF7F7;                     // initializationData
  bdi6->initializationData.u16[1] = 3;
  bdi6->initializationData.u16[2] = 0xFFF1;
  bdi6->initializationData.u16[3] = 0;
  bdi6->initializationData.u16[4] = 0xFFFD;
  bdi6->initializationData.u16[5] = 321;
  for (int i = 6 ; i < 20 ; ++i) {
    bdi6->initializationData.u16[i] = 0;
  }

  GlobalDecl_Test<BrigDirectiveSymbol> TestCase6(in, sbuf, &ref6, bdi6);
  TestCase6.Run_Test(&GlobalDecl);
  free(bdi6);  bdi6 = NULL;
  sbuf->clear();
  /************************************************ Case 7 ***************************************/
  in.assign("global_b64 &x2[5] = {0x1, 0x0, 0x123456789ABCDEF0, 0xFEDCBA01234, 0xFFFFFFFFFFFFFFFF};\n");
  name.assign("&x2"); sbuf->append(name);

  BrigDirectiveSymbol ref7 = {
  0,                           // size
  BrigEDirectiveSymbol ,       // kind
  {
    0,                         // c_code
    BrigGlobalSpace,           // storage class
    BrigNone ,                 // attribute
    0,                         // reserved
    BrigArray,                 // symbolModifier
    5,                         // dim
    0,                         // s_name
    Brigb64,                   // type
    1                          // align
  },
  0,                           // d_init
  0                            // reserved
  };

  ref7.size = sizeof(ref7);
  ref7.d_init = sizeof(ref7);

  BrigDirectiveInit* bdi7 = (BrigDirectiveInit*) (malloc(sizeof(BrigDirectiveInit) + sizeof(uint64_t) * 4));

  bdi7->size = sizeof(BrigDirectiveInit) + sizeof(uint64_t) * 4;   // size
  bdi7->kind = BrigEDirectiveInit;                                 // kind
  bdi7->c_code = 0;                                                // c_code
  bdi7->elementCount = 5;                                         // elementCount
  bdi7->type = Brigb64;                                            // type
  bdi7->reserved = 0;                                              // reserved

  bdi7->initializationData.u64[0] = 0x1;                     // initializationData
  bdi7->initializationData.u64[1] = 0x0;
  bdi7->initializationData.u64[2] = 0x123456789ABCDEF0;
  bdi7->initializationData.u64[3] = 0xFEDCBA01234; 
  bdi7->initializationData.u64[4] = 0xFFFFFFFFFFFFFFFF;

  GlobalDecl_Test<BrigDirectiveSymbol> TestCase7(in, sbuf, &ref7, bdi7);
  TestCase7.Run_Test(&GlobalDecl);
  free(bdi7);  bdi7 = NULL;
  sbuf->clear();

  /************************************************ Case 8 ***************************************/
  in.assign("global_f32 &n = -3.4028234663852e+38f;\n");
  name.assign("&n"); sbuf->append(name);

  BrigDirectiveSymbol ref8 = {
    0,                           // size
    BrigEDirectiveSymbol ,       // kind
    {
      0,                         // c_code
      BrigGlobalSpace,           // storage class
      BrigNone ,                 // attribute
      0,                         // reserved
      0,                         // symbolModifier
      0,                         // dim
      0,                         // s_name
      Brigf32,                   // type
      1                          // align
    },
    0,                           // d_init
    0                            // reserved
  };

  ref8.size = sizeof(ref8);
  ref8.d_init = sizeof(ref8);

  BrigDirectiveInit* bdi8 = (BrigDirectiveInit*) (malloc(sizeof(BrigDirectiveInit)));

  bdi8->size = sizeof(BrigDirectiveInit);   // size
  bdi8->kind = BrigEDirectiveInit;                                 // kind
  bdi8->c_code = 0;                                                // c_code
  bdi8->elementCount = 1;                                         // elementCount
  bdi8->type = Brigb32;                                            // type
  bdi8->reserved = 0;                                              // reserved

  *(float*)&bdi8->initializationData.u32[0] = -FLT_MAX;                     // initializationData
  bdi8->initializationData.u32[1] = 0;                     // initializationData

  GlobalDecl_Test<BrigDirectiveSymbol> TestCase8(in, sbuf, &ref8, bdi8);
  TestCase8.Run_Test(&GlobalDecl);
  free(bdi8);  bdi8 = NULL;
  sbuf->clear();

  /************************************************ Case 9 ***************************************/
  in.assign("global_f64 &n = -1.79769e+308;\n");
  name.assign("&n"); sbuf->append(name);

  BrigDirectiveSymbol ref9 = {
    0,                           // size
    BrigEDirectiveSymbol ,       // kind
    {
      0,                         // c_code
      BrigGlobalSpace,           // storage class
      BrigNone ,                 // attribute
      0,                         // reserved
      0,                         // symbolModifier
      0,                         // dim
      0,                         // s_name
      Brigf64,                   // type
      1                          // align
    },
    0,                           // d_init
    0                            // reserved
  };

  ref9.size = sizeof(ref9);
  ref9.d_init = sizeof(ref9);

  BrigDirectiveInit* bdi9 = (BrigDirectiveInit*) (malloc(sizeof(BrigDirectiveInit)));

  bdi9->size = sizeof(BrigDirectiveInit);   // size
  bdi9->kind = BrigEDirectiveInit;                                 // kind
  bdi9->c_code = 0;                                                // c_code
  bdi9->elementCount = 1;                                         // elementCount
  bdi9->type = Brigb64;                                            // type
  bdi9->reserved = 0;                                              // reserved

  *(double*)&bdi9->initializationData.u64[0] = -1.79769e+308;                     // initializationData

  GlobalDecl_Test<BrigDirectiveSymbol> TestCase9(in, sbuf, &ref9, bdi9);
  TestCase9.Run_Test(&GlobalDecl);
  free(bdi9);  bdi9 = NULL;
  sbuf->clear();

  /************************************************ Case 10 ***************************************/

  BrigDirectiveLabel labList10[2];
  in.assign("global_u32 %tab[] = {@even, @odd};\n");
  name.assign("%tab"); sbuf->append(name);

  BrigDirectiveSymbol ref10 = {
    0,                        // size
    BrigEDirectiveSymbol,     // kind
    {
      0,                      // c_code
      BrigGlobalSpace,        // storage class
      BrigNone,               // attribute
      0,                      // reserved
      BrigArray,              // symbolModifier
      2,                      // dim
      0,                      // s_name
      Brigu32,                // type
      1,                      // align
    },
    0,                        // d_init
    0                         // reserved
  };
  ref10.size = sizeof(ref10);
  ref10.d_init = sizeof(ref10) + sizeof(labList10[0]) + sizeof(labList10[1]);

  size_t arraySize = sizeof(BrigDirectiveLabelInit) + sizeof(BrigdOffset32_t);
  BrigDirectiveLabelInit* bdli10 = (BrigDirectiveLabelInit*) (malloc(arraySize));

  bdli10->size = arraySize;                              // size
  bdli10->kind = BrigEDirectiveLabelInit;                // kind
  bdli10->c_code = 0;                                    // c_code
  bdli10->elementCount = 2;                              // elementCount
  bdli10->s_name = 0;
  bdli10->d_labels[0] = sizeof(ref10);        // d_labels
  bdli10->d_labels[1] = sizeof(ref10) + sizeof(BrigDirectiveLabel);
  labList10[0].size = sizeof(labList10[0]);
  labList10[0].kind = BrigEDirectiveLabel;
  labList10[0].c_code = 0;
  labList10[0].s_name = 0;
  labList10[1].size = sizeof(labList10[1]);
  labList10[1].kind = BrigEDirectiveLabel;
  labList10[1].c_code = 0;
  labList10[1].s_name = 0;

  GlobalDecl_Test<BrigDirectiveSymbol, BrigDirectiveLabelInit> 
    TestCase10(in, sbuf, &ref10, bdli10, labList10, 2);
  TestCase10.Run_Test(&GlobalDecl);
  free(bdli10);  bdli10 = NULL;
  sbuf->clear();
  /************************************************ Case 11 ***************************************/

  BrigDirectiveLabel labList11;
  in.assign("global_u32 %tab[3] = {@odd};\n");
  name.assign("%tab"); sbuf->append(name);

  BrigDirectiveSymbol ref11 = {
    0,                        // size
    BrigEDirectiveSymbol,     // kind
    {
      0,                      // c_code
      BrigGlobalSpace,        // storage class
      BrigNone,               // attribute
      0,                      // reserved
      BrigArray,              // symbolModifier
      3,                      // dim
      0,                      // s_name
      Brigu32,                // type
      1,                      // align
    },
    0,                        // d_init
    0                         // reserved
  };
  ref11.size = sizeof(ref11);
  ref11.d_init = sizeof(labList11) + sizeof(ref11);

  arraySize = sizeof(BrigDirectiveLabelInit) + sizeof(BrigdOffset32_t) * 2;
  BrigDirectiveLabelInit* bdli11 = (BrigDirectiveLabelInit*) (malloc(arraySize));

  bdli11->size = arraySize;                              // size
  bdli11->kind = BrigEDirectiveLabelInit;                // kind
  bdli11->c_code = 0;                                    // c_code
  bdli11->elementCount = 3;                              // elementCount
  bdli11->s_name = 0;
  bdli11->d_labels[0] = sizeof(ref11);
  bdli11->d_labels[1] = 0;
  bdli11->d_labels[2] = 0;

  labList11.size = sizeof(labList11);
  labList11.kind = BrigEDirectiveLabel;
  labList11.c_code = 0;
  labList11.s_name = 0;

  GlobalDecl_Test<BrigDirectiveSymbol, BrigDirectiveLabelInit> 
    TestCase11(in, sbuf, &ref11, bdli11, &labList11, 1);
  TestCase11.Run_Test(&GlobalDecl);
  free(bdli11);  bdli11 = NULL;
  sbuf->clear();
  /************************************************ Case 12 ***************************************/

  BrigDirectiveLabel labList12;
  in.assign("global_u32 %tab = {@odd};\n");
  name.assign("%tab"); sbuf->append(name);

  BrigDirectiveSymbol ref12 = {
    0,                        // size
    BrigEDirectiveSymbol,     // kind
    {
      0,                      // c_code
      BrigGlobalSpace,        // storage class
      BrigNone,               // attribute
      0,                      // reserved
      0,                      // symbolModifier
      0,                      // dim
      0,                      // s_name
      Brigu32,                // type
      1,                      // align
    },
    0,                        // d_init
    0                         // reserved
  };
  ref12.size = sizeof(ref12);
  ref12.d_init = sizeof(labList12) + sizeof(ref12);

  arraySize = sizeof(BrigDirectiveLabelInit);  
  BrigDirectiveLabelInit* bdli12 = (BrigDirectiveLabelInit*) (malloc(arraySize));

  bdli12->size = arraySize;                              // size
  bdli12->kind = BrigEDirectiveLabelInit;                // kind
  bdli12->c_code = 0;                                    // c_code
  bdli12->elementCount = 1;                              // elementCount
  bdli12->s_name = 0;
  bdli12->d_labels[0] = sizeof(ref12);

  labList12.size = sizeof(labList12);
  labList12.kind = BrigEDirectiveLabel;
  labList12.c_code = 0;
  labList12.s_name = 0;

  GlobalDecl_Test<BrigDirectiveSymbol, BrigDirectiveLabelInit> 
    TestCase12(in, sbuf, &ref12, bdli12, &labList12, 1);
  TestCase12.Run_Test(&GlobalDecl);
  free(bdli12);  bdli12 = NULL;
  sbuf->clear();

  /*******************************************************End of tests ***********************************************/
  delete sbuf;

}

TEST(CodegenTest,GlobalImageDecl_Codegen){

  std::string in, name;
  StringBuffer *sbuf = new StringBuffer();

  /************************Test Case 1****************************/
  in.assign("global_RWImg &demo[9]");
  in.append("={format = signed_int32 ,order = r,width = 2,height = 3,depth = 4 };\n");
  name.assign("&demo"); sbuf->append(name);

  BrigDirectiveImage out1 = {
    0,                      //size
    BrigEDirectiveImage,    //kind
    {
      0,                         // c_code
      BrigGlobalSpace,          // storag class
      BrigNone ,                // attribut
      0,                        // reserved
      BrigArray,                // symbolModifier
      9,                        // dim
      0,                        // s_name
      BrigRWImg,                  // type
      1,                        // align
    },
    2,                      //width
    3,                      //height
    4,                      //depth
    1,                      //array
    BrigImage_R,             //order
    BrigSIGNED_INT32        //format
  };
  out1.size = sizeof(out1);

  GlobalDecl_Test<BrigDirectiveImage> TestCase1(in, sbuf, &out1);
  TestCase1.Run_Test(&GlobalImageDecl);
  sbuf->clear();


  /************************Test Case 2****************************/
  in.assign("global_RWImg &demo[9][9]={order = a};");
  name.assign("&demo"); sbuf->append(name);

  BrigDirectiveImage out2 = {
    0,                      //size
    BrigEDirectiveImage,    //kind
    {
      0,                         // c_code
      BrigGlobalSpace,          // storag class
      BrigNone ,                // attribut
      0,                        // reserved
      BrigArray,                // symbolModifier
      81,                        // dim
      0,                        // s_name
      BrigRWImg,                  // type
      1,                        // align
    },
    0,                      //width
    0,                      //height
    0,                      //depth
    1,                      //array
    BrigImage_A,                  //order
    BrigImageFormatUnknown        //format
  };
  out2.size = sizeof(out2);

  GlobalDecl_Test<BrigDirectiveImage> TestCase2(in, sbuf, &out2);
  TestCase2.Run_Test(&GlobalImageDecl);
  sbuf->clear();

  /************************Test Case 3****************************/
  in.assign("global_RWImg &demo");
  in.append("={format = signed_int32 ,order = r,width = 2,height = 3,depth = 4 };\n");
  name.assign("&demo"); sbuf->append(name);

  BrigDirectiveImage out3 = {
    0,                      //size
    BrigEDirectiveImage,    //kind
    {
      0,                         // c_code
      BrigGlobalSpace,          // storag class
      BrigNone ,                // attribut
      0,                        // reserved
      0,                // symbolModifier
      0,                        // dim
      0,                        // s_name
      BrigRWImg,                  // type
      1,                        // align
    },
    2,                      //width
    3,                      //height
    4,                      //depth
    1,                      //array
    BrigImage_R,             //order
    BrigSIGNED_INT32        //format
  };
  out3.size = sizeof(out3);

  GlobalDecl_Test<BrigDirectiveImage> TestCase3(in, sbuf, &out3);
  TestCase3.Run_Test(&GlobalImageDecl);
  sbuf->clear();

  /********************************/
  delete sbuf;
}

TEST(CodegenTest,GlobalReadOnlyImageDecl_Codegen){

  std::string in, name;
  StringBuffer *sbuf = new StringBuffer();

  /************************Test Case 1****************************/
  in.assign("global_ROImg &demo[9]");
  in.append("={format = unsigned_int32 ,order = r,width = 4 ,height = 5,depth = 6 };\n");
  name.assign("&demo"); sbuf->append(name);

  BrigDirectiveImage out1 = {
    0,                      //size
    BrigEDirectiveImage,    //kind
    {
      0,                         // c_code
      BrigGlobalSpace,          // storag class
      BrigNone ,                // attribut
      0,                        // reserved
      BrigArray,                // symbolModifier
      9,                        // dim
      0,                        // s_name
      BrigROImg,                  // type
      1,                        // align
    },
    4,                      //width
    5,                      //height
    6,                      //depth
    1,                      //array
    BrigImage_R,             //order
    BrigUNSIGNED_INT32        //format
  };
  out1.size = sizeof(out1);

  GlobalDecl_Test<BrigDirectiveImage> TestCase1(in, sbuf, &out1);
  TestCase1.Run_Test(&GlobalReadOnlyImageDecl);
  sbuf->clear();


  /************************Test Case 2****************************/
  in.assign("global_ROImg &demo[9][9]={format = unorm_short_101010, order = rgbx, width = 4 ,height = 5,depth = 6};");
  name.assign("&demo"); sbuf->append(name);

  BrigDirectiveImage out2 = {
    0,                      //size
    BrigEDirectiveImage,    //kind
    {
      0,                         // c_code
      BrigGlobalSpace,          // storag class
      BrigNone ,                // attribut
      0,                        // reserved
      BrigArray,                // symbolModifier
      81,                        // dim
      0,                        // s_name
      BrigROImg,                  // type
      1,                        // align
    },
    4,                      //width
    5,                      //height
    6,                      //depth
    1,                      //array
    BrigImage_RGBX,                  //order
    BrigUNORM_SHORT_101010        //format
  };
  out2.size = sizeof(out2);

  GlobalDecl_Test<BrigDirectiveImage> TestCase2(in, sbuf, &out2);
  TestCase2.Run_Test(&GlobalReadOnlyImageDecl);
  sbuf->clear();

/***************************test case 3********************************************/
  in.assign("global_ROImg &demo");
  in.append("={format = unsigned_int32 ,order = r,width = 4 ,height = 5,depth = 6 };\n");
  name.assign("&demo"); sbuf->append(name);

  BrigDirectiveImage out3 = {
    0,                      //size
    BrigEDirectiveImage,    //kind
    {
      0,                         // c_code
      BrigGlobalSpace,          // storag class
      BrigNone ,                // attribut
      0,                        // reserved
      0,                // symbolModifier
      0,                        // dim
      0,                        // s_name
      BrigROImg,                  // type
      1,                        // align
    },
    4,                      //width
    5,                      //height
    6,                      //depth
    1,                      //array
    BrigImage_R,             //order
    BrigUNSIGNED_INT32        //format
  };
  out3.size = sizeof(out3);

  GlobalDecl_Test<BrigDirectiveImage> TestCase3(in, sbuf, &out3);
  TestCase3.Run_Test(&GlobalReadOnlyImageDecl);
  sbuf->clear();

  /********************************/
  delete sbuf;
}

TEST(CodegenTest,GlobalSamplerDecl_Codegen){

  std::string in, name;
  StringBuffer *sbuf = new StringBuffer();

  /************************Test Case 1****************************/
  in.assign("global_Samp &demo[9]={coord = normalized, filter = linear,");
  in.append("boundaryU = clamp, boundaryV = wrap, boundaryW = mirror } ;\n");

  name.assign("&demo"); sbuf->append(name);

   BrigDirectiveSampler out1 = {
    0,                           //size
    BrigEDirectiveSampler,       //kind
    {
      0,                        // c_code
      BrigGlobalSpace,          // storag class
      BrigNone ,                // attribut
      0,                        // reserved
      BrigArray,                // symbolModifier
      9,                        // dim
      0,                        // s_name
      BrigSamp,                 // type
      1,                        // align
    },
    1,                      //valid
    1,                      //normalized
    BrigSamplerFilterLinear,//filter
    BrigSamplerClamp,       //boundaryU
    BrigSamplerWrap,        //boundaryV
    BrigSamplerMirror,      //boundaryW
    0                       //reserved1
  };
  out1.size = sizeof(out1);

  GlobalDecl_Test<BrigDirectiveSampler> TestCase1(in, sbuf, &out1);
  TestCase1.Run_Test(&GlobalSamplerDecl);
  sbuf->clear();


  /************************Test Case 2****************************/
  in.assign("global_Samp &demo[9]={coord = unnormalized, filter = nearest,");
  in.append("boundaryU = mirroronce, boundaryV = border, boundaryW = clamp } ;\n");

  name.assign("&demo"); sbuf->append(name);

   BrigDirectiveSampler out2 = {
    0,                           //size
    BrigEDirectiveSampler,       //kind
    {
      0,                        // c_code
      BrigGlobalSpace,          // storag class
      BrigNone ,                // attribut
      0,                        // reserved
      BrigArray,                // symbolModifier
      9,                        // dim
      0,                        // s_name
      BrigSamp,                 // type
      1,                        // align
    },
    1,                      //valid
    0,                      //normalized
    BrigSamplerFilterNearest,//filter
    BrigSamplerMirrorOnce,       //boundaryU
    BrigSamplerBorder,           //boundaryV
    BrigSamplerClamp,            //boundaryW
    0                            //reserved1
  };
  out2.size = sizeof(out2);

  GlobalDecl_Test<BrigDirectiveSampler> TestCase2(in, sbuf, &out2);
  TestCase2.Run_Test(&GlobalSamplerDecl);
  sbuf->clear();

  /************************Test Case 3****************************/
  in.assign("global_Samp &demo={coord = unnormalized, filter = nearest,");
  in.append("boundaryU = mirroronce, boundaryV = border, boundaryW = clamp } ;\n");

  name.assign("&demo"); sbuf->append(name);

   BrigDirectiveSampler out3 = {
    0,                           //size
    BrigEDirectiveSampler,       //kind
    {
      0,                        // c_code
      BrigGlobalSpace,          // storag class
      BrigNone ,                // attribut
      0,                        // reserved
      0,                // symbolModifier
      0,                        // dim
      0,                        // s_name
      BrigSamp,                 // type
      1,                        // align
    },
    1,                      //valid
    0,                      //normalized
    BrigSamplerFilterNearest,//filter
    BrigSamplerMirrorOnce,       //boundaryU
    BrigSamplerBorder,           //boundaryV
    BrigSamplerClamp,            //boundaryW
    0                            //reserved1
  };
  out3.size = sizeof(out3);

  GlobalDecl_Test<BrigDirectiveSampler> TestCase3(in, sbuf, &out3);
  TestCase3.Run_Test(&GlobalSamplerDecl);
  sbuf->clear();

  /********************************/
  delete sbuf;
}

TEST(ErrorReportTest, GlobalDecl) {  
  std::string input = "group_u32 &x3\n";
  GlobalDecl_Test<> TestCase1(input);
  TestCase1.Run_Test(&GlobalDecl, MISSING_SEMICOLON);

  input.assign("private &y5[8];\n");
  GlobalDecl_Test<> TestCase2(input);
  TestCase2.Run_Test(&GlobalDecl, MISSING_DATA_TYPE);

  input.assign("global_b16 &x2[17] = {0xF7, 3 0xF1, -3, 31};\n");
  GlobalDecl_Test<> TestCase3(input);
  TestCase3.Run_Test(&GlobalDecl, MISSING_CLOSING_BRACKET);
}

}
}
