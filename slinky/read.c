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
	read.c -- Routines to read stuff in from object files for the Slinky
		linker.

	Chip Morningstar -- Lucasfilm Ltd.

	10-March-1985

*/

#include "slinkyTypes.h"
#include "slinkyGlobals.h"
#include "read.h"
#include "debugPrint.h"
#include "errorStuff.h"
#include "initialize.h"
#include "instantiate.h"
#include "link.h"

#define isAbsolute(symbol) (((symbol)->symbolClass & SYMBOL_ABSOLUTE) != 0)
#define isRelocatable(symbol) (((symbol)->symbolClass &SYMBOL_RELOCATABLE)!=0)

  void
fileCheck(FILE *fildes, char *fileName)
{
	if (feof(fildes)) {
		error(PREMATURE_EOF_ERROR, fileName);
		chokePukeAndDie();
	} else if (ferror(fildes)) {
		error(ERROR_READING_OBJECT_FILE, fileName);
		perror("Unix says");
		chokePukeAndDie();
	}
}

  wordType
readWord(FILE *file, char *fileName)
{
	wordType	result;
	register char	loByte;
	register char	hiByte;

	loByte = getc(file);
	hiByte = getc(file);
	fileCheck(file, fileName);
	result = ((hiByte & 0xFF) << 8) | (loByte & 0xFF);
	return(result);
}

  byte
readByte(FILE *file, char *fileName)
{
	int	result;

	if ((result = getc(file)) == EOF) {
		fileCheck(file, fileName);
	}
	return(result);
}

  bigWord
readBigword(FILE *file, char *fileName)
{
	register bigWord	result;

	result = getc(file) & 0xFF;
	result |= (getc(file) & 0xFF) << 8;
	result |= (getc(file) & 0xFF) << 16;
	result |= (getc(file) & 0xFF) << 24;
	fileCheck(file, fileName);
	return(result);
}

  bigWord
read3ByteWord(FILE *file, char *fileName)
{
	register bigWord	result;

	result = getc(file) & 0xFF;
	result |= (getc(file) & 0xFF) << 8;
	result |= (getc(file) & 0xFF) << 16;
	fileCheck(file, fileName);
	return(result);
}

  int
readString(char *buffer, FILE *fildes, char *fileName)
{
	register char	 c;
	register char	*scratchBuffer;

	scratchBuffer = buffer;
	while ((c = getc(fildes)) != EOF  &&  c != '\0')
		*scratchBuffer++ = c;
	fileCheck(fildes, fileName);
	*scratchBuffer++ = '\0';
	return(scratchBuffer - buffer);
}

  void
readChunk(byte *buffer, int numberOfBytes, FILE *fildes, char *fileName)
{
	do {
		*buffer++ = getc(fildes);
	} while (--numberOfBytes > 0);
	fileCheck(fildes, fileName);
}

  void
readCode(addressType startAddress, addressType endAddress, int mode, objectFileListType *objectFile, FILE *objectFildes)
{
	int			 size;
	byte			*codeBuffer;
	codeSegmentHeaderType	*newCodeSegment;
	static int		 currentConstraint = 0;
	static int		 currentAlignment = 0;

	size = (short)endAddress - (short)startAddress + 1;
	if (size == -2) {
		entryPointAddress = startAddress;
		entryPointMode = mode;
		haveEntryPoint = TRUE;
		readByte(objectFildes, objectFile->name);
		return;
	} else if (size == 0) {
		currentConstraint = readWord(objectFildes, objectFile->name);
		return;
	} else if (size == -1) {
		currentAlignment = readWord(objectFildes, objectFile->name);
		return;
	}
	size = (wordType)endAddress - (wordType)startAddress + 1;
	codeBuffer = typeAllocBlock(byte, size);
	readChunk(codeBuffer, size, objectFildes, objectFile->name);
	newCodeSegment = typeAlloc(codeSegmentHeaderType);
	newCodeSegment->fileName = objectFile->name;
	newCodeSegment->segmentStartAddress = startAddress;
	newCodeSegment->segmentEndAddress = endAddress;
	newCodeSegment->segmentMode = mode;
	newCodeSegment->segmentCodeBuffer = codeBuffer;
	newCodeSegment->relocationOffset = 0;
	newCodeSegment->constraint = currentConstraint;
	newCodeSegment->alignment = currentAlignment;
	currentConstraint = currentAlignment = 0;
	if (objectFile->lastCodeSegment == NULL) {
		objectFile->codeSegments = newCodeSegment;
	} else {
		objectFile->lastCodeSegment->nextSegment = newCodeSegment;
	}
	objectFile->lastCodeSegment = newCodeSegment;
	newCodeSegment->nextSegment = NULL;
	if (mode == MODE_ABSOLUTE)
		installAbsoluteCodeSegment(newCodeSegment);
	if (debug)
		printCode(startAddress, endAddress, mode);
}

  bool
compareReferences(expressionReferenceType *reference1, expressionReferenceType *reference2)
{
	if (reference1->referenceMode == MODE_ABSOLUTE && reference2->
			referenceMode == MODE_RELOCATABLE)
		return(-1);
	else
		return(reference1->referenceAddress - reference2->
			referenceAddress);
/*	else if (reference1->referenceAddress < reference2->referenceAddress)
		return(-1);
	else if (reference1->referenceAddress > reference2->referenceAddress)
		return(1);
	else
		return(0);*/
}

  void
sortReferences(expressionReferenceType *theReferences, int numberOfReferences)
{
	qsort(theReferences, numberOfReferences,
			sizeof(expressionReferenceType), compareReferences);
}

  void
readReference(expressionReferenceType *reference, FILE *fildes, char *fileName)
{
	register byte	funnyByte;

	reference->referenceAddress = read3ByteWord(fildes, fileName);
	funnyByte = readByte(fildes, fileName);
	reference->referenceMode = funnyByte & 1;
	reference->referenceRelative = (funnyByte >> 1) & 1;
	reference->referenceExternal = (funnyByte >> 2) & 1;
	reference->referenceKind = (funnyByte >> 3) & 7;
	reference->referenceExpression.inFile = readBigword(fildes, fileName);
	if (debug) {
		printReference(reference);
	}
}

  void
readReferences(objectFileListType *objectFile, FILE *objectFildes)
{
	int			 count;
	int			 readCount;
	expressionReferenceType	*theReferences;
	expressionReferenceType	*readPtr;
	expressionReferenceType	*subPtr;
	codeSegmentHeaderType	*codeSegment;

	readCount = count = readBigword(objectFildes, objectFile->name);
	if (readCount == 0)
		readPtr = theReferences = NULL;
	else
		readPtr = theReferences =
			typeAllocBlock(expressionReferenceType, count);
	if (debug)
		printf("	%d references\n", count);
	while (readCount-- > 0)
		readReference(readPtr++, objectFildes, objectFile->name);
	sortReferences(theReferences, count);
	if (debug) {
		printf("	sorted references\n");
		for (readCount=0; readCount<count; readCount++)
			printReference(&(theReferences[readCount]));
	}
	readPtr = theReferences;
	subPtr = theReferences;
	readCount = count;
	for (codeSegment = objectFile->codeSegments; codeSegment != NULL;
			codeSegment = codeSegment->nextSegment) {
		codeSegment->segmentReferences = readPtr;
		while (codeSegment->nextSegment != NULL && readPtr != NULL &&
				readPtr->referenceAddress <= codeSegment->
				segmentEndAddress && readPtr->
				referenceMode == codeSegment->segmentMode &&
				readCount-- > 0) {
			readPtr++;
		}
		if (codeSegment->nextSegment != NULL) {
			codeSegment->referenceCount = readPtr - subPtr;
			subPtr = readPtr;
		} else {
			codeSegment->referenceCount = count - (readPtr -
				theReferences);
		}
	}
}

  bool
compareSymbolValues(symbolType **symbol1, symbolType **symbol2)
{
	if ((isAbsolute(*symbol1) && !isAbsolute(*symbol2)) ||
			(isRelocatable(*symbol1) && !isRelocatable(*symbol2)
			&& !isAbsolute(*symbol2)))
		return(-1);
	else if ((isAbsolute(*symbol2) && !isAbsolute(*symbol1)) ||
			(isRelocatable(*symbol2) && !isRelocatable(*symbol1)
			&& !isAbsolute(*symbol1)))
		return(1);
	else
		return((*symbol1)->symbolValue - (*symbol2)->symbolValue);
/*	else if ((*symbol1)->symbolValue < (*symbol2)->symbolValue)
		return(-1);
	else if ((*symbol1)->symbolValue == (*symbol2)->symbolValue)
		return(0);
	else
		return(1);*/
}

  void
readSymbols(objectFileListType *objectFile, FILE *objectFildes)
{
	symbolType	*symbolTable;
	symbolType     **symbolTableIndir;
	int		 symbolCount;
	int		 stringSize;
	char		*stringBuffer;
	char		*fileName;
	int		 symbolTag;

	fileName = objectFile->name;
	symbolCount = readBigword(objectFildes, fileName);
	objectFile->symbolCount = symbolCount;
	stringSize = readBigword(objectFildes, fileName);
	if (debug) {
		printf("	%d symbols with %d bytes of strings\n",
			symbolCount, stringSize);
	}
	symbolTable = typeAllocBlock(symbolType, symbolCount);
	symbolTableIndir = typeAllocBlock(symbolType *, symbolCount);
	stringBuffer = typeAllocBlock(char, stringSize);
	objectFile->symbolTable = symbolTableIndir;
	symbolTag = 0;
	while (symbolCount-- > 0) {
		*symbolTableIndir++ = symbolTable;
		symbolTable->symbolClass = readByte(objectFildes, fileName);
		symbolTable->symbolValue = readBigword(objectFildes,fileName);
		symbolTable->symbolName = stringBuffer;
		stringBuffer += readString(stringBuffer, objectFildes,
			fileName);
		if (debug) {
			printSymbol(symbolTag, symbolTable);
			symbolTag++;
		}
		if ((symbolTable->symbolClass & SYMBOL_EXTERNAL) &&
		    (symbolTable->symbolClass & ~SYMBOL_EXTERNAL))
			globalSymbolCount++;
		symbolTable++;
	}
}

  expressionPCType
readOneExpression(objectFileListType *objectFile, FILE *objectFildes)
{
	char			*fileName;
	int			 expressionSize;
	byte			*expressionBuffer;
	expressionPCType	 result;
	int			 saveExpressionSize;

	fileName = objectFile->name;
	expressionSize = readBigword(objectFildes, fileName);
	if (expressionSize == 0)
		return(NULL);
	expressionBuffer = typeAllocBlock(byte, expressionSize);
	result = expressionBuffer;
	saveExpressionSize = expressionSize;
	do {
		*expressionBuffer++ = readByte(objectFildes, fileName);
	} while (--expressionSize > 0);
	if (debug) {
		printExpression(result, saveExpressionSize);
	}
	return(result);
}

  void
readExpressions(objectFileListType *objectFile, FILE *objectFildes)
{
	expressionPCType	*expressions;
	int		         expressionCount;
	int			 expressionSize;
	int			 saveExpressionSize;
	char			*fileName;
	byte			*expressionBuffer;

	fileName = objectFile->name;
	expressionCount = readBigword(objectFildes, fileName);
	objectFile->expressionCount = expressionCount;
	if (debug)
		printf("	%d expressions\n", expressionCount);
	expressions = typeAllocBlock(expressionPCType, expressionCount);
	objectFile->expressions = expressions;
	while (expressionCount-- > 0) {
		*expressions = readOneExpression(objectFile, objectFildes);
		expressions++;
	}
	if (haveEntryPoint && entryPointAddress==0xFFFE && entryPointMode==
			MODE_RELOCATABLE && !haveExpressionEntryPoint) {
		if (debug)
			printf("Start address expression:\n");
		entryPointAddress = 0;
		entryPointExpression = readOneExpression(objectFile,
			objectFildes);
		readExpressionEntryPoint = TRUE;
	}
}

  argumentListType *
readArgumentList(objectFileListType *objectFile, FILE *objectFildes)
{
	int			 argumentCount;
	char			*fileName;
	argumentListType	*result;
	int			 i;
	int			 symbolNumber;

	fileName = objectFile->name;
	argumentCount = readByte(objectFildes, fileName);
	if (argumentCount == 0)
		return(NULL);
	result = typeAllocBlock(argumentListType, argumentCount);
	for (i=0; i<argumentCount-1; ++i)
		result[i].nextArgument = &(result[i+1]);
	result[argumentCount].nextArgument = NULL;
	for (i=0; i<argumentCount; ++i) {
		symbolNumber = readBigword(objectFildes, fileName);
		result[i].argumentSymbol = objectFile->
				symbolTable[symbolNumber];
	}
	return(result);
}

  void
readFunctions(objectFileListType *objectFile, FILE *objectFildes)
{
	functionType	*functions;
	int	         functionCount;
	int		 functionSize;
	int		 argumentCount;
	int		 useFunctionSize;
	char		*fileName;
	byte		*functionBuffer;

	fileName = objectFile->name;
	functionCount = readBigword(objectFildes, fileName);
	objectFile->functionCount = functionCount;
	if (debug)
		printf("	%d functions\n", functionCount);
	functions = typeAllocBlock(functionType, functionCount);
	objectFile->functions = functions;
	while (functionCount-- > 0) {
		functions->functionArguments = readArgumentList(objectFile,
			objectFildes);		
		functionSize = readBigword(objectFildes, fileName);
		functionBuffer = typeAllocBlock(byte, functionSize);
		functions->functionBody = functionBuffer;
		useFunctionSize = functionSize;
		while (useFunctionSize-- > 0)
			*functionBuffer++ = readByte(objectFildes,fileName);
		if (debug) {
			printExpression(functions->functionBody,functionSize);
		}
		functions++;
	}
}

  void
instantiateExpressionAndSymbolPointers(objectFileListType *objectFile)
{
    symbolType		       	       **symbolTable;
    expressionPCType			*expressions;
    functionType			*functions;
    int					 i;
    codeSegmentHeaderType		*codeSegment;
    expressionReferenceType		*reference;
    int				 	 referenceCount;

        currentSymbolTable = objectFile->symbolTable;
	currentFunctionTable = objectFile->functions;
	expressions = objectFile->expressions;
        for (codeSegment = objectFile->codeSegments; codeSegment != NULL;
		codeSegment = codeSegment->nextSegment) {
            for (reference = codeSegment->segmentReferences, referenceCount =
		    codeSegment->referenceCount; referenceCount > 0;
		    reference++, referenceCount--) {
                pc = reference->referenceExpression.inCore = expressions[
			reference->referenceExpression.inFile];
		putSymbolPointersIntoExpression();
	    }
	}
	functions = objectFile->functions;
	for (i=0; i<objectFile->functionCount; ++i) {
		pc = functions[i].functionBody;
		putSymbolPointersIntoExpression();
	}
}

  void
readReservations(objectFileListType *objectFile, FILE *objectFildes)
{
	addressType		 startAddress;

	if (debug)
		printf("	reservations\n");
	while ((startAddress = readWord(objectFildes, objectFile->name)) !=
			0xFFFF)
		reservationList = buildReservation(startAddress,
			readWord(objectFildes, objectFile->name),
			reservationList);
}

  reservationListType *
buildReservation(addressType startAddress, int blockSize, reservationListType *nextReservation)
{
	reservationListType	*result;

	result = typeAlloc(reservationListType);
	result->startAddress = startAddress;
	result->blockSize = blockSize;
	result->nextReservation = nextReservation;
	if (debug)
		printf("    Res: 0x%x 0x%x @0x%x -> 0x%x\n", startAddress,
			blockSize, result, nextReservation);
	return(result);
}
