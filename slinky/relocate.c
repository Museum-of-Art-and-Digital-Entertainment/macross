/*
	relocate.c -- Routines to relocate relocatable code for the Slinky
			linker.

	Chip Morningstar -- Lucasfilm Ltd.

	15-March-1985
*/

#include "slinkyTypes.h"
#include "slinkyGlobals.h"

#define isUndefined(symbol) (((symbol)->symbolClass & ~SYMBOL_EXTERNAL) == 0)

  void
removeZeroPageFromFreeList()
{
	while (freeSegmentList->segmentEndAddress <= 0x100)
		freeSegmentList = freeSegmentList->nextFreeSegment;
	if (freeSegmentList->segmentStartAddress < 0x100)
		freeSegmentList->segmentStartAddress = 0x100;
}

  addressType
align(address, alignment)
  addressType	address;
  int		alignment;
{
	if (alignment == 0)
		return(address);
	else
		return(((address + alignment - 1) / alignment) * alignment);
}

  addressType
constrain(address, size, constraint)
  addressType	address;
  int		size;
  addressType	constraint;
{
	if (constraint == 0)
		return(address);
	else if (address/constraint != (address + size - 1)/constraint)
		return(align(address, constraint));
	else
		return(address);
}
  void
moveRelocationBase(newBase)
  addressType	newBase;
{
	freeSegmentEntryType	*freePtr;
	freeSegmentEntryType	*newFreePtr;

	freePtr = freeSegmentList;
	while (freePtr != NULL && newBase > freePtr->segmentEndAddress)
		freePtr = freePtr->nextFreeSegment;
	if (freePtr == NULL || freePtr->segmentStartAddress == newBase) {
		effectiveFreeSegmentList = freePtr;
	} else {
		newFreePtr = typeAlloc(freeSegmentEntryType);
		newFreePtr->segmentEndAddress = freePtr->segmentEndAddress;
		freePtr->segmentEndAddress = newBase - 1;
		newFreePtr->segmentStartAddress = newBase;
		newFreePtr->nextFreeSegment = freePtr->nextFreeSegment;
		freePtr->nextFreeSegment = newFreePtr;
		effectiveFreeSegmentList = newFreePtr;
	}
}

  addressType
allocateRelocatable(codeSegment)
  codeSegmentHeaderType	*codeSegment;
{
	freeSegmentEntryType	*freePtr;
	freeSegmentEntryType	*previousPtr;
	freeSegmentEntryType	*newFreePtr;
	int			 size;
	addressType		 effectiveStartAddress;

	freePtr = effectiveFreeSegmentList;
	previousPtr = NULL;
	size = codeSegment->segmentEndAddress - codeSegment->
	    segmentStartAddress + 1;
	while (freePtr != NULL) {
	    effectiveStartAddress = align(freePtr->segmentStartAddress,
	        codeSegment->alignment);
	    effectiveStartAddress = constrain(effectiveStartAddress, size,
	        codeSegment->constraint);
	    if (freePtr->segmentEndAddress - effectiveStartAddress + 1 <
	            size) {
	        previousPtr = freePtr;
	        freePtr = freePtr->nextFreeSegment;
	        continue;
	    }
	    /* if we made it to here, we got a winner! */
	    if (effectiveStartAddress == freePtr->segmentStartAddress) {
		if (effectiveStartAddress + size - 1 == freePtr->
			segmentEndAddress) {
		    if (previousPtr == NULL)
			freeSegmentList = freePtr->nextFreeSegment;
		    else
			previousPtr->nextFreeSegment = freePtr->
			    nextFreeSegment;
		} else {
		    freePtr->segmentStartAddress += size;
		}
	    } else {
		if (effectiveStartAddress + size - 1 == freePtr->
			segmentEndAddress) {
		    freePtr->segmentEndAddress -= size;
		} else {
		    newFreePtr = typeAlloc(freeSegmentEntryType);
		    newFreePtr->nextFreeSegment = freePtr->nextFreeSegment;
		    freePtr->nextFreeSegment = newFreePtr;
		    newFreePtr->segmentEndAddress =freePtr->segmentEndAddress;
		    freePtr->segmentEndAddress = effectiveStartAddress - 1;
		    newFreePtr->segmentStartAddress = effectiveStartAddress +
			size;
		}
	    }
	    return(effectiveStartAddress);
	}
	return((addressType) -1);
}

  void
relocateOneCodeSegment(codeSegment, targetLocation)
  codeSegmentHeaderType	*codeSegment;
  addressType		 targetLocation;
{
	int	relocationOffset;

	relocationOffset = targetLocation - codeSegment->segmentStartAddress;
	if (verbose)
		printf(" 0x%04x:0x%04x --> 0x%04x", codeSegment->
			segmentStartAddress, codeSegment->segmentEndAddress,
			targetLocation);
	codeSegment->relocationOffset = relocationOffset;
	codeSegment->segmentStartAddress += relocationOffset;
	codeSegment->segmentEndAddress += relocationOffset;
	if (!packFlag) {
		moveRelocationBase(codeSegment->segmentEndAddress+1);
	}
	if (verbose)
		printf(" (0x%04x:0x%04x)\n", codeSegment->segmentStartAddress,
			codeSegment->segmentEndAddress);
	installSegment(codeSegment);
}

  void
relocatem()
{
	objectFileListType	*inputFileList;
	addressType		 targetLocation;
	codeSegmentHeaderType	*codeSegment;

	reserveReservations();
	removeZeroPageFromFreeList();
	for (inputFileList = objectFileList; inputFileList != NULL;
		inputFileList = inputFileList->nextObjectFile) {
	    if (inputFileList->name != NULL) {
		currentFileName = inputFileList->name;
		if (verbose)
			printf("%s:\n", inputFileList->name);
	        for (codeSegment = inputFileList->codeSegments; codeSegment !=
		        NULL; codeSegment = codeSegment->nextSegment) {
		    if (codeSegment->segmentMode == MODE_RELOCATABLE) {
		        if ((targetLocation=allocateRelocatable(codeSegment))
				== (addressType) -1) {
			    error(NO_PLACE_TO_PUT_CODE_SEGMENT_ERROR,
				codeSegment->segmentStartAddress,
				    inputFileList->name);
		        } else {
		            relocateOneCodeSegment(codeSegment,
				targetLocation);
		        }
		    }
		}
	    } else { /* null name encodes new relocation base */
		moveRelocationBase(inputFileList->symbolCount);
	    }
	}
}

  codeSegmentHeaderType *
matchModes(symbol, codeSegment)
  symbolType		*symbol;
  codeSegmentHeaderType	*codeSegment;
{
	while (codeSegment!=NULL && ((codeSegment->segmentMode==MODE_ABSOLUTE
			&& !(symbol->symbolClass & SYMBOL_ABSOLUTE)) ||
			(codeSegment->segmentMode==MODE_RELOCATABLE &&
			!(symbol->symbolClass & SYMBOL_RELOCATABLE) &&
			!(symbol->symbolClass & SYMBOL_ABSOLUTE))))
		codeSegment = codeSegment->nextSegment;
	return(codeSegment);
}

  bool
matchedModes(symbol, codeSegment)
  symbolType		*symbol;
  codeSegmentHeaderType	*codeSegment;
{
	return(((symbol->symbolClass & SYMBOL_ABSOLUTE) && codeSegment->
		segmentMode == MODE_ABSOLUTE) || ((symbol->symbolClass &
		SYMBOL_RELOCATABLE) && codeSegment->segmentMode ==
		MODE_RELOCATABLE));
}

  codeSegmentHeaderType	*
synchronizeCodeSegment(symbol, codeSegment)
  symbolType		*symbol;
  codeSegmentHeaderType	*codeSegment;
{
	codeSegment = matchModes(symbol, codeSegment);
	while (codeSegment != NULL && codeSegment->nextSegment != NULL &&
			codeSegment->segmentEndAddress - codeSegment->
			relocationOffset < symbol->symbolValue &&
			matchedModes(symbol, codeSegment)) {
		codeSegment = codeSegment->nextSegment;
	}
	return(codeSegment);
}

  void
handleGlobalSymbol(symbol)
  symbolType	*symbol;
{
}

  void
valueSymbol(symbol, codeSegment)
  symbolType		*symbol;
  codeSegmentHeaderType	*codeSegment;
{
	if (symbol->symbolClass & SYMBOL_ABSOLUTE) {
		return;
	} else if (symbol->symbolClass & SYMBOL_RELOCATABLE) {
		symbol->symbolValue += codeSegment->relocationOffset;
	} else {
		printf("Super botcho!  Undefined symbol in ValueSymbol.\n");
	}
}

  symbolType *
lookupGlobalSymbol(symbolName)
  char	*symbolName;
{
	int	guess;
	int	top;
	int	bottom;
	int	compare;
	int	oldTop;
	int	oldBottom;

	top = oldBottom = globalSymbolCount;
	bottom = oldTop = 0;
	while (top != bottom && top != oldTop && bottom != oldBottom) {
		guess = (top + bottom) >> 1;
		compare = strcmplc(globalSymbolTable[guess]->symbolName,
			symbolName);
		if (compare == 0)
			return(globalSymbolTable[guess]);
		else if (compare < 0) {
			oldBottom = bottom;
			bottom = guess;
		} else {
			oldTop = top;
			top = guess;
		}
	}
	return(NULL);
}

  void
valueUndefinedSymbol(symbol)
  symbolType	*symbol;
{
	symbolType	*globalSymbol;

	if ((globalSymbol = lookupGlobalSymbol(symbol->symbolName)) == NULL) {
		error(UNDEFINED_SYMBOL_ERROR, symbol->symbolName);
	} else {
		symbol->symbolClass = globalSymbol->symbolClass;
		symbol->symbolValue = globalSymbol->symbolValue;
	}
}

  void
valuem()
{
	objectFileListType	*inputFileList;
	codeSegmentHeaderType	*codeSegmentPtr;
	symbolType		*symbolPtr;
	int			 symbolCount;

	totalSymbolCount = 0;
	for (inputFileList = objectFileList; inputFileList != NULL;
		inputFileList = inputFileList->nextObjectFile) {
	    if (inputFileList->name != NULL) {
		currentFileName = inputFileList->name;
	        if (debug)
		    printf("\nSymbols with values (%s):\n",  inputFileList->
		        name);
 	        codeSegmentPtr = inputFileList->codeSegments;
	        for (symbolCount = 0; symbolCount<inputFileList->symbolCount;
		        symbolCount++) {
		    symbolPtr = inputFileList->symbolTable[symbolCount];
		    if (isUndefined(symbolPtr)) {
/*		        inputFileList->undefinedSymbols = symbolPtr;*/
			inputFileList->undefinedSymbols = &(inputFileList->symbolTable[symbolCount]);
		        break;
		    }
		    codeSegmentPtr = synchronizeCodeSegment(symbolPtr,
		        codeSegmentPtr);
		    valueSymbol(symbolPtr, codeSegmentPtr);
		    if (debug)
		        printSymbol(symbolCount, symbolPtr);
	        }
	        inputFileList->baseSymbolCount = symbolCount;
	        inputFileList->symbolCount -= symbolCount;
	    }
	}

	for (inputFileList = objectFileList; inputFileList != NULL;
		inputFileList = inputFileList->nextObjectFile) {
	    if (inputFileList->name != NULL) {
		currentFileName = inputFileList->name;
	        if (debug)
		    printf("\nGlobal symbols with values (%s):\n",
			inputFileList->name);
	        for (symbolCount = 0; symbolCount <inputFileList->symbolCount;
		        symbolCount++) {
		    symbolPtr = inputFileList->undefinedSymbols[symbolCount];
		    valueUndefinedSymbol(symbolPtr);
		    if (debug)
		        printSymbol(symbolCount, symbolPtr);
	        }
	        inputFileList->symbolCount = inputFileList->baseSymbolCount;
	        totalSymbolCount += inputFileList->baseSymbolCount;
	    }
	}
	if (debug)
		printf("\nTotal symbol count = %d\n", totalSymbolCount);
	if (produceLoadMap)
		outputLoadMap();
}
