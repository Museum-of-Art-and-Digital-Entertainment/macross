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
