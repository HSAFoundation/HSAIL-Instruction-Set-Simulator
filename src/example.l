/* Copyright 2012 <MulticorewareInc> */

%option noyywrap yylineno
%{
#include <stdio.h>
#include "tokens.h"


int     int_val;
float   float_val;
double  double_val;
char*   string_val;
int     yycolno;

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
","           { yycolno+=yyleng; return yytext[0];}

{OCT_INT}     { yycolno+=yyleng; int_val = strtol(yytext, NULL, 8); return TOKEN_INTEGER_CONSTANT; }
{DEC_INT}     { yycolno+=yyleng; int_val = strtol(yytext, NULL, 10); return TOKEN_INTEGER_CONSTANT; }
{HEX_INT}     { yycolno+=yyleng; int_val = strtol(yytext, NULL, 16); return TOKEN_INTEGER_CONSTANT; }

{DEC_SINGLE}  { yycolno+=yyleng; float_val = strtof(yytext,NULL); return TOKEN_SINGLE_CONSTANT; }
{HEX_SINGLE}  { yycolno+=yyleng; float_val = strtof(yytext,NULL); return TOKEN_SINGLE_CONSTANT; }
{IEEE_SINGLE} { yycolno+=yyleng; float_val = ieee754tof(yytext); return TOKEN_SINGLE_CONSTANT; }

{DEC_DOUBLE}  { yycolno+=yyleng; double_val = strtod(yytext,NULL); return TOKEN_DOUBLE_CONSTANT; }
{HEX_DOUBLE}  { yycolno+=yyleng; double_val = strtod(yytext,NULL); return TOKEN_DOUBLE_CONSTANT; }
{IEEE_DOUBLE} { yycolno+=yyleng; double_val = ieee754tod(yytext); return TOKEN_DOUBLE_CONSTANT; }



"/*"                          { BEGIN (COMMENT); }
<COMMENT>"*/"                 { BEGIN (INITIAL); return TOKEN_COMMENT; }
<COMMENT>([^*]|\n)+|./"*/"    {}
<COMMENT><<EOF>>              { printf("%d: Unterminated comment\n", yylineno); return 0; }

"//"                          { BEGIN(INLINE_COMMENT); }
<INLINE_COMMENT>.             {}
<INLINE_COMMENT>\n            { BEGIN(INITIAL); return TOKEN_COMMENT;}
<INLINE_COMMENT><<EOF>>       { printf("%d: Unterminated comment\n", yylineno); return 0; }


\$c(0|1|2|3|4|5|6|7)                        { yycolno+=yyleng; string_val = strdup(yytext);  return TOKEN_CREGISTER;}
\$d(0|1|2|3|4|5|6|7)                        { yycolno+=yyleng; string_val = strdup(yytext);  return TOKEN_DREGISTER;}
\$s(0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15)  { yycolno+=yyleng; string_val = strdup(yytext);  return TOKEN_SREGISTER;}
\$q(0|1|2|3|4|5|6|7)                        { yycolno+=yyleng; string_val = strdup(yytext);  return TOKEN_QREGISTER;}

\%[_a-zA-Z]+[_a-zA-Z0-9]*      { yycolno+=yyleng; string_val = strdup(yytext); return TOKEN_LOCAL_IDENTIFIER; }
\&[_a-zA-Z]+[_a-zA-Z0-9]*      { yycolno+=yyleng; string_val = strdup(yytext); return TOKEN_GLOBAL_IDENTIFIER; }

\@[_a-zA-Z]+[_a-zA-Z0-9]*      { yycolno+=yyleng; string_val = strdup(yytext);  return TOKEN_LABEL; }
\"(\\.|[^\\"])*\"              { yycolno+=yyleng; string_val = strdup(yytext);  return TOKEN_STRING; }

((snorm_|unorm_)(((int)(8|16|32))|((short_)(565|555|101010)))) |
((signed_|unsigned_)(int)(8|16|32)) |
((half_)?(float))                                                          { yycolno+=yyleng; return TOKEN_PROPERTY; }
(r|rx|a|rg|rgx|ra|rgb|rgbx|ra|rgb|rgbx|rgba|bgra|argb|intensity|luminance) { yycolno+=yyleng;return TOKEN_PROPERTY; }
(normalized|unnormalized)                                                  { yycolno+=yyleng;  return TOKEN_PROPERTY; }
(nearest|linear)                                                           { yycolno+=yyleng;  return TOKEN_PROPERTY; }
(wrap|clamp|mirror|mirroronce|border)                                      { yycolno+=yyleng; return TOKEN_PROPERTY;}

WAVESIZE      { yycolno+=yyleng; return TOKEN_WAVESIZE; }

  /* keywords */
workgroupid      { yycolno+=yyleng;  return WORKGROUPID;}
version          { yycolno+=yyleng;  return VERSION; }
global           { yycolno+=yyleng;  return GLOBAL; }
_Samp            { yycolno+=yyleng;  return _SAMP; }
_RWImg           { yycolno+=yyleng;  return _RWIMG; }
_ROImg           { yycolno+=yyleng;  return _ROIMG; }
align            { yycolno+=yyleng;  return ALIGN; }
file             { yycolno+=yyleng;  return _FILE; }
arg              { yycolno+=yyleng;  return ARG; }
kernarg          { yycolno+=yyleng;  return KERNARG; }
function         { yycolno+=yyleng;  return FUNCTION; }
:fbar            { yycolno+=yyleng;  return _FBAR; }
signature        { yycolno+=yyleng;  return SIGNATURE; }
block            { yycolno+=yyleng;  return BLOCK; }
endblock         { yycolno+=yyleng;  return ENDBLOCK; }
blocknumeric     { yycolno+=yyleng;  return BLOCKNUMERIC; }
blockstring      { yycolno+=yyleng;  return BLOCKSTRING; }
kernel           { yycolno+=yyleng;  return KERNEL; }
pragma           { yycolno+=yyleng;  return PRAGMA; }
labeltargets     { yycolno+=yyleng;  return LABELTARGETS; }
extension        { yycolno+=yyleng;  return EXTENSION; }
extern           { yycolno+=yyleng;  return EXTERN; }
static           { yycolno+=yyleng;  return STATIC; }
const            { yycolno+=yyleng;  return CONST; }
private          { yycolno+=yyleng;  return PRIVATE; }
spill            { yycolno+=yyleng;  return SPILL; }
group            { yycolno+=yyleng;  return GROUP; }
readonly         { yycolno+=yyleng;  return READONLY; }
loc              { yycolno+=yyleng;  return LOC; }
_width           { yycolno+=yyleng;  return _WIDTH; }
all              { yycolno+=yyleng;  return ALL; }

  /* -- addressSpaceIdentifier -- */
_readonly        { yycolno+=yyleng;  return  _READONLY; }
_kernarg         { yycolno+=yyleng;  return  _KERNARG;   }
_global          { yycolno+=yyleng;  return  _GLOBAL; }
_private         { yycolno+=yyleng;  return  _PRIVATE; }
_arg             { yycolno+=yyleng;  return  _ARG;   }
_group           { yycolno+=yyleng;  return  _GROUP; }
_spill           { yycolno+=yyleng;  return  _SPILL; }

  /* -- vector -- */
_v2              { yycolno+=yyleng;  return _V2; }
_v4              { yycolno+=yyleng;  return _V4; }


  /* -- init -- */
format           { yycolno+=yyleng;  return  FORMAT; }
order            { yycolno+=yyleng;  return  ORDER; }
coord            { yycolno+=yyleng;  return  COORD; }
filter           { yycolno+=yyleng;  return  FILTER; }
boundaryU        { yycolno+=yyleng;  return  BOUNDARYU; }
boundaryV        { yycolno+=yyleng;  return  BOUNDARYV; }
boundaryW        { yycolno+=yyleng;  return  BOUNDARYW; }

  /* -- control -- */
itemsperworkgroup  { yycolno+=yyleng;  return  ITEMS_PER_WORKGROUP; }
workgroupspercu    { yycolno+=yyleng;  return  WORKGROUPS_PER_CU; }
memopt_on          { yycolno+=yyleng;  return  MEMOPT_ON; }
memopt_off         { yycolno+=yyleng;  return  MEMOPT_OFF; }

  /* -- Opcode -- */
fbar_initSizeWg    { yycolno+=yyleng;  return  FBAR_INITSIZEWG; }
fbar_wait          { yycolno+=yyleng;  return  FBAR_WAIT; }
fbar_arrive        { yycolno+=yyleng;  return  FBAR_ARRIVE; }
fbar_skip          { yycolno+=yyleng;  return  FBAR_SKIP; }
fbar_release       { yycolno+=yyleng;  return  FBAR_RELEASE; }
countup            { yycolno+=yyleng;  return  COUNTUP; }

laneid             { yycolno+=yyleng;  return  LANEID; }
dynwaveid          { yycolno+=yyleng;  return  DYNWAVEID; }
maxdynwaveid       { yycolno+=yyleng;  return  MAXDYNWAVEID; }
dispatchid         { yycolno+=yyleng;  return  DISPATCHID; }
cu                 { yycolno+=yyleng;  return  CU; }
workdim            { yycolno+=yyleng;  return  WORKDIM; }
workitemid_flat    { yycolno+=yyleng;  return  WORKITEMID_FLAT; }
workitemaid_flat   { yycolno+=yyleng;  return  WORKITEMAID_FLAT; }
debugtrap          { yycolno+=yyleng;  return  DEBUGTRAP; }

abs                { yycolno+=yyleng;  return  ABS; }
neg                { yycolno+=yyleng;  return  NEG; }
not                { yycolno+=yyleng;  return  NOT; }
popcount           { yycolno+=yyleng;  return  POPCOUNT; }
firstbit           { yycolno+=yyleng;  return  FIRSTBIT; }
lastbit            { yycolno+=yyleng;  return  LASTBIT; }
bitrev             { yycolno+=yyleng;  return  BITREV; }
movs_lo            { yycolno+=yyleng;  return  MOVS_LO; }
movs_hi            { yycolno+=yyleng;  return  MOVS_HI; }
fbar_initSize      { yycolno+=yyleng;  return  FBAR_INITSIZE; }
fbar_init          { yycolno+=yyleng;  return  FBAR_INIT; }
fbar_releaseCF     { yycolno+=yyleng;  return  FBAR_RELEASECF; }
count              { yycolno+=yyleng;  return  COUNT; }
mask               { yycolno+=yyleng;  return  MASK; }

sqrt               { yycolno+=yyleng;  return  SQRT; }
fract              { yycolno+=yyleng;  return  FRACT; }
fcos               { yycolno+=yyleng;  return  FCOS; }
fsin               { yycolno+=yyleng;  return  FSIN; }
flog2              { yycolno+=yyleng;  return  FLOG2; }
fexp2              { yycolno+=yyleng;  return  FEXP2; }
fsqrt              { yycolno+=yyleng;  return  FSQRT; }
frsqrt             { yycolno+=yyleng;  return  FRSQRT; }
frcp               { yycolno+=yyleng;  return  FRCP; }

unpack3            { yycolno+=yyleng;  return  UNPACK3; }
unpack2            { yycolno+=yyleng;  return  UNPACK2; }
unpack1            { yycolno+=yyleng;  return  UNPACK1; }
unpack0            { yycolno+=yyleng;  return  UNPACK0; }
alloca             { yycolno+=yyleng;  return  ALLOCA;}
workitemid         { yycolno+=yyleng;  return  WORKITEMID; }
workitemaid        { yycolno+=yyleng;  return  WORKITEMAID;}
workgroupsize      { yycolno+=yyleng;  return  WORKGROUPSIZE;}
NDRangesize        { yycolno+=yyleng;  return  NDRANGESIZE;}
NDRangegroups      { yycolno+=yyleng;  return  NDRANGEGROUPS; }

add                { yycolno+=yyleng;  return  ADD;}
carry              { yycolno+=yyleng;  return  CARRY; }
borrow             { yycolno+=yyleng;  return  BORROW; }
div                { yycolno+=yyleng;  return  DIV; }
rem                { yycolno+=yyleng;  return  REM; }
sub                { yycolno+=yyleng;  return  SUB; }
shl                { yycolno+=yyleng;  return  SHL;}
shr                { yycolno+=yyleng;  return  SHR;}
and                { yycolno+=yyleng;  return  AND;}
xor                { yycolno+=yyleng;  return  XOR;}
or                 { yycolno+=yyleng;  return  OR;}
unpacklo           { yycolno+=yyleng;  return  UNPACKLO;}
unpackhi           { yycolno+=yyleng;  return  UNPACKHI;}
movd_lo            { yycolno+=yyleng;  return  MOVD_LO;}
movd_hi            { yycolno+=yyleng;  return  MOVD_HI;}
copysign           { yycolno+=yyleng;  return  COPYSIGN;}
class              { yycolno+=yyleng;  return  CLASS;}
send               { yycolno+=yyleng;  return  SEND;}
receive            { yycolno+=yyleng;  return  RECEIVE;}


max                { yycolno+=yyleng;  return  MAX;}
min                { yycolno+=yyleng;  return  MIN;}

mad                { yycolno+=yyleng;  return  MAD;}
extract            { yycolno+=yyleng;  return  EXTRACT;}
insert             { yycolno+=yyleng;  return  INSERT;}
shuffle            { yycolno+=yyleng;  return  SHUFFLE;}
cmov               { yycolno+=yyleng;  return  CMOV;}
fma                { yycolno+=yyleng;  return  FMA;}
bitalign           { yycolno+=yyleng;  return  BITALIGN;}
bytealign          { yycolno+=yyleng;  return  BYTEALIGN;}
lerp               { yycolno+=yyleng;  return  LERP;}
sad                { yycolno+=yyleng;  return  SAD;}
sad2               { yycolno+=yyleng;  return  SAD2;}
sad4               { yycolno+=yyleng;  return  SAD4;}
sad4hi             { yycolno+=yyleng;  return  SAD4HI;}
bitselect          { yycolno+=yyleng;  return  BITSELECT;}
cbr                { yycolno+=yyleng;  return CBR; }
brn                { yycolno+=yyleng;  return BRN; }
_fbar              { yycolno+=yyleng;  return __FBAR; }
call               { yycolno+=yyleng;  return CALL; }
  /*-- atomicOperationId --*/
_and               { yycolno+=yyleng;  return  _AND_;}
_or                { yycolno+=yyleng;  return  _OR_;}
_xor               { yycolno+=yyleng;  return  _XOR_;}
_exch              { yycolno+=yyleng;  return  _EXCH_;}
_add               { yycolno+=yyleng;  return  _ADD_;}
_sub               { yycolno+=yyleng;  return  _SUB_;}
_inc               { yycolno+=yyleng;  return  _INC_;}
_dec               { yycolno+=yyleng;  return  _DEC_;}
_max               { yycolno+=yyleng;  return   _MAX_;}
_min               { yycolno+=yyleng;  return   _MIN_;}

  /*-- comparison --*/
_eq                { yycolno+=yyleng;  return  _EQ;}
_ne                { yycolno+=yyleng;  return  _NE;}
_lt                { yycolno+=yyleng;  return  _LT;}
_le                { yycolno+=yyleng;  return  _LE;}
_gt                { yycolno+=yyleng;  return  _GT;}
_ge                { yycolno+=yyleng;  return  _GE;}
_equ               { yycolno+=yyleng;  return  _EQU;}
_neu               { yycolno+=yyleng;  return  _NEU;}
_ltu               { yycolno+=yyleng;  return  _LTU;}
_leu               { yycolno+=yyleng;  return  _LEU;}
_gtu               { yycolno+=yyleng;  return  _GTU;}
_geu               { yycolno+=yyleng;  return  _GEU;}
_num               { yycolno+=yyleng;  return  _NUM;}
_nan               { yycolno+=yyleng;  return  _NAN;}
_seq               { yycolno+=yyleng;  return  _SEQ;}
_sne               { yycolno+=yyleng;  return  _SNE;}
_slt               { yycolno+=yyleng;  return  _SLT;}
_sle               { yycolno+=yyleng;  return  _SLE;}
_sgt               { yycolno+=yyleng;  return  _SGT;}
_sge               { yycolno+=yyleng;  return  _SGE;}
_snum              { yycolno+=yyleng;  return  _SNUM;}
_snan              { yycolno+=yyleng;  return  _SNAN;}
_sequ              { yycolno+=yyleng;  return  _SEQU;}
_sneu              { yycolno+=yyleng;  return  _SNEU;}
_sltu              { yycolno+=yyleng;  return  _SLTU;}
_sleu              { yycolno+=yyleng;  return  _SLEU;}
_sgtu              { yycolno+=yyleng;  return  _SGTU;}
_sgeu              { yycolno+=yyleng;  return  _SGEU;}

  /*-- rounding --*/
_upi               { yycolno+=yyleng;  return  _UPI;}
_downi             { yycolno+=yyleng;  return  _DOWNI;}
_zeroi             { yycolno+=yyleng;  return  _ZEROI;}
_neari             { yycolno+=yyleng;  return  _NEARI;}

_up                { yycolno+=yyleng;  return  _UP;}
_down              { yycolno+=yyleng;  return  _DOWN;}
_zero              { yycolno+=yyleng;  return  _ZERO;}
_near              { yycolno+=yyleng;  return  _NEAR;}

  /*-- packing -- */
_pp                { yycolno+=yyleng;  return  _PP;}
_ps                { yycolno+=yyleng;  return  _PS;}
_sp                { yycolno+=yyleng;  return  _SP;}
_ss                { yycolno+=yyleng;  return  _SS;}
_s                 { yycolno+=yyleng;  return  __S;}
_p                 { yycolno+=yyleng;  return  __P;}
_pp_sat            { yycolno+=yyleng;  return  _PP_SAT;}
_ps_sat            { yycolno+=yyleng;  return  _PS_SAT;}
_sp_sat            { yycolno+=yyleng;  return  _SP_SAT;}
_ss_sat            { yycolno+=yyleng;  return  _SS_SAT;}
_s_sat             { yycolno+=yyleng;  return  _S_SAT;}
_p_sat             { yycolno+=yyleng;  return  _P_SAT;}

  /*-- target --*/
\$small            { yycolno+=yyleng; return  _SMALL;}
\$large            { yycolno+=yyleng; return  _LARGE;}
\$full             { yycolno+=yyleng;  return  _FULL;}
\$reduced          { yycolno+=yyleng;  return  _REDUCED;}
\$sftz             { yycolno+=yyleng;  return  _SFTZ;}
\$nosftz           { yycolno+=yyleng;  return  _NOSFTZ;}

width              { yycolno+=yyleng;  return WIDTH;}
height             { yycolno+=yyleng;  return HEIGHT;}
depth              { yycolno+=yyleng;  return DEPTH;}


  /*-- dataTypeId --*/
_u32               { yycolno+=yyleng;  return  _U32;}
_s32               { yycolno+=yyleng;  return  _S32;}
_s64               { yycolno+=yyleng;  return  _S64;}
_u64               { yycolno+=yyleng;  return  _U64;}
_b1                { yycolno+=yyleng;  return  _B1;}
_b32               { yycolno+=yyleng;  return  _B32;}
_f64               { yycolno+=yyleng;  return  _F64;}
_f32               { yycolno+=yyleng;  return  _F32;}
_b64               { yycolno+=yyleng;  return  _B64;}
_b8                { yycolno+=yyleng;  return  _B8;}
_b16               { yycolno+=yyleng;  return  _B16;}
_s8                { yycolno+=yyleng;  return  _S8;}
_s16               { yycolno+=yyleng;  return  _S16;}
_u8                { yycolno+=yyleng;  return  _U8;}
_u16               { yycolno+=yyleng;  return  _U16;}
_f16               { yycolno+=yyleng;  return  _F16;}
_b128              { yycolno+=yyleng;  return  _B128;}
_u8x4              { yycolno+=yyleng;  return  _U8X4;}
_s8x4              { yycolno+=yyleng;  return  _S8X4;}
_u16x2             { yycolno+=yyleng;  return  _U16X2;}
_s16x2             { yycolno+=yyleng;  return  _S16X2;}
_f16x2             { yycolno+=yyleng;  return  _F16X2;}
_f32x2             { yycolno+=yyleng;  return  _F32X2;}
_u8x8              { yycolno+=yyleng;  return  _U8X8;}
_s8x8              { yycolno+=yyleng;  return  _S8X8;}
_u16x4             { yycolno+=yyleng;  return  _U16X4;}
_s16x4             { yycolno+=yyleng;  return  _S16X4;}
_f16x4             { yycolno+=yyleng;  return  _F16X4;}
_u8x16             { yycolno+=yyleng;  return  _U8X16;}
_s8x16             { yycolno+=yyleng;  return  _S8X16;}
_u16x8             { yycolno+=yyleng;  return  _U16X8;}
_s16x8             { yycolno+=yyleng;  return  _S16X8;}
_f16x8             { yycolno+=yyleng;  return  _F16X8;}
_f32x4             { yycolno+=yyleng;  return  _F32X4;}
_s32x4             { yycolno+=yyleng;  return  _S32X4;}
_u32x4             { yycolno+=yyleng;  return  _U32X4;}
_f64x2             { yycolno+=yyleng;  return  _F64X2;}
_s64x2             { yycolno+=yyleng;  return  _S64X2;}
_u64x2             { yycolno+=yyleng;  return  _U64X2;}


_ftz               { yycolno+=yyleng;  return _FTZ;}
nop                { yycolno+=yyleng;  return NOP;}
clock              { yycolno+=yyleng;  return CLOCK;}
syscall            { yycolno+=yyleng;  return SYSCALL;}


  /*-- mul --*/
mul               { yycolno+=yyleng;  return MUL;}
mul_hi            { yycolno+=yyleng;  return MUL_HI;}
mul24_hi          { yycolno+=yyleng;  return MUL24_HI;}
mul24             { yycolno+=yyleng;  return MUL24;}
mad24             { yycolno+=yyleng;  return MAD24;}
mad24_hi          { yycolno+=yyleng;  return MAD24_HI;}

f2u4              { yycolno+=yyleng;  return F2U4;}

  /*-- Atom Modifiers --*/
_ar               { yycolno+=yyleng;  return _AR;}
_region           { yycolno+=yyleng;  return _REGION;}
atomic_cas        { yycolno+=yyleng;  return ATOMIC_CAS;}
atomic            { yycolno+=yyleng;  return ATOMIC;}
atomicNoRet       { yycolno+=yyleng;  return ATOMICNORET;}
atomicNoRet_cas   { yycolno+=yyleng;  return ATOMICNORET_CAS;}
atomic_image      { yycolno+=yyleng;  return ATOMIC_IMAGE;}
atomicNoRet_image { yycolno+=yyleng;  return ATOMICNORET_IMAGE;}

cvt               { yycolno+=yyleng;  return CVT;}


_dep              { yycolno+=yyleng;  return _DEP;}
_equiv            { yycolno+=yyleng;  return _EQUIV;}
_acq              { yycolno+=yyleng;  return _ACQ;}
_rel              { yycolno+=yyleng;  return _REL;}
ld                { yycolno+=yyleng;  return LD;}
sync              { yycolno+=yyleng;  return SYNC;}
barrier           { yycolno+=yyleng;  return BARRIER;}
segmentp          { yycolno+=yyleng;  return SEGMENTP;}
ftos              { yycolno+=yyleng;  return FTOS;}
stof              { yycolno+=yyleng;  return STOF; }
mov               { yycolno+=yyleng;  return MOV;}
lad               { yycolno+=yyleng;  return LAD;}
ldc               { yycolno+=yyleng;  return LDC;}
ret               { yycolno+=yyleng;  return RET;}
packedcmp         { yycolno+=yyleng;  return PACKEDCMP;}
cmp               { yycolno+=yyleng;  return CMP;}
st                { yycolno+=yyleng;  return ST;}



  /* -- geometry ID -- */
_1d               { yycolno+=yyleng;  return _1D;}
_2d               { yycolno+=yyleng;  return _2D;}
_3d               { yycolno+=yyleng;  return _3D;}
_1db              { yycolno+=yyleng;  return _1DB;}
_1da              { yycolno+=yyleng;  return _1DA;}
_2da              { yycolno+=yyleng;  return _2DA;}

rd_image          { yycolno+=yyleng;  return RD_IMAGE;}
ld_image          { yycolno+=yyleng;  return LD_IMAGE;}
st_image          { yycolno+=yyleng;  return ST_IMAGE;}




  /* -- queryOp --*/
query_order      { yycolno+=yyleng;  return QUERY_ORDER;}
query_data       { yycolno+=yyleng;  return QUERY_DATA;}
query_array      { yycolno+=yyleng;  return QUERY_ARRAY;}
query_width      { yycolno+=yyleng;  return QUERY_WIDTH;}
query_depth      { yycolno+=yyleng;  return QUERY_DEPTH;}
query_height     { yycolno+=yyleng;  return QUERY_HEIGHT;}
query_normalized { yycolno+=yyleng;  return QUERY_NORMALIZED;}
query_filtering  { yycolno+=yyleng;  return QUERY_FILTERING;}



.  {fprintf(stderr, "unexpected char"); return 0;}

