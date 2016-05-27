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
	errorStuff.c -- Error handling for the Macross assembler

	Chip Morningstar -- Lucasfilm Ltd.

	5-November-1984
*/

#include "macrossTypes.h"
#include "macrossGlobals.h"
#include "errorStuff.h"
#include "initialize.h"

#include <stdarg.h>

bool			 nullStatementFlag;

/* puntOnError handles syntax errors and the like encountered during parsing
   or lexical analysis: it issues the relevent error message and scans the
   input up to the end of the line, in a (probably futile) effort to recover
   from the booboo. */

  void
puntOnError(errorType theError, ...)
{
	va_list ap;
	char	c;

	while ((c = getc(input))!='\n' && c!=EOF)
		;
	ungetc(c, input);

	va_start(ap, theError);
	verror(theError, ap);
	va_end(ap);
}


/* printErrorMessage is the general error message handler */

  void
printErrorMessage(errorType theError, va_list ap)
{
/* This table MUST be maintained congruently with the definition of the
   enumerated type 'errorType'. */
	static bool dying = FALSE;

	static char *errorMessageStrings[] = {
		"assignment in 'define' statement is not '='",
		"digit '%c' is not valid in base %d",
		"closing \"'\" missing in character constant",
		"closing '\"' missing in string constant",
		"octal value '0%o' too large to fit in a character",
		"character '%c' not allowed here",
		"comment not closed at end of file",
		"line comment not closed at end of file",
		"fatal: unable to open temporary scratch file '%s'",
		"%s cannot be a valid address for an 'org'",
		"negative block size in 'block' statement",
		"'%s' is %s and hence unassignable",
		"left and right hand sides of assignment are incompatible for '%s' operator",
		"operands of '%s' are incompatible with each other",
		"postincrement operand is %s and hence un-postincrementable",
		"postdecrement operand is %s and hence un-postdecrementable",
		"preincrement operand is %s and hence un-preincrementable",
		"predecrement operand is %s and hence un-predecrementable",
		"operand of '%s' is incompatible with operator",
		"value is not an address",
		"value is not an integer",
		"can't 'align' to zero or negative value",
		"left hand operand of '%s' has no value",
		"too many operands for '%s' instruction (should have %d)",
		"illegal address mode for '%s' instruction",
		"value %d is too large to fit in a byte",
		"value %d is too large to fit in a word",
		"illegal address mode for '%s' instruction",
		"divide by zero",
		"symbol '%s' undefined",
		"relative branch offset too large",
		"label '%s' previously defined",
		"fatal: can't open object file '%s'",
		"%s can't be made external",
		"warning: more than one output file name given on command line",
		"warning: '-%c' is not a recognized command line flag",
		"fatal: no output file name given on command line after '-o'",
		"fatal: unable to open input file '%s'",
		"fatal: unable to open include file '%s'",
		"%s cannot be a valid boolean value",
		"symbol '%s' was previously defined",
		"assignment to undeclared variable '%s'",
		"redefinition of variable '%s'",
		"'%s' is not a struct",
		"struct is too large for all components to be addressed",
		"struct field '%s' previously defined",
		"struct definition inside a struct definition",
		"'%s' is not a struct field",
		"macro '%s' previously defined",
		"macro definition inside a macro or function",
		"too many arguments given to macro '%s' (expects no more than %d)",
		"function definition inside a macro or function",
		"function '%s' previously defined",
		"too many arguments given to function '%s' (expects no more than %d)",
		"'%s' is not a function",
		"function '%s' did not return a value",
		"operand already has an address mode",
		"multiple address modes in expression",
		"include file name given is not a string",
		"start address previously given",
		"attempt to use function name '%s' as a value",
		"statement with side-effects not allowed inside a function",
		"symbolName can't find symbol",
		"error inside macro '%s'",
		"error inside function '%s'",
		"fatal: no listing file name given on command line after '-l'",
		"fatal: no symbol dump file name given on command line after '-%c'",
		"warning: more than one listing file name given on command line",
		"warning: more than one symbol dump file name given on command line",
		"fatal: can't open listing file '%s'",
		"fatal: can't open symbol dump file '%s'",
		"fatal: source file '%s' does not have a Macross source file name",
		"fatal: output file '%s' has a Macross source file name",
		"fatal: list file '%s' has a Macross source file name",
		"fatal: symbol dump file '%s' has a Macross source file name",
		"constraint value is not an integer",
		"constrained block crosses 0x%x boundary",
		"symbol '%s' has no value",
		"bad start address",
		"align inside constraint block -- alignment ignored",
		"constrain inside constraint block -- constraint ignored",
		"can't emit %s as a word",
		"can't emit %s as a byte",
		"too few operands for '%s' instruction (should have %d)",
		"bad color argument to 'atasciiColor'",
		"'printf' format argument is not a string",
		"'%s' argument #%d is not a string",
		"no arguments given to '%s'",
		"'%s' argument is not a string",
		"too few arguments to '%s'",
		"'%s' argument #%d is not an absolute value",
		"bad substring indices",
		"invalid 'assert' condition",
		"invalid 'assert' message",
		"'assert' failed: %s",
		"'assert' failed",
		"can't use %s as a switch value",
		"can't use %s as a case value",
		"can't forward reference a block",
		"warning: turning macro expansion listing off because listing in general is off",

		"inappropriate address modes",
		"can't do byte address op",
		"quick data out of range",
		"ccr op is byte op",
		"sr op is word op",
		"shift op is word op",
		"cmp address is not byte op",
		"usp move is long op",
		"ccr move is word op",
		"sr move is word op",
		"cr move is long op",
		"address move not byte op",
		"trap data too large",
		"relative offset value too large",
		"can't do op from A register",
		"forward reference not allowed here",
		"fatal: address 0x%x is outside the available address space",
		"argument '%s' to 'apply' is not a macro name",
		"can't 'target' in relocatable mode",
		"%s cannot be a valid address for a 'target'",
		"macro '%s' is undefined",
		"bad position argument to 'nthChar'",
		"value argument to 'symbolDefine' is not an expression operand",
		"attempt to use %s as an array",
		"attempt to index an array with %s",
		"array index of %d exceeds maximum of %d",
		"too many initialization values for variable",
		"negative array index",
		"can't emit %s as a long",
		"can't use ++ or -- on an element of a string",
		"can't assign a non-character value into a string element",
		"negative array size given to 'makeArray'",
		"too many initialization values given to 'makeArray'",
		"array element has no value",
		"can't use prefix '%s' in an expression containing a forward reference",
		"can't use postfix '%s' in an expression containing a forward reference",
		"can't use assignment in an expression containing a forward reference",
		"fatal: too many errors (choke, gasp, wheeze, die...)",
		"expression too large for object file encoding",
		"operator assignment not allowed in object file expression",
		"temporary symbol '%s' in object file expression",
		"block operand in object file expression",
		"register operand in object file expression",
		"selection operand in object file expression (not yet)",
		"%s not allowed in object file expression",
		"array term not allowed in object file expression",
		"%s not allowed in object file function",
		"array mvariable declaration in object file function",
		"built-in function '%s' is not available in the linker",
		"fatal: no name definition given on command line after '-D'",
		"bad name definition: '%s'",
		"warning: perform statement has no side effects",
		"value %d is too large to fit in a bit",
		"fatal: no processor given on command line after '-P'",
		"fatal: unknown processor: %s",

	};

	static int errorCount = 0;

	if (!terseErrorMessages || currentFileName != lastErrorFile ||
			currentLineNumber != lastErrorLine) {
		lastErrorFile = currentFileName;
		lastErrorLine = currentLineNumber;
		printf("\"%s\", line %d: ", currentFileName, currentLineNumber
			-1);
		vprintf(errorMessageStrings[(int)theError], ap);
		printf("\n");
		fflush(stdout);
	}
	if (++errorCount > ERROR_LIMIT && !dying) {
		dying = TRUE;
		fatalError(TOO_MANY_ERRORS_ERROR);
	}
}

  void
error(errorType theError, ...)
{
	va_list ap;
	va_start(ap, theError);
	printErrorMessage(theError, ap);
	va_end(ap);
	errorFlag = TRUE;
}

  void
verror(errorType theError, va_list ap)
{
	printErrorMessage(theError, ap);
	errorFlag = TRUE;
}

  void
warning(errorType theError, ...)
{
	va_list ap;
	va_start(ap, theError);
	printErrorMessage(theError, ap);
	va_end(ap);
}

  void
fatalError(errorType theError, ...)
{
	va_list ap;
	va_start(ap, theError);
	printErrorMessage(theError, ap);
	va_end(ap);
	chokePukeAndDie();
}

  void
fatalSystemError(errorType theError, ...)
{
	va_list ap;
	va_start(ap, theError);
	printErrorMessage(theError, ap);
	va_end(ap);
	perror("Unix says");
	chokePukeAndDie();
}

/* yyerror is what yacc expects to call when it hits an error.  Yacc passes
   the error message as a string (this is almost always 'syntax error'). */

  void
yyerror(char *s)
{
	printf("\"%s\", line %d: %s\n", currentFileName, currentLineNumber,s);
	fflush(stdout);
	errorFlag = TRUE;
}


/* usageString is used in reporting errors having to do with symbol usage.
   It provides a translation between the internal representation and
   something printable (with the printable something designed to be embedded
   in a sentence). */

  char *
usageString(symbolUsageKindType usageKind)
{
/* This table MUST be maintained congruently with the definition of the
   enumerated type 'symbolUsageKindType'. */

	static char *usageStrings[NUM_OF_SYM_USAGES] = {
		"a struct name",
		"a struct field name",
		"a macro name",
		"an argument variable name",
		"a label",
		"an external symbol",
		"a variable",
		"a macro variable",
		"a symbol of unknown type",
		"a dead symbol",
		"a function name",
		"a built-in-function name",
		"a nested symbol of unknown type",
		"a define symbol",
		"a macro define symbol",
		"an unknown function",
		"an unknown macro",
	};
	return(usageStrings[(int)usageKind]);
}


/* valueKindString similarly deals with the different kinds of values. */

  char *
valueKindString(valueKindType valueKind)
{
/* This table MUST be maintained congruently with the definition of the
   enumerated type 'valueKindType'. */

	static char *valueKindStrings[] = {
		"an absolute value",
		"a data value",
		"a relocatable value",
		"a bss value",
		"a struct name",
		"a field name",
		"a macro name",
		"an operand value",
		"a string value",
		"a condition code",
		"an undefined value",
		"a function name",
		"a block of statements",
		"a built in function",
		"an array",
		"a failed expression"
	};
	return (valueKindStrings[(int)valueKind]);
}

/* assignmentString similarly handles assignments */

  char *
assignmentString(assignmentKindType assignment)
{
/* This table MUST be maintained congruently with the definition of the
   enumerated type 'assignmentKindType'. */

	static char *assignmentStringTable[] = {
		"=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=",
		"<<=", ">>=", "macross botch! no assigment"
	};
	return(assignmentStringTable[(int)assignment]);
}
