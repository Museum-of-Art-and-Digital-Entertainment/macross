/*
	link.c -- Primary routines for the Slinky linker.

	Chip Morningstar -- Lucasfilm Ltd.

	9-March-1985
*/

#include "slinkyTypes.h"
#include "slinkyGlobals.h"
/*#define readWord(f,fn) ((getc(f) & 0xFF) | ((getc(f) & 0xFF)<<8))*/

/*
   'internalizeOneObjectFile' does just that: it reads an object file into
   the master data structure.  The object file format is:

	0xFFFF				-- A "magic number" to signal the
					   start of an object file.
	ABSOLUTE code segments		-- In the form of a 2-byte start
					   address, followed by a 2-byte end
					   address, followed by the requisite
					   number of code bytes.
	0xFFFF				-- A partition.
	RELOCATABLE code segments	-- In the same format as the ABSOLUTE
					   code segments.
	0xFFFF				-- Another partition.
	space reservation blocks	-- each in the form of a 2-byte start
					   address followed by a 2-byte
					   length.
	0xFFFF				-- Yet another partition.
	number of reference items	-- A 4-byte count.
	that many reference items	-- Each a 'referenceType' in binary
					   form.
	number of symbol entries	-- A 4-byte count.
	number of bytes of strings	-- A 4-byte count of the sum total of
					   string space used by all of the
					   following symbol entries.
	that many symbol entries	-- Each a 'symbolType' in binary form
					   followed by a string with the
					   symbol name.
**new**
	number of expression entries	-- A 4-byte count.
	that many expression entries	-- Each a 4-byte length indicator
					   followed by the indicated number of
					   bytes representing a compacted
					   parse tree of an expression.
	number of function entries	-- A 4-byte count.
	that many function entries	-- Each a 1 byte argument count,
					   followed by that many symbol
					   numbers, followed by a compacted
					   parse tree of the function body
					   itself represented in the same
					   manner as expressions (i.e.,
					   length + bytes).
**end of new stuff**
	end-of-file

   Potential gotchas:

   1) A length 1 code segment signals the program entry point and is not
   really code (i.e., start address==end address==entry point).

   2) A length 0 code segment encodes a constraint instruction.  The 2-byte
   word following gives the multiple to constrain by.

   3) A length -1 code segment encodes an aligment instruction.  The 2-byte
   word following gives the multiple to align to.

   4) It is assumed that Macross will output all code segments of a given mode
   in strictly ascending order of start address.


 */

  bool
internalizeOneObjectFile(objectFile)
  objectFileListType	*objectFile;
{
	FILE		*objectFildes;
	int		 magic;
	int		 mode;
	addressType	 startAddress;
	addressType	 endAddress;
	bool		 compareSymbolValues();

	currentFileName = objectFile->name;
	if ((objectFildes = fopen(objectFile->name, "r")) == NULL) {
		error(CANT_OPEN_OBJECT_FILE_ERROR, objectFile->name);
		perror("Unix says");
		return(FALSE);
	}
	if (verbose)
		printf("internalizing %s:\n", objectFile->name);
	if ((magic = readWord(objectFildes, objectFile->name)) != 0xFFFF) {
		error(BAD_OBJECT_FILE_ERROR, objectFile->name);
		return(FALSE);
	}
	mode = MODE_ABSOLUTE;
	for (;;) {
		startAddress = readWord(objectFildes, objectFile->name);
		if (startAddress == 0xFFFF) {
			if (mode == MODE_ABSOLUTE) {
				mode = MODE_RELOCATABLE;
				continue;
			} else {
				break;
			}
		}
		endAddress = readWord(objectFildes, objectFile->name);
		readCode(startAddress, endAddress, mode, objectFile,
			objectFildes);
	}
	readReservations(objectFile, objectFildes);
	readReferences(objectFile, objectFildes);
	readSymbols(objectFile, objectFildes);
	readExpressions(objectFile, objectFildes);
	readFunctions(objectFile, objectFildes);
	instantiateExpressionAndSymbolPointers(objectFile);
	if (readExpressionEntryPoint) {
		pc = entryPointExpression;
		putSymbolPointersIntoExpression();
		readExpressionEntryPoint = FALSE;
		haveExpressionEntryPoint = TRUE;
	}
	qsort(objectFile->symbolTable, objectFile->symbolCount,
		sizeof(symbolType *), compareSymbolValues);
	fclose(objectFildes);
	return(TRUE);
}

#define toLowerCase(c) (('A'<=(c)&&(c)<='Z')?((c)-'A'+'a'):(c));

  bool
strcmplc(s1, s2)
  char	*s1;
  char	*s2;
{
	register char	c1;
	register char	c2;
	int		result;

	do {
		c1 = *s1++;
		c1 = toLowerCase(c1);
		c2 = *s2++;
		c2 = toLowerCase(c2);

		/* if result != 0, they differ */
		if (result = c1 - c2) {
			return(result);	/* c1<c2==neg, c1>c2==pos */
		} else if (!c1) {	/* if they're null, we're done */
			return(0);
		}
	} while (TRUE);
}

  bool
compareSymbols(symbol1, symbol2)
  symbolType	**symbol1;
  symbolType	**symbol2;
{
	bool	result;

	result = strcmplc((*symbol1)->symbolName, (*symbol2)->symbolName);
	if (result == 0 && (((*symbol1)->symbolClass & ~SYMBOL_EXTERNAL) &&
			((*symbol2)->symbolClass & ~SYMBOL_EXTERNAL))) {
		error(MULTIPLY_DEFINED_SYMBOL_ERROR, (*symbol1)->symbolName);
		(*symbol2)->symbolClass &= ~SYMBOL_ABSOLUTE;
		(*symbol2)->symbolClass &= ~SYMBOL_RELOCATABLE;
	}
	return(result);
}

  void
buildGlobalSymbolTable(inputFileList)
  objectFileListType	*inputFileList;
{
	int		  symbolCount;
	symbolType	**symbol;
	symbolType	**globalSymbolPtr;

	globalSymbolTable = typeAllocBlock(symbolType *, globalSymbolCount);
	globalSymbolPtr = globalSymbolTable;
	for (; inputFileList != NULL; inputFileList = inputFileList->
			nextObjectFile) {
		if (inputFileList->name != NULL) {
			for (symbol = inputFileList->symbolTable,
					symbolCount = inputFileList->
					symbolCount; symbolCount>0; symbol++,
					symbolCount--) {
				if (((*symbol)->symbolClass & 
						SYMBOL_EXTERNAL) &&
						((*symbol)->symbolClass &
						~SYMBOL_EXTERNAL)) {
					*globalSymbolPtr++ = (*symbol);
				}
			}
		}
	}
	qsort(globalSymbolTable, globalSymbolCount, sizeof(symbolType *),
		compareSymbols);
	if (debug) {
		printGlobalSymbols();
	}
}

  bool
readem()
{
	objectFileListType	*inputFileList;

	if (objectFileList == NULL) {
		error(NO_INPUT_FILES_ERROR);
		return(FALSE);
	}
	inputFileList = objectFileList;
	while (inputFileList != NULL) {
		if (inputFileList->name != NULL)
			if (!internalizeOneObjectFile(inputFileList))
				return(FALSE);
		inputFileList = inputFileList->nextObjectFile;
	}
	buildGlobalSymbolTable(objectFileList);
	return(TRUE);
}

  codeSegmentHeaderType	*
locateConflictingSegment(codeSegment)
  codeSegmentHeaderType	*codeSegment;
{
	segmentListType		*segmentPtr;
	int			 segmentListOffset;
	int			 segmentListOffsetStart;
	int			 segmentListOffsetEnd;
	addressType		 start;
	addressType		 end;

	start = codeSegment->segmentStartAddress;
	end = codeSegment->segmentEndAddress;
/*	segmentListOffsetStart = (start / CODE_REGION_SIZE) - 1;
	segmentListOffsetEnd = segmentListOffsetStart + 2;
	if (segmentListOffsetStart < 0)*/
		segmentListOffsetStart = 0;
/*	if (segmentListOffsetEnd >= CODE_REGIONS_IN_ADDRESS_SPACE)*/
		segmentListOffsetEnd = CODE_REGIONS_IN_ADDRESS_SPACE - 1;
	for (segmentListOffset = segmentListOffsetStart;
			segmentListOffset <= segmentListOffsetEnd;
			segmentListOffset++) {
	    segmentPtr = generatedLoadImage[segmentListOffset];
	    while (segmentPtr != NULL) {
		if ((segmentPtr->thisSegment->segmentStartAddress<=start &&
		     start<=segmentPtr->thisSegment->segmentEndAddress) ||
		    (segmentPtr->thisSegment->segmentStartAddress<=end &&
		     end<=segmentPtr->thisSegment->segmentEndAddress))
			return(segmentPtr->thisSegment);
		else
			segmentPtr = segmentPtr->nextSegment;
	    }
	}
	printf("Hey! segment list overrun in locateConflictingSegment\n");
	return(NULL);
}

  void
reserveSegment(start, end)
  addressType	start;
  addressType	end;
{
	freeSegmentEntryType	*freeSegmentPtr;
	freeSegmentEntryType	*previousSegmentPtr;
	freeSegmentEntryType	*newSegmentPtr;

	previousSegmentPtr = NULL;
	freeSegmentPtr = freeSegmentList;
	while (freeSegmentPtr != NULL && start > freeSegmentPtr->
			segmentEndAddress) {
		previousSegmentPtr = freeSegmentPtr;
		freeSegmentPtr = freeSegmentPtr->nextFreeSegment;
	}
	if (end < freeSegmentPtr->segmentStartAddress) {
		/* return; */
	} else if (start <= freeSegmentPtr->segmentStartAddress && end <
			freeSegmentPtr->segmentEndAddress) {
		freeSegmentPtr->segmentStartAddress = end + 1;
	} else if (end >= freeSegmentPtr->segmentEndAddress) {
		if (start > freeSegmentPtr->segmentStartAddress)
			freeSegmentPtr->segmentEndAddress = start - 1;
		if (previousSegmentPtr == NULL) {
			while (end >= freeSegmentPtr->segmentEndAddress) {
				freeSegmentList = freeSegmentPtr->
					nextFreeSegment;
				free(freeSegmentPtr);
				freeSegmentPtr = freeSegmentList;
			}
		} else {
			while (end >= freeSegmentPtr->segmentEndAddress) {
				previousSegmentPtr->nextFreeSegment =
					freeSegmentPtr->nextFreeSegment;
				free(freeSegmentPtr);
				freeSegmentPtr = previousSegmentPtr->
					nextFreeSegment;
			}
		}
		if (end >= freeSegmentPtr->segmentStartAddress) {
			freeSegmentPtr->segmentStartAddress = end + 1;
		}
	} else {
		newSegmentPtr = typeAlloc(freeSegmentEntryType);
		newSegmentPtr->nextFreeSegment = freeSegmentPtr->
			nextFreeSegment;
		freeSegmentPtr->nextFreeSegment = newSegmentPtr;
		newSegmentPtr->segmentEndAddress = freeSegmentPtr->
			segmentEndAddress;
		freeSegmentPtr->segmentEndAddress = start - 1;
		newSegmentPtr->segmentStartAddress = end + 1;
	}
}

  codeSegmentHeaderType	*
allocateAbsolute(codeSegment)
  codeSegmentHeaderType	*codeSegment;
{
	freeSegmentEntryType	*freeSegmentPtr;
	freeSegmentEntryType	*previousSegmentPtr;
	freeSegmentEntryType	*newSegmentPtr;
	addressType		 start;
	addressType		 end;

	previousSegmentPtr = NULL;
	freeSegmentPtr = freeSegmentList;
	start = codeSegment->segmentStartAddress;
	end = codeSegment->segmentEndAddress;
	while (freeSegmentPtr != NULL && start > freeSegmentPtr->
			segmentEndAddress) {
		previousSegmentPtr = freeSegmentPtr;
		freeSegmentPtr = freeSegmentPtr->nextFreeSegment;
	}
	if (freeSegmentPtr->segmentEndAddress < end || start<freeSegmentPtr->
			segmentStartAddress)
		return(locateConflictingSegment(codeSegment));
	if (freeSegmentPtr->segmentStartAddress == start || freeSegmentPtr->
			segmentEndAddress == end) {
		if (freeSegmentPtr->segmentStartAddress == start)
			freeSegmentPtr->segmentStartAddress = end + 1;
		if (freeSegmentPtr->segmentEndAddress == end)
			freeSegmentPtr->segmentEndAddress = start - 1;
		if (freeSegmentPtr->segmentEndAddress < freeSegmentPtr->
				segmentStartAddress) {
			if (previousSegmentPtr == NULL)
				freeSegmentList = freeSegmentPtr->
					nextFreeSegment;
			else
				previousSegmentPtr->nextFreeSegment =
					freeSegmentPtr->nextFreeSegment;
			free(freeSegmentPtr);
		}
	} else {
		newSegmentPtr = typeAlloc(freeSegmentEntryType);
		newSegmentPtr->nextFreeSegment = freeSegmentPtr->
			nextFreeSegment;
		freeSegmentPtr->nextFreeSegment = newSegmentPtr;
		newSegmentPtr->segmentEndAddress = freeSegmentPtr->
			segmentEndAddress;
		freeSegmentPtr->segmentEndAddress = start - 1;
		newSegmentPtr->segmentStartAddress = end + 1;
	}
	return(NULL);
}

  void
reserveReservations()
{
	while (reservationList != NULL) {
		reserveSegment(reservationList->startAddress,
			reservationList->startAddress + reservationList->
			blockSize - 1);
		reservationList = reservationList->nextReservation;
	}
}

  void
installSegment(codeSegment)
  codeSegmentHeaderType	*codeSegment;
{
    segmentListType	*previousSegment;
    segmentListType	*installSegmentList;
    segmentListType	*newSegmentListEntry;
    int			 regionNumber;
    int			 endRegion;

    regionNumber = regionOf(codeSegment->segmentStartAddress);
    previousSegment = NULL;
    installSegmentList = generatedLoadImage[regionNumber];
    while (installSegmentList != NULL && installSegmentList->thisSegment->
	    segmentStartAddress < codeSegment->segmentStartAddress) {
        previousSegment = installSegmentList;
        installSegmentList = installSegmentList->nextSegment;
    }
    newSegmentListEntry = typeAlloc(segmentListType);
    newSegmentListEntry->thisSegment = codeSegment;
    newSegmentListEntry->nextSegment = installSegmentList;
    if (previousSegment == NULL) {
        generatedLoadImage[regionNumber] = newSegmentListEntry;
    } else {
        previousSegment->nextSegment = newSegmentListEntry;
    }
    if (regionNumber < (endRegion=regionOf(codeSegment->segmentEndAddress))) {
        for (regionNumber++; regionNumber <= endRegion; regionNumber++) {
            newSegmentListEntry = typeAlloc(segmentListType);
            newSegmentListEntry->thisSegment = codeSegment;
            newSegmentListEntry->nextSegment=generatedLoadImage[regionNumber];
            generatedLoadImage[regionNumber] = newSegmentListEntry;
        }
    }
}

  void
installAbsoluteCodeSegment(codeSegment)
  codeSegmentHeaderType	*codeSegment;
{
    codeSegmentHeaderType	*conflictingSegment;

    if ((conflictingSegment = allocateAbsolute(codeSegment)) != NULL) {
        error(OVERLAPPING_ABSOLUTE_CODE_SEGMENTS_ERROR, conflictingSegment->
	    fileName, conflictingSegment->segmentStartAddress,
	    conflictingSegment->segmentEndAddress, codeSegment->fileName,
	    codeSegment->segmentStartAddress, codeSegment->segmentEndAddress);
    } else {
	installSegment(codeSegment);
    }
}

  void
linkem()
{
	if (!readem())
		return;
	relocatem();
	valuem();
	pokem();
	writem();
}
