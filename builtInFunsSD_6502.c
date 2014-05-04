/*
	builtInFunsSD_6502.c -- Code for target-processor dependent
		intrinsic user callable	functions in the Macross assembler.
		(6502 version).

	Chip Morningstar -- Lucasfilm Ltd.

	23-April-1985

*/

#include "macrossTypes.h"
#include "macrossGlobals.h"

/* Helper functions, defined in builtInFunctions.c */
valueType	*makeBooleanValue();
valueType	*makeFailureValue();
valueType	*makeIntegerValue();
valueType	*makeOperandValue();
valueType	*makeStringValue();
valueType	*makeUndefinedValue();


/* Check if operand is the accumulator */
  valueType *
isARegisterBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(evaluatedParameter->addressMode ==
			A_REGISTER_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand's address mode is DIRECT */
  valueType *
isDirectModeBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(evaluatedParameter->addressMode ==
			EXPRESSION_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand's address mode is IMMEDIATE */
  valueType *
isImmediateModeBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(evaluatedParameter->addressMode ==
			IMMEDIATE_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand's address mode is INDEXED (either X or Y) */
  valueType *
isIndexedModeBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(evaluatedParameter->addressMode ==
			X_INDEXED_OPND || evaluatedParameter->addressMode ==
			Y_INDEXED_OPND || evaluatedParameter->addressMode ==
			X_SELECTED_OPND || evaluatedParameter->addressMode ==
			Y_SELECTED_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand's address mode is INDIRECT */
  valueType *
isIndirectModeBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(evaluatedParameter->addressMode ==
			INDIRECT_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand's address mode is POST-INDEXED */
  valueType *
isPostIndexedModeBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(evaluatedParameter->addressMode ==
			POST_INDEXED_Y_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand's address mode is PRE-INDEXED */
  valueType *
isPreIndexedModeBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(evaluatedParameter->addressMode ==
			PRE_INDEXED_X_OPND || evaluatedParameter->
			addressMode == PRE_SELECTED_X_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand's address mode is X-INDEXED */
  valueType *
isXIndexedModeBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(evaluatedParameter->addressMode ==
			X_INDEXED_OPND || evaluatedParameter->addressMode ==
			X_SELECTED_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand is the X index register */
  valueType *
isXRegisterBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(evaluatedParameter->addressMode ==
			X_REGISTER_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand's address mode is Y-INDEXED */
  valueType *
isYIndexedModeBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(evaluatedParameter->addressMode ==
			Y_INDEXED_OPND || evaluatedParameter->addressMode ==
			Y_SELECTED_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand is the Y index register */
  valueType *
isYRegisterBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(evaluatedParameter->addressMode ==
			Y_REGISTER_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}
