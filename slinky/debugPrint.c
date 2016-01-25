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
	debugPrint.c -- Routines to dump stuff out in readable form for
		debugging the Slinky linker.

	Chip Morningstar -- Lucasfilm Ltd.

	18-March-1985

*/

#include "slinkyTypes.h"
#include "slinkyGlobals.h"
#include "debugPrint.h"

static char *modeStrings[2] = {
		"abs",
		"rel",
		};

static char *kindStrings[3] = {
		"byte",
		"word",
		"dbyt",
		};

static char *pcrelStrings[2] = {
		"     ",
		"pcrel",
		};

static char *externalStrings[2] = {
		"      ",
		"extern",
		};

static char *symbolStrings[6] = {
		"und    ", /* 0x00 */
		"und ext", /* 0x01 */
		"abs    ", /* 0x02 */
		"abs ext", /* 0x03 */
		"rel    ", /* 0x04 */
		"rel ext", /* 0x05 */
		};

  void
printCode(int startAddress, int endAddress, int mode)
{
	printf("    Code: 0x%04x:0x%04x %s\n", startAddress, endAddress,
		modeStrings[mode]);
}

  void
printReference(expressionReferenceType *reference)
{
	printf("    Ref : 0x%04x (%s %s %s %s) expression #%d\n", reference->
		referenceAddress, modeStrings[reference->referenceMode],
		pcrelStrings[reference->referenceRelative], externalStrings[
		reference->referenceExternal], kindStrings[reference->
		referenceKind], reference->referenceExpression.inFile);
}

  void
printReferenceFixup(expressionReferenceType *reference)
{
	printf("    Ref : 0x%04x (%s %s %s %s) expression=0x%x\n",
		reference->referenceAddress, modeStrings[reference->
		referenceMode],pcrelStrings[reference->referenceRelative],
		externalStrings[reference->referenceExternal], kindStrings[
		reference->referenceKind], reference->referenceExpression.
		inCore);
}

  void
printSymbol(int symbolTag, symbolType *symbol)
{
	printf("    Symb: %3d %s 0x%04x \"%s\"\n", symbolTag, symbolStrings[
		symbol->symbolClass], symbol->symbolValue,
		symbol->symbolName);
}

  void
printLoadMapSymbol(symbolType *symbol)
{
	fprintf(mapFileOutput, "%-20s 0x%04x %s  ", symbol->symbolName,
		symbol->symbolValue, symbolStrings[symbol->symbolClass]);
}

  void
printGlobalSymbols(void)
{
	int	symbolCount;

	printf("Globals:\n");
	printf("	%d global symbols\n", globalSymbolCount);
	for (symbolCount=0; symbolCount<globalSymbolCount; symbolCount++) {
		printf("    Gsym: %3d \"%s\"\n", symbolCount,
			globalSymbolTable[symbolCount]->symbolName);
	}
}

  void
printExpression(expressionPCType expression, int length)
{
	int	line;
	int	i;

	printf("expression length = %d:\n", length);
	for (line=0; line<length; line+=16) {
		printf(" %4x:", line);
		for (i=line; i<line+16 && i<length; i++)
			printf(" %2x", expression[i]);
		printf("\n");
	}
}
