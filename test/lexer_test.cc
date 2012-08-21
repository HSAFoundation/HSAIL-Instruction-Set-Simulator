/* Copyright 2012 <MulticorewareInc> */

#include <string>

#include "gtest/gtest.h"
#include "tokens.h"
#include "lexer_wrapper.h"
#include "context.h"

namespace hsa {
namespace brig {

Context* context = Context::get_instance();

// ------------------ BASIC LEXER TESTS -----------------
TEST(LexTest, Bug2_DecIntegerConstant) {
  std::string input("12345");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_INTEGER_CONSTANT, lexer->get_next_token());
  EXPECT_EQ(12345, context->token_value.int_val);

  delete lexer;
}

TEST(LexTest, Bug3_OctIntegerConstant) {
  std::string input("030");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_INTEGER_CONSTANT, lexer->get_next_token());
  EXPECT_EQ(24, context->token_value.int_val);

  delete lexer;
}

TEST(LexTest, Bug4_CRegister) {
  std::string input("$c7");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_CREGISTER, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug5_DRegister) {
  std::string input("$d7");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_DREGISTER, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug6_SRegister) {
  std::string input("$s15");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_SREGISTER, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug7_QRegister) {
  std::string input("$q5");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_QREGISTER, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug8_Label) {
  std::string input("@Go_to_this");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_LABEL, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug9_Comment) {
  std::string input("/* this is a comment */");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_COMMENT, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug10_Comment) {
  std::string input("//this is an inline comment\n");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_COMMENT, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug11_HexIntegerConstant) {
  std::string input("0x11");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_INTEGER_CONSTANT, lexer->get_next_token());
  EXPECT_EQ(17, context->token_value.int_val);

  delete lexer;
}

TEST(LexTest, Bug12_DecSingleConstant) {
  std::string input("0.5e3f");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_SINGLE_CONSTANT, lexer->get_next_token());
  EXPECT_EQ(0.5e3f, context->token_value.float_val);

  delete lexer;
}

TEST(LexTest, Bug17_LocalId) {
  std::string input("%Test_id_123");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_LOCAL_IDENTIFIER, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug18_GlobalId) {
  std::string input("&Test_global_id_123");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_GLOBAL_IDENTIFIER, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug19_String) {
  std::string input("\" This is a string\"");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_STRING, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug20_TokenProperty) {
  std::string input("snorm_int8");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_PROPERTY, lexer->get_next_token());

  input.assign("unorm_int16");
  lexer->set_source_string(input);
  EXPECT_EQ(TOKEN_PROPERTY, lexer->get_next_token());

  input.assign("rx");
  lexer->set_source_string(input);
  EXPECT_EQ(TOKEN_PROPERTY, lexer->get_next_token());

  input.assign("intensity");
  lexer->set_source_string(input);
  EXPECT_EQ(TOKEN_PROPERTY, lexer->get_next_token());

  input.assign("wrap");
  lexer->set_source_string(input);
  EXPECT_EQ(TOKEN_PROPERTY, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug21_TokenWavesize) {
  std::string input("WAVESIZE");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_WAVESIZE, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug22_CommonKeywords) {          // common keywords
  std::string input("workgroupid");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(WORKGROUPID, lexer->get_next_token());

  input.assign("version");
  lexer->set_source_string(input);
  EXPECT_EQ(VERSION, lexer->get_next_token());

  input.assign("global");
  lexer->set_source_string(input);
  EXPECT_EQ(GLOBAL, lexer->get_next_token());

  input.assign("_Samp");
  lexer->set_source_string(input);
  EXPECT_EQ(_SAMP, lexer->get_next_token());

  input.assign("_RWImg");
  lexer->set_source_string(input);
  EXPECT_EQ(_RWIMG, lexer->get_next_token());

  input.assign("_ROImg");
  lexer->set_source_string(input);
  EXPECT_EQ(_ROIMG, lexer->get_next_token());

  input.assign("align");
  lexer->set_source_string(input);
  EXPECT_EQ(ALIGN, lexer->get_next_token());

  input.assign("file");
  lexer->set_source_string(input);
  EXPECT_EQ(_FILE, lexer->get_next_token());

  input.assign("arg");
  lexer->set_source_string(input);
  EXPECT_EQ(ARG, lexer->get_next_token());

  input.assign("kernarg");
  lexer->set_source_string(input);
  EXPECT_EQ(KERNARG, lexer->get_next_token());

  input.assign("function");
  lexer->set_source_string(input);
  EXPECT_EQ(FUNCTION, lexer->get_next_token());

  input.assign(":fbar");
  lexer->set_source_string(input);
  EXPECT_EQ(_FBAR, lexer->get_next_token());

  input.assign("signature");
  lexer->set_source_string(input);
  EXPECT_EQ(SIGNATURE, lexer->get_next_token());

  input.assign("block");
  lexer->set_source_string(input);
  EXPECT_EQ(BLOCK, lexer->get_next_token());

  input.assign("endblock");
  lexer->set_source_string(input);
  EXPECT_EQ(ENDBLOCK, lexer->get_next_token());

  input.assign("blocknumeric");
  lexer->set_source_string(input);
  EXPECT_EQ(BLOCKNUMERIC, lexer->get_next_token());

  input.assign("blockstring");
  lexer->set_source_string(input);
  EXPECT_EQ(BLOCKSTRING, lexer->get_next_token());

  input.assign("kernel");
  lexer->set_source_string(input);
  EXPECT_EQ(KERNEL, lexer->get_next_token());

  input.assign("pragma");
  lexer->set_source_string(input);
  EXPECT_EQ(PRAGMA, lexer->get_next_token());

  input.assign("labeltargets");
  lexer->set_source_string(input);
  EXPECT_EQ(LABELTARGETS, lexer->get_next_token());

  input.assign("extension");
  lexer->set_source_string(input);
  EXPECT_EQ(EXTENSION, lexer->get_next_token());

  input.assign("extern");
  lexer->set_source_string(input);
  EXPECT_EQ(EXTERN, lexer->get_next_token());

  input.assign("static");
  lexer->set_source_string(input);
  EXPECT_EQ(STATIC, lexer->get_next_token());

  input.assign("const");
  lexer->set_source_string(input);
  EXPECT_EQ(CONST, lexer->get_next_token());

  input.assign("private");
  lexer->set_source_string(input);
  EXPECT_EQ(PRIVATE, lexer->get_next_token());

  input.assign("spill");
  lexer->set_source_string(input);
  EXPECT_EQ(SPILL, lexer->get_next_token());

  input.assign("group");
  lexer->set_source_string(input);
  EXPECT_EQ(GROUP, lexer->get_next_token());

  input.assign("readonly");
  lexer->set_source_string(input);
  EXPECT_EQ(READONLY, lexer->get_next_token());

  input.assign("loc");
  lexer->set_source_string(input);
  EXPECT_EQ(LOC, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug23_HexSingleConstant) {
  std::string input("0x1.0p0f");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_SINGLE_CONSTANT, lexer->get_next_token());
  EXPECT_EQ(1.0f, context->token_value.float_val);

  delete lexer;
}

TEST(LexTest, Bug24_IEEESingleConstant) {
  std::string input("0f3F800000");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_SINGLE_CONSTANT, lexer->get_next_token());
  EXPECT_EQ(1.0f, context->token_value.float_val);

  delete lexer;
}

TEST(LexTest, Bug25_AddressSpaceId) {
  // addressSpaceIdentifier keywords
  std::string input("_readonly");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(_READONLY, lexer->get_next_token());

  input.assign("_kernarg");
  lexer->set_source_string(input);
  EXPECT_EQ(_KERNARG, lexer->get_next_token());

  input.assign("_global");
  lexer->set_source_string(input);
  EXPECT_EQ(_GLOBAL, lexer->get_next_token());

  input.assign("_private");
  lexer->set_source_string(input);
  EXPECT_EQ(_PRIVATE, lexer->get_next_token());

  input.assign("_arg");
  lexer->set_source_string(input);
  EXPECT_EQ(_ARG, lexer->get_next_token());

  input.assign("_group");
  lexer->set_source_string(input);
  EXPECT_EQ(_GROUP, lexer->get_next_token());

  input.assign("_spill");
  lexer->set_source_string(input);
  EXPECT_EQ(_SPILL, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug26_DecDoubleConstant) {
  std::string input("0.5e3l");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_DOUBLE_CONSTANT, lexer->get_next_token());
  EXPECT_EQ(0.5e3, context->token_value.double_val);

  delete lexer;
}

TEST(LexTest, Bug27_VectorKeywords) {              // keywords _v2 and _v4
  std::string input("_v2");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(_V2, lexer->get_next_token());

  input.assign("_v4");
  lexer->set_source_string(input);
  EXPECT_EQ(_V4, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug28_HexDoubleConstant) {
  std::string input("0x1.0l");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_DOUBLE_CONSTANT, lexer->get_next_token());
  EXPECT_EQ(1.0, context->token_value.double_val);

  delete lexer;
}

TEST(LexTest, Bug29_IEEEDoubleConstant) {
  std::string input("0d3FF0000000000000");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(TOKEN_DOUBLE_CONSTANT, lexer->get_next_token());
  EXPECT_EQ(1.0, context->token_value.double_val);

  delete lexer;
}

// keywords format, order, coord, filter, boundaryU, boundaryV, boundaryW
TEST(LexTest, Bug30_CommonKeywords2) {
  std::string input("format");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(FORMAT, lexer->get_next_token());

  input.assign("order");
  lexer->set_source_string(input);
  EXPECT_EQ(ORDER, lexer->get_next_token());

  input.assign("coord");
  lexer->set_source_string(input);
  EXPECT_EQ(COORD, lexer->get_next_token());

  input.assign("filter");
  lexer->set_source_string(input);
  EXPECT_EQ(FILTER, lexer->get_next_token());

  input.assign("boundaryU");
  lexer->set_source_string(input);
  EXPECT_EQ(BOUNDARYU, lexer->get_next_token());

  input.assign("boundaryV");
  lexer->set_source_string(input);
  EXPECT_EQ(BOUNDARYV, lexer->get_next_token());

  input.assign("boundaryW");
  lexer->set_source_string(input);
  EXPECT_EQ(BOUNDARYW, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug33_ControlKeywords) {              // control keywords
  std::string input("itemsperworkgroup");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(ITEMS_PER_WORKGROUP, lexer->get_next_token());

  input.assign("workgroupspercu");
  lexer->set_source_string(input);
  EXPECT_EQ(WORKGROUPS_PER_CU, lexer->get_next_token());

  input.assign("memopt_on");
  lexer->set_source_string(input);
  EXPECT_EQ(MEMOPT_ON, lexer->get_next_token());

  input.assign("memopt_off");
  lexer->set_source_string(input);
  EXPECT_EQ(MEMOPT_OFF, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug34_Puctuations) {              // punctuations
  std::string input("+");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ('+', lexer->get_next_token());

  input.assign("-");
  lexer->set_source_string(input);
  EXPECT_EQ('-', lexer->get_next_token());

  input.assign("=");
  lexer->set_source_string(input);
  EXPECT_EQ('=', lexer->get_next_token());

  input.assign("<");
  lexer->set_source_string(input);
  EXPECT_EQ('<', lexer->get_next_token());


  input.assign(">");
  lexer->set_source_string(input);
  EXPECT_EQ('>', lexer->get_next_token());

  input.assign("[");
  lexer->set_source_string(input);
  EXPECT_EQ('[', lexer->get_next_token());

  input.assign("]");
  lexer->set_source_string(input);
  EXPECT_EQ(']', lexer->get_next_token());

  input.assign("{");
  lexer->set_source_string(input);
  EXPECT_EQ('{', lexer->get_next_token());

  input.assign("}");
  lexer->set_source_string(input);
  EXPECT_EQ('}', lexer->get_next_token());

  input.assign("(");
  lexer->set_source_string(input);
  EXPECT_EQ('(', lexer->get_next_token());

  input.assign(")");
  lexer->set_source_string(input);
  EXPECT_EQ(')', lexer->get_next_token());

  input.assign(":");
  lexer->set_source_string(input);
  EXPECT_EQ(':', lexer->get_next_token());

  input.assign(";");
  lexer->set_source_string(input);
  EXPECT_EQ(';', lexer->get_next_token());

  input.assign(",");
  lexer->set_source_string(input);
  EXPECT_EQ(',', lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug37_Opcodes) {              // opcodes
  std::string input("fbar_initSizeWg");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(FBAR_INITSIZEWG, lexer->get_next_token());

  input.assign("fbar_wait");
  lexer->set_source_string(input);
  EXPECT_EQ(FBAR_WAIT, lexer->get_next_token());

  input.assign("fbar_arrive");
  lexer->set_source_string(input);
  EXPECT_EQ(FBAR_ARRIVE, lexer->get_next_token());

  input.assign("fbar_skip");
  lexer->set_source_string(input);
  EXPECT_EQ(FBAR_SKIP, lexer->get_next_token());

  input.assign("fbar_release");
  lexer->set_source_string(input);
  EXPECT_EQ(FBAR_RELEASE, lexer->get_next_token());

  input.assign("countup");
  lexer->set_source_string(input);
  EXPECT_EQ(COUNTUP, lexer->get_next_token());

  input.assign("laneid");
  lexer->set_source_string(input);
  EXPECT_EQ(LANEID, lexer->get_next_token());

  input.assign("dynwaveid");
  lexer->set_source_string(input);
  EXPECT_EQ(DYNWAVEID, lexer->get_next_token());

  input.assign("maxdynwaveid");
  lexer->set_source_string(input);
  EXPECT_EQ(MAXDYNWAVEID, lexer->get_next_token());

  input.assign("dispatchid");
  lexer->set_source_string(input);
  EXPECT_EQ(DISPATCHID, lexer->get_next_token());

  input.assign("cu");
  lexer->set_source_string(input);
  EXPECT_EQ(CU, lexer->get_next_token());

  input.assign("workdim");
  lexer->set_source_string(input);
  EXPECT_EQ(WORKDIM, lexer->get_next_token());

  input.assign("workitemid_flat");
  lexer->set_source_string(input);
  EXPECT_EQ(WORKITEMID_FLAT, lexer->get_next_token());

  input.assign("workitemaid_flat");
  lexer->set_source_string(input);
  EXPECT_EQ(WORKITEMAID_FLAT, lexer->get_next_token());

  input.assign("debugtrap");
  lexer->set_source_string(input);
  EXPECT_EQ(DEBUGTRAP, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug38_Instruction2Opcode) {
  std::string input("abs");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(ABS, lexer->get_next_token());

  input.assign("neg");
  lexer->set_source_string(input);
  EXPECT_EQ(NEG, lexer->get_next_token());

  input.assign("not");
  lexer->set_source_string(input);
  EXPECT_EQ(NOT, lexer->get_next_token());

  input.assign("popcount");
  lexer->set_source_string(input);
  EXPECT_EQ(POPCOUNT, lexer->get_next_token());

  input.assign("firstbit");
  lexer->set_source_string(input);
  EXPECT_EQ(FIRSTBIT, lexer->get_next_token());

  input.assign("lastbit");
  lexer->set_source_string(input);
  EXPECT_EQ(LASTBIT, lexer->get_next_token());

  input.assign("bitrev");
  lexer->set_source_string(input);
  EXPECT_EQ(BITREV, lexer->get_next_token());

  input.assign("movs_lo");
  lexer->set_source_string(input);
  EXPECT_EQ(MOVS_LO, lexer->get_next_token());

  input.assign("movs_hi");
  lexer->set_source_string(input);
  EXPECT_EQ(MOVS_HI, lexer->get_next_token());

  input.assign("fbar_initSize");
  lexer->set_source_string(input);
  EXPECT_EQ(FBAR_INITSIZE, lexer->get_next_token());

  input.assign("fbar_releaseCF");
  lexer->set_source_string(input);
  EXPECT_EQ(FBAR_RELEASECF, lexer->get_next_token());

  input.assign("count");
  lexer->set_source_string(input);
  EXPECT_EQ(COUNT, lexer->get_next_token());

  input.assign("mask");
  lexer->set_source_string(input);
  EXPECT_EQ(MASK, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug39_Instruction2OpcodeFTZ) {
  std::string input("sqrt");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(SQRT, lexer->get_next_token());

  input.assign("fract");
  lexer->set_source_string(input);
  EXPECT_EQ(FRACT, lexer->get_next_token());

  input.assign("fcos");
  lexer->set_source_string(input);
  EXPECT_EQ(FCOS, lexer->get_next_token());

  input.assign("fsin");
  lexer->set_source_string(input);
  EXPECT_EQ(FSIN, lexer->get_next_token());

  input.assign("flog2");
  lexer->set_source_string(input);
  EXPECT_EQ(FLOG2, lexer->get_next_token());

  input.assign("fexp2");
  lexer->set_source_string(input);
  EXPECT_EQ(FEXP2, lexer->get_next_token());

  input.assign("fsqrt");
  lexer->set_source_string(input);
  EXPECT_EQ(FSQRT, lexer->get_next_token());

  input.assign("frsqrt");
  lexer->set_source_string(input);
  EXPECT_EQ(FRSQRT, lexer->get_next_token());

  input.assign("frcp");
  lexer->set_source_string(input);
  EXPECT_EQ(FRCP, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug40_Instruction2OpcodeNoDT) {
  std::string input("unpack3");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(UNPACK3, lexer->get_next_token());

  input.assign("unpack2");
  lexer->set_source_string(input);
  EXPECT_EQ(UNPACK2, lexer->get_next_token());

  input.assign("unpack1");
  lexer->set_source_string(input);
  EXPECT_EQ(UNPACK1, lexer->get_next_token());

  input.assign("unpack0");
  lexer->set_source_string(input);
  EXPECT_EQ(UNPACK0, lexer->get_next_token());


  input.assign("alloca");
  lexer->set_source_string(input);
  EXPECT_EQ(ALLOCA, lexer->get_next_token());

  input.assign("workitemid");
  lexer->set_source_string(input);
  EXPECT_EQ(WORKITEMID, lexer->get_next_token());

  input.assign("workitemaid");
  lexer->set_source_string(input);
  EXPECT_EQ(WORKITEMAID, lexer->get_next_token());

  input.assign("workgroupsize");
  lexer->set_source_string(input);
  EXPECT_EQ(WORKGROUPSIZE, lexer->get_next_token());

  input.assign("NDRangesize");
  lexer->set_source_string(input);
  EXPECT_EQ(NDRANGESIZE, lexer->get_next_token());

  input.assign("NDRangegroups");
  lexer->set_source_string(input);
  EXPECT_EQ(NDRANGEGROUPS, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug41_Instruction3Opcode) {
  std::string input("add");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(ADD, lexer->get_next_token());

  input.assign("carry");
  lexer->set_source_string(input);
  EXPECT_EQ(CARRY, lexer->get_next_token());

  input.assign("borrow");
  lexer->set_source_string(input);
  EXPECT_EQ(BORROW, lexer->get_next_token());

  input.assign("div");
  lexer->set_source_string(input);
  EXPECT_EQ(DIV, lexer->get_next_token());

  input.assign("rem");
  lexer->set_source_string(input);
  EXPECT_EQ(REM, lexer->get_next_token());

  input.assign("sub");
  lexer->set_source_string(input);
  EXPECT_EQ(SUB, lexer->get_next_token());

  input.assign("shl");
  lexer->set_source_string(input);
  EXPECT_EQ(SHL, lexer->get_next_token());

  input.assign("shr");
  lexer->set_source_string(input);
  EXPECT_EQ(SHR, lexer->get_next_token());

  input.assign("and");
  lexer->set_source_string(input);
  EXPECT_EQ(AND, lexer->get_next_token());

  input.assign("xor");
  lexer->set_source_string(input);
  EXPECT_EQ(XOR, lexer->get_next_token());

  input.assign("or");
  lexer->set_source_string(input);
  EXPECT_EQ(OR, lexer->get_next_token());

  input.assign("unpacklo");
  lexer->set_source_string(input);
  EXPECT_EQ(UNPACKLO, lexer->get_next_token());

  input.assign("unpackhi");
  lexer->set_source_string(input);
  EXPECT_EQ(UNPACKHI, lexer->get_next_token());

  input.assign("movd_lo");
  lexer->set_source_string(input);
  EXPECT_EQ(MOVD_LO, lexer->get_next_token());

  input.assign("movd_hi");
  lexer->set_source_string(input);
  EXPECT_EQ(MOVD_HI, lexer->get_next_token());

  input.assign("copysign");
  lexer->set_source_string(input);
  EXPECT_EQ(COPYSIGN, lexer->get_next_token());

  input.assign("class");
  lexer->set_source_string(input);
  EXPECT_EQ(CLASS, lexer->get_next_token());

  input.assign("send");
  lexer->set_source_string(input);
  EXPECT_EQ(SEND, lexer->get_next_token());

  input.assign("receive");
  lexer->set_source_string(input);
  EXPECT_EQ(RECEIVE, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug42_Instruction3OpcodeFTZ_Instruction4Opcode) {
  std::string input("max");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(MAX, lexer->get_next_token());

  input.assign("min");
  lexer->set_source_string(input);
  EXPECT_EQ(MIN, lexer->get_next_token());

  input.assign("mad");
  lexer->set_source_string(input);
  EXPECT_EQ(MAD, lexer->get_next_token());

  input.assign("extract");
  lexer->set_source_string(input);
  EXPECT_EQ(EXTRACT, lexer->get_next_token());

  input.assign("insert");
  lexer->set_source_string(input);
  EXPECT_EQ(INSERT, lexer->get_next_token());

  input.assign("shuffle");
  lexer->set_source_string(input);
  EXPECT_EQ(SHUFFLE, lexer->get_next_token());

  input.assign("cmov");
  lexer->set_source_string(input);
  EXPECT_EQ(CMOV, lexer->get_next_token());

  input.assign("fma");
  lexer->set_source_string(input);
  EXPECT_EQ(FMA, lexer->get_next_token());

  input.assign("bitalign");
  lexer->set_source_string(input);
  EXPECT_EQ(BITALIGN, lexer->get_next_token());

  input.assign("bytealign");
  lexer->set_source_string(input);
  EXPECT_EQ(BYTEALIGN, lexer->get_next_token());

  input.assign("lerp");
  lexer->set_source_string(input);
  EXPECT_EQ(LERP, lexer->get_next_token());

  input.assign("sad");
  lexer->set_source_string(input);
  EXPECT_EQ(SAD, lexer->get_next_token());

  input.assign("sad2");
  lexer->set_source_string(input);
  EXPECT_EQ(SAD2, lexer->get_next_token());

  input.assign("sad4");
  lexer->set_source_string(input);
  EXPECT_EQ(SAD4, lexer->get_next_token());

  input.assign("sad4hi");
  lexer->set_source_string(input);
  EXPECT_EQ(SAD4HI, lexer->get_next_token());

  input.assign("bitselect");
  lexer->set_source_string(input);
  EXPECT_EQ(BITSELECT, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug43_AtomicOperationId) {              // AtomicOperationId
  std::string input("_and");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(_AND_, lexer->get_next_token());

  input.assign("_or");
  lexer->set_source_string(input);
  EXPECT_EQ(_OR_, lexer->get_next_token());

  input.assign("_xor");
  lexer->set_source_string(input);
  EXPECT_EQ(_XOR_, lexer->get_next_token());

  input.assign("_exch");
  lexer->set_source_string(input);
  EXPECT_EQ(_EXCH_, lexer->get_next_token());

  input.assign("_add");
  lexer->set_source_string(input);
  EXPECT_EQ(_ADD_, lexer->get_next_token());

  input.assign("_sub");
  lexer->set_source_string(input);
  EXPECT_EQ(_SUB_, lexer->get_next_token());

  input.assign("_inc");
  lexer->set_source_string(input);
  EXPECT_EQ(_INC_, lexer->get_next_token());

  input.assign("_dec");
  lexer->set_source_string(input);
  EXPECT_EQ(_DEC_, lexer->get_next_token());

  input.assign("_max");
  lexer->set_source_string(input);
  EXPECT_EQ(_MAX_, lexer->get_next_token());

  input.assign("_min");
  lexer->set_source_string(input);
  EXPECT_EQ(_MIN_, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug44_ComparisonId) {              // Comparison
  std::string input("_eq");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(_EQ, lexer->get_next_token());

  input.assign("_ne");
  lexer->set_source_string(input);
  EXPECT_EQ(_NE, lexer->get_next_token());

  input.assign("_lt");
  lexer->set_source_string(input);
  EXPECT_EQ(_LT, lexer->get_next_token());

  input.assign("_le");
  lexer->set_source_string(input);
  EXPECT_EQ(_LE, lexer->get_next_token());

  input.assign("_gt");
  lexer->set_source_string(input);
  EXPECT_EQ(_GT, lexer->get_next_token());

  input.assign("_ge");
  lexer->set_source_string(input);
  EXPECT_EQ(_GE, lexer->get_next_token());

  input.assign("_equ");
  lexer->set_source_string(input);
  EXPECT_EQ(_EQU, lexer->get_next_token());

  input.assign("_neu");
  lexer->set_source_string(input);
  EXPECT_EQ(_NEU, lexer->get_next_token());

  input.assign("_ltu");
  lexer->set_source_string(input);
  EXPECT_EQ(_LTU, lexer->get_next_token());

  input.assign("_leu");
  lexer->set_source_string(input);
  EXPECT_EQ(_LEU, lexer->get_next_token());

  input.assign("_gtu");
  lexer->set_source_string(input);
  EXPECT_EQ(_GTU, lexer->get_next_token());

  input.assign("_geu");
  lexer->set_source_string(input);
  EXPECT_EQ(_GEU, lexer->get_next_token());

  input.assign("_num");
  lexer->set_source_string(input);
  EXPECT_EQ(_NUM, lexer->get_next_token());

  input.assign("_nan");
  lexer->set_source_string(input);
  EXPECT_EQ(_NAN, lexer->get_next_token());

  input.assign("_seq");
  lexer->set_source_string(input);
  EXPECT_EQ(_SEQ, lexer->get_next_token());

  input.assign("_sne");
  lexer->set_source_string(input);
  EXPECT_EQ(_SNE, lexer->get_next_token());

  input.assign("_slt");
  lexer->set_source_string(input);
  EXPECT_EQ(_SLT, lexer->get_next_token());

  input.assign("_sle");
  lexer->set_source_string(input);
  EXPECT_EQ(_SLE, lexer->get_next_token());

  input.assign("_sgt");
  lexer->set_source_string(input);
  EXPECT_EQ(_SGT, lexer->get_next_token());

  input.assign("_sge");
  lexer->set_source_string(input);
  EXPECT_EQ(_SGE, lexer->get_next_token());

  input.assign("_snum");
  lexer->set_source_string(input);
  EXPECT_EQ(_SNUM, lexer->get_next_token());

  input.assign("_snan");
  lexer->set_source_string(input);
  EXPECT_EQ(_SNAN, lexer->get_next_token());

  input.assign("_sequ");
  lexer->set_source_string(input);
  EXPECT_EQ(_SEQU, lexer->get_next_token());

  input.assign("_sneu");
  lexer->set_source_string(input);
  EXPECT_EQ(_SNEU, lexer->get_next_token());

  input.assign("_sltu");
  lexer->set_source_string(input);
  EXPECT_EQ(_SLTU, lexer->get_next_token());

  input.assign("_sleu");
  lexer->set_source_string(input);
  EXPECT_EQ(_SLEU, lexer->get_next_token());

  input.assign("_sgtu");
  lexer->set_source_string(input);
  EXPECT_EQ(_SGTU, lexer->get_next_token());

  input.assign("_sgeu");
  lexer->set_source_string(input);
  EXPECT_EQ(_SGEU, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug45_QueryOperation) {              // keywords for queryOp
  std::string input("query_order");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(QUERY_ORDER, lexer->get_next_token());

  input.assign("query_data");
  lexer->set_source_string(input);
  EXPECT_EQ(QUERY_DATA, lexer->get_next_token());

  input.assign("query_array");
  lexer->set_source_string(input);
  EXPECT_EQ(QUERY_ARRAY, lexer->get_next_token());

  input.assign("query_width");
  lexer->set_source_string(input);
  EXPECT_EQ(QUERY_WIDTH, lexer->get_next_token());

  input.assign("query_depth");
  lexer->set_source_string(input);
  EXPECT_EQ(QUERY_DEPTH, lexer->get_next_token());

  input.assign("query_height");
  lexer->set_source_string(input);
  EXPECT_EQ(QUERY_HEIGHT, lexer->get_next_token());

  input.assign("query_normalized");
  lexer->set_source_string(input);
  EXPECT_EQ(QUERY_NORMALIZED, lexer->get_next_token());

  input.assign("query_filtering");
  lexer->set_source_string(input);
  EXPECT_EQ(QUERY_FILTERING, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug46_RoundingModes) {              // Rounding modes
  std::string input("_upi");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(_UPI, lexer->get_next_token());

  input.assign("_downi");
  lexer->set_source_string(input);
  EXPECT_EQ(_DOWNI, lexer->get_next_token());

  input.assign("_zeroi");
  lexer->set_source_string(input);
  EXPECT_EQ(_ZEROI, lexer->get_next_token());

  input.assign("_neari");
  lexer->set_source_string(input);
  EXPECT_EQ(_NEARI, lexer->get_next_token());

  input.assign("_up");
  lexer->set_source_string(input);
  EXPECT_EQ(_UP, lexer->get_next_token());

  input.assign("_down");
  lexer->set_source_string(input);
  EXPECT_EQ(_DOWN, lexer->get_next_token());

  input.assign("_zero");
  lexer->set_source_string(input);
  EXPECT_EQ(_ZERO, lexer->get_next_token());

  input.assign("_near");
  lexer->set_source_string(input);
  EXPECT_EQ(_NEAR, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug47_PackingModes) {              // packing modes
  std::string input("_pp");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(_PP, lexer->get_next_token());

  input.assign("_ps");
  lexer->set_source_string(input);
  EXPECT_EQ(_PS, lexer->get_next_token());

  input.assign("_sp");
  lexer->set_source_string(input);
  EXPECT_EQ(_SP, lexer->get_next_token());

  input.assign("_ss");
  lexer->set_source_string(input);
  EXPECT_EQ(_SS, lexer->get_next_token());

  input.assign("_s");
  lexer->set_source_string(input);
  EXPECT_EQ(__S, lexer->get_next_token());

  input.assign("_p");
  lexer->set_source_string(input);
  EXPECT_EQ(__P, lexer->get_next_token());

  input.assign("_pp_sat");
  lexer->set_source_string(input);
  EXPECT_EQ(_PP_SAT, lexer->get_next_token());

  input.assign("_ps_sat");
  lexer->set_source_string(input);
  EXPECT_EQ(_PS_SAT, lexer->get_next_token());

  input.assign("_sp_sat");
  lexer->set_source_string(input);
  EXPECT_EQ(_SP_SAT, lexer->get_next_token());

  input.assign("_ss_sat");
  lexer->set_source_string(input);
  EXPECT_EQ(_SS_SAT, lexer->get_next_token());

  input.assign("_p_sat");
  lexer->set_source_string(input);
  EXPECT_EQ(_P_SAT, lexer->get_next_token());

  input.assign("_s_sat");
  lexer->set_source_string(input);
  EXPECT_EQ(_S_SAT, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug48_GeometryId) {              // keywords for geometry ID
  std::string input("_1d");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(_1D, lexer->get_next_token());

  input.assign("_2d");
  lexer->set_source_string(input);
  EXPECT_EQ(_2D, lexer->get_next_token());

  input.assign("_3d");
  lexer->set_source_string(input);
  EXPECT_EQ(_3D, lexer->get_next_token());

  input.assign("_1db");
  lexer->set_source_string(input);
  EXPECT_EQ(_1DB, lexer->get_next_token());

  input.assign("_1da");
  lexer->set_source_string(input);
  EXPECT_EQ(_1DA, lexer->get_next_token());

  input.assign("_2da");
  lexer->set_source_string(input);
  EXPECT_EQ(_2DA, lexer->get_next_token());

  input.assign("rd_image");
  lexer->set_source_string(input);
  EXPECT_EQ(RD_IMAGE, lexer->get_next_token());

  input.assign("ld_image");
  lexer->set_source_string(input);
  EXPECT_EQ(LD_IMAGE, lexer->get_next_token());

  input.assign("st_image");
  lexer->set_source_string(input);
  EXPECT_EQ(ST_IMAGE, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug49_Targets) {              // targets
  std::string input("$small");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(_SMALL, lexer->get_next_token());

  input.assign("$large");
  lexer->set_source_string(input);
  EXPECT_EQ(_LARGE, lexer->get_next_token());

  input.assign("$full");
  lexer->set_source_string(input);
  EXPECT_EQ(_FULL, lexer->get_next_token());

  input.assign("$reduced");
  lexer->set_source_string(input);
  EXPECT_EQ(_REDUCED, lexer->get_next_token());

  input.assign("$sftz");
  lexer->set_source_string(input);
  EXPECT_EQ(_SFTZ, lexer->get_next_token());

  input.assign("$nosftz");
  lexer->set_source_string(input);
  EXPECT_EQ(_NOSFTZ, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug50_WidthDepthHeight_Keywords) {
  std::string input("width");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(WIDTH, lexer->get_next_token());

  input.assign("height");
  lexer->set_source_string(input);
  EXPECT_EQ(HEIGHT, lexer->get_next_token());

  input.assign("depth");
  lexer->set_source_string(input);
  EXPECT_EQ(DEPTH, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug51_AtomModifier) {              // keywords for Atom Modifiers
  std::string input("_ar");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(_AR, lexer->get_next_token());

  input.assign("_region");
  lexer->set_source_string(input);
  EXPECT_EQ(_REGION, lexer->get_next_token());

  input.assign("atomic_cas");
  lexer->set_source_string(input);
  EXPECT_EQ(ATOMIC_CAS, lexer->get_next_token());

  input.assign("atomic");
  lexer->set_source_string(input);
  EXPECT_EQ(ATOMIC, lexer->get_next_token());

  input.assign("atomicNoRet");
  lexer->set_source_string(input);
  EXPECT_EQ(ATOMICNORET, lexer->get_next_token());

  input.assign("atomicNoRet_cas");
  lexer->set_source_string(input);
  EXPECT_EQ(ATOMICNORET_CAS, lexer->get_next_token());

  input.assign("atomic_image");
  lexer->set_source_string(input);
  EXPECT_EQ(ATOMIC_IMAGE, lexer->get_next_token());

  input.assign("atomicNoRet_image");
  lexer->set_source_string(input);
  EXPECT_EQ(ATOMICNORET_IMAGE, lexer->get_next_token());

  input.assign("cvt");
  lexer->set_source_string(input);
  EXPECT_EQ(CVT, lexer->get_next_token());

  input.assign("_dep");
  lexer->set_source_string(input);
  EXPECT_EQ(_DEP, lexer->get_next_token());

  input.assign("_equiv");
  lexer->set_source_string(input);
  EXPECT_EQ(_EQUIV, lexer->get_next_token());

  input.assign("_acq");
  lexer->set_source_string(input);
  EXPECT_EQ(_ACQ, lexer->get_next_token());

  input.assign("_rel");
  lexer->set_source_string(input);
  EXPECT_EQ(_REL, lexer->get_next_token());

  input.assign("ld");
  lexer->set_source_string(input);
  EXPECT_EQ(LD, lexer->get_next_token());

  input.assign("sync");
  lexer->set_source_string(input);
  EXPECT_EQ(SYNC, lexer->get_next_token());

  input.assign("barrier");
  lexer->set_source_string(input);
  EXPECT_EQ(BARRIER, lexer->get_next_token());

  input.assign("segmentp");
  lexer->set_source_string(input);
  EXPECT_EQ(SEGMENTP, lexer->get_next_token());

  input.assign("ftos");
  lexer->set_source_string(input);
  EXPECT_EQ(FTOS, lexer->get_next_token());

  input.assign("stof");
  lexer->set_source_string(input);
  EXPECT_EQ(STOF, lexer->get_next_token());

  input.assign("mov");
  lexer->set_source_string(input);
  EXPECT_EQ(MOV, lexer->get_next_token());

  input.assign("lad");
  lexer->set_source_string(input);
  EXPECT_EQ(LAD, lexer->get_next_token());

  input.assign("ldc");
  lexer->set_source_string(input);
  EXPECT_EQ(LDC, lexer->get_next_token());

  input.assign("ret");
  lexer->set_source_string(input);
  EXPECT_EQ(RET, lexer->get_next_token());

  input.assign("packedcmp");
  lexer->set_source_string(input);
  EXPECT_EQ(PACKEDCMP, lexer->get_next_token());

  input.assign("cmp");
  lexer->set_source_string(input);
  EXPECT_EQ(CMP, lexer->get_next_token());

  input.assign("st");
  lexer->set_source_string(input);
  EXPECT_EQ(ST, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug52_Mul) {              // keywords for mul
  std::string input("mul");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(MUL, lexer->get_next_token());

  input.assign("mul_hi");
  lexer->set_source_string(input);
  EXPECT_EQ(MUL_HI, lexer->get_next_token());

  input.assign("mul24_hi");
  lexer->set_source_string(input);
  EXPECT_EQ(MUL24_HI, lexer->get_next_token());

  input.assign("mul24");
  lexer->set_source_string(input);
  EXPECT_EQ(MUL24, lexer->get_next_token());

  input.assign("mad24");
  lexer->set_source_string(input);
  EXPECT_EQ(MAD24, lexer->get_next_token());

  input.assign("mad24_hi");
  lexer->set_source_string(input);
  EXPECT_EQ(MAD24_HI, lexer->get_next_token());

  input.assign("f2u4");
  lexer->set_source_string(input);
  EXPECT_EQ(F2U4, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug53_DataTypes) {          // dataTypeId
  std::string input("_u8");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(_U8, lexer->get_next_token());

  input.assign("_u16");
  lexer->set_source_string(input);
  EXPECT_EQ(_U16, lexer->get_next_token());

  input.assign("_u32");
  lexer->set_source_string(input);
  EXPECT_EQ(_U32, lexer->get_next_token());

  input.assign("_u64");
  lexer->set_source_string(input);
  EXPECT_EQ(_U64, lexer->get_next_token());

  input.assign("_s8");
  lexer->set_source_string(input);
  EXPECT_EQ(_S8, lexer->get_next_token());

  input.assign("_s16");
  lexer->set_source_string(input);
  EXPECT_EQ(_S16, lexer->get_next_token());

  input.assign("_s32");
  lexer->set_source_string(input);
  EXPECT_EQ(_S32, lexer->get_next_token());

  input.assign("_s64");
  lexer->set_source_string(input);
  EXPECT_EQ(_S64, lexer->get_next_token());

  input.assign("_f16");
  lexer->set_source_string(input);
  EXPECT_EQ(_F16, lexer->get_next_token());

  input.assign("_f32");
  lexer->set_source_string(input);
  EXPECT_EQ(_F32, lexer->get_next_token());

  input.assign("_f64");
  lexer->set_source_string(input);
  EXPECT_EQ(_F64, lexer->get_next_token());

  input.assign("_b1");
  lexer->set_source_string(input);
  EXPECT_EQ(_B1, lexer->get_next_token());

  input.assign("_b8");
  lexer->set_source_string(input);
  EXPECT_EQ(_B8, lexer->get_next_token());

  input.assign("_b16");
  lexer->set_source_string(input);
  EXPECT_EQ(_B16, lexer->get_next_token());

  input.assign("_b32");
  lexer->set_source_string(input);
  EXPECT_EQ(_B32, lexer->get_next_token());

  input.assign("_b64");
  lexer->set_source_string(input);
  EXPECT_EQ(_B64, lexer->get_next_token());

  input.assign("_b128");
  lexer->set_source_string(input);
  EXPECT_EQ(_B128, lexer->get_next_token());

  input.assign("_b32");
  lexer->set_source_string(input);
  EXPECT_EQ(_B32, lexer->get_next_token());

  input.assign("_b8");
  lexer->set_source_string(input);
  EXPECT_EQ(_B8, lexer->get_next_token());

  input.assign("_b16");
  lexer->set_source_string(input);
  EXPECT_EQ(_B16, lexer->get_next_token());

  input.assign("_b32");
  lexer->set_source_string(input);
  EXPECT_EQ(_B32, lexer->get_next_token());

  input.assign("_u8x4");
  lexer->set_source_string(input);
  EXPECT_EQ(_U8X4, lexer->get_next_token());

  input.assign("_s8x4");
  lexer->set_source_string(input);
  EXPECT_EQ(_S8X4, lexer->get_next_token());

  input.assign("_u16x2");
  lexer->set_source_string(input);
  EXPECT_EQ(_U16X2, lexer->get_next_token());

  input.assign("_s16x2");
  lexer->set_source_string(input);
  EXPECT_EQ(_S16X2, lexer->get_next_token());

  input.assign("_f16x2");
  lexer->set_source_string(input);
  EXPECT_EQ(_F16X2, lexer->get_next_token());

  input.assign("_f32x2");
  lexer->set_source_string(input);
  EXPECT_EQ(_F32X2, lexer->get_next_token());

  input.assign("_u8x8");
  lexer->set_source_string(input);
  EXPECT_EQ(_U8X8, lexer->get_next_token());

  input.assign("_s8x8");
  lexer->set_source_string(input);
  EXPECT_EQ(_S8X8, lexer->get_next_token());

  input.assign("_u16x4");
  lexer->set_source_string(input);
  EXPECT_EQ(_U16X4, lexer->get_next_token());

  input.assign("_s16x4");
  lexer->set_source_string(input);
  EXPECT_EQ(_S16X4, lexer->get_next_token());

  input.assign("_f16x4");
  lexer->set_source_string(input);
  EXPECT_EQ(_F16X4, lexer->get_next_token());

  input.assign("_u8x16");
  lexer->set_source_string(input);
  EXPECT_EQ(_U8X16, lexer->get_next_token());

  input.assign("_s8x16");
  lexer->set_source_string(input);
  EXPECT_EQ(_S8X16, lexer->get_next_token());

  input.assign("_u16x8");
  lexer->set_source_string(input);
  EXPECT_EQ(_U16X8, lexer->get_next_token());

  input.assign("_s16x8");
  lexer->set_source_string(input);
  EXPECT_EQ(_S16X8, lexer->get_next_token());

  input.assign("_f16x8");
  lexer->set_source_string(input);
  EXPECT_EQ(_F16X8, lexer->get_next_token());

  input.assign("_f32x4");
  lexer->set_source_string(input);
  EXPECT_EQ(_F32X4, lexer->get_next_token());

  input.assign("_s32x4");
  lexer->set_source_string(input);
  EXPECT_EQ(_S32X4, lexer->get_next_token());

  input.assign("_u32x4");
  lexer->set_source_string(input);
  EXPECT_EQ(_U32X4, lexer->get_next_token());

  input.assign("_f64x2");
  lexer->set_source_string(input);
  EXPECT_EQ(_F64X2, lexer->get_next_token());

  input.assign("_s64x2");
  lexer->set_source_string(input);
  EXPECT_EQ(_S64X2, lexer->get_next_token());

  input.assign("_u64x2");
  lexer->set_source_string(input);
  EXPECT_EQ(_U64X2, lexer->get_next_token());

  delete lexer;
}

TEST(LexTest, Bug54_SomeKeywords) {
  std::string input("_ftz");
  Lexer* lexer = new Lexer(input);

  EXPECT_EQ(_FTZ, lexer->get_next_token());

  input.assign("nop");
  lexer->set_source_string(input);
  EXPECT_EQ(NOP, lexer->get_next_token());

  input.assign("clock");
  lexer->set_source_string(input);
  EXPECT_EQ(CLOCK, lexer->get_next_token());

  input.assign("syscall");
  lexer->set_source_string(input);
  EXPECT_EQ(SYSCALL, lexer->get_next_token());

  delete lexer;
}

// ------------------ LEXER WRAPPER TESTS -----------------
TEST(LexerWrapperTest, TestLexWrapper) {
  Lexer* lexer = new Lexer();
  std::string input("$c1");
  lexer->set_source_string(input);

  int token = lexer->get_next_token();

  EXPECT_GE(token, 0);
  EXPECT_EQ(TOKEN_CREGISTER, token);

  EXPECT_STREQ("$c1", context->token_value.string_val);

  delete lexer;
}

TEST(LexerWrapperTest, GetIntValFromLexer) {
  std::string input("10");
  Lexer* lexer = new Lexer(input);


  int token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_INTEGER_CONSTANT);
  EXPECT_EQ(10, context->token_value.int_val);

  delete lexer;
}

TEST(LexerWrapperTest, GetFloatValFromLexer) {
  std::string input("10.0f");
  Lexer* lexer = new Lexer(input);


  int token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_SINGLE_CONSTANT);
  EXPECT_EQ(10.0, context->token_value.float_val);

  delete lexer;
}

TEST(LexerWrapperTest, GetDoubleValFromLexer) {
  std::string input("10.0l");
  Lexer* lexer = new Lexer(input);


  int token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_DOUBLE_CONSTANT);
  EXPECT_EQ(10.0, context->token_value.double_val);

  delete lexer;
}

TEST(LexerWrapperTest, ResetLexer) {
  std::string input("$c1 10 10.5f");
  Lexer* lexer = new Lexer(input);

  int token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_CREGISTER);

  token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_INTEGER_CONSTANT);
  EXPECT_EQ(10, context->token_value.int_val);

  token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_SINGLE_CONSTANT);
  EXPECT_EQ(10.5, context->token_value.float_val);

  // restart from begin
  lexer->restart();
  token = lexer->get_next_token();
  EXPECT_EQ(token, TOKEN_CREGISTER);

  delete lexer;
}

}  // namespace brig
}  // namespace hsa
