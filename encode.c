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
/* WARNING: this file has not yet been separated into 6502 and non-6052 parts */
/*
	encode.c -- Routines to encode expressions for Macross object files.

	Chip Morningstar -- Lucasfilm Ltd.

	8-November-1985
*/

#include "macrossTypes.h"
#include "macrossGlobals.h"
#include "y.tab.h"
#include "encode.h"
#include "debugPrint.h"
#include "errorStuff.h"
#include "parserMisc.h"
#include "semanticMisc.h"
#include "slinkyExpressions.h"

#define nullEncode(thing)   if (thing==NULL) return(TRUE);
#define byteOp(op) ((op)-256)

bool	encodingFunction;

bool
encodeByte(byte aByte)
{
	if (expressionBufferSize < EXPRESSION_BUFFER_LIMIT) {
		expressionBuffer[expressionBufferSize++] = aByte;
		return(TRUE);
	} else {
		error(EXPRESSION_TOO_BIG_TO_FIT_IN_OBJECT_ERROR);
		return(FALSE);
	}
}

  bool
encodeBigword(int bigword)
{
	int	i;
	for (i=0; i<sizeof(int); ++i) {
		if (!encodeByte(bigword & 0xFF))
			return(FALSE);
		bigword >>= 8;
	}
	return(TRUE);
}

  bool
encodeAssignmentTerm(binopTermType *assignmentTerm)
{
	nullEncode(assignmentTerm);
	if ((assignmentKindType)assignmentTerm->binop != ASSIGN_ASSIGN) {
		error(FUNNY_ASSIGNMENT_KIND_IN_OBJECT_EXPRESSION_ERROR);
		return(FALSE);
	}
	return(
		encodeByte(BINOP_TAG) &&
		encodeByte(byteOp(ASSIGN)) &&
		encodeIdentifier(assignmentTerm->leftArgument) &&
		encodeExpression(assignmentTerm->rightArgument)
	);
}

  bool
encodeBinopTerm(binopTermType *binopTerm)
{
	nullEncode(binopTerm);
	return (
		encodeByte(BINOP_TAG) &&
		encodeByte(byteOp(binopTerm->binop)) &&
		encodeExpression(binopTerm->leftArgument) &&
		encodeExpression(binopTerm->rightArgument)
	);
}

  bool
encodeCondition(conditionType condition)
{
	return(
		encodeByte(CONDITION_CODE_TAG) &&
		encodeByte(condition)
	);
}

  int
functionNumber(functionDefinitionType *function)
{
	if (function->ordinal == -1) {
		function->ordinal = externalFunctionCount++;
		if (externalFunctionList == NULL) {
			externalFunctionList = endOfExternalFunctionList =
				function;
		} else {
			endOfExternalFunctionList->nextExternalFunction =
				function;
			endOfExternalFunctionList = function;
		}
	}
	return(function->ordinal);
}

  bool
encodeFunctionCall(functionCallTermType *functionCall)
{
	functionDefinitionType	*theFunction;
	int			 functionOrdinal;
	symbolInContextType	*workingContext;
	operandListType		*parameterList;

	nullEncode(functionCall);
	workingContext = getWorkingContext(functionCall->functionName);
	if (isFunction(workingContext)) {
		if (!encodeByte(FUNCTION_CALL_TAG))
			return(FALSE);
		theFunction = (functionDefinitionType *)workingContext->
			value->value;
		if (!encodeBigword(functionNumber(theFunction)))
			return(FALSE);
	} else if (isBuiltInFunction(workingContext)) {
		functionOrdinal = workingContext->value->value;
		if (builtInFunctionTable[functionOrdinal].isSpecialFunction)
			return(encodeValue((*builtInFunctionTable[
				functionOrdinal].functionEntry)(functionCall->
				parameters, NO_FIXUP)));
		if (!encodeByte(BUILTIN_FUNCTION_CALL_TAG))
			return(FALSE);
		if (builtInFunctionTable[functionOrdinal].ordinal < 0) {
			error(BUILT_IN_FUNCTION_NOT_AVAILABLE_IN_OBJECT_ERROR,
				builtInFunctionTable[functionOrdinal].
				functionName);
			return(FALSE);
		} else if (!encodeBigword(builtInFunctionTable[
				functionOrdinal].ordinal)) {
			return(FALSE);
		}
	} else {
		error(NOT_A_FUNCTION_ERROR, symbName(functionCall->
			functionName));
		return(FALSE);
	}
	parameterList = functionCall->parameters;
	if (!encodeByte(countParameters(parameterList)))
		return(FALSE);
	while (parameterList != NULL)
		if (!encodeOperand(parameterList))
			return(FALSE);
		else
			parameterList = parameterList->nextOperand;
	return(TRUE);
}

  bool
encodeHere(void)
{
	return(encodeByte(HERE_TAG));
}

  bool
encodeIdentifier(symbolTableEntryType *identifier)
{
	symbolInContextType	*workingContext;
	environmentType		*saveEnvironment;
	bool			 result;

	nullEncode(identifier);

	if (symbName(identifier)[0] == '$') {
		error(TEMP_SYMBOL_IN_OBJECT_ERROR, symbName(identifier));
		return(FALSE);
	}
	if (encodingFunction) {
		return(encodeByte(IDENTIFIER_TAG) &&
		       encodeBigword(identifier->ordinal));
	}
	if ((workingContext = getWorkingContext(identifier)) == NULL) {
		error(UNDEFINED_SYMBOL_ERROR, symbName(identifier));
		return(FALSE);
	}
	if (workingContext->usage == FUNCTION_SYMBOL || workingContext->usage
			== BUILT_IN_FUNCTION_SYMBOL) {
		error(FUNCTION_IS_NOT_A_VALUE_ERROR, symbName(identifier));
		return(FALSE);
	}
	if (workingContext->value == NULL) {
		error(UNDEFINED_SYMBOL_ERROR, symbName(identifier));
		return(FALSE);
	}
	if (workingContext->value->kindOfValue == UNDEFINED_VALUE) {
		if (workingContext->attributes & GLOBAL_ATT) {
			return(encodeByte(IDENTIFIER_TAG) &&
			       encodeBigword(identifier->ordinal));
		} else {
			error(UNDEFINED_SYMBOL_ERROR, symbName(identifier));
			return(FALSE);
		}
	}
	if (workingContext->value->kindOfValue == RELOCATABLE_VALUE) {
		return(encodeByte(IDENTIFIER_TAG) &&
		       encodeBigword(identifier->ordinal));
	}
	if (workingContext->value->kindOfValue == FAIL) {
		error(UNASSIGNED_SYMBOL_ERROR, symbName(identifier));
		return(FALSE);
	}
	if (workingContext->value->kindOfValue == OPERAND_VALUE) {
		saveEnvironment = currentEnvironment;
		if (workingContext->usage == ARGUMENT_SYMBOL) {
			currentEnvironment = currentEnvironment->
				previousEnvironment;
		}
		result = encodeOperand(workingContext->value->value);
		currentEnvironment = saveEnvironment;
		return(result);
	}
	if (workingContext->value->kindOfValue == BLOCK_VALUE) {
		error(BLOCK_OPERAND_IN_OBJECT_EXPRESSION_ERROR);
		return(FALSE);
	}
	return(encodeValue(workingContext->value));
}

  bool
encodeNumber(numberTermType number)
{
	return(
		encodeByte(NUMBER_TAG) &&
		encodeBigword(number)
	);
}

  bool
encodeRelocatableNumber(numberTermType number)
{
	return(
		encodeByte(RELOCATABLE_TAG) &&
		encodeBigword(number)
	);
}

  bool
encodeOperand(operandType *operand)
{
	switch (operand->kindOfOperand) {
		case EXPRESSION_OPND:
		case IMMEDIATE_OPND:
		case INDIRECT_OPND:
		case POST_INDEXED_Y_OPND:
		case PRE_INDEXED_X_OPND:
		case X_INDEXED_OPND:
		case Y_INDEXED_OPND:
			return(encodeExpression(operand->theOperand.expressionUnion));

		case A_REGISTER_OPND:
		case X_REGISTER_OPND:
		case Y_REGISTER_OPND:
			error(REGISTER_OPERAND_IN_OBJECT_EXPRESSION_ERROR);
			return(FALSE);

		case X_SELECTED_OPND:
		case Y_SELECTED_OPND:
		case PRE_SELECTED_X_OPND:
			error(SELECTION_OPERAND_IN_OBJECT_EXPRESSION_ERROR);
			return(FALSE);

		case STRING_OPND:
			return(encodeString(operand->theOperand.stringUnion));

		case BLOCK_OPND:
			error(BLOCK_OPERAND_IN_OBJECT_EXPRESSION_ERROR);
			return(FALSE);
	}
}

  bool
encodePostopTerm(postOpTermType *postopTerm)
{
	nullEncode(postopTerm);
	return(
		encodeByte(POSTOP_TAG) &&
		encodeByte(byteOp(postopTerm->postOp)) &&
		encodeExpression(postopTerm->postOpArgument)
	);
}

  bool
encodePreopTerm(preOpTermType *preopTerm)
{
	nullEncode(preopTerm);
	return(
		encodeByte(PREOP_TAG) &&
		encodeByte(byteOp(preopTerm->preOp)) &&
		encodeExpression(preopTerm->preOpArgument)
	);
}

  bool
encodeString(stringType *string)
{
	if (!encodeByte(STRING_TAG))
		return(FALSE);
	while (*string != '\0') {
		if (!encodeByte(*string++))
			return(FALSE);
	}
	return(encodeByte('\0'));
}

  bool
encodeUnopTerm(unopTermType *unopTerm)
{
	nullEncode(unopTerm);
	return(
		encodeByte(UNOP_TAG) &&
		encodeByte(byteOp(unopTerm->unop)) &&
		encodeExpression(unopTerm->unopArgument)
	);
}

  bool
encodeValue(valueType *value)
{
	switch (value->kindOfValue) {
		case ABSOLUTE_VALUE:
			return(encodeNumber(value->value));

		case RELOCATABLE_VALUE:
			return(encodeRelocatableNumber(value->value));

		case OPERAND_VALUE:
			return(encodeOperand(value->value));

		case STRING_VALUE:
			return(encodeString(value->value));

		case CONDITION_VALUE:
			return(encodeCondition(value->value));

		case DATA_VALUE:
		case BSS_VALUE:
		case STRUCT_VALUE:
		case FIELD_VALUE:
		case MACRO_VALUE:
		case UNDEFINED_VALUE:
		case FUNCTION_VALUE:
		case BLOCK_VALUE:
		case BUILT_IN_FUNCTION_VALUE:
		case ARRAY_VALUE:
		case FAIL:
			error(WRONG_KIND_OF_VALUE_IN_OBJECT_EXPRESSION_ERROR,
				valueKindString(value->kindOfValue));
			return(FALSE);
	}
}

  bool
encodeExpression(expressionType *expression)
{
	nullEncode(expression);
	switch (expression->kindOfTerm) {

	case ARRAY_EXPR:
		error(ARRAY_TERM_IN_OBJECT_EXPRESSION_ERROR);
		return(FALSE);
		break;

	case ASSIGN_EXPR:
		return(encodeAssignmentTerm(expression->expressionTerm.binopUnion));
		break;

	case BINOP_EXPR:
		return(encodeBinopTerm(expression->expressionTerm.binopUnion));
		break;

	case CONDITION_CODE_EXPR:
		return(encodeCondition(expression->expressionTerm.conditionTypeUnion));
		break;

	case FUNCTION_CALL_EXPR:
		return(encodeFunctionCall(expression->expressionTerm.functionCallUnion));
		break;

	case HERE_EXPR:
		return(encodeHere());
		break;

	case IDENTIFIER_EXPR:
		return(encodeIdentifier(expression->expressionTerm.identifierUnion));
		break;

	case NUMBER_EXPR:
		return(encodeNumber(expression->expressionTerm.numberUnion));
		break;

	case POSTOP_EXPR:
		return(encodePostopTerm(expression->expressionTerm.postOpUnion));
		break;

	case PREOP_EXPR:
		return(encodePreopTerm(expression->expressionTerm.preOpUnion));
		break;

	case SUBEXPRESSION_EXPR:
		encodeExpression(expression->expressionTerm.expressionUnion);
		break;

	case STRING_EXPR:
		return(encodeString(expression->expressionTerm.stringUnion));
		break;

	case UNOP_EXPR:
		return(encodeUnopTerm(expression->expressionTerm.unopUnion));
		break;

	case VALUE_EXPR:
		return(encodeValue(expression->expressionTerm.valueUnion));
		break;

	default:
		botch("encodeExpression: funny expression kind %d\n",
			expression->kindOfTerm);
		break;
	}
}

  bool
encodeAssertStatement(assertStatementBodyType *assertStatement)
{
	return(
		encodeByte(ASSERT_TAG) &&
		encodeExpression(assertStatement->condition) &&
		encodeExpression(assertStatement->message)
	);
}

  bool
encodeFreturnStatement(freturnStatementBodyType *freturnStatement)
{
	return(
		encodeByte(FRETURN_TAG) &&
		encodeExpression(freturnStatement)
	);
}

  bool
encodeMdefineStatement(defineStatementBodyType *mdefineStatement)
{
	return(
		encodeByte(MDEFINE_TAG) &&
		encodeIdentifier(mdefineStatement->theSymbol) &&
		encodeExpression(mdefineStatement->theValue)
	);
}

  bool
encodeMdoUntilStatement(mdoUntilStatementBodyType *mdoUntilStatement)
{
	return(
		encodeByte(MDOUNTIL_TAG) &&
		encodeExpression(mdoUntilStatement->mdoUntilCondition) &&
		encodeBlock(mdoUntilStatement->mdoUntilLoop)
	);
}

  bool
encodeMdoWhileStatement(mdoWhileStatementBodyType *mdoWhileStatement)
{
	return(
		encodeByte(MDOWHILE_TAG) &&
		encodeExpression(mdoWhileStatement->mdoWhileCondition) &&
		encodeBlock(mdoWhileStatement->mdoWhileLoop)
	);
}

  bool
encodeMforStatement(mforStatementBodyType *mforStatement)
{
	return(
		encodeByte(MFOR_TAG) &&
		encodeExpression(mforStatement->initExpression) &&
		encodeExpression(mforStatement->testExpression) &&
		encodeExpression(mforStatement->incrExpression) &&
		encodeBlock(mforStatement->forLoop)
	);
}

  bool
encodeMifStatement(mifStatementBodyType *mifStatement)
{
	return(
		encodeByte(MIF_TAG) &&
		encodeExpression(mifStatement->mifCondition) &&
		encodeBlock(mifStatement->mifConsequence) &&
		encodeBlock(mifStatement->mifContinuation.mifBlockUnion)
	);
}

  bool
encodeMswitchStatement(mswitchStatementBodyType *mswitchStatement)
{
    caseListType	*caseList;
    caseType		*theCase;
    expressionListType	*tagExpressionList;

    if (!(encodeByte(MSWITCH_TAG) && encodeExpression(mswitchStatement->
	    switchExpression)))
        return(FALSE);
    for (caseList=mswitchStatement->cases; caseList!=NULL; caseList=caseList->
	    nextCase) {
        theCase = caseList->theCase;
        for (tagExpressionList=theCase->caseTags; tagExpressionList!=NULL;
		tagExpressionList=tagExpressionList->nextExpression) {
            if (!encodeExpression(tagExpressionList->theExpression))
                return(FALSE);
        }
        if (!encodeBlock(theCase->caseBody))
            return(FALSE);
    }
    return(encodeByte(END_TAG));
}

  bool
encodeMvariableStatement(mvariableStatementBodyType *mvariableStatement)
{
	int	length;	

	if ((length=expressionListLength(mvariableStatement->theValue) > 1) ||
			mvariableStatement->theDimension!=NULL) {
		error(ARRAY_MVARIABLE_IN_OBJECT_FUNCTION_ERROR);
		return(FALSE);
	}
	if (!(encodeByte(MVARIABLE_TAG) && encodeIdentifier(mvariableStatement->
			theSymbol)))
		return(FALSE);
	if (length == 1)
		return(encodeExpression(mvariableStatement->theValue->
			theExpression));
	else
		return(encodeExpression(NULL));
}

  bool
encodeMwhileStatement(mwhileStatementBodyType *mwhileStatement)
{
	return(
		encodeByte(MWHILE_TAG) &&
		encodeExpression(mwhileStatement->mwhileCondition) &&
		encodeBlock(mwhileStatement->mwhileLoop)
	);
}

  bool
encodeStatement(statementType *statement)
{
	switch(statement->kindOfStatement) {

	case ALIGN_STATEMENT:
	case BLOCK_STATEMENT:
	case BYTE_STATEMENT:
	case CONSTRAIN_STATEMENT:
	case DBYTE_STATEMENT:
	case DEFINE_STATEMENT:
	case DO_UNTIL_STATEMENT:
	case DO_WHILE_STATEMENT:
	case EXTERN_STATEMENT:
	case FUNCTION_STATEMENT:
	case IF_STATEMENT:
	case INCLUDE_STATEMENT:
	case INSTRUCTION_STATEMENT:
	case LONG_STATEMENT:
	case MACRO_STATEMENT:
	case ORG_STATEMENT:
	case REL_STATEMENT:
	case START_STATEMENT:
	case STRING_STATEMENT:
	case STRUCT_STATEMENT:
	case TARGET_STATEMENT:
	case UNDEFINE_STATEMENT:
	case VARIABLE_STATEMENT:
	case WHILE_STATEMENT:
	case WORD_STATEMENT:
		error(ILLEGAL_STATEMENT_IN_OBJECT_FILE_FUNCTION_ERROR,
			statementKindString(statement->kindOfStatement));
		return(FALSE);

	case ASSERT_STATEMENT:
		return(encodeAssertStatement(statement->statementBody.assertUnion));

	case FRETURN_STATEMENT:
		return(encodeFreturnStatement(statement->statementBody.freturnUnion));

	case GROUP_STATEMENT:
		return(encodeBlock(statement->statementBody.groupUnion));

	case MDEFINE_STATEMENT:
		return(encodeMdefineStatement(statement->statementBody.defineUnion));

	case MDO_UNTIL_STATEMENT:
		return(encodeMdoUntilStatement(statement->statementBody.mdoUntilUnion));

	case MDO_WHILE_STATEMENT:
		return(encodeMdoWhileStatement(statement->statementBody.mdoWhileUnion));

	case MFOR_STATEMENT:
		return(encodeMforStatement(statement->statementBody.mforUnion));

	case MIF_STATEMENT:
		return(encodeMifStatement(statement->statementBody.mifUnion));

	case MSWITCH_STATEMENT:
		return(encodeMswitchStatement(statement->statementBody.mswitchUnion));

	case MVARIABLE_STATEMENT:
		return(encodeMvariableStatement(statement->statementBody.mvariableUnion));

	case MWHILE_STATEMENT:
		return(encodeMwhileStatement(statement->statementBody.mwhileUnion));

	case NULL_STATEMENT:
		return(TRUE);

	case PERFORM_STATEMENT:
		return(encodeExpression(statement->statementBody.expressionUnion));

	default:
		botch("encodeStatementBody doesn't know kind %d\n",
			statement->kindOfStatement);
		return(FALSE);
	}
}

  bool
encodeBlock(blockType *block)
{
	if (!encodeByte(BLOCK_TAG))
		return(FALSE);
	while (block != NULL) {
		if (!encodeStatement(block))
			return(FALSE);
		block = block->nextStatement;
	}
	return(encodeByte(END_TAG));
}
