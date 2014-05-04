/*
	parserMisc.c -- Miscellaneous parser support routines for the Macross
			assembler.

	Chip Morningstar -- Lucasfilm Ltd.

	3-November-1984

*/

#include "macrossTypes.h"
#include "macrossGlobals.h"
#include "y.tab.h"

  statementType *
addLabelToStatement(labelList, statement)
  labelListType	*labelList;
  statementType	*statement;
{
	statementType	*newStatement();

	if (statement == NULL)
		statement = newStatement(NULL_STATEMENT, NULL);
	statement->labels = labelList;
	return(statement);
}

  void
botch(message, arg1, arg2, arg3)
  char		*message;
  anyOldThing	*arg1;
  anyOldThing	*arg2;
  anyOldThing	*arg3;
{
	printf("Macross horrible terrible internal botch: ");
	printf(message, arg1, arg2, arg3);
	chokePukeAndDie();
}

  void
checkDefineAssignmentOperator(assignmentOperator)
  assignmentKindType	assignmentOperator;
{
	if (assignmentOperator != ASSIGN_ASSIGN)
		puntOnError(DEFINE_ASSIGNMENT_WRONG_ERROR);
}

  statementType *
convertDefineToMdefine(defineStatement)
  statementType	*defineStatement;
{
	if (defineStatement->kindOfStatement != DEFINE_STATEMENT)
		botch("convertDefineToMdefine got statement kind: %d\n",
			defineStatement->kindOfStatement);
	defineStatement->kindOfStatement = MDEFINE_STATEMENT;
	return(defineStatement);
}

  ifStatementBodyType *
extractIfBody(ifStatement)
  statementType	*ifStatement;
{
	ifStatementBodyType	*result;

	result = ifStatement->statementBody.ifUnion;
	if (ifStatement->labels != NULL)
		botch("extract if body with non-null labels\n");
	else if (ifStatement->nextStatement != NULL)
		botch("extract if body with non-null next\n");
	else
		qfree(ifStatement);
	return(result);
}

  mifStatementBodyType *
extractMifBody(mifStatement)
  statementType	*mifStatement;
{
	mifStatementBodyType	*result;

	result = mifStatement->statementBody.mifUnion;
	if (mifStatement->labels != NULL)
		botch("extract mif body with non-null labels\n");
	else if (mifStatement->nextStatement != NULL)
		botch("extract mif body with non-null next\n");
	else
		qfree(mifStatement);
	return(result);
}

  stringType *
extractString(textExpression)
  operandType	*textExpression;
{
	stringType	*result;

	if (textExpression->kindOfOperand != STRING_OPND)
		botch("extract string got handed an opnd kind: %d\n",
			textExpression->kindOfOperand);
	result = textExpression->theOperand.stringUnion;
	qfree(textExpression);
	return(result);
}

  void
popMacroOrFunctionNestingDepth()
{
	if (--macroOrFunctionNestingDepth == 0)
		unknownSymbolTag = UNKNOWN_SYMBOL;
}

  void
pushMacroOrFunctionNestingDepth()
{
	macroOrFunctionNestingDepth++;
	unknownSymbolTag = NESTED_UNKNOWN_SYMBOL;
}

  char *
saveString(s)
  char	*s;
{
	char	*result;

	result = (char *)malloc(strlen(s)+1);
	strcpy(result, s);
	return(result);
}
