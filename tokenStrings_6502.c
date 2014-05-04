/*
	tokenStrings_6502.c -- Target processor dependent string generators
		for the Macross assembler (6502 version).

	Chip Morningstar -- Lucasfilm Ltd.

	23-April-1985

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
		"(zero)",
		"(minus)",
		"(overflow)",
		"(lt)",
		"(leq)",
		"(slt)",
		"(sleq)",
		"(always)",
		"(!carry)",
		"(!zero)",
		"(plus)",
		"(!overflow)",
		"(geq)",
		"(gt)",
		"(sgeq)",
		"(sgt)",
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
		"A",
		"ALIGN",
		"ASSERT",
		"BLOCK",
		"BYTE",
		"CONSTRAIN",
		"DBYTE",
		"DEFINE",
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
		"REL",
		"START",
		"STRING",
		"STRUCT",
		"TARGET",
		"UNDEFINE",
		"UNTIL",
		"VARIABLE",
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
