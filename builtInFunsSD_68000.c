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
	builtInFunsSD_68000.c -- Code for target-processor dependent
		intrinsic user callable	functions in the Macross assembler.
		(68000 version).

	Chip Morningstar -- Lucasfilm Ltd.

	26-April-1985

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


/* Get address register component of operand */
  valueType *
getAddressRegisterBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeIntegerValue(getAddressRegister(
			evaluatedParameter->addressMode)));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Get data register component of operand */
  valueType *
getDataRegisterBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeIntegerValue(getDataRegister(evaluatedParameter->
			addressMode)));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Get index register component of operand */
  valueType *
getIndexRegisterBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeIntegerValue(getIndexRegister(evaluatedParameter->
			addressMode)));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Get register component of operand */
  valueType *
getRegisterBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeIntegerValue(getRegister(evaluatedParameter->
			addressMode)));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Get word/long component of operand */
  valueType *
getWLBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeIntegerValue(getWL(evaluatedParameter->
			addressMode)));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand's address mode is absolute */
  valueType *
isAbsoluteModeBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == ABSOLUTE_SHORT_OPND ||
			operandKindField(evaluatedParameter->addressMode) ==
			ABSOLUTE_LONG_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand's address mode is absolute long */
  valueType *
isAbsoluteLongModeBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == ABSOLUTE_LONG_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand's address mode is absolute short */
  valueType *
isAbsoluteShortModeBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == ABSOLUTE_SHORT_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if operand is an address register */
  valueType *
isARegisterBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == A_REGISTER_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if operand is the condition code register */
  valueType *
isCCRegisterBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == CC_REGISTER_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if operand is a control register */
  valueType *
isControlRegisterBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == CONTROL_REGISTER_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if operand is a data register */
  valueType *
isDRegisterBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == D_REGISTER_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if operand is DFC register */
  valueType *
isDFCRegisterBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == CONTROL_REGISTER_OPND &&
			getRegister(evaluatedParameter->addressMode) ==
			DFC_REGISTER));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand's address mode is displacement */
  valueType *
isDisplacementModeBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == DISPLACEMENT_OPND));
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
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == IMMEDIATE_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand's address mode is indexed */
  valueType *
isIndexedModeBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == INDEXED_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand's address mode is indirect */
  valueType *
isIndirectModeBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == A_REGISTER_INDIRECT_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand's address mode is pc displacement */
  valueType *
isPCDisplacementModeBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == PC_DISPLACEMENT_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand's address mode is pc indexed */
  valueType *
isPCIndexedModeBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == PC_INDEXED_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand's address mode is postincrement */
  valueType *
isPostincrementModeBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == POSTINCREMENT_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if an operand's address mode is predecrement */
  valueType *
isPredecrementModeBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == PREDECREMENT_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if operand is SFC register */
  valueType *
isSFCRegisterBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == CONTROL_REGISTER_OPND &&
			getRegister(evaluatedParameter->addressMode) ==
			SFC_REGISTER));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if operand is the status register */
  valueType *
isStatusRegisterBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == STATUS_REGISTER_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if operand is user stack pointer */
  valueType *
isUSPBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == USP_REGISTER_OPND));
	} else {
		return(makeBooleanValue(FALSE));
	}
}

/* Check if operand is VB register */
  valueType *
isVBRegisterBIF(parameterList, kindOfFixup)
  operandListType	*parameterList;
  fixupKindType		 kindOfFixup;
{
	valueType	*evaluatedParameter;

	valueType	*evaluateOperand();

	if (parameterList != NULL) {
		evaluatedParameter = evaluateOperand(parameterList, NO_FIXUP);
		return(makeBooleanValue(operandKindField(evaluatedParameter->
			addressMode) == CONTROL_REGISTER_OPND &&
			getRegister(evaluatedParameter->addressMode) ==
			VBR_REGISTER));
	} else {
		return(makeBooleanValue(FALSE));
	}
}
