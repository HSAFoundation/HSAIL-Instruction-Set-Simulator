%option noyywrap
%{
#include <stdio.h>
#include "tokens.h"

int int_val;
float float_val;

/* helper function for IEEE to Single */
float ieee754tof(char* text) {
	float floatval;
	unsigned int mem_single=0; 
	unsigned int first; 
	char first2[3]; 
	
	first2[0] = text[2]; 
	first2[1] = text[3]; 
	first2[2] = '\0';
	first = strtol(first2, NULL, 16); 
	mem_single = strtol(text+4, NULL, 16); 
	first = first << 24; 
	mem_single += first; 
	memcpy(&floatval, &mem_single, sizeof(float)); 
	
	return floatval;
}

%}
OCT_INT		([+|-]?0[0-7]*)
DEC_INT		([+|-]?(0|([1-9]*[0-9]+)))
HEX_INT		([+|-]?0(x|X)[0-9a-fA-F]*)	

DEC_DOUBLE 	([+|-]?(([0-9]*("."[0-9]*)?((e|E)[+|-]?[0-9]*)?)|([0-9]+(e|E)[+|-]?[0-9]*))(l|L)?)  
HEX_DOUBLE	(0(x|X)[0-9a-fA-F]+(("."[0-9a-fA-F]*)((p|P)[+|-]?[0-9]*)?)(l|L)?)
IEEE_DOUBLE	(0(d|D)[0-9a-fA-F]{16}) 
	
DEC_SINGLE	([+|-]?(([0-9]*("."[0-9]*)?((e|E)[+|-]?[0-9]*)?)|([0-9]+(e|E)[+|-]?[0-9]*))(f|F))
HEX_SINGLE	(0(x|X)[0-9a-fA-F]+(("."[0-9a-fA-F]*)((p|P)[+|-]?[0-9]*)?)(f|F))
IEEE_SINGLE	(0(f|F)[0-9a-fA-F]{8})
%%
[a-z]+ { return 1; }
{OCT_INT} { int_val = strtol(yytext, NULL, 8); return TOKEN_INTEGER_CONSTANT; } 
{DEC_INT} { int_val = strtol(yytext, NULL, 10); return TOKEN_INTEGER_CONSTANT; }
{HEX_INT} { int_val = strtol(yytext, NULL, 16); return TOKEN_INTEGER_CONSTANT; }

{DEC_SINGLE} { float_val = strtof(yytext,NULL); return TOKEN_SINGLE_CONSTANT; }
{HEX_SINGLE} { float_val = strtof(yytext,NULL); return TOKEN_SINGLE_CONSTANT; }
{IEEE_SINGLE} { float_val = ieee754tof(yytext); return TOKEN_SINGLE_CONSTANT; }

{DEC_DOUBLE} { return 8; }
{HEX_DOUBLE} { return 9; }
{IEEE_DOUBLE} { return 10; }

\$c(0|1|2|3|4|5|6|7) 	{ return TOKEN_CREGISTER;}
\$d(0|1|2|3|4|5|6|7) 	{ return TOKEN_DREGISTER;}
\$s(0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15) { return TOKEN_SREGISTER;}
.	{ fprintf(stderr, "unexpected char"); return 0;}
