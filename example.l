%option noyywrap
%{
#include <stdio.h>
#include "tokens.h"
%}
OCT_INT		([+|-]?0[0-7]*)
DEC_INT		([+|-]?(0|([1-9]*[0-9]+)))
HEX_INT		([+|-]?0(x|X)[0-9a-fA-F]*)	
%x INLINE_COMMENT
%x COMMENT
%%
{OCT_INT} { return 3; } 
{DEC_INT} { return 2; }
{HEX_INT} { return 4; }

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
.	{ fprintf(stderr, "unexpected char"); return 0;}
