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
	garbage.c -- Routines to eat up a parse tree and throw it away

	Chip Morningstar -- Lucasfilm Ltd.

	4-February-1985
*/

#include "macrossTypes.h"
#include "macrossGlobals.h"
#include "garbage.h"
#include "operandStuff.h"
#include "parserMisc.h"
#include "y.tab.h"

#define nullFree(thing) if (thing == NULL) return;

void
freeArrayTerm(arrayTermType *arrayTerm)
{
	nullFree(arrayTerm);
	freeExpression(arrayTerm->arrayName);
	freeExpression(arrayTerm->arrayIndex);
	free(arrayTerm);
}

  void
freeAssignmentTerm(binopTermType *assignmentTerm)
{
	nullFree(assignmentTerm);
	freeExpression(assignmentTerm->rightArgument);
	free(assignmentTerm);
}

  void
freeBinopTerm(binopTermType *binopTerm)
{
	nullFree(binopTerm);
	freeExpression(binopTerm->leftArgument);
	if (binopTerm->binop != SELECT)
		freeExpression(binopTerm->rightArgument);
	free(binopTerm);
}

  void
freeFunctionCall(functionCallTermType *functionCall)
{
	nullFree(functionCall);
	freeOperandList(functionCall->parameters);
	free(functionCall);
}

  void
freePostopTerm(postOpTermType *postopTerm)
{
	nullFree(postopTerm);
	free(postopTerm);
}

  void
freePreopTerm(preOpTermType *preopTerm)
{
	nullFree(preopTerm);
	free(preopTerm);
}

  void
freeString(stringType *string)
{
	nullFree(string);
	free(string);
}

  void
freeUnopTerm(unopTermType *unopTerm)
{
	nullFree(unopTerm);
	freeExpression(unopTerm->unopArgument);
	free(unopTerm);
}

  void
freeExpression(expressionType *expression)
{
	nullFree(expression);
	switch (expression->kindOfTerm) {

	case CONDITION_CODE_EXPR:
	case HERE_EXPR:
	case IDENTIFIER_EXPR:
	case NUMBER_EXPR:
	case VALUE_EXPR:
	case STRING_EXPR:
		break;

	case ARRAY_EXPR:
		freeArrayTerm(expression->expressionTerm.arrayUnion);
		break;

	case ASSIGN_EXPR:
		freeAssignmentTerm(expression->expressionTerm.binopUnion);
		break;

	case BINOP_EXPR:
		freeBinopTerm(expression->expressionTerm.binopUnion);
		break;

	case FUNCTION_CALL_EXPR:
		freeFunctionCall(expression->expressionTerm.functionCallUnion);
		break;

	case POSTOP_EXPR:
		freePostopTerm(expression->expressionTerm.postOpUnion);
		break;

	case PREOP_EXPR:
		freePreopTerm(expression->expressionTerm.preOpUnion);
		break;

	case SUBEXPRESSION_EXPR:
		freeExpression(expression->expressionTerm.expressionUnion);
		break;

	case UNOP_EXPR:
		freeUnopTerm(expression->expressionTerm.unopUnion);
		break;

	default:
		botch("freeExpression: funny expression kind %d\n",
						expression->kindOfTerm);
		break;
	}
	free(expression);
}

  void
freeExpressionList(expressionListType *expressionList)
{
	expressionListType	*newExpressionList;

	while (expressionList != NULL) {
		freeExpression(expressionList->theExpression);
		newExpressionList = expressionList->nextExpression;
		free(expressionList);
		expressionList = newExpressionList;
	}
}

  void
freeIdentifierList(identifierListType *identifierList)
{
	identifierListType	*newIdentifierList;

	while (identifierList != NULL) {
		newIdentifierList = identifierList->nextIdentifier;
		free(identifierList);
		identifierList = newIdentifierList;
	}
}

  void
freeSelectionList(selectionListType *selectionList)
{
	freeIdentifierList(selectionList);
}

  void
freeBlock(blockType *block)
{
	void	freeStatement(statementType *statement);

	nullFree(block);
	freeStatement(block);
}

  void
freeCase(caseType *aCase)
{
	freeExpressionList(aCase->caseTags);
	freeBlock(aCase->caseBody);
	free(aCase);
}

  void
freeCaseList(caseListType *caseList)
{
	caseListType	*newCaseList;

	while (caseList != NULL) {
		freeCase(caseList->theCase);
		newCaseList = caseList->nextCase;
		free(caseList);
		caseList = newCaseList;
	}
}

  void
freeOperandList(operandListType *operandList)
{
	freeOperand(operandList);
}

  void
freeMacro(operandListType *operands)
{
	freeOperandList(operands);
}

  void
freeMachineInstruction(operandListType *operands)
{
	freeOperandList(operands);
}

  void
freeAlignStatement(alignStatementBodyType *alignStatement)
{
	nullFree(alignStatement);
	freeExpression(alignStatement);
}

  void
freeAssertStatement(assertStatementBodyType *assertStatement)
{
	nullFree(assertStatement);
	freeExpression(assertStatement->condition);
	freeExpression(assertStatement->message);
	free(assertStatement);
}

  void
freeBlockStatement(blockStatementBodyType *blockStatement)
{
	nullFree(blockStatement);
	freeExpressionList(blockStatement);
}

  void
freeByteStatement(byteStatementBodyType *byteStatement)
{
	byteStatementBodyType	*newByteStatement;

	nullFree(byteStatement);
	while (byteStatement != NULL) {
		freeExpression(byteStatement->theExpression);
		newByteStatement = byteStatement->nextExpression;
		free(byteStatement);
		byteStatement = newByteStatement;
	}
}

  void
freeConstrainStatement(constrainStatementBodyType *constrainStatement)
{
	nullFree(constrainStatement);
	freeExpression(constrainStatement->constraint);
	freeBlock(constrainStatement->constrainedBlock);
	free(constrainStatement);
}

  void
freeDbyteStatement(dbyteStatementBodyType *dbyteStatement)
{
	dbyteStatementBodyType	*newDbyteStatement;

	nullFree(dbyteStatement);
	while (dbyteStatement != NULL) {
		freeExpression(dbyteStatement->theExpression);
		newDbyteStatement = dbyteStatement->nextExpression;
		free(dbyteStatement);
		dbyteStatement = newDbyteStatement;
	}
}

  void
freeDefineStatement(defineStatementBodyType *defineStatement)
{
	nullFree(defineStatement);
	freeExpression(defineStatement->theValue);
	free(defineStatement);
}

  void
freeDoUntilStatement(doUntilStatementBodyType *doUntilStatement)
{
	nullFree(doUntilStatement);
	freeBlock(doUntilStatement->doUntilLoop);
	free(doUntilStatement);
}

  void
freeDoWhileStatement(doWhileStatementBodyType *doWhileStatement)
{
	nullFree(doWhileStatement);
	freeBlock(doWhileStatement->doWhileLoop);
	free(doWhileStatement);
}

  void
freeExternStatement(externStatementBodyType *externStatement)
{
	freeIdentifierList(externStatement);
}

  void
freeFreturnStatement(freturnStatementBodyType *freturnStatement)
{
	nullFree(freturnStatement);
	freeExpression(freturnStatement);
}

  void
freeFunctionStatement(functionStatementBodyType *functionStatement)
{
	nullFree(functionStatement);
	free(functionStatement->functionName);
	free(functionStatement);
}

  void
freeIfStatement(ifStatementBodyType *ifStatement)
{
	nullFree(ifStatement);
	if (ifStatement->consequence != NULL)
		freeBlock(ifStatement->consequence);
	if (ifStatement->continuation.continuationBodyUnion != NULL) {
		freeIfStatement((void *)ifStatement->continuation.blockUnion);
	}
	free(ifStatement);
}

  void
freeIncludeStatement(includeStatementBodyType *includeStatement)
{
	nullFree(includeStatement);
	freeExpression(includeStatement);
}

  void
freeInstructionStatement(instructionStatementBodyType *instructionStatement)
{
	nullFree(instructionStatement);
	switch(instructionStatement->kindOfInstruction) {

	case OPCODE_INSTRUCTION:
		freeMachineInstruction(instructionStatement->theOperands);
		break;

	case MACRO_INSTRUCTION:
		freeMacro(instructionStatement->theOperands);
		break;

	default:
		botch("bad instruction type=%d\n", instructionStatement->
					kindOfInstruction);
		break;
	}
}

  void
freeLongStatement(longStatementBodyType *longStatement)
{
	longStatementBodyType	*newLongStatement;

	nullFree(longStatement);
	while (longStatement != NULL) {
		freeExpression(longStatement->theExpression);
		newLongStatement = longStatement->nextExpression;
		free(longStatement);
		longStatement = newLongStatement;
	}
}

  void
freeMacroStatement(macroStatementBodyType *macroStatement)
{
	nullFree(macroStatement);
	free(macroStatement);
}

  void
freeMdefineStatement(defineStatementBodyType *mdefineStatement)
                              	                   /* MIST: shouldn't this be "mdefineStatementBodyType"? */
{
	valueType		*freeDefineExpression();

	nullFree(mdefineStatement);
	freeExpression(mdefineStatement->theValue);
	free(mdefineStatement);
}

  void
freeMdoUntilStatement(mdoUntilStatementBodyType *mdoUntilStatement)
{
	nullFree(mdoUntilStatement);
	freeBlock(mdoUntilStatement->mdoUntilLoop);
	freeExpression(mdoUntilStatement->mdoUntilCondition);
}

  void
freeMdoWhileStatement(mdoWhileStatementBodyType *mdoWhileStatement)
{
	nullFree(mdoWhileStatement);
	freeBlock(mdoWhileStatement->mdoWhileLoop);
	freeExpression(mdoWhileStatement->mdoWhileCondition);
	free(mdoWhileStatement);
}

  void
freeMifStatement(mifStatementBodyType *mifStatement)
{
	nullFree(mifStatement);
	freeExpression(mifStatement->mifCondition);
	freeBlock(mifStatement->mifConsequence);
	freeMifStatement(mifStatement->mifContinuation.
		mifContinuationBodyUnion);
	free(mifStatement);
}

  void
freeMswitchStatement(mswitchStatementBodyType *mswitchStatement)
{
	freeExpression(mswitchStatement->switchExpression);
	freeCaseList(mswitchStatement->cases);
	free(mswitchStatement);
}

  void
freeMforStatement(mforStatementBodyType *mforStatement)
{
	nullFree(mforStatement);
	freeExpression(mforStatement->initExpression);
	freeExpression(mforStatement->testExpression);
	freeExpression(mforStatement->incrExpression);
	freeBlock(mforStatement->forLoop);
	free(mforStatement);
}

  void
freeMvariableStatement(mvariableStatementBodyType *mvariableStatement)
{
	nullFree(mvariableStatement);
	if ((int)mvariableStatement->theDimension != -1)
		freeExpression(mvariableStatement->theDimension);
	freeExpressionList(mvariableStatement->theValue);
	free(mvariableStatement);
}

  void
freeMwhileStatement(mwhileStatementBodyType *mwhileStatement)
{
	nullFree(mwhileStatement);
	freeExpression(mwhileStatement->mwhileCondition);
	freeBlock(mwhileStatement->mwhileLoop);
	free(mwhileStatement);
}

  void
freeOrgStatement(orgStatementBodyType *orgStatement)
{
	nullFree(orgStatement);
	freeExpression(orgStatement);
}

  void
freePerformStatement(performStatementBodyType *performStatement)
{
	nullFree(performStatement);
	freeExpression(performStatement);
}

  void
freeRelStatement(relStatementBodyType *relStatement)
{
}

  void
freeStartStatement(startStatementBodyType *startStatement)
{
	nullFree(startStatement);
	freeExpression(startStatement);
}

  void
freeStringStatement(stringStatementBodyType *stringStatement)
{
	stringStatementBodyType	*newStringStatement;

	nullFree(stringStatement);
	while (stringStatement != NULL) {
		freeExpression(stringStatement->theExpression);
		newStringStatement = stringStatement->nextExpression;
		free(stringStatement);
		stringStatement = newStringStatement;
	}
}

  void
freeStructStatement(structStatementBodyType *structStatement)
{
	nullFree(structStatement);
	freeBlock(structStatement->structBody);
	free(structStatement);
}

  void
freeTargetStatement(targetStatementBodyType *targetStatement)
{
	nullFree(targetStatement);
	freeExpression(targetStatement);
}

  void
freeUndefineStatement(undefineStatementBodyType *undefineStatement)
{
	freeIdentifierList(undefineStatement);
}

  void
freeVariableStatement(variableStatementBodyType *variableStatement)
{
	nullFree(variableStatement);
	if ((int)variableStatement->theDimension != -1)
		freeExpression(variableStatement->theDimension);
	freeExpressionList(variableStatement->theValue);
	free(variableStatement);
}

  void
freeWhileStatement(whileStatementBodyType *whileStatement)
{
	nullFree(whileStatement);
	freeBlock(whileStatement->whileLoop);
	free(whileStatement);
}

  void
freeWordStatement(wordStatementBodyType *wordStatement)
{
	wordStatementBodyType	*newWordStatement;

	nullFree(wordStatement);
	while (wordStatement != NULL) {
		freeExpression(wordStatement->theExpression);
		newWordStatement = wordStatement->nextExpression;
		free(wordStatement);
		wordStatement = newWordStatement;
	}
}

  void
freeStatementBody(statementKindType kind, statementBodyType body)
{
	switch (kind) {

	case ALIGN_STATEMENT:
		freeAlignStatement(body.alignUnion);
		break;

	case ASSERT_STATEMENT:
		freeAssertStatement(body.assertUnion);
		break;

	case BLOCK_STATEMENT:
		freeBlockStatement(body.blockUnion);
		break;

	case BYTE_STATEMENT:
		freeByteStatement(body.byteUnion);
		break;

	case CONSTRAIN_STATEMENT:
		freeConstrainStatement(body.constrainUnion);
		break;

	case DBYTE_STATEMENT:
		freeDbyteStatement(body.dbyteUnion);
		break;

	case DEFINE_STATEMENT:
		freeDefineStatement(body.defineUnion);
		break;

	case DO_UNTIL_STATEMENT:
		freeDoUntilStatement(body.doUntilUnion);
		break;

	case DO_WHILE_STATEMENT:
		freeDoWhileStatement(body.doWhileUnion);
		break;

	case EXTERN_STATEMENT:
		freeExternStatement(body.externUnion);
		break;

	case FRETURN_STATEMENT:
		freeFreturnStatement(body.freturnUnion);
		break;

	case FUNCTION_STATEMENT:
		freeFunctionStatement(body.functionUnion);
		break;

	case GROUP_STATEMENT:
		freeBlock(body.groupUnion);
		break;

	case IF_STATEMENT:
		freeIfStatement(body.ifUnion);
		break;

	case INCLUDE_STATEMENT:
		freeIncludeStatement(body.includeUnion);
		break;

	case INSTRUCTION_STATEMENT:
		freeInstructionStatement(body.instructionUnion);
		break;

	case LONG_STATEMENT:
		freeLongStatement(body.longUnion);
		break;

	case MACRO_STATEMENT:
		freeMacroStatement(body.macroUnion);
		break;

	case MDEFINE_STATEMENT:
		freeMdefineStatement(body.defineUnion);
		break;

	case MDO_UNTIL_STATEMENT:
		freeMdoUntilStatement(body.mdoUntilUnion);
		break;

	case MDO_WHILE_STATEMENT:
		freeMdoWhileStatement(body.mdoWhileUnion);
		break;

	case MIF_STATEMENT:
		freeMifStatement(body.mifUnion);
		break;

	case MSWITCH_STATEMENT:
		freeMswitchStatement(body.mswitchUnion);
		break;

	case MFOR_STATEMENT:
		freeMforStatement(body.mforUnion);
		break;

	case MVARIABLE_STATEMENT:
		freeMvariableStatement(body.mvariableUnion);
		break;

	case MWHILE_STATEMENT:
		freeMwhileStatement(body.mwhileUnion);
		break;

	case NULL_STATEMENT:
		/* do nothing */
		break;

	case ORG_STATEMENT:
		freeOrgStatement(body.orgUnion);
		break;

	case PERFORM_STATEMENT:
		freePerformStatement(body.performUnion);
		break;

	case REL_STATEMENT:
		freeRelStatement(body.relUnion);
		break;

	case START_STATEMENT:
		freeStartStatement(body.startUnion);
		break;

	case STRING_STATEMENT:
		freeStringStatement(body.stringUnion);
		break;

	case STRUCT_STATEMENT:
		freeStructStatement(body.structUnion);
		break;

	case TARGET_STATEMENT:
		freeTargetStatement(body.targetUnion);
		break;

	case UNDEFINE_STATEMENT:
		freeUndefineStatement(body.undefineUnion);
		break;

	case VARIABLE_STATEMENT:
		freeVariableStatement(body.variableUnion);
		break;

	case WHILE_STATEMENT:
		freeWhileStatement(body.whileUnion);
		break;

	case WORD_STATEMENT:
		freeWordStatement(body.wordUnion);
		break;

	default:
		botch("freeStatementBody doesn't know kind %d\n", kind);
		break;
	}
}

  void
freeLabelList(labelListType *labelList)
{
	labelListType	*nextLabel;

	while (labelList != NULL) {
		nextLabel = labelList->nextLabel;
		free(labelList);
		labelList = nextLabel;
	}
}

  void
freeStatement(statementType *statement)
{
	statementType	*newStatement;

	while (statement != NULL) {
		freeLabelList(statement->labels);
		freeStatementBody(statement->kindOfStatement, statement->
			statementBody);
		newStatement = statement->nextStatement;
		free(statement);
		statement = newStatement;
	}
}

  void
freeArray(arrayType *array)
{
	int	i;

	if (array->arraySize > 0) {
		for (i=0; i<array->arraySize; i++)
			freeValue((array->arrayContents)[i]);
		free(array->arrayContents);
	}
	free(array);
}

  void
freeValue(valueType *value)
{
	if (value->kindOfValue == STRING_VALUE)
		freeString(value->value);
	else if (value->kindOfValue == ARRAY_VALUE)
		freeArray(value->value);
	free(value);
}
