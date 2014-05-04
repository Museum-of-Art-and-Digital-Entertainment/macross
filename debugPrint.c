/*
	debugPrint.c -- Routines to print out things for debugging purposes.

	Chip Morningstar -- Lucasfilm Ltd.

	5-November-1984

*/

#include "macrossTypes.h"
#include "macrossGlobals.h"
#include "y.tab.h"

/*
   Print out the parse-tree.
 */

int	tablevel = 0;

/* Fundamental nop print operation */
#define nullPrint(thing)   if (thing==NULL) { tab(); printf("()\n"); return; }

/* For keeping nested structures looking pretty */
  void
tab()
{
	int	n;

	n = tablevel;
	while (n-- > 0)
		printf("    ");
}

/* 
   The following routines print readable strings for the values of various
   enumerated types and pre-defined constants.  Each contains a static table
   of strings that must be kept consistent with the type or set of
   definitions that it represents.
 */

  void
printAssignmentKind(assignmentKind)
  assignmentKindType	assignmentKind;
{
/* This table MUST be maintained congruently with the definition of the
   enumerated type 'assignmentKindType'. */
	static char *assignmentTable[] = {
		"ASSIGN_ASSIGN",
		"ADD_ASSIGN",
		"SUB_ASSIGN",
		"MUL_ASSIGN",
		"DIV_ASSIGN",
		"MOD_ASSIGN",
		"AND_ASSIGN",
		"OR_ASSIGN",
		"XOR_ASSIGN",
		"LEFT_SHIFT_ASSIGN",
		"RIGHT_SHIFT_ASSIGN",
		"NO_ASSIGN",
	};

	printf("%s", assignmentTable[(int)assignmentKind]);
}

  void
printExpressionKind(kind)
  expressionTermKindType	kind;
{
/* This table MUST be maintained congruently with the definition of the
   enumerated type 'expressionTermKindType'. */
	static char *expressionKindTable[] = {
		"identifier expr",
		"function call expr",
		"number expr",
		"subexpression",
		"unop expr",
		"binop expr",
		"preop expr",
		"postop expr",
		"here",
		"assignment expr",
		"condition code expr",
		"string expr",
		"value",
		"array expr",
	};

	printf("%s", expressionKindTable[(int)kind]);
}

  stringType *
statementKindString(kind)
  statementKindType	kind;
{
/* This table MUST be maintained congruently with the definition of
   the enumerated type 'statementKindType'. */
static char *statementKindTable[] = {
		"if",
		"while",
		"do_while",
		"do_until",
		"word",
		"dbyte",
		"byte",
		"string",
		"struct",
		"instruction",
		"define",
		"undefine",
		"mif",
		"freturn",
		"mfor",
		"mdo_while",
		"mdo_until",
		"include",
		"extern",
		"start",
		"align",
		"org",
		"rel",
		"perform",
		"block",
		"mwhile",
		"mdefine",
		"null",
		"group",
		"macro",
		"function",
		"variable",
		"mvariable",
		"constrain",
		"assert",
		"mswitch",
		"long",
		"target",
	};
	return(statementKindTable[(int)kind]);
}

  void
printStatementKind(kind)
  statementKindType	kind;
{
	printf("%s", statementKindString(kind));
}


/*
   These routines collectively print expressions and values.
 */

  void
printValue(value)
  valueType	*value;
{
/* This table MUST be maintained congruently with the definition of the
   enumerated type 'valueKindType'. */
	static char *valueKindTable[NUM_OF_VALUE_KINDS] = {
		"ABSOLUTE_VALUE",
		"DATA_VALUE",
		"RELOCATABLE_VALUE",
		"BSS_VALUE",
		"STRUCT_VALUE",
		"FIELD_VALUE",
		"MACRO_VALUE",
		"OPERAND_VALUE",
		"STRING_VALUE",
		"CONDITION_VALUE",
		"UNDEFINED_VALUE",
		"FUNCTION_VALUE",
		"BLOCK_VALUE",
		"BUILT_IN_FUNCTION_VALUE",
		"ARRAY_VALUE",
		"FAIL",
	};

	if (value != NULL) {
		printf("(%s: %d==0x%x ", valueKindTable[(int)value->
				kindOfValue], value->value, value->value);
		printOperandKind(value->addressMode);
		printf(")");
	} else
		printf("(no value)");
}

  void
printSymbol(symbol)
  symbolTableEntryType	*symbol;
{
/* This table MUST be maintained congruent with the definition of the
   enumerated type 'symbolUsageKindType'. */
	static char *symbolUsageTable[] = {
		"STRUCT_NAME_SYMBOL",
		"STRUCT_FIELD_SYMBOL",
		"MACRO_SYMBOL",
		"ARGUMENT_SYMBOL",
		"LABEL_SYMBOL",
		"EXTERNAL_SYMBOL",
		"VARIABLE_SYMBOL",
		"MVARIABLE_SYMBOL",
		"UNKNOWN_SYMBOL",
		"DEAD_SYMBOL",
		"FUNCTION_SYMBOL",
		"BUILT_IN_FUNCTION_SYMBOL",
		"NESTED_UNKNOWN_SYMBOL",
		"DEFINE_SYMBOL",
		"MDEFINE_SYMBOL",
		"UNKNOWN_FUNCTION_SYMBOL",
		"UNKNOWN_MACRO_SYMBOL",
	};

	nullPrint(symbol);
	tab(); printf("(symbol: %s ", symbol->symbolName);
	printValue(symbol->context->value);
	printf(" %d %s", symbol->context->attributes,
			symbolUsageTable[(int)symbol->context->usage]);
	if (symbol->context->pushedContexts != NULL)
		printf(" ...");
	printf(")\n");
}

  void
printArgumentDefinitionList(list)
  argumentDefinitionListType	*list;
{
	nullPrint(list);
	tab(); printf("(arguments:\n");
	tablevel++;
	while (list != NULL) {
		printSymbol(list->theArgument);
		list = list->nextArgument;
	}
	tablevel--;
	tab(); printf(")\n");
}

  void
printBlock(block)
  blockType	*block;
{
	void	printStatement();

	nullPrint(block);
	tab(); printf("(block:\n");
	tablevel++;
	printStatement(block);
	tablevel--;
	tab(); printf(")\n");
}

  void
printArrayTerm(arrayTerm)
  arrayTermType	*arrayTerm;
{
	void	printIdentifier();
	void	printExpression();

	nullPrint(arrayTerm);
	tab(); printf("(array\n");
	tablevel++;
	printExpression(arrayTerm->arrayName);
	printExpression(arrayTerm->arrayIndex);
	tablevel--;
	tab(); printf(")\n");
}

  void
printAssignmentTerm(assignmentTerm)
  binopTermType	*assignmentTerm;
{
	void	printExpression();

	nullPrint(assignmentTerm);
	tab(); printf("(assignment [");
	       printAssignmentKind(assignmentTerm->binop);
	       printf("]\n");
	tablevel++;
	printExpression(assignmentTerm->leftArgument);
	printExpression(assignmentTerm->rightArgument);
	tablevel--;
	tab(); printf(")\n");
}

  void
printBinopTerm(binopTerm)
  binopTermType	*binopTerm;
{
	void	printIdentifier();
	void	printExpression();

	nullPrint(binopTerm);
	tab(); printf("(binop [");
	printToken(binopTerm->binop);
	printf("]\n");
	tablevel++;
	printExpression(binopTerm->leftArgument);
	if (binopTerm->binop == SELECT)
		printIdentifier(binopTerm->rightArgument);
	else
		printExpression(binopTerm->rightArgument);
	tablevel--;
	tab(); printf(")\n");
}

  void
printFunctionCall(functionCall)
  functionCallTermType	*functionCall;
{
	void	printOperandList();

	nullPrint(functionCall);
	tab(); printf("(function call %s\n", functionCall->functionName->
			symbolName);
	tablevel++;
	printOperandList(functionCall->parameters);
	tablevel--;
	tab(); printf(")\n");
}

  void
printHere()
{
	tab(); printf("(here)\n");
}

  void
printIdentifier(identifier)
  symbolTableEntryType	*identifier;
{
	nullPrint(identifier);
	printSymbol(identifier);
}

  void
printNumber(number)
  numberTermType	number;
{
	tab(); printf("(number: %d)\n", number);
}

  void
printPostopTerm(postopTerm)
  postOpTermType	*postopTerm;
{
	nullPrint(postopTerm);
	tab(); printf("(postop [");
	       printToken(postopTerm->postOp);
	       printf("]\n");
	tablevel++;
	printExpression(postopTerm->postOpArgument);
	tablevel--;
	tab(); printf(")\n");
}

  void
printPreopTerm(preopTerm)
  preOpTermType	*preopTerm;
{
	nullPrint(preopTerm);
	tab(); printf("(preop [");
	       printToken(preopTerm->preOp);
	       printf("]\n");
	tablevel++;
	printExpression(preopTerm->preOpArgument);
	tablevel--;
	tab(); printf(")\n");
}

  void
printUnopTerm(unopTerm)
  unopTermType	*unopTerm;
{
	void	printExpression();

	nullPrint(unopTerm);
	tab(); printf("(unop [");
	       printToken(unopTerm->unop);
	       printf("]\n");
	tablevel++;
	printExpression(unopTerm->unopArgument);
	tablevel--;
	tab(); printf(")\n");
}

  void
printExpression(expression)
  expressionType	*expression;
{
	nullPrint(expression);
	tab(); printf("(expression: [");
	printExpressionKind(expression->kindOfTerm);
	printf("]\n");
	tablevel++;
	switch (expression->kindOfTerm) {

	case ARRAY_EXPR:
		printArrayTerm(expression->expressionTerm);
		break;

	case ASSIGN_EXPR:
		printAssignmentTerm(expression->expressionTerm);
		break;

	case BINOP_EXPR:
		printBinopTerm(expression->expressionTerm);
		break;

	case CONDITION_CODE_EXPR:
		printCondition(expression->expressionTerm);
		break;

	case FUNCTION_CALL_EXPR:
		printFunctionCall(expression->expressionTerm);
		break;

	case HERE_EXPR:
		printHere();
		break;

	case IDENTIFIER_EXPR:
		printIdentifier(expression->expressionTerm);
		break;

	case NUMBER_EXPR:
		printNumber(expression->expressionTerm);
		break;

	case POSTOP_EXPR:
		printPostopTerm(expression->expressionTerm);
		break;

	case PREOP_EXPR:
		printPreopTerm(expression->expressionTerm);
		break;

	case STRING_EXPR:
		tab(); printf("%s\n", expression->expressionTerm);
		break;

	case SUBEXPRESSION_EXPR:
		printExpression(expression->expressionTerm);
		break;

	case UNOP_EXPR:
		printUnopTerm(expression->expressionTerm);
		break;

	case VALUE_EXPR:
		printValue(expression->expressionTerm);
		break;

	default:
		printf("eh?  funny expression kind %d\n",
						expression->kindOfTerm);
		break;
	}
	tablevel--;
	tab(); printf(")\n");
}

  void
printExpressionList(expressionList)
  expressionListType	*expressionList;
{
	while (expressionList != NULL) {
		printExpression(expressionList->theExpression);
		expressionList = expressionList->nextExpression;
	}
}

  void
printIdentifierList(identifierList)
  identifierListType	*identifierList;
{
	nullPrint(identifierList);
	printSymbol(identifierList->theSymbol);
	if (identifierList->nextIdentifier != NULL)
		printIdentifierList(identifierList->nextIdentifier);
}


/* 
   These routines collectively print statements and statement fragments.
 */

  void
printCase(aCase)
  caseType	*aCase;
{
	tab(); printf("(case:\n"); tablevel++;
	if (aCase->caseTags == NULL) {
		tab(); printf("(default case)\n");
	} else {
		printExpressionList(aCase->caseTags);
	}
	tablevel--;
	tab(); printf(")\n");
	tab(); printf("(body:\n"); tablevel++;
	printBlock(aCase->caseBody);
	tablevel--;
	tab(); printf(")\n");
}

  void
printCaseList(caseList)
  caseListType	*caseList;
{
	tab(); printf("(cases:\n");
	tablevel++;
	while (caseList != NULL) {
		printCase(caseList->theCase);
		caseList = caseList->nextCase;
	}
	tablevel--;
	tab(); printf(")\n");
}

  void
printMacro(macroInstruction)
  macroTableEntryType	*macroInstruction;
{
	nullPrint(macroInstruction);
	tab(); printf("(macro: %s)\n", macroInstruction->macroName);
}

  void
printOpcode(opcode)
  opcodeTableEntryType	*opcode;
{
	nullPrint(opcode);
	tab(); printf("(opcode: %s)\n", opcode->mnemonic);
}

  void
printOperandList(operandList)
  operandListType	*operandList;
{
	nullPrint(operandList);
	printOperand(operandList);
	if (operandList->nextOperand != NULL)
		printOperandList(operandList->nextOperand);
}

  void
printAlignStatement(alignStatement)
  alignStatementBodyType	*alignStatement;
{
	nullPrint(alignStatement);
	printExpression(alignStatement);
}

  void
printAssertStatement(assertStatement)
  assertStatementBodyType	*assertStatement;
{
	nullPrint(assertStatement);
	printExpression(assertStatement->condition);
	printExpression(assertStatement->message);
}

  void
printBlockStatement(blockStatement)
  blockStatementBodyType	*blockStatement;
{
	nullPrint(blockStatement);
	printExpressionList(blockStatement);
}

  void
printByteStatement(byteStatement)
  byteStatementBodyType	*byteStatement;
{
	nullPrint(byteStatement);
	printExpressionList(byteStatement);
}

  void
printConstrainStatement(constrainStatement)
  constrainStatementBodyType	*constrainStatement;
{
	nullPrint(constrainStatement);
	printExpression(constrainStatement->constraint);
	printBlock(constrainStatement->constrainedBlock);
}

  void
printDbyteStatement(dbyteStatement)
  dbyteStatementBodyType	*dbyteStatement;
{
	nullPrint(dbyteStatement);
	printExpressionList(dbyteStatement);
}

  void
printDefineStatement(defineStatement)
  defineStatementBodyType	*defineStatement;
{
	nullPrint(defineStatement);
	printSymbol(defineStatement->theSymbol);
	printExpression(defineStatement->theValue);
}

  void
printDoUntilStatement(doUntilStatement)
  doUntilStatementBodyType	*doUntilStatement;
{
	nullPrint(doUntilStatement);
	printBlock(doUntilStatement->doUntilLoop);
	tab(); printf("(condition: ");
	       printCondition(doUntilStatement->doUntilCondition);
	       printf(")\n");
}

  void
printDoWhileStatement(doWhileStatement)
  doWhileStatementBodyType	*doWhileStatement;
{
	nullPrint(doWhileStatement);
	printBlock(doWhileStatement->doWhileLoop);
	tab(); printf("(condition: ");
	       printCondition(doWhileStatement->doWhileCondition);
	       printf(")\n");
}

  void
printExternStatement(externStatement)
  externStatementBodyType	*externStatement;
{
	nullPrint(externStatement);
	printIdentifierList(externStatement);
}

  void
printFreturnStatement(freturnStatement)
  freturnStatementBodyType	*freturnStatement;
{
	nullPrint(freturnStatement);
	printExpression(freturnStatement);
}

  void
printFunctionStatement(functionStatement)
  functionStatementBodyType	*functionStatement;
{
	nullPrint(functionStatement);
	tab();printf("(function name: %s)\n",functionStatement->functionName);
	printArgumentDefinitionList(functionStatement->theArguments);
	printBlock(functionStatement->theBlock);
}

  void
printIfStatement(ifStatement)
  ifStatementBodyType	*ifStatement;
{
	nullPrint(ifStatement);
	tab(); printf("(condition: ");
	       printCondition(ifStatement->ifCondition);
	       printf(")\n");
	printBlock(ifStatement->consequence);
	tab(); printf("(else:\n");
	tablevel++;
	printIfStatement(ifStatement->continuation);
	tablevel--;
	tab(); printf(")\n");
}

  void
printIncludeStatement(includeStatement)
  includeStatementBodyType	*includeStatement;
{
	nullPrint(includeStatement);
	printExpression(includeStatement);
}

  void
printInstructionStatement(instructionStatement)
  instructionStatementBodyType	*instructionStatement;
{
	nullPrint(instructionStatement);
	switch(instructionStatement->kindOfInstruction) {
	case OPCODE_INSTRUCTION:
		printOpcode(instructionStatement->theInstruction);
		break;

	case MACRO_INSTRUCTION:
		printMacro(instructionStatement->theInstruction);
		break;

	default:
		printf("huh? instruction type=%d\n", instructionStatement->
					kindOfInstruction);
		break;
	}
	printOperandList(instructionStatement->theOperands);
}

  void
printLongStatement(longStatement)
  longStatementBodyType	*longStatement;
{
	nullPrint(longStatement);
	printExpressionList(longStatement);
}

  void
printMacroStatement(macroStatement)
  macroStatementBodyType	*macroStatement;
{
	nullPrint(macroStatement);
	tab(); printf("(macro name: %s)\n", macroStatement->theMacro);
	printArgumentDefinitionList(macroStatement->theArguments);
	printBlock(macroStatement->theBlock);
}

  void
printMdefineStatement(mdefineStatement)
  defineStatementBodyType	*mdefineStatement;
{
	nullPrint(mdefineStatement);
	printSymbol(mdefineStatement->theSymbol);
	printExpression(mdefineStatement->theValue);
}

  void
printMdoUntilStatement(mdoUntilStatement)
  mdoUntilStatementBodyType	*mdoUntilStatement;
{
	nullPrint(mdoUntilStatement);
	printBlock(mdoUntilStatement->mdoUntilLoop);
	printExpression(mdoUntilStatement->mdoUntilCondition);
}

  void
printMdoWhileStatement(mdoWhileStatement)
  mdoWhileStatementBodyType	*mdoWhileStatement;
{
	nullPrint(mdoWhileStatement);
	printBlock(mdoWhileStatement->mdoWhileLoop);
	printExpression(mdoWhileStatement->mdoWhileCondition);
}

  void
printMforStatement(mforStatement)
  mforStatementBodyType	*mforStatement;
{
	nullPrint(mforStatement);
	printExpression(mforStatement->initExpression);
	printExpression(mforStatement->testExpression);
	printExpression(mforStatement->incrExpression);
	printBlock(mforStatement->forLoop);
}

  void
printMifStatement(mifStatement)
  mifStatementBodyType	*mifStatement;
{
	nullPrint(mifStatement);
	printExpression(mifStatement->mifCondition);
	printBlock(mifStatement->mifConsequence);
	tab(); printf("(melse:\n");
	tablevel++;
	printMifStatement(mifStatement->mifContinuation);
	tablevel--;
	tab(); printf(")\n");
}

  void
printMswitchStatement(mswitchStatement)
  mswitchStatementBodyType	*mswitchStatement;
{
	nullPrint(mswitchStatement);
	printExpression(mswitchStatement->switchExpression);
	printCaseList(mswitchStatement->cases);
}

  void
printMvariableStatement(mvariableStatement)
  mvariableStatementBodyType	*mvariableStatement;
{
	nullPrint(mvariableStatement);
	printSymbol(mvariableStatement->theSymbol);
	printExpression(mvariableStatement->theValue);
}

  void
printMwhileStatement(mwhileStatement)
  mwhileStatementBodyType	*mwhileStatement;
{
	nullPrint(mwhileStatement);
	printExpression(mwhileStatement->mwhileCondition);
	printBlock(mwhileStatement->mwhileLoop);
}

  void
printOrgStatement(orgStatement)
  orgStatementBodyType	*orgStatement;
{
	nullPrint(orgStatement);
	printExpression(orgStatement);
}

  void
printPerformStatement(performStatement)
  performStatementBodyType	*performStatement;
{
	nullPrint(performStatement);
	printExpression(performStatement);
}

  void
printRelStatement(relStatement)
  relStatementBodyType	*relStatement;
{
	/* there's nothing here... */
}

  void
printStartStatement(startStatement)
  startStatementBodyType	*startStatement;
{
	nullPrint(startStatement);
	printExpression(startStatement);
}

  void
printStringStatement(stringStatement)
  stringStatementBodyType	*stringStatement;
{
	nullPrint(stringStatement);
	printExpressionList(stringStatement);
}

  void
printStructStatement(structStatement)
  structStatementBodyType	*structStatement;
{
	nullPrint(structStatement);
	printSymbol(structStatement->structName);
	printBlock(structStatement->structBody);
}

  void
printTargetStatement(targetStatement)
  targetStatementBodyType	*targetStatement;
{
	nullPrint(targetStatement);
	printExpression(targetStatement);
}

  void
printUndefineStatement(undefineStatement)
  undefineStatementBodyType	*undefineStatement;
{
	nullPrint(undefineStatement);
	printIdentifierList(undefineStatement);
}

  void
printVariableStatement(variableStatement)
  variableStatementBodyType	*variableStatement;
{
	nullPrint(variableStatement);
	printSymbol(variableStatement->theSymbol);
	printExpression(variableStatement->theValue);
}

  void
printWhileStatement(whileStatement)
  whileStatementBodyType	*whileStatement;
{
	nullPrint(whileStatement);
	tab(); printf("(condition: ");
	       printCondition(whileStatement->whileCondition);
	       printf(")\n");
	printBlock(whileStatement->whileLoop);
}

  void
printWordStatement(wordStatement)
  wordStatementBodyType	*wordStatement;
{
	nullPrint(wordStatement);
	printExpressionList(wordStatement);
}

  void
printLabelList(labelList)
  labelListType	*labelList;
{
	nullPrint(labelList);
	tab(); printf("(\n");
	tablevel++;
	while (labelList != NULL) {
		printSymbol(labelList->theLabel);
		labelList = labelList->nextLabel;
	}
	tablevel--;
	tab(); printf(")\n");
}

  void
printStatementBody(kind, body)
  statementKindType	kind;
  statementBodyType	body;
{
	switch (kind) {

	case ALIGN_STATEMENT:
		printAlignStatement(body);
		break;

	case ASSERT_STATEMENT:
		printAssertStatement(body);
		break;

	case BLOCK_STATEMENT:
		printBlockStatement(body);
		break;

	case BYTE_STATEMENT:
		printByteStatement(body);
		break;

	case CONSTRAIN_STATEMENT:
		printConstrainStatement(body);
		break;

	case DBYTE_STATEMENT:
		printDbyteStatement(body);
		break;

	case DEFINE_STATEMENT:
		printDefineStatement(body);
		break;

	case DO_UNTIL_STATEMENT:
		printDoUntilStatement(body);
		break;

	case DO_WHILE_STATEMENT:
		printDoWhileStatement(body);
		break;

	case EXTERN_STATEMENT:
		printExternStatement(body);
		break;

	case FRETURN_STATEMENT:
		printFreturnStatement(body);
		break;

	case FUNCTION_STATEMENT:
		printFunctionStatement(body);
		break;

	case GROUP_STATEMENT:
		printBlock(body);
		break;

	case IF_STATEMENT:
		printIfStatement(body);
		break;

	case INCLUDE_STATEMENT:
		printIncludeStatement(body);
		break;

	case INSTRUCTION_STATEMENT:
		printInstructionStatement(body);
		break;

	case LONG_STATEMENT:
		printLongStatement(body);
		break;

	case MACRO_STATEMENT:
		printMacroStatement(body);
		break;

	case MDEFINE_STATEMENT:
		printMdefineStatement(body);
		break;

	case MDO_UNTIL_STATEMENT:
		printMdoUntilStatement(body);
		break;

	case MDO_WHILE_STATEMENT:
		printMdoWhileStatement(body);
		break;

	case MFOR_STATEMENT:
		printMforStatement(body);
		break;

	case MIF_STATEMENT:
		printMifStatement(body);
		break;

	case MSWITCH_STATEMENT:
		printMswitchStatement(body);
		break;

	case MVARIABLE_STATEMENT:
		printMvariableStatement(body);
		break;

	case MWHILE_STATEMENT:
		printMwhileStatement(body);
		break;

	case NULL_STATEMENT:
		/* do nothing */
		break;

	case ORG_STATEMENT:
		printOrgStatement(body);
		break;

	case PERFORM_STATEMENT:
		printPerformStatement(body);
		break;

	case REL_STATEMENT:
		printRelStatement(body);
		break;

	case START_STATEMENT:
		printStartStatement(body);
		break;

	case STRING_STATEMENT:
		printStringStatement(body);
		break;

	case STRUCT_STATEMENT:
		printStructStatement(body);
		break;

	case TARGET_STATEMENT:
		printTargetStatement(body);
		break;

	case UNDEFINE_STATEMENT:
		printUndefineStatement(body);
		break;

	case VARIABLE_STATEMENT:
		printVariableStatement(body);
		break;

	case WHILE_STATEMENT:
		printWhileStatement(body);
		break;

	case WORD_STATEMENT:
		printWordStatement(body);
		break;

	default:
		printf("Say what? statement kind=%d\n", kind);
		break;
	}
}

  void
printStatement(statement)
  statementType	*statement;
{
	nullPrint(statement);
	tab(); printf("(statement[");
	printStatementKind(statement->kindOfStatement);
	printf("]: \n");
	tablevel++;
	printLabelList(statement->labels);
	printStatementBody(statement->kindOfStatement,
			statement->statementBody);
	tablevel--;
	tab(); printf(")\n");
	if (statement->nextStatement != NULL)
		printStatement(statement->nextStatement);
}

/* 
   Other debug print routines.
 */

  void
printPendingFixupList(fixupList)
  fixupListType	*fixupList;
{
	printf("fixup list: (");
	tablevel++;
	while (fixupList != NULL) {
		printExpression(fixupList->theFixupExpression);
		fixupList = fixupList->nextFixup;
	}
	tablevel--;
	printf(")\n");
}

  void
printCreateFixup(expression, location, kindOfFixup)
  expressionType	*expression;
  addressType		 location;
  fixupKindType		 kindOfFixup;
{
	static char	*fixupStringTable[] = {
		"BYTE FIXUP",
		"WORD FIXUP",
		"NO FIXUP",
		"OPERAND FIXUP",
		"BYTE RELATIVE FIXUP",
		"DBYTE FIXUP",
		"NO FIXUP (OK)",
		"LONG FIXUP",
		"WORD RELATIVE FIXUP",
	};

	printf("created fixup: expr=0x%x at 0x%x (%s)\n", expression,
		location, fixupStringTable[(int)kindOfFixup]);
}


/* 
   Print internal code buffers.
 */

  void
printExpressionBuffer()
{
	int	line;
	int	i;

	printf("expression length = %d:\n", expressionBufferSize);
	for (line=0; line<expressionBufferSize; line+=16) {
		printf(" %4x:", line);
		for (i=line; i<line+16 && i<expressionBufferSize; i++)
			printf(" %2x", expressionBuffer[i]);
		printf("\n");
	}
}

  void
printOneCodeBuffer(codeSegment, bufferNum)
  codeSegmentType	*codeSegment;
  int			 bufferNum;
{
    int			 line;
    int			 i;
    int			 start;
    int			 end;

    start = bufferPosition(codeSegment->codeStartAddress);
    end = bufferPosition(codeSegment->codeEndAddress);

    printf("buffer 0x%x:\n", bufferNum*CODE_BUFFER_SIZE);
        for (line=0; line<CODE_BUFFER_SIZE; line+=16) {
            if (line+15<start || line>end){
                /* skip it -- no code here */
            } else {
                printf(" %4x:", bufferNum*CODE_BUFFER_SIZE + line);
                for (i=line; i<line+16; i++) {
                    if (i<start || i>end)
                        printf("   ");
                    else
                        printf(" %2x", (*(codeSegment->codeBuffer))[i]);
                }
                printf("\n");
            }
        }
}

  void
printCodeBufferSection(codeBufferSection)
  codeRegionType	*codeBufferSection;
{
	bool	anyCodeThereFlag;
	int	i;

	anyCodeThereFlag = FALSE;
	for (i=0; i<CODE_BUFFERS_IN_ADDRESS_SPACE; i++)
		if (codeBufferSection->codeSegments[i] != NULL) {
			printOneCodeBuffer(codeBufferSection->codeSegments[i],
				i);
			anyCodeThereFlag = TRUE;
		}
	if (!anyCodeThereFlag)
		printf("no code in section\n");
	printf("\n");
}

  void
printCodeBuffers()
{
	printf("absolute code:\n");
	printCodeBufferSection(&absoluteCodeRegion);
	printf("relocatable code:\n");
	printCodeBufferSection(&relocatableCodeRegion);
}
