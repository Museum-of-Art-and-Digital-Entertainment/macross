/*
	listing.c -- Routines to generate a Macross assembly listing.

	Chip Morningstar -- Lucasfilm Ltd.

	10-February-1985
*/

#include "macrossTypes.h"
#include "macrossGlobals.h"

static char	 lineBuffer1[LINE_BUFFER_SIZE];
static char	 lineBuffer2[LINE_BUFFER_SIZE];
static int	 cumulativeLineNumber = 0;
static char	 macroLineBuffer[LINE_BUFFER_SIZE];
static char	 nextMacroLineBuffer[LINE_BUFFER_SIZE];
static int	 macroAddress;
static int	 nextMacroAddress;
static int	 macroDepth;
static int	 nextMacroDepth;

  void
outputListing()
{
	void	generateListing();

	rewind(saveFileForPass2);
	rewind(indexFileForPass2);
	rewind(macroFileForPass2);
	generateListing();
}

  void
terminateListingFiles()
{
	saveLineForListing("\n");
	saveIndexForListing(NULL_STATEMENT, cumulativeLineNumber);
	saveLineForListing("\n");
	saveIndexForListing(NULL_STATEMENT, cumulativeLineNumber);
}

#define advanceSourceFile() \
		if (nextSourceAddress == -1) {\
			readSourceFileLine(&sourceAddress, &sourceDepth,\
				sourceText = lineBuffer1, saveFileForPass2);\
			readSourceFileLine(&nextSourceAddress,\
				&nextSourceDepth, nextSourceText =\
				lineBuffer2, saveFileForPass2);\
			sourceLineNumber++;\
		} else {\
			sourceAddress = nextSourceAddress;\
			sourceDepth = nextSourceDepth;\
			tempText = sourceText;\
			sourceText = nextSourceText;\
			nextSourceText = tempText;\
			if (tempText == NULL)\
				nextSourceText = (sourceText==lineBuffer1) ?\
					lineBuffer2 : lineBuffer1;\
			readSourceFileLine(&nextSourceAddress,\
				&nextSourceDepth, nextSourceText,\
				saveFileForPass2);\
			sourceLineNumber++;\
		}

#define advanceIndexFile() {\
			indexAddress = nextIndexAddress;\
			indexLineNumber = nextIndexLineNumber;\
			indexKind = nextIndexKind;\
			readIndexFileLine(&nextIndexKind, &nextIndexAddress,\
				&nextIndexLineNumber);\
		}

/* This is the most horrible piece of code I have ever written in my entire
	life -- cbm */
  void
generateListing()
{
	int			 sourceAddress;
	int			 sourceDepth;
	char			*sourceText = lineBuffer1;
	int			 nextSourceAddress;
	int			 nextSourceDepth;
	char			*nextSourceText = lineBuffer2;
	int			 sourceLineNumber;

	int			 indexAddress;
	int			 indexLineNumber;
	statementKindType	 indexKind;
	int			 nextIndexAddress;
	int			 nextIndexLineNumber;
	statementKindType	 nextIndexKind;

	bool			 firstLine;
	bool			 alreadyListingMacroExpansion;
	bool			 finishingListingMacroExpansion;
	bool			 nullMacroLine;
	int			 callLineNumber;
	int			 targetLineNumber;
	bool			 mifEndFlag;
	int			 numberOfBytesToList;
	int			 numberOfBytesListed;
	char			*tempText;

	void			 readIndexFileLine();
	void			 readSourceFileLine();

	sourceLineNumber = 1;
	alreadyListingMacroExpansion = FALSE;
	readSourceFileLine(&sourceAddress, &sourceDepth, sourceText,
		saveFileForPass2);
	readSourceFileLine(&nextSourceAddress, &nextSourceDepth,
		nextSourceText, saveFileForPass2);
	if (expandMacros)
		readSourceFileLine(&nextMacroAddress, &nextMacroDepth,
			nextMacroLineBuffer, macroFileForPass2);
	readIndexFileLine(&indexKind, &indexAddress, &indexLineNumber);
	readIndexFileLine(&nextIndexKind, &nextIndexAddress,
		&nextIndexLineNumber);
	currentCodeMode = RELOCATABLE_BUFFER;
	if (indexKind == ORG_STATEMENT || indexKind == ALIGN_STATEMENT ||
			indexKind == REL_STATEMENT) {
		if (indexKind == ORG_STATEMENT) {
			currentCodeMode = ABSOLUTE_BUFFER;
		}
		advanceIndexFile();
	}
	while (!feof(saveFileForPass2) && !feof(indexFileForPass2)) {
		mifEndFlag = FALSE;
		if ((int)indexKind == -1) {
			advanceIndexFile();
			if ((int)indexKind == -1)
				currentCodeMode = ABSOLUTE_BUFFER;
			else
				currentCodeMode = RELOCATABLE_BUFFER;
			advanceIndexFile();
			sourceLineNumber = indexLineNumber;
			if (nextIndexKind == ORG_STATEMENT || nextIndexKind ==
					ALIGN_STATEMENT || nextIndexKind ==
					REL_STATEMENT) {
				if (nextIndexKind == ORG_STATEMENT) {
					currentCodeMode = ABSOLUTE_BUFFER;
				} else if (nextIndexKind == REL_STATEMENT) {
					currentCodeMode = RELOCATABLE_BUFFER;
				}
				advanceIndexFile();
			}
		}
		if (indexKind == MACRO_STATEMENT || indexKind ==
				FUNCTION_STATEMENT || indexKind ==
				STRUCT_STATEMENT) {
			while (sourceLineNumber < indexLineNumber) {
				printListingLine(0, indexAddress, sourceText,
					indexKind);
				advanceSourceFile();
			}
		} else if (!alreadyListingMacroExpansion && isBlockOpener(
				indexKind)) {
			while (sourceLineNumber < nextIndexLineNumber) {
				printListingLine(0, indexAddress, sourceText,
					indexKind);
				advanceSourceFile();
			}
			readIndexFileLine(&nextIndexKind, &nextIndexAddress,
				&nextIndexLineNumber);
		}
		while ((indexLineNumber == nextIndexLineNumber &&
				!alreadyListingMacroExpansion &&
				(int)nextIndexKind != -1 && indexKind !=
				ORG_STATEMENT) || (indexKind ==
				NULL_STATEMENT && nextSourceAddress == -1)) {
			while (nextSourceAddress == -1) {
				printListingLine(0, indexAddress, sourceText,
					indexKind);
				advanceSourceFile();
			}
			readIndexFileLine(&nextIndexKind, &nextIndexAddress,
				&nextIndexLineNumber);
		}
		if (nextIndexLineNumber >= 0) {
			targetLineNumber = alreadyListingMacroExpansion ?
				callLineNumber : sourceLineNumber;
			while (nextIndexLineNumber < targetLineNumber &&
					!isBlockOpener(indexKind)) {
				readIndexFileLine(&nextIndexKind,
					&nextIndexAddress,
					&nextIndexLineNumber);
			}
		}
		while (nextIndexAddress == -1 && !feof(indexFileForPass2)) {
			readIndexFileLine(&nextIndexKind, &nextIndexAddress,
				&nextIndexLineNumber);
			mifEndFlag = TRUE;
		}
		numberOfBytesToList = nextIndexAddress - indexAddress;
		firstLine = TRUE;
		nullMacroLine = FALSE;
		while (numberOfBytesToList > 0 || nextSourceAddress == -1 ||
				firstLine) {
			firstLine = FALSE;
			if (alreadyListingMacroExpansion) {
			   if (nullMacroLine) {
				numberOfBytesListed = printListingLine(
					numberOfBytesToList, indexAddress,
					NULL, indexKind);
				nullMacroLine = FALSE;
			   } else {
				numberOfBytesListed = printMacroLine(
					numberOfBytesToList, indexAddress,
					indexKind);
			   }
			} else if (!feof(indexFileForPass2)) {
				numberOfBytesListed = printListingLine(
					numberOfBytesToList, indexAddress,
					sourceText, indexKind);
			}
			numberOfBytesToList -= numberOfBytesListed;
			indexAddress += numberOfBytesListed;
			if (nextSourceAddress == -1 || (sourceLineNumber <
					indexLineNumber && indexKind ==
					INSTRUCTION_STATEMENT &&
					!alreadyListingMacroExpansion)) {
				advanceSourceFile();
				firstLine = TRUE;
			} else if (!alreadyListingMacroExpansion &&
					nextIndexLineNumber >= 0) {
				sourceText = NULL;
			} else if (alreadyListingMacroExpansion &&
					numberOfBytesToList > 0) {
				nullMacroLine = TRUE;
			}
		}
		while (mifEndFlag && sourceLineNumber+1<nextIndexLineNumber
				&& !alreadyListingMacroExpansion) {
			advanceSourceFile();
			printListingLine(0, indexAddress, sourceText,
				indexKind);
		}
		if (nextIndexKind == ORG_STATEMENT || nextIndexKind ==
				ALIGN_STATEMENT || nextIndexKind ==
				REL_STATEMENT) {
			if (nextIndexKind == ORG_STATEMENT) {
				currentCodeMode = ABSOLUTE_BUFFER;
			} else if (nextIndexKind == REL_STATEMENT) {	
				currentCodeMode = RELOCATABLE_BUFFER;
			}
			advanceIndexFile();
		}
		advanceIndexFile();
		if (!alreadyListingMacroExpansion)
			advanceSourceFile();
		if (indexLineNumber < 0) {
			while (nextIndexAddress == -1) {
				readIndexFileLine(&nextIndexKind,
					&nextIndexAddress,
					&nextIndexLineNumber);
			}
			if ((int)indexKind == -2) {
				indexKind = NULL_STATEMENT;
			}
			if (!alreadyListingMacroExpansion) {
				callLineNumber = sourceLineNumber;
				sourceLineNumber = -indexLineNumber;
				alreadyListingMacroExpansion = TRUE;
			}
		} else if (alreadyListingMacroExpansion) {
			alreadyListingMacroExpansion = FALSE;
			while (abs(indexLineNumber) < callLineNumber &&
					!feof(indexFileForPass2)) {
				advanceIndexFile();
			}
			sourceLineNumber = /*isBlockOpener(indexKind) ?*/
				callLineNumber /*+ 1 : indexLineNumber*/;
		}
	}
}

bool	longLineFlag; /* defined in lexer.c */

  int
printMacroLine(numberOfBytes, byteAddress, kind)
  int			 numberOfBytes;
  int			 byteAddress;
  statementKindType	 kind;
{
	macroAddress = nextMacroAddress;
	macroDepth = nextMacroDepth;
	strcpy(macroLineBuffer, nextMacroLineBuffer);
	readSourceFileLine(&nextMacroAddress, &nextMacroDepth,
		nextMacroLineBuffer, macroFileForPass2);
	return(printListingLine(numberOfBytes, byteAddress, macroLineBuffer,
		kind));
}

  void
readSourceFileLine(sourceAddressPtr, sourceDepthPtr, lineBuffer, file)
  int	*sourceAddressPtr;
  int	*sourceDepthPtr;
  char	 lineBuffer[];
  FILE	*file;
{
	char	c;

	*sourceAddressPtr = getw(file);
	*sourceDepthPtr = getw(file);
	myfgets(lineBuffer, LINE_BUFFER_SIZE, file);
	if (longLineFlag) {
		while ((c = getc(file)) != '\n' &&  c != EOF)
			;
		lineBuffer[LINE_BUFFER_SIZE - 2] = '\n';
	}
}

  void
readIndexFileLine(statementKindPtr, indexAddressPtr, indexLineNumberPtr)
  statementKindType	*statementKindPtr;
  int			*indexAddressPtr;
  int			*indexLineNumberPtr;
{
	statementKindType	 statementKindRead;
	int			 indexAddressRead;
	int			 indexLineNumberRead;

	statementKindRead = (statementKindType) getw(indexFileForPass2);
	indexAddressRead = getw(indexFileForPass2);
	indexLineNumberRead = getw(indexFileForPass2) - 1;
	if (!feof(indexFileForPass2)) {
		*statementKindPtr = statementKindRead;
		*indexAddressPtr = indexAddressRead;
		*indexLineNumberPtr = indexLineNumberRead;
	} else {
		*indexLineNumberPtr += 1;
	}
}

  int
printListingLine(numberOfBytes, byteAddress, text, kind)
  int			 numberOfBytes;
  int			 byteAddress;
  char			*text;
  statementKindType	 kind;
{
	int	i;

	void	tabPrint();

	if (kind != BLOCK_STATEMENT)
		numberOfBytes = (numberOfBytes < 4) ? numberOfBytes : 4;

	if (numberOfBytes==0 && isBlankStatement(kind))
		fprintf(listFileOutput, "     ");
	else
#if TARGET_CPU == CPU_6502
		fprintf(listFileOutput, "%04x ", byteAddress);
#else if TARGET_CPU == CPU_68000
		fprintf(listFileOutput, "%06x ", byteAddress);
#endif

	if (kind != BLOCK_STATEMENT) {
#if TARGET_CPU == CPU_6502
		for (i=0; i<numberOfBytes; i++)
			fprintf(listFileOutput, " %02x",
				getByte(byteAddress++));
		for (i=numberOfBytes; i<4; i++)
			fprintf(listFileOutput, "   ");
	} else {
		fprintf(listFileOutput, "            ");
#else if TARGET_CPU == CPU_68000
		for (i=0; i<numberOfBytes; i++) {
			fprintf(listFileOutput, "%02x",
				getByte(byteAddress++));
			if (i==1) fprintf(listFileOutput, " ");
		}
		for (i=numberOfBytes; i<4; i++) {
			fprintf(listFileOutput, "  ");
			if (i==1) fprintf(listFileOutput, " ");
		}
	} else {
		fprintf(listFileOutput, "         ");
#endif
	}

	if (text == NULL) {
		fprintf(listFileOutput, "\n");
	} else {
		fprintf(listFileOutput, "  ");
		tabPrint(text);
	}
	return(numberOfBytes);
}

	
  bool
isBlockOpener(statementKind)
  statementKindType	statementKind;
{
	return (statementKind==IF_STATEMENT ||
		statementKind==WHILE_STATEMENT ||
		statementKind==DO_WHILE_STATEMENT ||
		statementKind==DO_UNTIL_STATEMENT ||
		statementKind==CONSTRAIN_STATEMENT ||
/*		statementKind==STRUCT_STATEMENT ||*/
		statementKind==MIF_STATEMENT ||
		statementKind==MFOR_STATEMENT ||
		statementKind==MDO_WHILE_STATEMENT ||
		statementKind==MDO_UNTIL_STATEMENT ||
		statementKind==MWHILE_STATEMENT ||
		statementKind==INCLUDE_STATEMENT ||
		statementKind==GROUP_STATEMENT
	);
}

  bool
isBlankStatement(statementKind)
  statementKindType	statementKind;
{
	return(	statementKind == DEFINE_STATEMENT ||
		statementKind == NULL_STATEMENT ||
		statementKind == VARIABLE_STATEMENT ||
		statementKind == CONSTRAIN_STATEMENT ||
		statementKind == MACRO_STATEMENT ||
		statementKind == FUNCTION_STATEMENT ||
		statementKind == UNDEFINE_STATEMENT ||
		statementKind == MDEFINE_STATEMENT ||
		statementKind == MVARIABLE_STATEMENT ||
		statementKind == FRETURN_STATEMENT ||
		statementKind == UNDEFINE_STATEMENT ||
		statementKind == EXTERN_STATEMENT ||
		statementKind == START_STATEMENT ||
		statementKind == INCLUDE_STATEMENT ||
		statementKind == MIF_STATEMENT ||
		statementKind == MFOR_STATEMENT ||
		statementKind == MDO_WHILE_STATEMENT ||
		statementKind == MDO_UNTIL_STATEMENT ||
		statementKind == MWHILE_STATEMENT ||
		statementKind == PERFORM_STATEMENT
	);
}

  void
tabPrint(text)
  stringType	*text;
{
	int	column;
	int	spaces;

	column = 0;
	while (*text != '\0') {
		if (*text == '\t') {
			for (spaces = 8 - (column % 8); spaces>0; spaces--) {
				putc(' ', listFileOutput);
				column++;
			}
		} else {
			putc(*text, listFileOutput);
			column++;
		}
		text++;
	}
}

  void
printNTimes(aChar, times)
  char	aChar;
  int	times;
{
	void	moreText();

	while (times-- > 0)
		moreText("%c", aChar);
}

  void
tabIndent()
{
	printNTimes('\t', tabCount);
}

static char	 expressionString[LINE_BUFFER_SIZE] = { '\0' };
static char	*expressionStringPtr = expressionString;
static char	 expansionString[LINE_BUFFER_SIZE] = { '\0' };
static char	*expansionStringPtr = expansionString;
static char	 labelString[LINE_BUFFER_SIZE] = { '\0' };
static char	*labelStringPtr = labelString;

  bool
labeledLine()
{
	return(labelStringPtr != labelString);
}

  void
addText(buffer, bufferPtr, format, arg1, arg2, arg3)
  char  *buffer;
  char **bufferPtr;
  char	*format;
  int	 arg1;
  int	 arg2;
  int	 arg3;
{
	sprintf(*bufferPtr, format, arg1, arg2, arg3);
	*bufferPtr = buffer + strlen(buffer);
}

  void
moreTextOptional(buffer, bufferPtr, format, arg1, arg2, arg3)
  char  *buffer;
  char **bufferPtr;
  char	*format;
  int	 arg1;
  int	 arg2;
  int	 arg3;
{
	if (buffer == NULL)
		addText(expansionString, &expansionStringPtr, format, arg1,
			arg2, arg3);
	else
		addText(buffer, bufferPtr, format, arg1, arg2, arg3);
}

  void
moreText(format, arg1, arg2, arg3)
  char	*format;
  int	 arg1;
  int	 arg2;
  int	 arg3;
{
	addText(expansionString, &expansionStringPtr, format, arg1,arg2,arg3);
}

  void
moreLabel(format, arg1, arg2, arg3)
  char	*format;
  int	 arg1;
  int	 arg2;
  int	 arg3;
{
	sprintf(labelStringPtr, format, arg1, arg2, arg3);
	labelStringPtr = labelString + strlen(labelString);
}

static addressType	savedCurrentLocationCounterValue;
static int		savedIncludeNestingDepth;

  void
startLine()
{
	printNTimes('+', macroCallDepth);
	savedCurrentLocationCounterValue = currentLocationCounter.value;
	savedIncludeNestingDepth = includeNestingDepth;
}

  void
endLine()
{
	if (amListing()) {
		putw(savedCurrentLocationCounterValue, macroFileForPass2);
		putw(savedIncludeNestingDepth, macroFileForPass2);
		fprintf(macroFileForPass2, "%s\n", expansionString);
	}
	expansionStringPtr = expansionString;
	*expansionStringPtr = '\0';
}

  void
flushExpressionString()
{
	expressionStringPtr = expressionString;
	*expressionStringPtr = '\0';
}

  void
expandExpression(toBuffer, toBufferPtr)
  char	 *toBuffer;
  char	**toBufferPtr;
{
	if (toBuffer == NULL)
		moreText("%s", expressionString);
	else
		addText(toBuffer, toBufferPtr, "%s", expressionString);
	flushExpressionString();
}

  void
expandNum(buffer, bufferPtr, n)
  char	 *buffer;
  char	**bufferPtr;
  int	  n;
{
	moreTextOptional(buffer, bufferPtr, "%d", n);
}

  void
flushOperand(n)
  int	n;
{
	moreText("%s", operandBuffer[n]);
	operandBuffer[n][0] = '\0';
}

  void
expandOperands(op)
  int	op;
{
	int	i;

	if (op > 0) {
		flushOperand(0);
		for (i=1; i<op; i++) {
			moreText(", ");
			flushOperand(i);
		}
	}
}

  void
expandLabel()
{
	moreText("%s", labelString);
	labelStringPtr = labelString;
	*labelStringPtr = '\0';
}

  void
moreExpression(format, arg1, arg2, arg3)
  char	*format;
  int	 arg1;
  int	 arg2;
  int	 arg3;
{
	sprintf(expressionStringPtr, format, arg1, arg2, arg3);
	expressionStringPtr = expressionString + strlen(expressionString);
}

  void
startLineMarked()
{
	startLine();
	if (amListing())
		saveIndexForListing(-2, cumulativeLineNumber);
}

  bool
notListable(statementKind)
  statementKindType	statementKind;
{
	return(!listableStatement(statementKind) &&
		statementKind != INSTRUCTION_STATEMENT &&
		(int)statementKind != -2);
}
