/* Copyright 2012 <MulticorewareInc> */

%option noyywrap yylineno
%{
#include <stdio.h>
#include "tokens.h"
#include "brig.h"
#include "context.h"
#include "error_reporter.h"
// global variables
hsa::brig::Context* context = hsa::brig::Context::get_instance();

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
  double return_val = 0.0;  // return context->token_value
  unsigned int* int_ptr = 0;
  char first4[5];  // first 2 bytes(4 chars after 0d)
  char temp_str[5];  // temporary string to hold part of yytext
  unsigned int i;
  unsigned int first_int;    // convert the double as 2 integers
  unsigned int second_int;
  unsigned int temp = 0;

  // convert the first 2 bytes
  for (i=0; i<4; i++)
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
  // printf("Return context->token_value after update first 4 bytes: %2.16f\n",return_val);


  // for the last 4bytes, do the same thing, to avoid treating the first bit as the SIGN.
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

  //printf("Final return context->token_value: %2.14f\n",return_val);
  return return_val;
}

%}

%x INLINE_COMMENT
%x COMMENT

OCT_INT     ([+|-]?0[0-7]*)
DEC_INT     ([+|-]?(0|([1-9]*[0-9]+)))
HEX_INT     ([+|-]?0(x|X)[0-9a-fA-F]*)

DEC_DOUBLE  ([+|-]?(([0-9]*("."[0-9]*)?((e|E)[+|-]?[0-9]*)?)|([0-9]+(e|E)[+|-]?[0-9]*))(l|L)?)
HEX_DOUBLE  (0(x|X)[0-9a-fA-F]+(("."[0-9a-fA-F]*)((p|P)[+|-]?[0-9]*)?)(l|L)?)
IEEE_DOUBLE (0(d|D)[0-9a-fA-F]{16})

DEC_SINGLE  ([+|-]?(([0-9]*("."[0-9]*)?((e|E)[+|-]?[0-9]*)?)|([0-9]+(e|E)[+|-]?[0-9]*))(f|F))
HEX_SINGLE  (0(x|X)[0-9a-fA-F]+(("."[0-9a-fA-F]*)((p|P)[+|-]?[0-9]*)?)(f|F))
IEEE_SINGLE (0(f|F)[0-9a-fA-F]{8})

%%

\n                /* ignore EOL */ context->yycolno = 0;
[  \t]+           /* ignore whitespace */ context->yycolno+=yyleng;
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
","           { context->token_type = PUNCTUATION;
								context->yycolno+=yyleng;
								return yytext[0];}

{OCT_INT}     { context->token_type = CONSTANT;
								context->yycolno+=yyleng;
								context->token_value.int_val = strtol(yytext, NULL, 8);
								return TOKEN_INTEGER_CONSTANT; }
								
{DEC_INT}     { context->token_type = CONSTANT;
								context->yycolno+=yyleng;
								context->token_value.int_val = strtol(yytext, NULL, 10);
								return TOKEN_INTEGER_CONSTANT; }
								
{HEX_INT}     { context->token_type = CONSTANT;
								context->yycolno+=yyleng;
								context->token_value.int_val = strtol(yytext, NULL, 16);
								return TOKEN_INTEGER_CONSTANT; }

{DEC_SINGLE}  { context->token_type = CONSTANT;
								context->yycolno+=yyleng;
								context->token_value.float_val = strtof(yytext,NULL);
								return TOKEN_SINGLE_CONSTANT; }
								
{HEX_SINGLE}  { context->token_type = CONSTANT;
								context->yycolno+=yyleng;
								context->token_value.float_val = strtof(yytext,NULL);
								return TOKEN_SINGLE_CONSTANT; }
								
{IEEE_SINGLE} { context->token_type = CONSTANT;
								context->yycolno+=yyleng;
								context->token_value.float_val = ieee754tof(yytext);
								return TOKEN_SINGLE_CONSTANT; }

{DEC_DOUBLE}  { context->token_type = CONSTANT;
								context->yycolno+=yyleng;
								context->token_value.double_val = strtod(yytext,NULL);
								return TOKEN_DOUBLE_CONSTANT; }
								
{HEX_DOUBLE}  { context->token_type = CONSTANT;
								context->yycolno+=yyleng;
								context->token_value.double_val = strtod(yytext,NULL);
								return TOKEN_DOUBLE_CONSTANT; }
								
{IEEE_DOUBLE} { context->token_type = CONSTANT;
								context->yycolno+=yyleng;
								context->token_value.double_val = ieee754tod(yytext);
								return TOKEN_DOUBLE_CONSTANT; }



"/*"                          { BEGIN (COMMENT); }
<COMMENT>"*/"                 { context->token_type = CODE_COMMENTS; 							  
																context->token_value.string_val = strdup(yytext); 
																BEGIN (INITIAL); 
																return TOKEN_COMMENT; }
																
<COMMENT>([^*]|\n)+|./"*/"    {}
<COMMENT><<EOF>>              { printf("%d: Unterminated comment\n", yylineno); 
																return 0; }

"//"                          { BEGIN(INLINE_COMMENT); }

<INLINE_COMMENT>.             {}

<INLINE_COMMENT>\n            { context->token_type = CODE_COMMENTS; 
																context->token_value.string_val = strdup(yytext); 
																BEGIN(INITIAL); 
																return TOKEN_COMMENT;}
																
<INLINE_COMMENT><<EOF>>       { printf("%d: Unterminated comment\n", yylineno); 
																return 0; }

\$c(0|1|2|3|4|5|6|7)          { context->yycolno+=yyleng; 
																context->token_value.string_val = strdup(yytext);
																context->token_type = REGISTER; 
																return TOKEN_CREGISTER;}
																
\$d(0|1|2|3|4|5|6|7)                     {  context->yycolno+=yyleng;
																						context->token_value.string_val = strdup(yytext); 
																						context->token_type = REGISTER; 
																						return TOKEN_DREGISTER;}
																						
\$s(0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15)  { context->yycolno+=yyleng; 
																							context->token_value.string_val = strdup(yytext); 
																							context->token_type = REGISTER; 
																							return TOKEN_SREGISTER;}
																							
\$q(0|1|2|3|4|5|6|7)                        { context->yycolno+=yyleng;																																	  
																							context->token_value.string_val = strdup(yytext);
																							context->token_type = REGISTER;
																							return TOKEN_QREGISTER;}

\%[_a-zA-Z]+[_a-zA-Z0-9]*      { context->token_type = IDENTIFIER;
																 context->yycolno+=yyleng;
																 context->token_value.string_val = strdup(yytext);
																 return TOKEN_LOCAL_IDENTIFIER; }
																 
\&[_a-zA-Z]+[_a-zA-Z0-9]*      { context->token_type = IDENTIFIER;
																 context->yycolno+=yyleng;
																 context->token_value.string_val = strdup(yytext);
																 return TOKEN_GLOBAL_IDENTIFIER; }

\@[_a-zA-Z]+[_a-zA-Z0-9]*      { context->token_type = LABEL;
																 context->yycolno+=yyleng;
																 context->token_value.string_val = strdup(yytext);
																 return TOKEN_LABEL; }
																 
\"(\\.|[^\\"])*\"              { context->token_type = STRING;
																 context->yycolno+=yyleng;
																 context->token_value.string_val = strdup(yytext);
																 return TOKEN_STRING; }

((snorm_|unorm_)(((int)(8|16|32))|((short_)(565|555|101010)))) |
((signed_|unsigned_)(int)(8|16|32)) |
((half_)?(float))                                                          { context->token_type = PROPERTY; context->yycolno+=yyleng; return TOKEN_PROPERTY; }
(r|rx|a|rg|rgx|ra|rgb|rgbx|ra|rgb|rgbx|rgba|bgra|argb|intensity|luminance) { context->token_type = PROPERTY; context->yycolno+=yyleng;return TOKEN_PROPERTY; }
(normalized|unnormalized)                                                  { context->token_type = PROPERTY; context->yycolno+=yyleng;  return TOKEN_PROPERTY; }
(nearest|linear)                                                           { context->token_type = PROPERTY; context->yycolno+=yyleng;  return TOKEN_PROPERTY; }
(wrap|clamp|mirror|mirroronce|border)                                      { context->token_type = PROPERTY; context->yycolno+=yyleng; return TOKEN_PROPERTY;}

WAVESIZE      { context->token_type = WAVESIZE; context->yycolno+=yyleng; return TOKEN_WAVESIZE; }

  /* keywords */
workgroupid      { context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return WORKGROUPID;}
version          { context->yycolno+=yyleng;  return VERSION; }
global           { context->token_type = INITIALIZABLE_ADDRESS; context->yycolno+=yyleng;  return GLOBAL; }
_Samp            { context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return _SAMP; }
_RWImg           { context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return _RWIMG; }
_ROImg           { context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return _ROIMG; }
align            { context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return ALIGN; }
file             { context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return _FILE; }
arg              { context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return ARG; }
kernarg          {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return KERNARG; }
function         {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return FUNCTION; }
:fbar            {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return _FBAR; }
signature        {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return SIGNATURE; }
block            {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return BLOCK; }
endblock         {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return ENDBLOCK; }
blocknumeric     {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return BLOCKNUMERIC; }
blockstring      {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return BLOCKSTRING; }
kernel           {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return KERNEL; }
pragma           {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return PRAGMA; }
labeltargets     {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return LABELTARGETS; }
extension        {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return EXTENSION; }
extern           {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return EXTERN; }
static           {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return STATIC; }
const            {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return CONST; }
private          { context->token_type = UNINITIALIZABLE_ADDRESS; context->yycolno+=yyleng;  return PRIVATE; }
spill            { context->token_type = UNINITIALIZABLE_ADDRESS; context->yycolno+=yyleng;  return SPILL; }
group            { context->token_type = UNINITIALIZABLE_ADDRESS; context->yycolno+=yyleng;  return GROUP; }
readonly         { context->token_type = INITIALIZABLE_ADDRESS; context->yycolno+=yyleng;  return READONLY; }
loc              {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return LOC; }
_width           {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return _WIDTH; }
all              {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return ALL; }

  /* -- addressSpaceIdentifier -- */
_readonly        { context->token_type = ADDRESS_SPACE_IDENTIFIER; context->yycolno+=yyleng;  return  _READONLY; }
_kernarg         { context->token_type = ADDRESS_SPACE_IDENTIFIER; context->yycolno+=yyleng;  return  _KERNARG;   }
_global          { context->token_type = ADDRESS_SPACE_IDENTIFIER; context->yycolno+=yyleng;  return  _GLOBAL; }
_private         { context->token_type = ADDRESS_SPACE_IDENTIFIER; context->yycolno+=yyleng;  return  _PRIVATE; }
_arg             { context->token_type = ADDRESS_SPACE_IDENTIFIER; context->yycolno+=yyleng;  return  _ARG;   }
_group           { context->token_type = ADDRESS_SPACE_IDENTIFIER; context->yycolno+=yyleng;  return  _GROUP; }
_spill           { context->token_type = ADDRESS_SPACE_IDENTIFIER; context->yycolno+=yyleng;  return  _SPILL; }

  /* -- vector -- */
_v2              { context->token_type = VECTOR; context->yycolno+=yyleng;  return _V2; }
_v4              { context->token_type = VECTOR; context->yycolno+=yyleng;  return _V4; }


  /* -- init -- */
format           {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return  FORMAT; }
order            {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return  ORDER; }
coord            {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return  COORD; }
filter           {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return  FILTER; }
boundaryU        {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return  BOUNDARYU; }
boundaryV        {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return  BOUNDARYV; }
boundaryW        {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return  BOUNDARYW; }

  /* -- control -- */
itemsperworkgroup  {  context->token_type = CONTROL_KEYWORD; context->yycolno+=yyleng;  return  ITEMS_PER_WORKGROUP; }
workgroupspercu    {  context->token_type = CONTROL_KEYWORD; context->yycolno+=yyleng;  return  WORKGROUPS_PER_CU; }
memopt_on          {  context->token_type = CONTROL_KEYWORD; context->yycolno+=yyleng;  return  MEMOPT_ON; }
memopt_off         {  context->token_type = CONTROL_KEYWORD; context->yycolno+=yyleng;  return  MEMOPT_OFF; }

  /* -- Opcode -- */
fbar_initSizeWg    { context->token_type = INSTRUCTION1_OPCODE; context->yycolno+=yyleng;  return  FBAR_INITSIZEWG; }
fbar_wait          { context->token_type = INSTRUCTION1_OPCODE; context->yycolno+=yyleng;  return  FBAR_WAIT; }
fbar_arrive        { context->token_type = INSTRUCTION1_OPCODE; context->yycolno+=yyleng;  return  FBAR_ARRIVE; }
fbar_skip          { context->token_type = INSTRUCTION1_OPCODE; context->yycolno+=yyleng;  return  FBAR_SKIP; }
fbar_release       { context->token_type = INSTRUCTION1_OPCODE; context->yycolno+=yyleng;  return  FBAR_RELEASE; }
countup            { context->token_type = INSTRUCTION1_OPCODE; context->yycolno+=yyleng;  return  COUNTUP; }

laneid             { context->token_type = INSTRUCTION1_OPCODE_NODT; context->yycolno+=yyleng;  return  LANEID; }
dynwaveid          { context->token_type = INSTRUCTION1_OPCODE_NODT; context->yycolno+=yyleng;  return  DYNWAVEID; }
maxdynwaveid       { context->token_type = INSTRUCTION1_OPCODE_NODT; context->yycolno+=yyleng;  return  MAXDYNWAVEID; }
dispatchid         { context->token_type = INSTRUCTION1_OPCODE_NODT; context->yycolno+=yyleng;  return  DISPATCHID; }
cu                 { context->token_type = INSTRUCTION1_OPCODE_NODT; context->yycolno+=yyleng;  return  CU; }
workdim            { context->token_type = INSTRUCTION1_OPCODE_NODT; context->yycolno+=yyleng;  return  WORKDIM; }
workitemid_flat    { context->token_type = INSTRUCTION1_OPCODE_NODT; context->yycolno+=yyleng;  return  WORKITEMID_FLAT; }
workitemaid_flat   { context->token_type = INSTRUCTION1_OPCODE_NODT; context->yycolno+=yyleng;  return  WORKITEMAID_FLAT; }
debugtrap          { context->token_type = INSTRUCTION1_OPCODE_NODT; context->yycolno+=yyleng;  return  DEBUGTRAP; }

abs                { context->token_value.opcode = BrigAbs; context->token_type = INSTRUCTION2_OPCODE; context->yycolno+=yyleng;  return  ABS; }
neg                { context->token_type = INSTRUCTION2_OPCODE; context->yycolno+=yyleng;  return  NEG; }
not                { context->token_type = INSTRUCTION2_OPCODE; context->yycolno+=yyleng;  return  NOT; }
popcount           { context->token_type = INSTRUCTION2_OPCODE; context->yycolno+=yyleng;  return  POPCOUNT; }
firstbit           { context->token_type = INSTRUCTION2_OPCODE; context->yycolno+=yyleng;  return  FIRSTBIT; }
lastbit            { context->token_type = INSTRUCTION2_OPCODE; context->yycolno+=yyleng;  return  LASTBIT; }
bitrev             { context->token_type = INSTRUCTION2_OPCODE; context->yycolno+=yyleng;  return  BITREV; }
movs_lo            { context->token_type = INSTRUCTION2_OPCODE; context->yycolno+=yyleng;  return  MOVS_LO; }
movs_hi            { context->token_type = INSTRUCTION2_OPCODE; context->yycolno+=yyleng;  return  MOVS_HI; }
fbar_initSize      { context->token_type = INSTRUCTION2_OPCODE; context->yycolno+=yyleng;  return  FBAR_INITSIZE; }
fbar_init          { context->token_type = INSTRUCTION2_OPCODE; context->yycolno+=yyleng;  return  FBAR_INIT; }
fbar_releaseCF     { context->token_type = INSTRUCTION2_OPCODE; context->yycolno+=yyleng;  return  FBAR_RELEASECF; }
count              { context->token_type = INSTRUCTION2_OPCODE; context->yycolno+=yyleng;  return  COUNT; }
mask               { context->token_type = INSTRUCTION2_OPCODE; context->yycolno+=yyleng;  return  MASK; }

sqrt               { context->token_value.opcode = BrigSqrt; context->token_type = INSTRUCTION2_OPCODE_FTZ; context->yycolno+=yyleng;  return  SQRT; }
fract              { context->token_type = INSTRUCTION2_OPCODE_FTZ; context->yycolno+=yyleng;  return  FRACT; }
fcos               { context->token_type = INSTRUCTION2_OPCODE_FTZ; context->yycolno+=yyleng;  return  FCOS; }
fsin               { context->token_type = INSTRUCTION2_OPCODE_FTZ; context->yycolno+=yyleng;  return  FSIN; }
flog2              { context->token_type = INSTRUCTION2_OPCODE_FTZ; context->yycolno+=yyleng;  return  FLOG2; }
fexp2              { context->token_type = INSTRUCTION2_OPCODE_FTZ; context->yycolno+=yyleng;  return  FEXP2; }
fsqrt              { context->token_type = INSTRUCTION2_OPCODE_FTZ; context->yycolno+=yyleng;  return  FSQRT; }
frsqrt             { context->token_type = INSTRUCTION2_OPCODE_FTZ; context->yycolno+=yyleng;  return  FRSQRT; }
frcp               { context->token_type = INSTRUCTION2_OPCODE_FTZ; context->yycolno+=yyleng;  return  FRCP; }

unpack3            { context->token_type = INSTRUCTION2_OPCODE_NODT; context->yycolno+=yyleng;  return  UNPACK3; }
unpack2            { context->token_type = INSTRUCTION2_OPCODE_NODT; context->yycolno+=yyleng;  return  UNPACK2; }
unpack1            { context->token_type = INSTRUCTION2_OPCODE_NODT; context->yycolno+=yyleng;  return  UNPACK1; }
unpack0            { context->token_type = INSTRUCTION2_OPCODE_NODT; context->yycolno+=yyleng;  return  UNPACK0; }
alloca             { context->token_type = INSTRUCTION2_OPCODE_NODT; context->yycolno+=yyleng;  return  ALLOCA;}
workitemid         { context->token_type = INSTRUCTION2_OPCODE_NODT; context->yycolno+=yyleng;  return  WORKITEMID; }
workitemaid        { context->token_type = INSTRUCTION2_OPCODE_NODT; context->yycolno+=yyleng;  return  WORKITEMAID;}
workgroupsize      { context->token_type = INSTRUCTION2_OPCODE_NODT; context->yycolno+=yyleng;  return  WORKGROUPSIZE;}
NDRangesize        { context->token_type = INSTRUCTION2_OPCODE_NODT; context->yycolno+=yyleng;  return  NDRANGESIZE;}
NDRangegroups      { context->token_type = INSTRUCTION2_OPCODE_NODT; context->yycolno+=yyleng;  return  NDRANGEGROUPS; }

add                { context->token_value.opcode = BrigAdd; context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  ADD;}
carry              { context->token_value.opcode = BrigCarry; context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  CARRY; }
borrow             { context->token_value.opcode = BrigBorrow; context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  BORROW; }
div                { context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  DIV; }
rem                { context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  REM; }
sub                { context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  SUB; }
shl                { context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  SHL;}
shr                { context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  SHR;}
and                { context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  AND;}
xor                { context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  XOR;}
or                 { context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  OR;}
unpacklo           { context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  UNPACKLO;}
unpackhi           { context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  UNPACKHI;}
movd_lo            { context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  MOVD_LO;}
movd_hi            { context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  MOVD_HI;}
copysign           { context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  COPYSIGN;}
class              { context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  CLASS;}
send               { context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  SEND;}
receive            { context->token_type = INSTRUCTION3_OPCODE; context->yycolno+=yyleng;  return  RECEIVE;}


max                { context->token_type = INSTRUCTION3_OPCODE_FTZ; context->yycolno+=yyleng;  return  MAX;}
min                { context->token_type = INSTRUCTION3_OPCODE_FTZ; context->yycolno+=yyleng;  return  MIN;}

mad                { context->token_type = INSTRUCTION4_OPCODE; context->yycolno+=yyleng;  return  MAD;}
extract            { context->token_type = INSTRUCTION4_OPCODE; context->yycolno+=yyleng;  return  EXTRACT;}
insert             { context->token_type = INSTRUCTION4_OPCODE; context->yycolno+=yyleng;  return  INSERT;}
shuffle            { context->token_type = INSTRUCTION4_OPCODE; context->yycolno+=yyleng;  return  SHUFFLE;}
cmov               { context->token_type = INSTRUCTION4_OPCODE; context->yycolno+=yyleng;  return  CMOV;}
fma                { context->token_type = INSTRUCTION4_OPCODE; context->yycolno+=yyleng;  return  FMA;}
bitalign           { context->token_type = INSTRUCTION4_OPCODE; context->yycolno+=yyleng;  return  BITALIGN;}
bytealign          { context->token_type = INSTRUCTION4_OPCODE; context->yycolno+=yyleng;  return  BYTEALIGN;}
lerp               { context->token_type = INSTRUCTION4_OPCODE; context->yycolno+=yyleng;  return  LERP;}
sad                { context->token_type = INSTRUCTION4_OPCODE; context->yycolno+=yyleng;  return  SAD;}
sad2               { context->token_type = INSTRUCTION4_OPCODE; context->yycolno+=yyleng;  return  SAD2;}
sad4               { context->token_type = INSTRUCTION4_OPCODE; context->yycolno+=yyleng;  return  SAD4;}
sad4hi             { context->token_type = INSTRUCTION4_OPCODE; context->yycolno+=yyleng;  return  SAD4HI;}
bitselect          { context->token_type = INSTRUCTION4_OPCODE; context->yycolno+=yyleng;  return  BITSELECT;}

cbr                {  context->token_type =BRANCH_OP; context->yycolno+=yyleng;  return CBR; }
brn                {  context->token_type =BRANCH_OP; context->yycolno+=yyleng;  return BRN; }
_fbar              {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return __FBAR; }
call               {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return CALL; }
  /*-- atomicOperationId --*/
_and               {  context->token_type = ATOMIC_OP; context->yycolno+=yyleng;  return  _AND_;}
_or                {  context->token_type = ATOMIC_OP; context->yycolno+=yyleng;  return  _OR_;}
_xor               {  context->token_type = ATOMIC_OP; context->yycolno+=yyleng;  return  _XOR_;}
_exch              {  context->token_type = ATOMIC_OP; context->yycolno+=yyleng;  return  _EXCH_;}
_add               {  context->token_type = ATOMIC_OP; context->yycolno+=yyleng;  return  _ADD_;}
_sub               {  context->token_type = ATOMIC_OP; context->yycolno+=yyleng;  return  _SUB_;}
_inc               {  context->token_type = ATOMIC_OP; context->yycolno+=yyleng;  return  _INC_;}
_dec               {  context->token_type = ATOMIC_OP; context->yycolno+=yyleng;  return  _DEC_;}
_max               {  context->token_type = ATOMIC_OP; context->yycolno+=yyleng;  return   _MAX_;}
_min               {  context->token_type = ATOMIC_OP; context->yycolno+=yyleng;  return   _MIN_;}

  /*-- comparison --*/
_eq                {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _EQ;}
_ne                {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _NE;}
_lt                {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _LT;}
_le                {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _LE;}
_gt                {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _GT;}
_ge                {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _GE;}
_equ               {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _EQU;}
_neu               {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _NEU;}
_ltu               {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _LTU;}
_leu               {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _LEU;}
_gtu               {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _GTU;}
_geu               {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _GEU;}
_num               {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _NUM;}
_nan               {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _NAN;}
_seq               {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _SEQ;}
_sne               {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _SNE;}
_slt               {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _SLT;}
_sle               {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _SLE;}
_sgt               {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _SGT;}
_sge               {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _SGE;}
_snum              {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _SNUM;}
_snan              {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _SNAN;}
_sequ              {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _SEQU;}
_sneu              {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _SNEU;}
_sltu              {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _SLTU;}
_sleu              {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _SLEU;}
_sgtu              {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _SGTU;}
_sgeu              {  context->token_type = COMPARISON; context->yycolno+=yyleng;  return  _SGEU;}

  /*-- rounding --*/
_upi               { context->yycolno+=yyleng; context->token_type = INT_ROUNDING; return  _UPI;}
_downi             { context->yycolno+=yyleng; context->token_type = INT_ROUNDING;  return  _DOWNI;}
_zeroi             { context->yycolno+=yyleng; context->token_type = INT_ROUNDING;  return  _ZEROI;}
_neari             { context->yycolno+=yyleng; context->token_type = INT_ROUNDING;  return  _NEARI;}

_up                { context->yycolno+=yyleng; context->token_type = FLOAT_ROUNDING;  return  _UP;}
_down              { context->yycolno+=yyleng; context->token_type = FLOAT_ROUNDING;  return  _DOWN;}
_zero              { context->yycolno+=yyleng; context->token_type = FLOAT_ROUNDING;  return  _ZERO;}
_near              { context->yycolno+=yyleng; context->token_type = FLOAT_ROUNDING;  return  _NEAR;}

  /*-- context->token_value.packing -- */
_pp                { context->token_value.packing = BrigPackPP; context->token_type = PACKING; context->yycolno+=yyleng;  return  _PP;}
_ps                { context->token_value.packing = BrigPackPS; context->token_type = PACKING; context->yycolno+=yyleng;  return  _PS;}
_sp                { context->token_value.packing = BrigPackSP; context->token_type = PACKING; context->yycolno+=yyleng;  return  _SP;}
_ss                { context->token_value.packing = BrigPackSS; context->token_type = PACKING; context->yycolno+=yyleng;  return  _SS;}
_s                 { context->token_value.packing = BrigPackS; context->token_type = PACKING; context->yycolno+=yyleng;  return  __S;}
_p                 { context->token_value.packing = BrigPackP; context->token_type = PACKING; context->yycolno+=yyleng;  return  __P;}
_pp_sat            { context->token_value.packing = BrigPackPPsat; context->token_type = PACKING; context->yycolno+=yyleng;  return  _PP_SAT;}
_ps_sat            { context->token_value.packing = BrigPackPSsat; context->token_type = PACKING; context->yycolno+=yyleng;  return  _PS_SAT;}
_sp_sat            { context->token_value.packing = BrigPackSPsat; context->token_type = PACKING; context->yycolno+=yyleng;  return  _SP_SAT;}
_ss_sat            { context->token_value.packing = BrigPackSSsat; context->token_type = PACKING; context->yycolno+=yyleng;  return  _SS_SAT;}
_s_sat             { context->token_value.packing = BrigPackSsat; context->token_type = PACKING; context->yycolno+=yyleng;  return  _S_SAT;}
_p_sat             { context->token_value.packing = BrigPackPsat; context->token_type = PACKING; context->yycolno+=yyleng;  return  _P_SAT;}

  /*-- target --*/
\$small            { context->token_type = TARGET; context->yycolno+=yyleng; return  _SMALL;}
\$large            { context->token_type = TARGET; context->yycolno+=yyleng; return  _LARGE;}
\$full             { context->token_type = TARGET; context->yycolno+=yyleng;  return  _FULL;}
\$reduced          { context->token_type = TARGET; context->yycolno+=yyleng;  return  _REDUCED;}
\$sftz             { context->token_type = TARGET; context->yycolno+=yyleng;  return  _SFTZ;}
\$nosftz           { context->token_type = TARGET; context->yycolno+=yyleng;  return  _NOSFTZ;}

width              {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return WIDTH;}
height             {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return HEIGHT;}
depth              {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return DEPTH;}


  /*-- dataTypeId --*/
_u32               { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigu32; context->yycolno+=yyleng;  return  _U32;}
_s32               { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigs32; context->yycolno+=yyleng;  return  _S32;}
_s64               { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigs64; context->yycolno+=yyleng;  return  _S64;}
_u64               { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigu64; context->yycolno+=yyleng;  return  _U64;}
_b1                { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigb1; context->yycolno+=yyleng;  return  _B1;}
_b32               { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigb32; context->yycolno+=yyleng;  return  _B32;}
_f64               { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigf64; context->yycolno+=yyleng;  return  _F64;}
_f32               { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigf32; context->yycolno+=yyleng;  return  _F32;}
_b64               { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigb64; context->yycolno+=yyleng;  return  _B64;}
_b8                { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigb8; context->yycolno+=yyleng;  return  _B8;}
_b16               { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigb16; context->yycolno+=yyleng;  return  _B16;}
_s8                { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigs8; context->yycolno+=yyleng;  return  _S8;}
_s16               { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigs16; context->yycolno+=yyleng;  return  _S16;}
_u8                { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigu8; context->yycolno+=yyleng;  return  _U8;}
_u16               { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigu16; context->yycolno+=yyleng;  return  _U16;}
_f16               { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigf16; context->yycolno+=yyleng;  return  _F16;}
_b128              { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigb128; context->yycolno+=yyleng;  return  _B128;}
_u8x4              { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigu8x4; context->yycolno+=yyleng;  return  _U8X4;}
_s8x4              { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigs8x4; context->yycolno+=yyleng;  return  _S8X4;}
_u16x2             { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigu16x2; context->yycolno+=yyleng;  return  _U16X2;}
_s16x2             { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigs16x2; context->yycolno+=yyleng;  return  _S16X2;}
_f16x2             { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigf16x2; context->yycolno+=yyleng;  return  _F16X2;}
_f32x2             { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigf32x2; context->yycolno+=yyleng;  return  _F32X2;}
_u8x8              { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigu8x8; context->yycolno+=yyleng;  return  _U8X8;}
_s8x8              { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigs8x8; context->yycolno+=yyleng;  return  _S8X8;}
_u16x4             { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigu16x4; context->yycolno+=yyleng;  return  _U16X4;}
_s16x4             { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigs16x4; context->yycolno+=yyleng;  return  _S16X4;}
_f16x4             { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigf16x4; context->yycolno+=yyleng;  return  _F16X4;}
_u8x16             { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigu8x16; context->yycolno+=yyleng;  return  _U8X16;}
_s8x16             { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigs8x16; context->yycolno+=yyleng;  return  _S8X16;}
_u16x8             { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigu16x8; context->yycolno+=yyleng;  return  _U16X8;}
_s16x8             { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigs16x8; context->yycolno+=yyleng;  return  _S16X8;}
_f16x8             { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigf16x8; context->yycolno+=yyleng;  return  _F16X8;}
_f32x4             { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigf32x4; context->yycolno+=yyleng;  return  _F32X4;}
_s32x4             { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigs32x4; context->yycolno+=yyleng;  return  _S32X4;}
_u32x4             { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigu32x4; context->yycolno+=yyleng;  return  _U32X4;}
_f64x2             { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigf64x2; context->yycolno+=yyleng;  return  _F64X2;}
_s64x2             { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigs64x2; context->yycolno+=yyleng;  return  _S64X2;}
_u64x2             { context->token_type = DATA_TYPE_ID; context->token_value.data_type = Brigu64x2; context->yycolno+=yyleng;  return  _U64X2;}


_ftz               {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return _FTZ;}
nop                {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return NOP;}
clock              {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return CLOCK;}
syscall            {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return SYSCALL;}


  /*-- mul --*/
mul               {  context->token_type = MUL_OP; context->yycolno+=yyleng;  return MUL;}
mul_hi            {  context->token_type = MUL_OP; context->yycolno+=yyleng;  return MUL_HI;}
mul24_hi          {  context->token_type = MUL_OP; context->yycolno+=yyleng;  return MUL24_HI;}
mul24             {  context->token_type = MUL_OP; context->yycolno+=yyleng;  return MUL24;}
mad24             {  context->token_type = MUL_OP; context->yycolno+=yyleng;  return MAD24;}
mad24_hi          {  context->token_type = MUL_OP; context->yycolno+=yyleng;  return MAD24_HI;}

f2u4              { context->token_type = INSTRUCTION5_OPCODE; context->yycolno+=yyleng;  return F2U4;}

  /*-- Atom Modifiers --*/
_ar               {  context->token_type = ATOM_MODIFIER; context->yycolno+=yyleng;  return _AR;}
_region           {  context->token_type = ATOM_MODIFIER; context->yycolno+=yyleng;  return _REGION;}
atomic_cas        {  context->token_type = ATOM_MODIFIER; context->yycolno+=yyleng;  return ATOMIC_CAS;}
atomic            {  context->token_type = ATOM_MODIFIER; context->yycolno+=yyleng;  return ATOMIC;}
atomicNoRet       {  context->token_type = ATOM_MODIFIER; context->yycolno+=yyleng;  return ATOMICNORET;}
atomicNoRet_cas   {  context->token_type = ATOM_MODIFIER; context->yycolno+=yyleng;  return ATOMICNORET_CAS;}
atomic_image      {  context->token_type = ATOM_MODIFIER; context->yycolno+=yyleng;  return ATOMIC_IMAGE;}
atomicNoRet_image {  context->token_type = ATOM_MODIFIER; context->yycolno+=yyleng;  return ATOMICNORET_IMAGE;}

cvt               { context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return CVT;}


_dep              {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return _DEP;}
_equiv            {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return _EQUIV;}
_acq              {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return _ACQ;}
_rel              {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return _REL;}
ld                {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return LD;}
sync              {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return SYNC;}
barrier           {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return BARRIER;}
segmentp          {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return SEGMENTP;}
ftos              {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return FTOS;}
stof              {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return STOF; }
mov               {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return MOV;}
lad               {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return LAD;}
ldc               {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return LDC;}
ret               {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return RET;}
packedcmp         {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return PACKEDCMP;}
cmp               {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return CMP;}
st                {  context->token_type = COMMON_KEYWORD; context->yycolno+=yyleng;  return ST;}



  /* -- geometry ID -- */
_1d               {  context->token_type = GEOMETRY_ID; context->yycolno+=yyleng;  return _1D;}
_2d               {  context->token_type = GEOMETRY_ID; context->yycolno+=yyleng;  return _2D;}
_3d               {  context->token_type = GEOMETRY_ID; context->yycolno+=yyleng;  return _3D;}
_1db              {  context->token_type = GEOMETRY_ID; context->yycolno+=yyleng;  return _1DB;}
_1da              {  context->token_type = GEOMETRY_ID; context->yycolno+=yyleng;  return _1DA;}
_2da              {  context->token_type = GEOMETRY_ID; context->yycolno+=yyleng;  return _2DA;}

rd_image          {  context->token_type = IMAGE_OP; context->yycolno+=yyleng;  return RD_IMAGE;}
ld_image          {  context->token_type = IMAGE_OP; context->yycolno+=yyleng;  return LD_IMAGE;}
st_image          {  context->token_type = IMAGE_OP; context->yycolno+=yyleng;  return ST_IMAGE;}




  /* -- queryOp --*/
query_order      { context->token_type = QUERY_OP; context->yycolno+=yyleng;  return QUERY_ORDER;}
query_data       { context->token_type = QUERY_OP; context->yycolno+=yyleng;  return QUERY_DATA;}
query_array      { context->token_type = QUERY_OP; context->yycolno+=yyleng;  return QUERY_ARRAY;}
query_width      { context->token_type = QUERY_OP; context->yycolno+=yyleng;  return QUERY_WIDTH;}
query_depth      { context->token_type = QUERY_OP; context->yycolno+=yyleng;  return QUERY_DEPTH;}
query_height     { context->token_type = QUERY_OP; context->yycolno+=yyleng;  return QUERY_HEIGHT;}
query_normalized { context->token_type = QUERY_OP; context->yycolno+=yyleng;  return QUERY_NORMALIZED;}
query_filtering  { context->token_type = QUERY_OP; context->yycolno+=yyleng;  return QUERY_FILTERING;}



.  {fprintf(stderr, "unexpected char"); return 0;}
