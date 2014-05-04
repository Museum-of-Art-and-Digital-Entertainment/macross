/*
	write.c -- Routines to write out the eventual object module for the
		Slinky linker

	Chip Morningstar -- Lucasfilm Ltd.

	19-March-1985
*/

#include "slinkyTypes.h"
#include "slinkyGlobals.h"

#define writeWord(aWord) putc(aWord & 0xFF, loadFileOutput);\
			putc((aWord >> 8) & 0xFF, loadFileOutput)
#define writeByte(aByte) putc(aByte & 0xFF, loadFileOutput)

  void
writeEntryPoint()
{
	writeWord(entryPointAddress);
	writeWord(entryPointAddress);
	writeByte(0);
}

  void
writeCodeSegment(codeSegment)
  codeSegmentHeaderType	*codeSegment;
{
	int	length;
	int	i;

	writeWord(codeSegment->segmentStartAddress);
	writeWord(codeSegment->segmentEndAddress);
	length = codeSegment->segmentEndAddress - codeSegment->
		segmentStartAddress + 1;
	for (i=0; i<length; i++)
		writeByte(codeSegment->segmentCodeBuffer[i]);
}

  void
writem()
{
	int		 	 regionNumber;
	codeSegmentHeaderType	*lastSegment;
	segmentListType		*segment;

	writeWord(0xFFFF);
	if (haveEntryPoint)
		writeEntryPoint();
	lastSegment = NULL;
	for (regionNumber = 0; regionNumber < CODE_REGIONS_IN_ADDRESS_SPACE;
			regionNumber++) {
		for (segment = generatedLoadImage[regionNumber]; segment !=
				NULL; segment = segment->nextSegment) {
			if (segment->thisSegment != lastSegment)
				writeCodeSegment(lastSegment = segment->
					thisSegment);
		}
	}
}
