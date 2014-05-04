/*
	emitStuff.c -- Routines to actually generate binary stuff for the
		       Macross assembler.

	Chip Morningstar -- Lucasfilm Ltd.

	14-November-1984
*/

#include "macrossTypes.h"
#include "macrossGlobals.h"

/*
   Emitted code is stored in places that are allocated dynamically as they
   are needed.  This saves us from having to keep umpteenK of code buffer
   around ALL the time and also lets us keep track of where code has actually
   been put for purposes of generating the final object module.

   The structure of the code storage is: there are two code region structs
   (type 'codeRegionType'), one each for absolute and relocatable code.  These
   are stored in the global array 'codeRegions' which is indexed by
   'currentCodeMode' which in turn indicates whether we are emitting aboslute
   or relocatable code.  Each of these region structs contains a vector of
   [currently 64] pointers to code segment structs (type 'codeSegmentType').
   Each code segment represents a [currently 1Kbyte] piece of the target
   processor address space; it contains the high and low addresses within
   that space that have been occupied up until now and a pointer to a
   [1Kbyte] buffer that actually contains the code.

   Initially, the code regions are filled with vectors of null pointers and
   there are no code segments allocated.  As code is emitted, the space for
   the code segments and their associated buffers is allocated using 'malloc'.
   Only the actual buffers needed are allocated, resulting in substantial
   storage savings.

   A complication that can arise from Macross 'struct's.  Structs are
   assembled in a small scratch buffer and then transferred to the main code
   buffers as needed.
 */


/* incarnateCodeBuffer causes code buffer space to actually be allocated */

  void
incarnateCodeBuffer(bufferNum, bufferKind)
  int			bufferNum;
  codeBufferKindType	bufferKind;
{
	codeSegmentType	*newCodeSegment;
	codeBufferType	*newCodeBuffer;
	int		 i;

	newCodeSegment = typeAlloc(codeSegmentType);
	newCodeBuffer = typeAlloc(codeBufferType);
	for (i=0; i<CODE_BUFFER_SIZE; i++)
		(*newCodeBuffer)[i] = 0;
	newCodeSegment->codeStartAddress = 0xFFFF + 1;
	newCodeSegment->codeEndAddress = -1;
	newCodeSegment->codeBuffer = newCodeBuffer;
	codeRegions[(int)bufferKind]->codeSegments[bufferNum] =
		newCodeSegment;
}


/* putByte actually puts a byte in code storage somewhere, given the address
   and the byte itself.  It tracks down the appropriate code buffer, taking
   care to make sure said buffer actually exists before using it. */

  void
putByte(address, byteValue)
  addressType	address;
  byte		byteValue;
{
	int		 bufferNum;
	int		 bufferPos;
	codeBufferType	*theBuffer;
	codeSegmentType	*theSegment;

	bufferNum = bufferNumber(address);
	bufferPos = bufferPosition(address);
	if (bufferNum >= CODE_BUFFERS_IN_ADDRESS_SPACE) {
		fatalError(ADDRESS_OUTSIDE_ADDRESS_SPACE_ERROR, address);
		return;
	}
	theSegment = codeRegions[(int)currentCodeMode]->codeSegments[
		bufferNum];
	if (theSegment == NULL) {
		incarnateCodeBuffer(bufferNum, currentCodeMode);
		theSegment = codeRegions[(int)currentCodeMode]->codeSegments[
			bufferNum];
	}
	theBuffer = theSegment->codeBuffer;
	if (currentCodeMode == RELOCATABLE_BUFFER && address >
			relocatableHighWaterMark)
		relocatableHighWaterMark = address;
	if (address > theSegment->codeEndAddress)
		theSegment->codeEndAddress = address;
	if (address < theSegment->codeStartAddress)
		theSegment->codeStartAddress = address;
	if (address > codeRegions[(int)currentCodeMode]->regionEndAddress)
		codeRegions[(int)currentCodeMode]->regionEndAddress = address;
	if (address < codeRegions[(int)currentCodeMode]->regionStartAddress)
		codeRegions[(int)currentCodeMode]->regionStartAddress =
			address;
	(*theBuffer)[bufferPos] = byteValue;
}


/* mapByte is like 'putByte', but places its values in the struct assembly
   buffer */

  void
mapByte(address, byteValue)
  int		address;
  byte		byteValue;
{
	if (address < MAXIMUM_ALLOWED_STRUCT_SIZE)
		structScratchBuffer[address] = byteValue;
}


/* emitByte outputs one byte at the current location in either the current
   code buffer or the current struct assembly buffer */

  void
emitByte(byteValue)
  byte	byteValue;
{
	if (debug || emitPrint)
		if (structNestingDepth == 0)
			printf("emitByte(%x: %x)\n", currentLocationCounter.
							value, byteValue);
		else
			printf("emitByte in struct (%x: %x)\n",
					currentFieldOffset, byteValue);
	if (structNestingDepth == 0) {
		putByte(currentLocationCounter.value++, byteValue);
	} else {
		mapByte(currentFieldOffset++, byteValue);
	}
}


/* emitWord similarly emits a word */

  void
emitWord(wordValue)
  wordType	wordValue;
{
	byteToWordType	convert;
	int		loByte, hiByte;

/* We hack around with this, even though it's less portable, so that we can
   avoid doing a division and a modulo on every word we emit (since code
   emission is in the inner loop). */

#ifdef BYTESWAPPED
	loByte = 1;
	hiByte = 0;
#else
	loByte = 0;
	hiByte = 1;
#endif
	convert.wordPart = wordValue;
	if (debug || emitPrint)
		if (structNestingDepth == 0)
			printf("emitWord(%x: %x)\n", currentLocationCounter.
							value, wordValue);
		else
			printf("emitWord in struct (%x: %x)\n",
					currentFieldOffset, wordValue);
	if (structNestingDepth == 0) {
#if TARGET_CPU == CPU_6502
		putByte(currentLocationCounter.value++,
						convert.bytePart[loByte]);
		putByte(currentLocationCounter.value++,
						convert.bytePart[hiByte]);
#else if TARGET_CPU == CPU_68000
		putByte(currentLocationCounter.value++,
						convert.bytePart[hiByte]);
		putByte(currentLocationCounter.value++,
						convert.bytePart[loByte]);
#endif
	} else {
#if TARGET_CPU == CPU_6502
		mapByte(currentFieldOffset++, convert.bytePart[loByte]);
		mapByte(currentFieldOffset++, convert.bytePart[hiByte]);
#else if TARGET_CPU == CPU_68000
		mapByte(currentFieldOffset++, convert.bytePart[hiByte]);
		mapByte(currentFieldOffset++, convert.bytePart[loByte]);
#endif
	}
}

/* emitLong similarly emits a long */

  void
emitLong(longValue)
  longType	longValue;
{
	byteToLongType	convert;
	int		loByte, secondByte, thirdByte, hiByte;

/* We hack around with this, even though it's less portable, so that we can
   avoid doing a division and a modulo on every long we emit (since code
   emission is in the inner loop). */

#ifdef BYTESWAPPED
	/* Sun workstation... */
	loByte = 3;
	secondByte = 2;
	thirdByte = 1;
	hiByte = 0;
#else
	/* Vax... */
	loByte = 0;
	secondByte = 1;
	thirdByte = 2;
	hiByte = 3;
#endif
	convert.longPart = longValue;
	if (debug || emitPrint)
		if (structNestingDepth == 0)
			printf("emitLong(%x: %x)\n", currentLocationCounter.
							value, longValue);
		else
			printf("emitLong in struct (%x: %x)\n",
					currentFieldOffset, longValue);
	if (structNestingDepth == 0) {
#if TARGET_CPU == CPU_6502
		putByte(currentLocationCounter.value++,
						convert.bytePart[loByte]);
		putByte(currentLocationCounter.value++,
						convert.bytePart[secondByte]);
		putByte(currentLocationCounter.value++,
						convert.bytePart[thirdByte]);
		putByte(currentLocationCounter.value++,
						convert.bytePart[hiByte]);
#else if TARGET_CPU == CPU_68000
		putByte(currentLocationCounter.value++,
						convert.bytePart[hiByte]);
		putByte(currentLocationCounter.value++,
						convert.bytePart[thirdByte]);
		putByte(currentLocationCounter.value++,
						convert.bytePart[secondByte]);
		putByte(currentLocationCounter.value++,
						convert.bytePart[loByte]);
#endif
	} else {
#if TARGET_CPU == CPU_6502
		mapByte(currentFieldOffset++, convert.bytePart[loByte]);
		mapByte(currentFieldOffset++, convert.bytePart[secondByte]);
		mapByte(currentFieldOffset++, convert.bytePart[thirdByte]);
		mapByte(currentFieldOffset++, convert.bytePart[hiByte]);
#else if TARGET_CPU == CPU_68000
		mapByte(currentFieldOffset++, convert.bytePart[hiByte]);
		mapByte(currentFieldOffset++, convert.bytePart[thirdByte]);
		mapByte(currentFieldOffset++, convert.bytePart[secondByte]);
		mapByte(currentFieldOffset++, convert.bytePart[loByte]);
#endif
	}
}


/* emitByteValue takes the byte to be emitted out of a 'valueType' */

  void
emitByteValue(byteValue)
  valueType	*byteValue;
{
    if (byteValue->kindOfValue == ABSOLUTE_VALUE || byteValue->kindOfValue ==
	    RELOCATABLE_VALUE || byteValue->kindOfValue == UNDEFINED_VALUE) {
	if (byteCheck(byteValue->value) && byteValue->kindOfValue!=FAIL) {
	    if (debug || emitPrint) {
		if (structNestingDepth == 0)
		    printf("emitByteValue(%x: ",currentLocationCounter.value);
		else
		    printf("emitByteValue in struct (%x:",currentFieldOffset);
		printValue(byteValue);
		printf(")\n");
	    }
	    emitByte(byteValue->value);
	}
    } else {
	if (byteValue->kindOfValue != FAIL)
	    error(NON_ADDRESS_BYTE_VALUE_ERROR, valueKindString(byteValue->
	    	kindOfValue));
    }
}


/* emitString similarly spits out a string of bytes */

  void
emitString(string)
  stringType	*string;
{
	if (debug || emitPrint)
		if (structNestingDepth == 0)
			printf("emitString(%x: \"%s\")\n",
				currentLocationCounter.value, string);
		else
			printf("emitString in struct(%x: \"%s\")\n",
				currentFieldOffset, string);
/* Horrible terrible no good very bad cretinous ugly hack, but no graceful
   way to avoid it:  a 0xFF byte in the string means output a 0x00 byte in
   the object.  This is so we can embed nuls in strings without embedding
   nuls in strings, so to speak.  We assume that the character 0xFF is not
   likely to be needed since ASCII (and ATASCII) is a seven bit character
   code. */
	while (*string != NULL)
		if ((*string & 0xFF) == 0xFF) {
			emitByte('\0');
			string++;
		} else {
			emitByte(*string++);
		}
}


/* emitWordValue emits a word out of a 'valueType' */

  void
emitWordValue(wordValue)
  valueType	*wordValue;
{
    if (wordValue->kindOfValue == ABSOLUTE_VALUE || wordValue->kindOfValue ==
	    RELOCATABLE_VALUE || wordValue->kindOfValue == UNDEFINED_VALUE) {
	if (wordCheck(wordValue->value) && wordValue->kindOfValue!=FAIL) {
	    if (debug || emitPrint) {
		if (structNestingDepth == 0)
		    printf("emitWordValue(%x: ",currentLocationCounter.value);
		else
		    printf("emitWordValue in struct (%x:",currentFieldOffset);
		printValue(wordValue);
		printf(")\n");
	    }
	emitWord(wordValue->value);
	}
    } else {
	if (wordValue->kindOfValue != FAIL)
	    error(NON_ADDRESS_WORD_VALUE_ERROR, valueKindString(wordValue->
		kindOfValue));
    }
}


/* emitLongValue emits a long out of a 'valueType' */

  void
emitLongValue(longValue)
  valueType	*longValue;
{
    if (longValue->kindOfValue == ABSOLUTE_VALUE || longValue->kindOfValue ==
	    RELOCATABLE_VALUE || longValue->kindOfValue == UNDEFINED_VALUE) {
	if (longValue->kindOfValue != FAIL) {
	    if (debug || emitPrint) {
		if (structNestingDepth == 0)
		    printf("emitLongValue(%x: ",currentLocationCounter.value);
		else
		    printf("emitLongValue in struct (%x:",currentFieldOffset);
		printValue(longValue);
		printf(")\n");
	    }
	    emitLong(longValue->value);
	}
    } else {
	if (longValue->kindOfValue != FAIL)
	    error(NON_ADDRESS_LONG_VALUE_ERROR, valueKindString(longValue->
		kindOfValue));
    }
}


/* pokeByteValue is like 'emitByte' but it's random access */

  void
pokeByteValue(location, value)
  addressType	 location;
  valueType	*value;
{
	currentLocationCounter.value = location;
	emitByteValue(value);
}


/* ditto pokeWordValue */

  void
pokeWordValue(location, value)
  addressType	 location;
  valueType	*value;
{
	currentLocationCounter.value = location;
	emitWordValue(value);
}


/* ditto pokeLongValue */

  void
pokeLongValue(location, value)
  addressType	 location;
  valueType	*value;
{
	currentLocationCounter.value = location;
	emitLongValue(value);
}


/* ditto pokeRelativeByteValue.  This is a special case used in fixing up
   relative branches */

  void
pokeRelativeByteValue(location, value)
  addressType	 location;
  valueType	*value;
{
	int	offset;

	currentLocationCounter.value = location;
	offset = value->value - (location - targetOffset) - 1;
	if (offset < 0)
		offset--;
/*	if (currentCodeMode == RELOCATABLE_BUFFER)
		offset = 0;*/
	if (isByteOffset(offset)) {
		emitByte(offset);
	} else {
		error(RELATIVE_OFFSET_TOO_LARGE_ERROR);
	}
}

/* ditto pokeRelativeWordValue.  This is a special case used in fixing up
   relative branches */

  void
pokeRelativeWordValue(location, value)
  addressType	 location;
  valueType	*value;
{
	int	offset;

	currentLocationCounter.value = location;
	offset = value->value - (location - targetOffset);
	if (isWordOffset(offset)) {
		emitWord(offset);
	} else {
		error(RELATIVE_OFFSET_TOO_LARGE_ERROR);
	}
}


/* getByte fetches a byte back out of the labyrinth of code buffers */

  byte
getByte(address)
  addressType	address;
{
	int		 bufferNum;
	int		 bufferPos;
	codeBufferType	*theBuffer;
	codeSegmentType	*theSegment;

	bufferNum = bufferNumber(address);
	bufferPos = bufferPosition(address);
	theSegment = codeRegions[(int)currentCodeMode]->codeSegments[
		bufferNum];
	if (theSegment == NULL)
		return(0);
	else
		return((*(theSegment->codeBuffer))[bufferPos]);
}

  void
emitRelativeByteOffset(target)
  valueType	*target;
{
	int	saveTargetOffset;

	if (target == NULL) {
		emitByte(0);
	} else {
		(target->value)++;
		saveTargetOffset = targetOffset;
		targetOffset = 0;
		pokeRelativeByteValue(currentLocationCounter.value, target);
		targetOffset = saveTargetOffset;
		(target->value)--;
	}
}

  void
emitRelativeWordOffset(target)
  valueType	*target;
{
	int	saveTargetOffset;

	if (target == NULL) {
		emitWord(0);
	} else {
		saveTargetOffset = targetOffset;
		targetOffset = 0;
		pokeRelativeWordValue(currentLocationCounter.value, target);
		targetOffset = saveTargetOffset;
	}
}

/* fixupBranch repairs a previously undefined branch once the branch address
   has become known. */

  void
fixupBranch(location, target)
  valueType	 location[COMPOUND_BRANCH_MAX];
  valueType	 target;
{
	valueType	saveCurrentLocation;
	int		saveTargetOffset;
	int		i;

	saveCurrentLocation = currentLocationCounter;
	saveTargetOffset = targetOffset;
	targetOffset = 0;
	for (i=0; i<COMPOUND_BRANCH_MAX; i++) {
		if (location[i].value >= 0)
			pokeRelativeByteValue(location[i].value, &target);
	}
	targetOffset = saveTargetOffset;
	currentLocationCounter = saveCurrentLocation;
}


/* fixupJump similarly repairs a jump */

  void
fixupJump(locations, target)
  simpleFixupListType	*locations;
  valueType		 target;
{
	valueType		 saveCurrentLocation;
	simpleFixupListType	*oldLocation;

	saveCurrentLocation = currentLocationCounter;
	while (locations != NULL) {
		currentLocationCounter = locations->locationToFixup;
		noteAnonymousReference();
		target.value -= targetOffset;
		if (positionIndependentCodeMode)
			pokeRelativeByteValue(locations->locationToFixup.
				value, &target);
		else
			pokeWordValue(locations->locationToFixup.value,
				&target);
		target.value += targetOffset;
		oldLocation = locations;
		locations = locations->nextFixup;
		free(oldLocation);
	}
	currentLocationCounter = saveCurrentLocation;
}
