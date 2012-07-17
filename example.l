%option noyywrap
%{
#include <stdio.h>
#include "tokens.h"

int int_val;
float float_val;
double double_val;

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


double ieee754tod(char* text) {
	double return_val = 0.0;	// return value
	unsigned int* int_ptr = 0;
	char first4[5];	// first 2 bytes(4 chars after 0d)
	char temp_str[5];	// temporary string to hold part of yytext
	unsigned int i;
	unsigned int first_int;		// convert the double as 2 integers
	unsigned int second_int;
	unsigned int temp = 0;
	
	// convert the first 2 bytes 	
	for (i=0;i<4;i++)
		first4[i] = text[i+2];
	
	first4[5] = '\0';
	first_int = strtol(first4, NULL, 16);		
	
	// get the next 2 bytes;
	for (i=0;i<4;i++) 
		temp_str[i] = text[i+6];
	temp_str[5] = '\0';
	temp = strtol(temp_str,NULL,16);
	first_int <<= 16;
	
	temp += first_int;
	
	int_ptr = (unsigned int*)&return_val;
	
	memcpy(int_ptr+1,&temp, sizeof(float));
	//printf("Return value after update first 4 bytes: %2.16f\n",return_val);
	

	//for the last 4bytes, do the same thing, to avoid treating the first bit as the SIGN.
	for (i=0;i<4;i++)
		first4[i] = text[i+10];
	
	first4[5] = '\0';
	second_int = strtol(first4, NULL, 16);		
	
	// get the last 2 bytes;
	for (i=0;i<4;i++) 
		temp_str[i] = text[i+14];
	temp_str[5] = '\0';
	temp = strtol(temp_str,NULL,16);
	second_int <<= 16;
	temp += second_int;

	memcpy(int_ptr,&temp, sizeof(int));
	
	//printf("Final return value: %2.14f\n",return_val);
	return return_val;
}

%}

%x INLINE_COMMENT
%x COMMENT

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

{OCT_INT} { int_val = strtol(yytext, NULL, 8); return TOKEN_INTEGER_CONSTANT; } 
{DEC_INT} { int_val = strtol(yytext, NULL, 10); return TOKEN_INTEGER_CONSTANT; }
{HEX_INT} { int_val = strtol(yytext, NULL, 16); return TOKEN_INTEGER_CONSTANT; }

{DEC_SINGLE} { float_val = strtof(yytext,NULL); return TOKEN_SINGLE_CONSTANT; }
{HEX_SINGLE} { float_val = strtof(yytext,NULL); return TOKEN_SINGLE_CONSTANT; }
{IEEE_SINGLE} { float_val = ieee754tof(yytext); return TOKEN_SINGLE_CONSTANT; }

{DEC_DOUBLE} { double_val = strtod(yytext,NULL); return TOKEN_DOUBLE_CONSTANT; }
{HEX_DOUBLE} { double_val = strtod(yytext,NULL); return TOKEN_DOUBLE_CONSTANT; }
{IEEE_DOUBLE} { double_val = ieee754tod(yytext); return TOKEN_DOUBLE_CONSTANT; }
"/*"					{ BEGIN (COMMENT); }
<COMMENT>"*/"			{ BEGIN (INITIAL); return TOKEN_COMMENT; }
<COMMENT>([^*]|\n)+|./"*/" 	{}
<COMMENT><<EOF>>		{ printf("%d: Unterminated comment\n", yylineno); return 0; }

"//"					{ BEGIN(INLINE_COMMENT); }
<INLINE_COMMENT>\n		{ BEGIN(INITIAL); return TOKEN_COMMENT;}
<INLINE_COMMENT><<EOF>>		{ printf("%d: Unterminated comment\n", yylineno); return 0; }


\$c(0|1|2|3|4|5|6|7) 	{ return TOKEN_CREGISTER;}
\$d(0|1|2|3|4|5|6|7) 	{ return TOKEN_DREGISTER;}
\$s(0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15) { return TOKEN_SREGISTER;}
\$q(0|1|2|3|4|5|6|7) 	{ return TOKEN_QREGISTER;}

\%[_a-zA-Z]+[_a-zA-Z0-9]*  	{return TOKEN_LOCAL_IDENTIFIER; }
\&[_a-zA-Z]+[_a-zA-Z0-9]*	{return TOKEN_GLOBAL_IDENTIFIER; }

\@[_a-zA-Z]+[_a-zA-Z0-9]* 	{ return TOKEN_LABEL; }
\"(\\.|[^\\"])*\" 		{ return TOKEN_STRING; }

((snorm_|unorm_)(((int)(8|16|32))|((short_)(565|555|101010)))) |
((signed_|unsigned_)(int)(8|16|32)) |
((half_)?(float))													{return TOKEN_PROPERTY; }
(r|rx|a|rg|rgx|ra|rgb|rgbx|ra|rgb|rgbx|rgba|bgra|argb|intensity|luminance) {return TOKEN_PROPERTY; }
(normalized|unnormalized) { return TOKEN_PROPERTY; }
(nearest|linear) { return TOKEN_PROPERTY; }	
(wrap|clamp|mirror|mirroronce|border) {return TOKEN_PROPERTY;}

WAVESIZE			{return TOKEN_WAVESIZE; }

	/* keywords */
workgroupid			{ return WORKGROUPID; printf("WORKGROUPID\n");}
version				{ return VERSION; }
global				{ return GLOBAL; }
_Samp				{ return _SAMP; }
_RWImg				{ return _RWIMG; }
_ROImg				{ return _ROIMG; }
align				{ return ALIGN; }
file				{ return _FILE; }
arg					{ return ARG; }
kernarg				{ return KERNARG; }
function			{ return FUNCTION; }
:fbar				{ return _FBAR; }
signature			{ return SIGNATURE; }
block				{ return BLOCK; }
endblock			{ return ENDBLOCK; }
blocknumeric		{ return BLOCKNUMERIC; }
blockstring			{ return BLOCKSTRING; }
kernel				{ return KERNEL; }
pragma				{ return PRAGMA; }
labeltargets		{ return LABELTARGETS; }
extension			{ return EXTENSION; }
extern				{ return EXTERN; }
static				{ return STATIC; }
const				{ return CONST; }
private				{ return PRIVATE; }
spill				{ return SPILL; }
group				{ return GROUP; }
readonly			{ return READONLY; }
loc					{ return LOC; }
	/* -- addressSpaceIdentifier -- */
_readonly			{ return  _READONLY; }
_kernarg			{ return  _KERNARG; 	}
_global				{ return  _GLOBAL; }	
_private			{ return  _PRIVATE; }
_arg				{ return  _ARG; 	}
_group				{ return  _GROUP; }
_spill				{ return  _SPILL; }

	/* -- vector -- */
_v2					{ return _V2; }
_v4					{ return _V4; }


	/* -- init -- */
format				{ return  FORMAT; }
order				{ return  ORDER; }
coord				{ return  COORD; }
filter				{ return  FILTER; }
boundaryU			{ return  BOUNDARYU; }
boundaryV			{ return  BOUNDARYV; }
boundaryW			{ return  BOUNDARYW; }

.	{ fprintf(stderr, "unexpected char"); return 0;}
