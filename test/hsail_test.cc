#include "gtest/gtest.h"
#include "lexer.h"
#include <string>
#include "../tokens.h"

extern int int_val;

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
  std::string input("$s15");
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
  EXPECT_EQ(5,yylex());
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
 
 TEST(LexTest, Bug27) {				// keywords _v2 and _v4
  std::string input("_v2");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_V2,yylex());
  
  input.assign("_v4");
  yy_scan_string((char*)input.c_str());
  EXPECT_EQ(_V4,yylex());
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