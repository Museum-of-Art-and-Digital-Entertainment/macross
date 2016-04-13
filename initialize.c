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
	initialize.c -- Routines to get things going for the Macross assembler

	Chip Morningstar -- Lucasfilm Ltd.

	6-November-1984
*/

#include "macrossTypes.h"
#include "macrossGlobals.h"
#include "initialize.h"
#include "errorStuff.h"
#include "lexer.h"
#include "lookups.h"
#include "semanticMisc.h"

#include <string.h>
#include <unistd.h>

#define isAlphaNumeric(c)	(alphaNumericCharacterTable[c])

extern int yydebug;

static fileNameListType		*bottomOfInputFileStack;
static char			 *outputFileName;

static int machine = 0;

  void
chokePukeAndDie(void)
{
	unlink(outputFileName);
	exit(1);
}

  void
initializeStuff(int argc, char **argv)
{
	int	  i;
	int	  j;
	char	**args;
	char	 *arg;
	int	  outputFilesFound;
	int	  listFilesFound;
	int	  symbolDumpFilesFound;
	char	 *listFileName;
	char	 *symbolDumpFileName;
	bool	  dontUnlinkTempFiles;

	for (i=0; i<128; i++) {
		lowerCaseCharacterTable[i] = i;
		alphabeticCharacterTable[i] = FALSE;
		numericCharacterTable[i] = FALSE;
		alphaNumericCharacterTable[i] = FALSE;
	}
	for (i='A'; i<='Z'; i++) {
		lowerCaseCharacterTable[i] = i - 'A' + 'a';
		alphabeticCharacterTable[i] = TRUE;
		alphabeticCharacterTable[i - 'A' + 'a'] = TRUE;
		alphaNumericCharacterTable[i] = TRUE;
		alphaNumericCharacterTable[i - 'A' + 'a'] = TRUE;
	}
	alphabeticCharacterTable['_'] = TRUE;
	alphaNumericCharacterTable['_'] = TRUE;
	for (i='0'; i<='9'; i++) {
		numericCharacterTable[i] = TRUE;
		alphaNumericCharacterTable[i] = TRUE;
	}

	outputFilesFound = 0;
	listFilesFound = 0;
	symbolDumpFilesFound = 0;
	outputFileName = DEFAULT_OBJECT_FILE_NAME;
	dontUnlinkTempFiles = FALSE;
	produceLinkableObject = FALSE;

	currentFileName = "<command line>";	/* for error messages... */
	currentLineNumber = 1;			/* ...during startup */
	cumulativeLineNumber = 1;

	nextEnvironmentNumber = GLOBAL_ENVIRONMENT_NUMBER;
	currentEnvironment = NULL;
	pushEnvironment(globalEnvironment);
	beneathFunction = FALSE;
	includeNestingDepth = 0;
	macroOrFunctionNestingDepth = 0;
	macroCallDepth = 0;
	tabCount = 1;
	statementEvaluationDepth = 0;
	structNestingDepth = 0;

	performingFixups = FALSE;
	generatingFixup = FALSE;
	finishOperand = FALSE;
	unknownSymbolTag = UNKNOWN_SYMBOL;

	haveUserStartAddress = FALSE;
	fixupStartAddress = FALSE;

	currentLabelTagNumber = 0;
	currentLocalVariableList = NULL;
	inputFileStack = bottomOfInputFileStack = NULL;
	listingControlCounter = 0;
	nextLabelTagNumber = 1;
	commandLineDefines = NULL;

	showAllSymbolsFlag = FALSE;
	backwardsCompatibleIfFlag = FALSE;
	debug = FALSE;
	yydebug = FALSE;
	emitPrint = FALSE;
	freeFlag = TRUE;
	freturnExit = FALSE;
	listingOn = FALSE;
	expandMacros = FALSE;
	standaloneExpansionFlag = FALSE;
	terseErrorMessages = FALSE;
	lastErrorLine = -1;
	lastErrorFile = NULL;
	symbolTableDumpOn = 0;
	positionIndependentCodeMode = FALSE;
	hackFlag = 0;

	args = argv + 1;
	for (i=1; i<argc; i++) {
		arg = *args++;
		if (*arg != '-') {
			if (!isDotMName(arg)) {
				fatalError(FILE_NAME_IS_NOT_MACROSS_SOURCE_ERROR,
					arg);
			}
			queueInputFile(arg);
			continue;
		}
		for (j=1; arg[j]!='\0'; j++) switch (arg[j]) {

		case 'a':
			showAllSymbolsFlag = TRUE;
			continue;

		case 'B':
			backwardsCompatibleIfFlag = TRUE;
			continue;

		case 'c':
			produceLinkableObject = TRUE;
			continue;

		case 'd':
			debug = TRUE;
			continue;

		case 'D':
			if (++i >= argc) {
				fatalError(NO_DASH_D_FILE_NAME_ERROR);
			} else {
				noteCommandLineDefine(*args++);
			}
			continue;

		case 'e':
			emitPrint = TRUE;
			continue;

		case 'g':
			freeFlag = FALSE;
			continue;

		case 'l':
			if (++i >= argc) {
				fatalError(NO_LIST_FILE_NAME_ERROR);
			} else {
				if (isDotMName(*args)) {
					fatalError(LIST_FILE_NAME_IS_MACROSS_SOURCE_ERROR,
						*args);
				}
				listFileName = *args++;
				listFilesFound++;
			}
			listingOn = TRUE;
			continue;

		case 'm':
			expandMacros = TRUE;
			continue;

		case 'o':
			if (++i >= argc) {
				fatalError(NO_DASH_O_FILE_NAME_ERROR);
			} else {
				if (isDotMName(*args)) {
					fatalError(OUTPUT_FILE_NAME_IS_MACROSS_SOURCE_ERROR,
						*args);
				}
				outputFileName = *args++;
				outputFilesFound++;
			}
			continue;

		case 'p':
			positionIndependentCodeMode = TRUE;
			continue;

		case 's':
		case 'S':
		case 'h':
		case 'H':
			if (++i >= argc) {
				fatalError(NO_SYMBOL_DUMP_FILE_NAME_ERROR, arg[j]);
			} else {
				if (isDotMName(*args)) {
					fatalError(SYMBOL_DUMP_FILE_NAME_IS_MACROSS_SOURCE_ERROR,
						*args);
				}
				symbolDumpFileName = *args++;
				symbolDumpFilesFound++;
			}
			if (arg[j] == 'h') {
				hackFlag = 1;
				symbolTableDumpOn = 1;
			} else if (arg[j] == 'H') {
				hackFlag = 2;
				symbolTableDumpOn = 1;
			} else {
				symbolTableDumpOn = arg[j]=='s' ? 1 : 2;
			}
			continue;

		case 't':
			terseErrorMessages = TRUE;
			continue;

		case 'u':
			dontUnlinkTempFiles = TRUE;
			continue;

		case 'v':
			printVersion();
			continue;

		case 'x':
			/* -x6502 -x65c02 -x65c02s */
			machine++;
			continue;

		case 'Y':
			yydebug = TRUE;
			continue;

		default:
			warning(BAD_COMMAND_LINE_FLAG_ERROR, arg[j]);
			continue;
		}
	}

	openFirstInputFile();

	if (outputFilesFound > 1) {
		warning(MORE_THAN_ONE_OUTPUT_FILE_ERROR);
	} else if ((objectFileOutput = fopen(outputFileName, "w"))==NULL) {
		fatalSystemError(CANT_OPEN_OBJECT_FILE_ERROR, outputFileName);
	}

	if (listFilesFound > 1) {
		warning(MORE_THAN_ONE_LIST_FILE_ERROR);
	} else if (listFilesFound == 1) {
		if (strcmp(listFileName, "-") == 0) {
			listFileOutput = stdout;
		} else if ((listFileOutput = fopen(listFileName, "w"))==NULL){
			fatalSystemError(CANT_OPEN_LIST_FILE_ERROR, listFileName);
		}
	}

	if (symbolDumpFilesFound > 1) {
		warning(MORE_THAN_ONE_SYMBOL_DUMP_FILE_ERROR);
	} else if (symbolDumpFilesFound == 1) {
		if (strcmp(symbolDumpFileName, "-") == 0) {
			symbolDumpFileOutput = stdout;
		} else if ((symbolDumpFileOutput = fopen(symbolDumpFileName,
				"w"))==NULL) {
			fatalSystemError(CANT_OPEN_SYMBOL_DUMP_FILE_ERROR, symbolDumpFileName);
		}
	}

	expressionReferenceList[0] = expressionReferenceList[1] = NULL;
	numberOfReferencesInList[0] = numberOfReferencesInList[1] = 0;
	externalFunctionList = endOfExternalFunctionList = NULL;
	externalFunctionCount = 0;

	currentLocationCounter.kindOfValue = RELOCATABLE_VALUE;
	currentLocationCounter.value = 0;
	targetOffset = 0;
	currentCodeMode = RELOCATABLE_BUFFER;
	relocatableHighWaterMark = -1;
	codeBreakList = NULL;
	lastCodeBreak = NULL;
	reservationList = NULL;

	for (i=0; i<HASH_TABLE_SIZE; i++) {
		macroTable[i] = NULL;
		opcodeTable[i] = NULL;
		symbolTable[i] = NULL;
		keywordTable[i] = NULL;
		conditionTable[i] = NULL;
	}

	absoluteCodeRegion.regionStartAddress = relocatableCodeRegion.
		regionStartAddress = 0xFFFF + 1;
	absoluteCodeRegion.regionEndAddress = relocatableCodeRegion.
		regionEndAddress = -1;
	for (i=0; i<CODE_BUFFERS_IN_ADDRESS_SPACE; i++) {
		absoluteCodeRegion.codeSegments[i] = NULL;
		relocatableCodeRegion.codeSegments[i] = NULL;
	}
	emptyBuffer = typeAlloc(codeBufferType);
	for (i=0; i<CODE_BUFFER_SIZE; i++)
		(*emptyBuffer)[i] = 0;

	createHashTables();
	initializeLexDispatchTable();
	installBuiltInFunctions();
	installPredefinedSymbols();
	installCommandLineDefineSymbols();

	if (listingOn) {
		if ((saveFileForPass2 = fdopen(mkstemp(pass2SourceFileName),
				"w+")) == NULL) {
			fatalSystemError(UNABLE_TO_OPEN_PASS_2_FILE_ERROR,
					pass2SourceFileName);
		}
		if (!dontUnlinkTempFiles)
			unlink(pass2SourceFileName); /* will take effect on
							program exit */

		if ((indexFileForPass2 = fdopen(mkstemp(pass2IndexFileName),
				"w+")) == NULL) {
			fatalSystemError(UNABLE_TO_OPEN_PASS_2_FILE_ERROR,
					pass2IndexFileName);
		}
		if (!dontUnlinkTempFiles)
			unlink(pass2IndexFileName); /* will take effect on
							program exit */

		if (expandMacros) {
			if ((macroFileForPass2 = fdopen(mkstemp(
				pass2MacroExpansionFileName),"w+")) == NULL) {
				fatalSystemError(
					UNABLE_TO_OPEN_PASS_2_FILE_ERROR,
					pass2MacroExpansionFileName);
			}
			if (!dontUnlinkTempFiles)
				unlink(pass2MacroExpansionFileName); /* will
					take effect on program exit */
		} else {
			macroFileForPass2 = fopen("/dev/null", "w+");
		}
	} else {
		if (expandMacros) {
			warning(MACRO_EXPANSION_WITHOUT_LISTING_ERROR);
			expandMacros = FALSE;
		}
	}
}

  void
installBuiltInFunctions(void)
{
	int				 i;
	symbolTableEntryType		*newFunction;

	for (i=0; builtInFunctionTable[i].functionName!=NULL; i++) {
		newFunction = lookupOrEnterSymbol(builtInFunctionTable[i].
			functionName, BUILT_IN_FUNCTION_SYMBOL);
		newFunction->context->value =newValue(BUILT_IN_FUNCTION_VALUE,
			i, EXPRESSION_OPND);
		if (builtInFunctionTable[i].isSpecialFunction)
			newFunction->context->attributes |=
				SPECIAL_FUNCTION_ATT;
	}
}

  void
installPredefinedSymbols(void)
{
	int				 i;
	symbolTableEntryType		*newSymbol;

	for (i=0; predefinedSymbolTable[i].symbolName!=NULL; i++) {
		newSymbol = lookupOrEnterSymbol(predefinedSymbolTable[i].
			symbolName, DEFINE_SYMBOL);
		newSymbol->context->value = newValue(ABSOLUTE_VALUE,
			predefinedSymbolTable[i].symbolValue,
			EXPRESSION_OPND);
		newSymbol->context->attributes |= DEFINED_VARIABLE_ATT;
	}
}

  void
installCommandLineDefineSymbols(void)
{
	int				 i;
	symbolTableEntryType		*newSymbol;

	while (commandLineDefines != NULL) {
		newSymbol = lookupOrEnterSymbol(commandLineDefines->name,
			DEFINE_SYMBOL);
		newSymbol->context->value = newValue(ABSOLUTE_VALUE,
			commandLineDefines->value, EXPRESSION_OPND);
		newSymbol->context->attributes |= DEFINED_VARIABLE_ATT;
		commandLineDefines = commandLineDefines->nextDefine;
	}
}

  void
createHashTables(void)
{
	opcodeTableEntryType	*newOpcodeEntry;
	keywordTableEntryType	*newKeywordEntry;
	conditionTableEntryType	*newConditionEntry;

	newOpcodeEntry = theOpcodes;
	while (newOpcodeEntry->mnemonic != NULL)
		hashStringEnter(newOpcodeEntry++, opcodeTable);

	if (machine == 0) {
		newOpcodeEntry = theOpcodes_02;
		while (newOpcodeEntry->mnemonic != NULL)
			hashStringEnter(newOpcodeEntry++, opcodeTable);
	}
	else if (machine == 1) {
		newOpcodeEntry = theOpcodes_c02;
		while (newOpcodeEntry->mnemonic != NULL)
			hashStringEnter(newOpcodeEntry++, opcodeTable);	
	}
	else if (machine >= 2) {
		newOpcodeEntry = theOpcodes_c02;
		while (newOpcodeEntry->mnemonic != NULL)
			hashStringEnter(newOpcodeEntry++, opcodeTable);	
		newOpcodeEntry = theOpcodes_c02s;
		while (newOpcodeEntry->mnemonic != NULL)
			hashStringEnter(newOpcodeEntry++, opcodeTable);	
	}

	newKeywordEntry = theKeywords;
	while (newKeywordEntry->string != NULL)
		hashStringEnter(newKeywordEntry++, keywordTable);

	newConditionEntry = theConditions;
	while (newConditionEntry->string != NULL)
		hashStringEnter(newConditionEntry++, conditionTable);
}

  void
queueInputFile(char *name)
{
	fileNameListType	*newFileName;

	newFileName = typeAlloc(fileNameListType);
	newFileName->name = name;
	newFileName->fildes = NULL;
	newFileName->openFlag = FALSE;
	newFileName->nextFileName = NULL;
	newFileName->lineNumber = 1;
	if (inputFileStack == NULL) {
		inputFileStack = bottomOfInputFileStack = newFileName;
	} else {
		bottomOfInputFileStack->nextFileName = newFileName;
		bottomOfInputFileStack = newFileName;
	}
}

  void
openFirstInputFile(void)
{
	if (inputFileStack == NULL) {
		inputFileStack = typeAlloc(fileNameListType);
		inputFileStack->name = "<standard input>";
		inputFileStack->fildes = stdin;
		inputFileStack->openFlag = TRUE;
		inputFileStack->lineNumber = 1;
		inputFileStack->nextFileName = NULL;
	} else {
		if ((inputFileStack->fildes = fopen(inputFileStack->name,
							"r")) == NULL) {
			fatalSystemError(UNABLE_TO_OPEN_INPUT_FILE_ERROR,
				inputFileStack->name);
		} else {
			inputFileStack->openFlag = TRUE;
		}
	}
	input = inputFileStack->fildes;
	currentLineNumber = inputFileStack->lineNumber;
	currentFileName = inputFileStack->name;
	currentOperandNumber = 0;
}

  bool
isDotMName(stringType *fileName)
{
	int	length;

	length = strlen(fileName);
	return(length >= 2 && fileName[length - 1] == 'm' &&
		fileName[length - 2] == '.');
}

  bool
parseCommandLineDefine(char *arg, char **name, int *value)
{
	char	*ptr;
	char	 c;
	int	 base;
	int	 start;

	ptr = arg;
	while ((c = *ptr++) && (ptr-arg)<MAX_NAME_SIZE && isAlphaNumeric(c))
		;
	--ptr;
	if (c=='\0') {
		*value = 1;
	} else if (c == '=') {
		*ptr++ = '\0';
		if (ptr[0] != '0') {
			base = 10;
			start = 0;
		} else if (ptr[1]=='b' || ptr[1]=='B') {
			base = 2;
			start = 2;
		} else if (ptr[1]=='q' || ptr[1]=='Q') {
			base = 4;
			start = 2;
		} else if (ptr[1]=='x' || ptr[1]=='X') {
			base = 16;
			start = 2;
		} else {
			base = 8;
			start = 1;
		}
		*value = fancyAtoI(&ptr[start], base);
	} else {
		error(BAD_COMMAND_LINE_DEFINE_ERROR, arg);
		*name = NULL;
		*value = 0;
		return(FALSE);
	}
	*name = arg;
	return(TRUE);
}

  void
noteCommandLineDefine(char *arg)
{
	char			*name;
	int			 value;
	commandLineDefineType	*newCommandLineDefine;

	if (parseCommandLineDefine(arg, &name, &value)) {
		newCommandLineDefine = typeAlloc(commandLineDefineType);
		newCommandLineDefine->name = name;
		newCommandLineDefine->value = value;
		newCommandLineDefine->nextDefine = commandLineDefines;
		commandLineDefines = newCommandLineDefine;
	}
}
