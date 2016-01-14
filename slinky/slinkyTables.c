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
