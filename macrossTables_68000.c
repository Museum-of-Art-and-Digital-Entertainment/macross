/*
	macrossTables.c -- Define the contents of various tables and values
		of various initialized global variables.

	Chip Morningstar -- Lucasfilm Ltd.

	5-November-1984
*/

#include "macrossTypes.h"
#include "y.tab.h"

/* All those NULLs are used to string together lists after this all gets
   hashed */
conditionTableEntryType	 theConditions[] = {
	"always",	NULL,	ALWAYS_COND,
	"carry",	NULL,	CARRY_COND,
	"equal",	NULL,	EQUAL_COND,
	"geq",		NULL,	GEQ_COND,
	"greater",	NULL,	GT_COND,
	"gt",		NULL,	GT_COND,
	"high",		NULL,	HIGH_COND,
	"high_same",	NULL,	NOT_CARRY_COND,
	"hs",		NULL,	NOT_CARRY_COND,
	"less",		NULL,	LT_COND,
	"leq",		NULL,	LEQ_COND,
	"low",		NULL,	CARRY_COND,
	"low_same",	NULL,	LOW_OR_SAME_COND,
	"ls",		NULL,	LOW_OR_SAME_COND,
	"lt",		NULL,	LT_COND,
	"minus",	NULL,	MINUS_COND,
	"negative",	NULL,	MINUS_COND,
	"neq",		NULL,	NOT_EQUAL_COND,
	"never",	NULL,	NEVER_COND,
	"not_carry",	NULL,	NOT_CARRY_COND,		/* for dragon */
	"not_equal",	NULL,	NOT_EQUAL_COND,		/* for dragon */
	"not_overflow",	NULL,	NOT_OVERFLOW_COND,	/* for dragon */
	"not_zero",	NULL,	NOT_EQUAL_COND,		/* for dragon */
	"overflow",	NULL,	OVERFLOW_COND,
	"plus",		NULL,	PLUS_COND,
	"positive",	NULL,	PLUS_COND,
	"zero",		NULL,	EQUAL_COND,
	NULL,		NULL,	NEVER_COND,
		};

/* All those NULLs are used to string together lists after this all gets
   hashed */
keywordTableEntryType	theKeywords[] = {
	"a0",		NULL,	A0,
	"a1",		NULL,	A1,
	"a2",		NULL,	A2,
	"a3",		NULL,	A3,
	"a4",		NULL,	A4,
	"a5",		NULL,	A5,
	"a6",		NULL,	A6,
	"a7",		NULL,	A7,
	"align",	NULL,	ALIGN,
	"assert",	NULL,	ASSERT,
	"block",	NULL,	BLOCK,
	"byte",		NULL,	BYTE,
	"cc",		NULL,	CCR,
	"ccr",		NULL,	CCR,
	"constrain",	NULL,	CONSTRAIN,
	"d0",		NULL,	D0,
	"d1",		NULL,	D1,
	"d2",		NULL,	D2,
	"d3",		NULL,	D3,
	"d4",		NULL,	D4,
	"d5",		NULL,	D5,
	"d6",		NULL,	D6,
	"d7",		NULL,	D7,
	"dbyte",	NULL,	DBYTE,
	"define",	NULL,	DEFINE,
	"dfc",		NULL,	DFC,
	"do",		NULL,	DO,
	"else",		NULL,	ELSE,
	"elseif",	NULL,	ELSEIF,
	"extern",	NULL,	EXTERN,
	"freturn",	NULL,	FRETURN,
	"function",	NULL,	FUNCTION,
	"here",		NULL,	HERE,
	"if",		NULL,	IF,
	"include",	NULL,	INCLUDE,
	"l",		NULL,	L,
	"long",		NULL,	LONG,
	"macro",	NULL,	MACRO,
	"mcase",	NULL,	MCASE,
	"mdefault",	NULL,	MDEFAULT,
	"mdefine",	NULL,	MDEFINE,
	"mdo",		NULL,	MDO,
	"melse",	NULL,	MELSE,
	"melseif",	NULL,	MELSEIF,
	"mfor",		NULL,	MFOR,
	"mif",		NULL,	MIF,
	"mswitch",	NULL,	MSWITCH,
	"muntil",	NULL,	MUNTIL,
	"mvariable",	NULL,	MVARIABLE,
	"mwhile",	NULL,	MWHILE,
	"org",		NULL,	ORG,
	"pc",		NULL,	PC,
	"rel",		NULL,	REL,
	"sfc",		NULL,	SFC,
	"sr",		NULL,	SR,
	"start",	NULL,	START,
	"string",	NULL,	STRING,
	"struct",	NULL,	STRUCT,
	"target",	NULL,	TARGET,
	"undefine",	NULL,	UNDEFINE,
	"until",	NULL,	UNTIL,
	"usp",		NULL,	USP,
	"variable",	NULL,	VARIABLE,
	"vbr",		NULL,	VBR,
	"w",		NULL,	W,
	"while",	NULL,	WHILE,
	"word",		NULL,	WORD,
	NULL,		NULL,	0,
		};

#define HASH_TABLE_SIZE	509

macroTableEntryType	*macroTable[HASH_TABLE_SIZE];

opcodeTableEntryType	*opcodeTable[HASH_TABLE_SIZE];

symbolTableEntryType	*symbolTable[HASH_TABLE_SIZE];

keywordTableEntryType	*keywordTable[HASH_TABLE_SIZE];

conditionTableEntryType	*conditionTable[HASH_TABLE_SIZE];

/* All those NULLs are used to string together lists after this all gets
   hashed. */
opcodeTableEntryType	 theOpcodes[] = {
/* mnemonic, next, opcode, class, allowed modes, min#operands, max#operands,
 subclass/alternate opcode */
	"abcd",	NULL,	0xC100,	CLASS_I,	CLASS_I_BITS,	2,  2, 0,
	"addb",	NULL,	0xD000,	CLASS_II,	CLASS_II_BITS,	2,  2, 0x0600,
	"addl",	NULL,	0xD080,	CLASS_II,	CLASS_II_BITS,	2,  2, 0x0680,
	"addqb",NULL,	0x5000,	CLASS_III,	CLASS_III_BITS,	2,  2, 0,
	"addql",NULL,	0x5080,	CLASS_III,	CLASS_III_BITS,	2,  2, 0,
	"addqw",NULL,	0x5040,	CLASS_III,	CLASS_III_BITS,	2,  2, 0,
	"addw",	NULL,	0xD040,	CLASS_II,	CLASS_II_BITS,	2,  2, 0x0640,
	"addxb",NULL,	0xD100,	CLASS_I,	CLASS_I_BITS,	2,  2, 0,
	"addxl",NULL,	0xD180,	CLASS_I,	CLASS_I_BITS,	2,  2, 0,
	"addxw",NULL,	0xD140,	CLASS_I,	CLASS_I_BITS,	2,  2, 0,
	"andb",	NULL,	0xC000,	CLASS_IV,	CLASS_IV_BITS,	2,  2, 0x0200,
	"andl",	NULL,	0xC080,	CLASS_IV,	CLASS_IV_BITS,	2,  2, 0x0280,
	"andw",	NULL,	0xC040,	CLASS_IV,	CLASS_IV_BITS,	2,  2, 0x0240,
	"aslb",	NULL,	0xE100,	CLASS_V,	CLASS_V_BITS,	1,  2, 00,
	"asll",	NULL,	0xE180,	CLASS_V,	CLASS_V_BITS,	1,  2, 00,
	"aslw",	NULL,	0xE140,	CLASS_V,	CLASS_V_BITS,	1,  2, 00,
	"asrb",	NULL,	0xE000,	CLASS_V,	CLASS_V_BITS,	1,  2, 00,
	"asrl",	NULL,	0xE080,	CLASS_V,	CLASS_V_BITS,	1,  2, 00,
	"asrw",	NULL,	0xE040,	CLASS_V,	CLASS_V_BITS,	1,  2, 00,
	"bcc",	NULL,	0x64,	CLASS_VI,	CLASS_VI_BITS,	1,  1, 0,
	"bchg",	NULL,	0x0040,	CLASS_VII,	CLASS_VII_BITS,	2,  2, 0,
	"bclr",	NULL,	0x0080,	CLASS_VII,	CLASS_VII_BITS,	2,  2, 0,
	"bcs",	NULL,	0x65,	CLASS_VI,	CLASS_VI_BITS,	1,  1, 0,
	"beq",	NULL,	0x67,	CLASS_VI,	CLASS_VI_BITS,	1,  1, 0,
	"bge",	NULL,	0x6C,	CLASS_VI,	CLASS_VI_BITS,	1,  1, 0,
	"bgt",	NULL,	0x6E,	CLASS_VI,	CLASS_VI_BITS,	1,  1, 0,
	"bhi",	NULL,	0x62,	CLASS_VI,	CLASS_VI_BITS,	1,  1, 0,
	"ble",	NULL,	0x6F,	CLASS_VI,	CLASS_VI_BITS,	1,  1, 0,
	"bls",	NULL,	0x63,	CLASS_VI,	CLASS_VI_BITS,	1,  1, 0,
	"blt",	NULL,	0x6D,	CLASS_VI,	CLASS_VI_BITS,	1,  1, 0,
	"bmi",	NULL,	0x6B,	CLASS_VI,	CLASS_VI_BITS,	1,  1, 0,
	"bne",	NULL,	0x66,	CLASS_VI,	CLASS_VI_BITS,	1,  1, 0,
	"bpl",	NULL,	0x6A,	CLASS_VI,	CLASS_VI_BITS,	1,  1, 0,
	"bra",	NULL,	0x60,	CLASS_VI,	CLASS_VI_BITS,	1,  1, 0,
	"bset",	NULL,	0x00C0,	CLASS_VII,	CLASS_VII_BITS,	2,  2, 0,
	"bsr",	NULL,	0x61,	CLASS_VI,	CLASS_VI_BITS,	1,  1, 0,
	"btst",	NULL,	0x0000,	CLASS_VII,	CLASS_VII_BITS,	2,  2, 0,
	"bvc",	NULL,	0x68,	CLASS_VI,	CLASS_VI_BITS,	1,  1, 0,
	"bvs",	NULL,	0x69,	CLASS_VI,	CLASS_VI_BITS,	1,  1, 0,
	"chk",	NULL,	0x4180,	CLASS_VIII,	CLASS_VIII_BITS,2,  2, 0,
	"clrb",	NULL,	0x4200,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"clrl",	NULL,	0x4280,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"clrw",	NULL,	0x4240,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"cmpb",	NULL,	0x0000,	CLASS_X,	CLASS_X_BITS,	2,  2, 0x0000,
	"cmpl",	NULL,	0x0080,	CLASS_X,	CLASS_X_BITS,	2,  2, 0xB1C0,
	"cmpmb",NULL,	0xB108,	CLASS_XI,	CLASS_XI_BITS,	2,  2, 0,
	"cmpml",NULL,	0xB188,	CLASS_XI,	CLASS_XI_BITS,	2,  2, 0,
	"cmpmw",NULL,	0xB148,	CLASS_XI,	CLASS_XI_BITS,	2,  2, 0,
	"cmpw",	NULL,	0x0040,	CLASS_X,	CLASS_X_BITS,	2,  2, 0xB0C0,
	"dbcc",	NULL,	0x54C8,	CLASS_XII,	CLASS_XII_BITS,	2,  2, 0,
	"dbcs",	NULL,	0x55C8,	CLASS_XII,	CLASS_XII_BITS,	2,  2, 0,
	"dbeq",	NULL,	0x57C8,	CLASS_XII,	CLASS_XII_BITS,	2,  2, 0,
	"dbf",	NULL,	0x51C8,	CLASS_XII,	CLASS_XII_BITS,	2,  2, 0,
	"dbge",	NULL,	0x5CC8,	CLASS_XII,	CLASS_XII_BITS,	2,  2, 0,
	"dbgt",	NULL,	0x5EC8,	CLASS_XII,	CLASS_XII_BITS,	2,  2, 0,
	"dbhi",	NULL,	0x52C8,	CLASS_XII,	CLASS_XII_BITS,	2,  2, 0,
	"dble",	NULL,	0x5FC8,	CLASS_XII,	CLASS_XII_BITS,	2,  2, 0,
	"dbls",	NULL,	0x53C8,	CLASS_XII,	CLASS_XII_BITS,	2,  2, 0,
	"dblt",	NULL,	0x5DC8,	CLASS_XII,	CLASS_XII_BITS,	2,  2, 0,
	"dbmi",	NULL,	0x5BC8,	CLASS_XII,	CLASS_XII_BITS,	2,  2, 0,
	"dbne",	NULL,	0x56C8,	CLASS_XII,	CLASS_XII_BITS,	2,  2, 0,
	"dbpl",	NULL,	0x5BC8,	CLASS_XII,	CLASS_XII_BITS,	2,  2, 0,
	"dbra",	NULL,	0x50C8,	CLASS_XII,	CLASS_XII_BITS,	2,  2, 0,
	"dbt",	NULL,	0x50C8,	CLASS_XII,	CLASS_XII_BITS,	2,  2, 0,
	"dbvc",	NULL,	0x58C8,	CLASS_XII,	CLASS_XII_BITS,	2,  2, 0,
	"dbvs",	NULL,	0x59C8,	CLASS_XII,	CLASS_XII_BITS,	2,  2, 0,
	"divs",	NULL,	0x81C0,	CLASS_VIII,	CLASS_VIII_BITS,2,  2, 0,
	"divu",	NULL,	0x80C0,	CLASS_VIII,	CLASS_VIII_BITS,2,  2, 0,
	"eorb",	NULL,	0xB100,	CLASS_XIII,	CLASS_XIII_BITS,2,  2, 0x0A00,
	"eorl",	NULL,	0xB180,	CLASS_XIII,	CLASS_XIII_BITS,2,  2, 0x0A80,
	"eorw",	NULL,	0xB140,	CLASS_XIII,	CLASS_XIII_BITS,2,  2, 0x0A40,
	"exg",	NULL,	0xC100,	CLASS_XIV,	CLASS_XIV_BITS,	2,  2, 0,
	"extl",	NULL,	0x48C0,	CLASS_XV,	CLASS_XV_BITS,	1,  1, 0,
	"extw",	NULL,	0x4880,	CLASS_XV,	CLASS_XV_BITS,	1,  1, 0,
	"illegal",NULL,	0x4AFC,	CLASS_XVI,	CLASS_XVI_BITS,	0,  0, 0,
	"jmp",	NULL,	0x4EC0,	CLASS_XVII,	CLASS_XVII_BITS,1,  1, 0,
	"jsr",	NULL,	0x4E80,	CLASS_XVII,	CLASS_XVII_BITS,1,  1, 0,
	"lea",	NULL,	0x41C0,	CLASS_XVIII,	CLASS_XVIII_BITS,2, 2, 0,
	"link",	NULL,	0x4E50,	CLASS_XIX,	CLASS_XIX_BITS,	2,  2, 0,
	"lslb",	NULL,	0xE100,	CLASS_V,	CLASS_V_BITS,	1,  2, 01,
	"lsll",	NULL,	0xE180,	CLASS_V,	CLASS_V_BITS,	1,  2, 01,
	"lslw",	NULL,	0xE140,	CLASS_V,	CLASS_V_BITS,	1,  2, 01,
	"lsrb",	NULL,	0xE000,	CLASS_V,	CLASS_V_BITS,	1,  2, 01,
	"lsrl",	NULL,	0xE080,	CLASS_V,	CLASS_V_BITS,	1,  2, 01,
	"lsrw",	NULL,	0xE040,	CLASS_V,	CLASS_V_BITS,	1,  2, 01,
	"movb",	NULL,	0x1000,	CLASS_XX,	CLASS_XX_BITS,	2,  2, 0x0000,
	"moveb",NULL,	0x1000,	CLASS_XX,	CLASS_XX_BITS,	2,  2, 0x0000,
	"movel",NULL,	0x2000,	CLASS_XX,	CLASS_XX_BITS,	2,  2, 0x2040,
	"moveml",NULL,	0x48C0,	CLASS_XXI,	CLASS_XXI_BITS,	2, 17, 0,
	"movemw",NULL,	0x4880,	CLASS_XXI,	CLASS_XXI_BITS,	2, 17, 0,
	"movepl",NULL,	0x0148,	CLASS_XXII,	CLASS_XXII_BITS,2,  2, 0,
	"movepw",NULL,	0x0108,	CLASS_XXII,	CLASS_XXII_BITS,2,  2, 0,
	"moveq",NULL,	0x70,	CLASS_XXIII,	CLASS_XXIII_BITS,2, 2, 0,
	"movesb",NULL,	0x0E00,	CLASS_XXIV,	CLASS_XXIV_BITS,2,  2, 0,
	"movesl",NULL,	0x0E80,	CLASS_XXIV,	CLASS_XXIV_BITS,2,  2, 0,
	"movesw",NULL,	0x0E40,	CLASS_XXIV,	CLASS_XXIV_BITS,2,  2, 0,
	"movew",NULL,	0x3000,	CLASS_XX,	CLASS_XX_BITS,	2,  2, 0x3040,
	"movl",	NULL,	0x2000,	CLASS_XX,	CLASS_XX_BITS,	2,  2, 0x2040,
	"movml",NULL,	0x48C0,	CLASS_XXI,	CLASS_XXI_BITS,	2, 17, 0,
	"movmw",NULL,	0x4880,	CLASS_XXI,	CLASS_XXI_BITS,	2, 17, 0,
	"movpl",NULL,	0x0148,	CLASS_XXII,	CLASS_XXII_BITS,2,  2, 0,
	"movpw",NULL,	0x0108,	CLASS_XXII,	CLASS_XXII_BITS,2,  2, 0,
	"movq",	NULL,	0x70,	CLASS_XXIII,	CLASS_XXIII_BITS,2, 2, 0,
	"movsb",NULL,	0x0E00,	CLASS_XXIV,	CLASS_XXIV_BITS,2,  2, 0,
	"movsl",NULL,	0x0E80,	CLASS_XXIV,	CLASS_XXIV_BITS,2,  2, 0,
	"movsw",NULL,	0x0E40,	CLASS_XXIV,	CLASS_XXIV_BITS,2,  2, 0,
	"movw",	NULL,	0x3000,	CLASS_XX,	CLASS_XX_BITS,	2,  2, 0x3040,
	"muls",	NULL,	0xC1C0,	CLASS_VIII,	CLASS_VIII_BITS,2,  2, 0,
	"mulu",	NULL,	0xC0C0,	CLASS_VIII,	CLASS_VIII_BITS,2,  2, 0,
	"nbcd",	NULL,	0x4800,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"negb",	NULL,	0x4400,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"negl",	NULL,	0x4480,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"negw",	NULL,	0x4440,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"negxb",NULL,	0x4000,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"negxl",NULL,	0x4080,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"negxw",NULL,	0x4040,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"nop",	NULL,	0x4E71,	CLASS_XVI,	CLASS_XVI_BITS,	0,  0, 0,
	"notb",	NULL,	0x4600,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"notl",	NULL,	0x4680,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"notw",	NULL,	0x4640,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"orb",	NULL,	0x8000,	CLASS_IV,	CLASS_IV_BITS,	2,  2, 0x0000,
	"orl",	NULL,	0x8080,	CLASS_IV,	CLASS_IV_BITS,	2,  2, 0x0080,
	"orw",	NULL,	0x8040,	CLASS_IV,	CLASS_IV_BITS,	2,  2, 0x0040,
	"pea",	NULL,	0x4840,	CLASS_XVII,	CLASS_XVII_BITS,1,  1, 0,
	"reset",NULL,	0x4E70,	CLASS_XVI,	CLASS_XVI_BITS,	0,  0, 0,
	"rolb",	NULL,	0xE100,	CLASS_V,	CLASS_V_BITS,	1,  2, 02,
	"roll",	NULL,	0xE180,	CLASS_V,	CLASS_V_BITS,	1,  2, 02,
	"rolw",	NULL,	0xE140,	CLASS_V,	CLASS_V_BITS,	1,  2, 02,
	"rorb",	NULL,	0xE000,	CLASS_V,	CLASS_V_BITS,	1,  2, 02,
	"rorl",	NULL,	0xE080,	CLASS_V,	CLASS_V_BITS,	1,  2, 02,
	"rorw",	NULL,	0xE040,	CLASS_V,	CLASS_V_BITS,	1,  2, 02,
	"roxlb",NULL,	0xE100,	CLASS_V,	CLASS_V_BITS,	1,  2, 03,
	"roxll",NULL,	0xE180,	CLASS_V,	CLASS_V_BITS,	1,  2, 03,
	"roxlw",NULL,	0xE140,	CLASS_V,	CLASS_V_BITS,	1,  2, 03,
	"roxrb",NULL,	0xE000,	CLASS_V,	CLASS_V_BITS,	1,  2, 03,
	"roxrl",NULL,	0xE080,	CLASS_V,	CLASS_V_BITS,	1,  2, 03,
	"roxrw",NULL,	0xE040,	CLASS_V,	CLASS_V_BITS,	1,  2, 03,
	"rtd",	NULL,	0x4E74,	CLASS_XXV,	CLASS_XXV_BITS,	1,  1, 0,
	"rte",	NULL,	0x4E73,	CLASS_XVI,	CLASS_XVI_BITS,	0,  0, 0,
	"rtr",	NULL,	0x4E77,	CLASS_XVI,	CLASS_XVI_BITS,	0,  0, 0,
	"rts",	NULL,	0x4E75,	CLASS_XVI,	CLASS_XVI_BITS,	0,  0, 0,
	"sbcd",	NULL,	0x8100,	CLASS_I,	CLASS_I_BITS,	2,  2, 0,
	"scc",	NULL,	0x54C0,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"scs",	NULL,	0x55C0,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"seq",	NULL,	0x57C0,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"sf",	NULL,	0x51C0,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"sge",	NULL,	0x5CC0,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"sgt",	NULL,	0x5EC0,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"shi",	NULL,	0x52C0,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"sle",	NULL,	0x5FC0,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"sls",	NULL,	0x53C0,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"slt",	NULL,	0x5DC0,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"smi",	NULL,	0x5BC0,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"sne",	NULL,	0x56C0,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"spl",	NULL,	0x5AC0,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"st",	NULL,	0x50C0,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"stop",	NULL,	0x4E72,	CLASS_XXVII,	CLASS_XXVII_BITS,1, 1, 0,
	"subb",	NULL,	0x9000,	CLASS_II,	CLASS_II_BITS,	2,  2, 0x0400,
	"subl",	NULL,	0x9080,	CLASS_II,	CLASS_II_BITS,	2,  2, 0x0480,
	"subqb",NULL,	0x5100,	CLASS_III,	CLASS_III_BITS,	2,  2, 0,
	"subql",NULL,	0x5180,	CLASS_III,	CLASS_III_BITS,	2,  2, 0,
	"subqw",NULL,	0x5140,	CLASS_III,	CLASS_III_BITS,	2,  2, 0,
	"subw",	NULL,	0x9040,	CLASS_II,	CLASS_II_BITS,	2,  2, 0x0440,
	"subxb",NULL,	0x9100,	CLASS_I,	CLASS_I_BITS,	2,  2, 0,
	"subxl",NULL,	0x9180,	CLASS_I,	CLASS_I_BITS,	2,  2, 0,
	"subxw",NULL,	0x9140,	CLASS_I,	CLASS_I_BITS,	2,  2, 0,
	"svc",	NULL,	0x58C0,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"svs",	NULL,	0x59C0,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"swap",	NULL,	0x4840,	CLASS_XV,	CLASS_XV_BITS,	1,  1, 0,
	"tas",	NULL,	0x4AC0,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"trap",	NULL,	0x4E40,	CLASS_XXVIII,	CLASS_XXVIII_BITS,1,1, 0,
	"trapv",NULL,	0x4E76,	CLASS_XVI,	CLASS_XVI_BITS,	0,  0, 0,
	"tstb",	NULL,	0x4A00,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"tstl",	NULL,	0x4A80,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"tstw",	NULL,	0x4A40,	CLASS_IX,	CLASS_IX_BITS,	1,  1, 0,
	"unlk",	NULL,	0x4E58,	CLASS_XXVI,	CLASS_XXVI_BITS,1,  1, 0,
	NULL,	NULL,	0x0000,	CLASS_XVI,     	0,	        0,  0, 0,
};

int operandClassTable[] = {	/* indexed by operandKindType */
	EXPRESSION_OPND_BIT,
	STRING_OPND_BIT,
	BLOCK_OPND_BIT,
	D_REGISTER_OPND_BIT,
	A_REGISTER_OPND_BIT,
	A_REGISTER_INDIRECT_OPND_BIT,
	POSTINCREMENT_OPND_BIT,
	PREDECREMENT_OPND_BIT,
	DISPLACEMENT_OPND_BIT,
	INDEXED_OPND_BIT,
	PC_DISPLACEMENT_OPND_BIT,
	PC_INDEXED_OPND_BIT,
	IMMEDIATE_OPND_BIT,
	ABSOLUTE_SHORT_OPND_BIT,
	ABSOLUTE_LONG_OPND_BIT,
	CC_REGISTER_OPND_BIT,
	STATUS_REGISTER_OPND_BIT,
	USP_REGISTER_OPND_BIT,
	CONTROL_REGISTER_OPND_BIT,
	SELECTED_OPND_BIT,
	INDEX_SELECTED_OPND_BIT,
};

int	actionsClassI();
int	actionsClassII();
int	actionsClassIII();
int	actionsClassIV();
int	actionsClassV();
int	actionsClassVI();
int	actionsClassVII();
int	actionsClassVIII();
int	actionsClassIX();
int	actionsClassX();
int	actionsClassXI();
int	actionsClassXII();
int	actionsClassXIII();
int	actionsClassXIV();
int	actionsClassXV();
int	actionsClassXVI();
int	actionsClassXVII();
int	actionsClassXVIII();
int	actionsClassXIX();
int	actionsClassXX();
int	actionsClassXXI();
int	actionsClassXXII();
int	actionsClassXXIII();
int	actionsClassXXIV();
int	actionsClassXXV();
int	actionsClassXXVI();
int	actionsClassXXVII();
int	actionsClassXXVIII();

/* indexed by opcodeClass */
int		       (*instructionActionTable[])() = {
	actionsClassI,
	actionsClassII,
	actionsClassIII,
	actionsClassIV,
	actionsClassV,
	actionsClassVI,
	actionsClassVII,
	actionsClassVIII,
	actionsClassIX,
	actionsClassX,
	actionsClassXI,
	actionsClassXII,
	actionsClassXIII,
	actionsClassXIV,
	actionsClassXV,
	actionsClassXVI,
	actionsClassXVII,
	actionsClassXVIII,
	actionsClassXIX,
	actionsClassXX,
	actionsClassXXI,
	actionsClassXXII,
	actionsClassXXIII,
	actionsClassXXIV,
	actionsClassXXV,
	actionsClassXXVI,
	actionsClassXXVII,
	actionsClassXXVIII,
};

/* indexed by symbolUsageKindType */
int	validSymbolValues[NUM_OF_SYM_USAGES] = {
	/* STRUCT_NAME_SYMBOL */	STRUCT_VALUE_BIT,
	/* STRUCT_FIELD_SYMBOL */	FIELD_VALUE_BIT,
	/* MACRO_SYMBOL */		MACRO_VALUE_BIT,
	/* ARGUMENT_SYMBOL */		OPERAND_VALUE_BIT,
	/* LABEL_SYMBOL */		ABSOLUTE_VALUE_BIT | RELOCATABLE_VALUE_BIT |
					DATA_VALUE_BIT | BSS_VALUE_BIT,
	/* EXTERNAL_SYMBOL */		ABSOLUTE_VALUE_BIT | RELOCATABLE_VALUE_BIT |
					DATA_VALUE_BIT | BSS_VALUE_BIT |
					UNDEFINED_VALUE_BIT,
	/* VARIABLE_SYMBOL */		ABSOLUTE_VALUE_BIT | RELOCATABLE_VALUE_BIT |
					DATA_VALUE_BIT | BSS_VALUE_BIT |
					UNDEFINED_VALUE_BIT |
					STRING_VALUE_BIT | OPERAND_VALUE_BIT |
					CONDITION_VALUE_BIT,
	/* MVARIABLE_SYMBOL */		ABSOLUTE_VALUE_BIT | RELOCATABLE_VALUE_BIT |
					DATA_VALUE_BIT | BSS_VALUE_BIT |
					UNDEFINED_VALUE_BIT |
					STRING_VALUE_BIT | OPERAND_VALUE_BIT |
					CONDITION_VALUE_BIT,
	/* UNKNOWN_SYMBOL */		UNDEFINED_VALUE_BIT,
	/* FUNCTION_SYMBOL */		FUNCTION_VALUE_BIT,
	/* BUILT_IN_FUNCTION_SYMBOL */	BUILT_IN_FUNCTION_VALUE_BIT,
	/* NESTED_UNKNOWN_SYMBOL */	UNDEFINED_VALUE_BIT,
	/* DEFINE_SYMBOL */		OPERAND_VALUE_BIT,
	/* MDEFINE_SYMBOL */		OPERAND_VALUE_BIT,
	/* UNKNOWN_FUNCTION_SYMBOL */	UNDEFINED_VALUE_BIT,
	/* UNKNOWN_MACRO_SYMBOL */	UNDEFINED_VALUE_BIT,
};

/* indexed by valueKindType */
int	valueBitTable[] = {
		ABSOLUTE_VALUE_BIT,
		DATA_VALUE_BIT,
		RELOCATABLE_VALUE_BIT,
		BSS_VALUE_BIT,
		STRUCT_VALUE_BIT,
		FIELD_VALUE_BIT,
		MACRO_VALUE_BIT,
		OPERAND_VALUE_BIT,
		STRING_VALUE_BIT,
		CONDITION_VALUE_BIT,
		UNDEFINED_VALUE_BIT,
		FUNCTION_VALUE_BIT,
		BLOCK_VALUE_BIT,
		BUILT_IN_FUNCTION_VALUE_BIT,
		ARRAY_VALUE_BIT,
		FAIL_BIT,
};


codeRegionType		 absoluteCodeRegion;
codeRegionType		 relocatableCodeRegion;
codeRegionType		*codeRegions[2] = {
				&absoluteCodeRegion,
				&relocatableCodeRegion
};

/* A predefined undefined value so we don't have to make a new one every
   time we need one */
valueType		 undefinedValueValue = { UNDEFINED_VALUE, 0,
					EXPRESSION_OPND };
valueType		*UndefinedValue = &undefinedValueValue;

valueType	*addressModeBIF();
valueType	*applyBIF();
valueType	*atasciiBIF();
valueType	*atasciiColorBIF();
valueType	*debugModeOffBIF();
valueType	*debugModeOnBIF();
valueType	*emitModeOffBIF();
valueType	*emitModeOnBIF();
valueType	*getAddressRegisterBIF();
valueType	*getDataRegisterBIF();
valueType	*getIndexRegisterBIF();
valueType	*getRegisterBIF();
valueType	*getWLBIF();
valueType	*isARegisterBIF();
valueType	*isAbsoluteLongModeBIF();
valueType	*isAbsoluteModeBIF();
valueType	*isAbsoluteShortModeBIF();
valueType	*isAbsoluteValueBIF();
valueType	*isBlockBIF();
valueType	*isBuiltInFunctionBIF();
valueType	*isCCRegisterBIF();
valueType	*isConditionCodeBIF();
valueType	*isControlRegisterBIF();
valueType	*isDFCRegisterBIF();
valueType	*isDRegisterBIF();
valueType	*isDefinedBIF();
valueType	*isDisplacementModeBIF();
valueType	*isExternalBIF();
valueType	*isFieldBIF();
valueType	*isFunctionBIF();
valueType	*isImmediateModeBIF();
valueType	*isIndexedModeBIF();
valueType	*isIndirectModeBIF();
valueType	*isPCDisplacementModeBIF();
valueType	*isPCIndexedModeBIF();
valueType	*isPostincrementModeBIF();
valueType	*isPredecrementModeBIF();
valueType	*isRelocatableValueBIF();
valueType	*isSFCRegisterBIF();
valueType	*isStatusRegisterBIF();
valueType	*isStringBIF();
valueType	*isStructBIF();
valueType	*isSymbolBIF();
valueType	*isUSPBIF();
valueType	*isVBRegisterBIF();
valueType	*listingOffBIF();
valueType	*listingOnBIF();
valueType	*nthCharBIF();
valueType	*printfBIF();
valueType	*strcatBIF();
valueType	*strcmpBIF();
valueType	*strcmplcBIF();
valueType	*strlenBIF();
valueType	*substrBIF();
valueType	*symbolDefineBIF();
valueType	*symbolLookupBIF();
valueType	*symbolNameBIF();
valueType	*symbolUsageBIF();
valueType	*valueTypeBIF();

/* Used to initialize symbols representing built-in functions */
struct {
	stringType	*functionName;
	valueType	*(*functionEntry)();
	bool		 isSpecialFunction;
		} builtInFunctionTable[] = {
	"addressMode",		addressModeBIF,		FALSE,
	"apply",		applyBIF,		FALSE,
	"atascii",		atasciiBIF,		FALSE,
	"atasciiColor",		atasciiColorBIF,	FALSE,
	"debugModeOff",		debugModeOffBIF,	FALSE,
	"debugModeOn",		debugModeOnBIF,		FALSE,
	"emitModeOff",		emitModeOffBIF,		FALSE,
	"emitModeOn",		emitModeOnBIF,		FALSE,
	"getAddressRegister",	getAddressRegisterBIF,	FALSE,
	"getDataRegister",	getDataRegisterBIF,	FALSE,
	"getIndexRegister",	getIndexRegisterBIF,	FALSE,
	"getRegister",		getRegisterBIF,		FALSE,
	"getWL",		getWLBIF,		FALSE,
	"isARegister",		isARegisterBIF,		FALSE,
	"isAbsoluteLongMode",	isAbsoluteLongModeBIF,	FALSE,
	"isAbsoluteMode",	isAbsoluteModeBIF,	FALSE,
	"isAbsoluteShortMode",	isAbsoluteShortModeBIF,	FALSE,
	"isAbsoluteValue",	isAbsoluteValueBIF,	FALSE,
	"isBlock",		isBlockBIF,		FALSE,
	"isBuiltInFunction",	isBuiltInFunctionBIF,	FALSE,
	"isCCRegister",		isCCRegisterBIF,	FALSE,
	"isConditionCode",	isConditionCodeBIF,	FALSE,
	"isControlRegister",	isControlRegisterBIF,	FALSE,
	"isDFCRegister",	isDFCRegisterBIF,	FALSE,
	"isDRegister",		isDRegisterBIF,		FALSE,
	"isDefined",		isDefinedBIF,		TRUE,
	"isDisplacementMode",	isDisplacementModeBIF,	FALSE,
	"isExternal",		isExternalBIF,		TRUE,
	"isField",		isFieldBIF,		FALSE,
	"isFunction",		isFunctionBIF,		FALSE,
	"isImmediateMode",	isImmediateModeBIF,	FALSE,
	"isIndexedMode",	isIndexedModeBIF,	FALSE,
	"isIndirectMode",	isIndirectModeBIF,	FALSE,
	"isPCDisplacementMode",	isPCDisplacementModeBIF,FALSE,
	"isPCIndexedMode",	isPCIndexedModeBIF,	FALSE,
	"isPostincrementMode",	isPostincrementModeBIF,	FALSE,
	"isPredecrementMode",	isPredecrementModeBIF,	FALSE,
	"isRelocatableValue",	isRelocatableValueBIF,	FALSE,
	"isSFCRegister",	isSFCRegisterBIF,	FALSE,
	"isStatusRegister",	isStatusRegisterBIF,	FALSE,
	"isString",		isStringBIF,		FALSE,
	"isStruct",		isStructBIF,		FALSE,
	"isSymbol",		isSymbolBIF,		TRUE,
	"isUSP",		isUSPBIF,		FALSE,
	"isVBRegister",		isVBRegisterBIF,	FALSE,
	"listingOff",		listingOffBIF,		FALSE,
	"listingOn",		listingOnBIF,		FALSE,
	"nthChar",		nthCharBIF,		FALSE,
	"printf",		printfBIF,		FALSE,
	"strcat",		strcatBIF,		FALSE,
	"strcmp",		strcmpBIF,		FALSE,
	"strcmplc",		strcmplcBIF,		FALSE,
	"strlen",		strlenBIF,		FALSE,
	"substr",		substrBIF,		FALSE,
	"symbolDefine",		symbolDefineBIF,	FALSE,
	"symbolLookup",		symbolLookupBIF,	FALSE,
	"symbolName",		symbolNameBIF,		TRUE,
	"symbolUsage",		symbolUsageBIF,		TRUE,
	"valueType",		valueTypeBIF,		FALSE,
	NULL,			NULL,			FALSE,
};

/* Used to initialize predefined symbols */
struct {
	stringType	*symbolName;
	int		 symbolValue;
		} predefinedSymbolTable[] = {
	"FALSE",	0,
	"NULL",		0,
	"TRUE",		1,
	NULL,		0,
};

/* These define the temporary files used to hold scratch data used in the
   generation of listings.  The "XXXXXX"s get blasted by 'mktemp' */
char			 pass2SourceFileName[] = "/tmp/zsourceXXXXXX";
char			 pass2IndexFileName[] = "/tmp/zindexXXXXXX";
char			 pass2MacroExpansionFileName[] = "/tmp/zmacroXXXXXX";
