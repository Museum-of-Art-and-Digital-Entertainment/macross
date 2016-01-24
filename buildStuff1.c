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
	buildStuff1.c -- Routines to build parse-tree structures for the
			Macross assembler.  This module is target processor
			independent.

	Chip Morningstar -- Lucasfilm Ltd.

	1-November-1984

*/

#include "macrossTypes.h"
#include "macrossGlobals.h"
#include "errorStuff.h"
#include "lookups.h"
#include "parserMisc.h"

/* Generic routine to create statement nodes */
  
statementType *
newStatement(statementKindType kind, statementBodyType body)
{
	statementType	*result;

	result = typeAlloc(statementType);
	result->kindOfStatement = kind;
	result->labels = NULL;
	result->statementBody = body;
	result->nextStatement = NULL;
	result->fileName = currentFileName;
	result->lineNumber = currentLineNumber;
	result->cumulativeLineNumber = cumulativeLineNumber;
	return(result);
}

/*
   These routines specifically build top-level parse-tree nodes corresponding
   to the different types of Macross statements.  There is one such routine of
   the form "buildXxxxStatement" for each kind of Macross statement.
 */

  statementType *
buildAlignStatement(expressionType *expression)
{
	return(newStatement(ALIGN_STATEMENT,
	       (statementBodyType) expression));
}

  statementType *
buildAssertStatement(expressionType *condition, expressionType *message)
{
	assertStatementBodyType	*result;

	result = typeAlloc(assertStatementBodyType);

	result->condition = condition;
	result->message = message;
	return(newStatement(ASSERT_STATEMENT, (statementBodyType) result));
}

  statementType *
buildBlockStatement(expressionListType *expressionList)
{
	return(newStatement(BLOCK_STATEMENT,
	       (statementBodyType) expressionList));
}

  statementType *
buildByteStatement(expressionListType *expressionList)
{
	return(newStatement(BYTE_STATEMENT,
	       (statementBodyType) expressionList));
}

  statementType *
buildConstrainStatement(expressionType *expression, blockType *block)
{
	constrainStatementBodyType	*result;

	result = typeAlloc(constrainStatementBodyType);

	result->constraint = expression;
	result->constrainedBlock = block;
	return(newStatement(CONSTRAIN_STATEMENT, (statementBodyType) result));
}

  statementType *
buildDbyteStatement(expressionListType *expressionList)
{
	return(newStatement(DBYTE_STATEMENT,
	       (statementBodyType) expressionList));
}

  statementType *
buildDefineStatement(stringType *name, expressionType *value)
{
	defineStatementBodyType	*result;

	result = typeAlloc(defineStatementBodyType);
	result->theSymbol = lookupOrEnterSymbol(name, UNKNOWN_SYMBOL);
	result->theValue = value;
	return(newStatement(DEFINE_STATEMENT, (statementBodyType) result));
}

  statementType *
buildDoUntilStatement(blockType *body, conditionType condition)
{
	doUntilStatementBodyType	*result;

	result = typeAlloc(doUntilStatementBodyType);
	result->doUntilCondition = condition;
	result->doUntilLoop = body;
	return(newStatement(DO_UNTIL_STATEMENT, (statementBodyType) result));
}

  statementType *
buildDoWhileStatement(blockType *body, conditionType condition)
{
	doWhileStatementBodyType	*result;

	result = typeAlloc(doWhileStatementBodyType);
	result->doWhileCondition = condition;
	result->doWhileLoop = body;
	return(newStatement(DO_WHILE_STATEMENT, (statementBodyType) result));
}

  statementType *
buildDoStatement(blockType *body, doEndType *end)
{
	statementType	*result;

	if (end->doEndKind == UNTIL_END)
		result = buildDoUntilStatement(body, end->doEndCondition);
	else if (end->doEndKind == WHILE_END)
		result = buildDoWhileStatement(body, end->doEndCondition);
	else
		botch("bad do-end kind: %d\n", end->doEndCondition);
	qfree(end);
	return(result);
}

  statementType *
buildExternStatement(identifierListType *identifierList)
{
	return(newStatement(EXTERN_STATEMENT,
	       (statementBodyType) identifierList));
}

  statementType *
buildFreturnStatement(expressionType *expression)
{
	return(newStatement(FRETURN_STATEMENT,
	       (statementBodyType) expression));
}

  statementType *
buildFunctionStatement(stringType *name, argumentDefinitionListType *arguments, blockType *body)
{
	functionStatementBodyType	*result;
	symbolTableEntryType		*testSymbol;

	testSymbol = lookupOrEnterSymbol(name, FUNCTION_SYMBOL);
	if (testSymbol->context->usage != FUNCTION_SYMBOL && testSymbol->
			context->usage != UNKNOWN_FUNCTION_SYMBOL) {
		error(SYMBOL_ALREADY_THERE_ERROR, symbName(testSymbol));
		return(NULL);
	}
	result = typeAlloc(functionStatementBodyType);
	result->functionName = saveString(name);
	result->theArguments = arguments;
	result->theBlock = body;
	return(newStatement(FUNCTION_STATEMENT, (statementBodyType) result));
}

  statementType *
buildGroupStatement(blockType *block)
{
	return(newStatement(GROUP_STATEMENT,
		(statementBodyType) block));
}

  statementType *
buildIfStatement(ifHeadType *head, ifContinuationType continuation, ifContinuationKindType continuationKind)
{
	ifStatementBodyType	*result;

	result = typeAlloc(ifStatementBodyType);

	result->ifCondition = head->headCondition;
	result->consequence = head->headBody;
	if (continuationKind == NO_CONTINUATION)
		result->continuation.blockUnion = NULL;
	else if (continuationKind == ELSE_CONTINUATION) {
		result->continuation.continuationBodyUnion =
			typeAlloc(ifContinuationBodyType);
		result->continuation.continuationBodyUnion->ifCondition =
			ALWAYS_COND;
		result->continuation.continuationBodyUnion->consequence =
			continuation.blockUnion;
		result->continuation.continuationBodyUnion->
			continuation.continuationBodyUnion = NULL;
	} else if (continuationKind == ELSEIF_CONTINUATION)
		result->continuation = continuation;
	else
		botch("bad continuation kind: %d\n", continuationKind);
	qfree(head);
	return(newStatement(IF_STATEMENT, (statementBodyType) result));
}

  statementType *
buildIncludeStatement(expressionType *filename)
{
	return(newStatement(INCLUDE_STATEMENT,
	       (statementBodyType) filename));
}

  statementType *
buildInstructionStatement(opcodeTableEntryType *opcode, operandListType *operands)
{
	instructionStatementBodyType	*result;

	result = typeAlloc(instructionStatementBodyType);
	result->kindOfInstruction = OPCODE_INSTRUCTION;
	result->theInstruction.opcodeUnion = opcode;
	result->theOperands = operands;
	return(newStatement(INSTRUCTION_STATEMENT, (statementBodyType) result));
}

  statementType *
buildLongStatement(expressionListType *expressionList)
{
	return(newStatement(LONG_STATEMENT,
	       (statementBodyType)  expressionList));
}

  statementType *
buildMacroStatement(macroTableEntryType *macro, argumentDefinitionListType *arguments, blockType *body)
{
	macroStatementBodyType		*result;

	result = typeAlloc(macroStatementBodyType);
	result->theMacro = macro;
	result->theArguments = arguments;
	result->theBlock = body;
	return(newStatement(MACRO_STATEMENT, (statementBodyType) result));
}

  statementType *
buildMacroInstructionStatement(macroTableEntryType *macro, operandListType *operands)
{
	instructionStatementBodyType	*result;

	result = typeAlloc(instructionStatementBodyType);
	result->kindOfInstruction = MACRO_INSTRUCTION;
	result->theInstruction.macroUnion = macro;
	result->theOperands = operands;
	return(newStatement(INSTRUCTION_STATEMENT, (statementBodyType) result));
}

  statementType *
buildMdefineStatement(stringType *name, expressionType *value)
{
	mdefineStatementBodyType	*result;

	result = typeAlloc(mdefineStatementBodyType);
	result->theSymbol = lookupOrEnterSymbol(name, DEAD_SYMBOL);
	result->theValue = value;
	return(newStatement(MDEFINE_STATEMENT, (statementBodyType) result));
}

  statementType *
buildMdoUntilStatement(blockType *body, struct expressionTermStruct *condition)
{
	mdoUntilStatementBodyType	*result;

	result = typeAlloc(mdoUntilStatementBodyType);
	result->mdoUntilCondition = condition;
	result->mdoUntilLoop = body;
	return(newStatement(MDO_UNTIL_STATEMENT, (statementBodyType) result));
}

  statementType *
buildMdoWhileStatement(blockType *body, expressionType *condition)
{
	mdoWhileStatementBodyType	*result;

	result = typeAlloc(mdoWhileStatementBodyType);
	result->mdoWhileCondition = condition;
	result->mdoWhileLoop = body;
	return(newStatement(MDO_WHILE_STATEMENT, (statementBodyType) result));
}

  statementType *
buildMdoStatement(blockType *body, mdoEndType *end)
{
	statementType	*result;

	if (end->mdoEndKind == UNTIL_END)
		result = buildMdoUntilStatement(body, end->mdoEndCondition);
	else if (end->mdoEndKind == WHILE_END)
		result = buildMdoWhileStatement(body, end->mdoEndCondition);
	else
		botch("bad mdo-end kind: %d\n", end->mdoEndCondition);
	qfree(end);
	return(result);
}

  statementType *
buildMforStatement(forExpressionsType *forExpressions, blockType *body)
{
	mforStatementBodyType	*result;

	result = typeAlloc(mforStatementBodyType);
	result->initExpression = forExpressions->initExpression;
	result->testExpression = forExpressions->testExpression;
	result->incrExpression = forExpressions->incrExpression;
	result->forLoop = body;
	qfree(forExpressions);
	return(newStatement(MFOR_STATEMENT, (statementBodyType) result));
}

  statementType *
buildMifStatement(mifHeadType *head, mifContinuationType continuation, ifContinuationKindType continuationKind)
{
	mifStatementBodyType	*result;

	result = typeAlloc(mifStatementBodyType);

	result->mifCondition = head->headCondition;
	result->mifConsequence = head->headBody;
	if (continuationKind == NO_CONTINUATION)
		result->mifContinuation.mifContinuationBodyUnion = NULL;
	else if (continuationKind == ELSE_CONTINUATION) {
		result->mifContinuation.mifContinuationBodyUnion =
			typeAlloc(mifContinuationBodyType);
		result->mifContinuation.mifContinuationBodyUnion->
			mifCondition = NULL;
		result->mifContinuation.mifContinuationBodyUnion->
			mifConsequence = continuation.mifBlockUnion;
		result->mifContinuation.mifContinuationBodyUnion->
			mifContinuation.mifContinuationBodyUnion = NULL;
	} else if (continuationKind == ELSEIF_CONTINUATION)
		result->mifContinuation = continuation;
	else
		botch("bad mif continuation kind: %d\n", continuationKind);
	qfree(head);
	return(newStatement(MIF_STATEMENT, (statementBodyType) result));
}

  statementType *
buildMswitchStatement(expressionType *switchExpression, caseListType *cases)
{
	mswitchStatementBodyType	*result;

	result = typeAlloc(mswitchStatementBodyType);
	result->switchExpression = switchExpression;
	result->cases = cases;
	return(newStatement(MSWITCH_STATEMENT, (statementBodyType) result));
}

  statementType *
buildMvariableStatement(stringType *name, expressionListType *value, expressionType *dimension)
{
	mvariableStatementBodyType	*result;

	result = typeAlloc(mvariableStatementBodyType);
	result->theSymbol = lookupOrEnterSymbol(name, DEAD_SYMBOL);
	result->theValue = value;
	result->theDimension = dimension;
	return(newStatement(MVARIABLE_STATEMENT, (statementBodyType) result));
}

  statementType *
buildMwhileStatement(expressionType *condition, blockType *body)
{
	mwhileStatementBodyType	*result;

	result = typeAlloc(mwhileStatementBodyType);
	result->mwhileCondition = condition;
	result->mwhileLoop = body;
	return(newStatement(MWHILE_STATEMENT, (statementBodyType) result));
}

  statementType *
buildNullStatement(void)
{
	return(newStatement(NULL_STATEMENT, (statementBodyType){ .ifUnion = NULL }));
}

  statementType *
buildOrgStatement(expressionType *expression)
{
	return(newStatement(ORG_STATEMENT,
	       (statementBodyType) expression));
}

  statementType *
buildPerformStatement(expressionType *expression)
{
	return(newStatement(PERFORM_STATEMENT,
		(statementBodyType) expression));
}

  statementType *
buildRelStatement(void)
{
	return(newStatement(REL_STATEMENT, (statementBodyType){ .ifUnion = NULL }));
}

  statementType *
buildStartStatement(expressionType *expression)
{
	return(newStatement(START_STATEMENT,
	       (statementBodyType) expression));
}

  statementType *
buildStringStatement(expressionListType *expressionList)
{
	return(newStatement(STRING_STATEMENT,
	       (statementBodyType) expressionList));
}

  statementType *
buildStructStatement(symbolTableEntryType *name, blockType *body)
{
	structStatementBodyType		*result;

	result = typeAlloc(structStatementBodyType);
	result->structBody = body;
	result->structName = name;
	return(newStatement(STRUCT_STATEMENT, (statementBodyType) result));
}

  statementType *
buildTargetStatement(expressionType *expression)
{
	return(newStatement(TARGET_STATEMENT,
	       (statementBodyType) expression));
}

  statementType *
buildUndefineStatement(identifierListType *identifierList)
{
	return(newStatement(UNDEFINE_STATEMENT,
	       (statementBodyType) identifierList));
}

  statementType *
buildVariableStatement(stringType *name, expressionListType *value, expressionType *dimension)
{
	variableStatementBodyType	*result;

	result = typeAlloc(variableStatementBodyType);
	result->theSymbol = lookupOrEnterSymbol(name, VARIABLE_SYMBOL);
	result->theValue = value;
	result->theDimension = dimension;
	return(newStatement(VARIABLE_STATEMENT, (statementBodyType) result));
}

  statementType *
buildWhileStatement(conditionType condition, blockType *body)
{
	whileStatementBodyType	*result;

	result = typeAlloc(whileStatementBodyType);
	result->whileCondition = condition;
	result->whileLoop = body;
	return(newStatement(WHILE_STATEMENT, (statementBodyType) result));
}

  statementType *
buildWordStatement(expressionListType *expressionList)
{
	return(newStatement(WORD_STATEMENT,
	       (statementBodyType) expressionList));
}

