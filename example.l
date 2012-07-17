%option noyywrap
%{
#include <stdio.h>
#include "tokens.h"
%}
%%
[a-z]+ { return 1; }
[0-9]+ { return 2; }
\$c(0|1|2|3|4|5|6|7) 	{ return TOKEN_CREGISTER;}
\$d(0|1|2|3|4|5|6|7) 	{ return TOKEN_DREGISTER;}
\$s(0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15) { return TOKEN_SREGISTER;}
.	{ fprintf(stderr, "unexpected char"); return 0;}
