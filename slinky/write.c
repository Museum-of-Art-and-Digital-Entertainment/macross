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
	write.c -- Routines to write out the eventual object module for the
		Slinky linker

	Chip Morningstar -- Lucasfilm Ltd.

	19-March-1985
*/

#include "slinkyTypes.h"
#include "slinkyGlobals.h"
#include "write.h"

#define writeWord(aWord) putc(aWord & 0xFF, loadFileOutput);\
			putc((aWord >> 8) & 0xFF, loadFileOutput)
#define writeByte(aByte) putc(aByte & 0xFF, loadFileOutput)

  void
writeEntryPoint(void)
{
	writeWord(entryPointAddress);
	writeWord(entryPointAddress);
	writeByte(0);
}

  void
writeCodeSegment(codeSegmentHeaderType *codeSegment)
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
writem(void)
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
