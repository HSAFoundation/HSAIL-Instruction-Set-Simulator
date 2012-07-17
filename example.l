%option noyywrap
%{
#include <stdio.h>
#include "tokens.h"
%}
OCT_INT		([+|-]?0[0-7]*)
DEC_INT		([+|-]?(0|([1-9]*[0-9]+)))
HEX_INT		([+|-]?0(x|X)[0-9a-fA-F]*)	

%x COMMENT
%%
[a-z]+ { return 1; }
{OCT_INT} { return 3; } 
{DEC_INT} { return 2; }
{HEX_INT} { return 4; }

"/*"					{ BEGIN (COMMENT); }
<COMMENT>"*/"			{ BEGIN (INITIAL); return TOKEN_COMMENT; }
<COMMENT>([^*]|\n)+|./"*/" 	{}
<COMMENT><<EOF>>		{ printf("%d: Unterminated comment\n", yylineno); return 0; }

\$c(0|1|2|3|4|5|6|7) 	{ return TOKEN_CREGISTER;}
\$d(0|1|2|3|4|5|6|7) 	{ return TOKEN_DREGISTER;}
\$s(0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15) { return TOKEN_SREGISTER;}
\$q(0|1|2|3|4|5|6|7) 	{ return TOKEN_QREGISTER;}
.	{ fprintf(stderr, "unexpected char"); return 0;}
