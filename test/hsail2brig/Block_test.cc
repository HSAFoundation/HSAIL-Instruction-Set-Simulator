#include "parser.h"
#include "parser_wrapper.h"
#include "../codegen_test.h"

namespace hsa {
namespace brig {

class Block_Test: public BrigCodeGenTest {

private:
  Buffer* RefDir;
public:
  Block_Test(std::string& in, StringBuffer* sbuf, Buffer* dir):
    BrigCodeGenTest(in, sbuf),
    RefDir(dir) {}

  void Run_Test(int (*Rule)(Context*)){  
    Buffer* code = new Buffer();
    Buffer* oper = new Buffer();

    struct BrigSections RefOutput(reinterpret_cast<const char *>(&RefStr->get()[0]), 
      reinterpret_cast<const char *>(&RefDir->get()[0]),
      reinterpret_cast<const char *>(&code->get()[0]), 
      reinterpret_cast<const char *>(&oper->get()[0]), NULL, 
      RefStr->size(), RefDir->size(), 0, 0, 0);
    
    Parse_Validate(Rule, &RefOutput);
    delete code;
    delete oper;
  }
};


TEST(CodegenTest, Block_CodeGen) {

  /********************************** Common variables used by all tests******************************/
  //To keep the stack footprint low

  std::string in;
  std::string blockName, strBlock1, strBlock2, strBlock3, strBlock4;
  StringBuffer* symbols;

  BrigBlockStart start;
  BrigBlockEnd end;
  BrigBlockString str;
  BrigBlockNumeric* num = NULL;
  size_t arraySize = 0;

  symbols = new StringBuffer();
  Buffer* dir = new Buffer();
  /************************************* Test Case 1 ************************************/
  in.assign("block \"debug\"\n");
  in.append("blocknumeric_b32 1255, 0x323, 10, 23;\n");
  in.append("blocknumeric_b64 0x12345678, 0x323, 10, 23;\n");
  in.append("blockstring \"this is a string1\";\n");
  in.append("blocknumeric_b8 1, 255, 256, 127, 128;\n");
  in.append("blockstring \"this is a string2\";\n");
  in.append("endblock;\n");

  blockName.assign("\"debug\"");  
  strBlock1.assign("\"this is a string1\""); 
  strBlock2.assign("\"this is a string2\"");

  symbols->append(blockName);
  symbols->append(strBlock1);
  symbols->append(strBlock2);

  start.size = sizeof(start);
  start.kind = BrigEDirectiveBlockStart;
  start.c_code = 0;
  start.s_name = 0;
  
  dir->append(&start);

  // frist numeric block

  arraySize = sizeof(BrigBlockNumeric) + sizeof(uint64_t);
  num = reinterpret_cast<BrigBlockNumeric*>(new int8_t[arraySize]);

  num->size = arraySize;
  num->kind =  BrigEDirectiveBlockNumeric;
  num->type = Brigb32;
  num->elementCount = 4;
  num->u32[0] = 1255;
  num->u32[1] = 0x323;
  num->u32[2] = 10;
  num->u32[3] = 23;

  dir->append(num);
  delete[] num;
  num = NULL;

  arraySize = sizeof(BrigBlockNumeric) + 3 * sizeof(uint64_t);
  num = reinterpret_cast<BrigBlockNumeric*>(new int8_t[arraySize]);

  num->size = arraySize;
  num->kind = BrigEDirectiveBlockNumeric;
  num->type = Brigb64;
  num->elementCount = 4;
  num->u64[0] = 0x12345678;
  num->u64[1] = 0x323;
  num->u64[2] = 10;
  num->u64[3] = 23;
  
  dir->append(num);
  delete[] num;
  num = NULL;

  str.size = sizeof(str);
  str.kind = BrigEDirectiveBlockString;
  str.s_name = blockName.size() + 1;

  dir->append(&str);

  arraySize = sizeof(BrigBlockNumeric);
  num = new BrigBlockNumeric();

  num->size = arraySize;
  num->kind =  BrigEDirectiveBlockNumeric;
  num->type = Brigb8;
  num->elementCount = 5;
  num->u8[0] = 1;
  num->u8[1] = 255;
  num->u8[2] = 0;
  num->u8[3] = 127;
  num->u8[4] = 128;
  num->u8[5] = 0;
  num->u8[6] = 0;
  num->u8[7] = 0;

  dir->append(num);
  delete num;
  num = NULL;

  str.size = sizeof(str);
  str.kind = BrigEDirectiveBlockString;
  str.s_name = blockName.size() + strBlock1.size() + 2;
   
  dir->append(&str);

  end.size = sizeof(end);
  end.kind = BrigEDirectiveBlockEnd;
 
  dir->append(&end);
  
  Block_Test TestCase1(in, symbols, dir);
  TestCase1.Run_Test(&Block);

  symbols->clear();
  dir->clear();
  /************************************* Test Case 2 ************************************/
  in.assign("block \"string\"\n");
  in.append("blockstring \"this is a string1\";\n");
  in.append("blockstring \"this is a string2\";\n");
  in.append("blockstring \"this is a string3\";\n");
  in.append("blockstring \"this is a string4\";\n");
  in.append("endblock;\n");

  blockName.assign("\"string\"");  
  strBlock1.assign("\"this is a string1\""); 
  strBlock2.assign("\"this is a string2\"");
  strBlock3.assign("\"this is a string3\""); 
  strBlock4.assign("\"this is a string4\"");

  symbols->append(blockName);
  symbols->append(strBlock1);
  symbols->append(strBlock2);
  symbols->append(strBlock3);
  symbols->append(strBlock4);

  start.size = sizeof(start);
  start.kind = BrigEDirectiveBlockStart;
  start.c_code = 0;
  start.s_name = 0;

  dir->append(&start);

  str.size = sizeof(str);
  str.kind = BrigEDirectiveBlockString;
  str.s_name = blockName.size() + 1;

  dir->append(&str);

  str.size = sizeof(str);
  str.kind = BrigEDirectiveBlockString;
  str.s_name = blockName.size() + strBlock1.size() + 2;

  dir->append(&str);

  str.size = sizeof(str);
  str.kind = BrigEDirectiveBlockString;
  str.s_name = blockName.size() + strBlock1.size() + strBlock2.size() + 3;

  dir->append(&str);

  str.size = sizeof(str);
  str.kind = BrigEDirectiveBlockString;
  str.s_name = blockName.size() + strBlock1.size() + 
                      strBlock2.size() + strBlock4.size() + 4;
  dir->append(&str);

  end.size = sizeof(end);
  end.kind = BrigEDirectiveBlockEnd;
  
  dir->append(&end);
 
  Block_Test TestCase2(in, symbols, dir);
  TestCase2.Run_Test(&Block);

  symbols->clear();
  dir->clear();

  /************************************* Test Case 3 ************************************/
  in.assign("block \"numeric\"\n");
  in.append("blocknumeric_b32 11, 23, 10, 22;\n");
  in.append("endblock;\n");

  blockName.assign("\"numeric\"");  
  symbols->append(blockName);

  start.size = sizeof(start);
  start.kind = BrigEDirectiveBlockStart;
  start.c_code = 0;
  start.s_name = 0;

  dir->append(&start);

  arraySize = sizeof(BrigBlockNumeric) + sizeof(uint64_t);
  num = reinterpret_cast<BrigBlockNumeric*>(new int8_t[arraySize]);

  num->size = arraySize;
  num->kind =  BrigEDirectiveBlockNumeric;
  num->type = Brigb32;
  num->elementCount = 4;
  num->u32[0] = 11;
  num->u32[1] = 23;
  num->u32[2] = 10;
  num->u32[3] = 22;

  dir->append(num);
  delete[] num;
  num = NULL;

  end.size = sizeof(end);
  end.kind = BrigEDirectiveBlockEnd;
  
  dir->append(&end);

  Block_Test TestCase3(in, symbols, dir);
  TestCase3.Run_Test(&Block);

  symbols->clear();
  dir->clear();
  delete symbols;
  delete dir;
}
} // namespace hsa
} // namespace brig




