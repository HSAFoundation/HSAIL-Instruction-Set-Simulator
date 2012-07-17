%option noyywrap
%{
#include <stdio.h>
#include "tokens.h"

int int_val;
%}
OCT_INT		([+|-]?0[0-7]*)
DEC_INT		([+|-]?(0|([1-9]*[0-9]+)))
HEX_INT		([+|-]?0(x|X)[0-9a-fA-F]*)	
%x INLINE_COMMENT
%x COMMENT
DEC_SINGLE	([+|-]?(([0-9]*("."[0-9]*)?((e|E)[+|-]?[0-9]*)?)|([0-9]+(e|E)[+|-]?[0-9]*))(f|F))

%%
{OCT_INT} { int_val = strtol(yytext, NULL, 8); return TOKEN_INTEGER_CONSTANT; } 
{DEC_INT} { int_val = strtol(yytext, NULL, 10); return TOKEN_INTEGER_CONSTANT; }
{HEX_INT} { int_val = strtol(yytext, NULL, 16); return TOKEN_INTEGER_CONSTANT; }

{DEC_SINGLE} { return 5; }

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

"/*"					{ BEGIN (COMMENT); }
<COMMENT>"*/"			{ BEGIN (INITIAL); return TOKEN_COMMENT; }
<COMMENT>([^*]|\n)+|./"*/" 	{}
<COMMENT><<EOF>>		{ printf("%d: Unterminated comment\n", yylineno); return 0; }

"//"					{ BEGIN(INLINE_COMMENT); }
<INLINE_COMMENT>\n		{ BEGIN(INITIAL); return TOKEN_COMMENT;}
<INLINE_COMMENT><<EOF>>		{ printf("%d: Unterminated comment\n", yylineno); return 0; }

\$c(0|1|2|3|4|5|6|7) 	{ return TOKEN_CREGISTER;}
\$d(0|1|2|3|4|5|6|7) 	{ return TOKEN_DREGISTER;}
\$s(0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15) { return TOKEN_SREGISTER;}
\$q(0|1|2|3|4|5|6|7) 	{ return TOKEN_QREGISTER;}

\%[_a-zA-Z]+[_a-zA-Z0-9]*  	{return TOKEN_LOCAL_IDENTIFIER; }
\&[_a-zA-Z]+[_a-zA-Z0-9]*	{return TOKEN_GLOBAL_IDENTIFIER; }

\@[_a-zA-Z]+[_a-zA-Z0-9]* 	{ return TOKEN_LABEL; }
\"(\\.|[^\\"])*\" 		{ return TOKEN_STRING; }

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
.	{ fprintf(stderr, "unexpected char"); return 0;}
