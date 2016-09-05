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
	macrossGlobals.h -- Global variable definitions for the Macross
			    assembler.

	Chip Morningstar -- Lucasfilm Ltd.

	1-November-1984

*/

extern bool			 backwardsCompatibleIfFlag;
extern bool			 beneathFunction;
extern commandLineDefineType	*commandLineDefines;
extern int			 cumulativeLineNumber;
extern environmentType		*currentEnvironment;
extern int			 currentFieldOffset;
extern char			*currentFileName;
extern char			*lastErrorFile;
extern stringType		*currentFunctionName;
extern int			 currentLabelTagNumber;
extern int			 currentLineNumber;
extern int			 lastErrorLine;
extern identifierListType	*currentLocalVariableList;
extern valueType		 currentLocationCounter;
extern int			 currentOperandNumber;
extern int			 currentReferenceDepth;
extern bool			 debug;
extern bool			 emitPrint;
extern bool			 expandMacros;
extern bool			 errorFlag;
extern bool			 expressionFailed;
extern bool			 finishOperand;
extern operandKindType		 fixupAddressMode[MAX_NUMBER_OF_OPERANDS];
extern operandKindType		 newFixupAddressMode;
extern fixupListType		*fixupList;
extern bool			 freeFlag;
extern bool			 freturnExit;
extern bool			 generatingFixup;
extern environmentType		 globalEnvironment;
extern int			 hackFlag;
extern bool			 haveUserStartAddress;
extern bool			 fixupStartAddress;
extern int			 includeNestingDepth;
extern FILE			*indexFileForPass2;
extern FILE			*input;
extern fileNameListType	*inputFileStack;
extern FILE			*listFileOutput;
extern int			 listingControlCounter;
extern bool			 listingOn;
extern int			 macroCallDepth;
extern FILE			*macroFileForPass2;
extern int			 macroOrFunctionNestingDepth;
extern structInstanceType	*newStruct;
extern int			 nextEnvironmentNumber;
extern int			 nextLabelTagNumber;
extern FILE			*objectFileOutput;
extern char			 operandBuffer[MAX_NUMBER_OF_OPERANDS][LINE_BUFFER_SIZE];
extern char			 pass2IndexFileName[];
extern char			 pass2SourceFileName[];
extern char			 pass2MacroExpansionFileName[];
extern expressionType		*pendingFixup[MAX_NUMBER_OF_OPERANDS];
extern bool			 performingFixups;
extern bool			 positionIndependentCodeMode;
extern bool			 produceLinkableObject;
extern addressType		 relocatableHighWaterMark;
extern reservationListType	*reservationList;
extern valueType		*resultOfLastFunctionCall;
extern valueType		 savedRelocatableCurrentLocationCounter;
extern FILE			*saveFileForPass2;
extern bool			 showAllSymbolsFlag;
extern bool			 sideEffectFlag;
extern bool			 standaloneExpansionFlag;
extern valueType		*startAddress;
extern int			 statementEvaluationDepth;
extern int			 statementListNestingDepth;
extern int			 structNestingDepth;
extern FILE			*symbolDumpFileOutput;
extern bool			 symbolTableDumpOn;
extern int			 tabCount;
extern addressType		 targetOffset;
extern bool			 terseErrorMessages;
extern valueType		*UndefinedValue;
extern symbolUsageKindType	 unknownSymbolTag;
extern int			 processor;

#define DEFAULT_OBJECT_FILE_NAME	"m.out"

#define	LEX_DISPATCH_TABLE_SIZE	128
extern int			(*lexDispatchTable[128])();

#define HASH_TABLE_SIZE	512
#define HASH_TABLE_MASK 0x1FF

extern struct {
	stringType	*functionName;
	valueType	*(*functionEntry)();
	bool		 isSpecialFunction;
	int		 ordinal;
		} builtInFunctionTable[];

extern struct {
	stringType	*symbolName;
	int		 symbolValue;
		} predefinedSymbolTable[];

extern macroTableEntryType	*macroTable[HASH_TABLE_SIZE];

extern opcodeTableEntryType	*opcodeTable[HASH_TABLE_SIZE];

extern opcodeTableEntryType	 theOpcodes[];

extern keywordTableEntryType	*keywordTable[HASH_TABLE_SIZE];

extern keywordTableEntryType	 theKeywords[];

extern conditionTableEntryType	*conditionTable[HASH_TABLE_SIZE];

extern conditionTableEntryType	 theConditions[];

extern symbolTableEntryType	*symbolTable[HASH_TABLE_SIZE];

extern int		 operandClassTable[];

extern void	       (*instructionActionTable[])();

extern int			 validSymbolValues[NUM_OF_SYM_USAGES];

extern byte			 structScratchBuffer[MAXIMUM_ALLOWED_STRUCT_SIZE];

extern codeRegionType		 absoluteCodeRegion;
extern codeRegionType		 relocatableCodeRegion;
extern codeRegionType		*codeRegions[2];
extern codeBufferKindType	 currentCodeMode;
extern codeBufferType		*emptyBuffer; /* ??? */
extern codeBreakType		*codeBreakList;
extern codeBreakType		*lastCodeBreak;

extern expressionReferenceListType	*expressionReferenceList[3];
extern expressionReferenceListType	*referencesToNote[MAX_NUMBER_OF_OPERANDS];
extern int			 numberOfReferencesInList[3];
extern functionDefinitionType		*externalFunctionList;
extern functionDefinitionType		*endOfExternalFunctionList;
extern int				 externalFunctionCount;

extern char			 alphabeticCharacterTable[128];
extern char			 alphaNumericCharacterTable[128];
extern char			 lowerCaseCharacterTable[128];
extern char			 numericCharacterTable[128];

extern int		expressionBufferSize;
#define EXPRESSION_BUFFER_LIMIT	500
extern byte		expressionBuffer[EXPRESSION_BUFFER_LIMIT];
