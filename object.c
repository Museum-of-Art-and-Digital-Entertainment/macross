/*
	object.c -- Routines to output object files for the Macross assembler.

	Chip Morningstar -- Lucasfilm Ltd.

	20-November-1984
*/

#include "macrossTypes.h"
#include "macrossGlobals.h"

static int	symbolTableSize;
static int	symbolTableStringSize;
bool		encodingFunction;

  void
outputObjectFile()
{
	void	outputPartition();
	void	outputReferenceInfo();
	void	outputSymbolTableInfo();
	void	outputAbsoluteCode();
	void	outputRelocatableCode();
	void	outputReservations();
	void	outputExpressions();
	void	outputFunctions();
	void	dumpSymbolTable();
	void	enumerateAndCountSymbols();

	if (debug || emitPrint)
		printCodeBuffers();
	outputPartition();
	outputAbsoluteCode();
	if (produceLinkableObject) {
		outputPartition();
		outputRelocatableCode();
		outputPartition();
		outputReservations();
		outputPartition();
		enumerateAndCountSymbols();
		outputReferenceInfo();
		outputSymbolTableInfo();
		outputExpressions();
		outputFunctions();
	}
	if (symbolTableDumpOn)
		dumpSymbolTable();
	fclose(objectFileOutput);
}

  void
outputWord(aWord)
 int	aWord;
{
	putc(aWord & 0xFF, objectFileOutput);
	putc((aWord >> 8) & 0xFF, objectFileOutput);
}

  void
outputPartition()
{
	outputWord(0xFFFF);
}

  void
outputBigword(bigword)
  unsigned long	bigword;
{
	int	i;

	for (i=1; i<=4; i++) {
		putc(bigword & 0xFF, objectFileOutput);
		bigword >>= 8;
	}
}

  void
outputByte(aByte)
 byte	aByte;
{
	putc(aByte, objectFileOutput);
}

  void
outputString(string)
  stringType	*string;
{
	do {
		putc(*string, objectFileOutput);
	} while (*string++ != '\0');
}

  void
outputStartAddress(startAddress)
  addressType	startAddress;
{
	outputWord(startAddress);
	if (produceLinkableObject) {
		outputWord(startAddress - 3);
		outputByte(0);
	} else {
		outputWord(startAddress);
		outputByte(0);
	}
}

  void
outputRelocatableCode()
{
	int		 i;
	addressType	 codeStartAddress;
	addressType	 codeEndAddress;

	void		 outputPseudoSegment();
	void		 outputBreak();

	if (haveUserStartAddress && !fixupStartAddress && startAddress->
			kindOfValue == RELOCATABLE_VALUE)
		outputStartAddress(startAddress->value);
	else if (haveUserStartAddress && fixupStartAddress)
		outputStartAddress(0xFFFE);
	if (relocatableHighWaterMark < 0)
		return;
	codeEndAddress = -1;
	do {
		codeStartAddress = codeEndAddress + 1;
		while (codeBreakList != NULL && codeBreakList->breakAddress
				== codeStartAddress) {
			outputBreak(codeBreakList);
			codeBreakList = codeBreakList->nextBreak;
		}
		if (codeBreakList == NULL)
			codeEndAddress = relocatableHighWaterMark;
		else
			codeEndAddress = codeBreakList->breakAddress - 1;
		outputPseudoSegment(codeStartAddress, codeEndAddress);
	} while (codeEndAddress < relocatableHighWaterMark);
}

  void
outputBreak(codeBreak)
  codeBreakType	*codeBreak;
{
	switch (codeBreak->kindOfBreak) {
	case BREAK_BREAK:
	case END_CONSTRAIN_BREAK:
		/* do nothing */
		break;

	case ALIGN_BREAK:
		outputWord(codeBreak->breakAddress);
		outputWord(codeBreak->breakAddress - 2);
		outputWord(codeBreak->breakData);
		break;

	case CONSTRAIN_BREAK:
		outputWord(codeBreak->breakAddress);
		outputWord(codeBreak->breakAddress - 1);
		outputWord(codeBreak->breakData);
		break;
	}
}

  void
outputAbsoluteCode()
{
	int		 i;
	int		 startSegment;
	int		 endSegment;
	int		 nextSegment;
	void		 outputOneCodeBuffer();

	if (haveUserStartAddress && !fixupStartAddress && startAddress->
			kindOfValue==ABSOLUTE_VALUE)
		outputStartAddress(startAddress->value);
	for (startSegment=0; startSegment<CODE_BUFFERS_IN_ADDRESS_SPACE;
			startSegment = endSegment + 1) {
		endSegment = startSegment;
		nextSegment = endSegment + 1;
		while (nextSegment < CODE_BUFFERS_IN_ADDRESS_SPACE &&
				absoluteCodeRegion.codeSegments[nextSegment]
				!= NULL && absoluteCodeRegion.codeSegments[
				endSegment] != NULL && absoluteCodeRegion.
				codeSegments[endSegment]->codeEndAddress+1 ==
				absoluteCodeRegion.codeSegments[nextSegment]
				->codeStartAddress) {
			++endSegment;
			++nextSegment;
		}
		if (absoluteCodeRegion.codeSegments[startSegment] != NULL) {
			outputWord(absoluteCodeRegion.codeSegments[
				startSegment]->codeStartAddress);
			outputWord(absoluteCodeRegion.codeSegments[
				endSegment]->codeEndAddress);
			for (i=startSegment; i<=endSegment; ++i)
				outputOneCodeBuffer(absoluteCodeRegion.
					codeSegments[i]);
		}
	}
}

  void
outputOneCodeBuffer(segment)
  codeSegmentType	*segment;
{
	int	i;

	if (segment != NULL) {
/*		outputWord(segment->codeStartAddress);
		outputWord(segment->codeEndAddress);*/
		for (i=bufferPosition(segment->codeStartAddress); i<=
				bufferPosition(segment->codeEndAddress); i++)
			outputByte((*segment->codeBuffer)[i]);
	}
}

  void
outputPseudoSegment(codeStartAddress, codeEndAddress)
  addressType	codeStartAddress;
  addressType	codeEndAddress;
{
	int		 startSegment;
	int		 endSegment;
	int		 startPosition;
	int		 endPosition;
	int		 originalStartPosition;
	int		 ultimateEndPosition;
	int		 position;
	int		 segment;
	codeSegmentType	*segmentPtr;

	void		 outputWord();
	void		 outputByte();

	outputWord(codeStartAddress);
	outputWord(codeEndAddress);
	startSegment = bufferNumber(codeStartAddress);
	endSegment = bufferNumber(codeEndAddress);
	originalStartPosition = bufferPosition(codeStartAddress);
	ultimateEndPosition = bufferPosition(codeEndAddress);
	for (segment=startSegment; segment<=endSegment; segment++) {
		segmentPtr = relocatableCodeRegion.codeSegments[segment];
		startPosition = (segment == startSegment) ?
			originalStartPosition : 0;
		endPosition = (segment == endSegment) ? ultimateEndPosition :
			CODE_BUFFER_SIZE - 1;
		for(position=startPosition; position<=endPosition; position++)
			outputByte((*segmentPtr->codeBuffer)[position]);
	}
}

  bool
isObjectSymbol(symbol)
  symbolTableEntryType	*symbol;
{
	return(symbol != NULL && symbol->context->value != NULL &&
		symbol->context->value->kindOfValue != FUNCTION_VALUE &&
		symbol->context->value->kindOfValue != BUILT_IN_FUNCTION_VALUE
		&& symbol->context->value->kindOfValue != MACRO_VALUE &&
		((isExternal(symbol) && symbol->context->value->kindOfValue
		!= UNDEFINED_VALUE) || symbol->referenceCount > 0));
/*	return(symbol != NULL && symbol->context->value != NULL &&
		(symbol->context->value->kindOfValue == RELOCATABLE_VALUE ||
		 symbol->context->value->kindOfValue == ABSOLUTE_VALUE ||
		 isExternal(symbol)));*/
}

  void
enumerateAndCountSymbols()
{
	int			 i;
	symbolTableEntryType	*symb;

	symbolTableSize = 0;
	symbolTableStringSize = 0;
	for (i=0; i<HASH_TABLE_SIZE; i++)
	    for (symb=symbolTable[i]; symb!=NULL; symb=symb->nextSymbol)
		if (isObjectSymbol(symb)) {
		    symbolTableStringSize += strlen(symb->symbolName) + 1;
		    symb->ordinal = symbolTableSize++;
		}
}

  int
enumerateAndCountReferences()
{
    int				 result;
    int				 codeMode;
    expressionReferenceListType	*theReferences;

    result = 0;
    for (codeMode=0; codeMode<=1; codeMode++) {
        theReferences = expressionReferenceList[codeMode];
        while (theReferences != NULL) {
	    theReferences->relocation.referenceExpression = result++;
            theReferences = theReferences->nextReference;
        }
    }
    return(result);
}

  void
outputReference(reference)
  expressionReferenceType	*reference;
{
	byte	funnyByte;
	bigWord	funnyWord;

	/* This nonsense is to maintain byte-order and bit-order independence
	   across host machines so that Macross object files will be
	   portable. */
	funnyByte = reference->referenceMode |
		    (reference->referenceRelative << 1) |
		    (reference->referenceExternal << 2) |
		    (reference->referenceKind << 3);
	funnyWord = ((reference->referenceAddress) & 0xFFFFFF) |
	          (funnyByte << 24);
	outputBigword(funnyWord);
	outputBigword(reference->referenceExpression);

}

static int	referenceCount;

  void
outputReferenceInfo()
{
	expressionReferenceListType	*theReferences;
	int				 codeMode;

	outputBigword(referenceCount = enumerateAndCountReferences());

	for (codeMode=0; codeMode<=1; codeMode++) {
		theReferences = expressionReferenceList[codeMode];
		while (theReferences != NULL) {
			outputReference(&(theReferences->relocation));
			theReferences = theReferences->nextReference;
		}
	}
}

  void
outputOneSymbol(symbol)
  symbolTableEntryType	*symbol;
{
	byte			 symbolClass;
	valueType		*symbolValue;
	valueType		*evaluateIdentifier();

	if (symbol->context->usage == DEFINE_SYMBOL)
		symbolValue = evaluateIdentifier(symbol, FALSE, NO_FIXUP_OK);
	else
		symbolValue = symbol->context->value;
	if (symbolValue->kindOfValue == ABSOLUTE_VALUE)
		symbolClass = SYMBOL_ABSOLUTE;
	else if (symbolValue->kindOfValue == RELOCATABLE_VALUE)
		symbolClass = SYMBOL_RELOCATABLE;
	else
		symbolClass = SYMBOL_UNDEFINED;
	if (isExternal(symbol))
		symbolClass |= SYMBOL_EXTERNAL;
	else if (symbolClass == SYMBOL_UNDEFINED)
		symbolClass = SYMBOL_ABSOLUTE;
	outputByte(symbolClass);
	outputBigword(symbolValue->value);
	outputString(symbol->symbolName);
}

  void
outputSymbolTableInfo()
{
	int			 i;
	symbolTableEntryType	*symb;

	outputBigword(symbolTableSize);
	outputBigword(symbolTableStringSize);
	for (i=0; i<HASH_TABLE_SIZE; i++)
	    for (symb=symbolTable[i]; symb!=NULL; symb=symb->nextSymbol)
		if (isObjectSymbol(symb))
		    outputOneSymbol(symb);
}

  int
symbolCompare(symbol1, symbol2)
  symbolTableEntryType	**symbol1;
  symbolTableEntryType	**symbol2;
{
	return(strcmplc((*symbol1)->symbolName, (*symbol2)->symbolName));
}

  bool
shouldDumpSymbol(symbol)
  symbolTableEntryType	*symbol;
{
	return(symbolTableDumpOn == 2
		|| (symbol->context->usage != BUILT_IN_FUNCTION_SYMBOL
		   && (symbol->context->usage != DEFINE_SYMBOL
		      || symbol->context->referenceCount != 0
		      || showAllSymbolsFlag
		      )
		   && symbol->symbolName[0] != '$'
		   && symbol->symbolName[0] != '_'
		   )
	      );
}

  void
dumpSymbolTable()
{
	int			 i;
	symbolTableEntryType	*symb;
	symbolTableEntryType   **symbolVector;
	int			 numberOfSymbols;
	int			 symbolPtr;
	valueType		*value;

	valueType		*evaluateIdentifier();
	void			 printValueTersely();

	numberOfSymbols = 0;
	for (i=0; i<HASH_TABLE_SIZE; i++)
	    for (symb=symbolTable[i]; symb!=NULL; symb=symb->nextSymbol) {
		if (symb->context != NULL && symb->context->value != NULL &&
				(symb->context->value->kindOfValue !=
				UNDEFINED_VALUE || symb->context->usage ==
				LABEL_SYMBOL || symb->context->usage ==
				EXTERNAL_SYMBOL || symb->context->usage ==
				UNKNOWN_SYMBOL || symb->context->usage ==
				NESTED_UNKNOWN_SYMBOL)) {
			if (shouldDumpSymbol(symb)) {
				numberOfSymbols++;
			}
		}
	    }
	symbolVector = (symbolTableEntryType **)malloc(
		sizeof(symbolTableEntryType *) * numberOfSymbols);
	symbolPtr = 0;
	for (i=0; i<HASH_TABLE_SIZE; i++)
	    for (symb=symbolTable[i]; symb!=NULL; symb=symb->nextSymbol) {
		if (symb->context != NULL && symb->context->value != NULL &&
				(symb->context->value->kindOfValue !=
				UNDEFINED_VALUE || symb->context->usage ==
				LABEL_SYMBOL || symb->context->usage ==
				EXTERNAL_SYMBOL || symb->context->usage ==
				UNKNOWN_SYMBOL || symb->context->usage ==
				NESTED_UNKNOWN_SYMBOL)) {
			if (shouldDumpSymbol(symb)) {
				symbolVector[symbolPtr++] = symb;
			}
		}
	    }
	qsort(symbolVector, numberOfSymbols, sizeof(symbolTableEntryType *),
		symbolCompare);
	for (symbolPtr=0; symbolPtr<numberOfSymbols; symbolPtr++) {
		symb = symbolVector[symbolPtr];
		if (hackFlag != 0) {
			if (hackableSymbol(symb) && (isExternal(symb) ||
					hackFlag == 1)) {
				value = evaluateIdentifier(symb, TRUE,
					NO_FIXUP_OK);
				fprintf(symbolDumpFileOutput,
					"define %s = 0x%x\n", symb->
					symbolName, value->value);
			}
		} else if (symb->context->usage == FUNCTION_SYMBOL) {
			fprintf(symbolDumpFileOutput, "%-20s = FUNCTION\n",
				symb->symbolName);
		} else if (symbolTableDumpOn == 2 || symb->context->usage !=
				BUILT_IN_FUNCTION_SYMBOL) {
			value = evaluateIdentifier(symb, TRUE, NO_FIXUP_OK);
			fprintf(symbolDumpFileOutput, "%-20s = ", symb->
				symbolName);
			printValueTersely(value);
			fprintf(symbolDumpFileOutput, "  [%d]", symb->
				context->referenceCount - 1);
			if (isExternal(symb))
				fprintf(symbolDumpFileOutput, " ext");
			fprintf(symbolDumpFileOutput, "\n");
		}
	}
	free(symbolVector);
}

  bool
hackableSymbol(symbol)
  symbolTableEntryType	*symbol;
{
	return(symbol->context->usage == DEFINE_SYMBOL || symbol->context->
		usage == LABEL_SYMBOL);
}

  void
printValueTersely(value)
  valueType	*value;
{
	static char *valueKindTable[NUM_OF_VALUE_KINDS] = {
		"ABS",
		"DAT",
		"REL",
		"BSS",
		"STT",
		"FLD",
		"MAC",
		"OPN",
		"STR",
		"CND",
		"UND",
		"FUN",
		"BLK",
		"BIF",
		"FAL",
	};

	if (value != NULL)
		fprintf(symbolDumpFileOutput, "%s: %#6x %d", valueKindTable[
			(int)value->kindOfValue], value->value, value->value);
	else
		fprintf(symbolDumpFileOutput, "(no value)");
}

  void
outputReservations()
{
	while (reservationList != NULL) {
		outputWord(reservationList->startAddress);
		outputWord(reservationList->blockSize);
		reservationList = reservationList->nextReservation;
	}
}

  void
outputExpressionBuffer()
{
	int	i;

	outputBigword(expressionBufferSize);
	if (debug || emitPrint)
		printExpressionBuffer();
	for (i=0; i<expressionBufferSize; ++i)
		outputByte(expressionBuffer[i]);
}

  void
outputOneExpression(expression)
  expressionType	*expression;
{
	expressionType	*newExpression;

	expressionType	*generateFixupExpression();
	bool		 encodeExpression();

	expressionBufferSize = 0;
	if (expression == NULL) {
		encodeRelocatableNumber(0);
		outputExpressionBuffer();
	} else {
		newExpression = generateFixupExpression(expression);
		freeExpression(expression);
		if (encodeExpression(newExpression))
			outputExpressionBuffer();
	}
}

  void
outputExpressions()
{
    int				 codeMode;
    expressionReferenceListType	*theReferences;

    encodingFunction = FALSE;
    outputBigword(referenceCount);
    for (codeMode=0; codeMode<=1; codeMode++) {
        theReferences = expressionReferenceList[codeMode];
        while (theReferences != NULL) {
	    outputOneExpression(theReferences->expressionReferenced);
            theReferences = theReferences->nextReference;
        }
    }
    if (haveUserStartAddress && fixupStartAddress)
	outputOneExpression(startAddress);
}

  void
outputOneFunction(function)
  functionDefinitionType	*function;
{
	argumentDefinitionListType	*argumentList;
	bool				 encodeBlock();
	int				 countArguments();

	outputByte((byte)countArguments(function));
	argumentList = function->arguments;
	while (argumentList != NULL) {
		outputBigword(argumentList->theArgument->ordinal);
		argumentList = argumentList->nextArgument;
	}
	expressionBufferSize = 0;
	if (encodeBlock(function->body))
		outputExpressionBuffer();
}

  void
outputFunctions()
{
	outputBigword(externalFunctionCount);
	encodingFunction = TRUE;
	while (externalFunctionCount-- > 0) {
		outputOneFunction(externalFunctionList);
		externalFunctionList = externalFunctionList->
			nextExternalFunction;
	}
}
