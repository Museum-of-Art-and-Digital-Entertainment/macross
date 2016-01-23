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
	semanticMisc.c -- Miscellaneous helper routines for the semantic
			  portions of the Macross assembler.

	Chip Morningstar -- Lucasfilm Ltd.

	12-November-1984
*/

#include "macrossTypes.h"
#include "macrossGlobals.h"
#include "y.tab.h"

#include "semanticMisc.h"
#include "expressionSemantics.h"

#define expansionOff() {saveExpansion=expandMacros; expandMacros=FALSE;}
#define expansionOn()	expandMacros=saveExpansion;

/*
   These are miscellaneous routines called by the primary semantics routines.
 */

  bool
absoluteValue(address)
  valueType	*address;
{
	return(address->kindOfValue == ABSOLUTE_VALUE);
}

  void
addAttributeToSymbol(symbol, attribute)
  symbolTableEntryType	*symbol;
  symbolAttributesType	 attribute;
{
	symbolInContextType	*context;

	symbolInContextType	*getBaseContext();

	context = getBaseContext(symbol);
	if (context != NULL)
		context->attributes |= attribute;
}

  addressType
addressValue(value)
  valueType	*value;
{
	if (value->kindOfValue==STRING_VALUE ||
	    value->kindOfValue==CONDITION_VALUE ||
	    value->kindOfValue==FAIL)
		error(VALUE_IS_NOT_AN_ADDRESS_ERROR);
	else
		return(value->value);
}

  valueKindType
addValueKind(leftOperand, rightOperand)
  valueType	*leftOperand;
  valueType	*rightOperand;
{
/* This table MUST be maintained congruently with the definition of the
   enumerated type 'valueKindType'. */
	static valueKindType addValueKindTable[NUM_OF_VALUE_KINDS]
					      [NUM_OF_VALUE_KINDS] = {
/*		    ABSOLUTE_VALUE , DATA_VALUE     , RELOCATABLE_VALUE     ,
		     BSS_VALUE      , STRUCT_VALUE   , FIELD_VALUE    ,
		      MACRO_VALUE    , OPERAND_VALUE  , STRING_VALUE   ,
		       CONDITION_VALUE, UNDEFINED_VALUE, FUNCTION_VALUE ,
			BLOCK_VALUE    , BUILT_IN_FUNCTI, ARRAY_VALUE    ,
		         FAIL           ,*/

/*ABSOLUTE_VALUE */ ABSOLUTE_VALUE , DATA_VALUE     , RELOCATABLE_VALUE     ,
		     BSS_VALUE      , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , UNDEFINED_VALUE, FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
		         FAIL,

/*DATA_VALUE     */ DATA_VALUE     , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , UNDEFINED_VALUE, FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
		         FAIL,

/*RELOCATABLE_VAL*/ RELOCATABLE_VALUE, FAIL         , UNDEFINED_VALUE,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , UNDEFINED_VALUE, FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
		         FAIL,

/*BSS_VALUE      */ BSS_VALUE      , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , UNDEFINED_VALUE, FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
		         FAIL,

/*STRUCT_VALUE   */ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
		         FAIL,

/*FIELD_VALUE    */ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
		         FAIL,

/*MACRO_VALUE    */ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
		         FAIL,

/*OPERAND_VALUE  */ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
		         FAIL,

/*STRING_VALUE   */ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , STRING_VALUE   ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
		         FAIL,

/*CONDITION_VALUE*/ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
		         FAIL,

/*UNDEFINED_VALUE*/ UNDEFINED_VALUE, UNDEFINED_VALUE, UNDEFINED_VALUE,
		     UNDEFINED_VALUE, FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , UNDEFINED_VALUE, FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
		         FAIL,

/*FUNCTION_VALUE */ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
		         FAIL,

/*BLOCK_VALUE    */ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
		         FAIL,

/*BUILT_IN_FUNCT-*/ FAIL           , FAIL           , FAIL           ,
/*-ION_VALUE     */  FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
		         FAIL,

/*ARRAY_VALUE    */ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
		         FAIL,

/*FAIL           */ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
		         FAIL,
	};

	return(addValueKindTable[(int)leftOperand->kindOfValue]
				[(int)rightOperand->kindOfValue]);
}

  bool
alreadyDefined(context)
  symbolInContextType	*context;
{
	return((context->attributes & DEFINED_VARIABLE_ATT) != 0);
}

  bool
booleanTest(expression)
  expressionType	*expression;
{
	bool		 result;
	valueType	*expressionResult;
	bool		 saveExpansion;

	expansionOff();
	expressionResult = evaluateExpression(expression, NO_FIXUP);
	expansionOn();
	if (expressionResult == NULL) {
		return(FALSE);
	} else if (expressionResult->kindOfValue != ABSOLUTE_VALUE) {
		error(INVALID_BOOLEAN_VALUE_ERROR,
			valueKindString(expressionResult->kindOfValue));
		result = FALSE;
	} else {
		result = (expressionResult->value != 0);
	}
	qfree(expressionResult);
	return(result);
}

  int
countArguments(function)
  functionDefinitionType	*function;
{
	int				 result;
	argumentDefinitionListType	*arguments;

	arguments = function->arguments;
	result = 0;
	while (arguments != NULL) {
		result++;
		arguments = arguments->nextArgument;
	}
	return(result);
}

  int
countParameters(parameterList)
  operandListType	*parameterList;
{
	int	result;

	result = 0;
	while (parameterList != NULL) {
		result++;
		parameterList = parameterList->nextOperand;
	}
	return(result);
}

  arrayType *
allocArray(size, contentsPtr)
  int		   size;
  valueType	***contentsPtr;
{
	arrayType	*result;
	int		 i;

	result = typeAlloc(arrayType);
	result->arraySize = size;
	result->arrayContents = *contentsPtr = (valueType **) malloc(size *
		sizeof(valueType *));
	for (i=0; i<size; ++i)
		(*contentsPtr)[i] = NULL;
	return(result);
}

  valueType *
createArray(dimension, initializers)
  expressionType	*dimension;
  expressionListType	*initializers;
{
	int		 initCount;
	valueType	*arraySizeValue;
	int		 arraySize;
	valueType      **arrayContents;
	arrayType	*result;
	int		 i;

	valueType	*newValue();
	
	initCount = expressionListLength(initializers);
	if ((int)dimension == -1) {
		arraySize = initCount;
	} else {
		arraySizeValue = evaluateExpression(dimension, NO_FIXUP);
		if (arraySizeValue->kindOfValue != ABSOLUTE_VALUE) {
			return(newValue(FAIL, 0, EXPRESSION_OPND));
		} else {
			arraySize = arraySizeValue->value;
		}
	}
	result = allocArray(arraySize, &arrayContents);
	if (initCount > arraySize) {
		error(TOO_MANY_VARIABLE_INITIALIZERS_ERROR);
		initCount = arraySize;
	}
	for (i=0; i<initCount; ++i) {
		*arrayContents++ = evaluateExpression(initializers->
			theExpression, NO_FIXUP);
		initializers = initializers->nextExpression;
	}
	return(newValue(ARRAY_VALUE, result, EXPRESSION_OPND));
}

  bool
decrementable(value)
  valueType	*value;
{
/* This table MUST be maintained congruently with the definition of the
   enumerated type 'valueKindType'. */
	static bool decrementableTable[NUM_OF_VALUE_KINDS] = {
	/* ABSOLUTE_VALUE          */  TRUE,
	/* DATA_VALUE              */  TRUE,
	/* RELOCATABLE_VALUE       */  TRUE,
	/* BSS_VALUE               */  TRUE,
	/* STRUCT_VALUE	           */ FALSE,
	/* FIELD_VALUE	           */ FALSE,
	/* MACRO_VALUE	           */ FALSE,
	/* OPERAND_VALUE           */ FALSE,
	/* STRING_VALUE            */ FALSE,
	/* CONDITION_VALUE         */ FALSE,
	/* UNDEFINED_VALUE         */  TRUE,
	/* FUNCTION_VALUE          */ FALSE,
	/* BLOCK_VALUE	           */ FALSE,
	/* BUILT_IN_FUNCTION_VALUE */ FALSE,
	/* ARRAY_VALUE		   */ FALSE,
	/* FAIL                    */ FALSE,
	};

	if (value == NULL)
		return(FALSE);
	else
		return(decrementableTable[(int)value->kindOfValue]);
}

  int
expressionListLength(expressionList)
  expressionListType	*expressionList;
{
	int	result;

	result = 0;
	while (expressionList != NULL) {
		result++;
		expressionList = expressionList->nextExpression;
	}
	return(result);
}

  int
fieldValue(symbol)
  symbolTableEntryType		*symbol;
{
	valueType	*value;
	valueType	*evaluateIdentifier();

	value = evaluateIdentifier(symbol, FALSE, NO_FIXUP);
	if (value->kindOfValue != FIELD_VALUE) {
		error(VALUE_IS_NOT_A_FIELD_ERROR, symbName(symbol));
		return(0);
	} else {
		return(value->value);
	}
}

  bool
incrementable(value)
  valueType	*value;
{
/* This table MUST be maintained congruently with the definition of the
   enumerated type 'valueKindType'. */
	static bool incrementableTable[NUM_OF_VALUE_KINDS] = {
	/* ABSOLUTE_VALUE          */  TRUE,
	/* DATA_VALUE              */  TRUE,
	/* RELOCATABLE_VALUE       */  TRUE,
	/* BSS_VALUE               */  TRUE,
	/* STRUCT_VALUE	           */ FALSE,
	/* FIELD_VALUE	           */ FALSE,
	/* MACRO_VALUE	           */ FALSE,
	/* OPERAND_VALUE           */ FALSE,
	/* STRING_VALUE            */ FALSE,
	/* CONDITION_VALUE         */ FALSE,
	/* UNDEFINED_VALUE         */  TRUE,
	/* FUNCTION_VALUE          */ FALSE,
	/* BLOCK_VALUE	           */ FALSE,
	/* BUILT_IN_FUNCTION_VALUE */ FALSE,
	/* ARRAY_VALUE		   */ FALSE,
	/* FAIL                    */ FALSE,
	};

	if (value == NULL)
		return(FALSE);
	else
		return(incrementableTable[(int)value->kindOfValue]);
}

  int
intValue(value)
  valueType	*value;
{
	if (value->kindOfValue != ABSOLUTE_VALUE) {
		error(VALUE_IS_NOT_AN_INT_ERROR);
		return(0);
	} else {
		return(value->value);
	}
}

  bool
isAssignable(context)
  symbolInContextType	*context;
{
	return( context->usage==ARGUMENT_SYMBOL         ||
		context->usage==VARIABLE_SYMBOL  ||
		context->usage==MVARIABLE_SYMBOL );
}

  bool
isBuiltInFunction(context)
  symbolInContextType	*context;
{
	return(context!=NULL && context->usage==BUILT_IN_FUNCTION_SYMBOL);
}

  bool
isDefinable(context)
  symbolInContextType	*context;
{
	return( context->usage==DEFINE_SYMBOL ||
		context->usage==DEAD_SYMBOL ||
		context->usage==UNKNOWN_SYMBOL ||
		context->usage==NESTED_UNKNOWN_SYMBOL );
}

  bool
isExternal(symbol)
  symbolTableEntryType	*symbol;
{
	symbolInContextType	*context;

	symbolInContextType	*getBaseContext();

	context = getBaseContext(symbol);
	return (context!=NULL && (context->attributes & GLOBAL_ATT)!=0 &&
		context->usage != DEAD_SYMBOL);
}

  bool
isFailure(value)
  valueType	*value;
{
	if (value == NULL)
		return(FALSE);
	else
		return(value->kindOfValue == FAIL);
}

  bool
isFunction(context)
  symbolInContextType	*context;
{
	return(context!=NULL && context->usage==FUNCTION_SYMBOL);
}

  bool
isLastStatementInBlock(statement)
  statementType	*statement;
{
	statement = statement->nextStatement;
	while (statement != NULL) {
		if (statement->kindOfStatement != BLOCK_STATEMENT &&
			statement->kindOfStatement != BYTE_STATEMENT &&
			statement->kindOfStatement != CONSTRAIN_STATEMENT &&
			statement->kindOfStatement != DBYTE_STATEMENT &&
			statement->kindOfStatement != DO_UNTIL_STATEMENT &&
			statement->kindOfStatement != DO_WHILE_STATEMENT &&
			statement->kindOfStatement != GROUP_STATEMENT &&
			statement->kindOfStatement != IF_STATEMENT &&
			statement->kindOfStatement != INCLUDE_STATEMENT &&
			statement->kindOfStatement != INSTRUCTION_STATEMENT &&
			statement->kindOfStatement != LONG_STATEMENT &&
			statement->kindOfStatement != STRING_STATEMENT &&
			statement->kindOfStatement != STRUCT_STATEMENT &&
			statement->kindOfStatement != WHILE_STATEMENT &&
			statement->kindOfStatement != MIF_STATEMENT &&
			statement->kindOfStatement != MWHILE_STATEMENT &&
			statement->kindOfStatement != MFOR_STATEMENT &&
			statement->kindOfStatement != MDO_WHILE_STATEMENT &&
			statement->kindOfStatement != MDO_UNTIL_STATEMENT &&
			statement->kindOfStatement != MSWITCH_STATEMENT &&
			statement->kindOfStatement != WORD_STATEMENT)
		    statement = statement->nextStatement;
		else
		    return(FALSE);
	}
	return(TRUE);
}

  bool
isLogicalOp(op)
  int	op;
{
	return (op==EQUAL_TO || op==GREATER_THAN || op==
		GREATER_THAN_OR_EQUAL_TO || op==LESS_THAN || op==
		LESS_THAN_OR_EQUAL_TO || op==NOT_EQUAL_TO || op==
		LOGICAL_AND || op==LOGICAL_OR || op==LOGICAL_XOR);
}

  bool
isPotentialVariable(context)
  symbolInContextType	*context;
{
	return( context->usage == VARIABLE_SYMBOL ||
		context->usage == DEAD_SYMBOL ||
		context->usage == NESTED_UNKNOWN_SYMBOL );
}

  bool
isUndefined(value)
  valueType	*value;
{
	return(value==NULL || value->kindOfValue==UNDEFINED_VALUE);
}

  bool
isUsable(value)
  valueType	*value;
{
	return(value!=NULL && value->kindOfValue!=UNDEFINED_VALUE &&
			value->kindOfValue!=FAIL);
}

  bool
logicalXOR(int1, int2)
  int	int1;
  int	int2;
{
	return((int1 && !int2) || (int2 && !int1));
}

  valueType *
newValue(kindOfValue, value, addressMode)
  valueKindType	 	kindOfValue;
  int		 	value;
  operandKindType	addressMode;
{
	valueType	*result;

	result = typeAlloc(valueType);
	result->kindOfValue = kindOfValue;
	result->value = value;
	result->addressMode = addressMode;
	return(result);
}

  valueKindType
opValueKind(leftOperand, rightOperand)
  valueType	*leftOperand;
  valueType	*rightOperand;
{
	if (leftOperand->kindOfValue==ABSOLUTE_VALUE && rightOperand->
						kindOfValue==ABSOLUTE_VALUE)
		return(ABSOLUTE_VALUE);
	else if ((leftOperand->kindOfValue==ABSOLUTE_VALUE && rightOperand->
					kindOfValue==UNDEFINED_VALUE) ||
		 (leftOperand->kindOfValue==UNDEFINED_VALUE && rightOperand->
					kindOfValue==ABSOLUTE_VALUE) ||
		 (leftOperand->kindOfValue==RELOCATABLE_VALUE &&rightOperand->
					kindOfValue==UNDEFINED_VALUE) ||
		 (leftOperand->kindOfValue==UNDEFINED_VALUE && rightOperand->
					kindOfValue==RELOCATABLE_VALUE) ||
		 (leftOperand->kindOfValue==RELOCATABLE_VALUE &&rightOperand->
					kindOfValue==ABSOLUTE_VALUE) ||
		 (leftOperand->kindOfValue==ABSOLUTE_VALUE && rightOperand->
					kindOfValue==RELOCATABLE_VALUE) ||
		 (leftOperand->kindOfValue==RELOCATABLE_VALUE &&rightOperand->
					kindOfValue==RELOCATABLE_VALUE) ||
		 (leftOperand->kindOfValue==UNDEFINED_VALUE && rightOperand->
					kindOfValue==UNDEFINED_VALUE))
		return(UNDEFINED_VALUE);
	else
		return(FAIL);
}

  bool
relocatableValue(address)
  valueType	*address;
{
	return( address->kindOfValue==DATA_VALUE ||
		address->kindOfValue==RELOCATABLE_VALUE );
}

  valueKindType
selectValueKind(leftOperand, rightOperand)
  valueType	*leftOperand;
  valueType	*rightOperand;
{
	if (rightOperand->kindOfValue!=FIELD_VALUE ||
	    (leftOperand->kindOfValue!=ABSOLUTE_VALUE &&
	     leftOperand->kindOfValue!=RELOCATABLE_VALUE &&
	     leftOperand->kindOfValue!=DATA_VALUE))
		return(FAIL);
	else
		return(leftOperand->kindOfValue);
}

  valueKindType
subValueKind(leftOperand, rightOperand)
  valueType	*leftOperand;
  valueType	*rightOperand;
{
/* This table MUST be maintained congruently with the definition of the
   enumerated type 'valueKindType'. */
	static valueKindType subValueKindTable[NUM_OF_VALUE_KINDS]
					      [NUM_OF_VALUE_KINDS] = {
/*		    ABSOLUTE_VALUE , DATA_VALUE     , RELOCATABLE_VALUE     ,
		     BSS_VALUE      , STRUCT_VALUE   , FIELD_VALUE    ,
		      MACRO_VALUE    , OPERAND_VALUE  , STRING_VALUE   ,
		       CONDITION_VALUE, UNDEFINED_VALUE, FUNCTION_VALUE ,
			BLOCK_VALUE    , BUILT_IN_FUNCTI, ARRAY_VALUE    ,
			 FAIL           , */

/*ABSOLUTE_VALUE */ ABSOLUTE_VALUE , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
			 FAIL,

/*DATA_VALUE     */ DATA_VALUE     , ABSOLUTE_VALUE , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
			 FAIL,

/*RELOCATABLE_VAL*/ RELOCATABLE_VALUE, FAIL         , UNDEFINED_VALUE,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
			 FAIL,

/*BSS_VALUE      */ BSS_VALUE      , FAIL           , FAIL           ,
		     BSS_VALUE      , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
			 FAIL,

/*STRUCT_VALUE   */ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
			 FAIL,

/*FIELD_VALUE    */ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
			 FAIL,

/*MACRO_VALUE    */ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
			 FAIL,

/*OPERAND_VALUE  */ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
			 FAIL,

/*STRING_VALUE   */ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
			 FAIL,

/*CONDITION_VALUE*/ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
			 FAIL,

/*UNDEFINED_VALUE*/ UNDEFINED_VALUE, FAIL           , UNDEFINED_VALUE,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , UNDEFINED_VALUE, FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
			 FAIL,

/*FUNCTION_VALUE */ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
			 FAIL,

/*BLOCK_VALUE    */ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
			 FAIL,

/*BUILT_IN_FUNCT-*/ FAIL           , FAIL           , FAIL           ,
/*-ION_VALUE     */  FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
			 FAIL,

/*ARRAY_VALUE    */ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
			 FAIL,

/*FAIL           */ FAIL           , FAIL           , FAIL           ,
		     FAIL           , FAIL           , FAIL           ,
		      FAIL           , FAIL           , FAIL           ,
		       FAIL           , FAIL           , FAIL           ,
			FAIL	       , FAIL		, FAIL		 ,
			 FAIL,
	};

	return(subValueKindTable[(int)leftOperand->kindOfValue]
				[(int)rightOperand->kindOfValue]);
}

  int
swab(i)
  int	i;
{
	return(((i & 0xFF) << 8)  |  ((i & 0xFF00) >> 8));
}

  valueType *
swabValue(value)
  valueType *value;
{
	valueType	*newValue();

	return(newValue(value->kindOfValue, swab(value->value), value->
								addressMode));
}

  valueKindType
unopValueKind(operand)
  valueType	*operand;
{
	return(operand->kindOfValue==ABSOLUTE_VALUE || operand->kindOfValue==
		UNDEFINED_VALUE || operand->kindOfValue==RELOCATABLE_VALUE ?
		operand->kindOfValue : FAIL);
}

  void
valueField(symbol, value)
  symbolTableEntryType	*symbol;
  valueType		*value;
{
	symbolInContextType	*workingContext;

	symbolInContextType	*getWorkingContext();
	symbolTableEntryType	*effectiveSymbol();

	symbol = effectiveSymbol(symbol, &workingContext);
	expand(moreLabel("%s:", symbol->symbolName));
	if (workingContext->usage == DEAD_SYMBOL)
		reincarnateSymbol(workingContext, STRUCT_FIELD_SYMBOL);
	if (workingContext->usage!=LABEL_SYMBOL && workingContext->usage!=
			UNKNOWN_SYMBOL && workingContext->usage!=
			NESTED_UNKNOWN_SYMBOL)
		error(SYMBOL_ALREADY_THERE_ERROR, symbName(symbol));
	else if (workingContext->value==NULL || workingContext->value->
					kindOfValue==UNDEFINED_VALUE) {
		workingContext->value = value;
		workingContext->usage = STRUCT_FIELD_SYMBOL;
		workingContext->environmentNumber = GLOBAL_ENVIRONMENT_NUMBER;
	} else
		error(FIELD_ALREADY_DEFINED_ERROR, symbName(symbol));
}

  void
valueLabel(symbol, value)
  symbolTableEntryType	*symbol;
  valueType		*value;
{
	symbolInContextType	*workingContext;

	symbolTableEntryType	*generateLocalLabel();
	symbolInContextType	*getBaseContext();
	symbolTableEntryType	*effectiveSymbol();

	symbol = effectiveSymbol(symbol, &workingContext);
	expand(moreLabel("%s:", symbol->symbolName));
	if (workingContext->usage == DEAD_SYMBOL)
		reincarnateSymbol(workingContext, LABEL_SYMBOL);
	if (workingContext->usage!=LABEL_SYMBOL && workingContext->usage!=
			UNKNOWN_SYMBOL && workingContext->usage!=
			NESTED_UNKNOWN_SYMBOL)
		error(SYMBOL_ALREADY_THERE_ERROR, symbName(symbol));
	else if (symbName(symbol)[0] == '$') {
		symbol = generateLocalLabel(symbol);
		workingContext = getBaseContext(symbol);
		workingContext->value = value;
		if (targetOffset != 0) {
			workingContext->value->kindOfValue = ABSOLUTE_VALUE;
			workingContext->value->value -= targetOffset;
		}
		workingContext->usage = LABEL_SYMBOL;
		workingContext->environmentNumber = GLOBAL_ENVIRONMENT_NUMBER;
	} else if (workingContext->value==NULL || workingContext->value->
					kindOfValue==UNDEFINED_VALUE) {
		workingContext->value = value;
		if (targetOffset != 0) {
			workingContext->value->value -= targetOffset;
			workingContext->value->kindOfValue = ABSOLUTE_VALUE;
		}
		workingContext->usage = LABEL_SYMBOL;
		workingContext->environmentNumber = GLOBAL_ENVIRONMENT_NUMBER;
		if (workingContext->attributes & TENTATIVE_GLOBAL_ATT)
			workingContext->attributes |= GLOBAL_ATT;
	} else
		error(LABEL_ALREADY_DEFINED_ERROR, symbName(symbol));
	symbol->referenceCount++;
}

/*
   These routines deal with fixups and references.
 */

  void
createFixup(expression, location, kindOfFixup, codeMode, whichFixup)
  expressionType	*expression;
  addressType		 location;
  fixupKindType		 kindOfFixup;
  codeBufferKindType	 codeMode;
  int			 whichFixup;
{
	fixupListType	*newFixup;

	expressionType	*generateFixupExpression();

	if (debug || emitPrint)
		printCreateFixup(expression, location, kindOfFixup);
	newFixup = typeAlloc(fixupListType);
	newFixup->locationToFixup = location;
	newFixup->targetOffset = targetOffset;
	newFixup->codeMode = codeMode;
	newFixup->kindOfFixup = kindOfFixup;
	newFixup->fileName = currentFileName;
	newFixup->lineNumber = currentLineNumber;
	expand(flushExpressionString());
	newFixupAddressMode = EXPRESSION_OPND;
	newFixup->theFixupExpression = generateFixupExpression(expression);
	if (newFixupAddressMode != EXPRESSION_OPND)
		fixupAddressMode[whichFixup] = newFixupAddressMode;
	expand(finishOperand ? expandOperand(fixupAddressMode[whichFixup],
		operandBuffer[whichFixup]) : /*expandExpression(NULL)*/0);
	finishOperand = FALSE;
	if (structNestingDepth == 0) {
		newFixup->nextFixup = fixupList;
		fixupList = newFixup;
	} else {
		newFixup->nextFixup = newStruct->structFixups;
		newStruct->structFixups = newFixup;
	}
}

  void
finishUp()
{
	void	performFixups();
	void	performStartAddressFixup();

	if (listingOn)
		terminateListingFiles();
	performFixups(fixupList);
	if (haveUserStartAddress && fixupStartAddress)
		performStartAddressFixup();
	outputObjectFile();
	if (listingOn)
		outputListing();
}
/*
  bool
isReferenceToRemember(reference)
  expressionReferenceListType	*reference;
{
	symbolInContextType	*context;

	symbolInContextType	*getWorkingContext();

	context = getWorkingContext(reference->expressionReferenced);
	if (context != NULL)
		return ((context->usage == LABEL_SYMBOL && context->value->
			kindOfValue != ABSOLUTE_VALUE) || context->usage ==
			EXTERNAL_SYMBOL || context->usage == UNKNOWN_SYMBOL
			|| context->usage == NESTED_UNKNOWN_SYMBOL);
	else
		return(FALSE);
}
*/
  void
noteAnonymousReference()
{
	expressionReferenceListType	*newReference;

	if (!produceLinkableObject || targetOffset != 0)
		return;

	newReference = typeAlloc(expressionReferenceListType);
	newReference->nextReference = expressionReferenceList[(int)
							currentCodeMode];
	expressionReferenceList[(int)currentCodeMode] = newReference;
	newReference->expressionReferenced = NULL;
	newReference->relocation.referenceAddress = currentLocationCounter.
		value;
	newReference->relocation.referenceExpression = -1;
	newReference->relocation.referenceRelative = FALSE;
	newReference->relocation.referenceKind = REF_WORD;
	newReference->relocation.referenceExternal = FALSE;
	newReference->relocation.referenceMode = (int)currentCodeMode;
}

  void
noteReference(expression, kindOfFixup, location, codeMode)
  expressionType	*expression;
  fixupKindType		 kindOfFixup;
  addressType		 location;
  codeBufferKindType	 codeMode;
{
	expressionReferenceListType	*newReference;

	if (!produceLinkableObject)
		return;

	newReference = typeAlloc(expressionReferenceListType);
	numberOfReferencesInList[(int)codeMode]++;
	newReference->nextReference = expressionReferenceList[(int)codeMode];
	expressionReferenceList[(int)codeMode] = newReference;
	newReference->expressionReferenced = expression;
	newReference->relocation.referenceAddress = location;
	newReference->relocation.referenceMode = (int)codeMode;
	newReference->relocation.referenceExpression = 0;
	switch (kindOfFixup) {
		case BYTE_FIXUP:
			newReference->relocation.referenceRelative = FALSE;
			newReference->relocation.referenceKind = REF_BYTE;
			break;
		case WORD_FIXUP:
			newReference->relocation.referenceRelative = FALSE;
			newReference->relocation.referenceKind = REF_WORD;
			break;
		case LONG_FIXUP:
			newReference->relocation.referenceRelative = FALSE;
			newReference->relocation.referenceKind = REF_LONG;
			break;
		case DBYTE_FIXUP:
			newReference->relocation.referenceRelative = FALSE;
			newReference->relocation.referenceKind = REF_DBYTE;
			break;
		case BYTE_RELATIVE_FIXUP:
			newReference->relocation.referenceRelative = TRUE;
			newReference->relocation.referenceKind = REF_BYTE;
			break;
		case WORD_RELATIVE_FIXUP:
			newReference->relocation.referenceRelative = TRUE;
			newReference->relocation.referenceKind = REF_WORD;
			break;
		default:
			botch("bad fixup kind in noteReference %d\n",
					kindOfFixup);
	}
	newReference->relocation.referenceExternal = FALSE; /* irrelevent? */
}

  void
performFixups(fixups)
  fixupListType		*fixups;
{
	valueType	*valueToPoke;

	performingFixups = TRUE;
	statementEvaluationDepth = 2;	/* don't throw away functions! */
	while (fixups != NULL) {
		if (fixups->theFixupExpression == NULL)
			break;
		currentFileName = fixups->fileName;
		currentLineNumber = fixups->lineNumber;
		currentCodeMode = fixups->codeMode;
		targetOffset = fixups->targetOffset;
		valueToPoke = evaluateExpression(fixups->theFixupExpression,
			NO_FIXUP);
		if (isUndefined(valueToPoke)) {
			noteReference(fixups->theFixupExpression, fixups->
				kindOfFixup, fixups->locationToFixup, fixups->
				codeMode);
		} else {
			switch (fixups->kindOfFixup) {

			case BYTE_FIXUP:
				pokeByteValue(fixups->locationToFixup,
					valueToPoke);
				break;

			case WORD_FIXUP:
				pokeWordValue(fixups->locationToFixup,
					valueToPoke);
				break;

			case LONG_FIXUP:
				pokeLongValue(fixups->locationToFixup,
					valueToPoke);
				break;

			case DBYTE_FIXUP:
				pokeWordValue(fixups->locationToFixup,
					swabValue(valueToPoke));
				break;

			case BYTE_RELATIVE_FIXUP:
				pokeRelativeByteValue(fixups->locationToFixup,
					valueToPoke);
				break;

			case WORD_RELATIVE_FIXUP:
				pokeRelativeWordValue(fixups->locationToFixup,
					valueToPoke);
				break;

			default:
				botch("bad fixup at end\n");
			}
		}
		fixups = fixups->nextFixup;
	}
}

  void
performStartAddressFixup()
{
	expressionType	*startAddressExpression;

	expressionType	*generateFixupExpression();

	startAddressExpression = (expressionType *)startAddress;
	startAddress = evaluateExpression(startAddressExpression,NO_FIXUP_OK);
	if (startAddress->kindOfValue == UNDEFINED_VALUE &&
			produceLinkableObject) {
		startAddress = (valueType *)generateFixupExpression(
			startAddressExpression);
	} else if (startAddress->kindOfValue != ABSOLUTE_VALUE &&
			startAddress->kindOfValue != RELOCATABLE_VALUE) {
		error(BAD_START_ADDRESS_ERROR);
		haveUserStartAddress = FALSE;
	} else {
		fixupStartAddress = FALSE;
	}
}

  void
putFixupsHere(kindOfFixupsToPut, whichFixup)
  fixupKindType	kindOfFixupsToPut;
  int		whichFixup;
{
	int	location;

	location = structNestingDepth==0 ? currentLocationCounter.value :
		currentFieldOffset;
	if (pendingFixup[whichFixup] != NULL) {
		createFixup(pendingFixup[whichFixup], location,
			kindOfFixupsToPut, currentCodeMode, whichFixup);
	}
}
/*
  void
putReferencesHere(kindOfReferencesToPut, whichReference)
  fixupKindType	kindOfReferencesToPut;
{
	expressionReferenceListType	*worthyReference;
	int				 currentLocation;
	int				 currentMode;

	if (!produceLinkableObject)
		return;

	currentLocation = structNestingDepth==0 ? 
			currentLocationCounter.value : currentFieldOffset;
	currentMode = structNestingDepth==0 ? (int)currentCodeMode :
			STRUCT_BUFFER;
	while (referencesToNote[whichReference] != NULL) {
		if (isReferenceToRemember(referencesToNote[whichReference])) {
			switch (kindOfReferencesToPut) {
			case BYTE_FIXUP:
				referencesToNote[whichReference]->
					relocation.referenceKind = REF_BYTE;
				break;
			case WORD_FIXUP:
				referencesToNote[whichReference]->
					relocation.referenceKind = REF_WORD;
				break;
			case LONG_FIXUP:
				referencesToNote[whichReference]->
					relocation.referenceKind = REF_LONG;
				break;
			case BYTE_RELATIVE_FIXUP:
				referencesToNote[whichReference]->
					relocation.referenceKind = REF_BYTE;
				referencesToNote[whichReference]->
					relocation.referenceRelative = TRUE;
				break;
			case WORD_RELATIVE_FIXUP:
				referencesToNote[whichReference]->
					relocation.referenceKind = REF_WORD;
				referencesToNote[whichReference]->
					relocation.referenceRelative = TRUE;
				break;
			case DBYTE_FIXUP:
				referencesToNote[whichReference]->
					relocation.referenceKind = REF_DBYTE;
				break;
			default:
				botch("bad fixup kind in put references %d\n",
					kindOfReferencesToPut);
				break;
			}
			numberOfReferencesInList[currentMode]++;
			referencesToNote[whichReference]->
				relocation.referenceAddress = currentLocation;
			referencesToNote[whichReference]->
				relocation.referenceMode = currentMode;
			worthyReference = referencesToNote[whichReference];
			referencesToNote[whichReference] =
				referencesToNote[whichReference]->
				nextReference;
			worthyReference->nextReference =
				expressionReferenceList[currentMode];
			expressionReferenceList[currentMode]= worthyReference;
		} else {
			worthyReference = referencesToNote[whichReference];
			referencesToNote[whichReference] =
				referencesToNote[whichReference]->
				nextReference;
			qsfree(worthyReference);
		}
	}
}
*/

/*
   These routines handle contexts and dynamic symbol creation.
 */

  void
addNewLocalVariable(symbol)
  symbolTableEntryType	*symbol;
{
	identifierListType	*newLocalVariable;

	newLocalVariable = typeAlloc(identifierListType);
	newLocalVariable->theSymbol = symbol;
	newLocalVariable->nextIdentifier = currentLocalVariableList;
	currentLocalVariableList = newLocalVariable;
}

  symbolTableEntryType *
effectiveSymbol(symbol, assignmentTargetContext)
  symbolTableEntryType	*symbol;
  symbolInContextType  **assignmentTargetContext;
{
	symbolInContextType	*context;
	operandType		*operand;
	expressionType		*expression;
	environmentType		*saveEnvironment;

	symbolInContextType	*getWorkingContext();

	context = getWorkingContext(symbol);
	saveEnvironment = currentEnvironment;
	while (context != NULL && context->usage == ARGUMENT_SYMBOL &&
			context->value->kindOfValue == OPERAND_VALUE) {
		operand = (operandType *)context->value->value;
		if (operand->kindOfOperand == EXPRESSION_OPND) {
			expression = operand->theOperand.expressionUnion;
			if (expression->kindOfTerm == IDENTIFIER_EXPR) {
				currentEnvironment = currentEnvironment->
					previousEnvironment;
				context =getWorkingContext(symbol=expression->
					expressionTerm.identifierUnion);
			} else {
				break;
			}
		} else {
			break;
		}
	}
	currentEnvironment = saveEnvironment;
	if (assignmentTargetContext != NULL)
		*assignmentTargetContext = context;
	return(symbol);
}

  symbolTableEntryType *
generateLocalLabel(symbol)
  symbolTableEntryType	*symbol;
{
	stringType		*localLabelString();
	symbolTableEntryType	*lookupOrEnterSymbol();

	return(lookupOrEnterSymbol(localLabelString(symbol), LABEL_SYMBOL));
}

  symbolInContextType *
getBaseContext(identifier)
  symbolTableEntryType	*identifier;
{
	symbolInContextType	*result;

	result = identifier->context;
	while (result != NULL && result->environmentNumber !=
			GLOBAL_ENVIRONMENT_NUMBER)
		result = result->pushedContexts;
	return(result);
}

  symbolInContextType *
getWorkingContext(identifier)
  symbolTableEntryType	*identifier;
{
	symbolInContextType	*result;

	result = identifier->context;
	while (result != NULL && result->environmentNumber !=
			GLOBAL_ENVIRONMENT_NUMBER && result->
			environmentNumber != currentEnvironment->
			environmentNumber)
		result = result->pushedContexts;
	return(result);
}

  stringType *
localLabelString(symbol)
  symbolTableEntryType	*symbol;
{
#define TEMP_SYMBOL_SIZE_LIMIT 200
	char		 nameUnderConstruction[TEMP_SYMBOL_SIZE_LIMIT];

	stringType	*saveString();

	sprintf(nameUnderConstruction, "_%d_", localLabelTagValue(symbol));
	strncat(nameUnderConstruction, &(symbName(symbol)[1]),
		TEMP_SYMBOL_SIZE_LIMIT);
	return(saveString(nameUnderConstruction));
}

  int
localLabelTagValue(symbol)
  symbolTableEntryType	*symbol;
{
	symbolInContextType	*context;

	symbolInContextType	*getWorkingContext();
	void			 addNewLocalVariable();

	context = getWorkingContext(symbol);
	if (context == NULL)
		botch("local label doesn't have symbol context\n");
	if(context->environmentNumber!=currentEnvironment->environmentNumber){
		pushBinding(symbol, newValue(ABSOLUTE_VALUE,
			currentLabelTagNumber, EXPRESSION_OPND),
			LABEL_SYMBOL);
		addNewLocalVariable(symbol);
		return(currentLabelTagNumber);
	} 
	return(context->value->value);
}

  void
addBreak(kind, data)
  codeBreakKindType	kind;
  int			data;
{
	codeBreakType	*newBreak;
	codeBreakType	*buildCodeBreak();

	newBreak = buildCodeBreak(kind, currentLocationCounter.value, data);
	if (codeBreakList == NULL) {
		codeBreakList = lastCodeBreak = newBreak;
	} else {
		lastCodeBreak->nextBreak = newBreak;
		lastCodeBreak = newBreak;
	}
}

  void
reserveAbsolute(startAddress, blockSize)
  addressType	startAddress;
  int		blockSize;
{
	reservationListType	*buildReservation();

	if (reservationList != NULL && reservationList->startAddress +
			reservationList->blockSize == startAddress)
		reservationList->blockSize += blockSize;
	else
		reservationList = buildReservation(startAddress, blockSize,
			reservationList);
}

  bool
listableStatement(kind)
  statementKindType	kind;
{
	return( kind == ALIGN_STATEMENT || kind == BLOCK_STATEMENT ||
		kind == BYTE_STATEMENT || kind == CONSTRAIN_STATEMENT ||
		kind == DBYTE_STATEMENT || kind == DO_UNTIL_STATEMENT ||
		kind == DO_WHILE_STATEMENT || kind == IF_STATEMENT ||
		/*kind == INSTRUCTION_STATEMENT ||*/ kind == ORG_STATEMENT ||
		kind == REL_STATEMENT || kind == START_STATEMENT ||
		kind == STRUCT_STATEMENT || kind == LONG_STATEMENT ||
		kind == STRING_STATEMENT || kind == WHILE_STATEMENT ||
		kind == WORD_STATEMENT || kind == DEFINE_STATEMENT ||
		kind == EXTERN_STATEMENT || kind == INCLUDE_STATEMENT ||
		kind == UNDEFINE_STATEMENT || kind == VARIABLE_STATEMENT ||
		kind == TARGET_STATEMENT);
}
