/*
	errorStuff.c -- Error handling for the Slinky linker.

	Chip Morningstar -- Lucasfilm Ltd.

	9-March-1985
*/

#include "slinkyTypes.h"
#include "slinkyGlobals.h"

  void
error(theError, arg1, arg2, arg3, arg4, arg5, arg6)
  errorType	theError;
  anyOldThing	*arg1;
  anyOldThing	*arg2; 
  anyOldThing	*arg3;
  anyOldThing	*arg4;
  anyOldThing	*arg5;
  anyOldThing	*arg6;
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
	printf(errorMessageStrings[(int)theError], arg1, arg2, arg3, arg4,
		arg5, arg6);
	printf("\n");
	errorFlag = TRUE;
}
