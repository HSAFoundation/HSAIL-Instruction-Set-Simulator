// Copyright 2012 MulticoreWare Inc.

#include <iostream>
#include "gtest/gtest.h"
#include "tokens.h"
#include "lexer.h"
#include "parser.h"
#include "brig.h"
#include "error_reporter.h"
#include "context.h"
#include "parser_wrapper.h"
#include "globaldecl_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(CodegenTestGlobalSamplerDecl,GlobalSamplerDecl)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(inputarray_globalsamplerdecl[n]);
 
  Lexer* lexer = new Lexer(input);

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, GlobalSamplerDecl(context));

  BrigDirectiveSampler get;
  context->get_directive(0, &get);

  BrigDirectiveSampler ref = outputarray_globalsamplerdecl[n];

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.s.c_code, get.s.c_code);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.attribute, get.s.attribute);
  EXPECT_EQ(ref.s.reserved, get.s.reserved);
  EXPECT_EQ(ref.s.symbolModifier, get.s.symbolModifier);
  EXPECT_EQ(ref.s.dim, get.s.dim);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.s.type, get.s.type);
  EXPECT_EQ(ref.s.align, get.s.align);
  EXPECT_EQ(ref.valid, get.valid);
  EXPECT_EQ(ref.normalized, get.normalized);
  EXPECT_EQ(ref.filter, get.filter);
  EXPECT_EQ(ref.boundaryU, get.boundaryU);
  EXPECT_EQ(ref.boundaryV, get.boundaryV);
  EXPECT_EQ(ref.boundaryW, get.boundaryW);
  EXPECT_EQ(ref.reserved1, get.reserved1);
 
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(TestGlobalSamplerDecl,
                        CodegenTestGlobalSamplerDecl,
                        testing::Range(0,9));

TEST_P(CodegenTestGlobalImageDecl,GlobalImageDecl)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(inputarray_globalimagedecl[n]);
 
  Lexer* lexer = new Lexer(input);

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, GlobalImageDecl(context));

  BrigDirectiveImage get;
  context->get_directive(0, &get);

  BrigDirectiveImage ref = outputarray_globalimagedecl[n];

  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.width, get.width);
  EXPECT_EQ(ref.height, get.height);
  EXPECT_EQ(ref.depth, get.depth);
  EXPECT_EQ(ref.array, get.array);
  EXPECT_EQ(ref.order, get.order);
  EXPECT_EQ(ref.format, get.format);
  EXPECT_EQ(ref.s.c_code, get.s.c_code);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.attribute, get.s.attribute);
  EXPECT_EQ(ref.s.reserved, get.s.reserved);
  EXPECT_EQ(ref.s.symbolModifier, get.s.symbolModifier);
  EXPECT_EQ(ref.s.dim, get.s.dim);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.s.type, get.s.type);
  EXPECT_EQ(ref.s.align, get.s.align);
 
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(TestGlobalImageDecl,
                        CodegenTestGlobalImageDecl,
                        testing::Range(0,18));

TEST_P(CodegenTestGlobalReadOnlyImageDecl,GlobalReadOnlyImageDecl)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(inputarray_globalreadonlyimagedecl[n]);
 
  Lexer* lexer = new Lexer(input);

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, GlobalReadOnlyImageDecl(context));

  BrigDirectiveImage get;
  context->get_directive(0, &get);

  BrigDirectiveImage ref = outputarray_globalreadonlyimagedecl[n];

  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.width, get.width);
  EXPECT_EQ(ref.height, get.height);
  EXPECT_EQ(ref.depth, get.depth);
  EXPECT_EQ(ref.array, get.array);
  EXPECT_EQ(ref.order, get.order);
  EXPECT_EQ(ref.format, get.format);
  EXPECT_EQ(ref.s.c_code, get.s.c_code);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.attribute, get.s.attribute);
  EXPECT_EQ(ref.s.reserved, get.s.reserved);
  EXPECT_EQ(ref.s.symbolModifier, get.s.symbolModifier);
  EXPECT_EQ(ref.s.dim, get.s.dim);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.s.type, get.s.type);
  EXPECT_EQ(ref.s.align, get.s.align);
 
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(TestGlobalReadOnlyImageDecl,
                        CodegenTestGlobalReadOnlyImageDecl,
                        testing::Range(0,18));

TEST_P(CodegenTestGlobalPrivateDecl,GlobalPrivateDecl)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(inputarray_globalprivatedecl[n]);
 
  Lexer* lexer = new Lexer(input);

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, GlobalPrivateDecl(context));

  BrigDirectiveSymbol get;
  context->get_directive(0, &get);

  BrigDirectiveSymbol ref = outputarray_globalprivatedecl[n];

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.s.c_code, get.s.c_code);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.attribute, get.s.attribute);
  EXPECT_EQ(ref.s.reserved, get.s.reserved);
  EXPECT_EQ(ref.s.symbolModifier, get.s.symbolModifier);
  EXPECT_EQ(ref.s.dim, get.s.dim);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.s.type, get.s.type);
  EXPECT_EQ(ref.s.align, get.s.align);
  EXPECT_EQ(ref.s.type, get.s.type);
  EXPECT_EQ(ref.d_init, get.d_init);
  EXPECT_EQ(ref.reserved, get.reserved);
 
  delete lexer;
};

INSTANTIATE_TEST_CASE_P(TestGlobalPrivateDecl,
                        CodegenTestGlobalPrivateDecl,
                        testing::Range(0,3));

TEST_P(CodegenTestGlobalGroupDecl,GlobalGroupDecl)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(inputarray_globalgroupdecl[n]);
 
  Lexer* lexer = new Lexer(input);

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, GlobalGroupDecl(context));

  BrigDirectiveSymbol get;
  context->get_directive(0, &get);

  BrigDirectiveSymbol ref = outputarray_globalgroupdecl[n];

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.s.c_code, get.s.c_code);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.attribute, get.s.attribute);
  EXPECT_EQ(ref.s.reserved, get.s.reserved);
  EXPECT_EQ(ref.s.symbolModifier, get.s.symbolModifier);
  EXPECT_EQ(ref.s.dim, get.s.dim);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.s.type, get.s.type);
  EXPECT_EQ(ref.s.align, get.s.align);
  EXPECT_EQ(ref.s.type, get.s.type);
  EXPECT_EQ(ref.d_init, get.d_init);
  EXPECT_EQ(ref.reserved, get.reserved);
 
  delete lexer;
};

INSTANTIATE_TEST_CASE_P(TestGlobalGroupDecl,
                        CodegenTestGlobalGroupDecl,
                        testing::Range(0,3));

TEST(CodegenTest, initializableDeclCodeGen_readonly_s32) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("readonly_s32 &x[4]= {12, 13,14, -13};");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

   BrigDirectiveSymbol ref1 = {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
     {
    0,                      // c_code
    BrigReadonlySpace,      // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    4,                      // dim
    0,                      // s_name
    Brigs32,                // type
    1,                      // align
      },
  40,                       // d_init
  0,                        // reserved
   };

  size_t arraySize = sizeof(BrigDirectiveInit) + 2 * sizeof(int32_t); 
  int8_t *array = new int8_t[arraySize];
     
  BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit *>(array);

  bdi->size = arraySize;           //size
  bdi->kind = BrigEDirectiveInit;  //kind
  bdi->c_code = 0;                 //c_code
  bdi->elementCount = 4;           //elementCount
  bdi->type = Brigb32;             //type
  bdi->reserved = 0;               //reserved

  int32_t value = 12;              //initializationData
  memmove(&bdi->initializationData.u32[0], &value, sizeof(int32_t));
  value = 13;
  memmove(&bdi->initializationData.u32[1], &value, sizeof(int32_t));
  value = 14;
  memmove(&bdi->initializationData.u32[2], &value, sizeof(int32_t));
  value = -13;
  memmove(&bdi->initializationData.u32[3], &value, sizeof(int32_t));

  EXPECT_EQ(0,InitializableDecl(context));
  BrigDirectiveSymbol get1 ;

  context->get_directive(0, &get1);
  EXPECT_EQ(ref1.size, get1.size);
  EXPECT_EQ(ref1.kind, get1.kind);
  EXPECT_EQ(ref1.s.c_code, get1.s.c_code);
  EXPECT_EQ(ref1.s.storageClass, get1.s.storageClass);
  EXPECT_EQ(ref1.s.attribute, get1.s.attribute);
  EXPECT_EQ(ref1.s.reserved, get1.s.reserved);
  EXPECT_EQ(ref1.s.symbolModifier, get1.s.symbolModifier);
  EXPECT_EQ(ref1.s.dim, get1.s.dim);
  EXPECT_EQ(ref1.s.s_name, get1.s.s_name);
  EXPECT_EQ(ref1.s.type, get1.s.type);
  EXPECT_EQ(ref1.s.align, get1.s.align);
  EXPECT_EQ(ref1.s.type, get1.s.type);
  EXPECT_EQ(ref1.d_init, get1.d_init);
  EXPECT_EQ(ref1.reserved, get1.reserved);

  Buffer *dbuf = context->get_directive();
  BrigDirectiveInit *get2 = 
    reinterpret_cast<BrigDirectiveInit*>(&dbuf->get()[40]);

  EXPECT_EQ(bdi->size, get2->size);
  EXPECT_EQ(bdi->kind, get2->kind);
  EXPECT_EQ(bdi->c_code, get2->c_code);
  EXPECT_EQ(bdi->elementCount, get2->elementCount);
  EXPECT_EQ(bdi->type, get2->type);
  EXPECT_EQ(bdi->reserved, get2->reserved);
  EXPECT_EQ(bdi->initializationData.u32[0],get2->initializationData.u32[0]);
  EXPECT_EQ(bdi->initializationData.u32[1],get2->initializationData.u32[1]);
  EXPECT_EQ(bdi->initializationData.u32[2],get2->initializationData.u32[2]);
  EXPECT_EQ(bdi->initializationData.u32[3],get2->initializationData.u32[3]);

  delete lexer;
  delete bdi;
};

TEST(CodegenTest, initializableDeclCodeGen_global_u32) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("global_u32 &x[3] = 12, 13,14 ; \n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

   BrigDirectiveSymbol ref1 = {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
     {
    0,                      // c_code
    BrigGlobalSpace,        // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    4,                      // dim
    0,                      // s_name
    Brigu32,                // type
    1,                      // align
      },
  40,                        // d_init
  0,                         // reserved
   };

  size_t arraySize = sizeof(BrigDirectiveInit) + 2*sizeof(uint32_t); 
  uint8_t *array = new uint8_t[arraySize];

  BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit *>(array);

  bdi->size = arraySize;           //size
  bdi->kind = BrigEDirectiveInit;  //kind
  bdi->c_code = 0;                 //c_code
  bdi->elementCount = 3;           //elementCount
  bdi->type = Brigb32;             //type
  bdi->reserved = 0;               //reserved

  uint32_t value = 12;             //initializationData
  memmove(&bdi->initializationData.u32[0], &value, sizeof(uint32_t));
  value = 13;
  memmove(&bdi->initializationData.u32[1], &value, sizeof(uint32_t));
  value = 14;
  memmove(&bdi->initializationData.u32[2], &value, sizeof(uint32_t));
  value = 0;
  memmove(&bdi->initializationData.u32[3], &value, sizeof(uint32_t));

  EXPECT_EQ(0,InitializableDecl(context));
  BrigDirectiveSymbol get1 ;

  context->get_directive(0, &get1);
  EXPECT_EQ(ref1.size, get1.size);
  EXPECT_EQ(ref1.kind, get1.kind);
  EXPECT_EQ(ref1.s.c_code, get1.s.c_code);
  EXPECT_EQ(ref1.s.storageClass, get1.s.storageClass);
  EXPECT_EQ(ref1.s.attribute, get1.s.attribute);
  EXPECT_EQ(ref1.s.reserved, get1.s.reserved);
  EXPECT_EQ(ref1.s.symbolModifier, get1.s.symbolModifier);
  EXPECT_EQ(ref1.s.dim, get1.s.dim);
  EXPECT_EQ(ref1.s.s_name, get1.s.s_name);
  EXPECT_EQ(ref1.s.type, get1.s.type);
  EXPECT_EQ(ref1.s.align, get1.s.align);
  EXPECT_EQ(ref1.s.type, get1.s.type);
  EXPECT_EQ(ref1.d_init, get1.d_init);
  EXPECT_EQ(ref1.reserved, get1.reserved);

  Buffer *dbuf = context->get_directive();
  BrigDirectiveInit *get2 = 
    reinterpret_cast<BrigDirectiveInit*>(&dbuf->get()[40]);
  EXPECT_EQ(bdi->size, get2->size);
  EXPECT_EQ(bdi->kind, get2->kind);
  EXPECT_EQ(bdi->c_code, get2->c_code);
  EXPECT_EQ(bdi->elementCount, get2->elementCount);
  EXPECT_EQ(bdi->type, get2->type);
  EXPECT_EQ(bdi->reserved, get2->reserved);
  EXPECT_EQ(bdi->initializationData.u32[0],get2->initializationData.u32[0]);
  EXPECT_EQ(bdi->initializationData.u32[1],get2->initializationData.u32[1]);
  EXPECT_EQ(bdi->initializationData.u32[2],get2->initializationData.u32[2]);
  EXPECT_EQ(bdi->initializationData.u32[3],get2->initializationData.u32[3]);

  delete lexer;
  delete bdi;
};

TEST(CodegenTest, initializableDeclCodeGen_readonly_f32) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("readonly_f32 %f[3] = { 1.2f, 1.3f,1.4f };\n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

   BrigDirectiveSymbol ref1 = {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
     {
    0,                       // c_code
    BrigReadonlySpace,       // storag class 
    BrigNone ,               // attribut
    0,                       // reserved
    BrigArray,               // symbolModifier
    4,                       // dim
    0,                       // s_name
    Brigf32,                 // type
    1,                       // align
      },
  40,                        // d_init
  0,                         // reserved
   };

  size_t arraySize = sizeof(BrigDirectiveInit) + 2*sizeof(uint32_t); 
  uint8_t *array = new uint8_t[arraySize];

  BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit *>(array);

  bdi->size = arraySize;           //size
  bdi->kind = BrigEDirectiveInit;  //kind
  bdi->c_code = 0;                 //c_code
  bdi->elementCount = 3;           //elementCount
  bdi->type = Brigb32;             //type
  bdi->reserved = 0;               //reserved

  float value = 1.2;              //initializationData
  memmove(&bdi->initializationData.u32[0], &value, sizeof(uint32_t));
  value = 1.3;
  memmove(&bdi->initializationData.u32[1], &value, sizeof(uint32_t));
  value = 1.4;
  memmove(&bdi->initializationData.u32[2], &value, sizeof(uint32_t));
  value = 0;
  memmove(&bdi->initializationData.u32[3], &value, sizeof(uint32_t));

  EXPECT_EQ(0,InitializableDecl(context));
  BrigDirectiveSymbol get1 ;

  context->get_directive(0, &get1);
  EXPECT_EQ(ref1.size, get1.size);
  EXPECT_EQ(ref1.kind, get1.kind);
  EXPECT_EQ(ref1.s.c_code, get1.s.c_code);
  EXPECT_EQ(ref1.s.storageClass, get1.s.storageClass);
  EXPECT_EQ(ref1.s.attribute, get1.s.attribute);
  EXPECT_EQ(ref1.s.reserved, get1.s.reserved);
  EXPECT_EQ(ref1.s.symbolModifier, get1.s.symbolModifier);
  EXPECT_EQ(ref1.s.dim, get1.s.dim);
  EXPECT_EQ(ref1.s.s_name, get1.s.s_name);
  EXPECT_EQ(ref1.s.type, get1.s.type);
  EXPECT_EQ(ref1.s.align, get1.s.align);
  EXPECT_EQ(ref1.s.type, get1.s.type);
  EXPECT_EQ(ref1.d_init, get1.d_init);
  EXPECT_EQ(ref1.reserved, get1.reserved);

  Buffer *dbuf = context->get_directive();
  BrigDirectiveInit *get2 = 
    reinterpret_cast<BrigDirectiveInit*>(&dbuf->get()[40]);
  EXPECT_EQ(bdi->size, get2->size);
  EXPECT_EQ(bdi->kind, get2->kind);
  EXPECT_EQ(bdi->c_code, get2->c_code);
  EXPECT_EQ(bdi->elementCount, get2->elementCount);
  EXPECT_EQ(bdi->type, get2->type);
  EXPECT_EQ(bdi->reserved, get2->reserved);
  EXPECT_EQ(bdi->initializationData.u32[0],get2->initializationData.u32[0]);
  EXPECT_EQ(bdi->initializationData.u32[1],get2->initializationData.u32[1]);
  EXPECT_EQ(bdi->initializationData.u32[2],get2->initializationData.u32[2]);
  EXPECT_EQ(bdi->initializationData.u32[3],get2->initializationData.u32[3]);

  delete lexer;
  delete bdi;
};

TEST(CodegenTest, initializableDeclCodeGen_global_f32) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("global_f32 &c[3] = 1.2f, 1.3f,1.4f ;\n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

   BrigDirectiveSymbol ref1 = {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
     {
    0,                      // c_code
    BrigGlobalSpace,        // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    4,                      // dim
    0,                      // s_name
    Brigf32,                // type
    1,                      // align
      },
  40,                       // d_init
  0,                        // reserved
   };

  size_t arraySize = sizeof(BrigDirectiveInit) + 2*sizeof(uint32_t); 
  uint8_t *array = new uint8_t[arraySize];

  BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit *>(array);

  bdi->size = arraySize;           //size
  bdi->kind = BrigEDirectiveInit;  //kind
  bdi->c_code = 0;                 //c_code
  bdi->elementCount = 3;           //elementCount
  bdi->type = Brigb32;             //type
  bdi->reserved = 0;               //reserved

  float value = 1.2;              //initializationData
  memmove(&bdi->initializationData.u32[0], &value, sizeof(uint32_t));
  value = 1.3;
  memmove(&bdi->initializationData.u32[1], &value, sizeof(uint32_t));
  value = 1.4;
  memmove(&bdi->initializationData.u32[2], &value, sizeof(uint32_t));
  value = 0;
  memmove(&bdi->initializationData.u32[3], &value, sizeof(uint32_t));


  EXPECT_EQ(0,InitializableDecl(context));
  BrigDirectiveSymbol get1 ;

  context->get_directive(0, &get1);
  EXPECT_EQ(ref1.size, get1.size);
  EXPECT_EQ(ref1.kind, get1.kind);
  EXPECT_EQ(ref1.s.c_code, get1.s.c_code);
  EXPECT_EQ(ref1.s.storageClass, get1.s.storageClass);
  EXPECT_EQ(ref1.s.attribute, get1.s.attribute);
  EXPECT_EQ(ref1.s.reserved, get1.s.reserved);
  EXPECT_EQ(ref1.s.symbolModifier, get1.s.symbolModifier);
  EXPECT_EQ(ref1.s.dim, get1.s.dim);
  EXPECT_EQ(ref1.s.s_name, get1.s.s_name);
  EXPECT_EQ(ref1.s.type, get1.s.type);
  EXPECT_EQ(ref1.s.align, get1.s.align);
  EXPECT_EQ(ref1.s.type, get1.s.type);
  EXPECT_EQ(ref1.d_init, get1.d_init);
  EXPECT_EQ(ref1.reserved, get1.reserved);

  Buffer *dbuf = context->get_directive();
  BrigDirectiveInit *get2 = 
    reinterpret_cast<BrigDirectiveInit*>(&dbuf->get()[40]);
  EXPECT_EQ(bdi->size, get2->size);
  EXPECT_EQ(bdi->kind, get2->kind);
  EXPECT_EQ(bdi->c_code, get2->c_code);
  EXPECT_EQ(bdi->elementCount, get2->elementCount);
  EXPECT_EQ(bdi->type, get2->type);
  EXPECT_EQ(bdi->reserved, get2->reserved);
  EXPECT_EQ(bdi->initializationData.u32[0],get2->initializationData.u32[0]);
  EXPECT_EQ(bdi->initializationData.u32[1],get2->initializationData.u32[1]);
  EXPECT_EQ(bdi->initializationData.u32[2],get2->initializationData.u32[2]);
  EXPECT_EQ(bdi->initializationData.u32[3],get2->initializationData.u32[3]);

  delete lexer;
  delete bdi;
};

TEST(CodegenTest, initializableDeclCodeGen_readonly_f64) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("readonly_f64 %d[3] ={ 1.2L, 1.3L,1.4L };\n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

   BrigDirectiveSymbol ref1 = {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
     {
    0,                      // c_code
    BrigReadonlySpace,      // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    3,                      // dim
    0,                      // s_name
    Brigf64,                // type
    1,                      // align
      },
  40,                       // d_init
  0,                        // reserved
   };

  size_t arraySize = sizeof(BrigDirectiveInit) + 2*sizeof(uint64_t); 
  uint8_t *array = new uint8_t[arraySize];

  BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit *>(array);

  bdi->size = arraySize;           //size
  bdi->kind = BrigEDirectiveInit;  //kind
  bdi->c_code = 0;                 //c_code
  bdi->elementCount = 3;           //elementCount
  bdi->type = Brigb64;             //type
  bdi->reserved = 0;               //reserved

  double value = 1.2;             //initializationData
  memmove(&bdi->initializationData.u64[0], &value, sizeof(uint64_t));
  value = 1.3;
  memmove(&bdi->initializationData.u64[1], &value, sizeof(uint64_t));
  value = 1.4;
  memmove(&bdi->initializationData.u64[2], &value, sizeof(uint64_t));

  EXPECT_EQ(0,InitializableDecl(context));
  BrigDirectiveSymbol get1 ;

  context->get_directive(0, &get1);
  EXPECT_EQ(ref1.size, get1.size);
  EXPECT_EQ(ref1.kind, get1.kind);
  EXPECT_EQ(ref1.s.c_code, get1.s.c_code);
  EXPECT_EQ(ref1.s.storageClass, get1.s.storageClass);
  EXPECT_EQ(ref1.s.attribute, get1.s.attribute);
  EXPECT_EQ(ref1.s.reserved, get1.s.reserved);
  EXPECT_EQ(ref1.s.symbolModifier, get1.s.symbolModifier);
  EXPECT_EQ(ref1.s.dim, get1.s.dim);
  EXPECT_EQ(ref1.s.s_name, get1.s.s_name);
  EXPECT_EQ(ref1.s.type, get1.s.type);
  EXPECT_EQ(ref1.s.align, get1.s.align);
  EXPECT_EQ(ref1.s.type, get1.s.type);
  EXPECT_EQ(ref1.d_init, get1.d_init);
  EXPECT_EQ(ref1.reserved, get1.reserved);

  Buffer *dbuf = context->get_directive();
  BrigDirectiveInit *get2 = 
    reinterpret_cast<BrigDirectiveInit*>(&dbuf->get()[40]);
  EXPECT_EQ(bdi->size, get2->size);
  EXPECT_EQ(bdi->kind, get2->kind);
  EXPECT_EQ(bdi->c_code, get2->c_code);
  EXPECT_EQ(bdi->elementCount, get2->elementCount);
  EXPECT_EQ(bdi->type, get2->type);
  EXPECT_EQ(bdi->reserved, get2->reserved);
  EXPECT_EQ(bdi->initializationData.u64[0],get2->initializationData.u64[0]);
  EXPECT_EQ(bdi->initializationData.u64[1],get2->initializationData.u64[1]);
  EXPECT_EQ(bdi->initializationData.u64[2],get2->initializationData.u64[2]);

  delete lexer;
  delete bdi;
};

TEST(CodegenTest, initializableDeclCodeGen_global_f64) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("global_f64 %g[3] = 1.2L, 1.3L,1.4L ;\n");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

   BrigDirectiveSymbol ref1 = {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
     {
    0,                      // c_code
    BrigGlobalSpace,        // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    3,                      // dim
    0,                      // s_name
    Brigf64,                // type
    1,                      // align
      },
  40,                       // d_init
  0,                        // reserved
   };

  size_t arraySize = sizeof(BrigDirectiveInit) + 2*sizeof(uint64_t); 
  uint8_t *array = new uint8_t[arraySize];

  BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit *>(array);

  bdi->size = arraySize;           //size
  bdi->kind = BrigEDirectiveInit;  //kind
  bdi->c_code = 0;                 //c_code
  bdi->elementCount = 3;           //elementCount
  bdi->type = Brigb64;             //type
  bdi->reserved = 0;               //reserved

  double value = 1.2;             //initializationData
  memmove(&bdi->initializationData.u64[0], &value, sizeof(uint64_t));
  value = 1.3;
  memmove(&bdi->initializationData.u64[1], &value, sizeof(uint64_t));
  value = 1.4;
  memmove(&bdi->initializationData.u64[2], &value, sizeof(uint64_t));


  EXPECT_EQ(0,InitializableDecl(context));
  BrigDirectiveSymbol get1 ;

  context->get_directive(0, &get1);
  EXPECT_EQ(ref1.size, get1.size);
  EXPECT_EQ(ref1.kind, get1.kind);
  EXPECT_EQ(ref1.s.c_code, get1.s.c_code);
  EXPECT_EQ(ref1.s.storageClass, get1.s.storageClass);
  EXPECT_EQ(ref1.s.attribute, get1.s.attribute);
  EXPECT_EQ(ref1.s.reserved, get1.s.reserved);
  EXPECT_EQ(ref1.s.symbolModifier, get1.s.symbolModifier);
  EXPECT_EQ(ref1.s.dim, get1.s.dim);
  EXPECT_EQ(ref1.s.s_name, get1.s.s_name);
  EXPECT_EQ(ref1.s.type, get1.s.type);
  EXPECT_EQ(ref1.s.align, get1.s.align);
  EXPECT_EQ(ref1.s.type, get1.s.type);
  EXPECT_EQ(ref1.d_init, get1.d_init);
  EXPECT_EQ(ref1.reserved, get1.reserved);

  Buffer *dbuf = context->get_directive();
  BrigDirectiveInit *get2 = 
    reinterpret_cast<BrigDirectiveInit*>(&dbuf->get()[40]);
  EXPECT_EQ(bdi->size, get2->size);
  EXPECT_EQ(bdi->kind, get2->kind);
  EXPECT_EQ(bdi->c_code, get2->c_code);
  EXPECT_EQ(bdi->elementCount, get2->elementCount);
  EXPECT_EQ(bdi->type, get2->type);
  EXPECT_EQ(bdi->reserved, get2->reserved);
  EXPECT_EQ(bdi->initializationData.u64[0],get2->initializationData.u64[0]);
  EXPECT_EQ(bdi->initializationData.u64[1],get2->initializationData.u64[1]);
  EXPECT_EQ(bdi->initializationData.u64[2],get2->initializationData.u64[2]);


  delete lexer;
  delete bdi;
};

TEST(CodegenTest, InitializableDeclCodeGen_global_b8) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  std::string input("global_b8 &x[7] = { 1,2,3,4,5,6,7}; ");

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();

  BrigDirectiveSymbol ref = {
  40,                       // size
  BrigEDirectiveSymbol ,    // kind
  {
    0,                      // c_code
    BrigGlobalSpace,        // storag class 
    BrigNone ,              // attribut
    0,                      // reserved
    BrigArray,              // symbolModifier
    8,                      // dim
    0,                      // s_name
    Brigb8,                 // type
    1,                      // align
  },
  40,                       // d_init
  0,                        // reserved
  };

  size_t arraySize = sizeof(BrigDirectiveInit); 
  uint8_t *array = new uint8_t[arraySize];

  BrigDirectiveInit *bdi = reinterpret_cast<BrigDirectiveInit *>(array);

  bdi->size = arraySize;           //size
  bdi->kind = BrigEDirectiveInit;  //kind
  bdi->c_code = 0;                 //c_code
  bdi->elementCount = 7;           //elementCount
  bdi->type = Brigb8;              //type
  bdi->reserved = 0;               //reserved

  bdi->initializationData.u8[0] = 1;    //initializationData
  bdi->initializationData.u8[1] = 2;
  bdi->initializationData.u8[2] = 3;
  bdi->initializationData.u8[3] = 4;
  bdi->initializationData.u8[4] = 5;
  bdi->initializationData.u8[5] = 6;
  bdi->initializationData.u8[6] = 7;
  bdi->initializationData.u8[7] = 0;

  EXPECT_EQ(0,InitializableDecl(context));

  BrigDirectiveSymbol get1 ;
  context->get_directive(0, &get1);
  EXPECT_EQ(ref.size, get1.size);
  EXPECT_EQ(ref.kind, get1.kind);
  EXPECT_EQ(ref.s.c_code, get1.s.c_code);
  EXPECT_EQ(ref.s.storageClass, get1.s.storageClass);
  EXPECT_EQ(ref.s.attribute, get1.s.attribute);
  EXPECT_EQ(ref.s.reserved, get1.s.reserved);
  EXPECT_EQ(ref.s.symbolModifier, get1.s.symbolModifier);
  EXPECT_EQ(ref.s.dim, get1.s.dim);
  EXPECT_EQ(ref.s.s_name, get1.s.s_name);
  EXPECT_EQ(ref.s.type, get1.s.type);
  EXPECT_EQ(ref.s.align, get1.s.align);
  EXPECT_EQ(ref.s.type, get1.s.type);
  EXPECT_EQ(ref.d_init, get1.d_init);
  EXPECT_EQ(ref.reserved, get1.reserved);

  Buffer *dbuf = context->get_directive();
  BrigDirectiveInit *get2 = 
    reinterpret_cast<BrigDirectiveInit*>(&dbuf->get()[40]);
  EXPECT_EQ(bdi->size, get2->size);
  EXPECT_EQ(bdi->kind, get2->kind);
  EXPECT_EQ(bdi->c_code, get2->c_code);
  EXPECT_EQ(bdi->elementCount, get2->elementCount);
  EXPECT_EQ(bdi->type, get2->type);
  EXPECT_EQ(bdi->reserved, get2->reserved);
  EXPECT_EQ(bdi->initializationData.u8[0],get2->initializationData.u8[0]);
  EXPECT_EQ(bdi->initializationData.u8[1],get2->initializationData.u8[1]);
  EXPECT_EQ(bdi->initializationData.u8[2],get2->initializationData.u8[2]);
  EXPECT_EQ(bdi->initializationData.u8[3],get2->initializationData.u8[3]);
  EXPECT_EQ(bdi->initializationData.u8[4],get2->initializationData.u8[4]);
  EXPECT_EQ(bdi->initializationData.u8[5],get2->initializationData.u8[5]);
  EXPECT_EQ(bdi->initializationData.u8[6],get2->initializationData.u8[6]);
  EXPECT_EQ(bdi->initializationData.u8[7],get2->initializationData.u8[7]);

  delete lexer;
  delete bdi;
};


TEST_P(CodegenTestFileDecl,FileDecl)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(inputarray_filedecl[n]);
 
  Lexer* lexer = new Lexer(input);
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, FileDecl(context));

  BrigDirectiveFile get;
  context->get_directive(0, &get);

  BrigDirectiveFile ref = outputarray_filedecl[n];

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.c_code,get.c_code);
  EXPECT_EQ(ref.fileid,get.fileid);
  EXPECT_EQ(ref.s_filename,get.s_filename);
 
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(TestFileDecl,
                        CodegenTestFileDecl,
                        testing::Range(0,2));

TEST_P(TestFileDeclFalseInput, FileDeclFalseInput) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(inputarray_filedecl_false[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, FileDecl(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(TestFileDeclFalse,
                        TestFileDeclFalseInput,
                        testing::Range(0,7));

TEST_P(CodegenTestLocation,Location)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(inputarray_location[n]);
 
  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Location(context));

  BrigDirectiveLoc get;
  context->get_directive(0, &get);

  BrigDirectiveLoc ref = outputarray_location[n];

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.c_code,get.c_code);
  EXPECT_EQ(ref.sourceFile,get.sourceFile);
  EXPECT_EQ(ref.sourceLine,get.sourceLine);
  EXPECT_EQ(ref.sourceColumn,get.sourceColumn);
 
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(TestLocation,
                        CodegenTestLocation,
                        testing::Range(0,1));

TEST_P(TestLocationFalseInput, LocationFalseInput) {
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(inputarray_location_false[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Location(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(TestLocationFalse,
                        TestLocationFalseInput,
                        testing::Range(0,5));


}  // namespace brig
}  // namespace hsa
