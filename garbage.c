/*
	garbage.c -- Routines to eat up a parse tree and throw it away

	Chip Morningstar -- Lucasfilm Ltd.

	4-February-1985
*/

#include "macrossTypes.h"
#include "macrossGlobals.h"
#include "y.tab.h"

#define nullFree(thing) if (thing == NULL) return;

  void
freeArrayTerm(arrayTerm)
  arrayTermType	*arrayTerm;
{
	void	freeExpression();

	nullFree(arrayTerm);
	freeExpression(arrayTerm->arrayName);
	freeExpression(arrayTerm->arrayIndex);
	free(arrayTerm);
}

  void
freeAssignmentTerm(assignmentTerm)
  binopTermType	*assignmentTerm;
{
	void	freeExpression();

	nullFree(assignmentTerm);
	freeExpression(assignmentTerm->rightArgument);
	free(assignmentTerm);
}

  void
freeBinopTerm(binopTerm)
  binopTermType	*binopTerm;
{
	void	freeExpression();

	nullFree(binopTerm);
	freeExpression(binopTerm->leftArgument);
	if (binopTerm->binop != SELECT)
		freeExpression(binopTerm->rightArgument);
	free(binopTerm);
}

  void
freeFunctionCall(functionCall)
  functionCallTermType	*functionCall;
{
	void	freeOperandList();

	nullFree(functionCall);
	freeOperandList(functionCall->parameters);
	free(functionCall);
}

  void
freePostopTerm(postopTerm)
  postOpTermType	*postopTerm;
{
	nullFree(postopTerm);
	free(postopTerm);
}

  void
freePreopTerm(preopTerm)
  preOpTermType	*preopTerm;
{
	nullFree(preopTerm);
	free(preopTerm);
}

  void
freeString(string)
  stringType	*string;
{
	nullFree(string);
	free(string);
}

  void
freeUnopTerm(unopTerm)
  unopTermType	*unopTerm;
{
	void	freeExpression();

	nullFree(unopTerm);
	freeExpression(unopTerm->unopArgument);
	free(unopTerm);
}

  void
freeExpression(expression)
  expressionType	*expression;
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
		freeArrayTerm(expression->expressionTerm);
		break;

	case ASSIGN_EXPR:
		freeAssignmentTerm(expression->expressionTerm);
		break;

	case BINOP_EXPR:
		freeBinopTerm(expression->expressionTerm);
		break;

	case FUNCTION_CALL_EXPR:
		freeFunctionCall(expression->expressionTerm);
		break;

	case POSTOP_EXPR:
		freePostopTerm(expression->expressionTerm);
		break;

	case PREOP_EXPR:
		freePreopTerm(expression->expressionTerm);
		break;

	case SUBEXPRESSION_EXPR:
		freeExpression(expression->expressionTerm);
		break;

	case UNOP_EXPR:
		freeUnopTerm(expression->expressionTerm);
		break;

	default:
		botch("freeExpression: funny expression kind %d\n",
						expression->kindOfTerm);
		break;
	}
	free(expression);
}

  void
freeExpressionList(expressionList)
  expressionListType	*expressionList;
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
freeIdentifierList(identifierList)
  identifierListType	*identifierList;
{
	identifierListType	*newIdentifierList;

	while (identifierList != NULL) {
		newIdentifierList = identifierList->nextIdentifier;
		free(identifierList);
		identifierList = newIdentifierList;
	}
}

  void
freeSelectionList(selectionList)
  selectionListType	*selectionList;
{
	freeIdentifierList(selectionList);
}

  void
freeBlock(block)
  blockType	*block;
{
	void	freeStatement();

	nullFree(block);
	freeStatement(block);
}

  void
freeCase(aCase)
  caseType	*aCase;
{
	freeExpressionList(aCase->caseTags);
	freeBlock(aCase->caseBody);
	free(aCase);
}

  void
freeCaseList(caseList)
  caseListType	*caseList;
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
freeOperandList(operandList)
  operandListType	*operandList;
{
	freeOperand(operandList);
}

  void
freeMacro(operands)
  operandListType	*operands;
{
	freeOperandList(operands);
}

  void
freeMachineInstruction(operands)
  operandListType	*operands;
{
	freeOperandList(operands);
}

  void
freeAlignStatement(alignStatement)
  alignStatementBodyType	*alignStatement;
{
	nullFree(alignStatement);
	freeExpression(alignStatement);
}

  void
freeAssertStatement(assertStatement)
  assertStatementBodyType	*assertStatement;
{
	nullFree(assertStatement);
	freeExpression(assertStatement->condition);
	freeExpression(assertStatement->message);
	free(assertStatement);
}

  void
freeBlockStatement(blockStatement)
  blockStatementBodyType	*blockStatement;
{
	nullFree(blockStatement);
	freeExpressionList(blockStatement);
}

  void
freeByteStatement(byteStatement)
  byteStatementBodyType	*byteStatement;
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
freeConstrainStatement(constrainStatement)
  constrainStatementBodyType	*constrainStatement;
{
	nullFree(constrainStatement);
	freeExpression(constrainStatement->constraint);
	freeBlock(constrainStatement->constrainedBlock);
	free(constrainStatement);
}

  void
freeDbyteStatement(dbyteStatement)
  dbyteStatementBodyType	*dbyteStatement;
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
freeDefineStatement(defineStatement)
  defineStatementBodyType	*defineStatement;
{
	nullFree(defineStatement);
	freeExpression(defineStatement->theValue);
	free(defineStatement);
}

  void
freeDoUntilStatement(doUntilStatement)
  doUntilStatementBodyType	*doUntilStatement;
{
	nullFree(doUntilStatement);
	freeBlock(doUntilStatement->doUntilLoop);
	free(doUntilStatement);
}

  void
freeDoWhileStatement(doWhileStatement)
  doWhileStatementBodyType	*doWhileStatement;
{
	nullFree(doWhileStatement);
	freeBlock(doWhileStatement->doWhileLoop);
	free(doWhileStatement);
}

  void
freeExternStatement(externStatement)
  externStatementBodyType	*externStatement;
{
	freeIdentifierList(externStatement);
}

  void
freeFreturnStatement(freturnStatement)
  freturnStatementBodyType	*freturnStatement;
{
	nullFree(freturnStatement);
	freeExpression(freturnStatement);
}

  void
freeFunctionStatement(functionStatement)
  functionStatementBodyType	*functionStatement;
{
	nullFree(functionStatement);
	free(functionStatement->functionName);
	free(functionStatement);
}

  void
freeIfStatement(ifStatement)
  ifStatementBodyType	*ifStatement;
{
	nullFree(ifStatement);
	if (ifStatement->consequence != NULL)
		freeBlock(ifStatement->consequence);
	if (ifStatement->continuation.continuationBodyUnion != NULL) {
		freeIfStatement(ifStatement->continuation);
	}
	free(ifStatement);
}

  void
freeIncludeStatement(includeStatement)
  includeStatementBodyType	*includeStatement;
{
	nullFree(includeStatement);
	freeExpression(includeStatement);
}

  void
freeInstructionStatement(instructionStatement)
  instructionStatementBodyType	*instructionStatement;
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
freeLongStatement(longStatement)
  longStatementBodyType	*longStatement;
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
freeMacroStatement(macroStatement)
  macroStatementBodyType	*macroStatement;
{
	nullFree(macroStatement);
	free(macroStatement);
}

  void
freeMdefineStatement(mdefineStatement)
  defineStatementBodyType	*mdefineStatement;
{
	valueType		*freeDefineExpression();

	nullFree(mdefineStatement);
	freeExpression(mdefineStatement->theValue);
	free(mdefineStatement);
}

  void
freeMdoUntilStatement(mdoUntilStatement)
  mdoUntilStatementBodyType	*mdoUntilStatement;
{
	nullFree(mdoUntilStatement);
	freeBlock(mdoUntilStatement->mdoUntilLoop);
	freeExpression(mdoUntilStatement->mdoUntilCondition);
}

  void
freeMdoWhileStatement(mdoWhileStatement)
  mdoWhileStatementBodyType	*mdoWhileStatement;
{
	nullFree(mdoWhileStatement);
	freeBlock(mdoWhileStatement->mdoWhileLoop);
	freeExpression(mdoWhileStatement->mdoWhileCondition);
	free(mdoWhileStatement);
}

  void
freeMifStatement(mifStatement)
  mifStatementBodyType	*mifStatement;
{
	nullFree(mifStatement);
	freeExpression(mifStatement->mifCondition);
	freeBlock(mifStatement->mifConsequence);
	freeMifStatement(mifStatement->mifContinuation.
		mifContinuationBodyUnion);
	free(mifStatement);
}

  void
freeMswitchStatement(mswitchStatement)
  mswitchStatementBodyType	*mswitchStatement;
{
	freeExpression(mswitchStatement->switchExpression);
	freeCaseList(mswitchStatement->cases);
	free(mswitchStatement);
}

  void
freeMforStatement(mforStatement)
  mforStatementBodyType	*mforStatement;
{
	nullFree(mforStatement);
	freeExpression(mforStatement->initExpression);
	freeExpression(mforStatement->testExpression);
	freeExpression(mforStatement->incrExpression);
	freeBlock(mforStatement->forLoop);
	free(mforStatement);
}

  void
freeMvariableStatement(mvariableStatement)
  mvariableStatementBodyType	*mvariableStatement;
{
	nullFree(mvariableStatement);
	if ((int)mvariableStatement->theDimension != -1)
		freeExpression(mvariableStatement->theDimension);
	freeExpressionList(mvariableStatement->theValue);
	free(mvariableStatement);
}

  void
freeMwhileStatement(mwhileStatement)
  mwhileStatementBodyType	*mwhileStatement;
{
	nullFree(mwhileStatement);
	freeExpression(mwhileStatement->mwhileCondition);
	freeBlock(mwhileStatement->mwhileLoop);
	free(mwhileStatement);
}

  void
freeOrgStatement(orgStatement)
  orgStatementBodyType	*orgStatement;
{
	nullFree(orgStatement);
	freeExpression(orgStatement);
}

  void
freePerformStatement(performStatement)
  performStatementBodyType	*performStatement;
{
	nullFree(performStatement);
	freeExpression(performStatement);
}

  void
freeRelStatement(relStatement)
  relStatementBodyType	*relStatement;
{
}

  void
freeStartStatement(startStatement)
  startStatementBodyType	*startStatement;
{
	nullFree(startStatement);
	freeExpression(startStatement);
}

  void
freeStringStatement(stringStatement)
  stringStatementBodyType	*stringStatement;
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
freeStructStatement(structStatement)
  structStatementBodyType	*structStatement;
{
	nullFree(structStatement);
	freeBlock(structStatement->structBody);
	free(structStatement);
}

  void
freeTargetStatement(targetStatement)
  targetStatementBodyType	*targetStatement;
{
	nullFree(targetStatement);
	freeExpression(targetStatement);
}

  void
freeUndefineStatement(undefineStatement)
  undefineStatementBodyType	*undefineStatement;
{
	freeIdentifierList(undefineStatement);
}

  void
freeVariableStatement(variableStatement)
  variableStatementBodyType	*variableStatement;
{
	nullFree(variableStatement);
	if ((int)variableStatement->theDimension != -1)
		freeExpression(variableStatement->theDimension);
	freeExpressionList(variableStatement->theValue);
	free(variableStatement);
}

  void
freeWhileStatement(whileStatement)
  whileStatementBodyType	*whileStatement;
{
	nullFree(whileStatement);
	freeBlock(whileStatement->whileLoop);
	free(whileStatement);
}

  void
freeWordStatement(wordStatement)
  wordStatementBodyType	*wordStatement;
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
freeStatementBody(kind, body)
  statementKindType	kind;
  statementBodyType	body;
{
	switch (kind) {

	case ALIGN_STATEMENT:
		freeAlignStatement(body);
		break;

	case ASSERT_STATEMENT:
		freeAssertStatement(body);
		break;

	case BLOCK_STATEMENT:
		freeBlockStatement(body);
		break;

	case BYTE_STATEMENT:
		freeByteStatement(body);
		break;

	case CONSTRAIN_STATEMENT:
		freeConstrainStatement(body);
		break;

	case DBYTE_STATEMENT:
		freeDbyteStatement(body);
		break;

	case DEFINE_STATEMENT:
		freeDefineStatement(body);
		break;

	case DO_UNTIL_STATEMENT:
		freeDoUntilStatement(body);
		break;

	case DO_WHILE_STATEMENT:
		freeDoWhileStatement(body);
		break;

	case EXTERN_STATEMENT:
		freeExternStatement(body);
		break;

	case FRETURN_STATEMENT:
		freeFreturnStatement(body);
		break;

	case FUNCTION_STATEMENT:
		freeFunctionStatement(body);
		break;

	case GROUP_STATEMENT:
		freeBlock(body);
		break;

	case IF_STATEMENT:
		freeIfStatement(body);
		break;

	case INCLUDE_STATEMENT:
		freeIncludeStatement(body);
		break;

	case INSTRUCTION_STATEMENT:
		freeInstructionStatement(body);
		break;

	case LONG_STATEMENT:
		freeLongStatement(body);
		break;

	case MACRO_STATEMENT:
		freeMacroStatement(body);
		break;

	case MDEFINE_STATEMENT:
		freeMdefineStatement(body);
		break;

	case MDO_UNTIL_STATEMENT:
		freeMdoUntilStatement(body);
		break;

	case MDO_WHILE_STATEMENT:
		freeMdoWhileStatement(body);
		break;

	case MIF_STATEMENT:
		freeMifStatement(body);
		break;

	case MSWITCH_STATEMENT:
		freeMswitchStatement(body);
		break;

	case MFOR_STATEMENT:
		freeMforStatement(body);
		break;

	case MVARIABLE_STATEMENT:
		freeMvariableStatement(body);
		break;

	case MWHILE_STATEMENT:
		freeMwhileStatement(body);
		break;

	case NULL_STATEMENT:
		/* do nothing */
		break;

	case ORG_STATEMENT:
		freeOrgStatement(body);
		break;

	case PERFORM_STATEMENT:
		freePerformStatement(body);
		break;

	case REL_STATEMENT:
		freeRelStatement(body);
		break;

	case START_STATEMENT:
		freeStartStatement(body);
		break;

	case STRING_STATEMENT:
		freeStringStatement(body);
		break;

	case STRUCT_STATEMENT:
		freeStructStatement(body);
		break;

	case TARGET_STATEMENT:
		freeTargetStatement(body);
		break;

	case UNDEFINE_STATEMENT:
		freeUndefineStatement(body);
		break;

	case VARIABLE_STATEMENT:
		freeVariableStatement(body);
		break;

	case WHILE_STATEMENT:
		freeWhileStatement(body);
		break;

	case WORD_STATEMENT:
		freeWordStatement(body);
		break;

	default:
		botch("freeStatementBody doesn't know kind %d\n", kind);
		break;
	}
}

  void
freeLabelList(labelList)
  labelListType	*labelList;
{
	labelListType	*nextLabel;

	while (labelList != NULL) {
		nextLabel = labelList->nextLabel;
		free(labelList);
		labelList = nextLabel;
	}
}

  void
freeStatement(statement)
  statementType	*statement;
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
freeArray(array)
  arrayType	*array;
{
	int	i;
	void	freeValue();

	if (array->arraySize > 0) {
		for (i=0; i<array->arraySize; i++)
			freeValue((array->arrayContents)[i]);
		free(array->arrayContents);
	}
	free(array);
}

  void
freeValue(value)
  valueType	*value;
{
	if (value->kindOfValue == STRING_VALUE)
		freeString(value->value);
	else if (value->kindOfValue == ARRAY_VALUE)
		freeArray(value->value);
	free(value);
}
