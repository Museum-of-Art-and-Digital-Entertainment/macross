/*
	debugPrint.c -- Routines to dump stuff out in readable form for
		debugging the Slinky linker.

	Chip Morningstar -- Lucasfilm Ltd.

	18-March-1985

*/

#include "slinkyTypes.h"
#include "slinkyGlobals.h"


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
printCode(startAddress, endAddress, mode)
  int	startAddress;
  int	endAddress;
  int	mode;
{
	printf("    Code: 0x%04x:0x%04x %s\n", startAddress, endAddress,
		modeStrings[mode]);
}

  void
printReference(reference)
  expressionReferenceType	*reference;
{
	printf("    Ref : 0x%04x (%s %s %s %s) expression #%d\n", reference->
		referenceAddress, modeStrings[reference->referenceMode],
		pcrelStrings[reference->referenceRelative], externalStrings[
		reference->referenceExternal], kindStrings[reference->
		referenceKind], reference->referenceExpression.inFile);
}

  void
printReferenceFixup(reference)
  expressionReferenceType	*reference;
{
	printf("    Ref : 0x%04x (%s %s %s %s) expression=0x%x\n",
		reference->referenceAddress, modeStrings[reference->
		referenceMode],pcrelStrings[reference->referenceRelative],
		externalStrings[reference->referenceExternal], kindStrings[
		reference->referenceKind], reference->referenceExpression.
		inCore);
}

  void
printSymbol(symbolTag, symbol)
  int		 symbolTag;
  symbolType	*symbol;
{
	printf("    Symb: %3d %s 0x%04x \"%s\"\n", symbolTag, symbolStrings[
		symbol->symbolClass], symbol->symbolValue,
		symbol->symbolName);
}

  void
printLoadMapSymbol(symbol)
  symbolType	*symbol;
{
	fprintf(mapFileOutput, "%-20s 0x%04x %s  ", symbol->symbolName,
		symbol->symbolValue, symbolStrings[symbol->symbolClass]);
}

  void
printGlobalSymbols()
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
printExpression(expression, length)
  expressionPCType	expression;
  int			length;
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
