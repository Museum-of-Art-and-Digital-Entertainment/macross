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
	emitBranch_6502.c -- Routines to deal with code generation for
			branches and jumps in the Macross assembler (6502
			version).

	Chip Morningstar -- Lucasfilm Ltd.

	23-April-1985

*/

#include "macrossTypes.h"
#include "macrossGlobals.h"

/* emitRelativeBranch emits a relative branch instruction for the 6502,
   branching from the current location given a condition to branch upon and a
   target address. */

  void
emitRelativeBranch(condition, target, fixupLocation)
  conditionType	 condition;
  valueType	*target;
  valueType	 fixupLocation[COMPOUND_BRANCH_MAX];
{
	int	i;

#define COMPOUND	0x00
#define BPL_OPCODE	0x10
#define BMI_OPCODE	0x30
#define BVC_OPCODE	0x50
#define BVS_OPCODE	0x70
#define BCC_OPCODE	0x90
#define BCS_OPCODE	0xB0
#define BNE_OPCODE	0xD0
#define NOP_OPCODE	0xEA
#define BEQ_OPCODE	0xF0

	static byte	conditionalBranchOpcodes[] = {
		/* CARRY_COND */		BCS_OPCODE,
		/* ZERO_COND */			BEQ_OPCODE,
		/* NEGATIVE_COND */		BMI_OPCODE,
		/* OVERFLOW_COND */		BVS_OPCODE,
		/* LT_COND */			BCC_OPCODE,
		/* LEQ_COND */			COMPOUND,
		/* SLT_COND */			COMPOUND,
		/* SLEQ_COND */			COMPOUND,
		/* ALWAYS_COND */		NOP_OPCODE,
		/* NOT_CARRY_COND */		BCC_OPCODE,
		/* NOT_ZERO_COND */		BNE_OPCODE,
		/* NOT_NEGATIVE_COND */		BPL_OPCODE,
		/* NOT_OVERFLOW_COND */		BVC_OPCODE,
		/* GEQ_COND */			BCS_OPCODE,
		/* GT_COND */			COMPOUND,
		/* SGEQ_COND */			COMPOUND,
		/* SGT_COND */			COMPOUND,
		/* NEVER_COND */		NOP_OPCODE
	};

#define conditionalFixup(n) if (fixupLocation != NULL) \
				fixupLocation[n] = currentLocationCounter; \
			    emitRelativeByteOffset(target);

	if (fixupLocation != NULL)
		for (i=0; i<COMPOUND_BRANCH_MAX; i++)
			fixupLocation[i].value = -1;
	if (conditionalBranchOpcodes[(int)condition] != COMPOUND) {
		emitByte(conditionalBranchOpcodes[(int)condition]);
		conditionalFixup(0);
	} else switch (condition) {

	case GT_COND:
		emitByte(BEQ_OPCODE);	emitByte(2);
		emitByte(BCS_OPCODE);	conditionalFixup(0);
		break;

	case LEQ_COND:
		emitByte(BCC_OPCODE);	conditionalFixup(0);
		emitByte(BEQ_OPCODE);	conditionalFixup(1);
		break;

	case SGEQ_COND:
		emitByte(BVS_OPCODE);	emitByte(4);
		emitByte(BPL_OPCODE);	conditionalFixup(0);
		emitByte(BMI_OPCODE);	emitByte(2);
		emitByte(BMI_OPCODE);	conditionalFixup(1);
		break;

	case SGT_COND:
		emitByte(BEQ_OPCODE);	emitByte(8);
		emitByte(BVS_OPCODE);	emitByte(4);
		emitByte(BPL_OPCODE);	conditionalFixup(0);
		emitByte(BMI_OPCODE);	emitByte(2);
		emitByte(BMI_OPCODE);	conditionalFixup(1);
		break;

	case SLEQ_COND:
		emitByte(BEQ_OPCODE);	conditionalFixup(0);
		emitByte(BVS_OPCODE);	emitByte(4);
		emitByte(BMI_OPCODE);	conditionalFixup(1);
		emitByte(BPL_OPCODE);	emitByte(2);
		emitByte(BPL_OPCODE);	conditionalFixup(2);
		break;

	case SLT_COND:
		emitByte(BVS_OPCODE);	emitByte(4);
		emitByte(BMI_OPCODE);	conditionalFixup(0);
		emitByte(BPL_OPCODE);	emitByte(2);
		emitByte(BPL_OPCODE);	conditionalFixup(1);
		break;

	default:
		botch("non-compound condition leaked thru!\n");
		break;
	}
}


/* emitJump emits a 6502 jump instruction given the target address */

  simpleFixupListType *
emitJump(target, previousFixups)
  valueType		*target;
  simpleFixupListType	*previousFixups;
{
	simpleFixupListType	*result;
	valueType		 picFixup[COMPOUND_BRANCH_MAX];

	simpleFixupListType	*buildSimpleFixupList();

#define JUMP_OPCODE	0x4C

	result = previousFixups;
	if (positionIndependentCodeMode) {
		if (target == NULL) {
			emitRelativeBranch(CARRY_COND, NULL, picFixup);
			result = buildSimpleFixupList(picFixup[0], result);
			emitRelativeBranch(NOT_CARRY_COND, NULL, picFixup);
			result = buildSimpleFixupList(picFixup[0], result);
		} else {
			emitRelativeBranch(CARRY_COND, target, NULL);
			emitRelativeBranch(NOT_CARRY_COND, target, NULL);
		}
	} else {
		emitByte(JUMP_OPCODE);
		if (target == NULL) {
			result = buildSimpleFixupList(currentLocationCounter,
				result);
			emitWord(0);
		} else {
			if (target->kindOfValue != ABSOLUTE_VALUE)
				noteAnonymousReference();
			emitWordValue(target);
		}
	}
	return(result);
}

