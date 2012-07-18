#include "gtest/gtest.h"
#include "lexer.h"
#include <string>
#include "../parser.h"


// ------------------ PARSER TESTS -----------------
TEST(ParserTest, Identifier_Test) {

	// Identifier
	std::string input("&a_global_id123");   // global id
	yy_scan_string((char*)input.c_str());
	EXPECT_EQ(0, identifier(yylex()));

	input.assign("%a_local_id");  	// local id
	yy_scan_string((char*)input.c_str());
	EXPECT_EQ(0, identifier(yylex()));
	
	input.assign("$d7"); // register
	yy_scan_string((char*)input.c_str());
	EXPECT_EQ(0, identifier(yylex()));
}

TEST(ParserTest, BaseOperand_Test) {	
	std::string input("1352"); // Int constant
	yy_scan_string((char*)input.c_str());
	EXPECT_EQ(0, baseOperand(yylex()));
	
	input.assign("WAVESIZE");  // TOKEN_WAVE_SIZE
	yy_scan_string((char*)input.c_str());
	EXPECT_EQ(0, baseOperand(yylex()));
	
	// Need more test for decimalListSingle and floatListSingle
	
	input.assign("_u32(12, 13 ,14)");	// decimalListSingle
	yy_scan_string((char*)input.c_str());
	EXPECT_EQ(0, baseOperand(yylex()));
	
	
	
	
	
	
	
}

TEST(ParserTest, addressableOperand_Test) {
	std::string input("[%local_id]"); // Int constant
	yy_scan_string((char*)input.c_str());
	EXPECT_EQ(0, addressableOperand(yylex()));	

	input.assign("[%local_id <100> ]");
	yy_scan_string((char*)input.c_str());
	EXPECT_EQ(0, addressableOperand(yylex()));	
	
	input.assign("[%local_id<$d7>]");
	yy_scan_string((char*)input.c_str());
	EXPECT_EQ(0, addressableOperand(yylex()));	
	
	input.assign("[%global_id<$q5 + 10 >]");
	yy_scan_string((char*)input.c_str());
	EXPECT_EQ(0, addressableOperand(yylex()));	
	
	input.assign("[%global_id<$d6 - 10 >]");
	yy_scan_string((char*)input.c_str());
	EXPECT_EQ(0, addressableOperand(yylex()));	
	
}

TEST(ParserTest, Query_Test) {  
  std::string input("query_order_u32  $c1 , [&Test<$d7  + 100>]");
  EXPECT_EQ(0, parse(input));
  
  input.assign("query_order_u32 &a_global_id, [%a_local_id]");
  EXPECT_EQ(0, parse(input));

}

TEST(ParserTest, Bug_55) {
  std::string input("query_order_u32 _u32(1,2,3), [%a_local_id]");
  EXPECT_EQ(0, parse(input));
  
  

}
