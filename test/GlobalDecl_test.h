#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

//T = BrigDirectivefunction/BrigDirectiveSymbol/BrigDirectiveSignature

template <typename T> class GlobalDecl_Test: public BrigCodeGenTest{
private:
  T* RefDir; 
  
  BrigDirectiveSymbol* RefArgsList;
  BrigDirectiveInit* RefInit;
    
public:
  GlobalDecl_Test(std::string& in, StringBuffer* sbuf,  T* out, BrigDirectiveInit* d_init=NULL) : 
    BrigCodeGenTest(in, sbuf),
    RefDir(out),
    RefArgsList(NULL),
    RefInit(d_init)   { }
  
  GlobalDecl_Test(std::string& in, StringBuffer* sbuf, T* out, BrigDirectiveSymbol* arglist) : 
    BrigCodeGenTest(in, sbuf),
    RefDir(out), 
    RefArgsList(arglist),
    RefInit(NULL) { }  

  void validate(struct BrigSections* TestOutput){
    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;
        
    dir_iterator getdir = TestOutput->begin();
    const T* getdecl = (cast<T>(getdir));
    validate_brig::validate(RefDir,refbuf, getdecl, getbuf); 

    if(RefArgsList){
      const BrigDirectiveFunction* ref = reinterpret_cast<const BrigDirectiveFunction*> (getdecl);
      for(unsigned int i=0; i < (ref->inParamCount + ref->outParamCount); i++){
        const BrigDirectiveSymbol* getarg = (cast<BrigDirectiveSymbol>(++getdir));
        BrigDirectiveSymbol* refarg = &(RefArgsList[i]);
        validate_brig::validate(refarg, refbuf, getarg, getbuf);
      }  
    } else if(RefInit){
      const BrigDirectiveInit* getarg = (cast<BrigDirectiveInit>(++getdir));
      validate_brig::validate(RefInit, getarg);
    }
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
  GlobalDecl_Test<BrigDirectiveSymbol> TestCase4(in, sbuf, &ref);
  TestCase4.Run_Test(&GlobalDecl);
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
  GlobalDecl_Test<BrigDirectiveSymbol> TestCase5(in, sbuf, &ref);
  TestCase5.Run_Test(&GlobalDecl);
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
  GlobalDecl_Test<BrigDirectiveSymbol> TestCase6(in, sbuf, &ref);
  TestCase6.Run_Test(&GlobalDecl);
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
  in.append("arg_b32 %value[64], arg_b1 %jud1, arg_b1 %jud2) :fbar(128);\n");
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
    128,                              // fbarCount
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

  char* tmp1 = new char[sizeof(BrigDirectiveSignature) + sizeof(BrigDirectiveSignature::BrigProtoType) * 2];

  BrigDirectiveSignature* pRef1 = reinterpret_cast<BrigDirectiveSignature*>(tmp1);

  pRef1->size = sizeof(BrigDirectiveSignature) + sizeof(BrigDirectiveSignature::BrigProtoType) * 2;
  pRef1->kind = BrigEDirectiveSignature;  // kind
  pRef1->c_code = 0;                      // c_code
  pRef1->s_name = 0;        // s_name
  pRef1->fbarCount = 0;                   // fbarCount
  pRef1->reserved = 0;                    // reserved
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
  delete []tmp1;  tmp1 = NULL;
  sbuf->clear();

  /********************************* Case 2 ****************************************/
  in.assign("signature &fun_t (arg_u32) (arg_u32, arg_u32) :fbar(16);\n");
  name.assign("&fun_t");  sbuf->append(name);

  char* tmp2 = new char[sizeof(BrigDirectiveSignature) + sizeof(BrigDirectiveSignature::BrigProtoType) * 2];

  BrigDirectiveSignature* pRef2 = reinterpret_cast<BrigDirectiveSignature*>(tmp2);

  pRef2->size = sizeof(BrigDirectiveSignature) + sizeof(BrigDirectiveSignature::BrigProtoType) * 2;
  pRef2->kind = BrigEDirectiveSignature;  // kind
  pRef2->c_code = 0;                      // c_code
  pRef2->s_name = 0;        // s_name
  pRef2->fbarCount = 16;                   // fbarCount
  pRef2->reserved = 0;                    // reserved
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
  pRef2->types[2].dim = 0;               // dim

  GlobalDecl_Test<BrigDirectiveSignature> TestCase2(in, sbuf, pRef2);
  TestCase2.Run_Test(&GlobalDecl); 
  delete []tmp2;  tmp2 = NULL;
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
  0,                       // size
  BrigEDirectiveSymbol ,    // kind
  {
    0,                         // c_code
    BrigGlobalSpace,         // storag class
    BrigNone ,                // attribut
    0,                        // reserved
    2,                        // symbolModifier
    9,                        // dim
    0,                        // s_name
    Brigb8,                  // type
    1,                        // align
  },
  0,                        // d_init
  0,                         // reserved
  };
  ref.size = sizeof(ref);
  BrigDirectiveInit* bdi = (BrigDirectiveInit*) (malloc(sizeof(BrigDirectiveInit) + sizeof(uint64_t)));
  
  bdi->size = sizeof(BrigDirectiveInit) + sizeof(uint64_t);           //size
  bdi->kind = BrigEDirectiveInit;  //kind
  bdi->c_code = 0;                 //c_code
  bdi->elementCount = 9;           //elementCount
  bdi->type = Brigb8;              //type
  bdi->reserved = 0;               //reserved
  bdi->initializationData.u8[0] = 1;    //initializationData
  bdi->initializationData.u8[1] = 2;
  bdi->initializationData.u8[2] = 3;
  bdi->initializationData.u8[3] = 4;
  bdi->initializationData.u8[4] = 5;
  bdi->initializationData.u8[5] = 6;
  bdi->initializationData.u8[6] = 7;
  bdi->initializationData.u8[7] = 8;

  bdi->initializationData.u8[8] = 9;    //initializationData
  bdi->initializationData.u8[9] = 0;
  bdi->initializationData.u8[10] = 0;
  bdi->initializationData.u8[11] = 0;
  bdi->initializationData.u8[12] = 0;
  bdi->initializationData.u8[13] = 0;
  bdi->initializationData.u8[14] = 0;
  bdi->initializationData.u8[15] = 0;
  
  GlobalDecl_Test<BrigDirectiveSymbol> TestCase2(in, sbuf, &ref, bdi);
  TestCase2.Run_Test(&GlobalDecl); 
  free(bdi);
  sbuf->clear();
  /******************************************************* Add more tests *******************************************/
  
  /*******************************************************End of tests ***********************************************/
  delete sbuf;

}
}
}
