/*
	buildStuff2.c -- More routines to build parse-tree structures for the
			 Macross assembler.  This module is target processor
			 independent.

	Chip Morningstar -- Lucasfilm Ltd.

	2-November-1984

*/

#include "macrossTypes.h"
#include "macrossGlobals.h"

/* 
   These are all the miscellaneous routines for building pieces of parse-tree
   structures.
 */


/* Fragments of statement structures */

  caseType *
buildCase(caseTags, caseBody)
  expressionListType	*caseTags;
  blockType		*caseBody;
{
	caseType	*result;

	result = typeAlloc(caseType);
	result->caseTags = caseTags;
	result->caseBody = caseBody;
	return(result);
}

  doEndType *
buildDoEnd(condition, kindOfDoEnd)
  conditionType		 condition;
  doEndKindType		 kindOfDoEnd;
{
	doEndType	*result;

	result = typeAlloc(doEndType);
	result->doEndKind = kindOfDoEnd;
	result->doEndCondition = condition;
	return(result);
}

  forExpressionsType *
buildForExpressions(initExpression, testExpression, incrExpression)
  expressionType	*initExpression;
  expressionType	*testExpression;
  expressionType	*incrExpression;
{
	forExpressionsType	*result;

	result = typeAlloc(forExpressionsType);
	result->initExpression = initExpression;
	result->testExpression = testExpression;
	result->incrExpression = incrExpression;
	return(result);
}

  ifHeadType *
buildIfHead(condition, block)
  conditionType	 condition;
  blockType	*block;
{
	ifHeadType	*result;

	result = typeAlloc(ifHeadType);
	result->headCondition = condition;
	result->headBody = block;
	return(result);
}

  mdoEndType *
buildMdoEnd(condition, kindOfMdoEnd)
  expressionType	*condition;
  doEndKindType		 kindOfMdoEnd;
{
	mdoEndType	*result;

	result = typeAlloc(mdoEndType);
	result->mdoEndKind = kindOfMdoEnd;
	result->mdoEndCondition = condition;
	return(result);
}


  mifHeadType *
buildMifHead(condition, block)
  expressionType	*condition;
  blockType		*block;
{
	mifHeadType	*result;

	result = typeAlloc(mifHeadType);
	result->headCondition = condition;
	result->headBody = block;
	return(result);
}


/* Fragments of expression structures */

  arrayTermType *
buildArrayTerm(array, index)
  expressionType	*array;
  expressionType	*index;
{
	arrayTermType	*result;

	result = typeAlloc(arrayTermType);
	result->arrayName = array;
	result->arrayIndex = index;
	return(result);
}

  binopTermType *
buildBinopTerm(binop, leftArgument, rightArgument)
  binopKindType		 binop;
  expressionType	*leftArgument;
  expressionType	*rightArgument;
{
	binopTermType	*result;

	result = typeAlloc(binopTermType);
	result->binop = binop;
	result->leftArgument = leftArgument;
	result->rightArgument = rightArgument;
	return(result);
}

  functionCallTermType *
buildFunctionCall(functionName, arguments)
  stringType		*functionName;
  operandListType	*arguments;
{
	functionCallTermType	*result;
	symbolTableEntryType	*lookupOrEnterSymbol();

	result = typeAlloc(functionCallTermType);
	result->functionName = lookupOrEnterSymbol(functionName,
		UNKNOWN_FUNCTION_SYMBOL);
	result->parameters = arguments;
	return(result);
}

  postOpTermType *
buildPostOpTerm(postOp, postOpArgument)
  postOpKindType	 postOp;
  expressionType	*postOpArgument;
{
	postOpTermType	*result;

	result = typeAlloc(postOpTermType);
	result->postOp = postOp;
	result->postOpArgument = postOpArgument;
	return(result);
}

  preOpTermType *
buildPreOpTerm(preOp, preOpArgument)
  preOpKindType		 preOp;
  expressionType	*preOpArgument;
{
	preOpTermType	*result;

	result = typeAlloc(preOpTermType);
	result->preOp = preOp;
	result->preOpArgument = preOpArgument;
	return(result);
}

  unopTermType *
buildUnopTerm(unop, unopArgument)
  unopKindType		 unop;
  expressionType	*unopArgument;
{
	unopTermType	*result;

	result = typeAlloc(unopTermType);
	result->unop = unop;
	result->unopArgument = unopArgument;
	return(result);
}

  expressionTermType *  
buildExpressionTerm(kindOfExpressionTerm, arg1, arg2, arg3)
  expressionTermKindType	 kindOfExpressionTerm;
  anyOldThing			*arg1;
  anyOldThing			*arg2;
  anyOldThing			*arg3;
{
	expressionType		*result;

	symbolTableEntryType	*lookupOrEnterSymbol();

	result = typeAlloc(expressionType);
	result->kindOfTerm = kindOfExpressionTerm;
	switch (kindOfExpressionTerm) {

	case IDENTIFIER_EXPR:
		result->expressionTerm.identifierUnion =
			(identifierTermType *) arg1;
		break;

	case ARRAY_EXPR:
		result->expressionTerm.arrayUnion =
			buildArrayTerm((expressionType *) arg1,
			(expressionType *) arg2);
		break;

	case FUNCTION_CALL_EXPR:
		result->expressionTerm.functionCallUnion =
			(functionCallTermType *) arg1;
		break;

	case NUMBER_EXPR:
		result->expressionTerm.numberUnion = (numberTermType) arg1;
		break;

	case STRING_EXPR:
		result->expressionTerm.stringUnion = (stringTermType *) arg1;
		break;

	case SUBEXPRESSION_EXPR:
		result->expressionTerm.subexpressionUnion =
			(subexpressionTermType *) arg1;
		break;

	case UNOP_EXPR:
		result->expressionTerm.unopUnion =
			buildUnopTerm((unopKindType) arg1,
				      (expressionType *) arg2);
		break;

	case ASSIGN_EXPR:
	case BINOP_EXPR:
		result->expressionTerm.binopUnion =
			buildBinopTerm((binopKindType) arg1,
				       (expressionType *) arg2,
				       (expressionType *) arg3);
		break;

	case PREOP_EXPR:
		result->expressionTerm.preOpUnion =
			buildPreOpTerm((preOpKindType) arg1,
				       (expressionType *) arg2);
		break;

	case POSTOP_EXPR:
		result->expressionTerm.postOpUnion =
			buildPostOpTerm((postOpKindType) arg1,
					(expressionType *) arg2);
		break;

	case HERE_EXPR:
		result->expressionTerm.hereUnion = (hereTermType *) NULL;
		break;

	case CONDITION_CODE_EXPR:
		result->expressionTerm.conditionCodeUnion =
			(conditionCodeTermType) arg1;
		break;

	default:
		botch("invalid expression term kind: %d\n",
			kindOfExpressionTerm);
		break;
	}
	return(result);
}


/* Other stuff */

  macroTableEntryType *
buildMacroTableEntry(name)
  stringType		*name;
{
	macroTableEntryType	*result;
	char			*saveString();

	result = typeAlloc(macroTableEntryType);
	result->macroName = saveString(name);
	result->nextMacro = NULL;
	result->arguments = NULL;
	result->body = NULL;
	return(result);
}

  symbolTableEntryType *
buildSymbolTableEntry(name, usage)
  stringType		*name;
  symbolUsageKindType	 usage;
{
	symbolTableEntryType	*result;
	char			*saveString();

	result = typeAlloc(symbolTableEntryType);
	result->symbolName = saveString(name);
	result->nextSymbol = NULL;
	result->context = typeAlloc(symbolInContextType);
	result->referenceCount = 0;
	dupValue(result->context->value, UndefinedValue);
	result->context->attributes = 0;
	result->context->referenceCount = 0;
	result->context->usage = usage;
	result->context->pushedContexts = NULL;
	result->context->environmentNumber = GLOBAL_ENVIRONMENT_NUMBER;
	return(result);
}

  codeBreakType *
buildCodeBreak(kind, address, data)
  codeBreakKindType	kind;
  addressType		address;
  int			data;
{
	codeBreakType	*result;

	result = typeAlloc(codeBreakType);
	result->kindOfBreak = kind;
	result->breakAddress = address;
	result->breakData = data;
	result->nextBreak = NULL;
	return(result);
}

  reservationListType *
buildReservation(startAddress, blockSize, nextReservation)
  addressType		 startAddress;
  int			 blockSize;
  reservationListType	*nextReservation;
{
	reservationListType	*result;

	result = typeAlloc(reservationListType);
	result->startAddress = startAddress;
	result->blockSize = blockSize;
	result->nextReservation = nextReservation;
	return(result);
}

  simpleFixupListType *
buildSimpleFixupList(locationToFixup, previousList)
  valueType		 locationToFixup;
  simpleFixupListType	*previousList;
{
	simpleFixupListType	*result;

	result = typeAlloc(simpleFixupListType);
	result->locationToFixup = locationToFixup;
	result->nextFixup = previousList;
	return(result);
}
