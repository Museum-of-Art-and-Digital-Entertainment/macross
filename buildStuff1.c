/*
	buildStuff1.c -- Routines to build parse-tree structures for the
			Macross assembler.  This module is target processor
			independent.

	Chip Morningstar -- Lucasfilm Ltd.

	1-November-1984

*/

#include "macrossTypes.h"
#include "macrossGlobals.h"

symbolTableEntryType		*lookupOrEnterSymbol();
char				*saveString();

/* Generic routine to create statement nodes */
  statementType *
newStatement(kind, body)
  statementKindType	 kind;
  statementBodyType	 body;
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
buildAlignStatement(expression)
  expressionType *expression;
{
	return(newStatement(ALIGN_STATEMENT,
	       (alignStatementBodyType *) expression));
}

  statementType *
buildAssertStatement(condition, message)
  expressionType	*condition;
  expressionType	*message;
{
	assertStatementBodyType	*result;

	result = typeAlloc(assertStatementBodyType);

	result->condition = condition;
	result->message = message;
	return(newStatement(ASSERT_STATEMENT, result));
}

  statementType *
buildBlockStatement(expressionList)
  expressionListType *expressionList;
{
	return(newStatement(BLOCK_STATEMENT,
	       (blockStatementBodyType *) expressionList));
}

  statementType *
buildByteStatement(expressionList)
  expressionListType *expressionList;
{
	return(newStatement(BYTE_STATEMENT,
	       (byteStatementBodyType *) expressionList));
}

  statementType *
buildConstrainStatement(expression, block)
  expressionType	*expression;
  blockType		*block;
{
	constrainStatementBodyType	*result;

	result = typeAlloc(constrainStatementBodyType);

	result->constraint = expression;
	result->constrainedBlock = block;
	return(newStatement(CONSTRAIN_STATEMENT, result));
}

  statementType *
buildDbyteStatement(expressionList)
  expressionListType *expressionList;
{
	return(newStatement(DBYTE_STATEMENT,
	       (dbyteStatementBodyType *) expressionList));
}

  statementType *
buildDefineStatement(name, value)
  stringType		*name;
  expressionType	*value;
{
	defineStatementBodyType	*result;

	result = typeAlloc(defineStatementBodyType);
	result->theSymbol = lookupOrEnterSymbol(name, UNKNOWN_SYMBOL);
	result->theValue = value;
	return(newStatement(DEFINE_STATEMENT, result));
}

  statementType *
buildDoUntilStatement(body, condition)
  blockType	*body;
  conditionType	 condition;
{
	doUntilStatementBodyType	*result;

	result = typeAlloc(doUntilStatementBodyType);
	result->doUntilCondition = condition;
	result->doUntilLoop = body;
	return(newStatement(DO_UNTIL_STATEMENT,result));
}

  statementType *
buildDoWhileStatement(body, condition)
  blockType	*body;
  conditionType	 condition;
{
	doWhileStatementBodyType	*result;

	result = typeAlloc(doWhileStatementBodyType);
	result->doWhileCondition = condition;
	result->doWhileLoop = body;
	return(newStatement(DO_WHILE_STATEMENT, result));
}

  statementType *
buildDoStatement(body, end)
  blockType	*body;
  doEndType	*end;
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
buildExternStatement(identifierList)
  identifierListType	*identifierList;
{
	return(newStatement(EXTERN_STATEMENT,
	       (externStatementBodyType *) identifierList));
}

  statementType *
buildFreturnStatement(expression)
  expressionType	*expression;
{
	return(newStatement(FRETURN_STATEMENT,
	       (freturnStatementBodyType *) expression));
}

  statementType *
buildFunctionStatement(name, arguments, body)
  stringType			*name;
  argumentDefinitionListType	*arguments;
  blockType			*body;
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
	return(newStatement(FUNCTION_STATEMENT, result));
}

  statementType *
buildGroupStatement(block)
  blockType		*block;
{
	return(newStatement(GROUP_STATEMENT,
		(groupStatementBodyType *) block));
}

  statementType *
buildIfStatement(head, continuation, continuationKind)
  ifHeadType			*head;
  ifContinuationType		 continuation;
  ifContinuationKindType	 continuationKind;
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
	return(newStatement(IF_STATEMENT, result));
}

  statementType *
buildIncludeStatement(filename)
  expressionType	*filename;
{
	return(newStatement(INCLUDE_STATEMENT,
	       (includeStatementBodyType *) filename));
}

  statementType *
buildInstructionStatement(opcode, operands)
  opcodeTableEntryType	*opcode;
  operandListType	*operands;
{
	instructionStatementBodyType	*result;

	result = typeAlloc(instructionStatementBodyType);
	result->kindOfInstruction = OPCODE_INSTRUCTION;
	result->theInstruction.opcodeUnion = opcode;
	result->theOperands = operands;
	return(newStatement(INSTRUCTION_STATEMENT, result));
}

  statementType *
buildLongStatement(expressionList)
  expressionListType	*expressionList;
{
	return(newStatement(LONG_STATEMENT,
	       (longStatementBodyType *) expressionList));
}

  statementType *
buildMacroStatement(macro, arguments, body)
  macroTableEntryType		*macro;
  argumentDefinitionListType	*arguments;
  blockType			*body;
{
	macroStatementBodyType		*result;

	result = typeAlloc(macroStatementBodyType);
	result->theMacro = macro;
	result->theArguments = arguments;
	result->theBlock = body;
	return(newStatement(MACRO_STATEMENT, result));
}

  statementType *
buildMacroInstructionStatement(macro, operands)
  macroTableEntryType	*macro;
  operandListType	*operands;
{
	instructionStatementBodyType	*result;

	result = typeAlloc(instructionStatementBodyType);
	result->kindOfInstruction = MACRO_INSTRUCTION;
	result->theInstruction.macroUnion = macro;
	result->theOperands = operands;
	return(newStatement(INSTRUCTION_STATEMENT, result));
}

  statementType *
buildMdefineStatement(name, value)
  stringType		*name;
  expressionType	*value;
{
	mdefineStatementBodyType	*result;

	result = typeAlloc(mdefineStatementBodyType);
	result->theSymbol = lookupOrEnterSymbol(name, DEAD_SYMBOL);
	result->theValue = value;
	return(newStatement(MDEFINE_STATEMENT, result));
}

  statementType *
buildMdoUntilStatement(body, condition)
  blockType		*body;
  ExpressionType	*condition;
{
	mdoUntilStatementBodyType	*result;

	result = typeAlloc(mdoUntilStatementBodyType);
	result->mdoUntilCondition = condition;
	result->mdoUntilLoop = body;
	return(newStatement(MDO_UNTIL_STATEMENT,result));
}

  statementType *
buildMdoWhileStatement(body, condition)
  blockType		*body;
  expressionType	*condition;
{
	mdoWhileStatementBodyType	*result;

	result = typeAlloc(mdoWhileStatementBodyType);
	result->mdoWhileCondition = condition;
	result->mdoWhileLoop = body;
	return(newStatement(MDO_WHILE_STATEMENT, result));
}

  statementType *
buildMdoStatement(body, end)
  blockType	*body;
  doEndType	*end;
{
	statementType	*result;

	if (end->doEndKind == UNTIL_END)
		result = buildMdoUntilStatement(body, end->doEndCondition);
	else if (end->doEndKind == WHILE_END)
		result = buildMdoWhileStatement(body, end->doEndCondition);
	else
		botch("bad mdo-end kind: %d\n", end->doEndCondition);
	qfree(end);
	return(result);
}

  statementType *
buildMforStatement(forExpressions, body)
  forExpressionsType	*forExpressions;
  blockType		*body;
{
	mforStatementBodyType	*result;

	result = typeAlloc(mforStatementBodyType);
	result->initExpression = forExpressions->initExpression;
	result->testExpression = forExpressions->testExpression;
	result->incrExpression = forExpressions->incrExpression;
	result->forLoop = body;
	qfree(forExpressions);
	return(newStatement(MFOR_STATEMENT, result));
}

  statementType *
buildMifStatement(head, continuation, continuationKind)
  mifHeadType			*head;
  mifContinuationType		 continuation;
  ifContinuationKindType	 continuationKind;
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
	return(newStatement(MIF_STATEMENT, result));
}

  statementType *
buildMswitchStatement(switchExpression, cases)
  expressionType	*switchExpression;
  caseListType		*cases;
{
	mswitchStatementBodyType	*result;

	result = typeAlloc(mswitchStatementBodyType);
	result->switchExpression = switchExpression;
	result->cases = cases;
	return(newStatement(MSWITCH_STATEMENT, result));
}

  statementType *
buildMvariableStatement(name, value, dimension)
  stringType		*name;
  expressionListType	*value;
  expressionType	*dimension;
{
	mvariableStatementBodyType	*result;

	result = typeAlloc(mvariableStatementBodyType);
	result->theSymbol = lookupOrEnterSymbol(name, DEAD_SYMBOL);
	result->theValue = value;
	result->theDimension = dimension;
	return(newStatement(MVARIABLE_STATEMENT, result));
}

  statementType *
buildMwhileStatement(condition, body)
  expressionType	*condition;
  blockType		*body;
{
	mwhileStatementBodyType	*result;

	result = typeAlloc(mwhileStatementBodyType);
	result->mwhileCondition = condition;
	result->mwhileLoop = body;
	return(newStatement(MWHILE_STATEMENT, result));
}

  statementType *
buildNullStatement()
{
	return(newStatement(NULL_STATEMENT, NULL));
}

  statementType *
buildOrgStatement(expression)
  expressionType	*expression;
{
	return(newStatement(ORG_STATEMENT,
	       (orgStatementBodyType *) expression));
}

  statementType *
buildPerformStatement(expression)
  expressionType	*expression;
{
	return(newStatement(PERFORM_STATEMENT,
		(performStatementBodyType *) expression));
}

  statementType *
buildRelStatement()
{
	return(newStatement(REL_STATEMENT, NULL));
}

  statementType *
buildStartStatement(expression)
  expressionType	*expression;
{
	return(newStatement(START_STATEMENT,
	       (startStatementBodyType *) expression));
}

  statementType *
buildStringStatement(expressionList)
  expressionListType	*expressionList;
{
	return(newStatement(STRING_STATEMENT,
	       (stringStatementBodyType *) expressionList));
}

  statementType *
buildStructStatement(name, body)
  symbolTableEntryType	*name;
  blockType		*body;
{
	structStatementBodyType		*result;

	result = typeAlloc(structStatementBodyType);
	result->structBody = body;
	result->structName = name;
	return(newStatement(STRUCT_STATEMENT, result));
}

  statementType *
buildTargetStatement(expression)
  expressionType	*expression;
{
	return(newStatement(TARGET_STATEMENT,
	       (targetStatementBodyType *) expression));
}

  statementType *
buildUndefineStatement(identifierList)
  identifierListType	*identifierList;
{
	return(newStatement(UNDEFINE_STATEMENT,
	       (undefineStatementBodyType *) identifierList));
}

  statementType *
buildVariableStatement(name, value, dimension)
  stringType		*name;
  expressionListType	*value;
  expressionType	*dimension;
{
	variableStatementBodyType	*result;

	result = typeAlloc(variableStatementBodyType);
	result->theSymbol = lookupOrEnterSymbol(name, VARIABLE_SYMBOL);
	result->theValue = value;
	result->theDimension = dimension;
	return(newStatement(VARIABLE_STATEMENT, result));
}

  statementType *
buildWhileStatement(condition, body)
  conditionType		 condition;
  blockType		*body;
{
	whileStatementBodyType	*result;

	result = typeAlloc(whileStatementBodyType);
	result->whileCondition = condition;
	result->whileLoop = body;
	return(newStatement(WHILE_STATEMENT, result));
}

  statementType *
buildWordStatement(expressionList)
  expressionListType	*expressionList;
{
	return(newStatement(WORD_STATEMENT,
	       (wordStatementBodyType *) expressionList));
}

