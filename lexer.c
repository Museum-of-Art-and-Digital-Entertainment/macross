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
	lexer.c -- Lexical scanner for the Macross assembler

	Chip Morningstar -- Lucasfilm Ltd.

	3-November-1984
*/

#include "macrossTypes.h"
#include "macrossGlobals.h"
#include "y.tab.h"
#include "lexerTables.h"

extern int yylval;
extern int yydebug;

static char	lineBuffer[LINE_BUFFER_SIZE] = { '\0' };
static int	lineBufferPtr = 0;

#define getNextChar() (lineBuffer[lineBufferPtr] ? \
			lineBuffer[lineBufferPtr++] : \
			readAnotherLine())
/*int getNextChar() {int c;c=xgetNextChar();printf("read '%c'\n",c);return(c);}*/

#define oopsThatWasTheWrongChar(c) { if(lineBufferPtr) \
					lineBuffer[--lineBufferPtr] = c; }
/*oopsThatWasTheWrongChar(c)char c;{printf("ungetting '%c'\n", c);xoopsThatWasTheWrongChar(c);}*/

#define isAlphabetic(c)		(alphabeticCharacterTable[c])
#define isNumeric(c)		(numericCharacterTable[c])
#define isAlphaNumeric(c)	(alphaNumericCharacterTable[c])

  int
yylex()
{
	int	result;

	result = lexer();
	if (yydebug) {
		printf("lexer returns ");
		printToken(result);
		printf(", value=%d (0x%x)\n", yylval, yylval);
	}
	return(result);
}

  int
lexer()
{
	char	c;

	char	skipWhitespaceAndComments();

	if ((c = skipWhitespaceAndComments()) == EOF)
		return(lexLiteral(c));
	else
		return((*lexDispatchTable[c])(c));
}

  void
initializeLexDispatchTable()
{
	int	c;
	int	lexIdentifier();
	int	lexNumber();
	int	lexLiteral();
	int	lexCharacterConstant();
	int	lexStringConstant();
	int	lexOperator();

	for (c = 0; c < LEX_DISPATCH_TABLE_SIZE; c++) {
		if (isAlphabetic(c) || c=='$')
			lexDispatchTable[c] = lexIdentifier;
		else if (isNumeric(c))
			lexDispatchTable[c] = lexNumber;
		else if (isMacrossLiteralCharacter(c))
			lexDispatchTable[c] = lexLiteral;
		else if (c == '\'')
			lexDispatchTable[c] = lexCharacterConstant;
		else if (c == '"')
			lexDispatchTable[c] = lexStringConstant;
		else
			lexDispatchTable[c] = lexOperator;
	}
}

  bool
isMacrossLiteralCharacter(c)
  char	c;
{
	return(c==':' || c==',' || c=='@' || c=='#' ||
	       c=='(' || c==')' || c=='{' || c=='}' || c=='[' || c==']' ||
	       c=='\n' || c==EOF);
}

  void
snarfAlphanumericString(c, buffer)
  char	 c;
  char	*buffer;
{
	char	*bufferPtr;

	bufferPtr = buffer;
	do {
		if (bufferPtr < &buffer[MAX_NAME_SIZE])
			*bufferPtr++ = c;
		c = getNextChar();
	} while (c!=EOF && isAlphaNumeric(c));
	*bufferPtr = '\0';
	oopsThatWasTheWrongChar(c);
}

char	nameBuffer[MAX_NAME_SIZE+1];

  int
lexIdentifier(c)
  char	c;
{
	char	*saveString();
	int	 hashValue;

	snarfAlphanumericString(c, nameBuffer);
	hashValue = hashString(nameBuffer);
	if (yylval = lookupOpcode(nameBuffer, hashValue))
		return(Opcode);
	else if (yylval = lookupKeyword(nameBuffer, hashValue))
		return(yylval);
	else if ((yylval = lookupConditionCode(nameBuffer, hashValue)) !=
						(int)NOT_FOUND_COND)
		return(ConditionCode);
	else if (yylval = lookupMacroName(nameBuffer, hashValue))
		return(MacroName);
	else {
		yylval = (int) saveString(nameBuffer);
		return(Identifier);
	}
}

char	numberBuffer[MAX_NAME_SIZE+1];

  int
lexNumber(c)
  char	c;
{
	int	base;
	int	start;

	snarfAlphanumericString(c, numberBuffer);
	if (numberBuffer[0] != '0') {
		base = 10;
		start = 0;
	} else if (numberBuffer[1]=='b' || numberBuffer[1]=='B') {
		base = 2;
		start = 2;
	} else if (numberBuffer[1]=='q' || numberBuffer[1]=='Q') {
		base = 4;
		start = 2;
	} else if (numberBuffer[1]=='x' || numberBuffer[1]=='X') {
		base = 16;
		start = 2;
	} else {
		base = 8;
		start = 1;
	}
	yylval = fancyAtoI(&numberBuffer[start], base);
	return(Number);
}

  int
fancyAtoI(buffer, base)
  char	*buffer;
  int	 base;
{
	int	value;
	int	digit;
	char	c;

	value = 0;
	while (*buffer != '\0') {
		if ((digit = digitValue(c = *buffer++)) >= base) {
			error(DIGIT_OUT_OF_RADIX_ERROR, c, base);
			return(0);
		}
		value = value*base + digit;
	}
	return(value);
}

  int
digitValue(c)
  char	c;
{
	if (isNumeric(c))
		return(c - '0');
	else
		return(toLowerCase(c) - 'a' + 10);
}

  int
lexLiteral(c)
  char	c;
{
	static bool	passedEnd = FALSE;

	yylval = 0;
	if (c == '\n') {
		return(EOL);
	} else if (c == EOF) {
		if (passedEnd) {
			return(0);
		} else {
			passedEnd = TRUE;
			return(ENDFILE);
		}
	} else {
		return(c);
	}
}

  int
lexCharacterConstant()
{
	char	c;

	yylval = getStringCharacter(input);
	if (getNextChar() != '\'') {
		error(UNCLOSED_CHARACTER_CONSTANT_ERROR);
		while ((c = getNextChar())!='\'' && c!='\n' && c!=EOF)
			;
	}
	return(Number);
}

bool	escaped;	/* true if last string character read was an escape
					     code. */
  int
getStringCharacter(input)
  FILE	*input;
{
	char	 c;
	char	*numberPtr;
	int	 result;
	char	 controlCharacter();

	escaped = FALSE;
	c = getNextChar();
	if (c == '\\') {
		escaped = TRUE;
		c = getNextChar();
		if (c == '^')
			return(controlCharacter(getNextChar()));
		else if ('0'<=c && c<='7') {
			numberPtr = numberBuffer;
			while ('0'<=c && c<='7') {
				*numberPtr++ = c;
				c = getNextChar();
			}
			*numberPtr = '\0';
			oopsThatWasTheWrongChar(c);
			result = fancyAtoI(numberBuffer, 8);
			if (result > 0377)
				error(OCTAL_CHARACTER_TOO_BIG_ERROR, result);
			return (result % 0377);
		} else
			return(escapeCodes[c]);
	} else
		return(c);
}

char	stringBuffer[MAX_NAME_SIZE + 1];

  int
lexStringConstant()
{
	char	*stringPtr;
	char	 c;

	stringPtr = stringBuffer;
	while (((c = getStringCharacter(input))!='"' && c!='\n' && c!=EOF)
								|| escaped)
		*stringPtr++ = c;
	*stringPtr = '\0';
	if (c=='\n' || c==EOF)
		error(UNCLOSED_STRING_CONSTANT_ERROR);
	yylval = (int)saveString(stringBuffer);
	return(TextString);
}

  int
lexOperator(firstC)
  char	firstC;
{
	char	secondC;
	char	thirdC;
	int	op;
	int	oper;

	secondC = getNextChar();
	for (op=0; operatorTable[op].first!='\0'; op++) {
		if (operatorTable[op].first==firstC &&
					 operatorTable[op].second==secondC)
			break;
		else if (operatorTable[op].first==firstC &&
					 operatorTable[op].second=='\0') {
			oopsThatWasTheWrongChar(secondC);
			break;
		}
	}
	if (operatorTable[op].first == '\0') {
		error(UNRECOGNIZED_SOMETHING_OR_OTHER_ERROR, firstC);
		return(yylex());
	}
	/* kludge to deal with the two three-character operators: */
	if ((oper=operatorTable[op].token)==RIGHT_SHIFT || oper==LEFT_SHIFT) {
		thirdC = getNextChar();
		if (thirdC == '=') {
			yylval = (int)((oper==RIGHT_SHIFT) ?
				RIGHT_SHIFT_ASSIGN : LEFT_SHIFT_ASSIGN);
			return(ASSIGN);
		} else
			oopsThatWasTheWrongChar(thirdC);
	}
	yylval = (int)operatorTable[op].value;
	return(operatorTable[op].token);
}

  char
controlCharacter(c)
  char	c;
{
#define CONTROL_CHARACTER_MASK (~0100)

	return(c & CONTROL_CHARACTER_MASK);
}

  char
skipWhitespaceAndComments()
{
	char	c;

	while ((c=getNextChar())==' ' || c=='\t' || c=='/') {
	    if (c == '/') {
		if ((c = getNextChar()) == '*') {
		    while (TRUE) {
			while ((c = getNextChar()) != '*') {
			    if (c == EOF) {
				error(UNCLOSED_COMMENT_ERROR);
				return(c);
			    }
			}
			if ((c = getNextChar()) == '/') {
			    break;
			} else if (c == '*') {
			    oopsThatWasTheWrongChar(c);
			}
		    }
		} else {
		    oopsThatWasTheWrongChar(c);
		    return('/');
		}
	    }
	}
	if (c == ';') {
		while ((c = getNextChar()) != '\n') {
			if (c == EOF) {
				error(UNCLOSED_LINE_COMMENT_ERROR);
				return(c);
			}
		}
	}
	return(c);
}

  int
popInputFileStack()
{
	fileNameListType	*oldFile;

	if (inputFileStack->nextFileName == NULL)
		return(EOF);
	oldFile = inputFileStack;
	inputFileStack = inputFileStack->nextFileName;
	qfree(oldFile);
	currentLineNumber = inputFileStack->lineNumber;
	currentFileName = inputFileStack->name;
	cumulativeLineNumber--;
	fclose(input);
	if (!inputFileStack->openFlag) {
		if ((inputFileStack->fildes = fopen(inputFileStack->name,
							"r")) == NULL) {
			fatalSystemError(UNABLE_TO_OPEN_INPUT_FILE_ERROR,
				inputFileStack->name);
		} else {
			inputFileStack->openFlag = TRUE;
		}
	}
	input = inputFileStack->fildes;
	if (includeNestingDepth > 0) {
		includeNestingDepth--;
		currentLineNumber--;
	}
	return(getNextChar());
}

  void
pushInputFileStack(fileName)
  stringType	*fileName;
{
	fileNameListType	*newFileName;

	inputFileStack->lineNumber = currentLineNumber;
	newFileName = typeAlloc(fileNameListType);
	if ((input = newFileName->fildes = fopen(fileName, "r")) == NULL) {
		fatalSystemError(UNABLE_TO_OPEN_INCLUDE_FILE_ERROR, fileName);
	}
	newFileName->openFlag = TRUE;
	newFileName->nextFileName = inputFileStack;
	inputFileStack = newFileName;
	currentFileName = newFileName->name = fileName;
	currentLineNumber = newFileName->lineNumber = 1;
	includeNestingDepth++;
	if (statementEvaluationDepth == 1)
		oopsThatWasTheWrongChar('\n'); /* hack for line #'s */
}

  void
resynchronizeInput()
{
	char	c;
	while ((c = getNextChar())!='\n' && c!=EOF)
		;
	oopsThatWasTheWrongChar(c);
}

       bool	longLineFlag = FALSE;
static bool	previousLongLineFlag = FALSE;

  void
saveLineForListing(line)
  stringType	*line;
{
	if (!previousLongLineFlag) {
		putw(currentLocationCounter.value, saveFileForPass2);
		putw(includeNestingDepth, saveFileForPass2);
	}
	previousLongLineFlag = longLineFlag;
	fputs(line, saveFileForPass2);
}

  void
saveEOLForListing()
{
	putw(-1, saveFileForPass2);
	putw(includeNestingDepth, saveFileForPass2);
	fputs("\n", saveFileForPass2);
}

  void
saveIndexForListing(kindOfStatement, cumulativeLineNumber)
  statementKindType	kindOfStatement;
  int			cumulativeLineNumber;
{
	if (!amExpanding() || !notListable(kindOfStatement)) {
		putw(kindOfStatement, indexFileForPass2);
		putw(currentLocationCounter.value, indexFileForPass2);
		if (amExpanding())
			putw(-cumulativeLineNumber, indexFileForPass2);
		else
			putw( cumulativeLineNumber, indexFileForPass2);
	}
}

  void
saveEndMifForListing(cumulativeLineNumber)
  int	cumulativeLineNumber;
{
	putw(MIF_STATEMENT, indexFileForPass2);
	putw(-1, indexFileForPass2);
	if (amExpanding())
		putw(-cumulativeLineNumber, indexFileForPass2);
	else
		putw(cumulativeLineNumber, indexFileForPass2);
}

  void
saveListingOff()
{
	saveIndexForListing(-1, cumulativeLineNumber);
}

  void
saveListingOn()
{
	if (currentCodeMode == ABSOLUTE_BUFFER)
		saveIndexForListing(-1, cumulativeLineNumber);
	else
		saveIndexForListing(-2, cumulativeLineNumber);
}

  char *
myfgets(buffer, length, stream)
  char	*buffer;
  int	 length;
  FILE	*stream;
{
	char	*result;
	char	 c;

	result = buffer;
	while (length-- > 1 && (c = getc(stream)) != EOF && c != '\n')
		*buffer++ = c;
	if (c == EOF) {
		*result = '\0';
		return(NULL);
	}
	if (length > 0)
		*buffer++ = c;
	if (length == 0 && c != '\n')
		longLineFlag = TRUE;
	else
		longLineFlag = FALSE;
	*buffer = '\0';
	return(result);
}

  int
readAnotherLine()
{
	int	result;

	if (myfgets(lineBuffer, LINE_BUFFER_SIZE, input)) {
		if (amListing())
			saveLineForListing(lineBuffer);
		lineBufferPtr = 1;
		result = lineBuffer[0];
	} else {
		result = popInputFileStack();
	}
	currentLineNumber++;
	cumulativeLineNumber++;
	return(result);
}
