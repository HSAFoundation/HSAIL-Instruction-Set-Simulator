#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa{
namespace brig{

//Handles FunctionDecl, FunctionSignature, GlobalSymbolDecl
//T = BrigDirectivefunction/BrigDirectiveSymbol/BrigDirectiveSignature

template <typename T> class GlobalDecl_Test: public BrigCodeGenTest{
private:
  const std::string Fn_name;
  T* Output;
  BrigDirectiveSymbol** arglist;   
public:
  GlobalDecl_Test(std::string& in, std::string& name, T* out, BrigDirectiveSymbol** args = NULL) : 
    BrigCodeGenTest(in),
    Fn_name(name),
    Output(out),
    arglist(args) { }

  void validate(Context* context){  
    T getOut;
    int dir_offset = directive_start;
    context->get_directive(dir_offset, &getOut);
    validate_brig::validate(Output, &getOut);
    dir_offset+=sizeof(getOut);    

    if(arglist){ //Functiondecl
      BrigDirectiveFunction* ref = reinterpret_cast<BrigDirectiveFunction *> (Output);
      for(unsigned int i=0; i < (ref->inParamCount + ref->outParamCount); i++){
        BrigDirectiveSymbol get;
        context->get_directive(dir_offset, &get);
        validate_brig::validate(arglist[i], &get);
        dir_offset += sizeof(get);
      }
    }
  }
};


//Template specialisation for initializabledecl necessary

TEST(CodegenTest, GlobalSymbolDecl_Codegen){
  std::string in, name;
  int buffer_start = BUFFER_OFFSET;
    
  in.assign("align 1 static group_s8 &tmp[2];\n");
  name.assign("&tmp");
  BrigDirectiveSymbol ref = {
    sizeof(BrigDirectiveSymbol),                       // size
    BrigEDirectiveSymbol ,    // kind
    {
      buffer_start,                      // c_code
      BrigGroupSpace,         // storag class
      BrigStatic,             // attribute
      0,                      // reserved
      BrigArray,              // symbolModifier
      2,                      // dim
      buffer_start,                      // s_name
      Brigs8,                // type
      1,                      // align
    },
    0,                        // d_init
    0,                        // reserved
  };
  GlobalDecl_Test<BrigDirectiveSymbol> TestCase1(in, name, &ref);
  TestCase1.Run_Test(&GlobalDecl);
  
  /*********************************Add more test cases ********************************/
  
}

TEST(CodegenTest, FunctionDecl_Codegen){
  
  std::string in, name;
  int code_start = BUFFER_OFFSET;
  int string_start = BUFFER_OFFSET;
  int dir_start = BUFFER_OFFSET;
  
  in.assign("function &callee()(); \n");
  name.assign("&callee");
  BrigDirectiveFunction ref = {
    sizeof(BrigDirectiveFunction),                       // size
    BrigEDirectiveFunction,   // kind
    code_start,                       // c_code
    string_start,                        // s_name
    0,                        // inParamCount
    sizeof(BrigDirectiveFunction) + dir_start,                      // d_firstScopedDirective
    0,                        // operationCount
    sizeof(BrigDirectiveFunction) + dir_start,                      // d_nextDirective
    BrigNone,
    0,
    0,                        // outParamCount
    0,
  };
  GlobalDecl_Test<BrigDirectiveFunction> TestCase1(in, name, &ref);
  TestCase1.Run_Test(&GlobalDecl); 
  
  /***************************************************Test case 2********************************/
  
  in.assign("extern function &callee(arg_u32 %val1)(arg_u16 %val2);\n ");
  name.assign("&callee");
  
  int dir_size = sizeof(BrigDirectiveFunction) + sizeof(BrigDirectiveSymbol)*2 + dir_start;
  BrigdOffset32_t firstInParam = sizeof(BrigDirectiveFunction) + sizeof(BrigDirectiveSymbol)* 1 + dir_start;
  BrigDirectiveFunction ref2 = {
    sizeof(BrigDirectiveFunction),                       // size
    BrigEDirectiveFunction,   // kind
    code_start,                       // c_code
    string_start,                        // s_name
    1,                        // inParamCount
    dir_size,                      // d_firstScopedDirective
    0,                        // operationCount
    dir_size,                      // d_nextDirective
    BrigExtern,
    0,
    1,                        // outParamCount
    firstInParam
  };
  GlobalDecl_Test<BrigDirectiveFunction> TestCase2(in, name, &ref2);
  TestCase2.Run_Test(&GlobalDecl);
}

}
}