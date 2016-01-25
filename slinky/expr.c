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
	expr.c -- Expression evaluator for the Slinky linker

	Chip Morningstar -- Lucasfilm Ltd.

	6-November-1985
*/

#include "slinkyTypes.h"
#include "slinkyGlobals.h"
#include "slinkyExpressions.h"
#include "expr.h"
#include "errorStuff.h"
#include "initialize.h"
#include "y.tab.h"

#define overSymbol() (pc+=sizeof(symbolType *))
#define getSymbol() ((symbolType *)getNumber())
#define overFunction() (pc+=sizeof(functionType *))
#define getFunction() ((functionType *)getNumber())
#define overNumber() (pc+=sizeof(addressType))
#define overByte() pc++
#define nextByte(byt) (byt = *pc++)
#define intOp(byt) (byt+256)

static bool		hitFreturn = FALSE;
static addressType	functionResult;

  int
getNumber(void)
{
	register int	result;
	register int	i;
	register int	shift;

	result = 0;
	for (i=0, shift=0; i<sizeof(symbolType *); ++i, shift+=8)
		result |= (*pc++) << shift;
	return(result);
}

  addressType
evaluateArray(void)
{
	error(ARRAY_TERM_IN_OBJECT_ERROR);
	skipArray();
	return(0);
}

  addressType
evaluateAssert(void)
{
	if (!evaluateExpression())
		error(ASSERT_FAILED_ERROR, pc);
	skipString();
}

  addressType
evaluateBinop(void)
{
	int		op;
	symbolType     *leftSymbol;
	addressType	left;
	addressType	right;

	nextByte(op);
	if (intOp(op) == ASSIGN) {
		leftSymbol = getSymbol();
	} else {
		left = evaluateExpression();
	}
	right = evaluateExpression();
	switch (intOp(op)) {
	    case ASSIGN:
		leftSymbol->symbolValue = right;
		return(right);

	    case LOGICAL_OR:
		return(left || right);

	    case LOGICAL_XOR:
		return((left && !right) || (!left && right));

	    case LOGICAL_AND:
		return(left && right);

	    case BITWISE_OR:
		return(left | right);

	    case BITWISE_XOR:
		return(left ^ right);

	    case BITWISE_AND:
		return(left & right);

	    case EQUAL_TO:
		return(left == right);

	    case NOT_EQUAL_TO:
		return(left != right);

	    case LESS_THAN:
		return(left < right);

	    case LESS_THAN_OR_EQUAL_TO:
		return(left <= right);

	    case GREATER_THAN:
		return(left > right);

	    case GREATER_THAN_OR_EQUAL_TO:
		return(left >= right);

	    case LEFT_SHIFT:
		return(left << right);

	    case RIGHT_SHIFT:
		return(left >> right);

	    case ADD:
		return(left + right);

	    case SUB:
		return(left - right);

	    case MUL:
		return(left * right);

	    case DIV:
		return(left / right);

	    case MOD:
		return(left % right);
	}
}

  addressType
evaluateBlock(void)
{
	while (*pc != END_TAG) {
		evaluateExpression();
		if (hitFreturn)
			while (*pc != END_TAG)
				skipExpression();
	}
	overByte();
	return(0);
}

  addressType
evaluateConditionCode(void)
{
	overByte();
	error(CONDITION_CODE_EXPRESSION_ENCOUNTERED_ERROR);
	return(0);
}

  void
pushSymbol(symbolType *symbol, addressType value)
{
	bindingListType		*newBinding;

	newBinding = typeAlloc(bindingListType);
	newBinding->boundSymbol = symbol;
	newBinding->previousClass = symbol->symbolClass;
	symbol->symbolClass = SYMBOL_LOCAL;
	newBinding->previousValue = symbol->symbolValue;
	symbol->symbolValue = value;
	newBinding->nextBinding = localBindings;
	localBindings = newBinding;
}

  void
bindFunctionArguments(functionType *theFunction, int argCount)
{
	argumentListType	*argList;

	argList = theFunction->functionArguments;
	while (argCount > 0) {
		if (argList == NULL) {
			error(TOO_MANY_FUNCTION_ARGUMENTS_ERROR);
			while (argCount-- > 0)
				skipExpression();
		} else {
			pushSymbol(argList->argumentSymbol,
					evaluateExpression());
			argList = argList->nextArgument;
			argCount--;
		}
	}
	while (argList != NULL) {
		pushSymbol(argList->argumentSymbol, 0);
		argList = argList->nextArgument;
	}
}

  void
undoBindings(void)
{
	bindingListType	*deadBinding;

	while (localBindings != NULL) {
		localBindings->boundSymbol->symbolClass = localBindings->
			previousClass;
		localBindings->boundSymbol->symbolValue = localBindings->
			previousValue;
		deadBinding = localBindings;
		localBindings = localBindings->nextBinding;
		free(deadBinding);
	}
}

  addressType
evaluateFreturn(void)
{
	hitFreturn = TRUE;
	functionResult = evaluateExpression();
	return(0);
}

  addressType
evaluateBuiltinFunctionCall(void)
{
	int	theFunction;
	int	argCount;

	theFunction = getNumber();
	nextByte(argCount);
	if (theFunction<0 || MAX_FUNCTIONS<=theFunction) {
		printf("illegal built-in function #%d\n", theFunction);
		chokePukeAndDie();
	}
	return((*builtInFunctionTable[theFunction].functionEntry)(argCount));
}

  addressType
evaluateFunctionCall(void)
{
	expressionPCType	savePoint;
	functionType	       *theFunction;
	int			argCount;
	bindingListType	       *saveBindings;

	theFunction = getFunction();
	nextByte(argCount);
	saveBindings = localBindings;
	localBindings = NULL;
	bindFunctionArguments(theFunction, argCount);
	savePoint = pc;
	pc = theFunction->functionBody;
	evaluateExpression();
	undoBindings();
	localBindings = saveBindings;
	pc = savePoint;
	if (hitFreturn) {
		hitFreturn = FALSE;
		return(functionResult);
	} else
		return(0);
}

  addressType
evaluateHere(void)
{
	return(here);
}

  addressType
evaluateMdefine(void)
{
	symbolType	*symbol;

	symbol = getSymbol();
	pushSymbol(symbol, evaluateExpression);
}

  addressType
evaluateMdoUntil(void)
{
	expressionPCType	testPoint;
	expressionPCType	endPoint;

	testPoint = pc;
	skipExpression();
	do {
		evaluateExpression;
		endPoint = pc;
		pc = testPoint;
	} while (!evaluateExpression());
	pc = endPoint;
	return(0);
}

  addressType
evaluateMdoWhile(void)
{
	expressionPCType	testPoint;
	expressionPCType	endPoint;

	testPoint = pc;
	skipExpression();
	do {
		evaluateExpression;
		endPoint = pc;
		pc = testPoint;
	} while (evaluateExpression());
	pc = endPoint;
	return(0);
}

  addressType
evaluateMfor(void)
{
	expressionPCType	testPoint;
	expressionPCType	incrPoint;
	expressionPCType	endPoint;
	expressionPCType	bodyPoint;

	evaluateExpression();
	testPoint = pc;
	if (evaluateExpression()) {
		incrPoint = pc;
		skipExpression();
		bodyPoint = pc;
		do {
			pc = bodyPoint;
			evaluateExpression();
			endPoint = pc;
			pc = incrPoint;
			evaluateExpression();
			pc = testPoint;
		} while (evaluateExpression());
		pc = endPoint;
	} else {
		skipExpression();
		skipExpression();
	}
	return(0);
}

  addressType
evaluateMif(void)
{
	if (evaluateExpression()) {
		evaluateExpression();
		skipExpression();
	} else {
		skipExpression();
		evaluateExpression();
	}
	return(0);
}

  bool
evaluateClause(addressType pattern)
{
	bool	match;

	match = FALSE;
	while (*pc != BLOCK_TAG)
		if (match)
			skipExpression();
		else
			match = (evaluateExpression() == pattern);
	if (match)
		evaluateExpression();
	else
		skipExpression();
	return(match);
}

  addressType
evaluateMswitch(void)
{
	addressType	pattern;

	pattern = evaluateExpression();
	while (*pc != END_TAG)
		if (evaluateClause(pattern))
			break;
	while (*pc != END_TAG)
		skipClause();
	overByte();
	return(0);
}

  addressType
evaluateMwhile(void)
{
	expressionPCType	testPoint;
	expressionPCType	endPoint;

	testPoint = pc;
	if (evaluateExpression()) {
		do {
			evaluateExpression();
			endPoint = pc;
			pc = testPoint;
		} while (evaluateExpression());
		pc = endPoint;
	} else {
		skipExpression();
	}
}

  addressType
evaluateMvariable(void)
{
	symbolType	*symbol;

	symbol = getSymbol();
	pushSymbol(symbol, evaluateExpression);
}

  addressType
evaluateNumber(void)
{
	addressType	result;
	int		i;
	int		shift;

	result = 0;
	for (i=0, shift=0; i<sizeof(addressType); ++i, shift+=8)
		result |= (*pc++) << shift;
	return(result);
}

  addressType
evaluateRelocatableNumber(void)
{
	return(evaluateNumber() + relocationOffset);
}

  addressType
evaluatePerform(void)
{
	evaluateExpression();
	return(0);
}

  addressType
evaluatePostop(void)
{
	int		op;
	symbolType     *target;

	nextByte(op);
	target = getSymbol();
	switch (intOp(op)) {
	    case INCREMENT:
		return(target->symbolValue++);

	    case DECREMENT:
		return(target->symbolValue--);
	}
}

  addressType
evaluatePreop(void)
{
	int		op;
	symbolType     *target;

	nextByte(op);
	target = getSymbol();
	switch (intOp(op)) {
	    case INCREMENT:
		return(++target->symbolValue);

	    case DECREMENT:
		return(--target->symbolValue);
	}
}

  addressType
evaluateString(void)
{
	addressType	result;

	result = (addressType) pc;
	while (*pc++ != '\0')
		;
	return(result);
}

  addressType
evaluateSymbol(void)
{
	symbolType     *target;

	target = getSymbol();
	return(target->symbolValue);
}

  addressType
evaluateUnop(void)
{
	int		op;
	addressType	arg;

	nextByte(op);
	arg = evaluateExpression();
	switch(intOp(op)) {
	    case UNARY_MINUS:
		return(-arg);

	    case LOGICAL_NOT:
		return(!arg);

	    case BITWISE_NOT:
		return(~arg);

	    case HI_BYTE:
		return((arg & 0xFF00) >> 8);

	    case LO_BYTE:
		return(arg & 0xFF);
	}
}

  addressType
evaluateExpression(void)
{
	if (pc == NULL)
		return(0);
	switch (*pc++) {
	    case IDENTIFIER_TAG:
		return(evaluateSymbol());

	    case FUNCTION_CALL_TAG:
		return(evaluateFunctionCall());

	    case BUILTIN_FUNCTION_CALL_TAG:
		return(evaluateBuiltinFunctionCall());

	    case NUMBER_TAG:
		return(evaluateNumber());

	    case RELOCATABLE_TAG:
		return(evaluateRelocatableNumber());

	    case CONDITION_CODE_TAG:
		return(evaluateConditionCode());

	    case SUBEXPRESSION_TAG:
		return(evaluateExpression());

	    case UNOP_TAG:
		return(evaluateUnop());

	    case BINOP_TAG:
		return(evaluateBinop());

	    case PREOP_TAG:
		return(evaluatePreop());

	    case POSTOP_TAG:
		return(evaluatePostop());

	    case HERE_TAG:
		return(evaluateHere());

	    case STRING_TAG:
		return(evaluateString());

	    case ARRAY_TAG:
		return(evaluateArray());

	    case VALUE_TAG:
		return(evaluateNumber());

	    case NULL_TAG:
		return(0);

	    case BLOCK_TAG:
		return(evaluateBlock());

	    case MDEFINE_TAG:
		return(evaluateMdefine());

	    case MVARIABLE_TAG:
		return(evaluateMvariable());

	    case MIF_TAG:
		return(evaluateMif());

	    case MFOR_TAG:
		return(evaluateMfor());

	    case MWHILE_TAG:
		return(evaluateMwhile());

	    case MDOWHILE_TAG:
		return(evaluateMdoWhile());

	    case MDOUNTIL_TAG:
		return(evaluateMdoUntil());

	    case PERFORM_TAG:
		return(evaluatePerform());

	    case GROUP_TAG:
		return(evaluateBlock());

	    case ASSERT_TAG:
		return(evaluateAssert());

	    case MSWITCH_TAG:
		return(evaluateMswitch());

	    case CLAUSE_TAG:
		error(CLAUSE_AT_TOP_LEVEL_ERROR);
		chokePukeAndDie();

	    case FRETURN_TAG:
		return(evaluateFreturn());

	    case END_TAG:
		return(0);
	}
}

  void
skipArray(void)
{
	overSymbol();
	skipExpression();
}

  void
skipAssert(void)
{
	skipExpression();
	skipString();
}

  void
skipBinop(void)
{
	overByte();
	skipExpression();
	skipExpression();
}

  void
skipBlock(void)
{
	while (*pc != END_TAG)
		skipExpression();
	overByte();
}

  void
skipFunctionCall(void)
{
	int	argCount;

	overFunction();
	nextByte(argCount);
	while (argCount-- > 0)
		skipExpression();
}

  void
skipMdefine(void)
{
	overSymbol();
	skipExpression();
}

  void
skipMdoUntil(void)
{
	skipExpression();
	skipExpression();
}

  void
skipMdoWhile(void)
{
	skipExpression();
	skipExpression();
}

  void
skipMfor(void)
{
	skipExpression();
	skipExpression();
	skipExpression();
	skipExpression();
}

  void
skipMif(void)
{
	skipExpression();
	skipExpression();
	skipExpression();
}

  void
skipClause(void)
{
	while (*pc != BLOCK_TAG)
		skipExpression;
	skipBlock();
}

  void
skipMswitch(void)
{
	skipExpression();
	while (*pc != END_TAG)
		skipClause();
	overByte();
}

  void
skipMvariable(void)
{
	overSymbol();
	skipExpression();
}

  void
skipMwhile(void)
{
	skipExpression();
	skipExpression();
}

  void
skipPostop(void)
{
	overByte();
	skipExpression();
}

  void
skipPreop(void)
{
	overByte();
	skipExpression();
}

  void
skipString(void)
{
	while (*pc++ != '\0')
		;
}

  void
skipUnop(void)
{
	overByte();
	skipExpression();
}

  void
skipExpression(void)
{
	if (pc == NULL)
		return;
	switch (*pc++) {
	    case IDENTIFIER_TAG:
		overSymbol();
		break;

	    case FUNCTION_CALL_TAG:
		skipFunctionCall();
		break;

	    case NUMBER_TAG:
	    case RELOCATABLE_TAG:
		overNumber();
		break;

	    case CONDITION_CODE_TAG:
		overByte();
		break;

	    case SUBEXPRESSION_TAG:
		skipExpression();
		break;

	    case UNOP_TAG:
		skipUnop();
		break;

	    case BINOP_TAG:
		skipBinop();
		break;

	    case PREOP_TAG:
		skipPreop();
		break;

	    case POSTOP_TAG:
		skipPostop();
		break;

	    case HERE_TAG:
		break;

	    case STRING_TAG:
		skipString();
		break;

	    case ARRAY_TAG:
		skipArray();
		break;

	    case VALUE_TAG:
		overNumber();
		break;

	    case NULL_TAG:
		break;

	    case BLOCK_TAG:
		skipBlock();
		break;

	    case MDEFINE_TAG:
		skipMdefine();
		break;

	    case MVARIABLE_TAG:
		skipMvariable();
		break;

	    case MIF_TAG:
		skipMif();
		break;

	    case MFOR_TAG:
		skipMfor();
		break;

	    case MWHILE_TAG:
		skipMwhile();
		break;

	    case MDOWHILE_TAG:
		skipMdoWhile();
		break;

	    case MDOUNTIL_TAG:
		skipMdoUntil();
		break;

	    case PERFORM_TAG:
		skipExpression();
		break;

	    case GROUP_TAG:
		skipBlock();
		break;

	    case ASSERT_TAG:
		skipAssert();
		break;

	    case MSWITCH_TAG:
		skipMswitch();
		break;

	    case CLAUSE_TAG:
		skipClause();
		break;

	    case END_TAG:
		break;
	}
}
