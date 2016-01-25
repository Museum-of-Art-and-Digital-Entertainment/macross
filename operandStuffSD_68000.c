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
	operandStuff_68000.c -- Various target processor routines to handle
			operands in the Macross assembler (68000 version).

	Chip Morningstar -- Lucasfilm Ltd.

	26-April-1985
*/

#include "macrossTypes.h"
#include "macrossGlobals.h"

/* corresponds to routines in buildStuff2.c */

  operandType *
buildOperand(kindOfOperand, arg1, arg2, arg3, arg4)
  operandKindType	kindOfOperand;
  int			arg1;
  int			arg2;
  int			arg3;
  int			arg4;
{
	operandType	*result;

	result = typeAlloc(operandType);
	result->kindOfOperand = kindOfOperand;
	result->nextOperand = NULL;
	switch (kindOfOperand) {

	case EXPRESSION_OPND:
		result->theOperand.expressionUnion =
			(expressionOperandBodyType *) arg1;
		break;

	case STRING_OPND:
		result->theOperand.stringUnion =
			(stringOperandBodyType *) arg1;
		break;

	case BLOCK_OPND:
		result->theOperand.blockUnion = (blockOperandBodyType *) arg1;
		break;

	case D_REGISTER_OPND:
		result->theOperand.dRegisterUnion =
			(dRegisterOperandBodyType *) NULL;
		setRegister(result->kindOfOperand, arg1);
		break;

	case A_REGISTER_OPND:
		result->theOperand.aRegisterUnion =
			(aRegisterOperandBodyType *) NULL;
		setRegister(result->kindOfOperand, arg1);
		break;

	case A_REGISTER_INDIRECT_OPND:
		result->theOperand.aRegisterIndirectUnion =
			(aRegisterIndirectOperandBodyType *) NULL;
		setRegister(result->kindOfOperand, arg1);
		break;

	case POSTINCREMENT_OPND:
		result->theOperand.postincrementUnion =
			(postincrementOperandBodyType *) NULL;
		setRegister(result->kindOfOperand, arg1);
		break;

	case PREDECREMENT_OPND:
		result->theOperand.predecrementUnion =
			(predecrementOperandBodyType *) NULL;
		setRegister(result->kindOfOperand, arg1);
		break;

	case DISPLACEMENT_OPND:
		result->theOperand.displacementUnion =
			(displacementOperandBodyType *) arg2;
		setRegister(result->kindOfOperand, arg1);
		break;

	case SELECTED_OPND:
		result->theOperand.selectionUnion =
			(selectedOperandBodyType *) arg2;
		setRegister(result->kindOfOperand, arg1);
		break;

	case INDEXED_OPND:
		result->theOperand.indexedUnion =
			(indexedOperandBodyType *) arg1;
		setRegister(result->kindOfOperand, arg2);
		setIndexRegister(result->kindOfOperand, arg3);
		setWL(result->kindOfOperand, (int)arg4);
		break;

	case INDEX_SELECTED_OPND:
		result->theOperand.indexSelectedUnion =
			(indexSelectedOperandBodyType *) arg1;
		setRegister(result->kindOfOperand, arg2);
		setIndexRegister(result->kindOfOperand, arg3);
		setWL(result->kindOfOperand, (int)arg4);
		break;

	case PC_DISPLACEMENT_OPND:
		result->theOperand.pcDisplacementUnion =
			(pcDisplacementOperandBodyType *) arg1;
		break;

	case PC_INDEXED_OPND:
		result->theOperand.pcIndexedUnion =
			(pcIndexedOperandBodyType *) arg1;
		setIndexRegister(result->kindOfOperand, arg2);
		setWL(result->kindOfOperand, (int)arg3);
		break;

	case IMMEDIATE_OPND:
		result->theOperand.immediateUnion =
			(immediateOperandBodyType *) arg1;
		break;

	case ABSOLUTE_SHORT_OPND:
		result->theOperand.absoluteShortUnion =
			(absoluteShortOperandBodyType *) arg1;
		break;

	case ABSOLUTE_LONG_OPND:
		result->theOperand.absoluteLongUnion =
			(absoluteLongOperandBodyType *) arg1;
		break;

	case CC_REGISTER_OPND:
		result->theOperand.ccRegisterUnion =
			(ccRegisterOperandBodyType *) NULL;
		break;

	case STATUS_REGISTER_OPND:
		result->theOperand.statusRegisterUnion =
			(statusRegisterOperandBodyType *) NULL;
		break;

	case USP_REGISTER_OPND:
		result->theOperand.uspRegisterUnion =
			(uspRegisterOperandBodyType *) NULL;
		break;

	case CONTROL_REGISTER_OPND:
		result->theOperand.controlRegisterUnion =
			(controlRegisterOperandBodyType *) NULL;
		setRegister(result->kindOfOperand, arg1);
		break;

	default:
		botch("unknown operand kind: %d\n", kindOfOperand);
		break;
	}
	return(result);
}

/* corresponds to routines in fixups.c */

  operandListType *
duplicateOperandForFixup(operand, isSpecialFunctionOperand)
  operandListType	*operand;
  bool			 isSpecialFunctionOperand;
{
	operandListType	*result;

	expressionType	*duplicateExpressionForFixup();

	result = typeAlloc(operandListType);
	result->kindOfOperand = operand->kindOfOperand;
	result->nextOperand = NULL;
	if (operand->kindOfOperand != EXPRESSION_OPND)
		newFixupAddressMode = operand->kindOfOperand;
	switch (operandKindField(operand->kindOfOperand)) {

	case EXPRESSION_OPND:
	case DISPLACEMENT_OPND:
	case SELECTED_OPND:
	case INDEXED_OPND:
	case INDEX_SELECTED_OPND:
	case PC_DISPLACEMENT_OPND:
	case PC_INDEXED_OPND:
	case IMMEDIATE_OPND:
	case ABSOLUTE_SHORT_OPND:
	case ABSOLUTE_LONG_OPND:
		result->theOperand.expressionUnion =
			duplicateExpressionForFixup(operand->theOperand,
			FALSE, isSpecialFunctionOperand);
		break;
	case D_REGISTER_OPND:
	case A_REGISTER_OPND:
	case A_REGISTER_INDIRECT_OPND:
	case POSTINCREMENT_OPND:
	case PREDECREMENT_OPND:
	case CC_REGISTER_OPND:
	case STATUS_REGISTER_OPND:
	case USP_REGISTER_OPND:
	case CONTROL_REGISTER_OPND:
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
freeOperand(operand)
  operandType	*operand;
{
	nullFree(operand);
	switch (operandKindField(operand->kindOfOperand)) {

	case EXPRESSION_OPND:
	case DISPLACEMENT_OPND:
	case INDEXED_OPND:
	case PC_DISPLACEMENT_OPND:
	case PC_INDEXED_OPND:
	case IMMEDIATE_OPND:
	case ABSOLUTE_SHORT_OPND:
	case ABSOLUTE_LONG_OPND:
		freeExpression(operand->theOperand);
		break;

	case D_REGISTER_OPND:
	case A_REGISTER_OPND:
	case A_REGISTER_INDIRECT_OPND:
	case POSTINCREMENT_OPND:
	case PREDECREMENT_OPND:
	case CC_REGISTER_OPND:
	case STATUS_REGISTER_OPND:
	case USP_REGISTER_OPND:
	case CONTROL_REGISTER_OPND:
		break;

	case SELECTED_OPND:
	case INDEX_SELECTED_OPND:
		freeSelectionList(operand->theOperand);
		break;

	case STRING_OPND:
		freeString(operand->theOperand);
		break;

	case BLOCK_OPND:
		freeBlock(operand->theOperand);
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
expandOperand(addressMode, buffer)
  operandKindType	 addressMode;
  char			*buffer;
{
	char	*bufferPtr;

	bufferPtr = buffer;
	switch (operandKindField(addressMode)) {
	case D_REGISTER_OPND:
		moreTextOptional(buffer, &bufferPtr, "d");
		expandNum(buffer, &bufferPtr, getRegister(addressMode));
		break;
	case A_REGISTER_OPND:
		moreTextOptional(buffer, &bufferPtr, "a");
		expandNum(buffer, &bufferPtr, getRegister(addressMode));
		break;
	case A_REGISTER_INDIRECT_OPND:
		moreTextOptional(buffer, &bufferPtr, "[a");
		expandNum(buffer, &bufferPtr, getRegister(addressMode));
		moreTextOptional(buffer, &bufferPtr, "]");
		break;
	case POSTINCREMENT_OPND:
		moreTextOptional(buffer, &bufferPtr, "[a");
		expandNum(buffer, &bufferPtr, getRegister(addressMode));
		moreTextOptional(buffer, &bufferPtr, "]+");
		break;
	case PREDECREMENT_OPND:
		moreTextOptional(buffer, &bufferPtr, "-[a");
		expandNum(buffer, &bufferPtr, getRegister(addressMode));
		moreTextOptional(buffer, &bufferPtr, "]");
		break;
	case IMMEDIATE_OPND:
		moreTextOptional(buffer, &bufferPtr, "#");
		break;
	case CC_REGISTER_OPND:
		moreTextOptional(buffer, &bufferPtr, "ccr");
		break;
	case STATUS_REGISTER_OPND:
		moreTextOptional(buffer, &bufferPtr, "sr");
		break;
	case USP_REGISTER_OPND:
		moreTextOptional(buffer, &bufferPtr, "usp");
		break;
	case CONTROL_REGISTER_OPND:
		if (getRegister(addressMode) == 1)
			moreTextOptional(buffer, &bufferPtr, "sfc");
		else if (getRegister(addressMode) == 2)
			moreTextOptional(buffer, &bufferPtr, "dfc");
		else
			moreTextOptional(buffer, &bufferPtr, "vbr");
		break;
	case SELECTED_OPND:
		moreTextOptional(buffer, &bufferPtr, "a");
		expandNum(buffer, &bufferPtr, getRegister(addressMode));
		break;
	case INDEX_SELECTED_OPND:
		moreTextOptional(buffer, &bufferPtr, "a");
		expandNum(buffer, &bufferPtr, getRegister(addressMode));
		if (getIndexRegister(addressMode)<8) {
			moreTextOptional(buffer, &bufferPtr, "[a");
			expandNum(buffer, &bufferPtr,
				getIndexRegister(addressMode));
		} else {
			moreTextOptional(buffer, &bufferPtr, "[d");
			expandNum(buffer, &bufferPtr,
				getIndexRegister(addressMode)-8);
		}
		moreTextOptional(buffer, &bufferPtr, "]");
		break;

	default:
		break;
	}
	expandExpression(buffer, &bufferPtr);
	switch(operandKindField(addressMode)) {
	case DISPLACEMENT_OPND:
		moreTextOptional(buffer, &bufferPtr, "[a");
		expandNum(buffer, &bufferPtr, getRegister(addressMode));
		moreTextOptional(buffer, &bufferPtr, "]");
		break;
	case INDEXED_OPND:
		moreTextOptional(buffer, &bufferPtr, "[a");
		expandNum(buffer, &bufferPtr, getRegister(addressMode));
		moreTextOptional(buffer, &bufferPtr, ",");
		if (getIndexRegister(addressMode)<8) {
			moreTextOptional(buffer, &bufferPtr, "a");
			expandNum(buffer, &bufferPtr,
				getIndexRegister(addressMode));
		} else {
			moreTextOptional(buffer, &bufferPtr, "d");
			expandNum(buffer, &bufferPtr,
				getIndexRegister(addressMode)-8);
		}
		moreTextOptional(buffer, &bufferPtr, ".");
		if (getWL(addressMode))
			moreTextOptional(buffer, &bufferPtr, "l]");
		else
			moreTextOptional(buffer, &bufferPtr, "w]");
		break;
	case PC_DISPLACEMENT_OPND:
		moreTextOptional(buffer, &bufferPtr, "[pc]");
		break;
	case PC_INDEXED_OPND:
		moreTextOptional(buffer, &bufferPtr, "[pc,");
		if (getIndexRegister(addressMode)<8) {
			moreTextOptional(buffer, &bufferPtr, "a");
			expandNum(buffer, &bufferPtr,
				getIndexRegister(addressMode));
		} else {
			moreTextOptional(buffer, &bufferPtr, "d");
			expandNum(buffer, &bufferPtr,
				getIndexRegister(addressMode)-8);
		}
		moreTextOptional(buffer, &bufferPtr, ".");
		if (getWL(addressMode))
			moreTextOptional(buffer, &bufferPtr, "l]");
		else
			moreTextOptional(buffer, &bufferPtr, "w]");
		break;
	case ABSOLUTE_SHORT_OPND:
		moreTextOptional(buffer, &bufferPtr, ".w");
		break;
	case ABSOLUTE_LONG_OPND:
		moreTextOptional(buffer, &bufferPtr, ".l");
		break;
	default:
		break;
	}
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
evaluateOperand(operand)
  operandType	*operand;
{
	valueType		*result;
	bool			 saveExpansion;
	expressionType		*expression;

	valueType		*evaluateExpression();
	valueType		*evaluateSelectionList();
	valueType		*newValue();

	nullEvaluate(operand);
	if (operand->kindOfOperand != EXPRESSION_OPND)
		newFixupAddressMode = operand->kindOfOperand;
	switch (operandKindField(operand->kindOfOperand)) {

	case EXPRESSION_OPND:
	case DISPLACEMENT_OPND:
	case INDEXED_OPND:
	case PC_DISPLACEMENT_OPND:
	case PC_INDEXED_OPND:
	case IMMEDIATE_OPND:
	case ABSOLUTE_SHORT_OPND:
	case ABSOLUTE_LONG_OPND:
		result = evaluateExpression(operand->theOperand,
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

	case SELECTED_OPND:
	case INDEX_SELECTED_OPND:
		result = evaluateSelectionList(operand->theOperand);
		if (result->addressMode != EXPRESSION_OPND) {
			error(BAD_ADDRESS_MODE_ERROR);
			result->kindOfValue = FAIL;
		} else {
			result->addressMode = operand->kindOfOperand;
		}
		break;

	case D_REGISTER_OPND:
	case A_REGISTER_OPND:
	case A_REGISTER_INDIRECT_OPND:
	case POSTINCREMENT_OPND:
	case PREDECREMENT_OPND:
	case CC_REGISTER_OPND:
	case STATUS_REGISTER_OPND:
	case USP_REGISTER_OPND:
	case CONTROL_REGISTER_OPND:
		result = newValue(ABSOLUTE_VALUE, 0, operand->
			kindOfOperand);
		break;

	case STRING_OPND:
		result = newValue(STRING_VALUE, operand->theOperand,
			STRING_OPND);
		break;

	case BLOCK_OPND:
		if (standaloneExpansionFlag)
			forceExpansion();
		assembleBlock(operand->theOperand);
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
invertConditionCode(conditionCode)
  conditionType	conditionCode;
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
shouldParenthesize(operand)
  operandType	*operand;
{
	expressionTermKindType	 kind;

	switch(operandKindField(operand->kindOfOperand)) {
	case EXPRESSION_OPND:
	case DISPLACEMENT_OPND:
	case INDEXED_OPND:
	case PC_DISPLACEMENT_OPND:
	case PC_INDEXED_OPND:
	case IMMEDIATE_OPND:
	case ABSOLUTE_SHORT_OPND:
	case ABSOLUTE_LONG_OPND:
		kind = operand->theOperand.expressionUnion->kindOfTerm;
		return (kind==UNOP_EXPR || kind==BINOP_EXPR || kind==
			ASSIGN_EXPR);
		break;

	case STRING_OPND:
	case BLOCK_OPND:
	case D_REGISTER_OPND:
	case A_REGISTER_OPND:
	case A_REGISTER_INDIRECT_OPND:
	case POSTINCREMENT_OPND:
	case PREDECREMENT_OPND:
	case CC_REGISTER_OPND:
	case STATUS_REGISTER_OPND:
	case USP_REGISTER_OPND:
	case CONTROL_REGISTER_OPND:
	case SELECTED_OPND:
	case INDEX_SELECTED_OPND:
		return(FALSE);
		break;
	}
}
