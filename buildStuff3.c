/*
	buildStuff3.c -- Still more routines to build parse-tree structures
			for the Macross assembler.

	Chip Morningstar -- Lucasfilm Ltd.

	2-November-1984

*/

#include "macrossTypes.h"
#include "macrossGlobals.h"

/*
   These routines all build list-type structures.  Since yacc likes to build
   things left-recursively but we don't want lists to be backwards (which
   would then necessitate reversing them at some point -- yuck), we employ
   the ruse of substituting for the first element of a list a different
   struct which is identical to a normal one but contains an additional field
   for a pointer to the tail element of the list.  We can then add things
   directly onto the tail as we like.
 */

  argumentListHeadType *
buildArgumentList(new, rest, arrayTag)
  stringType		*new;
  argumentListHeadType	*rest;
  bool			 arrayTag;
{
	argumentListHeadType		*newListHead;
	argumentDefinitionListType	*newArgument;
	symbolTableEntryType		*lookupOrEnterSymbol();

	if (rest == NULL) {
		newListHead = typeAlloc(argumentListHeadType);
		newListHead->theArgument = lookupOrEnterSymbol(new,
			DEAD_SYMBOL);
		newListHead->nextArgument = NULL;
		newListHead->tail = NULL;
		newListHead->arrayTag = arrayTag;
		return(newListHead);
	} else {
		newArgument = typeAlloc(argumentDefinitionListType);
		newArgument->theArgument = lookupOrEnterSymbol(new,
			DEAD_SYMBOL);
		newArgument->nextArgument = NULL;
		if (rest->tail != NULL)
			rest->tail->nextArgument = newArgument;
		else
			rest->nextArgument = newArgument;
		rest->tail = newArgument;
		rest->arrayTag = arrayTag;
		return(rest);
	}
}

  caseListHeadType *
buildCaseList(new, rest)
  caseType		*new;
  caseListHeadType	*rest;
{
	caseListHeadType		*newListHead;
	caseListType			*newCase;

	if (rest == NULL) {
		newListHead = typeAlloc(caseListHeadType);
		newListHead->theCase = new;
		newListHead->nextCase = NULL;
		newListHead->tail = NULL;
		return(newListHead);
	} else {
		newCase = typeAlloc(caseListType);
		newCase->theCase = new;
		newCase->nextCase = NULL;
		if (rest->tail != NULL)
			rest->tail->nextCase = newCase;
		else
			rest->nextCase = newCase;
		rest->tail = newCase;
		return(rest);
	}
}

  expressionListHeadType *
buildExpressionList(new, rest)
  expressionType		*new;
  expressionListHeadType	*rest;
{
	expressionListHeadType	*newListHead;
	expressionListType	*newListEntry;

	if (rest == NULL) {
		newListHead = typeAlloc(expressionListHeadType);
		newListHead->theExpression = new;
		newListHead->nextExpression = NULL;
		newListHead->tail = NULL;
		return(newListHead);
	} else {
		newListEntry = typeAlloc(expressionListType);
		newListEntry->theExpression = new;
		newListEntry->nextExpression = NULL;
		if (rest->tail != NULL)
			rest->tail->nextExpression = newListEntry;
		else
			rest->nextExpression = newListEntry;
		rest->tail = newListEntry;
		return(rest);
	}
}

  identifierListHeadType *
buildIdentifierList(new, rest, usage)
  stringType			*new;
  identifierListHeadType	*rest;
  symbolUsageKindType		 usage;
{
	identifierListType	*newListEntry;
	identifierListHeadType	*newListHead;

	symbolTableEntryType	*lookupOrEnterSymbol();

	if (rest == NULL) {
		newListHead = typeAlloc(identifierListHeadType);
		newListHead->theSymbol = lookupOrEnterSymbol(new, usage);
		newListHead->nextIdentifier = NULL;
		newListHead->tail = NULL;
		return(newListHead);
	} else {
		newListEntry = typeAlloc(identifierListType);
		newListEntry->theSymbol = lookupOrEnterSymbol(new, usage);
		newListEntry->nextIdentifier = NULL;
		if (rest->tail != NULL)
			rest->tail->nextIdentifier = newListEntry;
		else
			rest->nextIdentifier = newListEntry;
		rest->tail = newListEntry;
		return(rest);
	}
}

  labelListHeadType *
buildLabelList(new, rest)
  symbolTableEntryType	*new;
  labelListHeadType	*rest;
{
	labelListHeadType	*newLabelHead;
	labelListType		*newLabel;

	if (rest == NULL) {
		newLabelHead = typeAlloc(labelListHeadType);
		newLabelHead->theLabel = new;
		newLabelHead->nextLabel = NULL;
		newLabelHead->tail = NULL;
		return(newLabelHead);
	} else {
		newLabel = typeAlloc(labelListType);
		newLabel->theLabel = new;
		newLabel->nextLabel = NULL;
		if (rest->tail != NULL)
			rest->tail->nextLabel = newLabel;
		else
			rest->nextLabel = newLabel;
		rest->tail = newLabel;
		return(rest);
	}
}

  operandListHeadType *
buildOperandList(new, rest)
  operandType		*new;
  operandListHeadType	*rest;
{
	operandListHeadType	*newListHead;

	if (rest == NULL) {
		newListHead = typeAlloc(operandListHeadType);
		newListHead->kindOfOperand = new->kindOfOperand;
		newListHead->theOperand = new->theOperand;
		newListHead->nextOperand = NULL;
		newListHead->tail = NULL;
		qfree(new);
		return(newListHead);
	} else {
		new->nextOperand = NULL;
		if (rest->tail != NULL)
			rest->tail->nextOperand = new;
		else
			rest->nextOperand = new;
		rest->tail = new;
		return(rest);
	}
}

  selectionListHeadType *
buildSelectionList(new, rest)
  selectionListType	*new;
  selectionListHeadType	*rest;
{
	return((selectionListHeadType *) buildIdentifierList(new, rest));
}

  statementListHeadType *
buildStatementList(new, rest)
  statementType		*new;
  statementListHeadType	*rest;
{
	statementListHeadType	*newListHead;

	if (new == NULL)
		return(rest);
	if (rest == NULL) {
		newListHead = typeAlloc(statementListHeadType);
		newListHead->kindOfStatement = new->kindOfStatement;
		newListHead->labels = new->labels;
		newListHead->statementBody = new->statementBody;
		newListHead->lineNumber = new->lineNumber;
		newListHead->fileName = new->fileName;
		newListHead->cumulativeLineNumber = new->cumulativeLineNumber;
		newListHead->nextStatement = NULL;
		newListHead->tail = NULL;
		qfree(new);
		return(newListHead);
	} else {
		new->nextStatement = NULL;
		if (rest->tail != NULL)
			rest->tail->nextStatement = new;
		else
			rest->nextStatement = new;
		rest->tail = new;
		return(rest);
	}
}
