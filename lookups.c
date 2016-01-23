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
	lookups.c -- Routines to lookup things in tables.

	Chip Morningstar -- Lucasfilm Ltd.
	
	5-November-1984
*/

#include "macrossTypes.h"
#include "macrossGlobals.h"

/*
   These routines all do basically the same thing.  Various kinds of keywords
   and so forth are stored in hash tables.  The hash buckets contain linked
   lists that are sorted alphabetically.
 */

  conditionType
lookupConditionCode(s, hashValue)
  char	*s;
  int	 hashValue;
{
	conditionTableEntryType	*result;

	genericTableEntryType	*prehashedStringLookup();

	result = (conditionTableEntryType *) prehashedStringLookup(s,
		conditionTable, hashValue);
	if (result != NULL)
		return(result->code);
	else
		return(NOT_FOUND_COND);
}

  int
lookupKeyword(s, hashValue)
  char	*s;
  int	 hashValue;
{
	keywordTableEntryType	*result;

	genericTableEntryType	*prehashedStringLookup();

	result = (keywordTableEntryType *) prehashedStringLookup(s,
		keywordTable, hashValue);
	if (result != NULL)
		return(result->token);
	else
		return(0);
}

  macroTableEntryType *
lookupMacroName(s, hashValue)
  char	*s;
  int	 hashValue;
{
	genericTableEntryType	*prehashedStringLookup();

	return((macroTableEntryType *) prehashedStringLookup(s, macroTable,
		hashValue));
}

  opcodeTableEntryType *
lookupOpcode(s, hashValue)
  char	*s;
  int	 hashValue;
{
	genericTableEntryType	*prehashedStringLookup();

	return((opcodeTableEntryType *) prehashedStringLookup(s,
		opcodeTable, hashValue));
}

/* lookupOrEnterSymbol -- if there is an entry in the symbol table for the
   given symbol, return that entry, otherwise create a new entry for it of
   the given kind and return *that* */

  symbolTableEntryType *
lookupOrEnterSymbol(s, kind)
  stringType		*s;
  symbolUsageKindType	 kind;
{
	symbolTableEntryType	*result;
	genericTableEntryType	*hashStringLookup();
	genericTableEntryType	*hashStringEnter();
	symbolTableEntryType	*buildSymbolTableEntry();

	if (result = (symbolTableEntryType *)hashStringLookup(s,symbolTable)){
/*		result->referenceCount++;*/
		return(result);
	} else
		return((symbolTableEntryType *)
			hashStringEnter(buildSymbolTableEntry(s, kind),
			symbolTable));
}

  void
pushSymbol(symbol)
  symbolTableEntryType *symbol;
{
	symbolInContextType	*newContext;

	newContext = typeAlloc(symbolInContextType);
	newContext->pushedContexts = symbol->context;
	symbol->context = newContext;
}

  void
popSymbol(symbol)
  symbolTableEntryType	*symbol;
{
	symbolInContextType	*deadContext;

	deadContext = symbol->context;
	if (deadContext == NULL)
		botch("symbol pop underflow\n");
	else {
		symbol->context = deadContext->pushedContexts;
		if (freeFlag) {
			if (deadContext->value != NULL)
				freeValue(deadContext->value);
			free(deadContext);
		}
	}
}

  macroTableEntryType *
createMacro(macroName)
  stringType	*macroName;
{
	macroTableEntryType		*result;
	symbolTableEntryType		*testSymbol;

	genericTableEntryType		*hashStringLookup();
	genericTableEntryType		*hashStringEnter();
	macroTableEntryType		*buildMacroTableEntry();
	symbolTableEntryType		*lookupOrEnterSymbol();

	testSymbol = lookupOrEnterSymbol(macroName, MACRO_SYMBOL);
	if (testSymbol->context->usage != MACRO_SYMBOL) {
		error(SYMBOL_ALREADY_THERE_ERROR, symbName(testSymbol));
		return(NULL);
	} else if (hashStringLookup(macroName, macroTable) != NULL) {
		error(SYMBOL_ALREADY_THERE_ERROR, symbName(testSymbol));
		return(NULL);
	} else {
		result = (macroTableEntryType *)
			hashStringEnter(buildMacroTableEntry(macroName),
			macroTable);
		result->body = NULL;
		return(result);
	}
}


/*
   Generic table lookup utility routines
 */

  genericTableEntryType *
prehashedStringLookup(s, table, hashValue)
  char			*s;
  genericTableEntryType	*table[];
  int			 hashValue;
{
	genericTableEntryType	*result;
	int			 test;

	result = table[hashValue];
	while (result != NULL) {
		if ((test = strcmplc(s, result->string)) == 0)
			break;
		else if (test > 0) {
			result = NULL;
			break;
		} else {
			result = result->next;
		}
	}
	return(result);
}

  genericTableEntryType *
hashStringLookup(s, table)
  char			*s;
  genericTableEntryType	*table[];
{
	return(prehashedStringLookup(s, table, hashString(s)));
}

  genericTableEntryType *
hashStringEnter(entry, table)
  genericTableEntryType	*entry;
  genericTableEntryType	*table[];
{
	genericTableEntryType	*result;
	genericTableEntryType	*oldResult;
	int			 test;
	int			 hashValue;

	hashValue = hashString(entry->string);
	result = table[hashValue];
	if (result == NULL) {
		table[hashValue] = entry;
		entry->next = NULL;
		return(entry);
	}
	oldResult = NULL;
	while (result != NULL) {
		if ((test = strcmplc(entry->string, result->string)) == 0) {
			botch("symbol table entry %s already there\n",
							entry->string);
		} else if (test > 0) {
			entry->next = result;
			if (oldResult == NULL)
				table[hashValue] = entry;
			else
				oldResult->next = entry;
			return(entry);
		} else {
			oldResult = result;
			result = result->next;
		}
	}
	if (oldResult == NULL)
		table[hashValue] = entry;
	else
		oldResult->next = entry;
	entry->next = NULL;
	return(entry);
}

  int
hashString(s)
  char	*s;
{
	unsigned	result;

	result = 0;
	while (*s != '\0')
		result = (result << 1) + toLowerCase(*s++);
	return(result & HASH_TABLE_MASK);
}

  bool
strcmplc(s1, s2)	/* string compare in lower case */
  char *s1;		/*  heavily optimized version */
  char *s2;
{
	char	c1;
	int	result;

	do {
		c1 = toLowerCase(*s1++);

		/* if result != 0, they differ */
		if (result = c1 - toLowerCase(*s2++)) {
			return(result);	     /* c1<c2==neg, c1>c2==pos */
		} else if (!c1) { 	     /* if they're null, we're done */
			return(0);
		}
	} while (TRUE);
}

  bool
strcmplct(s1, s2)	/* For tables: s2 is already lower case */
  char *s1;		/*  heavily optimized version.  */
  char *s2;
{
	char	c1;
	int	result;

	while (TRUE) {
		c1 = toLowerCase(*s1++);
		if (result = c1 - (*s2++)) { /* if result != 0, they differ */
			return(result);	     /* c1<c2==neg, c1>c2==pos */
		} else if (!c1) { 	     /* if they're null, we're done */
			return(0);
		}
	}
}

  void
purgeSymbol(symbol)
  symbolTableEntryType	*symbol;
{
	symbolInContextType	*context;

	symbolInContextType	*getWorkingContext();

	if ((context = getWorkingContext(symbol)) != NULL)
		context->usage = DEAD_SYMBOL;
}

  void
reincarnateSymbol(context, newUsage)
  symbolInContextType	*context;
  symbolUsageKindType	 newUsage;
{
	context->attributes = 0;
	dupValue(context->value, UndefinedValue);
	context->usage = newUsage;
}


/*
   Routines to handle assembly-time binding of symbols to contexts.
 */

  void
pushBinding(symbol, newBinding, newUsage)
  symbolTableEntryType	*symbol;
  valueType		*newBinding;
  symbolUsageKindType	 newUsage;
{
	valueType	*newValue();

	pushSymbol(symbol);
	if (newBinding == NULL)
		newBinding = newValue(FAIL, 0, EXPRESSION_OPND);
	symbol->context->value = newBinding;
	symbol->context->usage = newUsage;
	symbol->context->environmentNumber = currentEnvironment->
		environmentNumber;
	symbol->context->attributes = DEFINED_VARIABLE_ATT;
}

  void
popBinding(symbol)
  symbolTableEntryType	*symbol;
{
	popSymbol(symbol);
}

  int	/* returns number of bindings completed, negative this if failure */
bindMacroArguments(argumentList, parameterList, macroName)
  argumentDefinitionListType	*argumentList;
  operandListType		*parameterList;
  stringType			*macroName;
{
	int			 numberBound;
	bool			 arrayTag;
	int			 arrayLength;
	valueType	       **arrayContents;
	int			 i;

	valueType		*newValue();

	if (argumentList == NULL)
		arrayTag = FALSE;
	else
		arrayTag = ((argumentListHeadType *)argumentList)->arrayTag;
	numberBound = 1;
	while (argumentList!=NULL && (!arrayTag || argumentList->
			nextArgument!=NULL) && parameterList!=NULL) {
		pushBinding(argumentList->theArgument, newValue(OPERAND_VALUE,
			parameterList, EXPRESSION_OPND), ARGUMENT_SYMBOL);
		argumentList = argumentList->nextArgument;
		parameterList = parameterList->nextOperand;
		numberBound++;
	}
	if (!arrayTag) {
		if (parameterList != NULL) {
			error(TOO_MANY_ARGUMENTS_TO_MACRO_ERROR, macroName,
				numberBound-1);
			return(-numberBound);
		}
		while (argumentList != NULL) {
			pushBinding(argumentList->theArgument, newValue(FAIL,
				0, EXPRESSION_OPND), ARGUMENT_SYMBOL);
			argumentList = argumentList->nextArgument;
			numberBound++;
		}
	} else {
		if (parameterList == NULL) {
			while (argumentList->nextArgument != NULL) {
				pushBinding(argumentList->theArgument,
					newValue(FAIL, 0, EXPRESSION_OPND),
					ARGUMENT_SYMBOL);
				argumentList = argumentList->nextArgument;
				numberBound++;
			}
		} 
		arrayLength = countParameters(parameterList);
		pushBinding(argumentList->theArgument, newValue(ARRAY_VALUE,
			allocArray(arrayLength, &arrayContents),
			EXPRESSION_OPND), ARGUMENT_SYMBOL);
		for (i=0; i<arrayLength; ++i) {
			arrayContents[i] = newValue(OPERAND_VALUE,
				parameterList, EXPRESSION_OPND);
			parameterList = parameterList->nextOperand;
		}
		numberBound++;
	}
	return(numberBound);
}

  int	/* returns number of bindings completed, negative this if failure */
bindFunctionArguments(argumentList, parameterList, functionName)
  argumentDefinitionListType	*argumentList;
  operandListType		*parameterList;
  stringType			*functionName;
{
	valueType	*argument;
	bool		 arrayTag;
	int		 arrayLength;
	valueType      **arrayContents;
	int		 i;
	int		 numberBound;
	valueType	*firstArgument;
	environmentType	*saveEnvironment;

	valueType	*evaluateOperand();
	valueType	*newValue();

	if (argumentList == NULL)
		arrayTag = FALSE;
	else
		arrayTag = ((argumentListHeadType *)argumentList)->arrayTag;
	numberBound = 1;
	firstArgument = NULL;
	while (argumentList!=NULL && (!arrayTag || argumentList->
			nextArgument!=NULL) && parameterList!=NULL) {
		saveEnvironment = currentEnvironment;
		currentEnvironment = currentEnvironment->previousEnvironment;
		argument = evaluateOperand(parameterList);
		currentEnvironment = saveEnvironment;
		if (firstArgument == NULL)
			firstArgument = argument;
		if (isUsable(argument)) {
			pushBinding(argumentList->theArgument, argument,
				ARGUMENT_SYMBOL);
			argumentList = argumentList->nextArgument;
			parameterList = parameterList->nextOperand;
			expand((argumentList!=NULL && parameterList!=NULL) ?
				moreExpression(", ") : 0);
			numberBound++;
		} else {
			if (isUndefined(argument))
				resultOfLastFunctionCall =
					newValue(UNDEFINED_VALUE, 0,
						firstArgument->addressMode);
			return(-numberBound);
		}
	}
	if (!arrayTag) {
		if (parameterList != NULL) {
			error(TOO_MANY_ARGUMENTS_TO_FUNCTION_ERROR,
				functionName, numberBound - 1);
			return(-numberBound);
		}
		while (argumentList != NULL) {
			pushBinding(argumentList->theArgument, newValue(FAIL,
				0, EXPRESSION_OPND), ARGUMENT_SYMBOL);
			argumentList = argumentList->nextArgument;
			numberBound++;
		}
	} else {
		if (parameterList == NULL) {
			while (argumentList->nextArgument != NULL) {
				pushBinding(argumentList->theArgument,
					newValue(FAIL, 0, EXPRESSION_OPND),
					ARGUMENT_SYMBOL);
				argumentList = argumentList->nextArgument;
				numberBound++;
			}
		}
		arrayLength = countParameters(parameterList);
		pushBinding(argumentList->theArgument, newValue(ARRAY_VALUE,
			allocArray(arrayLength, &arrayContents),
			EXPRESSION_OPND), ARGUMENT_SYMBOL);
		numberBound++;
		for (i=0; i<arrayLength; ++i) {
			saveEnvironment = currentEnvironment;
			currentEnvironment = currentEnvironment->
				previousEnvironment;
			argument = evaluateOperand(parameterList);
			currentEnvironment = saveEnvironment;
			if (firstArgument == NULL)
				firstArgument = argument;
			if (isUsable(argument)) {
				arrayContents[i] = argument;
				parameterList = parameterList->nextOperand;
			} else {
				if (isUndefined(argument))
					resultOfLastFunctionCall =
						newValue(UNDEFINED_VALUE, 0,
						firstArgument->addressMode);
				return(-numberBound);
			}
		}
	}
	return(numberBound);
}

  void
unbindArguments(argumentList, numberToUnbind)
  argumentDefinitionListType	*argumentList;
  int				 numberToUnbind;
{
	while (argumentList != NULL  &&  numberToUnbind-- > 0) {
		popBinding(argumentList->theArgument);
		argumentList = argumentList->nextArgument;
	}
	if (numberToUnbind > 0)
		botch("binding count larger than number of bindings\n");
}

  void
unbindLocalVariables(identifierList)
  identifierListType	*identifierList;
{
	identifierListType	*deadEntry;

	while (identifierList != NULL) {
		popBinding(identifierList->theSymbol);
		deadEntry = identifierList;
		identifierList = identifierList->nextIdentifier;
		qfree(deadEntry);
	}
}
