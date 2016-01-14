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
	slinkyTypes.h -- Type definitions for the Slinky relocating linker.

	Chip Morningstar -- Lucasfilm Ltd.

	8-March-1985
*/

#include <stdio.h>

typedef	unsigned char	byte;

typedef unsigned short	wordType;

typedef unsigned int   *ptr;

typedef byte		anyOldThing;

#define TRUE		1
#define FALSE		0
typedef int		bool;

typedef unsigned long	bigWord;

typedef char		stringType;	/* a char is not a string but a
					   pointer to char is a pointer to a
					   string, and pointers are what we
					   care about when handling strings */

typedef	bigWord		addressType;	/* target machine address */

/* symbolClass bits */
#define	SYMBOL_UNDEFINED	0x00
#define SYMBOL_ABSOLUTE		0x02
#define SYMBOL_RELOCATABLE	0x04
#define SYMBOL_LOCAL		0x08
#define SYMBOL_EXTERNAL		0x01

typedef struct {
	byte			 symbolClass; /* see above */
	int			 symbolValue;
	stringType		*symbolName;
		       } symbolType; /* as managed in core */

typedef byte		*expressionPCType;

typedef struct bindingListStruct {
	symbolType		 *boundSymbol;
	byte			  previousClass;
	int			  previousValue;
	struct bindingListStruct *nextBinding;
			} bindingListType;

typedef struct argumentListStruct {
	symbolType		  *argumentSymbol;
	struct argumentListStruct *nextArgument;
			} argumentListType;

typedef struct {
	expressionPCType	 functionBody;
	argumentListType	*functionArguments;
			} functionType;

#define MODE_ABSOLUTE		0
#define MODE_RELOCATABLE	1

#define REF_BYTE		0
#define REF_WORD		1
#define REF_DBYTE		2

typedef struct {
	bigWord		 referenceAddress : 24, /* target machine address */
			 referenceMode     : 1, /* see above MODE_xx defines*/
			 referenceRelative : 1, /* TRUE==is relative */
			 referenceKind     : 3, /* see above REF_xx defines */
			 referenceExternal : 1, /* TRUE==is ref to external */
			 : 2; /* unused bits */
	union {
		expressionPCType inCore;
		int		 inFile;
		}	 referenceExpression;
		       } expressionReferenceType;

typedef struct codeSegmentHeaderStruct {
	struct codeSegmentHeaderStruct	*nextSegment;
	int				 relocationOffset;
	addressType			 constraint;
	addressType			 alignment;
	addressType			 segmentStartAddress;
	addressType			 segmentEndAddress;
	byte				 segmentMode;
	byte				*segmentCodeBuffer;
	int				 referenceCount;
	expressionReferenceType	        *segmentReferences;
	char				*fileName;
		      } codeSegmentHeaderType;

typedef struct segmentListStruct {
	codeSegmentHeaderType		*thisSegment;
	struct segmentListStruct	*nextSegment;
		      } segmentListType;

#define FIRST_AVAILABLE_LOCATION	0x0000
#define LAST_AVAILABLE_LOCATION		0xFFFF
typedef struct freeSegmentEntryStruct {
	addressType			 segmentStartAddress;
	addressType			 segmentEndAddress;
	struct freeSegmentEntryStruct	*nextFreeSegment;
		      } freeSegmentEntryType;

typedef struct objectFileListStruct {
	char				*name;
	symbolType		       **symbolTable;
	int				 symbolCount;
	int				 baseSymbolCount;
	symbolType		       **undefinedSymbols;
	int				 expressionCount;
	int				 functionCount;
	expressionPCType	        *expressions;
	functionType		        *functions;
	codeSegmentHeaderType		*codeSegments;
	codeSegmentHeaderType		*lastCodeSegment;
	struct objectFileListStruct	*nextObjectFile;
		      } objectFileListType;

typedef struct reservationListStruct {
	addressType			 startAddress;
	int				 blockSize;
	struct reservationListStruct	*nextReservation;
		      } reservationListType;

typedef enum {
	NO_DASH_O_FILE_NAME_ERROR,
	BAD_COMMAND_LINE_FLAG_ERROR,
	MORE_THAN_ONE_OUTPUT_FILE_ERROR,
	CANT_OPEN_LOAD_FILE_ERROR,
	CANT_OPEN_OBJECT_FILE_ERROR,
	NO_INPUT_FILES_ERROR,
	BAD_OBJECT_FILE_ERROR,
	PREMATURE_EOF_ERROR,
	ERROR_READING_OBJECT_FILE,
	OVERLAPPING_ABSOLUTE_CODE_SEGMENTS_ERROR,
	NO_PLACE_TO_PUT_CODE_SEGMENT_ERROR,
	MULTIPLY_DEFINED_SYMBOL_ERROR,
	UNDEFINED_SYMBOL_ERROR,
	RELATIVE_OFFSET_TOO_LARGE_ERROR,
	BYTE_VALUE_TOO_LARGE_ERROR,
	WORD_VALUE_TOO_LARGE_ERROR,
	NO_MAP_FILE_NAME_ERROR,
	MORE_THAN_ONE_MAP_FILE_ERROR,
	CANT_OPEN_MAP_FILE_ERROR,
	BAD_LOAD_ADDRESS_ERROR,
	NO_LOAD_ADDRESS_ERROR,
	ARRAY_TERM_IN_OBJECT_ERROR,
	ASSERT_FAILED_ERROR,
	CONDITION_CODE_EXPRESSION_ENCOUNTERED_ERROR,
	CLAUSE_AT_TOP_LEVEL_ERROR,
	TOO_MANY_FUNCTION_ARGUMENTS_ERROR,
	TOO_FEW_ARGUMENTS_TO_BIF_ERROR,
	TOO_MANY_ARGUMENTS_TO_BIF_ERROR,
	BAD_POSITION_ARGUMENT_TO_NTH_CHAR_ERROR,
	BAD_SUBSTRING_INDICES_ERROR,
	SYMBOL_NAME_ARG_IS_NOT_SYMBOL_ERROR,
	SYMBOL_NOT_FOUND_IN_SYMBOL_LOOKUP_ERROR,
		        } errorType;

#define typeAlloc(type) 		(type *)malloc(sizeof(type))
#define typeAllocBlock(type, size)	(type *)malloc(sizeof(type) * (size))
