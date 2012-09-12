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
#include "hsail2brig_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(CodegenTest_GlobalSamplerDecl,GlobalSamplerDecl)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(inputArray_GlobalSamplerDecl[n]);
 
  Lexer* lexer = new Lexer(input);

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, GlobalSamplerDecl(context));

  BrigDirectiveSampler get;
  context->get_directive(0, &get);

  BrigDirectiveSampler ref = outputArray_GlobalSamplerDecl[n];

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.s.c_code, get.s.c_code);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.attribute, get.s.attribute);
  EXPECT_EQ(ref.s.reserved, get.s.reserved);
  //EXPECT_EQ(ref.s.symbolModifier, get.s.symbolModifier);
  //EXPECT_EQ(ref.s.dim, get.s.dim);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.s.type, get.s.type);
  //EXPECT_EQ(ref.s.align, get.s.align);
  EXPECT_EQ(ref.valid, get.valid);
  EXPECT_EQ(ref.normalized, get.normalized);
  EXPECT_EQ(ref.filter, get.filter);
  EXPECT_EQ(ref.boundaryU, get.boundaryU);
  EXPECT_EQ(ref.boundaryV, get.boundaryV);
  EXPECT_EQ(ref.boundaryW, get.boundaryW);
  EXPECT_EQ(ref.reserved1, get.reserved1);
 
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(TestGlobalSamplerDecl,CodegenTest_GlobalSamplerDecl,testing::Range(0,9));

TEST_P(CodegenTest_GlobalImageDecl,GlobalImageDecl)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(inputArray_GlobalImageDecl[n]);
 
  Lexer* lexer = new Lexer(input);

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, GlobalImageDecl(context));

  BrigDirectiveImage get;
  context->get_directive(0, &get);

  BrigDirectiveImage ref = outputArray_GlobalImageDecl[n];

  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.width, get.width);
  EXPECT_EQ(ref.height, get.height);
  EXPECT_EQ(ref.depth, get.depth);
  //EXPECT_EQ(ref.array, get.array);
  EXPECT_EQ(ref.order, get.order);
  EXPECT_EQ(ref.format, get.format);
  EXPECT_EQ(ref.s.c_code, get.s.c_code);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.attribute, get.s.attribute);
  EXPECT_EQ(ref.s.reserved, get.s.reserved);
  //EXPECT_EQ(ref.s.symbolModifier, get.s.symbolModifier);
  //EXPECT_EQ(ref.s.dim, get.s.dim);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.s.type, get.s.type);
  //EXPECT_EQ(ref.s.align, get.s.align);
 
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(TestGlobalImageDecl,CodegenTest_GlobalImageDecl,testing::Range(0,18));

TEST_P(CodegenTest_GlobalReadOnlyImageDecl,GlobalReadOnlyImageDecl)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(inputArray_GlobalReadOnlyImageDecl[n]);
 
  Lexer* lexer = new Lexer(input);

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, GlobalReadOnlyImageDecl(context));

  BrigDirectiveImage get;
  context->get_directive(0, &get);

  BrigDirectiveImage ref = outputArray_GlobalReadOnlyImageDecl[n];

  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.width, get.width);
  EXPECT_EQ(ref.height, get.height);
  EXPECT_EQ(ref.depth, get.depth);
  //EXPECT_EQ(ref.array, get.array);
  EXPECT_EQ(ref.order, get.order);
  EXPECT_EQ(ref.format, get.format);
  EXPECT_EQ(ref.s.c_code, get.s.c_code);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.attribute, get.s.attribute);
  EXPECT_EQ(ref.s.reserved, get.s.reserved);
  //EXPECT_EQ(ref.s.symbolModifier, get.s.symbolModifier);
  //EXPECT_EQ(ref.s.dim, get.s.dim);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.s.type, get.s.type);
  //EXPECT_EQ(ref.s.align, get.s.align);
 
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(TestGlobalReadOnlyImageDecl,CodegenTest_GlobalReadOnlyImageDecl,testing::Range(0,18));

TEST_P(CodegenTest_GlobalPrivateDecl,GlobalPrivateDecl)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(inputArray_GlobalPrivateDecl[n]);
 
  Lexer* lexer = new Lexer(input);

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, GlobalPrivateDecl(context));

  BrigDirectiveSymbol get;
  context->get_directive(0, &get);

  BrigDirectiveSymbol ref = outputArray_GlobalPrivateDecl[n];

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.s.c_code, get.s.c_code);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.attribute, get.s.attribute);
  EXPECT_EQ(ref.s.reserved, get.s.reserved);
  //EXPECT_EQ(ref.s.symbolModifier, get.s.symbolModifier);
  //EXPECT_EQ(ref.s.dim, get.s.dim);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.s.type, get.s.type);
  //EXPECT_EQ(ref.s.align, get.s.align);
  EXPECT_EQ(ref.s.type, get.s.type);
  EXPECT_EQ(ref.d_init, get.d_init);
  EXPECT_EQ(ref.reserved, get.reserved);
 
  delete lexer;
};

INSTANTIATE_TEST_CASE_P(TestGlobalPrivateDecl,CodegenTest_GlobalPrivateDecl,testing::Range(0,3));

TEST_P(CodegenTest_GlobalGroupDecl,GlobalGroupDecl)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(inputArray_GlobalGroupDecl[n]);
 
  Lexer* lexer = new Lexer(input);

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, GlobalGroupDecl(context));

  BrigDirectiveSymbol get;
  context->get_directive(0, &get);

  BrigDirectiveSymbol ref = outputArray_GlobalGroupDecl[n];

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.s.c_code, get.s.c_code);
  EXPECT_EQ(ref.s.storageClass, get.s.storageClass);
  EXPECT_EQ(ref.s.attribute, get.s.attribute);
  EXPECT_EQ(ref.s.reserved, get.s.reserved);
  //EXPECT_EQ(ref.s.symbolModifier, get.s.symbolModifier);
  //EXPECT_EQ(ref.s.dim, get.s.dim);
  EXPECT_EQ(ref.s.s_name, get.s.s_name);
  EXPECT_EQ(ref.s.type, get.s.type);
  //EXPECT_EQ(ref.s.align, get.s.align);
  EXPECT_EQ(ref.s.type, get.s.type);
  EXPECT_EQ(ref.d_init, get.d_init);
  EXPECT_EQ(ref.reserved, get.reserved);
 
  delete lexer;
};

INSTANTIATE_TEST_CASE_P(TestGlobalGroupDecl,CodegenTest_GlobalGroupDecl,testing::Range(0,3));

}  // namespace brig
}  // namespace hsa
