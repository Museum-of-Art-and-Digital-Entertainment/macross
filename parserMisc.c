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
	parserMisc.c -- Miscellaneous parser support routines for the Macross
			assembler.

	Chip Morningstar -- Lucasfilm Ltd.

	3-November-1984

*/

#include "macrossTypes.h"
#include "macrossGlobals.h"
#include "y.tab.h"
#include "parserMisc.h"

#include <stdarg.h>
#include <string.h>

  
extern void chokePukeAndDie (void);
extern void puntOnError (errorType theError, ...);

statementType *
addLabelToStatement(labelListType *labelList, statementType *statement)
{
	statementType	*newStatement(statementKindType kind, statementBodyType body);

	if (statement == NULL)
		statement = newStatement(NULL_STATEMENT, (statementBodyType){ NULL });
	statement->labels = labelList;
	return(statement);
}

/* TODO: This should be a varargs function. In 1984 it probably wasn't
 * standardized, but here in Glorious Future Year 1989 the vprintf
 * function does almost exactly what we want. */

  void
botch(char *message, ...)
{
	va_list ap;
	printf("Macross horrible terrible internal botch: ");
	va_start(ap, message);
	vprintf(message, ap);
	va_end(ap);
	chokePukeAndDie();
}

  void
checkDefineAssignmentOperator(assignmentKindType assignmentOperator)
{
	if (assignmentOperator != ASSIGN_ASSIGN)
		puntOnError(DEFINE_ASSIGNMENT_WRONG_ERROR);
}

  statementType *
convertDefineToMdefine(statementType *defineStatement)
{
	if (defineStatement->kindOfStatement != DEFINE_STATEMENT)
		botch("convertDefineToMdefine got statement kind: %d\n",
			defineStatement->kindOfStatement, 0, 0);
	defineStatement->kindOfStatement = MDEFINE_STATEMENT;
	return(defineStatement);
}

  ifStatementBodyType *
extractIfBody(statementType *ifStatement)
{
	ifStatementBodyType	*result;

	result = ifStatement->statementBody.ifUnion;
	if (ifStatement->labels != NULL)
		botch("extract if body with non-null labels\n", 0, 0, 0);
	else if (ifStatement->nextStatement != NULL)
		botch("extract if body with non-null next\n", 0, 0, 0);
	else
		qfree(ifStatement);
	return(result);
}

  mifStatementBodyType *
extractMifBody(statementType *mifStatement)
{
	mifStatementBodyType	*result;

	result = mifStatement->statementBody.mifUnion;
	if (mifStatement->labels != NULL)
		botch("extract mif body with non-null labels\n", 0, 0, 0);
	else if (mifStatement->nextStatement != NULL)
		botch("extract mif body with non-null next\n", 0, 0, 0);
	else
		qfree(mifStatement);
	return(result);
}

  stringType *
extractString(operandType *textExpression)
{
	stringType	*result;

	if (textExpression->kindOfOperand != STRING_OPND)
		botch("extract string got handed an opnd kind: %d\n",
			textExpression->kindOfOperand, 0, 0);
	result = textExpression->theOperand.stringUnion;
	qfree(textExpression);
	return(result);
}

  void
popMacroOrFunctionNestingDepth(void)
{
	if (--macroOrFunctionNestingDepth == 0)
		unknownSymbolTag = UNKNOWN_SYMBOL;
}

  void
pushMacroOrFunctionNestingDepth(void)
{
	macroOrFunctionNestingDepth++;
	unknownSymbolTag = NESTED_UNKNOWN_SYMBOL;
}

  char *
saveString(char *s)
{
	char	*result;

	result = (char *)malloc(strlen(s)+1);
	strcpy(result, s);
	return(result);
}
