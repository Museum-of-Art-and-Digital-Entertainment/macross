/*
	tokenStrings_68000.c -- Target processor dependent string generators
		for the Macross assembler (68000 version).

	Chip Morningstar -- Lucasfilm Ltd.

	26-April-1985

*/

#include "macrossTypes.h"
#include "macrossGlobals.h"

/* conditionString similarly deals with condition codes */
  char *
conditionString(condition)
  conditionType	condition;
{
/* This table MUST be maintained congruently with the definition of the
	enumerated type 'conditionType' */

	static char *conditionStringTable[] = {
		"(carry)",
		"(equal)",
		"(overflow)",
		"(minus)",
		"(lt)",
		"(leq)",
		"(ls)",
		"(always)",
		"(!carry)",
		"(!equal)",
		"(!overflow)",
		"(plus)",
		"(geq)",
		"(gt)",
		"(high)",
		"(never)",
	};
	return(conditionStringTable[(int)condition]);
}


/* tokenString similarly deals with parser tokens */

  char *
tokenString(token)
  int	token;
{
/* This table MUST be maintained congruently with the token definitions in
   the file 'y.tab.h' as output by yacc. */

	static char *tokenTable[] = {
		"A0",
		"A1",
		"A2",
		"A3",
		"A4",
		"A5",
		"A6",
		"A7",
		"ALIGN",
		"ASSERT",
		"BLOCK",
		"BYTE",
		"CCR",
		"CONSTRAIN",
		"D0",
		"D1",
		"D2",
		"D3",
		"D4",
		"D5",
		"D6",
		"D7",
		"DBYTE",
		"DEFINE",
		"DFC",
		"DO",
		"ELSE",
		"ELSEIF",
		"ENDFILE",
		"EOL",
		"EXTERN",
		"FRETURN",
		"FUNCTION",
		"HERE",
		"IF",
		"INCLUDE",
		"L",
		"LONG",
		"MACRO",
		"MCASE",
		"MDEFAULT",
		"MDEFINE",
		"MDO",
		"MELSE",
		"MELSEIF",
		"MFOR",
		"MIF",
		"MSWITCH",
		"MUNTIL",
		"MVARIABLE",
		"MWHILE",
		"ORG",
		"PC",
		"REL",
		"SFC",
		"SR",
		"START",
		"STRING",
		"STRUCT",
		"TARGET",
		"UNDEFINE",
		"UNTIL",
		"USP",
		"VARIABLE",
		"VBR",
		"W",
		"WHILE",
		"WORD",
		"X",
		"Y",
		"ConditionCode",
		"Identifier",
		"MacroName",
		"Number",
		"Opcode",
		"TextString",
		"Assignment",
		"||",
		"^^",
		"&&",
		"|",
		"^",
		"&",
		"==",
		"!=",
		"<",
		"<=",
		">",
		">=",
		"<<",
		">>",
		"+",
		"-",
		"*",
		"/",
		"%",
		"-",
		"!",
		"~",
		"?",
		"/",
		".",
		"++",
		"--",
	};
	return(tokenTable[token-257]);
}
