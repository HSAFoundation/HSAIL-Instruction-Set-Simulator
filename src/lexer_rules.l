/* Copyright 2012 <MulticorewareInc> */

%option noyywrap yylineno
%option nounput
%{
#include <stdio.h>
#include "tokens.h"
#include "brig.h"
#include "context.h"
#include "error_reporter.h"

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
  char first4[5];    // first 2 bytes(4 chars after 0d)
  char temp_str[5];  // temporary string to hold part of yytext
  unsigned int i;
  unsigned int first_int;    // convert the double as 2 integers
  unsigned int second_int;
  unsigned int temp = 0;

  // convert the first 2 bytes
  for (i=0; i<4; i++)
    first4[i] = text[i+2];

  first4[4] = '\0';
  first_int = strtol(first4, NULL, 16);

  // get the next 2 bytes;
  for (i=0;i<4;i++)
    temp_str[i] = text[i+6];
  temp_str[4] = '\0';
  temp = strtol(temp_str,NULL,16);
  first_int <<= 16;

  temp += first_int;

  int_ptr = (unsigned int*)&return_val;

  memcpy(int_ptr+1,&temp, sizeof(float));
  // printf("Return context->token_value after update first 4 bytes: %2.16f\n",return_val);


  // for the last 4bytes, do the same thing, to avoid treating the first bit as the SIGN.
  for (i=0;i<4;i++)
    first4[i] = text[i+10];

  first4[4] = '\0';
  second_int = strtol(first4, NULL, 16);

  // get the last 2 bytes;
  for (i=0;i<4;i++)
    temp_str[i] = text[i+14];
  temp_str[4] = '\0';
  temp = strtol(temp_str,NULL,16);
  second_int <<= 16;
  temp += second_int;

  memcpy(int_ptr,&temp, sizeof(int));

  //printf("Final return context->token_value: %2.14f\n",return_val);
  return return_val;
}

void free_string(hsa::brig::Context* context) {
  if (context->valid_string) {
    free(context->token_value.string_val);
    context->valid_string = false;
  }
}

%}

%x INLINE_COMMENT
%x COMMENT

OCT_INT     (0[0-7]+)
DEC_INT     (0|([1-9]+[0-9]*))
HEX_INT     (0(x|X)[0-9a-fA-F]+)

DEC_DOUBLE  ([0-9]*((("."[0-9]*)(l|L)?)|([0-9]+(l|L))|(("."[0-9]*)?((e|E)[+|-]?[0-9]+)(l|L)?)))
HEX_DOUBLE  (0(x|X)[0-9a-fA-F]+(("."[0-9a-fA-F]*)((p|P)[+|-]?[0-9]*)?)(l|L)?)
IEEE_DOUBLE (0(d|D)[0-9a-fA-F]{16})

DEC_SINGLE  ([0-9]*(("."[0-9]*)|([0-9]+)|(("."[0-9]*)?((e|E)[+|-]?[0-9]+)))(f|F))
HEX_SINGLE  (0(x|X)[0-9a-fA-F]+(("."[0-9a-fA-F]*)((p|P)[+|-]?[0-9]*)?)(f|F))
IEEE_SINGLE (0(f|F)[0-9a-fA-F]{8})

%%

\n                /* ignore EOL */ context->yycolno = 0;
[  \t]+           /* ignore whitespace */ context->yycolno+=yyleng;
  /* puctuation */
"." |
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
                free_string(context);
                context->yycolno+=yyleng;
                return yytext[0];}

{OCT_INT}     { context->token_type = CONSTANT;
                free_string(context);
                context->yycolno+=yyleng;
                context->token_value.int_val = strtol(yytext, NULL, 8);
                return TOKEN_INTEGER_CONSTANT; }

{DEC_INT}     { context->token_type = CONSTANT;
                free_string(context);
                context->yycolno+=yyleng;
                context->token_value.int_val = strtol(yytext, NULL, 10);
                return TOKEN_INTEGER_CONSTANT; }

{HEX_INT}     { context->token_type = CONSTANT;
                free_string(context);
                context->yycolno+=yyleng;
                context->token_value.int_val = strtol(yytext, NULL, 16);
                return TOKEN_INTEGER_CONSTANT; }

{DEC_SINGLE}  { context->token_type = CONSTANT;
                free_string(context);
                context->yycolno+=yyleng;
                context->token_value.float_val = strtof(yytext,NULL);
                return TOKEN_SINGLE_CONSTANT; }

{HEX_SINGLE}  { context->token_type = CONSTANT;
                free_string(context);
                context->yycolno+=yyleng;
                context->token_value.float_val = strtof(yytext,NULL);
                return TOKEN_SINGLE_CONSTANT; }

{IEEE_SINGLE} { context->token_type = CONSTANT;
                free_string(context);
                context->yycolno+=yyleng;
                context->token_value.float_val = ieee754tof(yytext);
                return TOKEN_SINGLE_CONSTANT; }

{DEC_DOUBLE}  { context->token_type = CONSTANT;
                free_string(context);
                context->yycolno+=yyleng;
                context->token_value.double_val = strtod(yytext,NULL);
                return TOKEN_DOUBLE_CONSTANT; }

{HEX_DOUBLE}  { context->token_type = CONSTANT;
                free_string(context);
                context->yycolno+=yyleng;
                context->token_value.double_val = strtod(yytext,NULL);
                return TOKEN_DOUBLE_CONSTANT; }

{IEEE_DOUBLE} { context->token_type = CONSTANT;
                free_string(context);
                context->yycolno+=yyleng;
                context->token_value.double_val = ieee754tod(yytext);
                return TOKEN_DOUBLE_CONSTANT; }



"/*"                          { BEGIN (COMMENT); }
<COMMENT>"*/"                 { context->token_type = CODE_COMMENTS;
                                free_string(context);
                                context->token_value.string_val = strdup(yytext);
                                context->valid_string = true;
                                BEGIN (INITIAL);
                                return TOKEN_COMMENT; }

<COMMENT>([^*]|\n)+|./"*/"    {}
<COMMENT><<EOF>>              { printf("%d: Unterminated comment\n", yylineno);
                                return 0; }

"//"                          { BEGIN(INLINE_COMMENT); }

<INLINE_COMMENT>.             {}

<INLINE_COMMENT>\n            { context->token_type = CODE_COMMENTS;
                                free_string(context);
                                context->token_value.string_val = strdup(yytext);
                                context->valid_string = true;
                                BEGIN(INITIAL);
                                return TOKEN_COMMENT;}

<INLINE_COMMENT><<EOF>>       { printf("%d: Unterminated comment.\n", yylineno);
                                return 0; }

\$c(0|1|2|3|4|5|6|7)                        { context->yycolno+=yyleng;
                                              free_string(context);
                                              context->token_value.string_val = strdup(yytext);
                                              context->valid_string = true;
                                              context->token_type = REGISTER;
                                              return TOKEN_CREGISTER;}

\$d(0|1|2|3|4|5|6|7)                        { context->yycolno+=yyleng;
                                              free_string(context);
                                              context->token_value.string_val = strdup(yytext);
                                              context->valid_string = true;
                                              context->token_type = REGISTER;
                                              return TOKEN_DREGISTER;}

\$s(0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15)  { context->yycolno+=yyleng;
                                              free_string(context);
                                              context->token_value.string_val = strdup(yytext);
                                              context->valid_string = true;
                                              context->token_type = REGISTER;
                                              return TOKEN_SREGISTER;}

\$q(0|1|2|3|4|5|6|7)                        { context->yycolno+=yyleng;
                                              free_string(context);
                                              context->token_value.string_val = strdup(yytext);
                                              context->valid_string = true;
                                              context->token_type = REGISTER;
                                              return TOKEN_QREGISTER;}

\%[_a-zA-Z]+[_a-zA-Z0-9]*       { context->token_type = IDENTIFIER;
                                  context->yycolno+=yyleng;
                                  free_string(context);
                                  context->token_value.string_val = strdup(yytext);
                                  context->valid_string = true;
                                  return TOKEN_LOCAL_IDENTIFIER; }

\&[_a-zA-Z]+[_a-zA-Z0-9]*       { context->token_type = IDENTIFIER;
                                  context->yycolno+=yyleng;
                                  free_string(context);
                                  context->token_value.string_val = strdup(yytext);
                                  context->valid_string = true;
                                  return TOKEN_GLOBAL_IDENTIFIER; }

\@[_a-zA-Z]+[_a-zA-Z0-9]*       { context->token_type = LABEL;
                                  context->yycolno+=yyleng;
                                  free_string(context);
                                  context->token_value.string_val = strdup(yytext);
                                  context->valid_string = true;
                                  return TOKEN_LABEL; }

\"(\\.|[^\\"])*\"               { context->token_type = STRING;
                                  context->yycolno+=yyleng;
                                  free_string(context);
                                  context->token_value.string_val = strdup(yytext);
                                  context->valid_string = true;
                                  return TOKEN_STRING; }

      /* image format */
snorm_int8                                     { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.format = BrigSNORM_INT8;
                                                 return TOKEN_PROPERTY; }

snorm_int16                                    { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.format = BrigSNORM_INT16;
                                                 return TOKEN_PROPERTY; }

unorm_int8                                     { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.format = BrigUNORM_INT8;
                                                 return TOKEN_PROPERTY; }

unorm_int16                                    { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.format = BrigUNORM_INT16;
                                                 return TOKEN_PROPERTY; }

unorm_short_565                                { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.format = BrigUNORM_SHORT_565;
                                                 return TOKEN_PROPERTY; }

unorm_short_555                                { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.format = BrigUNORM_SHORT_555;
                                                 return TOKEN_PROPERTY; }

unorm_short_101010                             { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.format = BrigUNORM_SHORT_101010;
                                                 return TOKEN_PROPERTY; }

signed_int8                                    { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.format = BrigSIGNED_INT8;
                                                 return TOKEN_PROPERTY; }

signed_int16                                   { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.format = BrigSIGNED_INT16;
                                                 return TOKEN_PROPERTY; }

signed_int32                                   { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.format = BrigSIGNED_INT32;
                                                 return TOKEN_PROPERTY; }

unsigned_int8                                  { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.format = BrigUNSIGNED_INT8;
                                                 return TOKEN_PROPERTY; }

unsigned_int16                                 { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.format = BrigUNSIGNED_INT16;
                                                 return TOKEN_PROPERTY; }

unsigned_int32                                 { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.format = BrigUNSIGNED_INT32;
                                                 return TOKEN_PROPERTY; }

half_float                                     { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.format = BrigHALF_FLOAT;
                                                 return TOKEN_PROPERTY; }

float                                          { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.format = BrigFLOAT;
                                                 return TOKEN_PROPERTY; }

      /* image order */
r                                              { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.order = BrigImage_R;
                                                 return TOKEN_PROPERTY; }

a                                              { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.order = BrigImage_A;
                                                 return TOKEN_PROPERTY; }

rx                                             { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.order = BrigImage_RX;
                                                 return TOKEN_PROPERTY; }

rg                                             { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.order = BrigImage_RG;
                                                 return TOKEN_PROPERTY; }

rgx                                            { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.order = BrigImage_RGX;
                                                 return TOKEN_PROPERTY; }

ra                                             { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.order = BrigImage_RA;
                                                 return TOKEN_PROPERTY; }

rgb                                            { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.order = BrigImage_RGB;
                                                 return TOKEN_PROPERTY; }

rgba                                           { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.order = BrigImage_RGBA;
                                                 return TOKEN_PROPERTY; }

rgbx                                           { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.order = BrigImage_RGBX;
                                                 return TOKEN_PROPERTY; }

bgra                                           { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.order = BrigImage_BGRA;
                                                 return TOKEN_PROPERTY; }

argb                                           { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.order = BrigImage_ARGB;
                                                 return TOKEN_PROPERTY; }

intensity                                      { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.order = BrigImage_INTENSITY;
                                                 return TOKEN_PROPERTY; }

luminance                                      { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.order = BrigImage_LUMINANCE;
                                                 return TOKEN_PROPERTY; }

normalized                                     { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.normalized = true;
                                                 return TOKEN_PROPERTY; }

unnormalized                                   { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.normalized = false;
                                                 return TOKEN_PROPERTY; }

nearest                                        { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.filter = BrigSamplerFilterNearest;
                                                 return TOKEN_PROPERTY; }

linear                                         { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.filter = BrigSamplerFilterLinear;
                                                 return TOKEN_PROPERTY; }

wrap                                           { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.boundary_mode = BrigSamplerWrap;
                                                 return TOKEN_PROPERTY;}

clamp                                          { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.boundary_mode = BrigSamplerClamp;
                                                 return TOKEN_PROPERTY;}

mirror                                         { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.boundary_mode = BrigSamplerMirror;
                                                 return TOKEN_PROPERTY;}

mirroronce                                     { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.boundary_mode = BrigSamplerMirrorOnce;
                                                 return TOKEN_PROPERTY;}

border                                         { context->token_type = PROPERTY;
                                                 free_string(context);
                                                 context->yycolno+=yyleng;
                                                 context->token_value.boundary_mode = BrigSamplerBorder;
                                                 return TOKEN_PROPERTY;}

WAVESIZE      { context->token_type = WAVESIZE;
                free_string(context);
                context->token_value.string_val = strdup(yytext);
                context->valid_string = true;
                context->yycolno += yyleng;
                return TOKEN_WAVESIZE; }

  /* keywords */

version           { context->yycolno+=yyleng;
                    free_string(context);
                    return VERSION; }

global            { context->token_type = INITIALIZABLE_ADDRESS;
                    free_string(context);
                    context->yycolno+=yyleng;
                    context->token_value.storage_class = BrigGlobalSpace ;
                    return GLOBAL; }

_Samp             { context->token_type = COMMON_KEYWORD;
                    free_string(context);
                    context->yycolno+=yyleng;
                    context->token_value.data_type = BrigSamp;
                    return _SAMP; }

_RWImg            { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    context->token_value.data_type = BrigRWImg;
                    return _RWIMG; }

_ROImg            { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    context->token_value.data_type = BrigROImg;
                    return _ROIMG; }

align             { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return ALIGN; }

file              { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return _FILE; }

arg               { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    context->token_value.storage_class = BrigArgSpace ;
                    return ARG; }

kernarg           { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    context->token_value.storage_class = BrigKernargSpace ;
                    return KERNARG; }

function          { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return FUNCTION; }

:fbar             { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return _FBAR; }

signature         { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return SIGNATURE; }

block             { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return BLOCK; }

endblock          { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return ENDBLOCK; }

blocknumeric      { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return BLOCKNUMERIC; }

blockstring       { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return BLOCKSTRING; }

kernel            { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return KERNEL; }

pragma            { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return PRAGMA; }

labeltargets      { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return LABELTARGETS; }

extension         { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return EXTENSION; }

extern            { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return EXTERN; }

static            { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return STATIC; }

const             { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return CONST; }

private           { free_string(context);
                    context->token_type = UNINITIALIZABLE_ADDRESS;
                    context->yycolno+=yyleng;
                    context->token_value.storage_class = BrigPrivateSpace ;
                    return PRIVATE; }

spill             { free_string(context);
                    context->token_type = UNINITIALIZABLE_ADDRESS;
                    context->yycolno+=yyleng;
                    context->token_value.storage_class = BrigSpillSpace ;
                    return SPILL; }

group             { free_string(context);
                    context->token_type = UNINITIALIZABLE_ADDRESS;
                    context->yycolno+=yyleng;
                    context->token_value.storage_class = BrigGroupSpace ;
                    return GROUP; }

readonly          { free_string(context);
                    context->token_type = INITIALIZABLE_ADDRESS;
                    context->yycolno+=yyleng;
                    context->token_value.storage_class = BrigReadonlySpace ;
                    return READONLY; }

loc               { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return LOC; }

_width            { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return _WIDTH; }

all               { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return ALL; }

  /* -- addressSpaceIdentifier -- */
_readonly         { free_string(context);
                    context->token_type = ADDRESS_SPACE_IDENTIFIER;
                    context->yycolno+=yyleng;
                    context->token_value.storage_class = BrigReadonlySpace ;
                    return _READONLY; }

_kernarg          { free_string(context);
                    context->token_type = ADDRESS_SPACE_IDENTIFIER;
                    context->yycolno+=yyleng;
                    context->token_value.storage_class = BrigKernargSpace ;
                    return _KERNARG;   }

_global           { free_string(context);
                    context->token_type = ADDRESS_SPACE_IDENTIFIER;
                    context->yycolno+=yyleng;
                    context->token_value.storage_class = BrigGlobalSpace ;
                    return _GLOBAL; }

_private          { free_string(context);
                    context->token_type = ADDRESS_SPACE_IDENTIFIER;
                    context->yycolno+=yyleng;
                    context->token_value.storage_class = BrigPrivateSpace ;
                    return _PRIVATE; }

_arg              { free_string(context);
                    context->token_type = ADDRESS_SPACE_IDENTIFIER;
                    context->yycolno+=yyleng;
                    context->token_value.storage_class = BrigArgSpace ;
                    return _ARG; }

_group            { free_string(context);
                    context->token_type = ADDRESS_SPACE_IDENTIFIER;
                    context->yycolno+=yyleng;
                    context->token_value.storage_class = BrigGroupSpace ;
                    return _GROUP; }

_spill            { free_string(context);
                    context->token_type = ADDRESS_SPACE_IDENTIFIER;
                    context->token_value.storage_class = BrigSpillSpace ;
                    context->yycolno+=yyleng;
                    return _SPILL; }

  /* -- vector -- */
_v2               { free_string(context);
                    context->token_type = VECTOR;
                    context->yycolno+=yyleng;
                    return _V2; }

_v4               { free_string(context);
                    context->token_type = VECTOR;
                    context->yycolno+=yyleng;
                    return _V4; }

  /* -- init -- */
format            { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return FORMAT; }

order             { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return ORDER; }

coord             { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return COORD; }

filter            { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return FILTER; }

boundaryU         { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return BOUNDARYU; }

boundaryV         { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return BOUNDARYV; }

boundaryW         { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return BOUNDARYW; }

  /* -- control -- */
itemsperworkgroup   { free_string(context);
                      context->token_type = CONTROL_KEYWORD;
                      context->yycolno+=yyleng;
                      return ITEMS_PER_WORKGROUP; }

workgroupspercu     { free_string(context);
                      context->token_type = CONTROL_KEYWORD;
                      context->yycolno+=yyleng;
                      return WORKGROUPS_PER_CU; }

memopt_on           { free_string(context);
                      context->token_type = CONTROL_KEYWORD;
                      context->yycolno+=yyleng;
                      return MEMOPT_ON; }

memopt_off          { free_string(context);
                      context->token_type = CONTROL_KEYWORD;
                      context->yycolno+=yyleng;
                      return MEMOPT_OFF; }

  /* -- Opcode -- */
  /* TODO(Chuang): "fbar_initSizeDynamic"/"fbar_initSizeKnown" */
  /* fbar_release */

fbar_initSizeWg     { free_string(context);
                      context->token_type = INSTRUCTION1_OPCODE;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigFbarInitSizeDynamic;
                      return FBAR_INITSIZEWG; }

fbar_wait           { free_string(context);
                      context->token_type = INSTRUCTION1_OPCODE;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigFbarWait;
                      return FBAR_WAIT; }

fbar_arrive         { free_string(context);
                      context->token_type = INSTRUCTION1_OPCODE;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigFbarArrive;
                      return FBAR_ARRIVE; }

fbar_skip           { free_string(context);
                      context->token_type = INSTRUCTION1_OPCODE;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigFbarSkip;
                      return FBAR_SKIP; }

fbar_release        { free_string(context);
                      context->token_type = INSTRUCTION1_OPCODE;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigFbarRelease;
                      return FBAR_RELEASE; }

countup             { free_string(context);
                      context->token_type = INSTRUCTION1_OPCODE;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigCountup;
                      return COUNTUP; }

laneid              { free_string(context);
                      context->token_type = INSTRUCTION1_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigLaneId;
                      return LANEID; }

dynwaveid           { free_string(context);
                      context->token_type = INSTRUCTION1_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigDynWaveId;
                      return DYNWAVEID; }

maxdynwaveid        { free_string(context);
                      context->token_type = INSTRUCTION1_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigMaxDynWaveId;
                      return MAXDYNWAVEID; }

dispatchid          { free_string(context);
                      context->token_type = INSTRUCTION1_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigDispatchId;
                      return DISPATCHID; }

cu                  { free_string(context);
                      context->token_type = INSTRUCTION1_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigCU;
                      return CU; }

workdim             { free_string(context);
                      context->token_type = INSTRUCTION1_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigWorkDim;
                      return WORKDIM; }

workitemid_flat     { free_string(context);
                      context->token_type = INSTRUCTION1_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigWorkItemIdFlat;
                      return WORKITEMID_FLAT; }

workitemaid_flat    { free_string(context);
                      context->token_type = INSTRUCTION1_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigWorkItemAIdFlat;
                      return WORKITEMAID_FLAT; }

debugtrap           { free_string(context);
                      context->token_type = INSTRUCTION1_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigDebugtrap;
                      return DEBUGTRAP; }

abs                 { free_string(context);
                      context->token_value.opcode = BrigAbs;
                      context->token_type = INSTRUCTION2_OPCODE;
                      context->yycolno+=yyleng;
                      return ABS; }

neg                 { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigNeg;
                      return NEG; }

not                 { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigNot;
                      return NOT; }

popcount            { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigPopcount ;
                      return POPCOUNT; }

firstbit            { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigFirstbit ;
                      return FIRSTBIT; }

lastbit             { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigLastbit ;
                      return LASTBIT; }

bitrev              { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigBitRev ;
                      return BITREV; }

movs_lo             { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigMovsLo ;
                      return MOVS_LO; }

movs_hi             { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigMovsHi ;
                      return MOVS_HI; }

fbar_init           { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigFbarInitSizeKnown ;
                      return FBAR_INIT; }

count               { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigCount ;
                      return COUNT; }

mask                { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigMask ;
                      return MASK; }

sqrt                { free_string(context);
                      context->token_value.opcode = BrigSqrt;
                      context->token_type = INSTRUCTION2_OPCODE_FTZ;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigSqrt ;
                      return SQRT; }

fract               { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE_FTZ;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigFract ;
                      return FRACT; }

fcos                { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE_FTZ;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigFcos ;
                      return FCOS; }

fsin                { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE_FTZ;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigFsin ;
                      return FSIN; }

flog2               { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE_FTZ;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigFlog2 ;
                      return FLOG2; }

fexp2               { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE_FTZ;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigFexp2 ;
                      return FEXP2; }

fsqrt               { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE_FTZ;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigFsqrt;
                      return FSQRT; }

frsqrt              { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE_FTZ;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigFrsqrt;
                      return  FRSQRT; }

frcp                { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE_FTZ;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigFrcp;
                      return FRCP; }

unpack3             { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigUnpack3;
                      return UNPACK3; }

unpack2             { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigUnpack2;
                      return  UNPACK2; }

unpack1             { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigUnpack1;
                      return UNPACK1; }

unpack0             { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigUnpack0;
                      return UNPACK0; }

alloca              { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigAlloca;
                      return ALLOCA;}

workitemid          { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigWorkItemId;
                      return WORKITEMID; }

workitemaid         { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigWorkItemAId;
                      return  WORKITEMAID;}

workgroupsize       { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigWorkGroupSize;
                      return WORKGROUPSIZE;}

workgroupid       { free_string(context);
                    context->token_type = INSTRUCTION2_OPCODE_NODT;
                    context->yycolno+=yyleng;
                    context->token_value.opcode = BrigWorkGroupId ;
                    return WORKGROUPID;}

NDRangesize         { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigNDRangeSize;
                      return NDRANGESIZE;}

NDRangegroups       { free_string(context);
                      context->token_type = INSTRUCTION2_OPCODE_NODT;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigNDRangeGroups;
                      return NDRANGEGROUPS; }

add                 { free_string(context);
                      context->token_value.opcode = BrigAdd;
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->yycolno+=yyleng; return  ADD;}

carry               { free_string(context);
                      context->token_value.opcode = BrigCarry;
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->yycolno+=yyleng;
                      return CARRY; }

borrow              { free_string(context);
                      context->token_value.opcode = BrigBorrow;
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->yycolno+=yyleng;
                      return BORROW; }

div                 { free_string(context);
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->token_value.opcode = BrigDiv;
                      context->yycolno+=yyleng;
                      return DIV; }

rem                 { free_string(context);
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->token_value.opcode = BrigRem;
                      context->yycolno+=yyleng;
                      return REM; }

sub                 { free_string(context);
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->token_value.opcode = BrigSub;
                      context->yycolno+=yyleng;
                      return SUB; }

shl                 { free_string(context);
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->token_value.opcode = BrigShl;
                      context->yycolno+=yyleng;
                      return SHL;}

shr                 { free_string(context);
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->token_value.opcode = BrigShr;
                      context->yycolno+=yyleng;
                      return SHR;}

and                 { free_string(context);
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->token_value.opcode = BrigAnd;
                      context->yycolno+=yyleng;
                      return AND;}

xor                 { free_string(context);
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->token_value.opcode = BrigXor;
                      context->yycolno+=yyleng;
                      return XOR;}

or                  { free_string(context);
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->token_value.opcode = BrigOr;
                      context->yycolno+=yyleng;
                      return OR;}

unpacklo            { free_string(context);
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->token_value.opcode = BrigUnpackLo;
                      context->yycolno+=yyleng;
                      return UNPACKLO;}

unpackhi            { free_string(context);
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->token_value.opcode = BrigUnpackHi;
                      context->yycolno+=yyleng;
                      return UNPACKHI;}

movd_lo             { free_string(context);
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->token_value.opcode = BrigMovdLo;
                      context->yycolno+=yyleng;
                      return MOVD_LO;}

movd_hi             { free_string(context);
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->token_value.opcode = BrigMovdHi;
                      context->yycolno+=yyleng;
                      return MOVD_HI;}

copysign            { free_string(context);
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->token_value.opcode = BrigCopySign;
                      context->yycolno+=yyleng;
                      return COPYSIGN;}

class               { free_string(context);
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->token_value.opcode = BrigClass;
                      context->yycolno+=yyleng;
                      return CLASS;}

send                { free_string(context);
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->token_value.opcode = BrigSend;
                      context->yycolno+=yyleng;
                      return SEND;}

receive             { free_string(context);
                      context->token_type = INSTRUCTION3_OPCODE;
                      context->token_value.opcode = BrigReceive;
                      context->yycolno+=yyleng;
                      return RECEIVE;}

max                 { free_string(context);
                      context->token_type = INSTRUCTION3_OPCODE_FTZ;
                      context->token_value.opcode = BrigMax;
                      context->yycolno+=yyleng;
                      return MAX;}

min                 { free_string(context);
                      context->token_type = INSTRUCTION3_OPCODE_FTZ;
                      context->token_value.opcode = BrigMin;
                      context->yycolno+=yyleng;
                      return MIN;}

mad                 { free_string(context);
                      context->token_type = INSTRUCTION4_OPCODE;
                      context->token_value.opcode = BrigMad;
                      context->yycolno+=yyleng;
                      return MAD;}

extract             { free_string(context);
                      context->token_type = INSTRUCTION4_OPCODE;
                      context->token_value.opcode = BrigExtract;
                      context->yycolno+=yyleng;
                      return EXTRACT;}

insert              { free_string(context);
                      context->token_type = INSTRUCTION4_OPCODE;
                      context->token_value.opcode = BrigInsert;
                      context->yycolno+=yyleng;
                      return INSERT;}

shuffle             { free_string(context);
                      context->token_type = INSTRUCTION4_OPCODE;
                      context->token_value.opcode = BrigShuffle;
                      context->yycolno+=yyleng;
                      return SHUFFLE;}

cmov                { free_string(context);
                      context->token_type = INSTRUCTION4_OPCODE;
                      context->token_value.opcode = BrigCmov;
                      context->yycolno+=yyleng;
                      return CMOV;}

fma                 { free_string(context);
                      context->token_type = INSTRUCTION4_OPCODE;
                      context->token_value.opcode = BrigFma;
                      context->yycolno+=yyleng;
                      return FMA;}

bitalign            { free_string(context);
                      context->token_type = INSTRUCTION4_OPCODE;
                      context->token_value.opcode = BrigBitAlign;
                      context->yycolno+=yyleng;
                      return BITALIGN;}

bytealign           { free_string(context);
                      context->token_type = INSTRUCTION4_OPCODE;
                      context->token_value.opcode = BrigByteAlign;
                      context->yycolno+=yyleng;
                      return BYTEALIGN;}

lerp                { free_string(context);
                      context->token_type = INSTRUCTION4_OPCODE;
                      context->token_value.opcode = BrigLerp;
                      context->yycolno+=yyleng;
                      return LERP;}

sad                 { free_string(context);
                      context->token_type = INSTRUCTION4_OPCODE;
                      context->token_value.opcode = BrigSad;
                      context->yycolno+=yyleng;
                      return SAD;}

sad2                { free_string(context);
                      context->token_type = INSTRUCTION4_OPCODE;
                      context->token_value.opcode = BrigSad2;
                      context->yycolno+=yyleng;
                      return SAD2;}

sad4                { free_string(context);
                      context->token_type = INSTRUCTION4_OPCODE;
                      context->token_value.opcode = BrigSad4;
                      context->yycolno+=yyleng;
                      return SAD4;}

sad4hi              { free_string(context);
                      context->token_type = INSTRUCTION4_OPCODE;
                      context->token_value.opcode = BrigSad4Hi;
                      context->yycolno+=yyleng;
                      return  SAD4HI;}

bitselect           { free_string(context);
                      context->token_type = INSTRUCTION4_OPCODE;
                      context->token_value.opcode = BrigBitSelect;
                      context->yycolno+=yyleng;
                      return BITSELECT;}

cbr                 { free_string(context);
                      context->token_type =BRANCH_OP;
                      context->yycolno+=yyleng;
                      return CBR; }

brn                 { free_string(context);
                      context->token_type =BRANCH_OP;
                      context->yycolno+=yyleng;
                      return BRN; }

_fbar               { free_string(context);
                      context->token_type = COMMON_KEYWORD;
                      context->yycolno+=yyleng;
                      return __FBAR; }

call                { free_string(context);
                      context->token_type = COMMON_KEYWORD;
                      context->yycolno+=yyleng;
                      return CALL; }
  /*-- atomicOperationId --*/
_and                { free_string(context);
                      context->token_type = ATOMIC_OP;
                      context->yycolno+=yyleng;
                      return _AND_;}

_or                 { free_string(context);
                      context->token_type = ATOMIC_OP;
                      context->yycolno+=yyleng;
                      return _OR_;}

_xor                { free_string(context);
                      context->token_type = ATOMIC_OP;
                      context->yycolno+=yyleng;
                      return _XOR_;}

_exch               { free_string(context);
                      context->token_type = ATOMIC_OP;
                      context->yycolno+=yyleng;
                      return _EXCH_;}

_add                { free_string(context);
                      context->token_type = ATOMIC_OP;
                      context->yycolno+=yyleng;
                      return _ADD_;}

_sub                { free_string(context);
                      context->token_type = ATOMIC_OP;
                      context->yycolno+=yyleng;
                      return _SUB_;}

_inc                { free_string(context);
                      context->token_type = ATOMIC_OP;
                      context->yycolno+=yyleng;
                      return _INC_;}

_dec                { free_string(context);
                      context->token_type = ATOMIC_OP;
                      context->yycolno+=yyleng;
                      return _DEC_;}

_max                { free_string(context);
                      context->token_type = ATOMIC_OP;
                      context->yycolno+=yyleng;
                      return _MAX_;}

_min                { free_string(context);
                      context->token_type = ATOMIC_OP;
                      context->yycolno+=yyleng;
                      return _MIN_;}

  /*-- comparison --*/
_eq                 { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return _EQ;}

_ne                 { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _NE;}

_lt                 { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _LT;}

_le                 { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _LE;}

_gt                 { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _GT;}

_ge                 { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _GE;}

_equ                { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _EQU;}

_neu                { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _NEU;}

_ltu                { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _LTU;}

_leu                { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _LEU;}

_gtu                { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _GTU;}

_geu                { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _GEU;}

_num                { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _NUM;}

_nan                { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _NAN;}

_seq                { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _SEQ;}

_sne                { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _SNE;}

_slt                { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _SLT;}

_sle                { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _SLE;}

_sgt                { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _SGT;}

_sge                { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _SGE;}

_snum               { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _SNUM;}

_snan               { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _SNAN;}

_sequ               { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _SEQU;}

_sneu               { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _SNEU;}

_sltu               { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _SLTU;}

_sleu               { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _SLEU;}

_sgtu               { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _SGTU;}

_sgeu               { free_string(context);
                      context->token_type = COMPARISON;
                      context->yycolno+=yyleng;
                      return  _SGEU;}

  /*-- rounding --*/
_upi                { free_string(context);
                      context->yycolno+=yyleng;
                      context->token_type = INT_ROUNDING;
                      return  _UPI;}

_downi              { free_string(context);
                      context->yycolno+=yyleng;
                      context->token_type = INT_ROUNDING;
                      return  _DOWNI;}

_zeroi              { free_string(context);
                      context->yycolno+=yyleng;
                      context->token_type = INT_ROUNDING;
                      return  _ZEROI;}

_neari              { free_string(context);
                      context->yycolno+=yyleng;
                      context->token_type = INT_ROUNDING;
                      return  _NEARI;}

_up                 { free_string(context);
                      context->yycolno+=yyleng;
                      context->token_type = FLOAT_ROUNDING;
                      return  _UP;}

_down               { free_string(context);
                      context->yycolno+=yyleng;
                      context->token_type = FLOAT_ROUNDING;
                      return  _DOWN;}

_zero               { free_string(context);
                      context->yycolno+=yyleng;
                      context->token_type = FLOAT_ROUNDING;
                      return  _ZERO;}

_near               { free_string(context);
                      context->yycolno+=yyleng;
                      context->token_type = FLOAT_ROUNDING;
                      return  _NEAR;}

  /*-- context->token_value.packing -- */
_pp                 { free_string(context);
                      context->token_value.packing = BrigPackPP;
                      context->token_type = PACKING;
                      context->yycolno+=yyleng;
                      return  _PP;}

_ps                 { free_string(context);
                      context->token_value.packing = BrigPackPS;
                      context->token_type = PACKING;
                      context->yycolno+=yyleng;
                      return  _PS;}

_sp                 { free_string(context);
                      context->token_value.packing = BrigPackSP;
                      context->token_type = PACKING;
                      context->yycolno+=yyleng;
                      return  _SP;}

_ss                 { free_string(context);
                      context->token_value.packing = BrigPackSS;
                      context->token_type = PACKING;
                      context->yycolno+=yyleng;
                      return  _SS;}

_s                  { free_string(context);
                      context->token_value.packing = BrigPackS;
                      context->token_type = PACKING;
                      context->yycolno+=yyleng;
                      return  __S;}

_p                  { free_string(context);
                      context->token_value.packing = BrigPackP;
                      context->token_type = PACKING;
                      context->yycolno+=yyleng;
                      return  __P;}

_pp_sat             { free_string(context);
                      context->token_value.packing = BrigPackPPsat;
                      context->token_type = PACKING;
                      context->yycolno+=yyleng;
                      return  _PP_SAT;}

_ps_sat             { free_string(context);
                      context->token_value.packing = BrigPackPSsat;
                      context->token_type = PACKING;
                      context->yycolno+=yyleng;
                      return  _PS_SAT;}

_sp_sat             { free_string(context);
                      context->token_value.packing = BrigPackSPsat;
                      context->token_type = PACKING;
                      context->yycolno+=yyleng;
                      return  _SP_SAT;}

_ss_sat             { free_string(context);
                      context->token_value.packing = BrigPackSSsat;
                      context->token_type = PACKING;
                      context->yycolno+=yyleng;
                      return  _SS_SAT;}

_s_sat              { free_string(context);
                      context->token_value.packing = BrigPackSsat;
                      context->token_type = PACKING;
                      context->yycolno+=yyleng;
                      return  _S_SAT;}

_p_sat              { free_string(context);
                      context->token_value.packing = BrigPackPsat;
                      context->token_type = PACKING;
                      context->yycolno+=yyleng;
                      return  _P_SAT;}

  /*-- target --*/
\$small             { free_string(context);
                      context->token_type = TARGET;
                      context->yycolno+=yyleng;
                      return  _SMALL;}

\$large             { free_string(context);
                      context->token_type = TARGET;
                      context->yycolno+=yyleng;
                      return  _LARGE;}

\$full              { free_string(context);
                      context->token_type = TARGET;
                      context->yycolno+=yyleng;
                      return  _FULL;}

\$reduced           { free_string(context);
                      context->token_type = TARGET;
                      context->yycolno+=yyleng;
                      return  _REDUCED;}

\$sftz              { free_string(context);
                      context->token_type = TARGET;
                      context->yycolno+=yyleng;
                      return  _SFTZ;}

\$nosftz            { free_string(context);
                      context->token_type = TARGET;
                      context->yycolno+=yyleng;
                      return  _NOSFTZ;}

width               { free_string(context);
                      context->token_type = COMMON_KEYWORD;
                      context->yycolno+=yyleng;
                      return WIDTH;}

height              { free_string(context);
                      context->token_type = COMMON_KEYWORD;
                      context->yycolno+=yyleng;
                      return HEIGHT;}

depth               { free_string(context);
                      context->token_type = COMMON_KEYWORD;
                      context->yycolno+=yyleng;
                      return DEPTH;}

  /*-- dataTypeId --*/
_u32                { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigu32;
                      context->yycolno+=yyleng;
                      return  _U32;}

_s32                { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigs32;
                      context->yycolno+=yyleng;
                      return  _S32;}

_s64                { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigs64;
                      context->yycolno+=yyleng;
                      return  _S64;}

_u64                { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigu64;
                      context->yycolno+=yyleng;
                      return  _U64;}

_b1                 { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigb1;
                      context->yycolno+=yyleng;
                      return  _B1;}

_b32                { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigb32;
                      context->yycolno+=yyleng;
                      return  _B32;}

_f64                { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigf64;
                      context->yycolno+=yyleng;
                      return  _F64;}

_f32                { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigf32;
                      context->yycolno+=yyleng;
                      return  _F32;}

_b64                { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigb64;
                      context->yycolno+=yyleng;
                      return  _B64;}

_b8                 { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigb8;
                      context->yycolno+=yyleng;
                      return  _B8;}

_b16                { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigb16;
                      context->yycolno+=yyleng;
                      return  _B16;}

_s8                 { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigs8;
                      context->yycolno+=yyleng;
                      return  _S8;}

_s16                { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigs16;
                      context->yycolno+=yyleng;
                      return  _S16;}

_u8                 { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigu8;
                      context->yycolno+=yyleng;
                      return  _U8;}

_u16                { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigu16;
                      context->yycolno+=yyleng;
                      return  _U16;}

_f16                { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigf16;
                      context->yycolno+=yyleng;
                      return  _F16;}

_b128               { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigb128;
                      context->yycolno+=yyleng;
                      return  _B128;}

_u8x4               { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigu8x4;
                      context->yycolno+=yyleng;
                      return  _U8X4;}

_s8x4               { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigs8x4;
                      context->yycolno+=yyleng;
                      return  _S8X4;}

_u16x2              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigu16x2;
                      context->yycolno+=yyleng;
                      return  _U16X2;}

_s16x2              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigs16x2;
                      context->yycolno+=yyleng;
                      return  _S16X2;}

_f16x2              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigf16x2;
                      context->yycolno+=yyleng;
                      return  _F16X2;}

_f32x2              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigf32x2;
                      context->yycolno+=yyleng;
                      return  _F32X2;}

_u32x2              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigu32x2;
                      context->yycolno+=yyleng;
                      return  _U32X2;}

_s32x2              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigs32x2;
                      context->yycolno+=yyleng;
                      return  _S32X2;}


_u8x8               { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigu8x8;
                      context->yycolno+=yyleng;
                      return  _U8X8;}

_s8x8               { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigs8x8;
                      context->yycolno+=yyleng;
                      return  _S8X8;}

_u16x4              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigu16x4; context->yycolno+=yyleng;  return  _U16X4;}
_s16x4              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigs16x4;
                      context->yycolno+=yyleng;
                      return  _S16X4;}

_f16x4              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigf16x4;
                      context->yycolno+=yyleng;
                      return  _F16X4;}

_u8x16              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigu8x16;
                      context->yycolno+=yyleng;
                      return  _U8X16;}

_s8x16              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigs8x16;
                      context->yycolno+=yyleng;
                      return  _S8X16;}

_u16x8              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigu16x8;
                      context->yycolno+=yyleng;
                      return  _U16X8;}

_s16x8              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigs16x8;
                      context->yycolno+=yyleng;
                      return  _S16X8;}

_f16x8              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigf16x8;
                      context->yycolno+=yyleng;
                      return  _F16X8;}

_f32x4              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigf32x4;
                      context->yycolno+=yyleng;
                      return  _F32X4;}

_s32x4              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigs32x4;
                      context->yycolno+=yyleng;
                      return  _S32X4;}

_u32x4              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigu32x4;
                      context->yycolno+=yyleng;
                      return  _U32X4;}

_f64x2              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigf64x2;
                      context->yycolno+=yyleng;
                      return  _F64X2;}

_s64x2              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigs64x2;
                      context->yycolno+=yyleng;
                      return  _S64X2;}

_u64x2              { free_string(context);
                      context->token_type = DATA_TYPE_ID;
                      context->token_value.data_type = Brigu64x2;
                      context->yycolno+=yyleng;
                      return  _U64X2;}

_ftz                { free_string(context);
                      context->token_type = COMMON_KEYWORD;
                      context->yycolno+=yyleng;
                      return _FTZ;}

nop                 { free_string(context);
                      context->token_type = COMMON_KEYWORD;
                      context->yycolno+=yyleng;
                      return NOP;}

clock               { free_string(context);
                      context->token_type = COMMON_KEYWORD;
                      context->yycolno+=yyleng;
                      context->token_value.opcode = BrigClock;
                      return CLOCK;}

syscall             { free_string(context);
                      context->token_type = COMMON_KEYWORD;
                      context->yycolno+=yyleng;
                      return SYSCALL;}

  /*-- mul --*/
mul               { free_string(context);
                    context->token_type = MUL_OP;
                    context->yycolno+=yyleng;
                    return MUL;}

mul_hi            { free_string(context);
                    context->token_type = MUL_OP;
                    context->yycolno+=yyleng;
                    return MUL_HI;}

mul24_hi          { free_string(context);
                    context->token_type = MUL_OP;
                    context->yycolno+=yyleng;
                    return MUL24_HI;}

mul24             { free_string(context);
                    context->token_type = MUL_OP;
                    context->yycolno+=yyleng;
                    return MUL24;}

mad24             { free_string(context);
                    context->token_type = MUL_OP;
                    context->yycolno+=yyleng;
                    return MAD24;}

mad24_hi          { free_string(context);
                    context->token_type = MUL_OP;
                    context->yycolno+=yyleng;
                    return MAD24_HI;}

f2u4              { free_string(context);
                    context->token_type = INSTRUCTION5_OPCODE;
                    context->yycolno+=yyleng;
                    return F2U4;}

  /*-- Atom Modifiers --*/
_ar               { free_string(context);
                    context->token_type = ATOM_MODIFIER;
                    context->yycolno+=yyleng;
                    return _AR;}

_region           { free_string(context);
                    context->token_type = ATOM_MODIFIER;
                    context->yycolno+=yyleng;
                    return _REGION;}

_cas              { free_string(context);
                    context->token_type = ATOM_MODIFIER;
                    context->yycolno+=yyleng;
                    return _CAS;}

atomic_cas        { free_string(context);
                    context->token_type = ATOM_MODIFIER;
                    context->yycolno+=yyleng;
                    return ATOMIC_CAS;}

atomic            { free_string(context);
                    context->token_type = ATOM_MODIFIER;
                    context->yycolno+=yyleng;
                    return ATOMIC;}

atomicNoRet       { free_string(context);
                    context->token_type = ATOM_MODIFIER;
                    context->yycolno+=yyleng;
                    return ATOMICNORET;}

atomicNoRet_cas   { free_string(context);
                    context->token_type = ATOM_MODIFIER;
                    context->yycolno+=yyleng;
                    return ATOMICNORET_CAS;}

atomic_image      { free_string(context);
                    context->token_type = ATOM_MODIFIER;
                    context->yycolno+=yyleng;
                    return ATOMIC_IMAGE;}

atomicNoRet_image { free_string(context);
                    context->token_type = ATOM_MODIFIER;
                    context->yycolno+=yyleng;
                    return ATOMICNORET_IMAGE;}

cvt               { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return CVT;}

_dep              { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return _DEP;}

_equiv            { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return _EQUIV;}

_acq              { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return _ACQ;}

_rel              { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return _REL;}

ld                { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return LD;}

sync              { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    context->token_value.opcode = BrigSync;
                    return SYNC;}

barrier           { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    context->token_value.opcode = BrigBarrier;
                    return BARRIER;}

segmentp          { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return SEGMENTP;}

ftos              { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return FTOS;}

stof              { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return STOF; }

mov               { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return MOV;}

lad               { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return LAD;}

ldc               { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return LDC;}

lda               { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return LDA;}

ret               { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    context->token_value.opcode = BrigRet;
                    return RET;}

packedcmp         { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return PACKEDCMP;}

cmp               { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return CMP;}

st                { free_string(context);
                    context->token_type = COMMON_KEYWORD;
                    context->yycolno+=yyleng;
                    return ST;}

  /* -- geometry ID -- */
_1d               { free_string(context);
                    context->token_type = GEOMETRY_ID;
                    context->yycolno+=yyleng;
                    return _1D;}

_2d               { free_string(context);
                    context->token_type = GEOMETRY_ID;
                    context->yycolno+=yyleng;
                    return _2D;}

_3d               { free_string(context);
                    context->token_type = GEOMETRY_ID;
                    context->yycolno+=yyleng;
                    return _3D;}

_1db              { free_string(context);
                    context->token_type = GEOMETRY_ID;
                    context->yycolno+=yyleng;
                    return _1DB;}

_1da              { free_string(context);
                    context->token_type = GEOMETRY_ID;
                    context->yycolno+=yyleng;
                    return _1DA;}

_2da              { free_string(context);
                    context->token_type = GEOMETRY_ID;
                    context->yycolno+=yyleng;
                    return _2DA;}

rd_image          { free_string(context);
                    context->token_type = IMAGE_OP;
                    context->yycolno+=yyleng;
                    return RD_IMAGE;}

ld_image          { free_string(context);
                    context->token_type = IMAGE_OP;
                    context->yycolno+=yyleng;
                    return LD_IMAGE;}

st_image          { free_string(context);
                    context->token_type = IMAGE_OP;
                    context->yycolno+=yyleng;
                    return ST_IMAGE;}

  /* -- queryOp --*/
query_order       { free_string(context);
                    context->token_type = QUERY_OP;
                    context->yycolno+=yyleng;
                    return QUERY_ORDER;}

query_data        { free_string(context);
                    context->token_type = QUERY_OP;
                    context->yycolno+=yyleng;
                    return QUERY_DATA;}

query_array       { free_string(context);
                    context->token_type = QUERY_OP;
                    context->yycolno+=yyleng;
                    return QUERY_ARRAY;}

query_width       { free_string(context);
                    context->token_type = QUERY_OP;
                    context->yycolno+=yyleng;
                    return QUERY_WIDTH;}

query_depth       { free_string(context);
                    context->token_type = QUERY_OP;
                    context->yycolno+=yyleng;
                    return QUERY_DEPTH;}

query_height      { free_string(context);
                    context->token_type = QUERY_OP;
                    context->yycolno+=yyleng;
                    return QUERY_HEIGHT;}

query_normalized  { free_string(context);
                    context->token_type = QUERY_OP;
                    context->yycolno+=yyleng;
                    return QUERY_NORMALIZED;}

query_filtering   { free_string(context);
                    context->token_type = QUERY_OP;
                    context->yycolno+=yyleng;
                    return QUERY_FILTERING;}

.   { fprintf(stderr, "Unexpected char %s\n", yytext);
      free_string(context);
      context->token_to_scan = 0;
      context->token_type = UNKNOWN;
      return 0;}
