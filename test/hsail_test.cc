#include "gtest/gtest.h"
#include "lexer.h"
#include <string>
#include "../tokens.h"
#include "../parser.h"
extern int int_val;
extern float float_val;
extern double double_val;

int parser_main(std::string input);


// ------------------ LEXER TESTS -----------------
TEST(LexTest, Bug2) {
  std::string input("12345");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_INTEGER_CONSTANT,yylex());
  EXPECT_EQ(12345,int_val);
}

TEST(LexTest, Bug3) {
  std::string input("020");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_INTEGER_CONSTANT,yylex());
  EXPECT_EQ(16,int_val);
}

TEST(LexTest, Bug4) {
  std::string input("$c7");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_CREGISTER,yylex());
}

TEST(LexTest, Bug5) {
  std::string input("$d7");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_DREGISTER,yylex());
}

TEST(LexTest, Bug6) {
  std::string input("        $s15");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_SREGISTER,yylex());
}

TEST(LexTest, Bug7) {
  std::string input("$q5");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_QREGISTER,yylex());
}

TEST(LexTest, Bug8) {
  std::string input("@Go_to_this");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_LABEL,yylex());
}

TEST(LexTest, Bug9) {
  std::string input("/* this is a comment */");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_COMMENT,yylex());
}

TEST(LexTest, Bug10) {
  std::string input("//this is an inline comment\n");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_COMMENT,yylex());
}

TEST(LexTest, Bug11) {
  std::string input("0x11");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_INTEGER_CONSTANT,yylex());
  EXPECT_EQ(17,int_val);
}

TEST(LexTest, Bug12) {
  std::string input("0.5e3f");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_SINGLE_CONSTANT,yylex());
  EXPECT_EQ(0.5e3f, float_val);
}

TEST(LexTest, Bug17) {
  std::string input("%Test_id_123");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_LOCAL_IDENTIFIER,yylex());
}

TEST(LexTest, Bug18) {
  std::string input("&Test_global_id_123");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_GLOBAL_IDENTIFIER,yylex());
}

TEST(LexTest, Bug19) {
  std::string input("\" This is a string\"");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_STRING,yylex());
}

TEST(LexTest, Bug20) {
  std::string input("snorm_int8");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_PROPERTY,yylex());
  
  input.assign("unorm_int16");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_PROPERTY,yylex());
  
  input.assign("rx");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_PROPERTY,yylex());
  
  input.assign("intensity");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_PROPERTY,yylex());
  
  input.assign("wrap");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_PROPERTY,yylex());
 
}

TEST(LexTest, Bug21) {
  std::string input("WAVESIZE");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_WAVESIZE,yylex());
}

TEST(LexTest, Bug22) {			// common keywords


  std::string input("workgroupid");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(WORKGROUPID,yylex());
  
  input.assign("version");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(VERSION,yylex());
  
  input.assign("global");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(GLOBAL,yylex());
  
  input.assign("_Samp");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SAMP,yylex());
  
  input.assign("_RWImg");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_RWIMG,yylex());
  
  input.assign("_ROImg");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_ROIMG,yylex());
  
  input.assign("align");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ALIGN,yylex());
  
  input.assign("file");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_FILE,yylex());
  
  input.assign("arg");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ARG,yylex());
  
    input.assign("kernarg");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(KERNARG,yylex());
  
  input.assign("function");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FUNCTION,yylex());
  
  input.assign(":fbar");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_FBAR,yylex());
  
  input.assign("signature");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(SIGNATURE,yylex());
  
    input.assign("block");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(BLOCK,yylex());
  
  input.assign("endblock");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ENDBLOCK,yylex());
  
  input.assign("blocknumeric");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(BLOCKNUMERIC,yylex());
  
    input.assign("blockstring");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(BLOCKSTRING,yylex());
  
  input.assign("kernel");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(KERNEL,yylex());
  
  input.assign("pragma");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(PRAGMA,yylex());
  
  input.assign("labeltargets");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(LABELTARGETS,yylex());
  
    input.assign("extension");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(EXTENSION,yylex());
  
  input.assign("extern");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(EXTERN,yylex());
  
  input.assign("static");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(STATIC,yylex());
  
    input.assign("const");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(CONST,yylex());
  
  input.assign("private");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(PRIVATE,yylex());
  
  input.assign("spill");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(SPILL,yylex());
  
  input.assign("group");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(GROUP,yylex());
 
   input.assign("readonly");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(READONLY,yylex());
  
  input.assign("loc");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(LOC,yylex());
}

TEST(LexTest, Bug23) {
  std::string input("0x1.0p0f");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_SINGLE_CONSTANT,yylex());
  EXPECT_EQ(1.0f, float_val);
}

TEST(LexTest, Bug24) {
  std::string input("0f3F800000");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_SINGLE_CONSTANT,yylex());
  EXPECT_EQ(1.0f, float_val);
}

TEST(LexTest, Bug25) {			// addressSpaceIdentifier keywords
  std::string input("_readonly");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_READONLY,yylex());
  
  input.assign("_kernarg");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_KERNARG,yylex());
  
  input.assign("_global");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_GLOBAL,yylex());
  
  input.assign("_private");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_PRIVATE,yylex());
  
  input.assign("_arg");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_ARG,yylex());
  
  input.assign("_group");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_GROUP,yylex());
  
  input.assign("_spill");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SPILL,yylex());
  
 }
 
TEST(LexTest, Bug26) {
  std::string input("0.5e3l");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_DOUBLE_CONSTANT,yylex());
  EXPECT_EQ(0.5e3, double_val);
}

 TEST(LexTest, Bug27) {				// keywords _v2 and _v4
  std::string input("_v2");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_V2,yylex());
  
  input.assign("_v4");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_V4,yylex());
}

TEST(LexTest, Bug28) {
  std::string input("0x1.0l");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_DOUBLE_CONSTANT,yylex());
  EXPECT_EQ(1.0, double_val);
}

TEST(LexTest, Bug29) {
  std::string input("0d3FF0000000000000");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(TOKEN_DOUBLE_CONSTANT,yylex());
  EXPECT_EQ(1.0, double_val);
}

 TEST(LexTest, Bug30) {				// keywords format, order, coord, filter, boundaryU, boundaryV, boundaryW
  std::string input("format");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FORMAT,yylex());
  
  input.assign("order");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ORDER,yylex());
  
  input.assign("coord");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(COORD,yylex());
  
  input.assign("filter");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FILTER,yylex());
  
  input.assign("boundaryU");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(BOUNDARYU,yylex());
  
  input.assign("boundaryV");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(BOUNDARYV,yylex());
  
  input.assign("boundaryW");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(BOUNDARYW,yylex());
}

 TEST(LexTest, Bug33) {				//control keywords
  std::string input("itemsperworkgroup");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ITEMS_PER_WORKGROUP,yylex());
  
  input.assign("workgroupspercu");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(WORKGROUPS_PER_CU,yylex());
  
  input.assign("memopt_on");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(MEMOPT_ON,yylex());
  
  input.assign("memopt_off");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(MEMOPT_OFF,yylex());
 }
 
  TEST(LexTest, Bug34) {				//punctuations
  std::string input("+");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ('+',yylex());
  
  input.assign("-");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ('-',yylex());
  
  input.assign("=");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ('=',yylex());
  
  input.assign("<");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ('<',yylex());
  
    
  input.assign(">");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ('>',yylex());
  
  input.assign("[");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ('[',yylex());
  
  input.assign("]");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(']',yylex());
  
  input.assign("{");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ('{',yylex());
  
    
  input.assign("}");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ('}',yylex());
  
  input.assign("(");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ('(',yylex());
  
  input.assign(")");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(')',yylex());
  
    
  input.assign(":");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(':',yylex());
  
  input.assign(";");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(';',yylex());
  
  input.assign(",");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(',',yylex());
 }
 
  TEST(LexTest, Bug37) {				// opcodes 
  
  
  std::string input("fbar_initSizeWg");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FBAR_INITSIZEWG,yylex());
  
  input.assign("fbar_wait");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FBAR_WAIT,yylex());
  
  input.assign("fbar_arrive");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FBAR_ARRIVE,yylex());
  
  input.assign("fbar_skip");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FBAR_SKIP,yylex());
  
    
  input.assign("fbar_release");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FBAR_RELEASE,yylex());
  
  input.assign("countup");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(COUNTUP,yylex());
  
  input.assign("laneid");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(LANEID,yylex());
  
  input.assign("dynwaveid");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(DYNWAVEID,yylex());
  
    
  input.assign("maxdynwaveid");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(MAXDYNWAVEID,yylex());
  
  input.assign("dispatchid");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(DISPATCHID,yylex());
  
  input.assign("cu");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(CU,yylex());
  
    
  input.assign("workdim");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(WORKDIM,yylex());
  
  input.assign("workitemid_flat");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(WORKITEMID_FLAT,yylex());
  
  input.assign("workitemaid_flat");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(WORKITEMAID_FLAT,yylex());
  
    
  input.assign("debugtrap");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(DEBUGTRAP,yylex());
 }
 
 TEST(LexTest, Bug38) {			
  std::string input("abs");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ABS,yylex());
  
  input.assign("neg");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(NEG,yylex());
  
  input.assign("not");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(NOT,yylex());
  
  input.assign("popcount");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(POPCOUNT,yylex());
  
    
  input.assign("firstbit");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FIRSTBIT,yylex());
  
  input.assign("lastbit");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(LASTBIT,yylex());
  
  input.assign("bitrev");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(BITREV,yylex());
  
  input.assign("movs_lo");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(MOVS_LO,yylex());
  
  input.assign("movs_hi");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(MOVS_HI,yylex());
  
  input.assign("fbar_initSize");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FBAR_INITSIZE,yylex());
  
  input.assign("fbar_releaseCF");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FBAR_RELEASECF,yylex());
  
  input.assign("count");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(COUNT,yylex());
  
  input.assign("mask");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(MASK,yylex());  
  
  }
   
TEST(LexTest, Bug39) {			
  std::string input("sqrt");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(SQRT,yylex());
  
  input.assign("fract");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FRACT,yylex());
  
  input.assign("fcos");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FCOS,yylex());
  
  input.assign("fsin");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FSIN,yylex());
  
    
  input.assign("flog2");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FLOG2,yylex());
  
  input.assign("fexp2");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FEXP2,yylex());
  
  input.assign("fsqrt");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FSQRT,yylex());
  
  input.assign("frsqrt");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FRSQRT,yylex());
  
  input.assign("frcp");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FRCP,yylex());
  
  }
      
TEST(LexTest, Bug40) {			
  std::string input("unpack3");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(UNPACK3,yylex());
  
  input.assign("unpack2");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(UNPACK2,yylex());
  
  input.assign("unpack1");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(UNPACK1,yylex());
  
  input.assign("unpack0");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(UNPACK0,yylex());
  
    
  input.assign("alloca");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ALLOCA,yylex());
  
  input.assign("workitemid");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(WORKITEMID,yylex());
  
  input.assign("workitemaid");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(WORKITEMAID,yylex());
  
  input.assign("workgroupsize");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(WORKGROUPSIZE,yylex());
  
  input.assign("NDRangesize");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(NDRANGESIZE,yylex());
  
  input.assign("NDRangegroups");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(NDRANGEGROUPS,yylex());
  }
        
TEST(LexTest, Bug41) {			
  std::string input("add");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ADD,yylex());
  
  input.assign("carry");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(CARRY,yylex());
  
  input.assign("borrow");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(BORROW,yylex());
  
  input.assign("div");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(DIV,yylex());
  
    
  input.assign("rem");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(REM,yylex());
  
  input.assign("sub");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(SUB,yylex());
  
  input.assign("shl");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(SHL,yylex());
  
  input.assign("shr");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(SHR,yylex());
  
  input.assign("and");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(AND,yylex());
  
  input.assign("xor");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(XOR,yylex());
 
  input.assign("or");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(OR,yylex());
  
  input.assign("unpacklo");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(UNPACKLO,yylex());
  
  input.assign("unpackhi");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(UNPACKHI,yylex());
  
  input.assign("movd_lo");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(MOVD_LO,yylex());
  
  input.assign("movd_hi");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(MOVD_HI,yylex());
  
  input.assign("copysign");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(COPYSIGN,yylex());
  
  input.assign("class");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(CLASS,yylex());
  
  input.assign("send");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(SEND,yylex());
  
  input.assign("receive");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(RECEIVE,yylex());
 
  }
         
TEST(LexTest, Bug42) {			
  std::string input("max");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(MAX,yylex());
  
  input.assign("min");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(MIN,yylex());
  
  input.assign("mad");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(MAD,yylex());
  
  input.assign("extract");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(EXTRACT,yylex());
  
    
  input.assign("insert");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(INSERT,yylex());
  
  input.assign("shuffle");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(SHUFFLE,yylex());
  
  input.assign("cmov");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(CMOV,yylex());
  
  input.assign("fma");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FMA,yylex());
  
  input.assign("bitalign");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(BITALIGN,yylex());
  
  input.assign("bytealign");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(BYTEALIGN,yylex());
 
  input.assign("lerp");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(LERP,yylex());
  
  input.assign("sad");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(SAD,yylex());
  
  input.assign("sad2");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(SAD2,yylex());
  
  input.assign("sad4");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(SAD4,yylex());
  
  input.assign("sad4hi");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(SAD4HI,yylex());
  
  input.assign("bitselect");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(BITSELECT,yylex());
  
  }
         
TEST(LexTest, Bug43) {				// AtomicOperationId
  std::string input("_and");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_AND_,yylex());
  
  input.assign("_or");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_OR_,yylex());
  
  input.assign("_xor");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_XOR_,yylex());
  
  input.assign("_exch");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_EXCH_,yylex());
  
    
  input.assign("_add");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_ADD_,yylex());
  
  input.assign("_sub");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SUB_,yylex());
  
  input.assign("_inc");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_INC_,yylex());
  
  input.assign("_dec");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_DEC_,yylex());
  
  input.assign("_max");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_MAX_,yylex());
  
  input.assign("_min");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_MIN_,yylex());
}
        
TEST(LexTest, Bug44) {				// Comparison
  std::string input("_eq");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_EQ,yylex());
  
  input.assign("_ne");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_NE,yylex());
  
  input.assign("_lt");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_LT,yylex());
  
  input.assign("_le");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_LE,yylex());
  
    
  input.assign("_gt");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_GT,yylex());
  
  input.assign("_ge");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_GE,yylex());
  
  input.assign("_equ");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_EQU,yylex());
  
  input.assign("_neu");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_NEU,yylex());
  
  input.assign("_ltu");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_LTU,yylex());
  
  input.assign("_leu");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_LEU,yylex());
 
   input.assign("_gtu");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_GTU,yylex());
  
  input.assign("_geu");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_GEU,yylex());
  
  input.assign("_num");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_NUM,yylex());
  
    
  input.assign("_nan");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_NAN,yylex());
  
  input.assign("_seq");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SEQ,yylex());
  
  input.assign("_sne");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SNE,yylex());
  
  input.assign("_slt");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SLT,yylex());
  
  input.assign("_sle");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SLE,yylex());
  
  input.assign("_sgt");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SGT,yylex());
  
    input.assign("_sge");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SGE,yylex());
  
  input.assign("_snum");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SNUM,yylex());
  
  input.assign("_snan");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SNAN,yylex());
  
  input.assign("_sequ");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SEQU,yylex());
  
  input.assign("_sneu");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SNEU,yylex());
  
  input.assign("_sltu");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SLTU,yylex());
  
    input.assign("_sleu");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SLEU,yylex());
  
  input.assign("_sgtu");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SGTU,yylex());
  
  input.assign("_sgeu");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SGEU,yylex());

  
}

 TEST(LexTest, Bug45) {				// keywords for queryOp
  std::string input("query_order");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(QUERY_ORDER,yylex());
  
  input.assign("query_data");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(QUERY_DATA,yylex());
  
  input.assign("query_array");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(QUERY_ARRAY,yylex());
  
  input.assign("query_width");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(QUERY_WIDTH,yylex());
  
  input.assign("query_depth");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(QUERY_DEPTH,yylex());
  
  input.assign("query_height");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(QUERY_HEIGHT,yylex());
  
  input.assign("query_normalized");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(QUERY_NORMALIZED,yylex());

  input.assign("query_filtering");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(QUERY_FILTERING,yylex());

}
        
TEST(LexTest, Bug46) {				// Rounding modes
  std::string input("_upi");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_UPI,yylex());
  
  input.assign("_downi");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_DOWNI,yylex());
  
  input.assign("_zeroi");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_ZEROI,yylex());
  
  input.assign("_neari");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_NEARI,yylex());
  
    
  input.assign("_up");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_UP,yylex());
  
  input.assign("_down");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_DOWN,yylex());
  
  input.assign("_zero");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_ZERO,yylex());
  
  input.assign("_near");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_NEAR,yylex());
  
}
   
TEST(LexTest, Bug47) {				// packing modes
  std::string input("_pp");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_PP,yylex());
  
  input.assign("_ps");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_PS,yylex());
  
  input.assign("_sp");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SP,yylex());
  
  input.assign("_ss");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SS,yylex());
  
  input.assign("_s");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(__S,yylex());
  
  input.assign("_p");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(__P,yylex());
    
  input.assign("_pp_sat");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_PP_SAT,yylex());
  
  input.assign("_ps_sat");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_PS_SAT,yylex());
  
  input.assign("_sp_sat");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SP_SAT,yylex());
  
  input.assign("_ss_sat");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SS_SAT,yylex());
  
  input.assign("_p_sat");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_P_SAT,yylex());
  
  input.assign("_s_sat");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_S_SAT,yylex());
  
}      

 TEST(LexTest, Bug48) {				// keywords for geometry ID
  std::string input("_1d");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_1D,yylex());
  
  input.assign("_2d");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_2D,yylex());
  
  input.assign("_3d");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_3D,yylex());
  
  input.assign("_1db");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_1DB,yylex());
  
  input.assign("_1da");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_1DA,yylex());
  
  input.assign("_2da");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_2DA,yylex());
  
  input.assign("rd_image");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(RD_IMAGE,yylex());

  input.assign("ld_image");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(LD_IMAGE,yylex());

  input.assign("st_image");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ST_IMAGE,yylex());

}
		   
TEST(LexTest, Bug49) {				// targets
  std::string input("$small");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SMALL,yylex());
  
  input.assign("$large");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_LARGE,yylex());
  
  input.assign("$full");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_FULL,yylex());
  
  input.assign("$reduced");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_REDUCED,yylex());
  
  input.assign("$sftz");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_SFTZ,yylex());
  
  input.assign("$nosftz");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_NOSFTZ,yylex());
   
}  
   
TEST(LexTest, Bug50) {				
  std::string input("width");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(WIDTH,yylex());
  
  input.assign("height");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(HEIGHT,yylex());
  
  input.assign("depth");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(DEPTH,yylex());
}

 TEST(LexTest, Bug51) {				// keywords for Atom Modifiers
  std::string input("_ar");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_AR,yylex());
  
  input.assign("_region");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_REGION,yylex());
  
  input.assign("atomic_cas");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ATOMIC_CAS,yylex());
  
  input.assign("atomic");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ATOMIC,yylex());
  
  input.assign("atomicNoRet");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ATOMICNORET,yylex());
  
  input.assign("atomicNoRet_cas");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ATOMICNORET_CAS,yylex());
  
  input.assign("atomic_image");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ATOMIC_IMAGE,yylex());

  input.assign("atomicNoRet_image");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ATOMICNORET_IMAGE,yylex());

  input.assign("cvt");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(CVT,yylex());

  input.assign("_dep");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_DEP,yylex());
  
  input.assign("_equiv");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_EQUIV,yylex());
  
  input.assign("_acq");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_ACQ,yylex());
  
  input.assign("_rel");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_REL,yylex());
  
  input.assign("ld");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(LD,yylex());
  
  input.assign("sync");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(SYNC,yylex());

  input.assign("barrier");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(BARRIER,yylex());

  input.assign("segmentp");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(SEGMENTP,yylex());

  input.assign("ftos");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(FTOS,yylex());
  
  input.assign("stof");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(STOF,yylex());
  
  input.assign("mov");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(MOV,yylex());
  
  input.assign("lad");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(LAD,yylex());
  
  input.assign("ldc");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(LDC,yylex());
  
  input.assign("ret");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(RET,yylex());

  input.assign("packedcmp");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(PACKEDCMP,yylex());

  input.assign("cmp");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(CMP,yylex());
  
  input.assign("st");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(ST,yylex());



}

 TEST(LexTest, Bug52) {				// keywords for mul
  std::string input("mul");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(MUL,yylex());
  
  input.assign("mul_hi");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(MUL_HI,yylex());
  
  input.assign("mul24_hi");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(MUL24_HI,yylex());
  
  input.assign("mul24");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(MUL24,yylex());
  
  input.assign("mad24");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(MAD24,yylex());
  
  input.assign("mad24_hi");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(MAD24_HI,yylex());
  
  input.assign("f2u4");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(F2U4,yylex());
}
       
TEST(LexTest, Bug53) {			// dataTypeId		
  std::string input("_u8");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_U8,yylex());
  
  input.assign("_u16");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_U16,yylex());
  
  input.assign("_u32");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_U32,yylex());
  
  input.assign("_u64");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_U64,yylex());

  input.assign("_s8");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_S8,yylex());

  input.assign("_s16");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_S16,yylex());

  input.assign("_s32");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_S32,yylex());

  input.assign("_s64");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_S64,yylex());

  input.assign("_f16");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_F16,yylex());

  input.assign("_f32");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_F32,yylex());

  input.assign("_f64");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_F64,yylex());

  input.assign("_b1");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_B1,yylex());

  input.assign("_b8");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_B8,yylex());

  input.assign("_b16");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_B16,yylex());

  input.assign("_b32");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_B32,yylex());  
  
  input.assign("_b64");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_B64,yylex());

  input.assign("_b128");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_B128,yylex());

  input.assign("_b32");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_B32,yylex()); 
  
  input.assign("_b8");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_B8,yylex());

  input.assign("_b16");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_B16,yylex());

  input.assign("_b32");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_B32,yylex()); 
  

  input.assign("_u8x4");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_U8X4,yylex());

  input.assign("_s8x4");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_S8X4,yylex());

  input.assign("_u16x2");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_U16X2,yylex());  
  
  input.assign("_s16x2");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_S16X2,yylex());

  input.assign("_f16x2");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_F16X2,yylex());

  input.assign("_f32x2");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_F32X2,yylex()); 
  
  input.assign("_u8x8");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_U8X8,yylex());

  input.assign("_s8x8");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_S8X8,yylex());

  input.assign("_u16x4");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_U16X4,yylex());   
  
    input.assign("_s16x4");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_S16X4,yylex());

  input.assign("_f16x4");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_F16X4,yylex());  
  
  input.assign("_u8x16");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_U8X16,yylex());

  input.assign("_s8x16");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_S8X16,yylex());

  input.assign("_u16x8");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_U16X8,yylex()); 
  
  input.assign("_s16x8");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_S16X8,yylex());

  input.assign("_f16x8");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_F16X8,yylex());

  input.assign("_f32x4");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_F32X4,yylex());  
  
  input.assign("_s32x4");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_S32X4,yylex());  

  input.assign("_u32x4");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_U32X4,yylex());  

  input.assign("_f64x2");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_F64X2,yylex());    
  
  input.assign("_s64x2");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_S64X2,yylex()); 

  input.assign("_u64x2");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_U64X2,yylex());   
}  
          
TEST(LexTest, Bug54) {				
  std::string input("_ftz");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_FTZ,yylex());
  
  input.assign("nop");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(NOP,yylex());
  
  input.assign("clock");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(CLOCK,yylex());
  
    
  input.assign("syscall");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(SYSCALL,yylex());
}


// ------------------ PARSER TESTS -----------------
TEST(ParserTest, Bug1) {
  std::string input("query_width_u32 $s1. [%RWImg3]");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(1,parser_main(input));
}


