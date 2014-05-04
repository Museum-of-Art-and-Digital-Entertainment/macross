/*
	expressionSemantics.c -- Routines to eat up a parse tree and spit out
		code.

	Chip Morningstar -- Lucasfilm Ltd.

	12-November-1984
*/

#include "macrossTypes.h"
#include "macrossGlobals.h"
#include "y.tab.h"

operandType		*dbOperand;	/* safe temps for dbx hacking */
expressionType		*dbExpression;
symbolTableEntryType	*dbSymbol;
stringType		*dbString;

#define nullEvaluate(thing)   if (thing==NULL) return(NULL);
#define fail(err)	      if (!expressionFailed) {\
				      error(err);\
				      expressionFailed = TRUE;\
			      }
#define fail1(err,arg)	      if (!expressionFailed) {\
				      error(err,arg);\
				      expressionFailed = TRUE;\
			      }
#define fail2(err,arg1,arg2)	      if (!expressionFailed) {\
				      error(err,arg1,arg2);\
				      expressionFailed = TRUE;\
			      }

#define qfree2(a,b)	if (freeFlag) { free(a); free(b); }

#define expansionOff() {saveExpansion=expandMacros; expandMacros=FALSE;}
#define forceExpansion() {saveExpansion=expandMacros; expandMacros=TRUE;}
#define expansionOn()	expandMacros=saveExpansion;

#define moreStuff(f) (generatingFixup ? moreExpression(f) : moreText(f))
#define moreStuff1(f,x) (generatingFixup? moreExpression(f,x) : moreText(f,x))

  anyOldThing *
arrayLookup(arrayTerm, kindOfThing)
  arrayTermType	*arrayTerm;
  valueKindType	*kindOfThing;
{
	valueType		*arrayValue;
	valueType		*indexValue;
	arrayType		*array;
	int			 index;
	stringType		*string;

	valueType		*evaluateExpression();

	*kindOfThing = FAIL;
	arrayValue = evaluateExpression(arrayTerm->arrayName, NO_FIXUP);
	if (arrayValue->kindOfValue == FAIL)
		return(NULL);
	indexValue = evaluateExpression(arrayTerm->arrayIndex, NO_FIXUP);
	if (indexValue->kindOfValue == FAIL)
		return(NULL);
	if (arrayValue->kindOfValue != ARRAY_VALUE && arrayValue->kindOfValue
			!= STRING_VALUE) {
		error(ATTEMPT_TO_INDEX_NON_ARRAY_ERROR, valueKindString(
			arrayValue->kindOfValue));
		return(NULL);
	} else if (indexValue->kindOfValue != ABSOLUTE_VALUE) {
		error(NON_INTEGER_INDEX_ERROR, valueKindString(indexValue->
			kindOfValue));
		return(NULL);
	}

	index = indexValue->value;
	if (arrayValue->kindOfValue == ARRAY_VALUE) {
		array = (arrayType *)arrayValue->value;
		if (index >= array->arraySize) {
			error(ARRAY_INDEX_OUT_OF_RANGE_ERROR, index,
				array->arraySize-1);
			return(NULL);
		} else if (index < 0) {
			error(NEGATIVE_ARRAY_INDEX_ERROR);
			return(NULL);
		} else {
			*kindOfThing = ARRAY_VALUE;
			return((anyOldThing *)&((array->arrayContents)[index]));
		}
	} else {
		string = (stringType *)arrayValue->value;
		if (index >= strlen(string)) {
			error(ARRAY_INDEX_OUT_OF_RANGE_ERROR, index,
				strlen(string) - 1);
			return(NULL);
		} else if (index < 0) {
			error(NEGATIVE_ARRAY_INDEX_ERROR);
			return(NULL);
		} else {
			*kindOfThing = STRING_VALUE;
			return((anyOldThing *) &(string[index]));
		}
	}
}

  valueType *
evaluateArrayTerm(arrayTerm)
  arrayTermType	*arrayTerm;
{
	anyOldThing		*resultThing;
	valueKindType		 kindOfResult;
	stringType		*charPtr;
	valueType	      **valuePtr;
	valueType		*result;
	environmentType		*saveEnvironment;
	bool			 saveExpansion;

	valueType		*newValue();
	valueType		*evaluateOperand();

	expansionOff();
	resultThing = arrayLookup(arrayTerm, &kindOfResult);
	expansionOn();
	if (kindOfResult == STRING_VALUE) {
		charPtr = (stringType *) resultThing;
		expand(moreExpression("'%c'", *charPtr));
		return(newValue(ABSOLUTE_VALUE, *charPtr, EXPRESSION_OPND));
	} else if (kindOfResult == FAIL) {
		return(newValue(FAIL, 0, EXPRESSION_OPND));
	} else if (*(valueType **)resultThing == NULL) {
		error(VALUELESS_ARRAY_ELEMENT_ERROR);
		return(newValue(FAIL, 0, EXPRESSION_OPND));
	} else {
		valuePtr = (valueType **) resultThing;
		if ((*valuePtr)->kindOfValue == OPERAND_VALUE) {
			saveEnvironment = currentEnvironment;
			currentEnvironment = currentEnvironment->
				previousEnvironment;
			expansionOff();
			result = evaluateOperand((*valuePtr)->value);
			expansionOn();
			currentEnvironment = saveEnvironment;
		} else {
			dupValue(result, *valuePtr);
		}
		expand(result->kindOfValue==ABSOLUTE_VALUE ?
			moreExpression("0x%x", result->value) : (result->
			kindOfValue==STRING_VALUE ? moreExpression("\"%s\"",
			result->value) : 0));
		return(result);
	}
}

  valueType *
evaluateAssignmentTerm(assignmentTerm, kindOfFixup)
  binopTermType	*assignmentTerm;
  fixupKindType	 kindOfFixup;
{
	symbolTableEntryType	*targetSymbol;
	valueType		*target;
	valueType	       **targetPtr;
	anyOldThing		*targetThing;
	valueType		*object;
	valueType		*result;
	valueKindType		 kindOfTarget;
	valueKindType		 newKindOfValue;
	symbolInContextType	*targetContext;
	bool			 stringAssign;
	char			*charPtr;
	char			 objectChar;

	symbolTableEntryType	*effectiveSymbol();
	valueType		*evaluateExpressionInternally();
	valueKindType		 addValueKind();
	valueKindType		 selectValueKind();
	valueKindType		 subValueKind();
	valueKindType		 opValueKind();
	valueType		*makeUndefinedValue();
	valueType		*newValue();

    nullEvaluate(assignmentTerm);
    sideEffectFlag = TRUE;
    stringAssign = FALSE;
    if (assignmentTerm->leftArgument->kindOfTerm == IDENTIFIER_EXPR) {
	targetSymbol = effectiveSymbol(assignmentTerm->leftArgument->
	    expressionTerm.identifierUnion, &targetContext);
	if (isAssignable(targetContext)) {
	    if ((target = targetContext->value) == NULL &&
		    (assignmentKindType)assignmentTerm->binop !=
		    ASSIGN_ASSIGN) {
		error(OPERATOR_ASSIGNMENT_ON_NULL_TARGET_ERROR, tokenString(
		    assignmentTerm->binop));
		return(object);    /* wrong --needs evaluation */
	    }
	    targetPtr = &(targetContext->value);
	    expand(moreExpression("%s %s ", symbName(targetSymbol),
		assignmentString(assignmentTerm->binop)));
	} else {
	    if (targetContext->usage == DEAD_SYMBOL || targetContext->usage ==
		    UNKNOWN_SYMBOL || targetContext->usage ==
		    NESTED_UNKNOWN_SYMBOL) {
		fail1(UNDECLARED_VARIABLE_ERROR, symbName(targetSymbol));
	    } else {
		fail2(UNASSIGNABLE_SYMBOL_TYPE_ERROR, symbName(targetSymbol),
		    usageString(targetContext->usage));
	    }
	    return(newValue(FAIL, 0, EXPRESSION_OPND));
	}
    } else {
	targetThing = arrayLookup(assignmentTerm->leftArgument->
	    expressionTerm.arrayUnion, &kindOfTarget);
	if (kindOfTarget == FAIL) {
	    return(newValue(FAIL, 0, EXPRESSION_OPND));
	} else if (kindOfTarget == ARRAY_VALUE) {
	    targetPtr = (valueType **) targetThing;
	    target = *targetPtr;
	} else if (kindOfTarget == STRING_VALUE) {
	    charPtr = (char *) targetThing;
	    stringAssign = TRUE;
	}
    }

	if (isUndefined(object = evaluateExpressionInternally(assignmentTerm->
			rightArgument, FALSE, kindOfFixup, FALSE))) {
		result = makeUndefinedValue();
		result->addressMode = object->addressMode;
		return(result);
	} else if (object->kindOfValue == FAIL) {
		return(object); /* we can use it as it's a failure */
	}
	if (stringAssign) {
		if (object->kindOfValue == ABSOLUTE_VALUE) {
			objectChar = object->value & 0xFF;
		} else {
			fail(NON_CHARACTER_STRING_ASSIGNMENT_ERROR);
			return(newValue(FAIL, 0, EXPRESSION_OPND));
		}
	}

	switch (assignmentTerm->binop) {

	case ASSIGN_ASSIGN:
		if (stringAssign) {
			*charPtr = objectChar;
		} else if (target == NULL) {
			*targetPtr = object;
			dupValue(result, object);
			return(result);
		} else {
			newKindOfValue = object->kindOfValue;
			target->value = object->value;
		}
		break;

	case ADD_ASSIGN:
		if (stringAssign)
			*charPtr += objectChar;
		else if ((newKindOfValue = addValueKind(target, object)) !=
				FAIL)
			target->value += object->value;
		break;

	case AND_ASSIGN:
		if (stringAssign)
			*charPtr &= objectChar;
		else if ((newKindOfValue = opValueKind(target, object)) !=
				FAIL)
			target->value &= object->value;
		break;

	case DIV_ASSIGN:
		if (stringAssign)
			*charPtr /= objectChar;
		else if ((newKindOfValue = opValueKind(target, object)) !=
				FAIL)
			target->value /= object->value;
		break;

	case LEFT_SHIFT_ASSIGN:
		if (stringAssign)
			*charPtr <<= objectChar;
		else if ((newKindOfValue = opValueKind(target, object)) !=
				FAIL)
			target->value <<= object->value;
		break;

	case MOD_ASSIGN:
		if (stringAssign)
			*charPtr %= objectChar;
		else if ((newKindOfValue = opValueKind(target, object)) !=
				FAIL)
			target->value %= object->value;
		break;

	case MUL_ASSIGN:
		if (stringAssign)
			*charPtr *= objectChar;
		else if ((newKindOfValue = opValueKind(target, object)) !=
				FAIL)
			target->value *= object->value;
		break;

	case OR_ASSIGN:
		if (stringAssign)
			*charPtr |= objectChar;
		else if ((newKindOfValue = opValueKind(target, object)) !=
				FAIL)
			target->value |= object->value;
		break;

	case RIGHT_SHIFT_ASSIGN:
		if (stringAssign)
			*charPtr >>= objectChar;
		else if ((newKindOfValue = opValueKind(target, object)) !=
				FAIL)
			target->value >>= object->value;
		break;

	case SUB_ASSIGN:
		if (stringAssign)
			*charPtr -= objectChar;
		else if ((newKindOfValue = subValueKind(target, object)) !=
				FAIL)
			target->value -= object->value;
		break;

	case XOR_ASSIGN:
		if (stringAssign)
			*charPtr ^= objectChar;
		else if ((newKindOfValue = opValueKind(target, object)) !=
				FAIL)
			target->value ^= object->value;
		break;

	case NO_ASSIGN:
		botch("try to assign with NO_ASSIGN\n");
		break;

	default:
		botch("bad assignment kind\n");
		break;
	}
	if (newKindOfValue == FAIL) {
		fail1(INCOMPATIBLE_ASSIGNMENT_OPERANDS_ERROR,
			assignmentString(assignmentTerm->binop));
		target->value = 0;
	}
	if (stringAssign) {
		result = newValue(ABSOLUTE_VALUE, *charPtr, EXPRESSION_OPND);
	} else {
		target->kindOfValue = newKindOfValue;
		dupValue(result, target);
	}
	return(result);
}

  valueType *
evaluateBinopTerm(binopTerm, isTopLevel, kindOfFixup)
  binopTermType	*binopTerm;
  bool		 isTopLevel;
  fixupKindType	 kindOfFixup;
{
	valueType		*leftOperand;
	valueType		*rightOperand;
	symbolTableEntryType	*rightSelection;
	valueKindType		 resultKindOfValue;
	int			 resultValue;
	operandKindType		 resultAddressMode;
	valueType		*result;

	symbolInContextType	*getWorkingContext();
	valueType		*evaluateExpressionInternally();
	valueType		*newValue();
	valueType		*makeUndefinedValue();

	nullEvaluate(binopTerm);
	if (binopTerm->binop != SUB && binopTerm->binop != ADD)
		isTopLevel = FALSE;
	leftOperand = evaluateExpressionInternally(binopTerm->leftArgument,
		isTopLevel, kindOfFixup, FALSE);
	expand(moreExpression("%s", tokenString(binopTerm->binop)));
	if (binopTerm->binop != ADD)
		isTopLevel = FALSE;
	if (binopTerm->binop == SELECT) {
		expand(moreExpression("%s", symbName((symbolTableEntryType *)
			binopTerm->rightArgument)));
		dupValue(rightOperand, getWorkingContext(
			(symbolTableEntryType *)(binopTerm->rightArgument))->
			value);
	} else {
		rightOperand = evaluateExpressionInternally(binopTerm->
			rightArgument, FALSE, kindOfFixup, FALSE);
	}
	if (isLogicalOp(binopTerm->binop)) {
		resultAddressMode = EXPRESSION_OPND;
	} else {
		resultAddressMode = leftOperand->addressMode;
		if (leftOperand->addressMode!=EXPRESSION_OPND &&
				rightOperand->addressMode!=EXPRESSION_OPND) {
			error(MULTIPLE_ADDRESS_MODES_ERROR);
			qfree2(leftOperand, rightOperand);
			return(newValue(FAIL, 0, EXPRESSION_OPND));
		} else if (rightOperand->addressMode != EXPRESSION_OPND) {
			resultAddressMode = rightOperand->addressMode;
		}
	}
	if ((isUndefined(leftOperand) && !isFailure(rightOperand)) ||
		     (isUndefined(rightOperand) && !isFailure(leftOperand))) {
		qfree2(leftOperand, rightOperand);
		result = makeUndefinedValue();
		result->addressMode = resultAddressMode;
		return(result);
	}

	switch (binopTerm->binop) {

	case ADD:
		if ((resultKindOfValue = addValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = leftOperand->value +
							rightOperand->value;
		break;

	case ASSIGN:
		botch("assignment leaked thru to binop\n");
		break;

	case BITWISE_AND:
		if ((resultKindOfValue = opValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = leftOperand->value &
							rightOperand->value;
		break;

	case BITWISE_OR:
		if ((resultKindOfValue = opValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = leftOperand->value |
							rightOperand->value;
		break;

	case BITWISE_XOR:
		if ((resultKindOfValue = opValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = leftOperand->value ^
							rightOperand->value;
		break;

	case DIV:
		if ((resultKindOfValue = opValueKind(leftOperand,
						rightOperand)) != FAIL)
			if (rightOperand->value == 0) {
				fail(DIVIDE_BY_ZERO_ERROR);
				resultKindOfValue = FAIL;
			} else {
				resultValue = leftOperand->value /
							rightOperand->value;
			}
		break;

	case EQUAL_TO:
		if ((resultKindOfValue = opValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = leftOperand->value ==
							rightOperand->value;
		break;

	case GREATER_THAN:
		if ((resultKindOfValue = opValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = leftOperand->value >
							rightOperand->value;
		break;

	case GREATER_THAN_OR_EQUAL_TO:
		if ((resultKindOfValue = opValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = leftOperand->value >=
							rightOperand->value;
		break;

	case LEFT_SHIFT:
		if ((resultKindOfValue = opValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = leftOperand->value <<
							rightOperand->value;
		break;

	case LESS_THAN:
		if ((resultKindOfValue = opValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = leftOperand->value <
							rightOperand->value;
		break;

	case LESS_THAN_OR_EQUAL_TO:
		if ((resultKindOfValue = opValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = leftOperand->value <=
							rightOperand->value;
		break;

	case LOGICAL_AND:
		if ((resultKindOfValue = opValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = leftOperand->value &&
							rightOperand->value;
		break;

	case LOGICAL_OR:
		if ((resultKindOfValue = opValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = leftOperand->value ||
							rightOperand->value;
		break;

	case LOGICAL_XOR:
		if ((resultKindOfValue = opValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = logicalXOR(leftOperand->value,
							rightOperand->value);
		break;

	case MOD:
		if ((resultKindOfValue = opValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = leftOperand->value %
							rightOperand->value;
		break;

	case MUL:
		if ((resultKindOfValue = opValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = leftOperand->value *
							rightOperand->value;
		break;

	case NOT_EQUAL_TO:
		if ((resultKindOfValue = opValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = leftOperand->value !=
							rightOperand->value;
		break;

	case RIGHT_SHIFT:
		if ((resultKindOfValue = opValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = leftOperand->value >>
							rightOperand->value;
		break;

	case SELECT:
		if ((resultKindOfValue = selectValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = leftOperand->value +
							rightOperand->value;
		break;

	case SUB:
		if ((resultKindOfValue = subValueKind(leftOperand,
						rightOperand)) != FAIL)
			resultValue = leftOperand->value -
							rightOperand->value;
		break;

	default:
		botch("bad binop kind\n");
		break;
	}
	if (resultKindOfValue == FAIL) {
		resultValue = 0;
		fail1(INCOMPATIBLE_BINOP_OPERANDS_ERROR,
			tokenString(binopTerm->binop));
	}
	qfree2(leftOperand, rightOperand);
	return(newValue(resultKindOfValue, resultValue, resultAddressMode));
}

  valueType *
evaluateCondition(condition)
  conditionType	condition;
{
	valueType	*newValue();

	expand(moreExpression("%s", conditionString(condition)));
	return(newValue(CONDITION_VALUE, condition, EXPRESSION_OPND));
}

  valueType *
evaluateBuiltInFunctionCall(workingContext, parameters, kindOfFixup)
  symbolInContextType	*workingContext;
  operandListType	*parameters;
  fixupKindType		 kindOfFixup;
{
	sideEffectFlag = TRUE;
	return((*builtInFunctionTable[workingContext->value->value].
		functionEntry)(parameters, kindOfFixup));
}

  valueType *
evaluateFunctionCall(functionCall, kindOfFixup, isStandalone)
  functionCallTermType	*functionCall;
  fixupKindType		 kindOfFixup;
  bool			 isStandalone;
{
	functionDefinitionType	*theFunction;
	int			 numberBound;
	identifierListType	*savedLocalVariableList;
	int			 savedLabelTagNumber;
	stringType		*savedFunctionName;
	bool			 savedErrorFlag;
	bool			 savedBeneathFunction;
	symbolInContextType	*workingContext;
	environmentType		 newEnvironment;
	valueType		*result;
	bool			 saveExpansion;

	symbolInContextType	*getWorkingContext();
	valueType		*evaluateBuiltInFunctionCall();
	valueType		*newValue();

	nullEvaluate(functionCall);
	sideEffectFlag = TRUE;
	pushEnvironment(newEnvironment);
	resultOfLastFunctionCall = NULL;
	freturnExit = FALSE;
	workingContext = getWorkingContext(functionCall->functionName);
	expand(moreExpression("%s(", symbName(functionCall->functionName)));
	if (isBuiltInFunction(workingContext)) {
		popEnvironment();
		result = evaluateBuiltInFunctionCall(workingContext,
			functionCall->parameters, kindOfFixup);
		expand(moreExpression(")"));
		return(result);
	} if (isFunction(workingContext)) {
		theFunction = (functionDefinitionType *)workingContext->
			value->value;
	} else {
		error(NOT_A_FUNCTION_ERROR, symbName(functionCall->
			functionName));
		popEnvironment();
		return(newValue(FAIL, 0, EXPRESSION_OPND));
	}
	savedLocalVariableList = currentLocalVariableList;
	currentLocalVariableList = NULL;
	savedLabelTagNumber = currentLabelTagNumber;
	savedFunctionName = currentFunctionName;
	currentFunctionName = symbName(theFunction->functionName);
	currentLabelTagNumber = nextLabelTagNumber++;
	savedErrorFlag = errorFlag;
	savedBeneathFunction = beneathFunction;
	errorFlag = FALSE;
	if ((numberBound = bindFunctionArguments(theFunction->arguments,
				functionCall->parameters, symbName(
				theFunction->functionName))) > 0) {
		expansionOff();
		beneathFunction = TRUE;
		assembleBlock(theFunction->body);
		freturnExit = FALSE;
		expansionOn();
	} else
		numberBound = -numberBound;
	if (numberBound > 1)
		unbindArguments(theFunction->arguments, numberBound-1);
	unbindLocalVariables(currentLocalVariableList);
	beneathFunction = savedBeneathFunction;
	expand(moreExpression(")"));
	currentLabelTagNumber = savedLabelTagNumber;
	currentLocalVariableList = savedLocalVariableList;
	currentFunctionName = savedFunctionName;
	if (errorFlag)
		error(ERROR_INSIDE_FUNCTION_ERROR, symbName(theFunction->
			functionName));
	else
		errorFlag = savedErrorFlag;
	popEnvironment();
	if (resultOfLastFunctionCall != NULL) {
		return(resultOfLastFunctionCall);
	} else {
		if (!isStandalone)
			error(FUNCTION_DID_NOT_RETURN_A_VALUE_ERROR, symbName(
				theFunction->functionName));
		return(newValue(FAIL, 0, EXPRESSION_OPND));
	}
}

  valueType *
evaluateHere()
{
	valueType	*newValue();
	valueType	*result;

	expand(moreExpression("here"));
	result = newValue(currentLocationCounter.kindOfValue,
		currentLocationCounter.value, EXPRESSION_OPND);
	result->value -= targetOffset;
	return(result);
}

  valueType *
evaluateIdentifier(identifier, isTopLevel, kindOfFixup)
  symbolTableEntryType	*identifier;
  bool			 isTopLevel;
  fixupKindType		 kindOfFixup;
{
	symbolInContextType	*workingContext;
	valueType		*resultValue;
	valueType		*result;
	environmentType		*saveEnvironment;
	bool			 saveExpansion;

	valueType		*newValue();
	valueType		*evaluateOperand();
	symbolInContextType	*getWorkingContext();
	symbolTableEntryType	*generateLocalLabel();

	nullEvaluate(identifier);
	identifier->referenceCount++;
	if (symbName(identifier)[0] == '$')
		identifier = generateLocalLabel(identifier);
	if ((workingContext = getWorkingContext(identifier)) == NULL) {
		fail1(UNDEFINED_SYMBOL_ERROR, symbName(identifier));
		expand(moreExpression("%s", symbName(identifier)));
		return(newValue(FAIL, 0, EXPRESSION_OPND));
	}
	if (workingContext->usage == DEAD_SYMBOL) {
		reincarnateSymbol(workingContext, UNKNOWN_SYMBOL);
	}
	if (workingContext->usage == FUNCTION_SYMBOL || workingContext->usage
			== BUILT_IN_FUNCTION_SYMBOL) {
		fail1(FUNCTION_IS_NOT_A_VALUE_ERROR, symbName(identifier));
		expand(moreExpression("%s", symbName(identifier)));
		return(newValue(FAIL, 0, EXPRESSION_OPND));
	}
	if (workingContext->value == NULL) {
		if (kindOfFixup == NO_FIXUP) {
			fail1(UNDEFINED_SYMBOL_ERROR, symbName(identifier));
			expand(moreExpression("%s", symbName(identifier)));
			return(newValue(FAIL, 0, EXPRESSION_OPND));
		} else if (kindOfFixup != NO_FIXUP_OK) {
			expand(moreExpression("%s", symbName(identifier)));
			return(newValue(FAIL, 0, EXPRESSION_OPND));
		}
	} else {
		resultValue = workingContext->value;
		if (workingContext->value->kindOfValue == UNDEFINED_VALUE){
			if (kindOfFixup==NO_FIXUP ||
			    (workingContext->usage!=LABEL_SYMBOL &&
			     workingContext->usage!=EXTERNAL_SYMBOL &&
			     workingContext->usage!=UNKNOWN_SYMBOL &&
			     workingContext->usage!=NESTED_UNKNOWN_SYMBOL)) {
				expand(moreExpression("%s", symbName(
					identifier)));
				if ((workingContext->attributes & GLOBAL_ATT)
						!=0 && performingFixups) {
					return(newValue(UNDEFINED_VALUE, 0,
						resultValue->addressMode));
				} else {
					fail1(UNDEFINED_SYMBOL_ERROR,
						symbName(identifier));
					return(newValue(FAIL, 0,
						EXPRESSION_OPND));
				}
			} else if (kindOfFixup == NO_FIXUP_OK) {
				expand(moreExpression("%s", symbName(
					identifier)));
				return(newValue(UNDEFINED_VALUE, 0,
					EXPRESSION_OPND));
			}
		} else if (workingContext->value->kindOfValue == FAIL) {
			fail1(UNASSIGNED_SYMBOL_ERROR, symbName(identifier));
			expand(moreExpression("%s", symbName(identifier)));
			return(newValue(FAIL, 0, EXPRESSION_OPND));
		}
		if (workingContext->value->kindOfValue == OPERAND_VALUE) {
			saveEnvironment = currentEnvironment;
			if (workingContext->usage == ARGUMENT_SYMBOL) {
				currentEnvironment = currentEnvironment->
					previousEnvironment;
				expand(shouldParenthesize(workingContext->
					value->value) ? moreExpression("(") :
					0);
			} else {
				expand(moreExpression("%s",
					symbName(identifier)));
				expansionOff();
			}
			resultValue = evaluateOperand(workingContext->
				value->value);
			if (workingContext->usage == ARGUMENT_SYMBOL) {
				expand(shouldParenthesize(workingContext->
					value->value) ? moreExpression(")") :
				0);
			} else {
				expansionOn();
			}
			currentEnvironment = saveEnvironment;
		} else if (workingContext->value->kindOfValue == BLOCK_VALUE){
			sideEffectFlag = TRUE;
			saveEnvironment = currentEnvironment;
			currentEnvironment = currentEnvironment->
				previousEnvironment;
			expand((moreText("{"), endLine(), tabCount++));
			assembleBlock(workingContext->value->value);
			expand((tabCount--, tabIndent(), moreText("}"),
				endLine()));
			currentEnvironment = saveEnvironment;
			return(newValue(UNDEFINED_VALUE, 0, EXPRESSION_OPND));
		} else if (workingContext->usage==MVARIABLE_SYMBOL ||
				workingContext->usage==VARIABLE_SYMBOL ||
				workingContext->usage==MDEFINE_SYMBOL) {
			expand(workingContext->value->kindOfValue==
				ABSOLUTE_VALUE ? moreExpression("0x%x",
				workingContext->value->value) : (
				workingContext->value->kindOfValue==
				STRING_VALUE ? moreExpression("\"%s\"",
				workingContext->value->value) : 0));
		} else {
			expand(moreExpression("%s", symbName(identifier)));
		}

		dupValue(result, resultValue);
		if (result->kindOfValue != UNDEFINED_VALUE)
			workingContext->referenceCount += 1;
		if (result->kindOfValue==RELOCATABLE_VALUE && (kindOfFixup!=
				NO_FIXUP || performingFixups))
			result->value = 0; /* it'll be relocated */
		return(result);
	}
}

  valueType *
evaluateNumber(number)
  numberTermType	number;
{
	valueType	*newValue();

	expand(moreExpression("0x%x", number));
	return(newValue(ABSOLUTE_VALUE, number, EXPRESSION_OPND));
}

  valueType *
evaluatePostopTerm(postopTerm)
  postOpTermType	*postopTerm;
{
	valueType		*theOperand;
	valueType	       **theOperandPtr;
	valueKindType		 kindOfOperand;
	valueType		*result;
	symbolInContextType	*workingContext;
	symbolTableEntryType	*targetSymbol;

	symbolTableEntryType	*effectiveSymbol();
	symbolInContextType	*getWorkingContext();

	nullEvaluate(postopTerm);
	sideEffectFlag = TRUE;
	if (postopTerm->postOpArgument->kindOfTerm == IDENTIFIER_EXPR) {
		targetSymbol = effectiveSymbol(postopTerm->postOpArgument->
			expressionTerm.identifierUnion, &workingContext);
		if (workingContext != NULL)
			theOperand = workingContext->value;
		else
			theOperand = NULL;
	} else {
		theOperandPtr = (valueType **) arrayLookup(postopTerm->
			postOpArgument->expressionTerm.arrayUnion,
			&kindOfOperand);
		if (kindOfOperand == FAIL)
			theOperand = NULL;
		else if (kindOfOperand == ARRAY_VALUE)
			theOperand = *theOperandPtr;
		else {
			fail(CANT_POSTPREOP_STRING_ERROR);
			return(newValue(FAIL, 0, EXPRESSION_OPND));
		}
	}

	switch (postopTerm->postOp) {

	case INCREMENT:
		if (incrementable(theOperand)) {
			expand(moreExpression("%s++",symbName(targetSymbol)));
			dupValue(result, theOperand);
			theOperand->value += 1;
			return(result);
		} else {
			fail1(UNPOSTINCREMENTABLE_OPERAND_ERROR,
				valueKindString(theOperand->kindOfValue));
			return(newValue(FAIL, 0, EXPRESSION_OPND));
		}
		break;

	case DECREMENT:
		if (decrementable(theOperand)) {
			expand(moreExpression("%s--",symbName(targetSymbol)));
			dupValue(result, theOperand);
			theOperand->value -= 1;
			return(result);
		} else {
			fail1(UNPOSTDECREMENTABLE_OPERAND_ERROR,
				valueKindString(theOperand->kindOfValue));
			return(newValue(FAIL, 0, EXPRESSION_OPND));
		}
		break;

	default:
		botch("bad postop type\n");
		break;
	}
}

  valueType *
evaluatePreopTerm(preopTerm)
  preOpTermType	*preopTerm;
{
	valueType		*theOperand;
	valueType		*result;
	valueType	       **theOperandPtr;
	valueKindType		 kindOfOperand;
	symbolInContextType	*workingContext;
	symbolTableEntryType	*targetSymbol;

	symbolTableEntryType	*effectiveSymbol();
	symbolInContextType	*getWorkingContext();

	nullEvaluate(preopTerm);
	sideEffectFlag = TRUE;
	if (preopTerm->preOpArgument->kindOfTerm == IDENTIFIER_EXPR) {
		targetSymbol = effectiveSymbol(preopTerm->preOpArgument->
			expressionTerm.identifierUnion, &workingContext);
		if (workingContext != NULL)
			theOperand = workingContext->value;
		else
			theOperand = NULL;
	} else {
		theOperandPtr = (valueType **) arrayLookup(preopTerm->
			preOpArgument->expressionTerm.arrayUnion,
			&kindOfOperand);
		if (kindOfOperand == FAIL)
			theOperand = NULL;
		else if (kindOfOperand == ARRAY_VALUE)
			theOperand = *theOperandPtr;
		else {
			fail(CANT_POSTPREOP_STRING_ERROR);
			return(newValue(FAIL, 0, EXPRESSION_OPND));
		}
	}

	switch (preopTerm->preOp) {

	case INCREMENT:
		if (incrementable(theOperand)) {
			expand(moreExpression("%++s",symbName(targetSymbol)));
			theOperand->value += 1;
			dupValue(result, theOperand);
			return(result);
		} else {
			fail1(UNPREINCREMENTABLE_OPERAND_ERROR,
				valueKindString(theOperand->kindOfValue));
			return(newValue(FAIL, 0, EXPRESSION_OPND));
		}
		break;

	case DECREMENT:
		if (decrementable(theOperand)) {
			expand(moreExpression("%--s",symbName(targetSymbol)));
			theOperand->value -= 1;
			dupValue(result, theOperand);
			return(result);
		} else {
			fail1(UNPREDECREMENTABLE_OPERAND_ERROR,
				valueKindString(theOperand->kindOfValue));
			return(newValue(FAIL, 0, EXPRESSION_OPND));
		}
		break;

	default:
		botch("bad preop type\n");
		break;
	}
}

  valueType *
evaluateString(string)
  stringType	*string;
{
	valueType	*newValue();
	stringType	*newString;

	expand(moreExpression("\"%s\"", string));
	newString = (stringType *)malloc(strlen(string)+1);
	strcpy(newString, string);
	return(newValue(STRING_VALUE, newString, EXPRESSION_OPND));
}

  valueType *
evaluateUnopTerm(unopTerm, kindOfFixup)
  unopTermType	*unopTerm;
  fixupKindType	 kindOfFixup;
{
	valueType	*theOperand;
	valueKindType	 resultKindOfValue;
	int		 resultValue;
	operandKindType	 resultAddressMode;
	valueType	*result;

	valueKindType	 unopValueKind();
	valueType	*newValue();
	valueType	*evaluateExpressionInternally();
	valueType	*makeUndefinedValue();

	nullEvaluate(unopTerm);
	expand(moreExpression("%s", tokenString(unopTerm->unop)));
	theOperand = evaluateExpressionInternally(unopTerm->unopArgument,
		FALSE, kindOfFixup, FALSE);
	resultAddressMode = theOperand->addressMode;
	if (isUndefined(theOperand)) {
		qfree(theOperand);
		result = makeUndefinedValue();
		result->addressMode = resultAddressMode;
		return(result);
	}
	if ((resultKindOfValue = unopValueKind(theOperand)) != FAIL) {
		switch (unopTerm->unop) {

		case BITWISE_NOT:
			resultValue = ~theOperand->value;
			break;

		case HI_BYTE:
			resultValue = (theOperand->value>>8) & 0xFF;
			break;

		case LO_BYTE:
			resultValue = theOperand->value & 0xFF;
			break;

		case LOGICAL_NOT:
			resultValue = !theOperand->value;
			break;

		case UNARY_MINUS:
			resultValue = -theOperand->value;
			break;
		}
	} else {
		fail1(INCOMPATIBLE_UNOP_OPERAND_ERROR, tokenString(unopTerm->
			unop));
		resultValue = 0;
	}
	qfree(theOperand);
	return(newValue(resultKindOfValue, resultValue, resultAddressMode));
}

  valueType *
evaluateExpressionInternally(expression, isTopLevel, kindOfFixup,isStandalone)
  expressionType	*expression;
  bool			 isTopLevel;
  fixupKindType		 kindOfFixup;
  bool			 isStandalone;
{
	valueType	*result;

	nullEvaluate(expression);
	switch (expression->kindOfTerm) {

	case ARRAY_EXPR:
		return(evaluateArrayTerm(expression->expressionTerm));
		break;

	case ASSIGN_EXPR:
		return(evaluateAssignmentTerm(expression->expressionTerm,
			kindOfFixup));
		break;

	case BINOP_EXPR:
		return(evaluateBinopTerm(expression->expressionTerm,
			isTopLevel, kindOfFixup));
		break;

	case CONDITION_CODE_EXPR:
		return(evaluateCondition(expression->expressionTerm));
		break;

	case FUNCTION_CALL_EXPR:
		return(evaluateFunctionCall(expression->expressionTerm,
			kindOfFixup, isStandalone));
		break;

	case HERE_EXPR:
		return(evaluateHere());
		break;

	case IDENTIFIER_EXPR:
		result = evaluateIdentifier(expression->expressionTerm,
			isTopLevel, kindOfFixup);
		return(result);
		break;

	case NUMBER_EXPR:
		return(evaluateNumber(expression->expressionTerm));
		break;

	case POSTOP_EXPR:
		return(evaluatePostopTerm(expression->expressionTerm));
		break;

	case PREOP_EXPR:
		return(evaluatePreopTerm(expression->expressionTerm));
		break;

	case SUBEXPRESSION_EXPR:
		expand(moreExpression("("));
		result = evaluateExpressionInternally(expression->
			expressionTerm, isTopLevel, kindOfFixup, FALSE);
		expand(moreExpression(")"));
		return(result);
		break;

	case STRING_EXPR:
		return(evaluateString(expression->expressionTerm));
		break;

	case UNOP_EXPR:
		return(evaluateUnopTerm(expression->expressionTerm,
			kindOfFixup));
		break;

	case VALUE_EXPR:
		if (expression->expressionTerm.valueUnion->kindOfValue ==
				OPERAND_VALUE) {
			return(evaluateOperand(expression->expressionTerm.
				valueUnion->value));
		} else {
			dupValue(result, expression->expressionTerm.
				valueUnion);
			expand(moreExpression("0x%x", result->value));
			return(result);
		}
		break;

	default:
		botch("evaluateExpression: funny expression kind %d\n",
			expression->kindOfTerm);
		break;
	}
}

  valueType *
evaluateExpression(expression, kindOfFixup)
  expressionType	*expression;
  fixupKindType		 kindOfFixup;
{
	valueType	*result;

	expressionFailed = FALSE;
	referencesToNote[currentOperandNumber] = NULL;
	result = evaluateExpressionInternally(expression, TRUE, kindOfFixup,
		FALSE);
	if (result->kindOfValue == RELOCATABLE_VALUE) {
		result->kindOfValue = UNDEFINED_VALUE;
		result->value = 0; /* fix linker bug */
	}
	if (isUndefined(result)) {
		flushExpressionString();
		pendingFixup[currentOperandNumber] = expression;
	} else
		pendingFixup[currentOperandNumber] = NULL;
	return(result);
}

  void
evaluateExpressionStandalone(expression)
  expressionType	*expression;
{
	bool		 saveExpansion;
	valueType	*expressionResult;

	expressionFailed = FALSE;
	referencesToNote[currentOperandNumber] = NULL;

	standaloneExpansionFlag = amExpanding();
	expansionOff();
	expressionResult = evaluateExpressionInternally(expression, TRUE,
		NO_FIXUP, TRUE);
	expansionOn();
	standaloneExpansionFlag = FALSE;
	if (expressionResult != NULL)
		qfree(expressionResult);
	pendingFixup[0] = NULL;
}

  valueType *
evaluateDefineExpression(expression)
  expressionType	*expression;
{
	valueType	*newValue();
	expressionType	*generateFixupExpression();
	operandType	*buildOperand();

	nullEvaluate(expression);
	return(newValue(OPERAND_VALUE, buildOperand(EXPRESSION_OPND,
		generateFixupExpression(expression)), EXPRESSION_OPND));
}

  valueType *
evaluateSelectionList(selectionList)
  selectionListType	*selectionList;
{
	int		 offset;
	valueType	*newValue();

	offset = 0;
	while (selectionList != NULL) {
		expand(moreExpression(".%s", symbName(selectionList->
			theSymbol)));
		offset += fieldValue(selectionList->theSymbol);
		selectionList = selectionList->nextIdentifier;
	}
	return(newValue(ABSOLUTE_VALUE, offset, EXPRESSION_OPND));
}
