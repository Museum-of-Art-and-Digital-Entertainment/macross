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
	errorStuff.c -- Error handling for the Slinky linker.

	Chip Morningstar -- Lucasfilm Ltd.

	9-March-1985
*/

#include "slinkyTypes.h"
#include "slinkyGlobals.h"
#include "errorStuff.h"

#include <stdarg.h>

  static void
verror(errorType theError, va_list ap)
{
/* This table MUST be maintained congruently with the definition of the
   enumerated type 'errorType'. */

	static char *errorMessageStrings[] = {
		"no file name on command line after '-o'",
		"unknown command line flag '-%c'",
		"more than one output file name given on command line",
		"can't open load file '%s'",
		"can't open object file '%s'",
		"no input files given!",
		"file '%s' is not a valid Macross object file",
		"premature end of file on object file '%s'",
		"error reading object file '%s'",
		"absolute code in '%s' with addresses 0x%04x:0x%04x overlaps absolute code in '%s' with addresses 0x%04x:0x%04x",
		"can't find a place to relocate relocatable code segment starting at 0x%04x in file '%s'",
		"multiply defined symbol '%s'",
		"'%s' undefined",
		"relative offset %d too large to fit at byte (0x%04x) 0x%04x",
		"value 0x%x too large to fit at byte (0x%04x) 0x%04x",
		"value 0x%x too large to fit at word (0x%04x) 0x%04x",
		"no file name on command line after '-m'",
		"more than one load map file name given on command line",
		"can't open load map file '%s'",
		"bad load address ('%s') given on command line",
		"no load address on command line after '-l'",
		"array term found in expression in object file",
		"ASSERT failed: %s",
		"condition code term in expression in object file",
		"'mswitch' clause at top level -- boo boo",
		"too many arguments to function inside object",
		"too few arguments to built in function '%s'",
		"too many arguments to built in function '%s'",
		"bad position argument to 'nthChar'",
		"bad substring indices to 'substr'",
		"argument to 'symbolName' is not a symbol",
		"symbol '%s' undefined in 'symbolLookup()'",
	};

	printf("\"%s\": ", currentFileName);
	vprintf(errorMessageStrings[(int)theError], ap);
	printf("\n");
	errorFlag = TRUE;
}

  void
error(errorType theError, ...)
{
	va_list ap;
	va_start(ap, theError);
	verror(theError, ap);
	va_end(ap);
}
