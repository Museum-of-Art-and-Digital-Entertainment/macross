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
	initialize.c -- Program initialization for the Slinky linker.

	Chip Morningstar -- Lucasfilm Ltd.

	9-March-1985
*/

#include "slinkyTypes.h"
#include "slinkyGlobals.h"
#include "initialize.h"
#include "errorStuff.h"
#include "main.h"

#include <string.h>
#include <unistd.h>

static char	*outputFileName;

  void
chokePukeAndDie()
{
	unlink(outputFileName);
	exit(1);
}

  void
initializeStuff(argc, argv)
  int	 argc;
  char	*argv[];
{
	int	 i;
	int	 j;
	char   **args;
	char	*arg;
	int	 outputFilesFound;
	int	 mapFilesFound;
	char	*mapFileName;

	currentFileName = "<command line>";
	errorFlag = FALSE;
	packFlag = FALSE;
	debug = FALSE;
	verbose = FALSE;
	objectFileList = endOfObjectFileList = NULL;
	reservationList = NULL;
	haveEntryPoint = FALSE;
	readExpressionEntryPoint = FALSE;
	haveExpressionEntryPoint = FALSE;
	globalSymbolCount = 0;
	produceLoadMap = FALSE;
	leaveOffLoadFiles = FALSE;

	for (i=0; i<CODE_REGIONS_IN_ADDRESS_SPACE; i++)
		generatedLoadImage[i] = NULL;

	effectiveFreeSegmentList = freeSegmentList =
		typeAlloc(freeSegmentEntryType);
	freeSegmentList->segmentStartAddress = FIRST_AVAILABLE_LOCATION;
	freeSegmentList->segmentEndAddress = LAST_AVAILABLE_LOCATION;
	freeSegmentList->nextFreeSegment = NULL;

	outputFilesFound = 0;
	outputFileName = DEFAULT_LOAD_FILE_NAME;
	mapFilesFound = 0;
	mapFileName = NULL;

	args = argv + 1;
	for (i=1; i<argc; i++) {
		arg = *args++;
		if (*arg != '-') {
			queueInputFile(arg);
			continue;
		}
		for (j=1; arg[j]!='\0'; j++) switch (arg[j]) {

		case 'd':
			debug = TRUE;
			continue;

		case 'e':
			verbose = TRUE;
			continue;

		case 'l':
			if (++i >= argc) {
				error(NO_LOAD_ADDRESS_ERROR);
				chokePukeAndDie();
			} else {
				queueLoadAddress(*args++);
			}
			continue;

		case 'm':
			if (++i >= argc) {
				error(NO_MAP_FILE_NAME_ERROR);
				chokePukeAndDie();
			} else {
				mapFileName = *args++;
				mapFilesFound++;
			}
			produceLoadMap = TRUE;
			continue;

		case 'n':
			leaveOffLoadFiles = TRUE;
			continue;

		case 'o':
			if (++i >= argc) {
				error(NO_DASH_O_FILE_NAME_ERROR);
				chokePukeAndDie();
			} else {
				outputFileName = *args++;
				outputFilesFound++;
			}
			continue;

		case 'p':
			packFlag = TRUE;
			continue;

		case 'v':
			printVersion();
			continue;

		default:
			error(BAD_COMMAND_LINE_FLAG_ERROR, arg[j]);
			continue;
		}
	}

	if (outputFilesFound > 1) {
		error(MORE_THAN_ONE_OUTPUT_FILE_ERROR);
		chokePukeAndDie();
	} else if ((loadFileOutput = fopen(outputFileName, "w"))==NULL) {
		error(CANT_OPEN_LOAD_FILE_ERROR, outputFileName);
		perror("Unix says");
		chokePukeAndDie();
	}

	if (mapFilesFound > 1) {
		error(MORE_THAN_ONE_MAP_FILE_ERROR);
		chokePukeAndDie();
	} else if (mapFilesFound == 1) {
		if (strcmp(mapFileName, "-") == 0) {
			mapFileOutput = stdout;
		} else if ((mapFileOutput = fopen(mapFileName, "w")) == NULL){
			error(CANT_OPEN_MAP_FILE_ERROR, mapFileName);
			perror("Unix says");
			chokePukeAndDie();
		}
	}
	currentFileName = outputFileName;
}


  void
queueInputFile(name)
  char	*name;
{
	objectFileListType	*newObjectFile;

	newObjectFile = typeAlloc(objectFileListType);
	newObjectFile->name = name;
	newObjectFile->symbolTable = NULL;
	newObjectFile->codeSegments = NULL;
	newObjectFile->lastCodeSegment = NULL;
	newObjectFile->nextObjectFile = NULL;
	if (objectFileList == NULL) {
		objectFileList = endOfObjectFileList = newObjectFile;
	} else {
		endOfObjectFileList->nextObjectFile = newObjectFile;
		endOfObjectFileList = newObjectFile;
	}
}

  void
queueLoadAddress(addressString)
  char	*addressString;
{
	int			 loadAddress;
	objectFileListType	*newObjectFile;

	if (sscanf(addressString, "0x%x", &loadAddress) != 1) {
	if (sscanf(addressString, "0X%x", &loadAddress) != 1) {
	if (sscanf(addressString,   "%x", &loadAddress) != 1) {
		error(BAD_LOAD_ADDRESS_ERROR, addressString);
		chokePukeAndDie();
	}}}
	newObjectFile = typeAlloc(objectFileListType);
	newObjectFile->name = NULL;/* NULL name encodes load address change */
	newObjectFile->symbolCount = loadAddress; /* then address goes here */
	newObjectFile->nextObjectFile = NULL;
	if (objectFileList == NULL) {
		objectFileList = endOfObjectFileList = newObjectFile;
	} else {
		endOfObjectFileList->nextObjectFile = newObjectFile;
		endOfObjectFileList = newObjectFile;
	}
}
