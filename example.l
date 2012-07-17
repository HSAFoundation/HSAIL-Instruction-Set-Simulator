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
.	{ fprintf(stderr, "unexpected char"); return 0;}
