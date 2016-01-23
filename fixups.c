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
	fixups.c -- Routines to handle expressions being saved away for fixups
			in the Macross assembler.

	Chip Morningstar -- Lucasfilm Ltd.

	20-December-1984
*/

#include "macrossTypes.h"
#include "macrossGlobals.h"

operandType		*dbOperand;	/* safe temps for dbx hacking */
expressionType		*dbExpression;
symbolTableEntryType	*dbSymbol;

stringType		*dbString = "graphics2.m";

#define nullDup(thing) if (thing==NULL) { return (NULL); }

#define expansionOff() {saveExpansion=expandMacros; expandMacros=FALSE;}
#define expansionOn()	expandMacros=saveExpansion;

/*
   Whenever a value is needed as the operand of an instruction or data
   statement, but one or more of the terms of the expression which define the
   value are themselves undefined symbols, we have to get fancy.  What we do
   is use a zero value and save away a copy of the expression for later
   evaluation at a time (the end of assembly) when (hopefully) the undefined
   symbols will have been defined.  It is necessary to save a copy of the
   expression, rather that the expression itself, since some of the terms of
   the expression may be variables whose value may change between now and
   when the saved expression gets reevaluated.  When we are making the copy
   we replace any variables with their present values.

   The routines below collectively duplicate expressions for later evaluation.
 */

  
extern void moreExpression (char *format, ...);
extern bool isUndefined (valueType *value);
extern char *tokenString (int token);
extern void error (errorType theError, ...);
extern char *conditionString (conditionType condition);
extern void botch (char *message, ...);

expressionType *
generateFixupExpression(expressionType *expression)
{
	expressionType	*result;
	expressionType	*duplicateExpressionForFixup(expressionType *expression, bool isTopLevel, bool isSpecialFunctionOperand);

	generatingFixup = TRUE;
	result = duplicateExpressionForFixup(expression, TRUE, FALSE);
	generatingFixup = FALSE;
	return(result);
}

  expressionType *
duplicateExpressionForFixup(expressionType *expression, bool isTopLevel, bool isSpecialFunctionOperand)
{
	expressionType		*result;
	expressionType		*originalResult;
	valueType		*value;
	symbolTableEntryType	*identifier;
	symbolInContextType	*context;
	operandType		*newOperand;
	environmentType		*saveEnvironment;
	bool			 saveExpansion;

	operandType		*duplicateOperandForFixup(operandListType *operand, bool isSpecialFunctionOperand);
	symbolInContextType	*getWorkingContext(symbolTableEntryType *identifier);
	functionCallTermType	*duplicateFunctionCall(functionCallTermType *functionCall);
	expressionType		*duplicateArrayReference(arrayTermType *arrayTerm);
	valueType		*evaluateIdentifier(symbolTableEntryType *identifier, bool isTopLevel, fixupKindType kindOfFixup);
	valueType		*evaluateHere(void);
	valueType		*newValue(valueKindType kindOfValue, int value, operandKindType addressMode);
	symbolTableEntryType	*generateLocalLabel(symbolTableEntryType *symbol);
	stringType		*saveString(char *s);

    nullDup(expression);
    result = originalResult = typeAlloc(expressionType);
    *result = *expression;
    switch (expression->kindOfTerm) {

    case STRING_EXPR:
	expand(moreExpression("%s", expression->expressionTerm.stringUnion));
	result->expressionTerm.stringUnion = saveString(originalResult->
		expressionTerm.stringUnion);
	break;

    case IDENTIFIER_EXPR:
        identifier = expression->expressionTerm.identifierUnion;
	if (symbName(identifier)[0] == '$')
		identifier = result->expressionTerm.identifierUnion =
			generateLocalLabel(identifier);
	context = getWorkingContext(identifier);
        if (context->usage == ARGUMENT_SYMBOL && context->value->
                kindOfValue == OPERAND_VALUE) {
            if (isSpecialFunctionOperand) {
                while (context->usage == ARGUMENT_SYMBOL && context->value->
                        kindOfValue == OPERAND_VALUE) {
                    newOperand = (operandType *)context->value->value;
                    if (newOperand->kindOfOperand == EXPRESSION_OPND &&
                            (expression = newOperand->theOperand.
                            expressionUnion) != NULL && expression->
                            kindOfTerm == IDENTIFIER_EXPR) {
                        identifier = expression->expressionTerm.
			    identifierUnion;
                        context = getWorkingContext(identifier);
                    } else {
			expand(moreExpression("*fail*"));
                        return(NULL);
                    }
                }
		expand(moreExpression("%s", symbName(identifier)));
                result->expressionTerm.identifierUnion = identifier;
            } else {
		saveEnvironment = currentEnvironment;
		currentEnvironment = currentEnvironment->previousEnvironment;
                if ((newOperand = duplicateOperandForFixup(
                        context->value->value,
                        FALSE)) == NULL) {
                    result = NULL;
                } else {
                    result->kindOfTerm = VALUE_EXPR;
                    result->expressionTerm.valueUnion =
                        newValue(OPERAND_VALUE,
                        newOperand, newOperand->
                        kindOfOperand);
                }
		currentEnvironment = saveEnvironment;
            }
        } else {
	    expansionOff();
            if(!isUndefined(value = evaluateIdentifier(identifier,
                    isTopLevel, NO_FIXUP_OK)) && value->kindOfValue !=
		    RELOCATABLE_VALUE) {
		expansionOn();
                result->kindOfTerm = VALUE_EXPR;
                result->expressionTerm.valueUnion = value;
		expand((context->usage==MVARIABLE_SYMBOL ||
			context->usage==VARIABLE_SYMBOL ||
			context->usage==MDEFINE_SYMBOL)	?
			moreExpression("0x%x", value->value) :
			moreExpression("%s", symbName(identifier)));
            } else {
		expansionOn();
		expand(moreExpression("%s", symbName(identifier)));
	    }
        }
        break;

    case FUNCTION_CALL_EXPR:
        if ((result->expressionTerm.functionCallUnion =
                duplicateFunctionCall(expression->
                expressionTerm.functionCallUnion)) == NULL)
            result = NULL;
        break;

    case NUMBER_EXPR:
	expand(moreExpression("0x%x", result->expressionTerm.numberUnion));
        /* keep unchanged */
        break;

    case SUBEXPRESSION_EXPR:
	expand(moreExpression("("));
        if ((result->expressionTerm.subexpressionUnion =
                duplicateExpressionForFixup(expression->
                expressionTerm.subexpressionUnion,
                isTopLevel, FALSE)) == NULL)
            result = NULL;
	expand(moreExpression(")"));
        break;

    case UNOP_EXPR:
	expand(moreExpression("%s", tokenString(expression->expressionTerm.
		unopUnion->unop)));
	result->expressionTerm.unopUnion = typeAlloc(unopTermType);
	result->expressionTerm.unopUnion->unop = expression->expressionTerm.
		unopUnion->unop;
        if ((result->expressionTerm.unopUnion->unopArgument =
                duplicateExpressionForFixup(expression->
                expressionTerm.unopUnion->unopArgument,
                FALSE, FALSE)) == NULL)
            result = NULL;
        break;

    case BINOP_EXPR:
	result->expressionTerm.binopUnion = typeAlloc(binopTermType);
	result->expressionTerm.binopUnion->binop = expression->expressionTerm.
		binopUnion->binop;
        if ((result->expressionTerm.binopUnion->leftArgument =
                duplicateExpressionForFixup(expression->
                expressionTerm.binopUnion->leftArgument,
                FALSE, FALSE)) == NULL) {
	    expand(moreExpression("*fail*"));
            result = NULL;
        } else {
	    expand(moreExpression("%s", tokenString(expression->
		expressionTerm.binopUnion->binop)));
	    if ((result->expressionTerm.binopUnion->rightArgument =
                    duplicateExpressionForFixup(expression->
                    expressionTerm.binopUnion->rightArgument,
                    FALSE, FALSE)) == NULL) {
                result = NULL;
		expand(moreExpression("*fail*"));
	    }
	}
        break;


    case PREOP_EXPR:
	expand(moreExpression("*fail*"));
	error(CANT_USE_PREOP_WITH_FORWARD_REFERENCE_ERROR, tokenString(
		expression->expressionTerm.preOpUnion->preOp));
        result = NULL;
        break;

    case POSTOP_EXPR:
	expand(moreExpression("*fail*"));
	error(CANT_USE_POSTOP_WITH_FORWARD_REFERENCE_ERROR, tokenString(
		expression->expressionTerm.postOpUnion->postOp));
        result = NULL;
        break;

    case HERE_EXPR:
        result->kindOfTerm = VALUE_EXPR;
        result->expressionTerm.valueUnion = evaluateHere();
	expand(moreExpression("0x%x", result->expressionTerm.valueUnion->
		value));
        break;

    case ASSIGN_EXPR:
	expand(moreExpression("*fail*"));
	error(CANT_USE_ASSIGNMENT_WITH_FORWARD_REFERENCE_ERROR);
        result = NULL;
        break;

    case CONDITION_CODE_EXPR:
        /* keep unchanged */
	expand(moreExpression("%s", conditionString(expression->
		expressionTerm.conditionCodeUnion)));
	break;

    case VALUE_EXPR:
        /* keep unchanged */
	expand(moreExpression("0x%x", expression->expressionTerm.valueUnion->
		value));
        break;

    case ARRAY_EXPR:
        result = duplicateArrayReference(expression->expressionTerm.
		arrayUnion);
	break;

    default:
        botch("duplicateExpression: funny expression kind %d\n",
                        expression->kindOfTerm);
        break;
    }
    return(result);
}

  functionCallTermType *
duplicateFunctionCall(functionCallTermType *functionCall)
{
	functionCallTermType	*result;
	operandListType        **argument;
	operandListType		*parameterList;

	operandListType		*duplicateOperandForFixup(operandListType *operand, bool isSpecialFunctionOperand);

	result = typeAlloc(functionCallTermType);
	result->functionName = functionCall->functionName;
	expand(moreExpression("%s(", symbName(functionCall->functionName)));
	result->parameters = NULL;
	parameterList = functionCall->parameters;
	argument = &(result->parameters);
	while (parameterList != NULL) {
		*argument = duplicateOperandForFixup(parameterList,
			(functionCall->functionName->context->attributes &
			 SPECIAL_FUNCTION_ATT) != 0);
		(*argument)->nextOperand = NULL;
		argument = &((*argument)->nextOperand);
		parameterList = parameterList->nextOperand;
		expand(parameterList != NULL ? moreExpression(", ") : 0);
	}
	expand(moreExpression(")"));
	return(result);
}


  expressionType *
duplicateArrayReference(arrayTermType *arrayTerm)
{
	anyOldThing	*resultThing;
	valueKindType	 kindOfResult;
	expressionType	*result;
	stringType	*charPtr;
	valueType      **valuePtr;
	environmentType	*saveEnvironment;
	bool		 saveExpansion;
	operandType	*newOperand;

	valueType	*newValue(valueKindType kindOfValue, int value, operandKindType addressMode);
	operandType	*duplicateOperandForFixup(operandListType *operand, bool isSpecialFunctionOperand);
	anyOldThing	*arrayLookup(arrayTermType *arrayTerm, valueKindType *kindOfThing);

	expansionOff();
	resultThing = arrayLookup(arrayTerm, &kindOfResult);
	expansionOn();
	if (*(valueType **)resultThing == NULL) {
		error(VALUELESS_ARRAY_ELEMENT_ERROR);
		result = NULL;
	} else if (kindOfResult == FAIL) {
		result = NULL;
	} else if (kindOfResult == STRING_VALUE) {
		charPtr = (stringType *)resultThing;
		result = typeAlloc(expressionType);
		result->kindOfTerm = VALUE_EXPR;
		result->expressionTerm.valueUnion = newValue(ABSOLUTE_VALUE,
			*charPtr, EXPRESSION_OPND);
	} else {
		valuePtr = (valueType **) resultThing;
		if ((*valuePtr)->kindOfValue == OPERAND_VALUE) {
			saveEnvironment = currentEnvironment;
			currentEnvironment = currentEnvironment->
				previousEnvironment;
			if ((newOperand = duplicateOperandForFixup((*
					valuePtr)->value, FALSE)) == NULL) {
				result = NULL;
			} else {
				result = typeAlloc(expressionType);
				result->kindOfTerm = VALUE_EXPR;
				result->expressionTerm.valueUnion = newValue(
					OPERAND_VALUE, newOperand,
					newOperand->kindOfOperand);
			}
			currentEnvironment = saveEnvironment;
		} else {
			result = typeAlloc(expressionType);
			result->kindOfTerm = VALUE_EXPR;
			dupValue(result->expressionTerm.valueUnion,*valuePtr);
		}
	}
	return(result);
}
