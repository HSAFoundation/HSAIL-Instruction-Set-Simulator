// Copyright 2012 MulticoreWare Inc.

#include "directive_test.h"

namespace hsa {
namespace brig {

extern ErrorReporter* main_reporter;
extern Context* context;

TEST_P(TestExtension,Extension)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(extension_pair[n].str);
 
  Lexer* lexer = new Lexer(input);

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Extension(context));

  BrigDirectiveExtension get;
  context->get_directive(directive_offset, &get);

  BrigDirectiveExtension ref = extension_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.c_code, get.c_code);
  EXPECT_EQ(ref.s_name, get.s_name);
 
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestExtension, testing::Range(0,1));

TEST_P(TestExtensionInvalid, ExtensionInvalid) 
{
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(inputarray_extension_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Extension(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestExtensionInvalid, testing::Range(0,7));

TEST_P(TestPragma,Pragma)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(pragma_pair[n].str);
 
  Lexer* lexer = new Lexer(input);

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Pragma(context));

  BrigDirectivePragma get;
  context->get_directive(directive_offset, &get);

  BrigDirectivePragma ref = pragma_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.c_code, get.c_code);
  EXPECT_EQ(ref.s_name, get.s_name);
 
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestPragma, testing::Range(0,1));

TEST_P(TestPragmaInvalid, PragmaInvalid) 
{
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(inputarray_pragma_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Pragma(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestPragmaInvalid, testing::Range(0,5));

TEST_P(TestControl,Control)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(control_pair[n].str);
 
  Lexer* lexer = new Lexer(input);

  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, Control(context));

  BrigDirectiveControl get;
  context->get_directive(directive_offset, &get);

  BrigDirectiveControl ref = control_pair[n].ref;

  EXPECT_EQ(ref.size, get.size);
  EXPECT_EQ(ref.kind, get.kind);
  EXPECT_EQ(ref.c_code, get.c_code);
  EXPECT_EQ(ref.controlType, get.controlType);
  EXPECT_EQ(ref.values[0], get.values[0]);
  EXPECT_EQ(ref.values[1], get.values[1]);
  EXPECT_EQ(ref.values[2], get.values[2]);

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestControl, testing::Range(0,4));

TEST_P(TestControlInvalid, ControlInvalid) 
{
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(inputarray_control_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, Control(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestControlInvalid, testing::Range(0,6));

 TEST_P(TestFileDecl,FileDecl)
{
  context->set_error_reporter(main_reporter);
  context->clear_context();
 
  int n = GetParam();
  std::string input(filedecl_pair[n].str);
 
  Lexer* lexer = new Lexer(input);
  lexer->set_source_string(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_EQ(0, FileDecl(context));

  BrigDirectiveFile get;
  context->get_directive(directive_offset, &get);

  BrigDirectiveFile ref = filedecl_pair[n].ref;

  EXPECT_EQ(ref.size,get.size);
  EXPECT_EQ(ref.kind,get.kind);
  EXPECT_EQ(ref.c_code,get.c_code);
  EXPECT_EQ(ref.fileid,get.fileid);
  EXPECT_EQ(ref.s_filename,get.s_filename);
 
  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestFileDecl, testing::Range(0,2));

TEST_P(TestFileDeclInvalid, FileDeclInvalidInput) 
{
  context->set_error_reporter(main_reporter);
  context->clear_context();

  int n = GetParam();
  std::string input(inputarray_filedecl_invalid[n]);

  Lexer* lexer = new Lexer(input);
  context->token_to_scan = lexer->get_next_token();
  EXPECT_NE(0, FileDecl(context));

  delete lexer;
}

INSTANTIATE_TEST_CASE_P(CodegenTest, TestFileDeclInvalid, testing::Range(0,7));

}  // namespace brig
}  // namespace hsa
