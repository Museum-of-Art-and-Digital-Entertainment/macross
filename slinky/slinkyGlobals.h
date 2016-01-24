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
	slinkyGlobals.h -- Global variables for the Slinky linker.

	Chip Morningstar -- Lucasfilm Ltd.

	9-March-1985
*/

extern bool	 debug;		/* TRUE iff we should print debug diagnostics */
extern bool	 errorFlag;	/* TRUE iff an error occured during linking */
extern bool	 verbose;
extern bool	 packFlag;
#define	DEFAULT_LOAD_FILE_NAME	"s.out"
extern FILE			*loadFileOutput;	/* where to put the results */
extern FILE			*mapFileOutput;
extern objectFileListType	*objectFileList;
extern objectFileListType	*endOfObjectFileList;
extern char			*currentFileName;
extern freeSegmentEntryType	*freeSegmentList;
extern freeSegmentEntryType	*effectiveFreeSegmentList;
extern addressType		 relocationOffset;
extern addressType		 entryPointAddress;
extern int			 entryPointMode;
extern expressionPCType	 entryPointExpression;
extern bool			 produceLoadMap;
extern bool			 leaveOffLoadFiles;
extern bool			 haveEntryPoint;
extern bool			 haveExpressionEntryPoint;
extern bool			 readExpressionEntryPoint;
extern symbolType	       **globalSymbolTable;
extern int			 globalSymbolCount;
extern symbolType	       **currentSymbolTable;
extern functionType	        *currentFunctionTable;
extern reservationListType	*reservationList;
extern int			 totalSymbolCount;
extern expressionPCType	 pc;
extern addressType		 here;
extern bindingListType		*localBindings;

#define CODE_REGIONS_IN_ADDRESS_SPACE	256
#define CODE_REGION_SIZE		0x100
extern segmentListType		*generatedLoadImage[CODE_REGIONS_IN_ADDRESS_SPACE];
#define	regionOf(addr)	(addr / CODE_REGION_SIZE)

extern struct {
	stringType	*functionName;
	addressType	 (*functionEntry)();
		} builtInFunctionTable[];
#define MAX_FUNCTIONS 15
