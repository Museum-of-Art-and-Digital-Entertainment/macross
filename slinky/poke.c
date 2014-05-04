/*
	poke.c -- Routines to poke the values of symbolic references into the
		generated code for the Slinky linker.

	Chip Morningstar -- Lucasfilm Ltd.

	18-March-1985

*/

#include "slinkyTypes.h"
#include "slinkyGlobals.h"

  bool
isWordSized(value)
  int	value;
{
	return (-32768<=value && value<=65535);
}

  bool
isByteSized(value)
  int	value;
{
	return (-128<=value && value<=255);
}

  bool
isByteOffset(value)
  int	value;
{
	return (-128<=value && value<=127);
}

  int
computeRelativeValue(valueToPoke, codeSegment, offset)
  int			 valueToPoke;
  codeSegmentHeaderType	*codeSegment;
  int			 offset;
{
	int	fromLocation;
	int	result;

	fromLocation = codeSegment->segmentStartAddress + offset;
	result = valueToPoke - fromLocation;
/*	if (result > 0)*/ if (result != 0)
		result = result - 1;
	else if (result < 0)
		result = result + 1;
	if (!isByteOffset(result)) {
		error(RELATIVE_OFFSET_TOO_LARGE_ERROR, result, offset,
			fromLocation);
		result = 0;
	}
	return(result);
}

  int
getBaseValue(codeBuffer, offset, referenceKind)
  byte	codeBuffer[];
  int	offset;
  int	referenceKind;
{
	int	result;

	switch (referenceKind) {
	case REF_BYTE:
		result = codeBuffer[offset];
		break;

	case REF_DBYTE:
		result = (codeBuffer[offset] << 8) | codeBuffer[offset+1];
		break;

	case REF_WORD:
		result = codeBuffer[offset] | (codeBuffer[offset+1] << 8);
		break;
	}
	return(result);
}

  void
pokeValue(value, codeBuffer, offset, referenceKind, trueAddress)
  int	value;
  byte	codeBuffer[];
  int	offset;
  int	referenceKind;
  int	trueAddress;
{
	switch (referenceKind) {
	case REF_BYTE:
		if (!isByteSized(value)) {
			error(BYTE_VALUE_TOO_LARGE_ERROR, value, offset,
				trueAddress);
		} else {
			codeBuffer[offset] = value;
		}
		if (debug)
			printf("    Byte:   0x%02x at (0x%04x) 0x%04x\n\n",
				value&0xFF, offset, trueAddress);
		break;

	case REF_WORD:
		if (!isWordSized(value)) {
			error(WORD_VALUE_TOO_LARGE_ERROR, value, offset,
				trueAddress);
		} else {
			codeBuffer[offset] = value & 0xFF;
			codeBuffer[offset+1] = (value >> 8) & 0xFF;
		}
		if (debug)
			printf("    Word: 0x%04x at (0x%04x) 0x%04x\n\n",
				value, offset, trueAddress);
		break;

	case REF_DBYTE:
		if (!isWordSized(value)) {
			error(WORD_VALUE_TOO_LARGE_ERROR, value, offset,
				trueAddress);
		} else {
			codeBuffer[offset] = (value >> 8) & 0xFF;
			codeBuffer[offset+1] = 0xFF;
		}
		if (debug)
			printf("    Dbyt: 0x%04x at (0x%04x) 0x%04x\n\n",
				value, offset, trueAddress);
		break;
	}
}

  void
fixupReference(reference, codeSegment)
  expressionReferenceType	*reference;
  codeSegmentHeaderType		*codeSegment;
{
	int		 offset;
	addressType	 baseValue;
	int		 valueToPoke;

	/* at this point, we assume we are already pointing at the
	correct codeSegment! */
	if (debug)
		printReferenceFixup(reference);
	offset = reference->referenceAddress - codeSegment->
		segmentStartAddress + codeSegment->relocationOffset;
	relocationOffset = codeSegment->relocationOffset;
	baseValue = getBaseValue(codeSegment->segmentCodeBuffer, offset,
		reference->referenceKind);
	
	pc = reference->referenceExpression.inCore;
	valueToPoke = evaluateExpression();
	if (reference->referenceRelative)
		valueToPoke = computeRelativeValue(valueToPoke, codeSegment,
			offset);
	else
		valueToPoke = (short)baseValue + (short)valueToPoke;
	pokeValue(valueToPoke, codeSegment->segmentCodeBuffer, offset,
		reference->referenceKind, codeSegment->segmentStartAddress +
		offset);

}

  void
pokem()
{
	objectFileListType	*inputFileList;
	codeSegmentHeaderType	*codeSegment;
	expressionReferenceType	*reference;
	int			 referenceCount;

	if (debug)
	    printf("\nFixups:\n");

	if (haveExpressionEntryPoint) {
		pc = entryPointExpression;
		entryPointAddress = evaluateExpression();
	}

	for (inputFileList = objectFileList; inputFileList != NULL;
		inputFileList = inputFileList->nextObjectFile) {
	    if (inputFileList->name != NULL) {
		currentFileName = inputFileList->name;
	        for (codeSegment = inputFileList->codeSegments; codeSegment!=
		        NULL; codeSegment = codeSegment->nextSegment) {
		    for (reference=codeSegment->segmentReferences,
			    referenceCount=codeSegment->referenceCount;
			    referenceCount >0; reference++, referenceCount--){
		        fixupReference(reference, codeSegment);
		    }
		}
	    }
	}
}
