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