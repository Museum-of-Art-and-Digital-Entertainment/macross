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
	debugPrint.c -- Routines to print out things for debugging purposes.

	Chip Morningstar -- Lucasfilm Ltd.

	5-November-1984

*/

#include "macrossTypes.h"
#include "macrossGlobals.h"
#include "debugPrint.h"
#include "y.tab.h"

/*
   Print out the parse-tree.
 */

int	tablevel = 0;

/* Fundamental nop print operation */
#define nullPrint(thing)   if (thing==NULL) { tab(); printf("()\n"); return; }

/* For keeping nested structures looking pretty */

void
tab(void)
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
printAssignmentKind(assignmentKindType assignmentKind)
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
printExpressionKind(expressionTermKindType kind)
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
statementKindString(statementKindType kind)
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
printStatementKind(statementKindType kind)
{
	printf("%s", statementKindString(kind));
}


/*
   These routines collectively print expressions and values.
 */

  void
printValue(valueType *value)
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
printSymbol(symbolTableEntryType *symbol)
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
printArgumentDefinitionList(argumentDefinitionListType *list)
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
printBlock(blockType *block)
{
	nullPrint(block);
	tab(); printf("(block:\n");
	tablevel++;
	printStatement(block);
	tablevel--;
	tab(); printf(")\n");
}

  void
printArrayTerm(arrayTermType *arrayTerm)
{
	nullPrint(arrayTerm);
	tab(); printf("(array\n");
	tablevel++;
	printExpression(arrayTerm->arrayName);
	printExpression(arrayTerm->arrayIndex);
	tablevel--;
	tab(); printf(")\n");
}

  void
printAssignmentTerm(binopTermType *assignmentTerm)
{

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
printBinopTerm(binopTermType *binopTerm)
{
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
printFunctionCall(functionCallTermType *functionCall)
{
	nullPrint(functionCall);
	tab(); printf("(function call %s\n", functionCall->functionName->
			symbolName);
	tablevel++;
	printOperandList(functionCall->parameters);
	tablevel--;
	tab(); printf(")\n");
}

  void
printHere(void)
{
	tab(); printf("(here)\n");
}

  void
printIdentifier(symbolTableEntryType *identifier)
{
	nullPrint(identifier);
	printSymbol(identifier);
}

  void
printNumber(numberTermType number)
{
	tab(); printf("(number: %d)\n", number);
}

  void
printPostopTerm(postOpTermType *postopTerm)
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
printPreopTerm(preOpTermType *preopTerm)
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
printUnopTerm(unopTermType *unopTerm)
{
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
printExpression(expressionType *expression)
{
	nullPrint(expression);
	tab(); printf("(expression: [");
	printExpressionKind(expression->kindOfTerm);
	printf("]\n");
	tablevel++;
	switch (expression->kindOfTerm) {

	case ARRAY_EXPR:
		printArrayTerm(expression->expressionTerm.arrayUnion);
		break;

	case ASSIGN_EXPR:
		printAssignmentTerm(expression->expressionTerm.binopUnion);
		break;

	case BINOP_EXPR:
		printBinopTerm(expression->expressionTerm.binopUnion);
		break;

	case CONDITION_CODE_EXPR:
		printCondition(expression->expressionTerm.conditionTypeUnion);
		break;

	case FUNCTION_CALL_EXPR:
		printFunctionCall(expression->expressionTerm.functionCallUnion);
		break;

	case HERE_EXPR:
		printHere();
		break;

	case IDENTIFIER_EXPR:
		printIdentifier(expression->expressionTerm.symbolTableUnion);
		break;

	case NUMBER_EXPR:
		printNumber(expression->expressionTerm.numberUnion);
		break;

	case POSTOP_EXPR:
		printPostopTerm(expression->expressionTerm.postOpUnion);
		break;

	case PREOP_EXPR:
		printPreopTerm(expression->expressionTerm.preOpUnion);
		break;

	case STRING_EXPR:
		tab(); printf("%s\n", expression->expressionTerm);
		break;

	case SUBEXPRESSION_EXPR:
		printExpression(expression->expressionTerm.expressionUnion);
		break;

	case UNOP_EXPR:
		printUnopTerm(expression->expressionTerm.unopUnion);
		break;

	case VALUE_EXPR:
		printValue(expression->expressionTerm.valueUnion);
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
printExpressionList(expressionListType *expressionList)
{
	while (expressionList != NULL) {
		printExpression(expressionList->theExpression);
		expressionList = expressionList->nextExpression;
	}
}

  void
printIdentifierList(identifierListType *identifierList)
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
printCase(caseType *aCase)
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
printCaseList(caseListType *caseList)
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
printMacro(macroTableEntryType *macroInstruction)
{
	nullPrint(macroInstruction);
	tab(); printf("(macro: %s)\n", macroInstruction->macroName);
}

  void
printOpcode(opcodeTableEntryType *opcode)
{
	nullPrint(opcode);
	tab(); printf("(opcode: %s)\n", opcode->mnemonic);
}

  void
printOperandList(operandListType *operandList)
{
	nullPrint(operandList);
	printOperand(operandList);
	if (operandList->nextOperand != NULL)
		printOperandList(operandList->nextOperand);
}

  void
printAlignStatement(alignStatementBodyType *alignStatement)
{
	nullPrint(alignStatement);
	printExpression(alignStatement);
}

  void
printAssertStatement(assertStatementBodyType *assertStatement)
{
	nullPrint(assertStatement);
	printExpression(assertStatement->condition);
	printExpression(assertStatement->message);
}

  void
printBlockStatement(blockStatementBodyType *blockStatement)
{
	nullPrint(blockStatement);
	printExpressionList(blockStatement);
}

  void
printByteStatement(byteStatementBodyType *byteStatement)
{
	nullPrint(byteStatement);
	printExpressionList(byteStatement);
}

  void
printConstrainStatement(constrainStatementBodyType *constrainStatement)
{
	nullPrint(constrainStatement);
	printExpression(constrainStatement->constraint);
	printBlock(constrainStatement->constrainedBlock);
}

  void
printDbyteStatement(dbyteStatementBodyType *dbyteStatement)
{
	nullPrint(dbyteStatement);
	printExpressionList(dbyteStatement);
}

  void
printDefineStatement(defineStatementBodyType *defineStatement)
{
	nullPrint(defineStatement);
	printSymbol(defineStatement->theSymbol);
	printExpression(defineStatement->theValue);
}

  void
printDoUntilStatement(doUntilStatementBodyType *doUntilStatement)
{
	nullPrint(doUntilStatement);
	printBlock(doUntilStatement->doUntilLoop);
	tab(); printf("(condition: ");
	       printCondition(doUntilStatement->doUntilCondition);
	       printf(")\n");
}

  void
printDoWhileStatement(doWhileStatementBodyType *doWhileStatement)
{
	nullPrint(doWhileStatement);
	printBlock(doWhileStatement->doWhileLoop);
	tab(); printf("(condition: ");
	       printCondition(doWhileStatement->doWhileCondition);
	       printf(")\n");
}

  void
printExternStatement(externStatementBodyType *externStatement)
{
	nullPrint(externStatement);
	printIdentifierList(externStatement);
}

  void
printFreturnStatement(freturnStatementBodyType *freturnStatement)
{
	nullPrint(freturnStatement);
	printExpression(freturnStatement);
}

  void
printFunctionStatement(functionStatementBodyType *functionStatement)
{
	nullPrint(functionStatement);
	tab();printf("(function name: %s)\n",functionStatement->functionName);
	printArgumentDefinitionList(functionStatement->theArguments);
	printBlock(functionStatement->theBlock);
}

  void
printIfStatement(ifStatementBodyType *ifStatement)
{
	nullPrint(ifStatement);
	tab(); printf("(condition: ");
	       printCondition(ifStatement->ifCondition);
	       printf(")\n");
	printBlock(ifStatement->consequence);
	tab(); printf("(else:\n");
	tablevel++;
	printIfStatement(ifStatement->continuation.blockUnion);
	tablevel--;
	tab(); printf(")\n");
}

  void
printIncludeStatement(includeStatementBodyType *includeStatement)
{
	nullPrint(includeStatement);
	printExpression(includeStatement);
}

  void
printInstructionStatement(instructionStatementBodyType *instructionStatement)
{
	nullPrint(instructionStatement);
	switch(instructionStatement->kindOfInstruction) {
	case OPCODE_INSTRUCTION:
		printOpcode(instructionStatement->theInstruction.opcodeUnion);
		break;

	case MACRO_INSTRUCTION:
		printMacro(instructionStatement->theInstruction.macroUnion);
		break;

	default:
		printf("huh? instruction type=%d\n", instructionStatement->
					kindOfInstruction);
		break;
	}
	printOperandList(instructionStatement->theOperands);
}

  void
printLongStatement(longStatementBodyType *longStatement)
{
	nullPrint(longStatement);
	printExpressionList(longStatement);
}

  void
printMacroStatement(macroStatementBodyType *macroStatement)
{
	nullPrint(macroStatement);
	tab(); printf("(macro name: %s)\n", macroStatement->theMacro);
	printArgumentDefinitionList(macroStatement->theArguments);
	printBlock(macroStatement->theBlock);
}

  void
printMdefineStatement(defineStatementBodyType *mdefineStatement)
{
	nullPrint(mdefineStatement);
	printSymbol(mdefineStatement->theSymbol);
	printExpression(mdefineStatement->theValue);
}

  void
printMdoUntilStatement(mdoUntilStatementBodyType *mdoUntilStatement)
{
	nullPrint(mdoUntilStatement);
	printBlock(mdoUntilStatement->mdoUntilLoop);
	printExpression(mdoUntilStatement->mdoUntilCondition);
}

  void
printMdoWhileStatement(mdoWhileStatementBodyType *mdoWhileStatement)
{
	nullPrint(mdoWhileStatement);
	printBlock(mdoWhileStatement->mdoWhileLoop);
	printExpression(mdoWhileStatement->mdoWhileCondition);
}

  void
printMforStatement(mforStatementBodyType *mforStatement)
{
	nullPrint(mforStatement);
	printExpression(mforStatement->initExpression);
	printExpression(mforStatement->testExpression);
	printExpression(mforStatement->incrExpression);
	printBlock(mforStatement->forLoop);
}

  void
printMifStatement(mifStatementBodyType *mifStatement)
{
	nullPrint(mifStatement);
	printExpression(mifStatement->mifCondition);
	printBlock(mifStatement->mifConsequence);
	tab(); printf("(melse:\n");
	tablevel++;
	printMifStatement(mifStatement->mifContinuation.mifContinuationBodyUnion);
	tablevel--;
	tab(); printf(")\n");
}

  void
printMswitchStatement(mswitchStatementBodyType *mswitchStatement)
{
	nullPrint(mswitchStatement);
	printExpression(mswitchStatement->switchExpression);
	printCaseList(mswitchStatement->cases);
}

  void
printMvariableStatement(mvariableStatementBodyType *mvariableStatement)
{
	nullPrint(mvariableStatement);
	printSymbol(mvariableStatement->theSymbol);
	printExpression(mvariableStatement->theValue);
}

  void
printMwhileStatement(mwhileStatementBodyType *mwhileStatement)
{
	nullPrint(mwhileStatement);
	printExpression(mwhileStatement->mwhileCondition);
	printBlock(mwhileStatement->mwhileLoop);
}

  void
printOrgStatement(orgStatementBodyType *orgStatement)
{
	nullPrint(orgStatement);
	printExpression(orgStatement);
}

  void
printPerformStatement(performStatementBodyType *performStatement)
{
	nullPrint(performStatement);
	printExpression(performStatement);
}

  void
printRelStatement(relStatementBodyType *relStatement)
{
	/* there's nothing here... */
}

  void
printStartStatement(startStatementBodyType *startStatement)
{
	nullPrint(startStatement);
	printExpression(startStatement);
}

  void
printStringStatement(stringStatementBodyType *stringStatement)
{
	nullPrint(stringStatement);
	printExpressionList(stringStatement);
}

  void
printStructStatement(structStatementBodyType *structStatement)
{
	nullPrint(structStatement);
	printSymbol(structStatement->structName);
	printBlock(structStatement->structBody);
}

  void
printTargetStatement(targetStatementBodyType *targetStatement)
{
	nullPrint(targetStatement);
	printExpression(targetStatement);
}

  void
printUndefineStatement(undefineStatementBodyType *undefineStatement)
{
	nullPrint(undefineStatement);
	printIdentifierList(undefineStatement);
}

  void
printVariableStatement(variableStatementBodyType *variableStatement)
{
	nullPrint(variableStatement);
	printSymbol(variableStatement->theSymbol);
	printExpression(variableStatement->theValue);
}

  void
printWhileStatement(whileStatementBodyType *whileStatement)
{
	nullPrint(whileStatement);
	tab(); printf("(condition: ");
	       printCondition(whileStatement->whileCondition);
	       printf(")\n");
	printBlock(whileStatement->whileLoop);
}

  void
printWordStatement(wordStatementBodyType *wordStatement)
{
	nullPrint(wordStatement);
	printExpressionList(wordStatement);
}

  void
printLabelList(labelListType *labelList)
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
printStatementBody(statementKindType kind, statementBodyType body)
{
	switch (kind) {

	case ALIGN_STATEMENT:
		printAlignStatement(body.alignUnion);
		break;

	case ASSERT_STATEMENT:
		printAssertStatement(body.assertUnion);
		break;

	case BLOCK_STATEMENT:
		printBlockStatement(body.blockUnion);
		break;

	case BYTE_STATEMENT:
		printByteStatement(body.byteUnion);
		break;

	case CONSTRAIN_STATEMENT:
		printConstrainStatement(body.constrainUnion);
		break;

	case DBYTE_STATEMENT:
		printDbyteStatement(body.dbyteUnion);
		break;

	case DEFINE_STATEMENT:
		printDefineStatement(body.defineUnion);
		break;

	case DO_UNTIL_STATEMENT:
		printDoUntilStatement(body.doUntilUnion);
		break;

	case DO_WHILE_STATEMENT:
		printDoWhileStatement(body.doWhileUnion);
		break;

	case EXTERN_STATEMENT:
		printExternStatement(body.externUnion);
		break;

	case FRETURN_STATEMENT:
		printFreturnStatement(body.freturnUnion);
		break;

	case FUNCTION_STATEMENT:
		printFunctionStatement(body.functionUnion);
		break;

	case GROUP_STATEMENT:
		printBlock(body.groupUnion);
		break;

	case IF_STATEMENT:
		printIfStatement(body.ifUnion);
		break;

	case INCLUDE_STATEMENT:
		printIncludeStatement(body.includeUnion);
		break;

	case INSTRUCTION_STATEMENT:
		printInstructionStatement(body.instructionUnion);
		break;

	case LONG_STATEMENT:
		printLongStatement(body.longUnion);
		break;

	case MACRO_STATEMENT:
		printMacroStatement(body.macroUnion);
		break;

	case MDEFINE_STATEMENT:
		printMdefineStatement(body.defineUnion);
		break;

	case MDO_UNTIL_STATEMENT:
		printMdoUntilStatement(body.mdoUntilUnion);
		break;

	case MDO_WHILE_STATEMENT:
		printMdoWhileStatement(body.mdoWhileUnion);
		break;

	case MFOR_STATEMENT:
		printMforStatement(body.mforUnion);
		break;

	case MIF_STATEMENT:
		printMifStatement(body.mifUnion);
		break;

	case MSWITCH_STATEMENT:
		printMswitchStatement(body.mswitchUnion);
		break;

	case MVARIABLE_STATEMENT:
		printMvariableStatement(body.mvariableUnion);
		break;

	case MWHILE_STATEMENT:
		printMwhileStatement(body.mwhileUnion);
		break;

	case NULL_STATEMENT:
		/* do nothing */
		break;

	case ORG_STATEMENT:
		printOrgStatement(body.orgUnion);
		break;

	case PERFORM_STATEMENT:
		printPerformStatement(body.performUnion);
		break;

	case REL_STATEMENT:
		printRelStatement(body.relUnion);
		break;

	case START_STATEMENT:
		printStartStatement(body.startUnion);
		break;

	case STRING_STATEMENT:
		printStringStatement(body.stringUnion);
		break;

	case STRUCT_STATEMENT:
		printStructStatement(body.structUnion);
		break;

	case TARGET_STATEMENT:
		printTargetStatement(body.targetUnion);
		break;

	case UNDEFINE_STATEMENT:
		printUndefineStatement(body.undefineUnion);
		break;

	case VARIABLE_STATEMENT:
		printVariableStatement(body.variableUnion);
		break;

	case WHILE_STATEMENT:
		printWhileStatement(body.whileUnion);
		break;

	case WORD_STATEMENT:
		printWordStatement(body.wordUnion);
		break;

	default:
		printf("Say what? statement kind=%d\n", kind);
		break;
	}
}

  void
printStatement(statementType *statement)
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
printPendingFixupList(fixupListType *fixupList)
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
printCreateFixup(expressionType *expression, addressType location, fixupKindType kindOfFixup)
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
printExpressionBuffer(void)
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
printOneCodeBuffer(codeSegmentType *codeSegment, int bufferNum)
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
printCodeBufferSection(codeRegionType *codeBufferSection)
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
printCodeBuffers(void)
{
	printf("absolute code:\n");
	printCodeBufferSection(&absoluteCodeRegion);
	printf("relocatable code:\n");
	printCodeBufferSection(&relocatableCodeRegion);
}
