/*
	actions.c -- Actions associated with various machine instruction
		     classes (68000 version).

	Chip Morningstar -- Lucasfilm Ltd.

	26-April-1985

*/

#include "macrossTypes.h"
#include "macrossGlobals.h"

#define binary		opcode->opcode
#define altBinary	opcode->subClass
#define sub		opcode->subClass

#define mode(n)		((evaluatedOperands[n])->addressMode)
#define trueMode(n)	(operandKindField(mode(n)))
#define address(n)	(evaluatedOperands[n]->value)
#define forward(n)	(!isDefined(evaluatedOperands[n]))
#define register(n)	(getRegister(mode(n)))
#define controlRegister(n) (trueMode(n)==USP_REGISTER_OPND ? \
				0x800 : (register(n) == SFC_REGISTER ? \
				0x000 : (register(n) == DFC_REGISTER ? \
				0x001 : 0x801)))
#define eaPC(n)		(effectiveAddressBits(mode(n), address(n), \
				forward(n), TRUE))
#define eaNoPC(n)	(effectiveAddressBits(mode(n), address(n), \
				forward(n), FALSE))
#define eaFlop(n)	(eaNoPC(n)>>3 | (eaNoPC(n)&7)<<3)
#define extendPC(n)	emitPossibleExtension(address(n), mode(n), binary, n,\
				forward(n), TRUE)
#define extendNoPC(n)	emitPossibleExtension(address(n), mode(n), binary, n,\
				forward(n), FALSE)
#define check(g, n)	((operandClassTable[(int)trueMode(n)] &\
				g) != 0)
#define defcheck(n)	isDefined(evaluatedOperands[n])
#define check1(g)	(numberOfOperands==1 && check(g, 0))
#define check2(g1, g2)	(numberOfOperands==2 && check(g1, 0) && check(g2, 1))
/*#define check1(g)	(numberOfOperands==1 && check(g, 0) && defcheck(0))
#define check2(g1, g2)	(numberOfOperands==2 && check(g1, 0) && check(g2, 1)\
				&& defcheck(0) && defcheck(1))*/

#define iifCheck0()		if (numberOfOperands==0) {
#define iifCheck1(g)		if (check1(g)) {
#define eifCheck1(g)		} else if (check1(g)) {
#define iifCheck2(g1, g2)	if (check2(g1, g2)) {
#define eifCheck2(g1, g2)	} else if (check2(g1, g2)) {
#define done			} else {\
				    error(INAPPROPRIATE_ADDRESS_MODES_ERROR);\
				}

#define D_TO_EA_BIT		0x0100
#define EA_TO_D_BIT		0x0000
#define R_SHIFT			9
#define Q_DATA_SHIFT		9

#define OP_SIZE_BITS		0x00C0
#define BYTE_OP_SIZE_BITS	0x0000
#define WORD_OP_SIZE_BITS	0x0040
#define LONG_OP_SIZE_BITS	0x0080

#define byteOp(op)		(((op) & OP_SIZE_BITS) == BYTE_OP_SIZE_BITS)
#define wordOp(op)		(((op) & OP_SIZE_BITS) == WORD_OP_SIZE_BITS)
#define longOp(op)		(((op) & OP_SIZE_BITS) == LONG_OP_SIZE_BITS)

#define MOVE_OP_SIZE_BITS	0x3000
#define MOVE_BYTE_OP_SIZE_BITS	0x1000
#define MOVE_WORD_OP_SIZE_BITS	0x3000
#define MOVE_LONG_OP_SIZE_BITS	0x2000

#define moveByteOp(op)		(((op) & MOVE_OP_SIZE_BITS) == \
					MOVE_BYTE_OP_SIZE_BITS)
#define moveWordOp(op)		(((op) & MOVE_OP_SIZE_BITS) == \
					MOVE_WORD_OP_SIZE_BITS)
#define moveLongOp(op)		(((op) & MOVE_OP_SIZE_BITS) == \
					MOVE_LONG_OP_SIZE_BITS)

#define isMoveOp(op)		((op)==0x1000 || (op)==0x2000 || (op)==0x3000)
#define byteOpGeneral(op)	(isMoveOp(op) ? moveByteOp(op) : byteOp(op))
#define wordOpGeneral(op)	(isMoveOp(op) ? moveWordOp(op) : wordOp(op))

void	emitPossibleExtension();

#define emitImmediate(n) \
	if (byteOp(binary)) {\
		if (byteCheck(address(n))) {\
			emitByte(0);\
			putFixupsHere(BYTE_FIXUP, n);\
			emitByte(address(n));\
		}\
	} else if (wordOp(binary)) {\
		if (wordCheck(address(n))) {\
			putFixupsHere(WORD_FIXUP, n);\
			emitWord(address(n));\
		}\
	} else {\
		putFixupsHere(LONG_FIXUP, n);\
		emitLong(address(n));\
	}

/*
   These routines are vectored off of the opcode lookup table.  Each
   instruction is of a particular category that defines which address modes
   it accepts its operands in and what size the operands are.  There is one
   "actionsXXXX" routine for each of these categories that grabs the operands,
   checks their address modes, and emits the binary instruction and operands.
 */

/* Class I: abcd, addxb, addxw, addxl, sbcd, subxb, subxw, subxl */
  void
actionsClassI(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
#define D_REGISTER_BIT_I	0x0000
#define PREDECREMENT_BIT_I	0x0008

iifCheck2(GROUP_A, GROUP_A)
	emitWord(binary | register(1)<<R_SHIFT | register(0) |
		D_REGISTER_BIT_I);
eifCheck2(GROUP_D, GROUP_D)
	emitWord(binary | register(1)<<R_SHIFT | register(0) |
		PREDECREMENT_BIT_I);
done
}

/* Class II: addb, addw, addl, subb, subw, subl (implicitly includes addib,
	addiw, addil, addaw, addal, subib, subiw, subil, subab, subaw, subal)
*/
  void
actionsClassII(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
#define WORD_ADDR_BITS_II	0x00C0
#define LONG_ADDR_BITS_II	0x01C0

iifCheck2(GROUP_A, GROUP_S)
	emitWord(binary | register(0)<<R_SHIFT | D_TO_EA_BIT | eaNoPC(1));
	extendNoPC(1);
eifCheck2(GROUP_F, GROUP_P)
	emitWord(altBinary | eaNoPC(1));
	emitImmediate(0);
	extendNoPC(1);
eifCheck2(GROUP_O, GROUP_A)
	if (!byteOp(binary) || trueMode(0)!=A_REGISTER_OPND) {
		emitWord(binary | register(1)<<R_SHIFT | EA_TO_D_BIT |
			eaPC(0));
		extendPC(0);
	} else {
		error(CANT_DO_OP_FROM_A_REGISTER_ERROR);
	}
eifCheck2(GROUP_O, GROUP_B)
	if (wordOp(binary)) {
		emitWord(binary | register(1)<<R_SHIFT | WORD_ADDR_BITS_II |
			eaPC(0));
		extendPC(0);
	} else if (longOp(binary)) {
		emitWord(binary | register(1)<<R_SHIFT | LONG_ADDR_BITS_II |
			eaPC(0));
		extendPC(0);
	} else {
		error(CANT_DO_BYTE_ADDRESS_OP_ERROR);
	}
done
}

/* Class III: addqb, addqw, addql, subqb, subqw, subql */
  void
actionsClassIII(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
iifCheck2(GROUP_F, GROUP_N)

	if (forward(0)) {
		error(FORWARD_REFERENCE_NOT_ALLOWED_HERE_ERROR);
	} else if (0<address(0) && address(0)<=8) {
		emitWord(binary | (address(0)%8)<<Q_DATA_SHIFT | eaNoPC(1));
		extendNoPC(1);
	} else {
		error(QUICK_DATA_OUT_OF_RANGE_ERROR, address(0));
	}
done
}

/* Class IV: andb, andw, andl, orb, orw, orl (implicitly includes andib,
	andiw, andil, orib, oriw, oril, andiccr, oriccr, andisr, orisr) */
  void
actionsClassIV(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
#define	CCR_BITS_IV	0x003C
#define SR_BITS_IV	0x007C

iifCheck2(GROUP_A, GROUP_S)
	emitWord(binary | register(0)<<R_SHIFT | D_TO_EA_BIT | eaNoPC(1));
	extendNoPC(1);
eifCheck2(GROUP_F, GROUP_H)
	if (byteOp(binary)) {
		if (byteCheck(address(0))) {
			emitWord(altBinary | CCR_BITS_IV);
			emitByte(0);
			putFixupsHere(BYTE_FIXUP, 0);
			emitByte(address(0));
		}
	} else {
		error(CCR_OP_IS_BYTE_OP_ERROR);
	}
eifCheck2(GROUP_F, GROUP_I)
	if (wordOp(binary)) {
		if (wordCheck(address(0))) {
			emitWord(altBinary | SR_BITS_IV);
			putFixupsHere(WORD_FIXUP, 0);
			emitWord(address(0));
		}
	} else {
		error(SR_OP_IS_WORD_OP_ERROR);
	}
eifCheck2(GROUP_F, GROUP_P)
	emitWord(altBinary | eaNoPC(1));
	emitImmediate(0);
	extendNoPC(1);
eifCheck2(GROUP_Q, GROUP_A)
	emitWord(binary | register(1)<<R_SHIFT | EA_TO_D_BIT | eaPC(0));
	extendPC(0);
done
}

/* Class V: aslb, aslw, asll, asrb, asrw, asrl, lslb, lslw, lsll, lsrb, lsrw,
	lsrl, rolb, rolw, roll, rorb, rorw, rorl, roxlb, roxlw, roxll, roxrb,
	roxrw, roxrl */
  void
actionsClassV(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
#define REGISTER_SHIFT_BIT_V	0x0020
#define MEMORY_SHIFT_BITS_V	0x00C0
#define LOGICAL_REGISTER_BIT_V	0x0008
#define LOGICAL_MEMORY_BIT_V	0x0200
#define ROTATE_REGISTER_BITS_V	0x0018
#define ROTATE_MEMORY_BITS_V	0x0600
#define ROTATEX_REGISTER_BITS_V 0x0010
#define ROTATEX_MEMORY_BITS_V	0x0400

iifCheck2(GROUP_A, GROUP_A)
	emitWord(binary | register(0)<<R_SHIFT | REGISTER_SHIFT_BIT_V |
		register(1) | (sub==1 ? LOGICAL_REGISTER_BIT_V : 0) |
		(sub==2 ? ROTATE_REGISTER_BITS_V : 0) |
		(sub==3 ? ROTATEX_REGISTER_BITS_V : 0));
eifCheck2(GROUP_F, GROUP_A)
	if (forward(0))
		error(FORWARD_REFERENCE_NOT_ALLOWED_HERE_ERROR);
	else if (0<address(0) && address(0)<=8)
		emitWord(binary | (address(0)%8)<<Q_DATA_SHIFT | register(1) |
			(sub==1 ? LOGICAL_REGISTER_BIT_V : 0) |
			(sub==2 ? ROTATE_REGISTER_BITS_V : 0) |
			(sub==3 ? ROTATEX_REGISTER_BITS_V : 0));
	else
		error(QUICK_DATA_OUT_OF_RANGE_ERROR, address(0));
eifCheck1(GROUP_S)
	if (wordOp(binary)) {
		emitWord(binary | MEMORY_SHIFT_BITS_V | eaNoPC(0) |
			(sub==1 ? LOGICAL_MEMORY_BIT_V : 0) |
			(sub==2 ? ROTATE_MEMORY_BITS_V : 0) |
			(sub==3 ? ROTATEX_MEMORY_BITS_V : 0));
		extendNoPC(0);
	} else {
		error(SHIFT_OP_IS_WORD_OP_ERROR);
	}
done
}

/* Class VI: bCC, bra, bsr */
  void
actionsClassVI(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
	int	offset;

iifCheck1(GROUP_G)
	if (forward(0))
		offset = 0;
	else
		offset = address(0) - (currentLocationCounter.value -
			targetOffset + 2);
	if (isByteOffset(offset) && offset!=0) {
		emitByte(binary);
		emitByte(offset);
	} else if (wordOffsetCheck(offset)) {
		emitByte(binary);
		emitByte(0);
		putFixupsHere(WORD_RELATIVE_FIXUP, 0);
		emitWord(offset);
	}
done
}

/* Class VII: bchg, bclr, bset, btst */
  void
actionsClassVII(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
#define REGISTER_BIT_VII	0x0100
#define IMMEDIATE_BIT_VII	0x0800

iifCheck2(GROUP_A, GROUP_P)
	emitWord(binary | register(0)<<R_SHIFT | eaNoPC(1) |
		REGISTER_BIT_VII);
	extendNoPC(1);
eifCheck2(GROUP_F, GROUP_P)
	if (byteCheck(address(0))) {
		emitWord(binary | eaNoPC(1) | IMMEDIATE_BIT_VII);
		emitByte(0);
		putFixupsHere(BYTE_FIXUP, 0);
		emitByte(address(0));
		extendNoPC(1);
	}
done
}

/* Class VIII: chk, divs, divu, muls, mulu */
  void
actionsClassVIII(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
iifCheck2(GROUP_Q, GROUP_A)
	emitWord(binary | register(1)<<R_SHIFT | eaPC(0));
	extendPC(0);
done
}

/* Class IX: clrb, clrw, clrl, nbcd, negxb, negxw, negxl, notb, notw, notl,
	sCC, tas, tstb, tstw, tstl */
  void
actionsClassIX(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
iifCheck1(GROUP_P)
	emitWord(binary | eaNoPC(0));
	extendNoPC(0);
done
}

/* Class X: cmpb, cmpw, cmpl */
  void
actionsClassX(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
#define IMMEDIATE_BITS_X	0x0C00
#define REGISTER_BITS_X		0xB000

iifCheck2(GROUP_F, GROUP_P)
	emitWord(binary | IMMEDIATE_BITS_X | eaNoPC(1));
	emitImmediate(0);
	extendNoPC(1);
eifCheck2(GROUP_O, GROUP_A)
	emitWord(binary | register(1)<<R_SHIFT | REGISTER_BITS_X | eaPC(0));
	extendPC(0);
eifCheck2(GROUP_O, GROUP_B)
	if (!byteOp(binary)) {
		emitWord(altBinary | register(1)<<R_SHIFT | eaPC(0));
		extendPC(0);
	} else {
		error(CMP_ADDRESS_NOT_BYTE_OP_ERROR);
	}
done
}

/* Class XI: cmpmb, cmpmw, cmpml */
  void
actionsClassXI(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
iifCheck2(GROUP_C, GROUP_C)
	emitWord(binary | register(1)<<R_SHIFT | register(0));
done
}

/* Class XII: dbCC */
  void
actionsClassXII(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
	int	offset;

iifCheck2(GROUP_A, GROUP_G)
	if (forward(1))
		offset = 0;
	else
		offset = address(1) - (currentLocationCounter.value -
			targetOffset + 2);
	emitWord(binary | register(0));
	if (wordOffsetCheck(offset)) {
		putFixupsHere(WORD_RELATIVE_FIXUP, 1);
		emitWord(offset);
	}
done
}

/* Class XIII: eorb, eorw, eorl */
  void
actionsClassXIII(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
#define CCR_OP_XIII	0x0A3C
#define SR_OP_XIII	0x0A7C

iifCheck2(GROUP_A, GROUP_P)
	emitWord(binary | register(0)<<R_SHIFT | eaNoPC(1));
	extendNoPC(1);
eifCheck2(GROUP_F, GROUP_H)
	if (byteCheck(address(0))) {
		emitWord(CCR_OP_XIII);
		emitByte(0);
		putFixupsHere(BYTE_FIXUP, 0);
		emitByte(address(0));
	}
eifCheck2(GROUP_F, GROUP_I)
	if (wordCheck(address(0))) {
		emitWord(SR_OP_XIII);
		putFixupsHere(WORD_FIXUP, 0);
		emitWord(address(0));
	}
eifCheck2(GROUP_F, GROUP_P)
	emitWord(altBinary | eaNoPC(1));
	emitImmediate(0);
	extendNoPC(1);
done
}

/* Class XIV: exg */
  void
actionsClassXIV(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
#define DATA_DATA_EXG_BITS_XIV	0x0040
#define DATA_ADDR_EXG_BITS_XIV	0x0048
#define ADDR_ADDR_EXG_BITS_XIV	0x0088

iifCheck2(GROUP_M, GROUP_M)
	if (trueMode(0)==D_REGISTER_OPND && trueMode(1)==
				D_REGISTER_OPND) { /* data<-->data */
		emitWord(binary | register(0)<<R_SHIFT | register(1) |
			DATA_DATA_EXG_BITS_XIV);
	} else if (trueMode(0)==D_REGISTER_OPND) { /* data<-->addr */
		emitWord(binary | register(0)<<R_SHIFT | register(1) |
			DATA_ADDR_EXG_BITS_XIV);
	} else if (trueMode(1)==D_REGISTER_OPND) { /* addr<-->data */
		emitWord(binary | register(1)<<R_SHIFT | register(0) |
			DATA_ADDR_EXG_BITS_XIV);
	} else { /* addr<-->addr */
		emitWord(binary | register(0)<<R_SHIFT | register(1) |
			ADDR_ADDR_EXG_BITS_XIV);
	}
done
}

/* Class XV: extw, extl, swap */
  void
actionsClassXV(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
iifCheck1(GROUP_A)
	emitWord(binary | register(0));
done
}

/* Class XVI: illegal, nop, reset, rte, rtr, rts, trapv */
  void
actionsClassXVI(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
iifCheck0()
	emitWord(binary);
done
}

/* Class XVII: jmp, jsr, pea */
  void
actionsClassXVII(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
iifCheck1(GROUP_U)
	emitWord(binary | eaPC(0));
	extendPC(0);
done
}

/* Class XVIII: lea */
  void
actionsClassXVIII(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
iifCheck2(GROUP_U, GROUP_B)
	emitWord(binary | register(1)<<R_SHIFT | eaPC(0));
	extendPC(0);
done
}

/* Class XIX: link */
  void
actionsClassXIX(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
iifCheck2(GROUP_B, GROUP_F)
	if (wordOffsetCheck(address(1))) {
		emitWord(binary | register(0));
		putFixupsHere(WORD_FIXUP, 1);
		emitWord(address(1));
	}
done
}

/* Class XX: movb, movw, movl */
  void
actionsClassXX(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
#define MOVE_TO_USP_OP		0x4E60
#define MOVE_FROM_USP_OP	0x4E68
#define MOVE_TO_CCR_OP		0x44C0
#define MOVE_FROM_CCR_OP	0x42C0
#define MOVE_TO_SR_OP		0x46C0
#define MOVE_FROM_SR_OP		0x40C0
#define MOVE_TO_CR_OP		0x4E7B
#define MOVE_FROM_CR_OP		0x4E7A

#define R_SHIFT_XX		12
#define EA_SHIFT_XX		 6
#define A_REGISTER_BIT_XX	0x8000

iifCheck2(GROUP_B, GROUP_K)
	if (moveLongOp(binary)) {
		emitWord(MOVE_TO_USP_OP | register(0));
	} else {
		error(USP_MOVE_IS_LONG_OP_ERROR);
	}
eifCheck2(GROUP_H, GROUP_P)
	if (moveWordOp(binary)) {
		emitWord(MOVE_FROM_CCR_OP | eaNoPC(1));
		extendNoPC(1);
	} else {
		error(CCR_MOVE_IS_WORD_OP_ERROR);
	}
eifCheck2(GROUP_I, GROUP_P)
	if (moveWordOp(binary)) {
		emitWord(MOVE_FROM_SR_OP | eaNoPC(1));
		extendNoPC(1);
	} else {
		error(SR_MOVE_IS_WORD_OP_ERROR);
	}
eifCheck2(GROUP_K, GROUP_B)
	if (moveLongOp(binary)) {
		emitWord(MOVE_FROM_USP_OP | register(1));
	} else {
		error(USP_MOVE_IS_LONG_OP_ERROR);
	}
eifCheck2(GROUP_L, GROUP_M)
	if (moveLongOp(binary)) {
		emitWord(MOVE_FROM_CR_OP);
		emitWord((trueMode(1)==A_REGISTER_OPND ? A_REGISTER_BIT_XX:0)
			| register(1)<<R_SHIFT_XX | controlRegister(0));
	} else {
		error(CR_MOVE_IS_LONG_OP_ERROR);
	}
eifCheck2(GROUP_M, GROUP_L)
	if (moveLongOp(binary)) {
		emitWord(MOVE_TO_CR_OP);
		emitWord((trueMode(0)==A_REGISTER_OPND ? A_REGISTER_BIT_XX:0)
			| register(0)<<R_SHIFT_XX | controlRegister(1));
	} else {
		error(CR_MOVE_IS_LONG_OP_ERROR);
	}
eifCheck2(GROUP_O, GROUP_B)
	if (!moveByteOp(binary)) {
		emitWord(altBinary | register(1)<<R_SHIFT | eaPC(0));
		extendPC(0);
	} else {
		error(ADDRESS_MOVE_NOT_BYTE_OP_ERROR);
	}
eifCheck2(GROUP_O, GROUP_P)
	if (!moveByteOp(binary) || trueMode(0)!=A_REGISTER_OPND) {
		emitWord(binary | eaFlop(1)<<EA_SHIFT_XX | eaPC(0));
		extendPC(0);
		extendNoPC(1);
	} else {
		error(ADDRESS_MOVE_NOT_BYTE_OP_ERROR);
	}
eifCheck2(GROUP_Q, GROUP_H)
	if (moveWordOp(binary)) {
		emitWord(MOVE_TO_CCR_OP | eaPC(0));
		extendPC(0);
	} else {
		error(CCR_MOVE_IS_WORD_OP_ERROR);
	}
eifCheck2(GROUP_Q, GROUP_I)
	if (moveWordOp(binary)) {
		emitWord(MOVE_TO_SR_OP | eaPC(0));
		extendPC(0);
	} else {
		error(SR_MOVE_IS_WORD_OP_ERROR);
	}
done
}

/* Class XXI: movemw, moveml */
  void
actionsClassXXI(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
	int	mask;
	int	i;

#define FROM_MEMORY_BIT_XXI	0x0400
#define TO_MEMORY_BIT_XXI	0x0000

if (check(GROUP_R, 0)) {
	mask = 0;
	for (i=1; i<numberOfOperands; i++) {
		if (check(GROUP_M, i)) {
			mask = mask | (1 << (trueMode(i)==A_REGISTER_OPND ?
				register(i)+8 : register(i)));
		} else {
			error(INAPPROPRIATE_ADDRESS_MODES_ERROR);
			return;
		}
	}
	emitWord(binary | FROM_MEMORY_BIT_XXI | eaPC(0));
	emitWord(mask);
	extendPC(0);
} else if (check(GROUP_T, numberOfOperands - 1)) {
	mask = 0;
	if (trueMode(numberOfOperands - 1) == PREDECREMENT_OPND) {
		for (i=0; i<numberOfOperands-1; i++) {
			if (check(GROUP_M, i)) {
				mask = mask | (1 << (trueMode(i)==
					A_REGISTER_OPND ? 7-register(i) :
					15-register(i)));
			} else {
				error(INAPPROPRIATE_ADDRESS_MODES_ERROR);
				return;
			}
		}
	} else {
		for (i=0; i<numberOfOperands-1; i++) {
			if (check(GROUP_M, i)) {
				mask = mask | (1 << (trueMode(i)==
					A_REGISTER_OPND ? register(i)+8 :
					register(i)));
			} else {
				error(INAPPROPRIATE_ADDRESS_MODES_ERROR);
				return;
			}
		}
	}
	emitWord(binary | TO_MEMORY_BIT_XXI | eaNoPC(numberOfOperands - 1));
	emitWord(mask);
	extendNoPC(numberOfOperands - 1);
} else {
	error(INAPPROPRIATE_ADDRESS_MODES_ERROR);
}
}

/* Class XXII: movepw, movepl */
  void
actionsClassXXII(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
#define TO_MEMORY_BIT_XXII	0x0080
#define FROM_MEMORY_BIT_XXII	0x0000

iifCheck2(GROUP_A, GROUP_E)
	if (wordOffsetCheck(address(1))) {
		emitWord(binary | register(0)<<R_SHIFT | register(1) |
			TO_MEMORY_BIT_XXII);
		putFixupsHere(WORD_FIXUP, 1);
		emitWord(address(1));
	}
eifCheck2(GROUP_E, GROUP_A)
	if (wordOffsetCheck(address(0))) {
		emitWord(binary | register(1)<<R_SHIFT | register(0) |
			FROM_MEMORY_BIT_XXII);
		putFixupsHere(WORD_FIXUP, 0);
		emitWord(address(0));
	}
done
}

/* Class XXIII: moveq */
  void
actionsClassXXIII(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
#define R_SHIFT_XXIII	1

iifCheck2(GROUP_F, GROUP_A)
	if (byteCheck(address(0))) {
		emitByte(binary | register(1)<<R_SHIFT_XXIII);
		putFixupsHere(BYTE_FIXUP, 0);
		emitByte(address(0));
	}
done
}

/* Class XXIV: movsb, movsw, movsl */
  void
actionsClassXXIV(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
#define R_SHIFT_XXIV		12
#define TO_MEMORY_BIT_XXIV	0x0800
#define FROM_MEMORY_BIT_XXIV	0x0000
#define A_REGISTER_BIT_XXIV	0x8000

iifCheck2(GROUP_M, GROUP_S)
	emitWord(binary | eaNoPC(1));
	emitWord((trueMode(0)==A_REGISTER_OPND ? A_REGISTER_BIT_XXIV : 0) |
		register(0)<<R_SHIFT_XXIV | TO_MEMORY_BIT_XXIV);
	extendNoPC(1);
eifCheck2(GROUP_S, GROUP_M)
	emitWord(binary | eaNoPC(0));
	emitWord((trueMode(1)==A_REGISTER_OPND ? A_REGISTER_BIT_XXIV : 0) |
		register(1)<<R_SHIFT_XXIV | FROM_MEMORY_BIT_XXIV);
	extendNoPC(0);
done
}

/* Class XXV: rtd */
  void
actionsClassXXV(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
iifCheck1(GROUP_F)
	if (wordOffsetCheck(address(0))) {
		emitWord(binary);
		putFixupsHere(WORD_FIXUP, 0);
		emitWord(address(0));
	}
done
}

/* Class XXVI: unlk */
  void
actionsClassXXVI(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
iifCheck1(GROUP_B)
	emitWord(binary | register(0));
done
}

/* Class XXVII: stop */
  void
actionsClassXXVII(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
iifCheck1(GROUP_F)
	if (wordCheck(address(0))) {
		emitWord(binary);
		putFixupsHere(WORD_FIXUP, 0);
		emitWord(address(0));
	}
done
}

/* Class XXVIII: trap */
  void
actionsClassXXVIII(opcode, numberOfOperands, evaluatedOperands)
  opcodeTableEntryType	*opcode;
  int			 numberOfOperands;
  valueType		*evaluatedOperands[];
{
iifCheck1(GROUP_F)
	if (forward(0)) {
		error(FORWARD_REFERENCE_NOT_ALLOWED_HERE_ERROR);
	} else if (0<=address(0) && address(0)<=0xF) {
		emitWord(binary | address(0));
	} else {
		error(TRAP_DATA_TOO_LARGE_ERROR, address(0));
	}
done
}


/* 
   Miscellaneous helper predicates.
 */

  bool
isByte(value)
  int	value;
{
	return (-129<value && value<256);
}

  bool
isByteOffset(value)
  int	value;
{
	return (-129<value && value<128);
}

  bool
isByteAddress(value)
  valueType	*value;
{
	return(value->kindOfValue==ABSOLUTE_VALUE && isByte(value->value));
}

  bool
isWord(value)
  int	value;
{
	return (-32769<value && value<65536);
}

  bool
isWordOffset(value)
  int	value;
{
	return (-32769<value && value<32768);
}

  bool
byteCheck(value)
  int	value;
{
	if (isByte(value)) {
		return(TRUE);
	} else {
		error(BYTE_VALUE_TOO_LARGE_ERROR, value);
		return(FALSE);
	}
}

  bool
wordCheck(value)
  int	value;
{
	if (isWord(value)) {
		return(TRUE);
	} else {
		error(WORD_VALUE_TOO_LARGE_ERROR, value);
		return(FALSE);
	}
}

  bool
wordOffsetCheck(value)
  int	value;
{
	if (isWordOffset(value)) {
		return(TRUE);
	} else {
		printf("word: %d\n", value);
		error(RELATIVE_OFFSET_VALUE_TOO_LARGE_ERROR, value);
		return(FALSE);
	}
}

  bool
isDefined(value)
  valueType	*value;
{
	return(value!=NULL && value->kindOfValue!=UNDEFINED_VALUE);
}

#define	D_REGISTER_EA		0x0000
#define A_REGISTER_EA		0x0008
#define A_REGISTER_INDIRECT_EA	0x0010
#define POSTINCREMENT_EA	0x0018
#define PREDECREMENT_EA		0x0020
#define DISPLACEMENT_EA		0x0028
#define SELECTED_EA		0x0028
#define INDEXED_EA		0x0030
#define INDEX_SELECTED_EA	0x0030
#define PC_DISPLACEMENT_EA	0x003A
#define PC_INDEXED_EA		0x003B
#define IMMEDIATE_EA		0x003C
#define ABSOLUTE_SHORT_EA	0x0038
#define ABSOLUTE_LONG_EA	0x0039

  int
effectiveAddressBits(operandKind, value, isForwardRef, pcRelativeOK)
  operandKindType	operandKind;
  int			value;
  bool			isForwardRef;
  bool			pcRelativeOK;
{
	switch(operandKindField(operandKind)) {
	case D_REGISTER_OPND:
		return(D_REGISTER_EA | getRegister(operandKind));
		break;
	case A_REGISTER_OPND:
		return(A_REGISTER_EA | getRegister(operandKind));
		break;
	case A_REGISTER_INDIRECT_OPND:
		return(A_REGISTER_INDIRECT_EA | getRegister(operandKind));
		break;
	case POSTINCREMENT_OPND:
		return(POSTINCREMENT_EA | getRegister(operandKind));
		break;
	case PREDECREMENT_OPND:
		return(PREDECREMENT_EA | getRegister(operandKind));
		break;
	case DISPLACEMENT_OPND:
		return(DISPLACEMENT_EA | getRegister(operandKind));
		break;
	case SELECTED_OPND:
		return(SELECTED_EA | getRegister(operandKind));
		break;
	case INDEXED_OPND:
		return(INDEXED_EA | getRegister(operandKind));
		break;
	case INDEX_SELECTED_OPND:
		return(INDEX_SELECTED_EA | getRegister(operandKind));
		break;
	case PC_DISPLACEMENT_OPND:
		return(PC_DISPLACEMENT_EA);
		break;
	case PC_INDEXED_OPND:
		return(PC_INDEXED_EA);
		break;
	case IMMEDIATE_OPND:
		return(IMMEDIATE_EA);
		break;
	case ABSOLUTE_SHORT_OPND:
		return(ABSOLUTE_SHORT_EA);
		break;
	case ABSOLUTE_LONG_OPND:
		return(ABSOLUTE_LONG_EA);
		break;
	case EXPRESSION_OPND:
		if (pcRelativeOK && isWordOffset(value -
				(currentLocationCounter.value - targetOffset
				+ 2)) && !isForwardRef)
			return(PC_DISPLACEMENT_EA);
		else if (isWord(value) && !isForwardRef)
			return(ABSOLUTE_SHORT_EA);
		else
			return(ABSOLUTE_LONG_EA);
		break;
	case STRING_OPND:
		botch("string opnd to get ea\n");
		break;
	case BLOCK_OPND:
		botch("block opnd to get ea\n");
		break;
	case CC_REGISTER_OPND:
	case STATUS_REGISTER_OPND:
	case USP_REGISTER_OPND:
	case CONTROL_REGISTER_OPND:
		botch("funny opnd to get ea\n");
		break;
	default:
		botch("non-existant operand kind to get ea: %x\n",
			operandKind);
		break;
	}
}

  void
emitPossibleExtension(value, addressMode, opcode, operandNumber, isForwardRef,
	pcRelativeOK)
  int			value;
  operandKindType	addressMode;
  int			opcode;
  int			operandNumber;
  bool			isForwardRef;
  bool			pcRelativeOK;
{
	switch (operandKindField(addressMode)) {

	case DISPLACEMENT_OPND:
	case SELECTED_OPND:
	case ABSOLUTE_SHORT_OPND:
	case PC_DISPLACEMENT_OPND:
		putFixupsHere(WORD_FIXUP, operandNumber);
		wordCheck(value);
		emitWord(value);
		break;
	case INDEXED_OPND:
	case INDEX_SELECTED_OPND:
	case PC_INDEXED_OPND:
		emitByte(indexByte(addressMode));
		putFixupsHere(BYTE_FIXUP,operandNumber);
		byteCheck(value);
		emitByte(value);
		break;
	case ABSOLUTE_LONG_OPND:
		putFixupsHere(LONG_FIXUP, operandNumber);
		emitLong(value);
		break;
	case IMMEDIATE_OPND:
		if (byteOpGeneral(opcode)) {
			emitByte(0);
			putFixupsHere(BYTE_FIXUP, operandNumber);
			byteCheck(value);
			emitByte(value);
		} else if (wordOpGeneral(opcode)) {
			putFixupsHere(WORD_FIXUP, operandNumber);
			wordCheck(value);
			emitWord(value);
		} else {
			putFixupsHere(LONG_FIXUP, operandNumber);
			emitLong(value);
		}
		break;
	case EXPRESSION_OPND:
		if (pcRelativeOK && isWordOffset(value -
				(currentLocationCounter.value -
				targetOffset)) && !isForwardRef) {
			putFixupsHere(WORD_RELATIVE_FIXUP,
				operandNumber);
			emitWord(value - (currentLocationCounter.value -
				targetOffset));
		} else if (isWord(value) && !isForwardRef) {
			putFixupsHere(WORD_FIXUP, operandNumber);
			emitWord(value);
		} else {
			putFixupsHere(LONG_FIXUP, operandNumber);
			emitLong(value);
		}
		break;
	default:
		break;
	}
}

  int
indexByte(operandKind)
  operandKindType	operandKind;
{
	return((getIndexRegister(operandKind)<<4) | (getWL(operandKind)<<3));
}
