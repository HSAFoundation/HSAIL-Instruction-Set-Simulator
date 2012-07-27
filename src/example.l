/* Copyright 2012 <MulticorewareInc> */

%option noyywrap 
%{
#include <stdio.h>
#include "./tokens.h"

int int_val;
float float_val;
double double_val;
char* string_val;

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
	double return_val = 0.0;	// return value
	unsigned int* int_ptr = 0;
	char first4[5];	// first 2 bytes(4 chars after 0d)
	char temp_str[5];	// temporary string to hold part of yytext
	unsigned int i;
	unsigned int first_int;		// convert the double as 2 integers
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

OCT_INT		([+|-]?0[0-7]*)
DEC_INT		([+|-]?(0|([1-9]*[0-9]+)))
HEX_INT		([+|-]?0(x|X)[0-9a-fA-F]*)	

DEC_DOUBLE 	([+|-]?(([0-9]*("."[0-9]*)?((e|E)[+|-]?[0-9]*)?)|([0-9]+(e|E)[+|-]?[0-9]*))(l|L)?)  
HEX_DOUBLE	(0(x|X)[0-9a-fA-F]+(("."[0-9a-fA-F]*)((p|P)[+|-]?[0-9]*)?)(l|L)?)
IEEE_DOUBLE	(0(d|D)[0-9a-fA-F]{16}) 
	
DEC_SINGLE	([+|-]?(([0-9]*("."[0-9]*)?((e|E)[+|-]?[0-9]*)?)|([0-9]+(e|E)[+|-]?[0-9]*))(f|F))
HEX_SINGLE	(0(x|X)[0-9a-fA-F]+(("."[0-9a-fA-F]*)((p|P)[+|-]?[0-9]*)?)(f|F))
IEEE_SINGLE	(0(f|F)[0-9a-fA-F]{8})

%%

\n					/* ignore EOL */;
[  \t]+					/* ignore whitespace */;
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
","		               { return yytext[0];}

{OCT_INT} { int_val = strtol(yytext, NULL, 8); return TOKEN_INTEGER_CONSTANT; } 
{DEC_INT} { int_val = strtol(yytext, NULL, 10); return TOKEN_INTEGER_CONSTANT; }
{HEX_INT} { int_val = strtol(yytext, NULL, 16); return TOKEN_INTEGER_CONSTANT; }

{DEC_SINGLE} { float_val = strtof(yytext,NULL); return TOKEN_SINGLE_CONSTANT; }
{HEX_SINGLE} { float_val = strtof(yytext,NULL); return TOKEN_SINGLE_CONSTANT; }
{IEEE_SINGLE} { float_val = ieee754tof(yytext); return TOKEN_SINGLE_CONSTANT; }

{DEC_DOUBLE} {double_val = strtod(yytext,NULL); return TOKEN_DOUBLE_CONSTANT; }
{HEX_DOUBLE} {double_val = strtod(yytext,NULL); return TOKEN_DOUBLE_CONSTANT; }
{IEEE_DOUBLE} {  double_val = ieee754tod(yytext); return TOKEN_DOUBLE_CONSTANT; }



"/*"					{ BEGIN (COMMENT); }
<COMMENT>"*/"			{ BEGIN (INITIAL); return TOKEN_COMMENT; }
<COMMENT>([^*]|\n)+|./"*/" 	{}
<COMMENT><<EOF>>		{ printf("%d: Unterminated comment\n", yylineno); return 0; }

"//"					{ BEGIN(INLINE_COMMENT); }
<INLINE_COMMENT>.		{}
<INLINE_COMMENT>\n		{ BEGIN(INITIAL); return TOKEN_COMMENT;}
<INLINE_COMMENT><<EOF>>		{ printf("%d: Unterminated comment\n", yylineno); return 0; }


\$c(0|1|2|3|4|5|6|7) 	{string_val = strdup(yytext);  return TOKEN_CREGISTER;}
\$d(0|1|2|3|4|5|6|7) 	{string_val = strdup(yytext);  return TOKEN_DREGISTER;}
\$s(0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15) {string_val = strdup(yytext);  return TOKEN_SREGISTER;}
\$q(0|1|2|3|4|5|6|7) 	{string_val = strdup(yytext);  return TOKEN_QREGISTER;}

\%[_a-zA-Z]+[_a-zA-Z0-9]*  	{string_val = strdup(yytext); return TOKEN_LOCAL_IDENTIFIER; }
\&[_a-zA-Z]+[_a-zA-Z0-9]*	  {string_val = strdup(yytext); return TOKEN_GLOBAL_IDENTIFIER; }

\@[_a-zA-Z]+[_a-zA-Z0-9]* 	{string_val = strdup(yytext);  return TOKEN_LABEL; }
\"(\\.|[^\\"])*\" 		{string_val = strdup(yytext);  return TOKEN_STRING; }

((snorm_|unorm_)(((int)(8|16|32))|((short_)(565|555|101010)))) |
((signed_|unsigned_)(int)(8|16|32)) |
((half_)?(float))													{return TOKEN_PROPERTY; }
(r|rx|a|rg|rgx|ra|rgb|rgbx|ra|rgb|rgbx|rgba|bgra|argb|intensity|luminance) {return TOKEN_PROPERTY; }
(normalized|unnormalized) { return TOKEN_PROPERTY; }
(nearest|linear) { return TOKEN_PROPERTY; }	
(wrap|clamp|mirror|mirroronce|border) {return TOKEN_PROPERTY;}

WAVESIZE			{return TOKEN_WAVESIZE; }

	/* keywords */
workgroupid			{ return WORKGROUPID; printf("WORKGROUPID\n");}
version				{ return VERSION; }
global				{ return GLOBAL; }
_Samp				{ return _SAMP; }
_RWImg				{ return _RWIMG; }
_ROImg				{ return _ROIMG; }
align				{ return ALIGN; }
file				{ return _FILE; }
arg					{ return ARG; }
kernarg				{ return KERNARG; }
function			{ return FUNCTION; }
:fbar				{ return _FBAR; }
signature			{ return SIGNATURE; }
block				{ return BLOCK; }
endblock			{ return ENDBLOCK; }
blocknumeric		{ return BLOCKNUMERIC; }
blockstring			{ return BLOCKSTRING; }
kernel				{ return KERNEL; }
pragma				{ return PRAGMA; }
labeltargets		{ return LABELTARGETS; }
extension			{ return EXTENSION; }
extern				{ return EXTERN; }
static				{ return STATIC; }
const				{ return CONST; }
private				{ return PRIVATE; }
spill				{ return SPILL; }
group				{ return GROUP; }
readonly			{ return READONLY; }
loc					{ return LOC; }
_width				{ return _WIDTH; }
all					{ return ALL; }

	/* -- addressSpaceIdentifier -- */
_readonly			{ return  _READONLY; }
_kernarg			{ return  _KERNARG; 	}
_global				{ return  _GLOBAL; }	
_private			{ return  _PRIVATE; }
_arg				{ return  _ARG; 	}
_group				{ return  _GROUP; }
_spill				{ return  _SPILL; }

	/* -- vector -- */
_v2					{ return _V2; }
_v4					{ return _V4; }


	/* -- init -- */
format				{ return  FORMAT; }
order				{ return  ORDER; }
coord				{ return  COORD; }
filter				{ return  FILTER; }
boundaryU			{ return  BOUNDARYU; }
boundaryV			{ return  BOUNDARYV; }
boundaryW			{ return  BOUNDARYW; }

	/* -- control -- */
itemsperworkgroup	{ return  ITEMS_PER_WORKGROUP; }
workgroupspercu		{ return  WORKGROUPS_PER_CU; }
memopt_on			{ return  MEMOPT_ON; }
memopt_off			{ return  MEMOPT_OFF; }

	/* -- Opcode -- */
fbar_initSizeWg		{ return  FBAR_INITSIZEWG; }
fbar_wait			{ return  FBAR_WAIT; }
fbar_arrive			{ return  FBAR_ARRIVE; }
fbar_skip			{ return  FBAR_SKIP; }
fbar_release		{ return  FBAR_RELEASE; }
countup				{ return  COUNTUP; }

laneid				{ return  LANEID; }
dynwaveid			{ return  DYNWAVEID; }
maxdynwaveid		{ return  MAXDYNWAVEID; }
dispatchid			{ return  DISPATCHID; }
cu					{ return  CU; }
workdim				{ return  WORKDIM; }
workitemid_flat		{ return  WORKITEMID_FLAT; }
workitemaid_flat	{ return  WORKITEMAID_FLAT; }
debugtrap			{ return  DEBUGTRAP; }


abs					{ return  ABS; }
neg					{ return  NEG; }
not					{ return  NOT; }
popcount			{ return  POPCOUNT; }
firstbit			{ return  FIRSTBIT; }
lastbit				{ return  LASTBIT; }
bitrev				{ return  BITREV; }
movs_lo				{ return  MOVS_LO; }
movs_hi				{ return  MOVS_HI; }
fbar_initSize		{ return  FBAR_INITSIZE; }
fbar_init			{ return  FBAR_INIT; }
fbar_releaseCF		{ return  FBAR_RELEASECF; }
count				{ return  COUNT; }
mask				{ return  MASK; }

sqrt				{ return  SQRT; }
fract				{ return  FRACT; }
fcos				{ return  FCOS; }
fsin				{ return  FSIN; }
flog2				{ return  FLOG2; }
fexp2				{ return  FEXP2; }
fsqrt				{ return  FSQRT; }
frsqrt				{ return  FRSQRT; }
frcp				{ return  FRCP; }


unpack3				{ return  UNPACK3; }
unpack2				{ return  UNPACK2; }
unpack1				{ return  UNPACK1; }
unpack0				{ return  UNPACK0; }
alloca				{ return  ALLOCA;}
workitemid			{ return  WORKITEMID; }
workitemaid			{ return  WORKITEMAID;}
workgroupsize		{ return  WORKGROUPSIZE;}
NDRangesize			{ return  NDRANGESIZE;}
NDRangegroups		{ return  NDRANGEGROUPS; }

add					{ return  ADD;}
carry				{ return  CARRY; }
borrow				{ return  BORROW; }
div					{ return  DIV; }
rem					{ return  REM; }
sub					{ return  SUB; }
shl					{ return  SHL;}
shr					{ return  SHR;}
and					{ return  AND;}
xor					{ return  XOR;}
or					{ return  OR;}
unpacklo			{ return  UNPACKLO;}
unpackhi			{ return  UNPACKHI;}
movd_lo				{ return  MOVD_LO;}
movd_hi				{ return  MOVD_HI;}
copysign			{ return  COPYSIGN;}
class				{ return  CLASS;}
send				{ return  SEND;}
receive				{ return  RECEIVE;}


max					{ return  MAX;}
min					{ return  MIN;}

mad					{ return  MAD;}
extract				{ return  EXTRACT;}
insert				{ return  INSERT;}
shuffle				{ return  SHUFFLE;}
cmov				{ return  CMOV;}
fma					{ return  FMA;}
bitalign			{ return  BITALIGN;}
bytealign			{ return  BYTEALIGN;}
lerp				{ return  LERP;}
sad					{ return  SAD;}
sad2				{ return  SAD2;}
sad4				{ return  SAD4;}
sad4hi				{ return  SAD4HI;}
bitselect			{ return  BITSELECT;}
cbr					{ return CBR; }
brn                 { return BRN; }
_fbar				{ return __FBAR; }
call				{ return CALL; }
	/*-- atomicOperationId --*/
_and				{ return  _AND_;}
_or					{ return  _OR_;}
_xor				{ return  _XOR_;}
_exch				{ return  _EXCH_;}
_add				{ return  _ADD_;}
_sub				{ return  _SUB_;}
_inc				{ return  _INC_;}
_dec				{ return  _DEC_;}
_max				{ return   _MAX_;}
_min				{ return   _MIN_;}

	/*-- comparison --*/
_eq					{ return  _EQ;}
_ne					{ return  _NE;}
_lt					{ return  _LT;}
_le					{ return  _LE;}
_gt					{ return  _GT;}
_ge					{ return  _GE;}
_equ				{ return  _EQU;}
_neu				{ return  _NEU;}
_ltu				{ return  _LTU;}
_leu				{ return  _LEU;}
_gtu				{ return  _GTU;}
_geu				{ return  _GEU;}
_num				{ return  _NUM;}
_nan				{ return  _NAN;}
_seq				{ return  _SEQ;}
_sne				{ return  _SNE;}
_slt				{ return  _SLT;}
_sle				{ return  _SLE;}
_sgt				{ return  _SGT;}
_sge				{ return  _SGE;}
_snum				{ return  _SNUM;}
_snan				{ return  _SNAN;}
_sequ				{ return  _SEQU;}
_sneu				{ return  _SNEU;}
_sltu				{ return  _SLTU;}
_sleu				{ return  _SLEU;}
_sgtu				{ return  _SGTU;}
_sgeu				{ return  _SGEU;}

	/*-- rounding --*/
_upi				{ return  _UPI;}
_downi				{ return  _DOWNI;}
_zeroi				{ return  _ZEROI;}
_neari				{ return  _NEARI;}

_up					{ return  _UP;}
_down				{ return  _DOWN;}
_zero				{ return  _ZERO;}
_near				{ return  _NEAR;}

	/*-- packing -- */
_pp					{ return  _PP;}
_ps					{ return  _PS;}
_sp					{ return  _SP;}
_ss					{ return  _SS;}
_s					{ return  __S;}
_p					{ return  __P;}
_pp_sat				{ return  _PP_SAT;}
_ps_sat				{ return  _PS_SAT;}
_sp_sat				{ return  _SP_SAT;}
_ss_sat				{ return  _SS_SAT;}
_s_sat				{ return  _S_SAT;}
_p_sat				{ return  _P_SAT;}

	/*-- target --*/
\$small				{return  _SMALL;}
\$large				{return  _LARGE;}
\$full				{ return  _FULL;}
\$reduced			{ return  _REDUCED;}
\$sftz				{ return  _SFTZ;}
\$nosftz			{ return  _NOSFTZ;}		

width				{ return WIDTH;}
height				{ return HEIGHT;}
depth				{ return DEPTH;}


	/*-- dataTypeId --*/
_u32				{ return  _U32;}
_s32				{ return  _S32;}
_s64				{ return  _S64;}
_u64				{ return  _U64;}
_b1					{ return  _B1;}
_b32				{ return  _B32;}
_f64				{ return  _F64;}
_f32				{ return  _F32;}
_b64				{ return  _B64;}
_b8					{ return  _B8;}
_b16				{ return  _B16;}
_s8					{ return  _S8;}
_s16				{ return  _S16;}
_u8					{ return  _U8;}
_u16				{ return  _U16;}
_f16				{ return  _F16;}
_b128				{ return  _B128;}
_u8x4				{ return  _U8X4;}
_s8x4				{ return  _S8X4;}
_u16x2				{ return  _U16X2;}
_s16x2				{ return  _S16X2;}
_f16x2				{ return  _F16X2;}
_f32x2				{ return  _F32X2;}
_u8x8				{ return  _U8X8;}
_s8x8				{ return  _S8X8;}
_u16x4				{ return  _U16X4;}
_s16x4				{ return  _S16X4;}
_f16x4				{ return  _F16X4;}
_u8x16				{ return  _U8X16;}
_s8x16				{ return  _S8X16;}
_u16x8				{ return  _U16X8;}
_s16x8				{ return  _S16X8;}
_f16x8				{ return  _F16X8;}
_f32x4				{ return  _F32X4;}
_s32x4				{ return  _S32X4;}
_u32x4				{ return  _U32X4;}
_f64x2				{ return  _F64X2;}
_s64x2				{ return  _S64X2;}
_u64x2				{ return  _U64X2;}


_ftz				{ return _FTZ;}
nop					{ return NOP;}
clock				{ return CLOCK;}
syscall				{ return SYSCALL;}


	/*-- mul --*/
mul				{ return MUL;}
mul_hi			{ return MUL_HI;}
mul24_hi		{ return MUL24_HI;}
mul24			{ return MUL24;}
mad24			{ return MAD24;}
mad24_hi		{ return MAD24_HI;}

f2u4			{ return F2U4;}



	/*-- Atom Modifiers --*/
_ar			{ return _AR;}
_region			{ return _REGION;}
atomic_cas		{ return ATOMIC_CAS;}
atomic			{ return ATOMIC;}
atomicNoRet		{ return ATOMICNORET;}
atomicNoRet_cas		{ return ATOMICNORET_CAS;}
atomic_image		{ return ATOMIC_IMAGE;}
atomicNoRet_image	{ return ATOMICNORET_IMAGE;}

cvt			{ return CVT;}


_dep			{ return _DEP;}
_equiv			{ return _EQUIV;}
_acq			{ return _ACQ;}
_rel			{ return _REL;}
ld			{ return LD;}
sync			{ return SYNC;}
barrier			{ return BARRIER;}
segmentp		{ return SEGMENTP;}
ftos			{ return FTOS;}
stof			{ return STOF; }
mov			{ return MOV;}
lad			{ return LAD;}
ldc			{ return LDC;}
ret			{ return RET;}
packedcmp		{ return PACKEDCMP;}
cmp			{ return CMP;}
st			{ return ST;}



	/* -- geometry ID -- */
_1d			{ return _1D;}
_2d			{ return _2D;}
_3d			{ return _3D;}
_1db			{ return _1DB;}
_1da			{ return _1DA;}
_2da			{ return _2DA;}

rd_image		{ return RD_IMAGE;}
ld_image		{ return LD_IMAGE;}
st_image		{ return ST_IMAGE;}




	/* -- queryOp --*/
query_order		{ return QUERY_ORDER;}
query_data		{ return QUERY_DATA;}
query_array		{ return QUERY_ARRAY;}
query_width		{ return QUERY_WIDTH;}
query_depth		{ return QUERY_DEPTH;}
query_height		{ return QUERY_HEIGHT;}
query_normalized	{ return QUERY_NORMALIZED;}
query_filtering		{ return QUERY_FILTERING;}



.	{ fprintf(stderr, "unexpected char"); return 0;}
