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
	operandStuff_6502.c -- Various target processor routines to handle
			operands in the Macross assembler (6502 version).

	Chip Morningstar -- Lucasfilm Ltd.

	23-April-1985
*/

#include "macrossTypes.h"
#include "macrossGlobals.h"

/* corresponds to routines in buildStuff2.c */

  
extern void botch (char *message, ...);
extern void error (errorType theError, ...);
extern void freeExpression (expressionType *expression);
extern void freeSelectionList (selectionListType *selectionList);
extern void freeString (stringType *string);
extern void freeBlock (blockType *block);
extern void moreText (char *format, ...);
extern void expandExpression (char *toBuffer, char **toBufferPtr);
extern void assembleBlock (blockType *block);

operandType *
buildOperand(operandKindType kindOfOperand, anyOldThing *arg)
{
	operandType	*result;

	result = typeAlloc(operandType);
	result->kindOfOperand = kindOfOperand;
	result->nextOperand = NULL;
	switch (kindOfOperand) {

	case EXPRESSION_OPND:
		result->theOperand.expressionUnion =
			(expressionOperandBodyType *) arg;
		break;

	case IMMEDIATE_OPND:
		result->theOperand.immediateUnion = 
			(immediateOperandBodyType *) arg;
		break;

	case INDIRECT_OPND:
		result->theOperand.indirectUnion =
			(indirectOperandBodyType *) arg;
		break;

	case A_REGISTER_OPND:
		result->theOperand.aRegisterUnion =
			(aRegisterOperandBodyType *) NULL;
		break;

	case X_REGISTER_OPND:
		result->theOperand.xRegisterUnion =
			(xRegisterOperandBodyType *) NULL;
		break;

	case Y_REGISTER_OPND:
		result->theOperand.yRegisterUnion =
			(yRegisterOperandBodyType *) NULL;
		break;

	case POST_INDEXED_Y_OPND:
		result->theOperand.postIndexedYUnion =
			(postIndexedYOperandBodyType *) arg;
		break;

	case PRE_INDEXED_X_OPND:
		result->theOperand.preIndexedXUnion =
			(preIndexedXOperandBodyType *) arg;
		break;

	case X_INDEXED_OPND:
		result->theOperand.xIndexedUnion =
			(xIndexedOperandBodyType *) arg;
		break;

	case Y_INDEXED_OPND:
		result->theOperand.yIndexedUnion =
			(yIndexedOperandBodyType *) arg;
		break;

	case X_SELECTED_OPND:
		result->theOperand.xSelectedUnion =
			(xSelectedOperandBodyType *) arg;
		break;

	case Y_SELECTED_OPND:
		result->theOperand.ySelectedUnion =
			(ySelectedOperandBodyType *) arg;
		break;

	case PRE_SELECTED_X_OPND:
		result->theOperand.preSelectedUnion =
			(preSelectedOperandBodyType *) arg;
		break;

	case STRING_OPND:
		result->theOperand.stringUnion =
			(stringOperandBodyType *) arg;
		break;

	case BLOCK_OPND:
		result->theOperand.blockUnion = (blockOperandBodyType *) arg;
		break;

	default:
		botch("unknown operand kind: %d\n", kindOfOperand);
		break;
	}
	return(result);
}

/* corresponds to routines in fixups.c */

  operandListType *
duplicateOperandForFixup(operandListType *operand, bool isSpecialFunctionOperand)
{
	operandListType	*result;

	expressionType	*duplicateExpressionForFixup(expressionType *expression, bool isTopLevel, bool isSpecialFunctionOperand);

	result = typeAlloc(operandListType);
	result->kindOfOperand = operand->kindOfOperand;
	result->nextOperand = NULL;
	if (operand->kindOfOperand != EXPRESSION_OPND)
		newFixupAddressMode = operand->kindOfOperand;
	switch (operand->kindOfOperand) {
	case EXPRESSION_OPND:
	case IMMEDIATE_OPND:
	case INDIRECT_OPND:
	case POST_INDEXED_Y_OPND:
	case PRE_INDEXED_X_OPND:
	case X_INDEXED_OPND:
	case Y_INDEXED_OPND:
		result->theOperand.expressionUnion =
			duplicateExpressionForFixup(operand->theOperand.expressionUnion,
			FALSE, isSpecialFunctionOperand);
		break;
	case X_SELECTED_OPND:
	case Y_SELECTED_OPND:
	case PRE_SELECTED_X_OPND:
		result->theOperand.expressionUnion =
			duplicateExpressionForFixup(operand->theOperand.xSelectedUnion,
			FALSE, isSpecialFunctionOperand);
		break;
	case A_REGISTER_OPND:
	case X_REGISTER_OPND:
	case Y_REGISTER_OPND:
		result->theOperand = operand->theOperand;
		break;
	case STRING_OPND:
		result->theOperand = operand->theOperand;
		break;
	case BLOCK_OPND:
		error(CANT_FORWARD_REFERENCE_BLOCK_ERROR);
		result = NULL;
		break;
	}
	return(result);
}

/* corresponds to routines in garbage.c */

#define nullFree(thing) if (thing == NULL) return;

  void
freeOperand(operandType *operand)
{
	nullFree(operand);
	switch (operand->kindOfOperand) {

	case EXPRESSION_OPND:
	case IMMEDIATE_OPND:
	case INDIRECT_OPND:
	case POST_INDEXED_Y_OPND:
	case PRE_INDEXED_X_OPND:
	case X_INDEXED_OPND:
	case Y_INDEXED_OPND:
		freeExpression(operand->theOperand.expressionUnion);
		break;

	case A_REGISTER_OPND:
	case X_REGISTER_OPND:
	case Y_REGISTER_OPND:
		break;

	case X_SELECTED_OPND:
	case Y_SELECTED_OPND:
	case PRE_SELECTED_X_OPND:
		freeSelectionList(operand->theOperand.xSelectedUnion);
		break;

	case STRING_OPND:
		freeString(operand->theOperand.stringUnion);
		break;

	case BLOCK_OPND:
		freeBlock(operand->theOperand.blockUnion);
		break;

	default:
		botch("bad operand kind in freeOperand %d\n",
						operand->kindOfOperand);
		break;
	}
	freeOperand(operand->nextOperand);
	free(operand);
}

/* corresponds to routines in listing.c */

  void
expandOperand(operandKindType addressMode, char *buffer)
{
	switch (addressMode) {
	case IMMEDIATE_OPND:		moreText("#"); break;
	case INDIRECT_OPND:		moreText("@"); break;
	case POST_INDEXED_Y_OPND:	moreText("y[@"); break;
	case PRE_INDEXED_X_OPND:	moreText("@x["); break;
	case X_INDEXED_OPND:		moreText("x["); break;
	case Y_INDEXED_OPND:		moreText("y["); break;
	case A_REGISTER_OPND:		moreText("a"); break;
	case X_REGISTER_OPND:		moreText("x"); break;
	case Y_REGISTER_OPND:		moreText("y"); break;
	case X_SELECTED_OPND:		moreText("x"); break;
	case Y_SELECTED_OPND:		moreText("y"); break;
	case PRE_SELECTED_X_OPND:	moreText("@x"); break;
	default:			break;
	}
	expandExpression(NULL, NULL);
	if (addressMode == POST_INDEXED_Y_OPND ||
	    addressMode == PRE_INDEXED_X_OPND ||
	    addressMode == X_INDEXED_OPND ||
	    addressMode == Y_INDEXED_OPND)
		moreText("]");
}

/* corresponds to routines in expressionSemantics.c */

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

  valueType *
evaluateOperand(operandType *operand)
{
	valueType		*result;
	bool			 saveExpansion;
	expressionType		*expression;

	valueType		*evaluateExpression(expressionType *expression, fixupKindType kindOfFixup);
	valueType		*evaluateSelectionList(selectionListType *selectionList);
	valueType		*newValue(valueKindType kindOfValue, int value, operandKindType addressMode);

	nullEvaluate(operand);
	if (operand->kindOfOperand != EXPRESSION_OPND)
		newFixupAddressMode = operand->kindOfOperand;
	switch (operand->kindOfOperand) {

	case EXPRESSION_OPND:
	case IMMEDIATE_OPND:
	case INDIRECT_OPND:
	case POST_INDEXED_Y_OPND:
	case PRE_INDEXED_X_OPND:
	case X_INDEXED_OPND:
	case Y_INDEXED_OPND:
		result = evaluateExpression(operand->theOperand.expressionUnion,
			performingFixups ? NO_FIXUP : OPERAND_FIXUP);
		if (operand->kindOfOperand != EXPRESSION_OPND) {
			if (result->addressMode != EXPRESSION_OPND) {
				error(BAD_ADDRESS_MODE_ERROR);
				result->kindOfValue = FAIL;
			} else {
				result->addressMode = operand->kindOfOperand;
			}
		}
		break;

	case A_REGISTER_OPND:
	case X_REGISTER_OPND:
	case Y_REGISTER_OPND:
		result = newValue(UNDEFINED_VALUE, 0, operand->
			kindOfOperand);
		break;

	case X_SELECTED_OPND:
	case Y_SELECTED_OPND:
	case PRE_SELECTED_X_OPND:
		result = evaluateSelectionList(operand->theOperand.xSelectedUnion);
		if (result->addressMode != EXPRESSION_OPND) {
			error(BAD_ADDRESS_MODE_ERROR);
			result->kindOfValue = FAIL;
		} else {
			result->addressMode = operand->kindOfOperand;
		}
		break;

	case STRING_OPND:
		result = newValue(STRING_VALUE, operand->theOperand.stringUnion,
			STRING_OPND);
		break;

	case BLOCK_OPND:
		if (standaloneExpansionFlag)
			forceExpansion();
		sideEffectFlag = TRUE;
		assembleBlock(operand->theOperand.blockUnion);
		expansionOn();
		result = newValue(FAIL, 0, BLOCK_OPND);
		break;

	default:
		botch("bad operand kind in evaluateOperand %d\n",
						operand->kindOfOperand);
		break;
	}
	return(result);
}

/* from parserMisc.c */

  conditionType
invertConditionCode(conditionType conditionCode)
{
#define cc (int)conditionCode

	if ((int)CARRY_COND<=cc && cc<=(int)ALWAYS_COND)
		return((conditionType)
			(cc - (int)CARRY_COND + (int)NOT_CARRY_COND));
	else if ((int)NOT_CARRY_COND<=cc && cc<=(int)NEVER_COND)
		return((conditionType)
			(cc + (int)CARRY_COND - (int)NOT_CARRY_COND));
	else
		botch("invertConditionCode given %d, not a condition code\n");
}

/* from semanticMisc.c */

  bool
shouldParenthesize(operandType *operand)
{
	expressionTermKindType	 kind;

	switch(operand->kindOfOperand) {
	case EXPRESSION_OPND:
	case IMMEDIATE_OPND:
	case INDIRECT_OPND:
	case POST_INDEXED_Y_OPND:
	case PRE_INDEXED_X_OPND:
	case X_INDEXED_OPND:
	case Y_INDEXED_OPND:
		kind = operand->theOperand.expressionUnion->kindOfTerm;
		return (kind==UNOP_EXPR || kind==BINOP_EXPR || kind==
			ASSIGN_EXPR);
		break;

	case X_SELECTED_OPND:
	case Y_SELECTED_OPND:
	case PRE_SELECTED_X_OPND:
		return(TRUE);
		break;

	case A_REGISTER_OPND:
	case X_REGISTER_OPND:
	case Y_REGISTER_OPND:
	case STRING_OPND:
	case BLOCK_OPND:
		return(FALSE);
		break;
	}
}
