#include <iostream>
#include <string>
#include "codegen_validate.h"
#include "codegen_test.h"

namespace hsa {
namespace brig {

struct BrigBlockNode {
  BrigBlockNode* next;
  BrigBlockNumeric* num;
  BrigBlockString* str;
  BrigBlockNode() {
    next = NULL;
    str = NULL;
    num = NULL;
  }
  ~BrigBlockNode() {
    if (str != NULL) {
      delete str;
      str = NULL;
    }
    if (num != NULL) {
      delete num;
      num = NULL;
    }
  }
};

struct BrigBlockList {
  uint32_t size;
  BrigBlockNode* head;
  BrigBlockList() {
    size = 0;
    head = NULL;
  }
  void empty() {
    if (head != NULL) {
      BrigBlockNode* step = head->next;
      BrigBlockNode* tmp;
      while (step != NULL) {
        tmp = step; 
        step = step->next;
        delete tmp;
      }
      head->next = NULL;
    }
    size = 0; 
  }
  ~BrigBlockList() {
    if (head != NULL) {
      BrigBlockNode* step = head->next;
      BrigBlockNode* tmp;
      while (step != NULL) {
        tmp = step; 
        step = step->next;
        delete tmp;
      }
      delete head;
      head = NULL; 
    }
    size = 0;
  }
};


class Block_Test: public BrigCodeGenTest {

private:
  const BrigBlockStart* blockStart;
  const BrigBlockList* blockList;
  const BrigBlockEnd* blockEnd;

public:
  Block_Test(std::string& in, StringBuffer* sbuf, BrigBlockStart* blockStart, 
             BrigBlockList* blockList, BrigBlockEnd* blockEnd):
    BrigCodeGenTest(in, sbuf),
    blockStart(blockStart),
    blockList(blockList),
    blockEnd(blockEnd) { }

  void validate(struct BrigSections* TestOutput) {

    const char* refbuf = reinterpret_cast<const char *>(&RefStr->get()[0]);
    const char* getbuf = TestOutput->strings;

    dir_iterator getdirective = TestOutput->begin();
    const BrigBlockStart* getstart = cast<BrigBlockStart>(getdirective);
    validate_brig::validate(blockStart, refbuf, getstart, getbuf);
  
    ++getdirective;

    BrigBlockNode* ref = blockList->head->next;
    const BrigBlockNumeric* getnum;
    const BrigBlockString* getstr;

    for (uint32_t i = 0 ; i < blockList->size ; ++i) {
      if (getdirective->kind == BrigEDirectivePad) {
        ++getdirective;
      }
      if (ref->num != NULL) {
        getnum = cast<BrigBlockNumeric>(getdirective);
        validate_brig::validate(ref->num, getnum);
      } else if (ref->str != NULL) {
        getstr = cast<BrigBlockString>(getdirective);
        validate_brig::validate(ref->str, refbuf, getstr, getbuf);
      }
      ++getdirective;
      ref = ref->next;
    }
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
  BrigBlockList blockList;
  BrigBlockNode *push;
  size_t arraySize;

  blockList.head = new BrigBlockNode();
  // Note: first node is empty;


  symbols = new StringBuffer();

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

  push = blockList.head;

  // frist numeric block
  blockList.size++;
  push->next = new BrigBlockNode();
  push = push->next;

  arraySize = sizeof(BrigBlockNumeric) + sizeof(uint64_t);
  push->num = reinterpret_cast<BrigBlockNumeric*>(new int8_t[arraySize]);

  push->num->size = arraySize;
  push->num->kind =  BrigEDirectiveBlockNumeric;
  push->num->type = Brigb32;
  push->num->elementCount = 4;
  push->num->u32[0] = 1255;
  push->num->u32[1] = 0x323;
  push->num->u32[2] = 10;
  push->num->u32[3] = 23;

  // second numeric block
  blockList.size++;
  push->next = new BrigBlockNode();
  push = push->next;

  arraySize = sizeof(BrigBlockNumeric) + 3 * sizeof(uint64_t);
  push->num = reinterpret_cast<BrigBlockNumeric*>(new int8_t[arraySize]);

  push->num->size = arraySize;
  push->num->kind = BrigEDirectiveBlockNumeric;
  push->num->type = Brigb64;
  push->num->elementCount = 4;
  push->num->u64[0] = 0x12345678;
  push->num->u64[1] = 0x323;
  push->num->u64[2] = 10;
  push->num->u64[3] = 23;
  
  // third string block
  blockList.size++;
  push->next = new BrigBlockNode();
  push = push->next;

  push->str = new BrigBlockString();
  push->str->size = sizeof(str);
  push->str->kind = BrigEDirectiveBlockString;
  push->str->s_name = blockName.size() + 1;

  // forth numeric block
  blockList.size++;
  push->next = new BrigBlockNode();
  push = push->next;

  arraySize = sizeof(BrigBlockNumeric);
  push->num = new BrigBlockNumeric();

  push->num->size = arraySize;
  push->num->kind =  BrigEDirectiveBlockNumeric;
  push->num->type = Brigb8;
  push->num->elementCount = 5;
  push->num->u8[0] = 1;
  push->num->u8[1] = 255;
  push->num->u8[2] = 0;
  push->num->u8[3] = 127;
  push->num->u8[4] = 128;
  push->num->u8[5] = 0;
  push->num->u8[6] = 0;
  push->num->u8[7] = 0;

  // fifth string block
  blockList.size++;
  push->next = new BrigBlockNode();
  push = push->next;

  push->str = new BrigBlockString();
  push->str->size = sizeof(str);
  push->str->kind = BrigEDirectiveBlockString;
  push->str->s_name = blockName.size() + strBlock1.size() + 2;

  end.size = sizeof(end);
  end.kind = BrigEDirectiveBlockEnd;
  
  Block_Test TestCase1(in, symbols, &start, &blockList, &end);
  TestCase1.Run_Test(&Block);

  blockList.empty();
  push = NULL;
  symbols->clear();

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

  push = blockList.head;

  // first block string
  blockList.size++;
  push->next = new BrigBlockNode();
  push = push->next;

  push->str = new BrigBlockString();
  push->str->size = sizeof(str);
  push->str->kind = BrigEDirectiveBlockString;
  push->str->s_name = blockName.size() + 1;

  // second block string
  blockList.size++;
  push->next = new BrigBlockNode();
  push = push->next;

  push->str = new BrigBlockString();
  push->str->size = sizeof(str);
  push->str->kind = BrigEDirectiveBlockString;
  push->str->s_name = blockName.size() + strBlock1.size() + 2;

  // third block string
  blockList.size++;
  push->next = new BrigBlockNode();
  push = push->next;

  push->str = new BrigBlockString();
  push->str->size = sizeof(str);
  push->str->kind = BrigEDirectiveBlockString;
  push->str->s_name = blockName.size() + strBlock1.size() + strBlock2.size() + 3;

  // forth block string
  blockList.size++;
  push->next = new BrigBlockNode();
  push = push->next;

  push->str = new BrigBlockString();
  push->str->size = sizeof(str);
  push->str->kind = BrigEDirectiveBlockString;
  push->str->s_name = blockName.size() + strBlock1.size() + 
                      strBlock2.size() + strBlock4.size() + 4;

  end.size = sizeof(end);
  end.kind = BrigEDirectiveBlockEnd;
  
  Block_Test TestCase2(in, symbols, &start, &blockList, &end);
  TestCase2.Run_Test(&Block);

  blockList.empty();
  push = NULL;
  symbols->clear();

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

  push = blockList.head;

  blockList.size++;
  push->next = new BrigBlockNode();
  push = push->next;

  arraySize = sizeof(BrigBlockNumeric) + sizeof(uint64_t);
  push->num = reinterpret_cast<BrigBlockNumeric*>(new int8_t[arraySize]);

  push->num->size = arraySize;
  push->num->kind =  BrigEDirectiveBlockNumeric;
  push->num->type = Brigb32;
  push->num->elementCount = 4;
  push->num->u32[0] = 11;
  push->num->u32[1] = 23;
  push->num->u32[2] = 10;
  push->num->u32[3] = 22;

  end.size = sizeof(end);
  end.kind = BrigEDirectiveBlockEnd;
  
  Block_Test TestCase3(in, symbols, &start, &blockList, &end);
  TestCase3.Run_Test(&Block);

  blockList.empty();
  push = NULL;
  symbols->clear();

  delete symbols;

}
} // namespace hsa
} // namespace brig




