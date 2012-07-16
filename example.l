%option noyywrap
%{
#include <stdio.h>

%}
%%
[a-z]+ { return 1; }
[0-9]+ { return 2; }
.	{ fprintf(stderr, "unexpected char"); return 0;}
