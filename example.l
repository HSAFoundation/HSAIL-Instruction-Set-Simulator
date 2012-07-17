%option noyywrap
%{
#include <stdio.h>
#include "tokens.h"

int int_val;
%}
OCT_INT		([+|-]?0[0-7]*)
DEC_INT		([+|-]?(0|([1-9]*[0-9]+)))
HEX_INT		([+|-]?0(x|X)[0-9a-fA-F]*)	
%x INLINE_COMMENT
%x COMMENT
DEC_SINGLE	([+|-]?(([0-9]*("."[0-9]*)?((e|E)[+|-]?[0-9]*)?)|([0-9]+(e|E)[+|-]?[0-9]*))(f|F))

%%
{OCT_INT} { int_val = strtol(yytext, NULL, 8); return TOKEN_INTEGER_CONSTANT; } 
{DEC_INT} { int_val = strtol(yytext, NULL, 10); return TOKEN_INTEGER_CONSTANT; }
{HEX_INT} { int_val = strtol(yytext, NULL, 16); return TOKEN_INTEGER_CONSTANT; }

{DEC_SINGLE} { return 5; }

	/* puctuation */
"+" |
"-" |
"=" |
"<" |
">" |
"[" |
"]" |
"{" |
"}" |
"(" |
")" |
":" |
";" |
","		               { return yytext[0];}

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

	/* -- control -- */
itemsperworkgroup	{ return  ITEMS_PER_WORKGROUP; }
workgroupspercu		{ return  WORKGROUPS_PER_CU; }
memopt_on			{ return  MEMOPT_ON; }
memopt_off			{ return  MEMOPT_OFF; }

	/* -- Opcode -- */
fbar_initSizeWg		{ return  FBAR_INITSIZEWG; }
fbar_wait			{ return  FBAR_WAIT; }
fbar_arrive			{ return  FBAR_ARRIVE; }
fbar_skip			{ return  FBAR_SKIP; }
fbar_release		{ return  FBAR_RELEASE; }
countup				{ return  COUNTUP; }

laneid				{ return  LANEID; }
dynwaveid			{ return  DYNWAVEID; }
maxdynwaveid		{ return  MAXDYNWAVEID; }
dispatchid			{ return  DISPATCHID; }
cu					{ return  CU; }
workdim				{ return  WORKDIM; }
workitemid_flat		{ return  WORKITEMID_FLAT; }
workitemaid_flat	{ return  WORKITEMAID_FLAT; }
debugtrap			{ return  DEBUGTRAP; }



.	{ fprintf(stderr, "unexpected char"); return 0;}
