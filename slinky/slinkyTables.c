/*
	slinkyTables.c -- Global tables for the Slinky linker.

	Chip Morningstar -- Lucasfilm Ltd.

	14-November-1985
*/

#include "slinkyTypes.h"

addressType	atasciiBIF();
addressType	atasciiColorBIF();
addressType	isAbsoluteValueBIF();
addressType	isConditionCodeBIF();
addressType	isDefinedBIF();
addressType	isExternalBIF();
addressType	nthCharBIF();
addressType	printfBIF();
addressType	strcatBIF();
addressType	strcmpBIF();
addressType	strcmplcBIF();
addressType	strlenBIF();
addressType	substrBIF();
addressType	symbolDefineBIF();
addressType	symbolLookupBIF();
addressType	symbolNameBIF();

/* Used to initialize symbols representing built-in functions */
struct {
	stringType	*functionName;
	addressType	 (*functionEntry)();
		} builtInFunctionTable[] = {
	"atascii",		atasciiBIF,	 	/*  0 */
	"atasciiColor",		atasciiColorBIF,	/*  1 */
	"isAbsoluteValue",	isAbsoluteValueBIF,	/*  2 */
	"isConditionCode",	isConditionCodeBIF,	/*  3 */
	"isDefined",		isDefinedBIF,	 	/*  4 */
	"isExternal",		isExternalBIF,	 	/*  5 */
	"nthChar",		nthCharBIF,	 	/*  6 */
	"printf",		printfBIF,	 	/*  7 */
	"strcat",		strcatBIF,	 	/*  8 */
	"strcmp",		strcmpBIF,		/*  9 */
	"strcmplc",		strcmplcBIF,		/* 10 */
	"strlen",		strlenBIF,		/* 11 */
	"substr",		substrBIF,		/* 12 */
	"symbolLookup",		symbolLookupBIF,	/* 13 */
	"symbolName",		symbolNameBIF,		/* 14 */
	NULL,			NULL,
};
