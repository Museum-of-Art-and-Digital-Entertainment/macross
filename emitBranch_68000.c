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
	emitBranch_68000.c -- Routines to deal with code generation for
			branches and jumps in the Macross assembler (68000
			version).

	Chip Morningstar -- Lucasfilm Ltd.

	29-April-1985

*/

#include "macrossTypes.h"
#include "macrossGlobals.h"

/* emitRelativeBranch emits a relative branch instruction for the 68000,
   branching from the current location given a condition to branch upon and a
   target address. */

  void
emitRelativeBranch(condition, target, fixupLocation)
  conditionType	 condition;
  valueType	*target;
  valueType	*fixupLocation;
{
	int	i;

#define BCS_OPCODE 0x65
#define BCC_OPCODE 0x64
#define BEQ_OPCODE 0x67
#define BGE_OPCODE 0x6C
#define BGT_OPCODE 0x6E
#define BHI_OPCODE 0x62
#define BLE_OPCODE 0x6F
#define BLS_OPCODE 0x63
#define BLT_OPCODE 0x6D
#define BMI_OPCODE 0x6B
#define BNE_OPCODE 0x66
#define BPL_OPCODE 0x6A
#define BVC_OPCODE 0x68
#define BVS_OPCODE 0x69
#define BRA_OPCODE 0x60

	static byte	conditionalBranchOpcodes[] = {
		/* CARRY_COND */		BCS_OPCODE,
		/* EQUAL_COND */		BEQ_OPCODE,
		/* OVERFLOW_COND */		BVS_OPCODE,
		/* MINUS_COND */		BMI_OPCODE,
		/* LT_COND */			BLT_OPCODE,
		/* LEQ_COND */			BLE_OPCODE,
		/* LOW_OR_SAME */		BLS_OPCODE,
		/* ALWAYS_COND */		BRA_OPCODE,
		/* NOT_CARRY_COND */		BCC_OPCODE,
		/* NOT_EQUAL_COND */		BNE_OPCODE,
		/* NOT_OVERFLOW_COND */		BVC_OPCODE,
		/* PLUS_COND */			BPL_OPCODE,
		/* GEQ_COND */			BGE_OPCODE,
		/* GT_COND */			BGT_OPCODE,
		/* HIGH_COND */			BHI_OPCODE,
		/* NEVER_COND */		0,
	};

	if (fixupLocation != NULL)
		fixupLocation->value = -1;
	if (condition == NEVER_COND)
		return;
	emitByte(conditionalBranchOpcodes[(int)condition]);
	if (isByteOffset(target - (currentLocationCounter.value + 1))) {
		if (fixupLocation != NULL)
			*fixupLocation = currentLocationCounter;
		emitRelativeByteOffset(target);
	} else {
		emitByte(0);
		if (fixupLocation != NULL)
			*fixupLocation = currentLocationCounter;
		emitRelativeWordOffset(target);
	}
}


/* emitJump emits a 68000 jump instruction given the target address */

  simpleFixupListType *
emitJump(target, previousFixups)
  valueType		*target;
  simpleFixupListType	*previousFixups;
{
	simpleFixupListType	*result;

	simpleFixupListType	*buildSimpleFixupList();

#define JUMP_OPCODE_WORD	0x4EF8
#define JUMP_OPCODE_LONG	0x4EF9
#define JUMP_OPCODE_PC_REL	0x4EFA

	result = previousFixups;
	if (target == NULL) {
		emitWord(JUMP_OPCODE_LONG);
		result = buildSimpleFixupList(currentLocationCounter, result);
		emitWord(0);
	} else if (target->kindOfValue != ABSOLUTE_VALUE) {
		emitWord(JUMP_OPCODE_LONG);
		noteAnonymousReference();
		emitLongValue(target);
	} else if (isWordOffset(target->value-currentLocationCounter.value)) {
		emitWord(JUMP_OPCODE_PC_REL);
		emitWord(target->value - currentLocationCounter.value);
	} else if (isWord(target)) {
		emitWord(JUMP_OPCODE_WORD);
		emitWord(target);
	} else {
		emitWord(JUMP_OPCODE_LONG);
		emitLong(target);
	}
	return(result);
}

