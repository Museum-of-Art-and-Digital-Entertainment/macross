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
	builtins.c -- Built-in functions for the Slinky linker

	Chip Morningstar -- Lucasfilm Ltd.

	14-November-1985
*/

#include "slinkyTypes.h"
#include "slinkyGlobals.h"
#include "slinkyExpressions.h"
#include "builtins.h"
#include "errorStuff.h"
#include "expr.h"
#include "link.h"
#include "relocate.h"
#include <string.h>

#define getSymbol() ((symbolType *)getNumber())

  void
tooFewArgs(int argCount, stringType *name)
{
	error(TOO_FEW_ARGUMENTS_TO_BIF_ERROR, name);
	while (argCount-- > 0)
		skipExpression();
}

  void
tooManyArgs(int argCount, stringType *name)
{
	error(TOO_MANY_ARGUMENTS_TO_BIF_ERROR, name);
	while (argCount-- > 0)
		skipExpression();
}

/*
   The built-in-functions themselves: pointers to these are loaded into the
   builtInFunctionTable at linker compile time.  These are all known within
   the Macross assembler: each symbol of the form "xxx" is assigned a pointer
   to "xxxBIF" (BIF == Built-In-Function).
 */

/* The two ATASCII related BIF's refer to this table -- This really only
   makes sense for the 6502 version, but is harmless in other versions. */
static char	 atasciiTable[] = { /* 0xFFs will become 0x00s on output */
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
		0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
		0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
		0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
		0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
		0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
		0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
		0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
		0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0xFF,
};

/* Convert a string to ATASCII */
   stringType *
atasciiBIF(int argCount)
{
	stringType	*string;
	stringType	*newString;
	int		 i;

	if (argCount == 0) {
		return("");
	} else if (argCount > 1) {
		tooManyArgs(argCount, "atascii");
		return("");
	} else {
		string = (stringType *)evaluateExpression();
		newString = (stringType *)malloc(strlen(string)+1);
		for (i=0; string[i]!='\0'; i++)
			newString[i] = atasciiTable[string[i]];
		newString[i] = '\0';
		return(newString);
	}
}

/* Convert a string to ATASCII while setting high-order color bits */
  stringType *
atasciiColorBIF(int argCount)
{
	stringType	*string;
	stringType	*newString;
	int		 color;
	int		 i;
	byte		 testChar;

	if (argCount == 0) {
		return("");
	} else if (argCount == 1) {
		return(atasciiBIF(argCount));
	} else if (argCount > 2) {
		tooManyArgs(argCount, "atasciiColor");
		return("");
	} else {
		string = (stringType *)evaluateExpression();
		color = (evaluateExpression() & 0x03) << 6;
		newString = (stringType *)malloc(strlen(string)+1);
		for (i=0; string[i]!='\0'; i++) {
			testChar = atasciiTable[string[i]];
			if (testChar == 0xFF)
				testChar = 0;
			testChar = (testChar & 0x3F) | color;
			if (testChar == 0)
				testChar = 0xFF;
			newString[i] = testChar;
		}
		newString[i] = '\0';
		return(newString);
	}
}

/* Check if an operand is absolute (as opposed to relocatable) */
  bool
isAbsoluteValueBIF(int argCount)
{
	if (argCount > 1)
		tooManyArgs(argCount, "isAbsoluteValue");
	return(TRUE);
}

/* Check if operand is a condition code */
  bool
isConditionCodeBIF(int argCount)
{
	bool	result;

	if (argCount == 0)
		return(FALSE);
	else if (argCount > 1) {
		tooManyArgs(argCount, "isConditionCode");
		return(FALSE);
	}
	result = (*pc == CONDITION_CODE_TAG);
	skipExpression();
	return(result);
}

/* Check if a symbol is defined */
  bool
isDefinedBIF(int argCount)
{
	symbolType	*symbol;

	if (argCount == 0)
		return(FALSE);
	else if (argCount > 1) {
		tooManyArgs(argCount, "isDefined");
		return(FALSE);
	}
	if (*pc != IDENTIFIER_TAG) {
		skipExpression();
		return(TRUE);
	}
	pc++;	/* skip tag */
	symbol = getSymbol();
	return((symbol->symbolClass & ~SYMBOL_EXTERNAL) != 0);
}

/* Check if a symbol is externally visible */
  bool
isExternalBIF(int argCount)
{
	symbolType	*symbol;

	if (argCount == 0)
		return(FALSE);
	else if (argCount > 1) {
		tooManyArgs(argCount, "isExternal");
		return(FALSE);
	}
	if (*pc != IDENTIFIER_TAG) {
		skipExpression();
		return(TRUE);
	}
	pc++;	/* skip tag */
	symbol = getSymbol();
	return((symbol->symbolClass & SYMBOL_EXTERNAL) != 0);
}

/* Return the Nth character of a string (as an integer) */
  int
nthCharBIF(int argCount)
{
	stringType	*string;
	int		 position;

	if (argCount > 2) {
		tooManyArgs(argCount, "nthChar");
		return(0);
	} else if (argCount != 2) {
		tooFewArgs(argCount, "nthChar");
		return(0);
	}
	string = (stringType *)evaluateExpression();
	position = evaluateExpression();
	if (position >= strlen(string)) {
		error(BAD_POSITION_ARGUMENT_TO_NTH_CHAR_ERROR);
		return(0);
	}
	return(string[position]);
}

/* Pass stuff through to stdio's 'printf' function */
  int
printfBIF(int argCount)
{
	stringType	*formatString;
	int		 argument[20];
	int		 i;

	if (argCount < 1) {
		tooFewArgs(argCount, "printf");
		return(0);
	} else if (argCount > 21) {
		tooManyArgs(argCount, "printf");
		return(0);
	}
	formatString = (stringType *)evaluateExpression();
	argCount--;
	for (i=0; i<argCount; ++i)
		argument[i] = evaluateExpression();
	/* cretinous hack */
	return(printf(formatString, argument[0], argument[1], argument[2],
		argument[3], argument[4], argument[5], argument[6],
		argument[7], argument[8], argument[9], argument[10],
		argument[11], argument[12], argument[13],
		argument[14], argument[15], argument[16],
		argument[17], argument[18], argument[19]));
}

/* Concatenate two strings */
  stringType *
strcatBIF(int argCount)
{
	stringType	*string1;
	stringType	*string2;
	stringType	*newString;

	if (argCount == 0)
		return("");
	else if (argCount == 1)
		return((stringType *)evaluateExpression());
	else if (argCount > 2) {
		tooManyArgs(argCount, "strcat");
		return("");
	}
	string1 = (stringType *)evaluateExpression();
	string2 = (stringType *)evaluateExpression();
	newString = (stringType *)malloc(strlen(string1)+strlen(string2)+1);
	strcpy(newString, string1);
	strcat(newString, string2);
	return(newString);
}

/* Compare two strings */
  int
strcmpBIF(int argCount)
{
	stringType	*string1;
	stringType	*string2;

	if (argCount < 2) {
		tooFewArgs(argCount, "strcmp");
		return(0);
	} else if (argCount > 2) {
		tooManyArgs(argCount, "strcmp");
		return(0);
	}
	string1 = (stringType *)evaluateExpression();
	string2 = (stringType *)evaluateExpression();
	return(strcmp(string1, string2));
}

/* Compare two strings in a case-independent fashion */
  int
strcmplcBIF(int argCount)
{
	stringType	*string1;
	stringType	*string2;

	if (argCount < 2) {
		tooFewArgs(argCount, "strcmplc");
		return(0);
	} else if (argCount > 2) {
		tooManyArgs(argCount, "strcmplc");
		return(0);
	}
	string1 = (stringType *)evaluateExpression();
	string2 = (stringType *)evaluateExpression();
	return(strcmplc(string1, string2));
}

/* Return the length of a string */
  int
strlenBIF(int argCount)
{
	if (argCount < 1)
		return(0);
	else if (argCount > 1) {
		tooManyArgs(argCount, "strlen");
		return(0);
	}
	return(strlen(evaluateExpression()));
}

/* Return a substring of a string */
  char *
substrBIF(int argCount)
{
	stringType	*string;
	int		 start;
	int		 length;
	stringType	*newString;
	int		 originalLength;
	bool		 backwards;

	if (argCount < 1)
		return("");
	else if (argCount == 1)
		return((stringType *)evaluateExpression());
	else if (argCount > 3) {
		tooManyArgs(argCount, "substr");
		return("");
	}
	string = (stringType *)evaluateExpression();
	originalLength = strlen(string);
	start = evaluateExpression();
	if (start < 0) {
		start = -start - 1;
		backwards = TRUE;
	} else
		backwards = FALSE;
	if (argCount == 2) {
		length = originalLength - start;
		if (backwards)
			length = -length;
	} else {
		length = evaluateExpression();
	}
	if (length < 0) {
		length = -length;
		if (backwards)
			start = start + length - 1;
		else
			start = start - length + 1;
	}
	if (backwards)
		start = originalLength - start - 1;
	if (originalLength <= start || originalLength < length + start ||
			start < 0 ){
		error(BAD_SUBSTRING_INDICES_ERROR);
		return("");
	}
	newString = (stringType *)malloc(length+1);
	strncpy(newString, string+start, length);
	newString[length] = '\0';
	return(newString);
}

/* Turn a string into a symbol and return its value */
  addressType
symbolLookupBIF(int argCount)
{
	symbolType	*symbol;
	stringType	*symbolName;

	if (argCount < 1) {
		tooFewArgs(argCount, "symbolLookup");
		return(0);
	} else if (argCount > 1) {
		tooManyArgs(argCount, "symbolLookup");
		return(0);
	}
	symbol = lookupGlobalSymbol(symbolName = (stringType *)
					evaluateExpression());
	if (symbol == NULL) {
		error(SYMBOL_NOT_FOUND_IN_SYMBOL_LOOKUP_ERROR, symbolName);
		return(0);
	} else
		return(symbol->symbolValue);
}

/* Turn a symbol into a string */
  stringType *
symbolNameBIF(int argCount)
{
	symbolType	*symbol;

	if (argCount < 1) {
		tooFewArgs(argCount, "symbolName");
		return(NULL);
	} else if (argCount > 1) {
		tooManyArgs(argCount, "symbolName");
		return(NULL);
	}
	if (*pc != IDENTIFIER_TAG) {
		error(SYMBOL_NAME_ARG_IS_NOT_SYMBOL_ERROR);
		skipExpression();
		return("");
	}
	pc++;
	symbol = getSymbol();
	return(symbol->symbolName);
}
