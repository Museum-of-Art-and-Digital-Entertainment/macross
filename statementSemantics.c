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
	statementSemantics.c -- Routines to eat up a parse tree and spit out
		code.

	Chip Morningstar -- Lucasfilm Ltd.

	12-November-1984
*/

#include "macrossTypes.h"
#include "macrossGlobals.h"
#include "emitBranch.h"
#include "expressionSemantics.h"
#include "operandStuff.h"
#include "parserMisc.h"
#include "semanticMisc.h"
#include "statementSemantics.h"

operandType		*dbOperand;	/* safe temps for dbx hacking */
expressionType		*dbExpression;
symbolTableEntryType	*dbSymbol;

bool			 newIncludeFile = FALSE;
static bool		 insideConstraintBlock = FALSE;
bool			 nullStatementFlag = FALSE;

#define nullAssemble(thing)	if (thing==NULL) return;
#define nullAssembleNULL(thing)	if (thing==NULL) return NULL;
#define sideEffectBomb()	if (beneathFunction) {\
			error(SIDE_EFFECT_ERROR, currentFunctionName);\
			break; }

#define expansionOff() {saveExpansion=expandMacros; expandMacros=FALSE;}
#define expansionOn()	expandMacros=saveExpansion;

  void
assembleBlock(block)
  blockType	*block;
{
	nullAssemble(block);
	assembleStatement(block, FALSE, NULL);
}

  simpleFixupListType *
assembleBlockInsideIf(block, ongoingFixupList)
  blockType		*block;
  simpleFixupListType	*ongoingFixupList;
{
	nullAssembleNULL(block);
	return(assembleStatement(block, TRUE, ongoingFixupList));
}

  bool
operandCheck(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
	int	i;

	for (i=0; i<numberOfOperands; i++) {
		if ((operandClassTable[(int)operandKindField(
				evaluatedOperands[i]->addressMode)] & opcode->
				permissibleOperandModes) == 0) {
			error(ILLEGAL_OPERAND_TYPE_ERROR, opcode->mnemonic);
			return(FALSE);
		}
	}
	return(TRUE);
}

  void
assembleMachineInstruction(opcode, operands)
  opcodeTableEntryType	*opcode;
  operandListType	*operands;
{
	int		 i;
	int		 op;
	valueType	*evaluatedOperands[MAX_NUMBER_OF_OPERANDS];
	valueType	*value;

	nullAssemble(opcode);
	expand(moreText("%s\t", opcode->mnemonic));
	for (op=1; operands!=NULL && op<=opcode->maxNumberOfOperands;
			op++, operands=operands->nextOperand) {
		newFixupAddressMode = EXPRESSION_OPND;
		evaluatedOperands[op-1] = value = evaluateOperand(operands);
		fixupAddressMode[op-1] = newFixupAddressMode;
		currentOperandNumber++;
		expand(isUndefined(value) ? (finishOperand = TRUE) :
			expandOperand(value->addressMode,
			operandBuffer[op-1]));
		if (value==NULL || value->kindOfValue == FAIL){
			error(BAD_INSTRUCTION_OPERAND_ERROR, opcode->
				mnemonic);
			if (freeFlag)
				for (i=1; i<op; i++)
					free(evaluatedOperands[i-1]);
			currentOperandNumber = 0;
			return;
		}
	}
	currentOperandNumber = 0;
	op--;
	if (operands != NULL) {
		error(TOO_MANY_INSTRUCTION_OPERANDS_ERROR, opcode->mnemonic,
			opcode->maxNumberOfOperands);
	} else if (op < opcode->minNumberOfOperands) {
		error(TOO_FEW_INSTRUCTION_OPERANDS_ERROR, opcode->mnemonic,
			opcode->minNumberOfOperands);
	} else if (operandCheck(opcode, op, evaluatedOperands)) {
		(*instructionActionTable[(int)opcode->addressClass])(opcode,
						op, evaluatedOperands);
	}
	expand((expandOperands(op), endLine()));
	if (freeFlag)
		for (i=0; i<op; i++)
			free(evaluatedOperands[i]);
}

  void
assembleMacro(macroInstruction, operands)
  macroTableEntryType	*macroInstruction;
  operandListType	*operands;
{
	int			 numberBound;
	identifierListType	*savedLocalVariableList;
	int			 savedLabelTagNumber;
	bool			 savedErrorFlag;
	bool			 saveListingOn;
	environmentType		 newEnvironment;

	nullAssemble(macroInstruction);
	if (macroInstruction->body == NULL) {
		error(UNDEFINED_MACRO_ERROR, macroInstruction->macroName);
		return;
	}
	pushEnvironment(newEnvironment);
	savedLocalVariableList = currentLocalVariableList;
	currentLocalVariableList = NULL;
	savedLabelTagNumber = currentLabelTagNumber;
	currentLabelTagNumber = nextLabelTagNumber++;
	savedErrorFlag = errorFlag;
	errorFlag = FALSE;
	saveListingOn = listingOn;
	if ((numberBound = bindMacroArguments(macroInstruction->arguments,
			operands, macroInstruction->macroName)) > 0) {
		macroCallDepth++;
		listingOn = amExpanding();
		assembleBlock(macroInstruction->body);
		macroCallDepth--;
	} else
		numberBound = -numberBound;
	if (numberBound > 1)
		unbindArguments(macroInstruction->arguments, numberBound-1);
	unbindLocalVariables(currentLocalVariableList);
	if (errorFlag)
		error(ERROR_INSIDE_MACRO_ERROR, macroInstruction->macroName);
	else
		errorFlag = savedErrorFlag;
	currentLabelTagNumber = savedLabelTagNumber;
	currentLocalVariableList = savedLocalVariableList;
	listingOn = saveListingOn;
	popEnvironment();
}

  void
assembleAlignStatement(alignStatement)
  alignStatementBodyType	*alignStatement;
{
	int		 alignment;
	bool		 saveExpansion;

	nullAssemble(alignStatement);
	expansionOff();
	alignment = intValue(evaluateExpression(alignStatement, NO_FIXUP));
	expansionOn();
	expand((moreText("align\t0x%x", alignment), endLine()));
	if (alignment > 0)
		if (currentCodeMode == ABSOLUTE_BUFFER) {
			if (structNestingDepth == 0)
				currentLocationCounter.value = alignment *
					((currentLocationCounter.value +
					alignment - 1) / alignment);
			else
				currentFieldOffset = alignment *
					((currentFieldOffset + alignment -
					1) / alignment);
		} else {
			if (insideConstraintBlock)
				error(ALIGN_INSIDE_CONSTRAIN_ERROR);
			else
				addBreak(ALIGN_BREAK, alignment);
		}
	else
		error(IMPROPER_ALIGNMENT_VALUE_ERROR);
}

  void
assembleAssertStatement(assertStatement)
  assertStatementBodyType	*assertStatement;
{
	valueType	*test;
	valueType	*message;
	bool		 saveExpansion;

	expansionOff();
	test = evaluateExpression(assertStatement->condition, NO_FIXUP);
	expansionOn();
	if (test == NULL || test->kindOfValue != ABSOLUTE_VALUE) {
		error(INVALID_ASSERT_CONDITION_ERROR);
	} else if (!test->value) {
		if (assertStatement->message == NULL)
			error(ASSERT_ERROR);
		else {
			expansionOff();
			message = evaluateExpression(assertStatement->message,
				NO_FIXUP);
			expansionOn();
			if (message == NULL || message->kindOfValue !=
					STRING_VALUE)
				error(INVALID_ASSERT_MESSAGE_ERROR);
			else
				error(ASSERT_WITH_MESSAGE_ERROR, message->
					value);
		}
	}
}

  void
assembleBlockStatement(blockStatement)
  blockStatementBodyType	*blockStatement;
{
	valueType		*blockIncrement;
	int			 blockSize;

	blockSize = 0;
	nullAssemble(blockStatement);
	expand(moreText("block\t"));
	while (blockStatement != NULL) {
		blockIncrement = evaluateExpression(blockStatement->
				theExpression, NO_FIXUP);
		blockSize += intValue(blockIncrement);
		blockStatement = blockStatement->nextExpression;
		expand((expandExpression(NULL), blockStatement!=NULL ?
			moreText(", ") : 0));
		qfree(blockIncrement);
	}
	expand(endLine());
	if (blockSize >= 0) {
		if (structNestingDepth != 0) {
			while (blockSize-- > 0)
				emitByte(0);
		} else if (currentCodeMode == RELOCATABLE_BUFFER) {
			while (blockSize-- > 0)
				emitByte(0);
		} else {
			if (produceLinkableObject)
				reserveAbsolute(currentLocationCounter.value,
					blockSize);
			currentLocationCounter.value += blockSize;
		}
	} else
		error(NEGATIVE_BLOCK_SIZE_ERROR);
}

  void
assembleByteStatement(byteStatement)
  byteStatementBodyType	*byteStatement;
{
	valueType		*byteValue;

	nullAssemble(byteStatement);
	expand(moreText("byte\t"));
	while (byteStatement != NULL) {
		byteValue = evaluateExpression(byteStatement->theExpression,
			BYTE_FIXUP);
		if (byteValue->kindOfValue == STRING_VALUE) {
			emitString(byteValue->value);
		} else {
			putFixupsHere(BYTE_FIXUP, 0);
			emitByteValue(byteValue);
		}
		qfree(byteValue);
		byteStatement = byteStatement->nextExpression;
		expand((expandExpression(NULL), byteStatement != NULL ?
			moreText(", ") : 0));
	}
	expand(endLine());
}

  void
assembleConstrainStatement(constrainStatement)
  constrainStatementBodyType	*constrainStatement;
{
	valueType	*constraintValue;
	int		 constraint;
	addressType	 startAddress;
	addressType	 endAddress;
	bool		 saveExpansion;

	expansionOff();
	constraintValue = evaluateExpression(constrainStatement->constraint,
		NO_FIXUP);
	expansionOn();
	if (constraintValue == NULL || constraintValue->kindOfValue !=
			ABSOLUTE_VALUE) {
		error(INVALID_CONSTRAINT_VALUE_ERROR);
		return;
	}
	constraint = constraintValue->value;
	expand((moreText("constrain (0x%x) {", constraint), endLine(),
		tabCount++));
	if (currentCodeMode == ABSOLUTE_BUFFER) {
		startAddress = currentLocationCounter.value;
		assembleBlock(constrainStatement->constrainedBlock);
		endAddress = currentLocationCounter.value;
		if ((startAddress / constraint) != (endAddress / constraint))
			error(CONSTRAINT_ERROR, constraint);
	} else {
		if (insideConstraintBlock) {
			error(CONSTRAIN_INSIDE_CONSTRAIN_ERROR);
			assembleBlock(constrainStatement->constrainedBlock);
		} else {
			insideConstraintBlock = TRUE;
			addBreak(CONSTRAIN_BREAK, constraint);
			assembleBlock(constrainStatement->constrainedBlock);
			addBreak(END_CONSTRAIN_BREAK, 0);
			insideConstraintBlock = FALSE;
		}
	}
	expand((tabCount--, startLineMarked(), tabIndent(), moreText("}"),
		endLine()));
}

  void
assembleDbyteStatement(dbyteStatement)
  dbyteStatementBodyType	*dbyteStatement;
{
	valueType		*wordValue;

	nullAssemble(dbyteStatement);
	expand(moreText("dbyte\t"));
	while (dbyteStatement != NULL) {
		wordValue = swabValue(evaluateExpression(dbyteStatement->
			theExpression, DBYTE_FIXUP));
		dbyteStatement = dbyteStatement->nextExpression;
		expand((expandExpression(NULL), dbyteStatement != NULL ?
			moreText(", ") : 0));
		putFixupsHere(DBYTE_FIXUP, 0);
		emitWordValue(wordValue);
		qfree(wordValue);
	}
	expand(endLine());
}

  void
assembleDefineStatement(defineStatement)
  defineStatementBodyType	*defineStatement;
{
	symbolTableEntryType	*symbolToDefine;
	symbolInContextType	*contextToDefine;
	bool			 saveExpansion;

	nullAssemble(defineStatement);
	symbolToDefine = effectiveSymbol(defineStatement->theSymbol,
		&contextToDefine);
	if (isDefinable(contextToDefine)) {
		expand(moreText("define %s = ", symbName(symbolToDefine)));
		if (contextToDefine->usage == DEAD_SYMBOL) {
			reincarnateSymbol(contextToDefine, DEFINE_SYMBOL);
		} else if (contextToDefine->usage == NESTED_UNKNOWN_SYMBOL) {
			contextToDefine->usage = DEFINE_SYMBOL;
		}
		if (alreadyDefined(contextToDefine))
			error(REDEFINITION_OF_SYMBOL_ERROR, symbName(
				symbolToDefine));
		else {
			if (defineStatement->theValue != NULL) {
				contextToDefine->value =
					evaluateDefineExpression(
					defineStatement->theValue);
				contextToDefine->usage = DEFINE_SYMBOL;
				expand(expandExpression(NULL));
			} else
				contextToDefine->value = newValue(FAIL, 0,
					EXPRESSION_OPND);
			contextToDefine->environmentNumber =
				GLOBAL_ENVIRONMENT_NUMBER;
			contextToDefine->attributes |= DEFINED_VARIABLE_ATT;
		}
		expand(endLine());
	} else {
		error(SYMBOL_ALREADY_THERE_ERROR, symbName(symbolToDefine));
	}
}

  void
assembleDoUntilStatement(doUntilStatement)
  doUntilStatementBodyType	*doUntilStatement;
{
	valueType	topOfLoop;

	nullAssemble(doUntilStatement);
	topOfLoop = currentLocationCounter;
	if (doUntilStatement->doUntilCondition == NEVER_COND)
		topOfLoop.value -= targetOffset;
	expand((moreText("do {"), endLine(), tabCount++));
	assembleBlock(doUntilStatement->doUntilLoop);
	expand((tabCount--, startLineMarked(), tabIndent(),
		moreText("} until %s", conditionString(doUntilStatement->
		doUntilCondition)), endLine()));
	if (doUntilStatement->doUntilCondition == ALWAYS_COND) {
		return;
	} else if (doUntilStatement->doUntilCondition != NEVER_COND) {
		emitRelativeBranch(invertConditionCode(doUntilStatement->
				doUntilCondition), &topOfLoop, NULL);
	} else {
		emitJump(&topOfLoop, NULL);
	}
}

  void
assembleDoWhileStatement(doWhileStatement)
  doWhileStatementBodyType	*doWhileStatement;
{
	valueType	topOfLoop;

	nullAssemble(doWhileStatement);
	topOfLoop = currentLocationCounter;
	if (doWhileStatement->doWhileCondition == ALWAYS_COND)
		topOfLoop.value -= targetOffset;
	expand((moreText("do {"), endLine(), tabCount++));
	assembleBlock(doWhileStatement->doWhileLoop);
	expand((tabCount--, startLineMarked(), tabIndent(),
		moreText("} while %s", conditionString(doWhileStatement->
		doWhileCondition)), endLine()));
	if (doWhileStatement->doWhileCondition == NEVER_COND) {
		return;
	} else if (doWhileStatement->doWhileCondition != ALWAYS_COND) {
		emitRelativeBranch(doWhileStatement->doWhileCondition,
			&topOfLoop, NULL);
	} else {
		emitJump(&topOfLoop, NULL);
	}
}

  void
assembleExternStatement(externStatement)
  externStatementBodyType	*externStatement;
{
	symbolInContextType	*context;
	symbolTableEntryType	*theSymbol;

	expand(moreText("extern\t"));
	while (externStatement != NULL) {
		theSymbol = effectiveSymbol(externStatement->theSymbol,
			&context);
		expand(moreText("%s", symbName(theSymbol)));
		if (context != NULL) {
			if (context->usage == LABEL_SYMBOL || context->usage
					== EXTERNAL_SYMBOL || context->usage
					== UNKNOWN_SYMBOL || context->usage
					== NESTED_UNKNOWN_SYMBOL) {
			    context->attributes |= GLOBAL_ATT;
			} else {
			    error(BAD_KIND_OF_SYMBOL_TO_MAKE_EXTERNAL_ERROR,
					usageString(context->usage));
			}
		}
		externStatement = externStatement->nextIdentifier;
		expand(externStatement != NULL ? moreText(", ") : 0);
	}
	expand(endLine());
}

  void
assembleFreturnStatement(freturnStatement)
  freturnStatementBodyType	*freturnStatement;
{
	freturnExit = TRUE;
	resultOfLastFunctionCall = evaluateExpression(freturnStatement,
							NO_FIXUP_OK);
}

  void
assembleFunctionStatement(functionStatement)
  functionStatementBodyType	*functionStatement;
{
	symbolTableEntryType		*newFunctionSymbol;
	functionDefinitionType		*newFunction;
	symbolInContextType		*context;

	nullAssemble(functionStatement);
	if (currentEnvironment != &globalEnvironment) {
		error(FUNCTION_DEFINITION_INSIDE_FUNCTION_ERROR);
		return;
	}
	newFunctionSymbol = lookupOrEnterSymbol(functionStatement->
		functionName, FUNCTION_SYMBOL);
	context = getBaseContext(newFunctionSymbol);
	if (context->usage != FUNCTION_SYMBOL) {
		if (context->usage == UNKNOWN_FUNCTION_SYMBOL) {
			context->usage = FUNCTION_SYMBOL;
		} else {
			error(SYMBOL_ALREADY_THERE_ERROR, symbName(
				newFunctionSymbol));
			return;
		}
	} else if (isDefined(context->value)) {
		error(FUNCTION_ALREADY_EXISTS_ERROR, symbName(
			newFunctionSymbol));
		return;
	}
	newFunction = typeAlloc(functionDefinitionType);
	newFunction->functionName = newFunctionSymbol;
	newFunction->arguments = functionStatement->theArguments;
	newFunction->body = functionStatement->theBlock;
	newFunction->ordinal = -1;
	newFunction->nextExternalFunction = NULL;
	context->value = newValue(FUNCTION_VALUE, newFunction,
		EXPRESSION_OPND);
	context->environmentNumber = GLOBAL_ENVIRONMENT_NUMBER;
}

  void
assembleGroupStatement(groupStatement)
  blockType	*groupStatement;
{
	expand((startLineMarked(), tabIndent(), moreText("{"), endLine(),
		tabCount++));
	assembleBlock(groupStatement);
	expand((tabCount--, startLineMarked(), tabIndent(), moreText("}"),
		endLine()));
}

  simpleFixupListType *
assembleIfStatement(ifStatement, terminalIf, ongoingFixupList)
  ifStatementBodyType	*ifStatement;
  bool			 terminalIf;
  simpleFixupListType	*ongoingFixupList;
{
	valueType		 fixupLocation1[COMPOUND_BRANCH_MAX];
	simpleFixupListType	*fixupLocation2;

	if (backwardsCompatibleIfFlag) {
		assembleIfStatementOldStyle(ifStatement);
		return(NULL);
	}

	nullAssembleNULL(ifStatement);
	fixupLocation2 = ongoingFixupList;
	if (ifStatement->ifCondition != ALWAYS_COND && ifStatement->
			ifCondition != NEVER_COND) {
		emitRelativeBranch(invertConditionCode(ifStatement->
			ifCondition), NULL, fixupLocation1);
		expand((moreText("if %s {", conditionString(ifStatement->
			ifCondition)), endLine(), tabCount++));
	} else if (ifStatement->ifCondition == ALWAYS_COND) {
		expand((moreText("{"), endLine(), tabCount++));
	}
	if (ifStatement->consequence != NULL && ifStatement->ifCondition !=
			NEVER_COND)
		fixupLocation2 = assembleBlockInsideIf(ifStatement->
			consequence, fixupLocation2);
	expand((tabCount--, startLineMarked()));
	if (ifStatement->continuation.continuationBodyUnion != NULL &&
			ifStatement->ifCondition != NEVER_COND &&
			ifStatement->ifCondition != ALWAYS_COND)
		fixupLocation2 = emitJump(NULL, fixupLocation2);
	if (ifStatement->ifCondition != ALWAYS_COND && ifStatement->
			ifCondition != NEVER_COND)
		fixupBranch(fixupLocation1, currentLocationCounter);
	if (ifStatement->continuation.continuationBodyUnion != NULL &&
			ifStatement->ifCondition != ALWAYS_COND) {
		expand((tabIndent(), moreText("} else ")));
		fixupLocation2 = assembleIfStatement(ifStatement->
			continuation.continuationBodyUnion, terminalIf, fixupLocation2);
	} else {
		expand((tabIndent(), moreText("}"), endLine()));
	}
	if (terminalIf) {
		return(fixupLocation2);
	} else {
		fixupJump(fixupLocation2, currentLocationCounter);
		return(NULL);
	}
}

  void
assembleIfStatementOldStyle(ifStatement)
  ifStatementBodyType	*ifStatement;
{
	valueType		 fixupLocation1[COMPOUND_BRANCH_MAX];
	simpleFixupListType	*fixupLocation2;

	nullAssemble(ifStatement);
	if (ifStatement->ifCondition != ALWAYS_COND) {
		emitRelativeBranch(invertConditionCode(ifStatement->
			ifCondition), NULL, fixupLocation1);
		expand((moreText("if %s {", conditionString(ifStatement->
			ifCondition)), endLine(), tabCount++));
	} else {
		expand((moreText("{"), endLine(), tabCount++));
	}
	if (ifStatement->consequence != NULL)
		assembleBlock(ifStatement->consequence);
	expand((tabCount--, startLineMarked()));
	if (ifStatement->continuation.continuationBodyUnion != NULL)
		fixupLocation2 = emitJump(NULL, NULL);
	if (ifStatement->ifCondition != ALWAYS_COND)
		fixupBranch(fixupLocation1, currentLocationCounter);
	if (ifStatement->continuation.continuationBodyUnion != NULL) {
		expand((tabIndent(), moreText("} else ")));
		assembleIfStatementOldStyle(ifStatement->continuation.continuationBodyUnion);
	} else {
		expand((tabIndent(), moreText("}"), endLine()));
	}
	if (ifStatement->continuation.continuationBodyUnion != NULL)
		fixupJump(fixupLocation2, currentLocationCounter);
}

  void
assembleIncludeStatement(includeStatement)
  includeStatementBodyType	*includeStatement;
{
	stringType	*fileName;
	valueType	*possibleFileName;
	bool		 saveExpansion;

	expansionOff();
	possibleFileName = evaluateExpression(includeStatement, NO_FIXUP);
	expansionOn();
	if (possibleFileName->kindOfValue == STRING_VALUE) {
		fileName = (stringType *)possibleFileName->value;
		newIncludeFile = TRUE;
		expand((moreText("include \"%s\"", fileName), endLine()));
		pushInputFileStack(saveString(fileName));
	} else {
		error(INCLUDE_FILE_NOT_A_STRING_VALUE_ERROR);
	}
}

  void
assembleInstructionStatement(instructionStatement, cumulativeLineNumber)
  instructionStatementBodyType	*instructionStatement;
  int				 cumulativeLineNumber;
{
	nullAssemble(instructionStatement);
	switch(instructionStatement->kindOfInstruction) {

	case OPCODE_INSTRUCTION:
		if (amListing())
			saveIndexForListing(INSTRUCTION_STATEMENT,
				cumulativeLineNumber);
		expand((startLine(), expandLabel(), tabIndent()));
		assembleMachineInstruction(instructionStatement->
			theInstruction.opcodeUnion, instructionStatement->theOperands);
		break;

	case MACRO_INSTRUCTION:
		if (amListing() && !amExpanding())
			saveIndexForListing(INSTRUCTION_STATEMENT,
				cumulativeLineNumber);
		assembleMacro(instructionStatement->theInstruction.macroUnion,
					instructionStatement->theOperands);
		break;

	default:
		botch("bad instruction type=%d\n", instructionStatement->
                      kindOfInstruction, 0, 0);
		break;
	}
}

  void
assembleLongStatement(longStatement)
  longStatementBodyType	*longStatement;
{
	valueType		*longValue;

	nullAssemble(longStatement);
	expand(moreText("long\t"));
	while (longStatement != NULL) {
		longValue = evaluateExpression(longStatement->theExpression,
			LONG_FIXUP);
		longStatement = longStatement->nextExpression;
		expand((expandExpression(NULL), longStatement != NULL ?
			moreText(", ") : 0));
		putFixupsHere(LONG_FIXUP, 0);
		emitLongValue(longValue);
		qfree(longValue);
	}
	expand(endLine());
}

  void
assembleMacroStatement(macroStatement)
  macroStatementBodyType	*macroStatement;
{
	macroTableEntryType		*newMacro;

	nullAssemble(macroStatement);
	if (currentEnvironment != &globalEnvironment) {
		error(MACRO_DEFINITION_INSIDE_MACRO_ERROR);
		return;
	}
	newMacro = macroStatement->theMacro;
	if (newMacro == NULL)
		return;
	if (newMacro->body != NULL) {
		error(MACRO_ALREADY_EXISTS_ERROR, macroStatement->theMacro->
			macroName);
		return;
	}
	newMacro->arguments = macroStatement->theArguments;
	newMacro->body = macroStatement->theBlock;
}

  void
assembleMdefineStatement(mdefineStatement)
  defineStatementBodyType	*mdefineStatement;
{
	bool			 saveExpansion;

	nullAssemble(mdefineStatement);
	expansionOff();
	pushBinding(mdefineStatement->theSymbol, evaluateDefineExpression(
		mdefineStatement->theValue), MDEFINE_SYMBOL);
	expansionOn();
	addNewLocalVariable(mdefineStatement->theSymbol);
}

  void
assembleMdoUntilStatement(mdoUntilStatement)
  mdoUntilStatementBodyType	*mdoUntilStatement;
{
	bool	saveListingOn;

	nullAssemble(mdoUntilStatement);
	saveListingOn = listingOn;
	do {
		assembleBlock(mdoUntilStatement->mdoUntilLoop);
		listingOn = amExpanding();
	} while (!booleanTest(mdoUntilStatement->mdoUntilCondition));
	listingOn = saveListingOn;
}

  void
assembleMdoWhileStatement(mdoWhileStatement)
  mdoWhileStatementBodyType	*mdoWhileStatement;
{
	bool	saveListingOn;

	nullAssemble(mdoWhileStatement);
	saveListingOn = listingOn;
	do {
		assembleBlock(mdoWhileStatement->mdoWhileLoop);
		listingOn = amExpanding();
	} while (booleanTest(mdoWhileStatement->mdoWhileCondition));
	listingOn = saveListingOn;
}

  void
assembleMforStatement(mforStatement)
  mforStatementBodyType	*mforStatement;
{
	bool	saveListingOn;
	bool	saveExpansion;

	nullAssemble(mforStatement);
	saveListingOn = listingOn;
	expansionOff();
	for (evaluateExpression(mforStatement->initExpression, NO_FIXUP);
	     booleanTest(mforStatement->testExpression);
	     evaluateExpression(mforStatement->incrExpression, NO_FIXUP)) {
		        expansionOn();
			assembleBlock(mforStatement->forLoop);
			listingOn = amExpanding();
		        expansionOff();
	}
	expansionOn();
	listingOn = saveListingOn;
}

  void
assembleMifStatement(mifStatement, cumulativeLineNumber)
  mifStatementBodyType	*mifStatement;
  int			 cumulativeLineNumber;
{
	while (mifStatement != NULL) {
		if (mifStatement->mifCondition == NULL) {
			assembleBlock(mifStatement->mifConsequence);
			break;
		} else if (booleanTest(mifStatement->mifCondition)) {
			assembleBlock(mifStatement->mifConsequence);
			break;
		} else {
			mifStatement = mifStatement->mifContinuation.
						mifContinuationBodyUnion;
		}
	}
	if (amListing())
		saveEndMifForListing(cumulativeLineNumber);
}

  void
assembleMswitchStatement(mswitchStatement)
  mswitchStatementBodyType	*mswitchStatement;
{
    valueType		*switchValue;
    caseListType	*caseList;
    caseType		*theCase;
    bool		 matched;
    expressionListType	*tagExpressionList;
    valueType		*tagValue;
    bool		 saveExpansion;

    expansionOff();
    switchValue = evaluateExpression(mswitchStatement->switchExpression,
        NO_FIXUP);
    expansionOn();
    if (switchValue->kindOfValue != ABSOLUTE_VALUE && switchValue->kindOfValue
	    != STRING_VALUE) {
        error(INVALID_SWITCH_VALUE_KIND, valueKindString(switchValue->
            kindOfValue));
        return;
    }
    for (caseList=mswitchStatement->cases; caseList!=NULL; caseList=caseList->
	    nextCase) {
        theCase = caseList->theCase;
        if (theCase->caseTags == NULL) {
            assembleBlock(theCase->caseBody);
            break;
        } else {
	    matched = FALSE;
            for (tagExpressionList=theCase->caseTags; tagExpressionList!=NULL;
                    tagExpressionList=tagExpressionList->nextExpression) {
		expansionOff();
                tagValue = evaluateExpression(tagExpressionList->
		    theExpression, NO_FIXUP);
		expansionOn();
                if (tagValue->kindOfValue != ABSOLUTE_VALUE && tagValue->
			kindOfValue != STRING_VALUE) {
		    error(INVALID_CASE_VALUE_KIND, valueKindString(tagValue->
			kindOfValue));
		    continue;
		}
		if (tagValue->kindOfValue != switchValue->kindOfValue)
		    continue;
		if (tagValue->kindOfValue == ABSOLUTE_VALUE) {
		    if (tagValue->value != switchValue->value)
			continue;
		} else {
		    if (strcmplc(tagValue->value, switchValue->value) != 0)
			continue;
		}
		matched = TRUE;
		break;
	    }
	    if (matched) {
		assembleBlock(theCase->caseBody);
		break;
	    }
	}
    }
}

  void
assembleMvariableStatement(mvariableStatement)
  mvariableStatementBodyType	*mvariableStatement;
{
	identifierListType	*newLocalVariable;
	int			 initCount;
	bool			 saveExpansion;

	nullAssemble(mvariableStatement);
	expansionOff();
	if (mvariableStatement->theDimension == NULL) {
		if ((initCount = expressionListLength(mvariableStatement->
				theValue)) == 1) {
			pushBinding(mvariableStatement->theSymbol,
				evaluateExpression(mvariableStatement->
				theValue->theExpression, NO_FIXUP), 
				MVARIABLE_SYMBOL);
		} else if (initCount > 1) {
			error(TOO_MANY_VARIABLE_INITIALIZERS_ERROR);
		} else {
			pushBinding(mvariableStatement->theSymbol, NULL,
				MVARIABLE_SYMBOL);
		}
	} else {
		pushBinding(mvariableStatement->theSymbol, createArray(
			mvariableStatement->theDimension, mvariableStatement->
			theValue), MVARIABLE_SYMBOL);
	}
	expansionOn();
	newLocalVariable = typeAlloc(identifierListType);
	newLocalVariable->theSymbol = mvariableStatement->theSymbol;
	newLocalVariable->nextIdentifier = currentLocalVariableList;
	currentLocalVariableList = newLocalVariable;
}

  void
assembleMwhileStatement(mwhileStatement)
  mwhileStatementBodyType	*mwhileStatement;
{
	bool	saveListingOn;

	nullAssemble(mwhileStatement);
	saveListingOn = listingOn;
	while (booleanTest(mwhileStatement->mwhileCondition)) {
		assembleBlock(mwhileStatement->mwhileLoop);
		listingOn = amExpanding();
	}
	listingOn = saveListingOn;
}

  void
assembleOrgStatement(orgStatement)
  orgStatementBodyType	*orgStatement;
{
	valueType	*orgAddress;
	bool		 saveExpansion;

	nullAssemble(orgStatement);
	targetOffset = 0;
	expansionOff();
	orgAddress = evaluateExpression(orgStatement, NO_FIXUP);
	expansionOn();
	if (absoluteValue(orgAddress) || relocatableValue(orgAddress)) {
		if (relocatableValue(&currentLocationCounter) &&
						absoluteValue(orgAddress)) {
			savedRelocatableCurrentLocationCounter =
						currentLocationCounter;
			currentCodeMode = ABSOLUTE_BUFFER;
		}
		currentLocationCounter.kindOfValue = orgAddress->kindOfValue;
		currentLocationCounter.value = orgAddress->value;
		expand((moreText("org\t0x%x", orgAddress->value), endLine()));
	} else {
		error(BAD_ORG_ADDRESS_ERROR, valueKindString(orgAddress->
			kindOfValue));
	}
	qfree(orgAddress);
}

  void
assemblePerformStatement(performStatement)
  performStatementBodyType	*performStatement;
{
	nullAssemble(performStatement);
	sideEffectFlag = FALSE;
	evaluateExpressionStandalone(performStatement);
	if (!sideEffectFlag)
		warning(PERFORM_WITHOUT_SIDE_EFFECT_ERROR);
}

  void
assembleRelStatement(relStatement)
  relStatementBodyType	*relStatement;
{
	targetOffset = 0;
	if (!relocatableValue(&currentLocationCounter)) {
		currentLocationCounter =
				savedRelocatableCurrentLocationCounter;
		currentCodeMode = RELOCATABLE_BUFFER;
	}
	expand((moreText("rel"), endLine()));
	addBreak(BREAK_BREAK, 0);
}

  void
assembleStartStatement(startStatement)
  startStatementBodyType	*startStatement;
{
	nullAssemble(startStatement);
	expand(moreText("start\t"));
	if (haveUserStartAddress) {
		expand((moreText("*fail*"), endLine()));
		error(START_ADDRESS_ALREADY_GIVEN_ERROR);
	} else {
		startAddress = evaluateExpression(startStatement,NO_FIXUP_OK);
		if (startAddress->kindOfValue == UNDEFINED_VALUE) {
			startAddress = (valueType *)generateFixupExpression(
				startStatement);
			haveUserStartAddress = TRUE;
			fixupStartAddress = TRUE;
		} else if (startAddress->kindOfValue != ABSOLUTE_VALUE &&
				startAddress->kindOfValue!=RELOCATABLE_VALUE){
			expand((moreText("*fail*"), endLine()));
			error(BAD_START_ADDRESS_ERROR);
		} else {
			expand((expandExpression(NULL), endLine()));
			haveUserStartAddress = TRUE;
			fixupStartAddress = FALSE;
		}
	}
}

  void
assembleStringStatement(stringStatement)
  stringStatementBodyType	*stringStatement;
{
	valueType		*byteValue;

	nullAssemble(stringStatement);
	expand(moreText("string\t"));
	while (stringStatement != NULL) {
		byteValue = evaluateExpression(stringStatement->theExpression,
			BYTE_FIXUP);
		if (byteValue->kindOfValue == STRING_VALUE) {
			emitString(byteValue->value);
			emitByte('\0');
		} else {
			putFixupsHere(BYTE_FIXUP, 0);
			emitByteValue(byteValue);
		}
		qfree(byteValue);
		stringStatement = stringStatement->nextExpression;
		expand((expandExpression(NULL), stringStatement != NULL ?
			moreText(", ") : 0));
	}
	expand(endLine());
}

  void
assembleStructStatement(structStatement)
  structStatementBodyType	*structStatement;
{
	nullAssemble(structStatement);
	if (structStatement->structBody == NULL)
		instantiateStruct(structStatement);
	else
		assembleStructDefinition(structStatement);
}

  void
assembleTargetStatement(targetStatement)
  targetStatementBodyType	*targetStatement;
{
	valueType	*targetAddress;
	bool		 saveExpansion;

	nullAssemble(targetStatement);
	targetOffset = 0;
	expansionOff();
	targetAddress = evaluateExpression(targetStatement, NO_FIXUP);
	expansionOn();
	if (absoluteValue(targetAddress)) {
		expand((moreText("target\t0x%x", targetAddress->
			value), endLine()));
		targetOffset = currentLocationCounter.value -
				targetAddress->value;
	} else {
		error(BAD_TARGET_ADDRESS_ERROR, valueKindString(
			targetAddress->kindOfValue));
	}
	qfree(targetAddress);
}

  void
assembleUndefineStatement(undefineStatement)
  undefineStatementBodyType	*undefineStatement;
{
	expand(moreText("undefine\t"));
	while (undefineStatement != NULL) {
		expand(moreText("%s", symbName(undefineStatement->theSymbol)));
		purgeSymbol(effectiveSymbol(undefineStatement->theSymbol,
			NULL));
		undefineStatement = undefineStatement->nextIdentifier;
		expand(undefineStatement != NULL ? moreText(", ") : 0);
	}
	expand(endLine());
}

  void
assembleVariableStatement(variableStatement)
  variableStatementBodyType	*variableStatement;
{
    symbolTableEntryType	*newVariable;
    symbolInContextType		*contextForVariable;
    int				 initCount;

    nullAssemble(variableStatement);
    newVariable = effectiveSymbol(variableStatement->theSymbol,
	&contextForVariable);
    if (isPotentialVariable(contextForVariable)) {
	expand(moreText("variable %s = ", symbName(newVariable)));
	if (contextForVariable->usage == DEAD_SYMBOL) {
	    reincarnateSymbol(contextForVariable, VARIABLE_SYMBOL);
	} else if (contextForVariable->usage == NESTED_UNKNOWN_SYMBOL) {
	    contextForVariable->usage = VARIABLE_SYMBOL;
	}
	if (alreadyDefined(contextForVariable)) {
	    error(REDEFINITION_OF_SYMBOL_ERROR, symbName(newVariable));
	} else {
	    if (variableStatement->theDimension == NULL) {
		if ((initCount = expressionListLength(variableStatement->
			theValue)) == 1) {
		    contextForVariable->value = evaluateExpression(
			variableStatement->theValue->theExpression, NO_FIXUP);
		    expand(expandExpression(NULL));
		} else {
		    if (initCount > 1)
			error(TOO_MANY_VARIABLE_INITIALIZERS_ERROR);
		    expand(moreText("*fail*"));
		    contextForVariable->value = newValue(FAIL, 0,
			EXPRESSION_OPND);
		}
	    } else {
		contextForVariable->value = createArray(variableStatement->
		    theDimension, variableStatement->theValue);
	    }
	    contextForVariable->environmentNumber = GLOBAL_ENVIRONMENT_NUMBER;
	    contextForVariable->attributes |= DEFINED_VARIABLE_ATT;
	}
	expand(endLine());
    } else {
	error(SYMBOL_ALREADY_THERE_ERROR, symbName(newVariable));
    }
}

  void
assembleWhileStatement(whileStatement)
  whileStatementBodyType	*whileStatement;
{
	valueType	 topOfLoop;
	valueType	 fixupLocation[COMPOUND_BRANCH_MAX];

	nullAssemble(whileStatement);
	if (whileStatement->whileCondition == NEVER_COND) {
		return;
	} else if (whileStatement->whileCondition != ALWAYS_COND) {
		emitRelativeBranch(invertConditionCode(whileStatement->
			whileCondition), NULL, fixupLocation);
		topOfLoop = currentLocationCounter;
	} else {
		topOfLoop = currentLocationCounter;
		topOfLoop.value -= targetOffset;
	}
	expand((moreText("while %s {", conditionString(whileStatement->
		whileCondition)), endLine(), tabCount++));
	assembleBlock(whileStatement->whileLoop);
	expand((tabCount--, startLineMarked(), tabIndent(), moreText("}"),
		endLine()));
	if (whileStatement->whileCondition != ALWAYS_COND) {
		emitRelativeBranch(whileStatement->whileCondition, &topOfLoop,
			NULL);
		fixupBranch(fixupLocation, currentLocationCounter);
	} else {
		emitJump(&topOfLoop, NULL);
	}
}

  void
assembleWordStatement(wordStatement)
  wordStatementBodyType	*wordStatement;
{
	valueType		*word;

	nullAssemble(wordStatement);
	expand(moreText("word\t"));
	while (wordStatement != NULL) {
		word = evaluateExpression(wordStatement->theExpression,
			WORD_FIXUP);
		wordStatement = wordStatement->nextExpression;
		expand((expandExpression(NULL), wordStatement != NULL ?
			moreText(", ") : 0));
		putFixupsHere(WORD_FIXUP, 0);
		emitWordValue(word);
		qfree(word);
	}
	expand(endLine());
}

  bool
assembleStatementBody(kind, body, cumulativeLineNumber, worryAboutIf,
								ifFixupList)
  statementKindType	  kind;
  statementBodyType	  body;
  int			  cumulativeLineNumber;
  bool			  worryAboutIf;
  simpleFixupListType	**ifFixupList;
{
	bool	result;

	result = TRUE;
	if (amListing() && kind != INSTRUCTION_STATEMENT && !beneathFunction)
		saveIndexForListing(kind, cumulativeLineNumber);
	switch (kind) {

	case ALIGN_STATEMENT:
		sideEffectBomb();
		assembleAlignStatement(body.alignUnion);
		if (amListing())
			saveIndexForListing(ALIGN_STATEMENT,
				cumulativeLineNumber);
		break;

	case ASSERT_STATEMENT:
		assembleAssertStatement(body.assertUnion);
		break;

	case BLOCK_STATEMENT:
		sideEffectBomb();
		assembleBlockStatement(body.blockUnion);
		break;

	case BYTE_STATEMENT:
		sideEffectBomb();
		assembleByteStatement(body.byteUnion);
		break;

	case CONSTRAIN_STATEMENT:
		sideEffectBomb();
		assembleConstrainStatement(body.constrainUnion);
		break;

	case DBYTE_STATEMENT:
		sideEffectBomb();
		assembleDbyteStatement(body.dbyteUnion);
		break;

	case DEFINE_STATEMENT:
		sideEffectBomb();
		assembleDefineStatement(body.defineUnion);
		break;

	case DO_UNTIL_STATEMENT:
		sideEffectBomb();
		assembleDoUntilStatement(body.doUntilUnion);
		break;

	case DO_WHILE_STATEMENT:
		sideEffectBomb();
		assembleDoWhileStatement(body.doWhileUnion);
		break;

	case EXTERN_STATEMENT:
		sideEffectBomb();
		assembleExternStatement(body.externUnion);
		break;

	case FRETURN_STATEMENT:
		assembleFreturnStatement(body.freturnUnion);
/*		result = FALSE;*/
		break;

	case FUNCTION_STATEMENT:
		assembleFunctionStatement(body.functionUnion);
		break;

	case GROUP_STATEMENT:
		assembleGroupStatement(body.groupUnion);
		break;

	case IF_STATEMENT:
		sideEffectBomb();
		if (worryAboutIf) {
			*ifFixupList = assembleIfStatement(body.ifUnion, worryAboutIf,
				*ifFixupList);
		} else {
			assembleIfStatement(body.ifUnion, worryAboutIf, NULL);
		}
		break;

	case INCLUDE_STATEMENT:
		sideEffectBomb();
		assembleIncludeStatement(body.includeUnion);
		break;

	case INSTRUCTION_STATEMENT:
		sideEffectBomb();
		assembleInstructionStatement(body.instructionUnion, cumulativeLineNumber);
		break;

	case LONG_STATEMENT:
		sideEffectBomb();
		assembleLongStatement(body.longUnion);
		break;

	case MACRO_STATEMENT:
		assembleMacroStatement(body.macroUnion);
		break;

	case MDEFINE_STATEMENT:
		assembleMdefineStatement(body.defineUnion);
		break;

	case MDO_UNTIL_STATEMENT:
		assembleMdoUntilStatement(body.mdoUntilUnion);
		break;

	case MDO_WHILE_STATEMENT:
		assembleMdoWhileStatement(body.mdoWhileUnion);
		break;

	case MFOR_STATEMENT:
		assembleMforStatement(body.mforUnion);
		break;

	case MIF_STATEMENT:
		assembleMifStatement(body.mifUnion, cumulativeLineNumber);
		break;

	case MSWITCH_STATEMENT:
		assembleMswitchStatement(body.mswitchUnion);
		break;

	case MVARIABLE_STATEMENT:
		assembleMvariableStatement(body.mvariableUnion);
		break;

	case MWHILE_STATEMENT:
		assembleMwhileStatement(body.mwhileUnion);
		break;

	case NULL_STATEMENT:
		nullStatementFlag = TRUE;
		expand(labeledLine() ? (startLineMarked(), expandLabel(),
			endLine()) : 0);
		break;

	case ORG_STATEMENT:
		sideEffectBomb();
		assembleOrgStatement(body.orgUnion);
		if (amListing())
			saveIndexForListing(ORG_STATEMENT,
				cumulativeLineNumber);
		break;

	case PERFORM_STATEMENT:
		assemblePerformStatement(body.performUnion);
		break;

	case REL_STATEMENT:
		sideEffectBomb();
		assembleRelStatement(body.relUnion);
		if (amListing())
			saveIndexForListing(REL_STATEMENT,
				cumulativeLineNumber);
		break;

	case START_STATEMENT:
		assembleStartStatement(body.startUnion);
		break;

	case STRING_STATEMENT:
		sideEffectBomb();
		assembleStringStatement(body.stringUnion);
		break;

	case STRUCT_STATEMENT:
		sideEffectBomb();
		assembleStructStatement(body.structUnion);
		break;

	case TARGET_STATEMENT:
		sideEffectBomb();
		assembleTargetStatement(body.targetUnion);
		break;

	case UNDEFINE_STATEMENT:
		sideEffectBomb();
		assembleUndefineStatement(body.undefineUnion);
		break;

	case VARIABLE_STATEMENT:
		sideEffectBomb();
		assembleVariableStatement(body.variableUnion);
		break;

	case WHILE_STATEMENT:
		sideEffectBomb();
		assembleWhileStatement(body.whileUnion);
		break;

	case WORD_STATEMENT:
		sideEffectBomb();
		assembleWordStatement(body.wordUnion);
		break;

	default:
            botch("assembleStatementBody doesn't know kind %d\n", kind, 0, 0);
		break;
	}
/*	return(result);*/
	return(!freturnExit);
}

  void
assembleLabelList(labelList)
  labelListType	*labelList;
{
	while (labelList != NULL) {
		if (structNestingDepth == 0)
			valueLabel(labelList->theLabel,
				newValue(currentLocationCounter.kindOfValue,
					currentLocationCounter.value,
					EXPRESSION_OPND));
		else
			valueField(labelList->theLabel, newValue(FIELD_VALUE,
					currentFieldOffset, EXPRESSION_OPND));
		labelList = labelList->nextLabel;
	}
}

  simpleFixupListType *
assembleStatement(statement, insideIf, ongoingFixupList)
  statementType		*statement;
  bool			 insideIf;
  simpleFixupListType	*ongoingFixupList;
{
	char			*saveFileName;
	int			 saveLineNumber;
	int			 saveCumulativeLineNumber;
	simpleFixupListType	*result;

	statementEvaluationDepth++;
	expand(flushExpressionString());
	saveFileName = currentFileName;
	saveLineNumber = currentLineNumber;
	result = ongoingFixupList;
	while (statement != NULL) {
		currentFileName = statement->fileName;
		currentLineNumber = statement->lineNumber;
		expand(listableStatement(statement->kindOfStatement) ?
			startLine() : 0);
		assembleLabelList(statement->labels);
		expand(listableStatement(statement->kindOfStatement) ?
			(expandLabel(), tabIndent()) : 0);
		if (assembleStatementBody(statement->kindOfStatement,
				statement->statementBody, statement->
				cumulativeLineNumber, insideIf &&
				isLastStatementInBlock(statement), &result)) {
			if (freeFlag && statementEvaluationDepth == 1)
				freeStatement(statement);
			statement = statement->nextStatement;
		} else {
			if (freeFlag && statementEvaluationDepth == 1)
				freeStatement(statement);
			freturnExit = TRUE;
			break;
		}
	}
	if (!newIncludeFile) {
		currentFileName = saveFileName;
		currentLineNumber = saveLineNumber;
	} else {
		newIncludeFile = FALSE;
	}
	statementEvaluationDepth--;
	return(result);
}

  void
eatStatement(statement)
  statementType	*statement;
{
	if (debug) {
		printf("assembling:\n");
		printStatement(statement);
	}
	assembleStatement(statement, FALSE, NULL);
}
