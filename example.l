%option noyywrap
%{
#include <stdio.h>

%}
%%
[a-z]+ { return 1; }
.	{ fprintf(stderr, "unexpected char"); return 0;}
