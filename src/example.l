/* Copyright 2012 <MulticorewareInc> */

%option noyywrap yylineno
%{
#include <stdio.h>
#include "tokens.h"
#include "brig.h"

// global variables
int               int_val;
float             float_val;
double            double_val;
char*             string_val;
int               yycolno;

TerminalType      token_type;
BrigDataType16_t  data_type;
BrigOpcode32_t    opcode;
BrigPacking16_t   packing;
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
  double return_val = 0.0;  // return value
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
  // printf("Return value after update first 4 bytes: %2.16f\n",return_val);


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

  //printf("Final return value: %2.14f\n",return_val);
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

\n                /* ignore EOL */ yycolno = 0;
[  \t]+           /* ignore whitespace */ yycolno+=yyleng;
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
","           { token_type = PUNCTUATION; yycolno+=yyleng; return yytext[0];}

{OCT_INT}     { token_type = CONSTANT; yycolno+=yyleng; int_val = strtol(yytext, NULL, 8); return TOKEN_INTEGER_CONSTANT; }
{DEC_INT}     { token_type = CONSTANT; yycolno+=yyleng; int_val = strtol(yytext, NULL, 10); return TOKEN_INTEGER_CONSTANT; }
{HEX_INT}     { token_type = CONSTANT; yycolno+=yyleng; int_val = strtol(yytext, NULL, 16); return TOKEN_INTEGER_CONSTANT; }

{DEC_SINGLE}  { token_type = CONSTANT; yycolno+=yyleng; float_val = strtof(yytext,NULL); return TOKEN_SINGLE_CONSTANT; }
{HEX_SINGLE}  { token_type = CONSTANT; yycolno+=yyleng; float_val = strtof(yytext,NULL); return TOKEN_SINGLE_CONSTANT; }
{IEEE_SINGLE} { token_type = CONSTANT; yycolno+=yyleng; float_val = ieee754tof(yytext); return TOKEN_SINGLE_CONSTANT; }

{DEC_DOUBLE}  { token_type = CONSTANT; yycolno+=yyleng; double_val = strtod(yytext,NULL); return TOKEN_DOUBLE_CONSTANT; }
{HEX_DOUBLE}  { token_type = CONSTANT; yycolno+=yyleng; double_val = strtod(yytext,NULL); return TOKEN_DOUBLE_CONSTANT; }
{IEEE_DOUBLE} { token_type = CONSTANT; yycolno+=yyleng; double_val = ieee754tod(yytext); return TOKEN_DOUBLE_CONSTANT; }



"/*"                          { BEGIN (COMMENT); }
<COMMENT>"*/"                 { token_type = CODE_COMMENTS; string_val = strdup(yytext); BEGIN (INITIAL); return TOKEN_COMMENT; }
<COMMENT>([^*]|\n)+|./"*/"    {}
<COMMENT><<EOF>>              { printf("%d: Unterminated comment\n", yylineno); return 0; }

"//"                          { BEGIN(INLINE_COMMENT); }
<INLINE_COMMENT>.             {}
<INLINE_COMMENT>\n            { token_type = CODE_COMMENTS; string_val = strdup(yytext); BEGIN(INITIAL); return TOKEN_COMMENT;}
<INLINE_COMMENT><<EOF>>       { printf("%d: Unterminated comment\n", yylineno); return 0; }


\$c(0|1|2|3|4|5|6|7)                        { yycolno+=yyleng; string_val = strdup(yytext); token_type = REGISTER; return TOKEN_CREGISTER;}
\$d(0|1|2|3|4|5|6|7)                        { yycolno+=yyleng; string_val = strdup(yytext); token_type = REGISTER; return TOKEN_DREGISTER;}
\$s(0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15)  { yycolno+=yyleng; string_val = strdup(yytext); token_type = REGISTER; return TOKEN_SREGISTER;}
\$q(0|1|2|3|4|5|6|7)                        { yycolno+=yyleng; string_val = strdup(yytext); token_type = REGISTER; return TOKEN_QREGISTER;}

\%[_a-zA-Z]+[_a-zA-Z0-9]*      { token_type = IDENTIFIER; yycolno+=yyleng; string_val = strdup(yytext); return TOKEN_LOCAL_IDENTIFIER; }
\&[_a-zA-Z]+[_a-zA-Z0-9]*      { token_type = IDENTIFIER; yycolno+=yyleng; string_val = strdup(yytext); return TOKEN_GLOBAL_IDENTIFIER; }

\@[_a-zA-Z]+[_a-zA-Z0-9]*      { token_type = LABEL; yycolno+=yyleng; string_val = strdup(yytext);  return TOKEN_LABEL; }
\"(\\.|[^\\"])*\"              { token_type = STRING; yycolno+=yyleng; string_val = strdup(yytext);  return TOKEN_STRING; }

((snorm_|unorm_)(((int)(8|16|32))|((short_)(565|555|101010)))) |
((signed_|unsigned_)(int)(8|16|32)) |
((half_)?(float))                                                          { token_type = PROPERTY; yycolno+=yyleng; return TOKEN_PROPERTY; }
(r|rx|a|rg|rgx|ra|rgb|rgbx|ra|rgb|rgbx|rgba|bgra|argb|intensity|luminance) { token_type = PROPERTY; yycolno+=yyleng;return TOKEN_PROPERTY; }
(normalized|unnormalized)                                                  { token_type = PROPERTY; yycolno+=yyleng;  return TOKEN_PROPERTY; }
(nearest|linear)                                                           { token_type = PROPERTY; yycolno+=yyleng;  return TOKEN_PROPERTY; }
(wrap|clamp|mirror|mirroronce|border)                                      { token_type = PROPERTY; yycolno+=yyleng; return TOKEN_PROPERTY;}

WAVESIZE      { token_type = WAVESIZE; yycolno+=yyleng; return TOKEN_WAVESIZE; }

  /* keywords */
workgroupid      { token_type = COMMON_KEYWORD; yycolno+=yyleng;  return WORKGROUPID;}
version          { yycolno+=yyleng;  return VERSION; }
global           { token_type = INITIALIZABLE_ADDRESS; yycolno+=yyleng;  return GLOBAL; }
_Samp            { token_type = COMMON_KEYWORD; yycolno+=yyleng;  return _SAMP; }
_RWImg           { token_type = COMMON_KEYWORD; yycolno+=yyleng;  return _RWIMG; }
_ROImg           { token_type = COMMON_KEYWORD; yycolno+=yyleng;  return _ROIMG; }
align            { token_type = COMMON_KEYWORD; yycolno+=yyleng;  return ALIGN; }
file             { token_type = COMMON_KEYWORD; yycolno+=yyleng;  return _FILE; }
arg              { token_type = COMMON_KEYWORD; yycolno+=yyleng;  return ARG; }
kernarg          {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return KERNARG; }
function         {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return FUNCTION; }
:fbar            {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return _FBAR; }
signature        {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return SIGNATURE; }
block            {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return BLOCK; }
endblock         {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return ENDBLOCK; }
blocknumeric     {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return BLOCKNUMERIC; }
blockstring      {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return BLOCKSTRING; }
kernel           {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return KERNEL; }
pragma           {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return PRAGMA; }
labeltargets     {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return LABELTARGETS; }
extension        {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return EXTENSION; }
extern           {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return EXTERN; }
static           {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return STATIC; }
const            {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return CONST; }
private          { token_type = UNINITIALIZABLE_ADDRESS; yycolno+=yyleng;  return PRIVATE; }
spill            { token_type = UNINITIALIZABLE_ADDRESS; yycolno+=yyleng;  return SPILL; }
group            { token_type = UNINITIALIZABLE_ADDRESS; yycolno+=yyleng;  return GROUP; }
readonly         { token_type = INITIALIZABLE_ADDRESS; yycolno+=yyleng;  return READONLY; }
loc              {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return LOC; }
_width           {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return _WIDTH; }
all              {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return ALL; }

  /* -- addressSpaceIdentifier -- */
_readonly        { token_type = ADDRESS_SPACE_IDENTIFIER; yycolno+=yyleng;  return  _READONLY; }
_kernarg         { token_type = ADDRESS_SPACE_IDENTIFIER; yycolno+=yyleng;  return  _KERNARG;   }
_global          { token_type = ADDRESS_SPACE_IDENTIFIER; yycolno+=yyleng;  return  _GLOBAL; }
_private         { token_type = ADDRESS_SPACE_IDENTIFIER; yycolno+=yyleng;  return  _PRIVATE; }
_arg             { token_type = ADDRESS_SPACE_IDENTIFIER; yycolno+=yyleng;  return  _ARG;   }
_group           { token_type = ADDRESS_SPACE_IDENTIFIER; yycolno+=yyleng;  return  _GROUP; }
_spill           { token_type = ADDRESS_SPACE_IDENTIFIER; yycolno+=yyleng;  return  _SPILL; }

  /* -- vector -- */
_v2              { token_type = VECTOR; yycolno+=yyleng;  return _V2; }
_v4              { token_type = VECTOR; yycolno+=yyleng;  return _V4; }


  /* -- init -- */
format           {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return  FORMAT; }
order            {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return  ORDER; }
coord            {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return  COORD; }
filter           {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return  FILTER; }
boundaryU        {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return  BOUNDARYU; }
boundaryV        {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return  BOUNDARYV; }
boundaryW        {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return  BOUNDARYW; }

  /* -- control -- */
itemsperworkgroup  {  token_type = CONTROL_KEYWORD; yycolno+=yyleng;  return  ITEMS_PER_WORKGROUP; }
workgroupspercu    {  token_type = CONTROL_KEYWORD; yycolno+=yyleng;  return  WORKGROUPS_PER_CU; }
memopt_on          {  token_type = CONTROL_KEYWORD; yycolno+=yyleng;  return  MEMOPT_ON; }
memopt_off         {  token_type = CONTROL_KEYWORD; yycolno+=yyleng;  return  MEMOPT_OFF; }

  /* -- Opcode -- */
fbar_initSizeWg    { token_type = INSTRUCTION1_OPCODE; yycolno+=yyleng;  return  FBAR_INITSIZEWG; }
fbar_wait          { token_type = INSTRUCTION1_OPCODE; yycolno+=yyleng;  return  FBAR_WAIT; }
fbar_arrive        { token_type = INSTRUCTION1_OPCODE; yycolno+=yyleng;  return  FBAR_ARRIVE; }
fbar_skip          { token_type = INSTRUCTION1_OPCODE; yycolno+=yyleng;  return  FBAR_SKIP; }
fbar_release       { token_type = INSTRUCTION1_OPCODE; yycolno+=yyleng;  return  FBAR_RELEASE; }
countup            { token_type = INSTRUCTION1_OPCODE; yycolno+=yyleng;  return  COUNTUP; }

laneid             { token_type = INSTRUCTION1_OPCODE_NODT; yycolno+=yyleng;  return  LANEID; }
dynwaveid          { token_type = INSTRUCTION1_OPCODE_NODT; yycolno+=yyleng;  return  DYNWAVEID; }
maxdynwaveid       { token_type = INSTRUCTION1_OPCODE_NODT; yycolno+=yyleng;  return  MAXDYNWAVEID; }
dispatchid         { token_type = INSTRUCTION1_OPCODE_NODT; yycolno+=yyleng;  return  DISPATCHID; }
cu                 { token_type = INSTRUCTION1_OPCODE_NODT; yycolno+=yyleng;  return  CU; }
workdim            { token_type = INSTRUCTION1_OPCODE_NODT; yycolno+=yyleng;  return  WORKDIM; }
workitemid_flat    { token_type = INSTRUCTION1_OPCODE_NODT; yycolno+=yyleng;  return  WORKITEMID_FLAT; }
workitemaid_flat   { token_type = INSTRUCTION1_OPCODE_NODT; yycolno+=yyleng;  return  WORKITEMAID_FLAT; }
debugtrap          { token_type = INSTRUCTION1_OPCODE_NODT; yycolno+=yyleng;  return  DEBUGTRAP; }

abs                { opcode = BrigAbs; token_type = INSTRUCTION2_OPCODE; yycolno+=yyleng;  return  ABS; }
neg                { token_type = INSTRUCTION2_OPCODE; yycolno+=yyleng;  return  NEG; }
not                { token_type = INSTRUCTION2_OPCODE; yycolno+=yyleng;  return  NOT; }
popcount           { token_type = INSTRUCTION2_OPCODE; yycolno+=yyleng;  return  POPCOUNT; }
firstbit           { token_type = INSTRUCTION2_OPCODE; yycolno+=yyleng;  return  FIRSTBIT; }
lastbit            { token_type = INSTRUCTION2_OPCODE; yycolno+=yyleng;  return  LASTBIT; }
bitrev             { token_type = INSTRUCTION2_OPCODE; yycolno+=yyleng;  return  BITREV; }
movs_lo            { token_type = INSTRUCTION2_OPCODE; yycolno+=yyleng;  return  MOVS_LO; }
movs_hi            { token_type = INSTRUCTION2_OPCODE; yycolno+=yyleng;  return  MOVS_HI; }
fbar_initSize      { token_type = INSTRUCTION2_OPCODE; yycolno+=yyleng;  return  FBAR_INITSIZE; }
fbar_init          { token_type = INSTRUCTION2_OPCODE; yycolno+=yyleng;  return  FBAR_INIT; }
fbar_releaseCF     { token_type = INSTRUCTION2_OPCODE; yycolno+=yyleng;  return  FBAR_RELEASECF; }
count              { token_type = INSTRUCTION2_OPCODE; yycolno+=yyleng;  return  COUNT; }
mask               { token_type = INSTRUCTION2_OPCODE; yycolno+=yyleng;  return  MASK; }

sqrt               { opcode = BrigSqrt; token_type = INSTRUCTION2_OPCODE_FTZ; yycolno+=yyleng;  return  SQRT; }
fract              { token_type = INSTRUCTION2_OPCODE_FTZ; yycolno+=yyleng;  return  FRACT; }
fcos               { token_type = INSTRUCTION2_OPCODE_FTZ; yycolno+=yyleng;  return  FCOS; }
fsin               { token_type = INSTRUCTION2_OPCODE_FTZ; yycolno+=yyleng;  return  FSIN; }
flog2              { token_type = INSTRUCTION2_OPCODE_FTZ; yycolno+=yyleng;  return  FLOG2; }
fexp2              { token_type = INSTRUCTION2_OPCODE_FTZ; yycolno+=yyleng;  return  FEXP2; }
fsqrt              { token_type = INSTRUCTION2_OPCODE_FTZ; yycolno+=yyleng;  return  FSQRT; }
frsqrt             { token_type = INSTRUCTION2_OPCODE_FTZ; yycolno+=yyleng;  return  FRSQRT; }
frcp               { token_type = INSTRUCTION2_OPCODE_FTZ; yycolno+=yyleng;  return  FRCP; }

unpack3            { token_type = INSTRUCTION2_OPCODE_NODT; yycolno+=yyleng;  return  UNPACK3; }
unpack2            { token_type = INSTRUCTION2_OPCODE_NODT; yycolno+=yyleng;  return  UNPACK2; }
unpack1            { token_type = INSTRUCTION2_OPCODE_NODT; yycolno+=yyleng;  return  UNPACK1; }
unpack0            { token_type = INSTRUCTION2_OPCODE_NODT; yycolno+=yyleng;  return  UNPACK0; }
alloca             { token_type = INSTRUCTION2_OPCODE_NODT; yycolno+=yyleng;  return  ALLOCA;}
workitemid         { token_type = INSTRUCTION2_OPCODE_NODT; yycolno+=yyleng;  return  WORKITEMID; }
workitemaid        { token_type = INSTRUCTION2_OPCODE_NODT; yycolno+=yyleng;  return  WORKITEMAID;}
workgroupsize      { token_type = INSTRUCTION2_OPCODE_NODT; yycolno+=yyleng;  return  WORKGROUPSIZE;}
NDRangesize        { token_type = INSTRUCTION2_OPCODE_NODT; yycolno+=yyleng;  return  NDRANGESIZE;}
NDRangegroups      { token_type = INSTRUCTION2_OPCODE_NODT; yycolno+=yyleng;  return  NDRANGEGROUPS; }

add                { opcode = BrigAdd; token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  ADD;}
carry              { opcode = BrigCarry; token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  CARRY; }
borrow             { opcode = BrigBorrow; token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  BORROW; }
div                { token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  DIV; }
rem                { token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  REM; }
sub                { token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  SUB; }
shl                { token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  SHL;}
shr                { token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  SHR;}
and                { token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  AND;}
xor                { token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  XOR;}
or                 { token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  OR;}
unpacklo           { token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  UNPACKLO;}
unpackhi           { token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  UNPACKHI;}
movd_lo            { token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  MOVD_LO;}
movd_hi            { token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  MOVD_HI;}
copysign           { token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  COPYSIGN;}
class              { token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  CLASS;}
send               { token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  SEND;}
receive            { token_type = INSTRUCTION3_OPCODE; yycolno+=yyleng;  return  RECEIVE;}


max                { token_type = INSTRUCTION3_OPCODE_FTZ; yycolno+=yyleng;  return  MAX;}
min                { token_type = INSTRUCTION3_OPCODE_FTZ; yycolno+=yyleng;  return  MIN;}

mad                { token_type = INSTRUCTION4_OPCODE; yycolno+=yyleng;  return  MAD;}
extract            { token_type = INSTRUCTION4_OPCODE; yycolno+=yyleng;  return  EXTRACT;}
insert             { token_type = INSTRUCTION4_OPCODE; yycolno+=yyleng;  return  INSERT;}
shuffle            { token_type = INSTRUCTION4_OPCODE; yycolno+=yyleng;  return  SHUFFLE;}
cmov               { token_type = INSTRUCTION4_OPCODE; yycolno+=yyleng;  return  CMOV;}
fma                { token_type = INSTRUCTION4_OPCODE; yycolno+=yyleng;  return  FMA;}
bitalign           { token_type = INSTRUCTION4_OPCODE; yycolno+=yyleng;  return  BITALIGN;}
bytealign          { token_type = INSTRUCTION4_OPCODE; yycolno+=yyleng;  return  BYTEALIGN;}
lerp               { token_type = INSTRUCTION4_OPCODE; yycolno+=yyleng;  return  LERP;}
sad                { token_type = INSTRUCTION4_OPCODE; yycolno+=yyleng;  return  SAD;}
sad2               { token_type = INSTRUCTION4_OPCODE; yycolno+=yyleng;  return  SAD2;}
sad4               { token_type = INSTRUCTION4_OPCODE; yycolno+=yyleng;  return  SAD4;}
sad4hi             { token_type = INSTRUCTION4_OPCODE; yycolno+=yyleng;  return  SAD4HI;}
bitselect          { token_type = INSTRUCTION4_OPCODE; yycolno+=yyleng;  return  BITSELECT;}

cbr                {  token_type =BRANCH_OP; yycolno+=yyleng;  return CBR; }
brn                {  token_type =BRANCH_OP; yycolno+=yyleng;  return BRN; }
_fbar              {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return __FBAR; }
call               {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return CALL; }
  /*-- atomicOperationId --*/
_and               {  token_type = ATOMIC_OP; yycolno+=yyleng;  return  _AND_;}
_or                {  token_type = ATOMIC_OP; yycolno+=yyleng;  return  _OR_;}
_xor               {  token_type = ATOMIC_OP; yycolno+=yyleng;  return  _XOR_;}
_exch              {  token_type = ATOMIC_OP; yycolno+=yyleng;  return  _EXCH_;}
_add               {  token_type = ATOMIC_OP; yycolno+=yyleng;  return  _ADD_;}
_sub               {  token_type = ATOMIC_OP; yycolno+=yyleng;  return  _SUB_;}
_inc               {  token_type = ATOMIC_OP; yycolno+=yyleng;  return  _INC_;}
_dec               {  token_type = ATOMIC_OP; yycolno+=yyleng;  return  _DEC_;}
_max               {  token_type = ATOMIC_OP; yycolno+=yyleng;  return   _MAX_;}
_min               {  token_type = ATOMIC_OP; yycolno+=yyleng;  return   _MIN_;}

  /*-- comparison --*/
_eq                {  token_type = COMPARISON; yycolno+=yyleng;  return  _EQ;}
_ne                {  token_type = COMPARISON; yycolno+=yyleng;  return  _NE;}
_lt                {  token_type = COMPARISON; yycolno+=yyleng;  return  _LT;}
_le                {  token_type = COMPARISON; yycolno+=yyleng;  return  _LE;}
_gt                {  token_type = COMPARISON; yycolno+=yyleng;  return  _GT;}
_ge                {  token_type = COMPARISON; yycolno+=yyleng;  return  _GE;}
_equ               {  token_type = COMPARISON; yycolno+=yyleng;  return  _EQU;}
_neu               {  token_type = COMPARISON; yycolno+=yyleng;  return  _NEU;}
_ltu               {  token_type = COMPARISON; yycolno+=yyleng;  return  _LTU;}
_leu               {  token_type = COMPARISON; yycolno+=yyleng;  return  _LEU;}
_gtu               {  token_type = COMPARISON; yycolno+=yyleng;  return  _GTU;}
_geu               {  token_type = COMPARISON; yycolno+=yyleng;  return  _GEU;}
_num               {  token_type = COMPARISON; yycolno+=yyleng;  return  _NUM;}
_nan               {  token_type = COMPARISON; yycolno+=yyleng;  return  _NAN;}
_seq               {  token_type = COMPARISON; yycolno+=yyleng;  return  _SEQ;}
_sne               {  token_type = COMPARISON; yycolno+=yyleng;  return  _SNE;}
_slt               {  token_type = COMPARISON; yycolno+=yyleng;  return  _SLT;}
_sle               {  token_type = COMPARISON; yycolno+=yyleng;  return  _SLE;}
_sgt               {  token_type = COMPARISON; yycolno+=yyleng;  return  _SGT;}
_sge               {  token_type = COMPARISON; yycolno+=yyleng;  return  _SGE;}
_snum              {  token_type = COMPARISON; yycolno+=yyleng;  return  _SNUM;}
_snan              {  token_type = COMPARISON; yycolno+=yyleng;  return  _SNAN;}
_sequ              {  token_type = COMPARISON; yycolno+=yyleng;  return  _SEQU;}
_sneu              {  token_type = COMPARISON; yycolno+=yyleng;  return  _SNEU;}
_sltu              {  token_type = COMPARISON; yycolno+=yyleng;  return  _SLTU;}
_sleu              {  token_type = COMPARISON; yycolno+=yyleng;  return  _SLEU;}
_sgtu              {  token_type = COMPARISON; yycolno+=yyleng;  return  _SGTU;}
_sgeu              {  token_type = COMPARISON; yycolno+=yyleng;  return  _SGEU;}

  /*-- rounding --*/
_upi               { yycolno+=yyleng; token_type = INT_ROUNDING; return  _UPI;}
_downi             { yycolno+=yyleng; token_type = INT_ROUNDING;  return  _DOWNI;}
_zeroi             { yycolno+=yyleng; token_type = INT_ROUNDING;  return  _ZEROI;}
_neari             { yycolno+=yyleng; token_type = INT_ROUNDING;  return  _NEARI;}

_up                { yycolno+=yyleng; token_type = FLOAT_ROUNDING;  return  _UP;}
_down              { yycolno+=yyleng; token_type = FLOAT_ROUNDING;  return  _DOWN;}
_zero              { yycolno+=yyleng; token_type = FLOAT_ROUNDING;  return  _ZERO;}
_near              { yycolno+=yyleng; token_type = FLOAT_ROUNDING;  return  _NEAR;}

  /*-- packing -- */
_pp                { packing = BrigPackPP; token_type = PACKING; yycolno+=yyleng;  return  _PP;}
_ps                { packing = BrigPackPS; token_type = PACKING; yycolno+=yyleng;  return  _PS;}
_sp                { packing = BrigPackSP; token_type = PACKING; yycolno+=yyleng;  return  _SP;}
_ss                { packing = BrigPackSS; token_type = PACKING; yycolno+=yyleng;  return  _SS;}
_s                 { packing = BrigPackS; token_type = PACKING; yycolno+=yyleng;  return  __S;}
_p                 { packing = BrigPackP; token_type = PACKING; yycolno+=yyleng;  return  __P;}
_pp_sat            { packing = BrigPackPPsat; token_type = PACKING; yycolno+=yyleng;  return  _PP_SAT;}
_ps_sat            { packing = BrigPackPSsat; token_type = PACKING; yycolno+=yyleng;  return  _PS_SAT;}
_sp_sat            { packing = BrigPackSPsat; token_type = PACKING; yycolno+=yyleng;  return  _SP_SAT;}
_ss_sat            { packing = BrigPackSSsat; token_type = PACKING; yycolno+=yyleng;  return  _SS_SAT;}
_s_sat             { packing = BrigPackSsat; token_type = PACKING; yycolno+=yyleng;  return  _S_SAT;}
_p_sat             { packing = BrigPackPsat; token_type = PACKING; yycolno+=yyleng;  return  _P_SAT;}

  /*-- target --*/
\$small            { token_type = TARGET; yycolno+=yyleng; return  _SMALL;}
\$large            { token_type = TARGET; yycolno+=yyleng; return  _LARGE;}
\$full             { token_type = TARGET; yycolno+=yyleng;  return  _FULL;}
\$reduced          { token_type = TARGET; yycolno+=yyleng;  return  _REDUCED;}
\$sftz             { token_type = TARGET; yycolno+=yyleng;  return  _SFTZ;}
\$nosftz           { token_type = TARGET; yycolno+=yyleng;  return  _NOSFTZ;}

width              {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return WIDTH;}
height             {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return HEIGHT;}
depth              {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return DEPTH;}


  /*-- dataTypeId --*/
_u32               { token_type = DATA_TYPE_ID; data_type = Brigu32; yycolno+=yyleng;  return  _U32;}
_s32               { token_type = DATA_TYPE_ID; data_type = Brigs32; yycolno+=yyleng;  return  _S32;}
_s64               { token_type = DATA_TYPE_ID; data_type = Brigs64; yycolno+=yyleng;  return  _S64;}
_u64               { token_type = DATA_TYPE_ID; data_type = Brigu64; yycolno+=yyleng;  return  _U64;}
_b1                { token_type = DATA_TYPE_ID; data_type = Brigb1; yycolno+=yyleng;  return  _B1;}
_b32               { token_type = DATA_TYPE_ID; data_type = Brigb32; yycolno+=yyleng;  return  _B32;}
_f64               { token_type = DATA_TYPE_ID; data_type = Brigf64; yycolno+=yyleng;  return  _F64;}
_f32               { token_type = DATA_TYPE_ID; data_type = Brigf32; yycolno+=yyleng;  return  _F32;}
_b64               { token_type = DATA_TYPE_ID; data_type = Brigb64; yycolno+=yyleng;  return  _B64;}
_b8                { token_type = DATA_TYPE_ID; data_type = Brigb8; yycolno+=yyleng;  return  _B8;}
_b16               { token_type = DATA_TYPE_ID; data_type = Brigb16; yycolno+=yyleng;  return  _B16;}
_s8                { token_type = DATA_TYPE_ID; data_type = Brigs8; yycolno+=yyleng;  return  _S8;}
_s16               { token_type = DATA_TYPE_ID; data_type = Brigs16; yycolno+=yyleng;  return  _S16;}
_u8                { token_type = DATA_TYPE_ID; data_type = Brigu8; yycolno+=yyleng;  return  _U8;}
_u16               { token_type = DATA_TYPE_ID; data_type = Brigu16; yycolno+=yyleng;  return  _U16;}
_f16               { token_type = DATA_TYPE_ID; data_type = Brigf16; yycolno+=yyleng;  return  _F16;}
_b128              { token_type = DATA_TYPE_ID; data_type = Brigb128; yycolno+=yyleng;  return  _B128;}
_u8x4              { token_type = DATA_TYPE_ID; data_type = Brigu8x4; yycolno+=yyleng;  return  _U8X4;}
_s8x4              { token_type = DATA_TYPE_ID; data_type = Brigs8x4; yycolno+=yyleng;  return  _S8X4;}
_u16x2             { token_type = DATA_TYPE_ID; data_type = Brigu16x2; yycolno+=yyleng;  return  _U16X2;}
_s16x2             { token_type = DATA_TYPE_ID; data_type = Brigs16x2; yycolno+=yyleng;  return  _S16X2;}
_f16x2             { token_type = DATA_TYPE_ID; data_type = Brigf16x2; yycolno+=yyleng;  return  _F16X2;}
_f32x2             { token_type = DATA_TYPE_ID; data_type = Brigf32x2; yycolno+=yyleng;  return  _F32X2;}
_u8x8              { token_type = DATA_TYPE_ID; data_type = Brigu8x8; yycolno+=yyleng;  return  _U8X8;}
_s8x8              { token_type = DATA_TYPE_ID; data_type = Brigs8x8; yycolno+=yyleng;  return  _S8X8;}
_u16x4             { token_type = DATA_TYPE_ID; data_type = Brigu16x4; yycolno+=yyleng;  return  _U16X4;}
_s16x4             { token_type = DATA_TYPE_ID; data_type = Brigs16x4; yycolno+=yyleng;  return  _S16X4;}
_f16x4             { token_type = DATA_TYPE_ID; data_type = Brigf16x4; yycolno+=yyleng;  return  _F16X4;}
_u8x16             { token_type = DATA_TYPE_ID; data_type = Brigu8x16; yycolno+=yyleng;  return  _U8X16;}
_s8x16             { token_type = DATA_TYPE_ID; data_type = Brigs8x16; yycolno+=yyleng;  return  _S8X16;}
_u16x8             { token_type = DATA_TYPE_ID; data_type = Brigu16x8; yycolno+=yyleng;  return  _U16X8;}
_s16x8             { token_type = DATA_TYPE_ID; data_type = Brigs16x8; yycolno+=yyleng;  return  _S16X8;}
_f16x8             { token_type = DATA_TYPE_ID; data_type = Brigf16x8; yycolno+=yyleng;  return  _F16X8;}
_f32x4             { token_type = DATA_TYPE_ID; data_type = Brigf32x4; yycolno+=yyleng;  return  _F32X4;}
_s32x4             { token_type = DATA_TYPE_ID; data_type = Brigs32x4; yycolno+=yyleng;  return  _S32X4;}
_u32x4             { token_type = DATA_TYPE_ID; data_type = Brigu32x4; yycolno+=yyleng;  return  _U32X4;}
_f64x2             { token_type = DATA_TYPE_ID; data_type = Brigf64x2; yycolno+=yyleng;  return  _F64X2;}
_s64x2             { token_type = DATA_TYPE_ID; data_type = Brigs64x2; yycolno+=yyleng;  return  _S64X2;}
_u64x2             { token_type = DATA_TYPE_ID; data_type = Brigu64x2; yycolno+=yyleng;  return  _U64X2;}


_ftz               {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return _FTZ;}
nop                {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return NOP;}
clock              {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return CLOCK;}
syscall            {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return SYSCALL;}


  /*-- mul --*/
mul               {  token_type = MUL_OP; yycolno+=yyleng;  return MUL;}
mul_hi            {  token_type = MUL_OP; yycolno+=yyleng;  return MUL_HI;}
mul24_hi          {  token_type = MUL_OP; yycolno+=yyleng;  return MUL24_HI;}
mul24             {  token_type = MUL_OP; yycolno+=yyleng;  return MUL24;}
mad24             {  token_type = MUL_OP; yycolno+=yyleng;  return MAD24;}
mad24_hi          {  token_type = MUL_OP; yycolno+=yyleng;  return MAD24_HI;}

f2u4              { token_type = INSTRUCTION5_OPCODE; yycolno+=yyleng;  return F2U4;}

  /*-- Atom Modifiers --*/
_ar               {  token_type = ATOM_MODIFIER; yycolno+=yyleng;  return _AR;}
_region           {  token_type = ATOM_MODIFIER; yycolno+=yyleng;  return _REGION;}
atomic_cas        {  token_type = ATOM_MODIFIER; yycolno+=yyleng;  return ATOMIC_CAS;}
atomic            {  token_type = ATOM_MODIFIER; yycolno+=yyleng;  return ATOMIC;}
atomicNoRet       {  token_type = ATOM_MODIFIER; yycolno+=yyleng;  return ATOMICNORET;}
atomicNoRet_cas   {  token_type = ATOM_MODIFIER; yycolno+=yyleng;  return ATOMICNORET_CAS;}
atomic_image      {  token_type = ATOM_MODIFIER; yycolno+=yyleng;  return ATOMIC_IMAGE;}
atomicNoRet_image {  token_type = ATOM_MODIFIER; yycolno+=yyleng;  return ATOMICNORET_IMAGE;}

cvt               { token_type = COMMON_KEYWORD; yycolno+=yyleng;  return CVT;}


_dep              {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return _DEP;}
_equiv            {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return _EQUIV;}
_acq              {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return _ACQ;}
_rel              {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return _REL;}
ld                {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return LD;}
sync              {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return SYNC;}
barrier           {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return BARRIER;}
segmentp          {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return SEGMENTP;}
ftos              {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return FTOS;}
stof              {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return STOF; }
mov               {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return MOV;}
lad               {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return LAD;}
ldc               {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return LDC;}
ret               {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return RET;}
packedcmp         {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return PACKEDCMP;}
cmp               {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return CMP;}
st                {  token_type = COMMON_KEYWORD; yycolno+=yyleng;  return ST;}



  /* -- geometry ID -- */
_1d               {  token_type = GEOMETRY_ID; yycolno+=yyleng;  return _1D;}
_2d               {  token_type = GEOMETRY_ID; yycolno+=yyleng;  return _2D;}
_3d               {  token_type = GEOMETRY_ID; yycolno+=yyleng;  return _3D;}
_1db              {  token_type = GEOMETRY_ID; yycolno+=yyleng;  return _1DB;}
_1da              {  token_type = GEOMETRY_ID; yycolno+=yyleng;  return _1DA;}
_2da              {  token_type = GEOMETRY_ID; yycolno+=yyleng;  return _2DA;}

rd_image          {  token_type = IMAGE_OP; yycolno+=yyleng;  return RD_IMAGE;}
ld_image          {  token_type = IMAGE_OP; yycolno+=yyleng;  return LD_IMAGE;}
st_image          {  token_type = IMAGE_OP; yycolno+=yyleng;  return ST_IMAGE;}




  /* -- queryOp --*/
query_order      { token_type = QUERY_OP; yycolno+=yyleng;  return QUERY_ORDER;}
query_data       { token_type = QUERY_OP; yycolno+=yyleng;  return QUERY_DATA;}
query_array      { token_type = QUERY_OP; yycolno+=yyleng;  return QUERY_ARRAY;}
query_width      { token_type = QUERY_OP; yycolno+=yyleng;  return QUERY_WIDTH;}
query_depth      { token_type = QUERY_OP; yycolno+=yyleng;  return QUERY_DEPTH;}
query_height     { token_type = QUERY_OP; yycolno+=yyleng;  return QUERY_HEIGHT;}
query_normalized { token_type = QUERY_OP; yycolno+=yyleng;  return QUERY_NORMALIZED;}
query_filtering  { token_type = QUERY_OP; yycolno+=yyleng;  return QUERY_FILTERING;}



.  {fprintf(stderr, "unexpected char"); return 0;}
