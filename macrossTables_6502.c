/*
 * Copyright (c) 1987 Fujitsu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
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
	"equal",	NULL,	ZERO_COND,
	"geq",		NULL,	GEQ_COND,
	"greater",	NULL,	GT_COND,
	"gt",		NULL,	GT_COND,
	"leq",		NULL,	LEQ_COND,
	"less",		NULL,	LT_COND,
	"lt",		NULL,	LT_COND,
	"minus",	NULL,	NEGATIVE_COND,
	"negative",	NULL,	NEGATIVE_COND,
	"neq",		NULL,	NOT_ZERO_COND,
	"never",	NULL,	NEVER_COND,
	"not_carry",	NULL,	NOT_CARRY_COND,		/* for dragon */
	"not_equal",	NULL,	NOT_ZERO_COND,		/* for dragon */
	"not_overflow",	NULL,	NOT_OVERFLOW_COND,	/* for dragon */
	"not_zero",	NULL,	NOT_ZERO_COND,		/* for dragon */
	"overflow",	NULL,	OVERFLOW_COND,
	"plus",		NULL,	NOT_NEGATIVE_COND,
	"positive",	NULL,	NOT_NEGATIVE_COND,
	"sgeq",		NULL,	SGEQ_COND,
	"sgt",		NULL,	SGT_COND,
	"sleq",		NULL,	SLEQ_COND,
	"slt",		NULL,	SLT_COND,
	"sneq",		NULL,	NOT_ZERO_COND,
	"zero",		NULL,	ZERO_COND,
	NULL,		NULL,	NEVER_COND,
		};

/* All those NULLs are used to string together lists after this all gets
   hashed */
keywordTableEntryType	theKeywords[] = {
	"a",		NULL,	A,
	"align",	NULL,	ALIGN,
	"assert",	NULL,	ASSERT,
	"block",	NULL,	BLOCK,
	"byte",		NULL,	BYTE,
	"constrain",	NULL,	CONSTRAIN,
	"dbyte",	NULL,	DBYTE,
	"define",	NULL,	DEFINE,
	"do",		NULL,	DO,
	"else",		NULL,	ELSE,
	"elseif",	NULL,	ELSEIF,
	"extern",	NULL,	EXTERN,
	"freturn",	NULL,	FRETURN,
	"function",	NULL,	FUNCTION,
	"here",		NULL,	HERE,
	"if",		NULL,	IF,
	"include",	NULL,	INCLUDE,
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
	"rel",		NULL,	REL,
	"start",	NULL,	START,
	"string",	NULL,	STRING,
	"struct",	NULL,	STRUCT,
	"target",	NULL,	TARGET,
	"undefine",	NULL,	UNDEFINE,
	"until",	NULL,	UNTIL,
	"variable",	NULL,	VARIABLE,
	"while",	NULL,	WHILE,
	"word",		NULL,	WORD,
	"x",		NULL,	X,
	"y",		NULL,	Y,
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
	"adc", NULL, 0x61, IMM_INDEX, IMM_INDEX_CLASS_BITS, 1,	1, 0,
	"and", NULL, 0x21, IMM_INDEX, IMM_INDEX_CLASS_BITS, 1,	1, 0,
	"asl", NULL, 0x02, DIR_X_1,   DIR_X_1_CLASS_BITS,   1,	1, 0,
	"bcc", NULL, 0x90, RELATIVE,  REL_CLASS_BITS,	    1,	1, 0,
	"bcs", NULL, 0xB0, RELATIVE,  REL_CLASS_BITS,	    1,	1, 0,
	"beq", NULL, 0xF0, RELATIVE,  REL_CLASS_BITS,	    1,	1, 0,
	"bit", NULL, 0x24, DIR_1,     DIR_1_CLASS_BITS,	    1,	1, 0,
	"bmi", NULL, 0x30, RELATIVE,  REL_CLASS_BITS,	    1,	1, 0,
	"bne", NULL, 0xD0, RELATIVE,  REL_CLASS_BITS,	    1,	1, 0,
	"bpl", NULL, 0x10, RELATIVE,  REL_CLASS_BITS,	    1,	1, 0,
	"brk", NULL, 0x00, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"bvc", NULL, 0x50, RELATIVE,  REL_CLASS_BITS,	    1,	1, 0,
	"bvs", NULL, 0x70, RELATIVE,  REL_CLASS_BITS,	    1,	1, 0,
	"clc", NULL, 0x18, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"cld", NULL, 0xD8, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"cli", NULL, 0x58, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"clv", NULL, 0xB8, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"cmp", NULL, 0xC1, IMM_INDEX, IMM_INDEX_CLASS_BITS, 1,	1, 0,
	"cpx", NULL, 0xE0, IMM_DIR,   IMM_DIR_CLASS_BITS,   1,	1, 0,
	"cpy", NULL, 0xC0, IMM_DIR,   IMM_DIR_CLASS_BITS,   1,	1, 0,
	"dec", NULL, 0xC6, DIR_X_2,   DIR_X_2_CLASS_BITS,   1,	1, 0,
	"dex", NULL, 0xCA, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"dey", NULL, 0x88, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"eor", NULL, 0x41, IMM_INDEX, IMM_INDEX_CLASS_BITS, 1,	1, 0,
	"inc", NULL, 0xE6, DIR_X_2,   DIR_X_2_CLASS_BITS,   1,	1, 0,
	"inx", NULL, 0xE8, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"iny", NULL, 0xC8, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"jmp", NULL, 0x4C, DIR_INDIR, DIR_INDIR_CLASS_BITS, 1,	1, 0,
	"jsr", NULL, 0x20, DIR_2,     DIR_2_CLASS_BITS,	    1,	1, 0,
	"lda", NULL, 0xA1, IMM_INDEX, IMM_INDEX_CLASS_BITS, 1,	1, 0,
	"ldx", NULL, 0xA2, IMM_DIR_Y, IMM_DIR_Y_CLASS_BITS, 1,	1, 0,
	"ldy", NULL, 0xA0, IMM_DIR_X, IMM_DIR_X_CLASS_BITS, 1,	1, 0,
	"lsr", NULL, 0x42, DIR_X_1,   DIR_X_1_CLASS_BITS,   1,	1, 0,
	"nop", NULL, 0xEA, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"ora", NULL, 0x01, IMM_INDEX, IMM_INDEX_CLASS_BITS, 1,	1, 0,
	"pha", NULL, 0x48, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"php", NULL, 0x08, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"pla", NULL, 0x68, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"plp", NULL, 0x28, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"rol", NULL, 0x22, DIR_X_1,   DIR_X_1_CLASS_BITS,   1,	1, 0,
	"ror", NULL, 0x62, DIR_X_1,   DIR_X_1_CLASS_BITS,   1,	1, 0,
	"rti", NULL, 0x40, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"rts", NULL, 0x60, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"sbc", NULL, 0xE1, IMM_INDEX, IMM_INDEX_CLASS_BITS, 1,	1, 0,
	"sec", NULL, 0x38, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"sed", NULL, 0xF8, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"sei", NULL, 0x78, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"sta", NULL, 0x81, INDEX,     INDEX_CLASS_BITS,	    1,	1, 0,
	"stx", NULL, 0x86, DIR_Y,     DIR_Y_CLASS_BITS,	    1,	1, 0,
	"sty", NULL, 0x84, DIR_X_3,   DIR_X_3_CLASS_BITS,   1,	1, 0,
	"tax", NULL, 0xAA, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"tay", NULL, 0xA8, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"tsx", NULL, 0xBA, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"txa", NULL, 0x8A, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"txs", NULL, 0x9A, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	"tya", NULL, 0x98, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
	NULL,  NULL, 0x00, NONE,      NONE_CLASS_BITS,	    0,	0, 0,
};

int operandClassTable[] = {	/* indexed by operandKindType */
	EXPRESSION_OPND_BIT,
	IMMEDIATE_OPND_BIT,
	INDIRECT_OPND_BIT,
	A_REGISTER_OPND_BIT,
	X_REGISTER_OPND_BIT,
	Y_REGISTER_OPND_BIT,
	POST_INDEXED_Y_OPND_BIT,
	PRE_INDEXED_X_OPND_BIT,
	X_INDEXED_OPND_BIT,
	Y_INDEXED_OPND_BIT,
	X_SELECTED_OPND_BIT,
	Y_SELECTED_OPND_BIT,
	PRE_SELECTED_X_OPND_BIT,
	STRING_OPND_BIT,
	BLOCK_OPND_BIT,
};

int	actionsRelative();
int	actionsDir1();
int	actionsDir2();
int	actionsDirIndir();
int	actionsDirX1();
int	actionsDirX2();
int	actionsDirX3();
int	actionsDirY();
int	actionsImmDir();
int	actionsImmDirX();
int	actionsImmDirY();
int	actionsNone();
int	actionsIndex();
int	actionsImmIndex();

/* indexed by opcodeClass */
int		       (*instructionActionTable[])() = {
	actionsRelative,
	actionsDir1,
	actionsDir2,
	actionsDirIndir,
	actionsDirX1,
	actionsDirX2,
	actionsDirX3,
	actionsDirY,
	actionsImmDir,
	actionsImmDirX,
	actionsImmDirY,
	actionsNone,
	actionsIndex,
	actionsImmIndex,
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
valueType	*arrayLengthBIF();
valueType	*atasciiBIF();
valueType	*atasciiColorBIF();
valueType	*debugModeOffBIF();
valueType	*debugModeOnBIF();
valueType	*emitModeOffBIF();
valueType	*emitModeOnBIF();
valueType	*isAbsoluteValueBIF();
valueType	*isARegisterBIF();
valueType	*isBlockBIF();
valueType	*isBuiltInFunctionBIF();
valueType	*isConditionCodeBIF();
valueType	*isDefinedBIF();
valueType	*isDirectModeBIF();
valueType	*isExternalBIF();
valueType	*isFieldBIF();
valueType	*isFunctionBIF();
valueType	*isImmediateModeBIF();
valueType	*isIndexedModeBIF();
valueType	*isIndirectModeBIF();
valueType	*isPostIndexedModeBIF();
valueType	*isPreIndexedModeBIF();
valueType	*isRelocatableValueBIF();
valueType	*isStringBIF();
valueType	*isStructBIF();
valueType	*isSymbolBIF();
valueType	*isXIndexedModeBIF();
valueType	*isXRegisterBIF();
valueType	*isYIndexedModeBIF();
valueType	*isYRegisterBIF();
valueType	*listingOffBIF();
valueType	*listingOnBIF();
valueType	*makeArrayBIF();
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
	int		 ordinal;
		} builtInFunctionTable[] = {
	"addressMode",		addressModeBIF,		FALSE,	-1,
	"apply",		applyBIF,		FALSE,	-1,
	"arrayLength",		arrayLengthBIF,		FALSE,	-1,
	"atascii",		atasciiBIF,		FALSE,	 0,
	"atasciiColor",		atasciiColorBIF,	FALSE,	 1,
	"debugModeOff",		debugModeOffBIF,	FALSE,	-1,
	"debugModeOn",		debugModeOnBIF,		FALSE,	-1,
	"emitModeOff",		emitModeOffBIF,		FALSE,	-1,
	"emitModeOn",		emitModeOnBIF,		FALSE,	-1,
	"isAbsoluteValue",	isAbsoluteValueBIF,	FALSE,	 2,
	"isARegister",		isARegisterBIF,		FALSE,	-1,
	"isBlock",		isBlockBIF,		FALSE,	-1,
	"isBuiltInFunction",	isBuiltInFunctionBIF,	FALSE,	-1,
	"isConditionCode",	isConditionCodeBIF,	FALSE,	 3,
	"isDefined",		isDefinedBIF,		TRUE,	 4,
	"isDirectMode",		isDirectModeBIF,	FALSE,	-1,
	"isExternal",		isExternalBIF,		TRUE,	 5,
	"isField",		isFieldBIF,		FALSE,	-1,
	"isFunction",		isFunctionBIF,		FALSE,	-1,
	"isImmediateMode",	isImmediateModeBIF,	FALSE,	-1,
	"isIndexedMode",	isIndexedModeBIF,	FALSE,	-1,
	"isIndirectMode",	isIndirectModeBIF,	FALSE,	-1,
	"isPostIndexedMode",	isPostIndexedModeBIF,	FALSE,	-1,
	"isPreIndexedMode",	isPreIndexedModeBIF,	FALSE,	-1,
	"isRelocatableValue",	isRelocatableValueBIF,	FALSE,	-1,
	"isString",		isStringBIF,		FALSE,	-1,
	"isStruct",		isStructBIF,		FALSE,	-1,
	"isSymbol",		isSymbolBIF,		TRUE,	-1,
	"isXIndexedMode",	isXIndexedModeBIF,	FALSE,	-1,
	"isXRegister",		isXRegisterBIF,		FALSE,	-1,
	"isYIndexedMode",	isYIndexedModeBIF,	FALSE,	-1,
	"isYRegister",		isYRegisterBIF,		FALSE,	-1,
	"listingOff",		listingOffBIF,		FALSE,	-1,
	"listingOn",		listingOnBIF,		FALSE,	-1,
	"makeArray",		makeArrayBIF,		FALSE,	-1,
	"nthChar",		nthCharBIF,		FALSE,	 6,
	"printf",		printfBIF,		FALSE,	 7,
	"strcat",		strcatBIF,		FALSE,	 8,
	"strcmp",		strcmpBIF,		FALSE,	 9,
	"strcmplc",		strcmplcBIF,		FALSE,	10,
	"strlen",		strlenBIF,		FALSE,	11,
	"substr",		substrBIF,		FALSE,	12,
	"symbolDefine",		symbolDefineBIF,	FALSE,	-1,
	"symbolLookup",		symbolLookupBIF,	FALSE,	13,
	"symbolName",		symbolNameBIF,		TRUE,	14,
	"symbolUsage",		symbolUsageBIF,		TRUE,	-1,
	"valueType",		valueTypeBIF,		FALSE,	-1,
	NULL,			NULL,			FALSE,	-1,
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
