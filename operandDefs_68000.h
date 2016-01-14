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
	operandDefs_68000.h -- Define operand types and flag bits for 68000.

	Chip Morningstar -- Lucasfilm Ltd.

	25-April-1985
*/

typedef enum {
		EXPRESSION_OPND, STRING_OPND, BLOCK_OPND, D_REGISTER_OPND,
		A_REGISTER_OPND, A_REGISTER_INDIRECT_OPND, POSTINCREMENT_OPND,
		PREDECREMENT_OPND, DISPLACEMENT_OPND, INDEXED_OPND,
		PC_DISPLACEMENT_OPND, PC_INDEXED_OPND, IMMEDIATE_OPND,
		ABSOLUTE_SHORT_OPND, ABSOLUTE_LONG_OPND, CC_REGISTER_OPND,
		STATUS_REGISTER_OPND, USP_REGISTER_OPND,
		CONTROL_REGISTER_OPND, SELECTED_OPND, INDEX_SELECTED_OPND,
			       } operandKindType;

/*** Hack warning ***/

/* We know that enumerated types are actually just ints.  Some of the address
   modes require some additional information in the form of one or two
   register numbers.  We want to still encode this information in one int.
   Therefor, we are going to OR some bits representing the register into the
   higher-order bits of the type word, thus violating the principle of
   enumerated types (it's just that the 'enum' syntax above is so much cleaner
   than a bunch of '#define's) */

#define getRegisterField(op, n)	((((int)op)>>(8+(n-1)*4)) & 0xF)
#define setRegisterField(op, n, r) (op=((int)op | \
					    (((r)&0xF)<<(8+((n)-1)*4))))

#define getAddressRegister(op)	(getRegisterField(op, 1) - 8)
#define getDataRegister(op)	getRegisterField(op, 1)
#define getRegister(op)		getRegisterField(op, 1)
#define getIndexRegister(op)	getRegisterField(op, 2)
#define getWL(op)		(((int)op >> 16) & 1)

#define setAddressRegister(op, r)	setRegisterField(op, 1, (int)r + 8)
#define setDataRegister(op, r)		setRegisterField(op, 1, (int)r)
#define setRegister(op, r)		setRegisterField(op, 1, (int)r)
#define setIndexRegister(op, r)		setRegisterField(op, 2, (int)r)
#define setWL(op, wl)	(op = ((int)op | (((wl) & 1) << 16)))

#define operandKindField(op)	(operandKindType)((int)op & 0xFF)

/* Opcodes: */
/* In the opcode table we want to have information that tells which possible
   address modes the corresponding instruction can utilize.  The
   instruction set of the 68000 breaks up into classes of instructions, such
   that all of the instructions in a class accept the same address modes.
   We encode the instructions by class and the classes by permissible address
   modes.  This helps us reduce errors in construction of the opcode table,
   since getting one instruction of a given class right means getting all the
   others right too.  Ordinarily we'd give each class a nice mnemonic name,
   but the 68000 has 26 idiosyncratic classes and a unique name for each
   would confuse more than it would help. */

typedef enum {
		CLASS_I, CLASS_II, CLASS_III, CLASS_IV, CLASS_V, CLASS_VI,
		CLASS_VII, CLASS_VIII, CLASS_IX, CLASS_X, CLASS_XI, CLASS_XII,
		CLASS_XIII, CLASS_XIV, CLASS_XV, CLASS_XVI, CLASS_XVII,
		CLASS_XVIII, CLASS_XIX, CLASS_XX, CLASS_XXI, CLASS_XXII,
		CLASS_XXIII, CLASS_XXIV, CLASS_XXV, CLASS_XXVI, CLASS_XXVII,
		CLASS_XXVIII,
			       } addressClassType;

/* The various address modes as required by the various instructions manage to
   sort themselves into a number of independent groups.  We will give these
   groups names to facilitate operand address mode checking */

#define NO_OPND_BIT			0x000000
#define EXPRESSION_OPND_BIT		0x000001
#define STRING_OPND_BIT			0x000002
#define BLOCK_OPND_BIT			0x000004
#define D_REGISTER_OPND_BIT		0x000008
#define A_REGISTER_OPND_BIT		0x000010
#define A_REGISTER_INDIRECT_OPND_BIT	0x000020
#define POSTINCREMENT_OPND_BIT		0x000040
#define PREDECREMENT_OPND_BIT		0x000080
#define DISPLACEMENT_OPND_BIT		0x000100
#define INDEXED_OPND_BIT		0x000200
#define PC_DISPLACEMENT_OPND_BIT	0x000400
#define PC_INDEXED_OPND_BIT		0x000800
#define IMMEDIATE_OPND_BIT		0x001000
#define ABSOLUTE_SHORT_OPND_BIT		0x002000
#define ABSOLUTE_LONG_OPND_BIT		0x004000
#define CC_REGISTER_OPND_BIT		0x008000
#define STATUS_REGISTER_OPND_BIT	0x010000
#define USP_REGISTER_OPND_BIT		0x020000
#define CONTROL_REGISTER_OPND_BIT	0x040000
#define SELECTED_OPND_BIT		0x080000
#define INDEX_SELECTED_OPND_BIT		0x100000
#define ANY_OPND_BITS			0xFFFFFF

#define D_REGISTER_GROUP_BITS		D_REGISTER_OPND_BIT
#define GROUP_A		D_REGISTER_GROUP_BITS

#define A_REGISTER_GROUP_BITS		A_REGISTER_OPND_BIT
#define GROUP_B		A_REGISTER_GROUP_BITS

#define POSTINCREMENT_GROUP_BITS	POSTINCREMENT_OPND_BIT
#define GROUP_C		POSTINCREMENT_GROUP_BITS

#define PREDECREMENT_GROUP_BITS		PREDECREMENT_OPND_BIT
#define GROUP_D		PREDECREMENT_GROUP_BITS

#define DISPLACEMENT_GROUP_BITS		(DISPLACEMENT_OPND_BIT | \
					SELECTED_OPND_BIT)
#define GROUP_E		DISPLACEMENT_GROUP_BITS

#define IMMEDIATE_GROUP_BITS		IMMEDIATE_OPND_BIT
#define GROUP_F		IMMEDIATE_GROUP_BITS

#define NORMAL_GROUP_BITS		EXPRESSION_OPND_BIT
#define GROUP_G		NORMAL_GROUP_BITS

#define CC_REGISTER_GROUP_BITS		CC_REGISTER_OPND_BIT
#define GROUP_H		CC_REGISTER_GROUP_BITS

#define STATUS_REGISTER_GROUP_BITS	STATUS_REGISTER_OPND_BIT
#define GROUP_I		STATUS_REGISTER_GROUP_BITS

#define IMPLIED_GROUP_BITS		NO_OPND_BIT
#define GROUP_J		IMPLIED_GROUP_BITS

#define USP_REGISTER_GROUP_BITS		USP_REGISTER_OPND_BIT
#define GROUP_K		USP_REGISTER_GROUP_BITS

#define CONTROL_REGISTER_GROUP_BITS	(USP_REGISTER_OPND_BIT | \
					CONTROL_REGISTER_OPND_BIT)
#define GROUP_L		CONTROL_REGISTER_GROUP_BITS

#define REGISTER_GROUP_BITS		(D_REGISTER_OPND_BIT | \
					A_REGISTER_OPND_BIT)
#define GROUP_M		REGISTER_GROUP_BITS

#define STORABLE_GROUP_BITS		(D_REGISTER_OPND_BIT | \
					A_REGISTER_OPND_BIT | \
					A_REGISTER_INDIRECT_OPND_BIT | \
					POSTINCREMENT_OPND_BIT | \
					PREDECREMENT_OPND_BIT | \
					DISPLACEMENT_OPND_BIT | \
					SELECTED_OPND_BIT | \
					INDEXED_OPND_BIT | \
					INDEX_SELECTED_OPND_BIT | \
					ABSOLUTE_SHORT_OPND_BIT | \
					ABSOLUTE_LONG_OPND_BIT | \
					EXPRESSION_OPND_BIT)
#define GROUP_N		STORABLE_GROUP_BITS

#define FULL_EA_GROUP_BITS		(D_REGISTER_OPND_BIT | \
					A_REGISTER_OPND_BIT | \
					A_REGISTER_INDIRECT_OPND_BIT | \
					POSTINCREMENT_OPND_BIT | \
					PREDECREMENT_OPND_BIT | \
					DISPLACEMENT_OPND_BIT | \
					SELECTED_OPND_BIT | \
					INDEXED_OPND_BIT | \
					INDEX_SELECTED_OPND_BIT | \
					PC_DISPLACEMENT_OPND_BIT | \
					PC_INDEXED_OPND_BIT | \
					IMMEDIATE_OPND_BIT | \
					ABSOLUTE_SHORT_OPND_BIT | \
					ABSOLUTE_LONG_OPND_BIT | \
					EXPRESSION_OPND_BIT)
#define GROUP_O		FULL_EA_GROUP_BITS

#define STORABLE_NO_A_REGISTER_GROUP_BITS	\
			 		(D_REGISTER_OPND_BIT | \
					A_REGISTER_INDIRECT_OPND_BIT | \
					POSTINCREMENT_OPND_BIT | \
					PREDECREMENT_OPND_BIT | \
					DISPLACEMENT_OPND_BIT | \
					SELECTED_OPND_BIT | \
					INDEXED_OPND_BIT | \
					INDEX_SELECTED_OPND_BIT | \
					ABSOLUTE_SHORT_OPND_BIT | \
					ABSOLUTE_LONG_OPND_BIT | \
					EXPRESSION_OPND_BIT)
#define GROUP_P		STORABLE_NO_A_REGISTER_GROUP_BITS

#define FULL_EA_NO_A_REGISTER_GROUP_BITS	(D_REGISTER_OPND_BIT | \
					A_REGISTER_INDIRECT_OPND_BIT | \
					POSTINCREMENT_OPND_BIT | \
					PREDECREMENT_OPND_BIT | \
					DISPLACEMENT_OPND_BIT | \
					SELECTED_OPND_BIT | \
					INDEXED_OPND_BIT | \
					INDEX_SELECTED_OPND_BIT | \
					PC_DISPLACEMENT_OPND_BIT | \
					PC_INDEXED_OPND_BIT | \
					IMMEDIATE_OPND_BIT | \
					ABSOLUTE_SHORT_OPND_BIT | \
					ABSOLUTE_LONG_OPND_BIT | \
					EXPRESSION_OPND_BIT)
#define GROUP_Q		FULL_EA_NO_A_REGISTER_GROUP_BITS

#define MISC_1_GROUP_BITS		(A_REGISTER_INDIRECT_OPND_BIT | \
					POSTINCREMENT_OPND_BIT | \
					DISPLACEMENT_OPND_BIT | \
					SELECTED_OPND_BIT | \
					INDEXED_OPND_BIT | \
					INDEX_SELECTED_OPND_BIT | \
					PC_DISPLACEMENT_OPND_BIT | \
					PC_INDEXED_OPND_BIT | \
					ABSOLUTE_SHORT_OPND_BIT | \
					ABSOLUTE_LONG_OPND_BIT | \
					EXPRESSION_OPND_BIT)
#define GROUP_R		MISC_1_GROUP_BITS

#define STORABLE_NO_REGISTER_GROUP_BITS	(A_REGISTER_INDIRECT_OPND_BIT | \
					POSTINCREMENT_OPND_BIT | \
					PREDECREMENT_OPND_BIT | \
					DISPLACEMENT_OPND_BIT | \
					SELECTED_OPND_BIT | \
					INDEXED_OPND_BIT | \
					INDEX_SELECTED_OPND_BIT | \
					ABSOLUTE_SHORT_OPND_BIT | \
					ABSOLUTE_LONG_OPND_BIT | \
					EXPRESSION_OPND_BIT)
#define GROUP_S		STORABLE_NO_REGISTER_GROUP_BITS

#define MISC_2_GROUP_BITS		(A_REGISTER_INDIRECT_OPND_BIT | \
					PREDECREMENT_OPND_BIT | \
					DISPLACEMENT_OPND_BIT | \
					SELECTED_OPND_BIT | \
					INDEXED_OPND_BIT | \
					INDEX_SELECTED_OPND_BIT | \
					ABSOLUTE_SHORT_OPND_BIT | \
					ABSOLUTE_LONG_OPND_BIT | \
					EXPRESSION_OPND_BIT)
#define GROUP_T		MISC_2_GROUP_BITS

#define MISC_3_GROUP_BITS		(A_REGISTER_INDIRECT_OPND_BIT | \
					DISPLACEMENT_OPND_BIT | \
					SELECTED_OPND_BIT | \
					INDEXED_OPND_BIT | \
					INDEX_SELECTED_OPND_BIT | \
					PC_DISPLACEMENT_OPND_BIT | \
					PC_INDEXED_OPND_BIT | \
					ABSOLUTE_SHORT_OPND_BIT | \
					ABSOLUTE_LONG_OPND_BIT | \
					EXPRESSION_OPND_BIT)
#define GROUP_U		MISC_3_GROUP_BITS

#define CLASS_I_BITS        (GROUP_A | GROUP_D)
#define CLASS_II_BITS       (GROUP_A | GROUP_B | GROUP_F | GROUP_O | \
			    GROUP_P | GROUP_S)
#define CLASS_III_BITS      (GROUP_F | GROUP_N)
#define CLASS_IV_BITS       (GROUP_A | GROUP_F | GROUP_H | GROUP_I | \
			    GROUP_P | GROUP_Q | GROUP_S)
#define CLASS_V_BITS        (GROUP_A | GROUP_F | GROUP_S)
#define CLASS_VI_BITS       GROUP_G
#define CLASS_VII_BITS      (GROUP_A | GROUP_F | GROUP_P)
#define CLASS_VIII_BITS     (GROUP_A | GROUP_Q)
#define CLASS_IX_BITS       GROUP_P
#define CLASS_X_BITS        (GROUP_A | GROUP_B | GROUP_F | GROUP_O | GROUP_P)
#define CLASS_XI_BITS       GROUP_C
#define CLASS_XII_BITS      (GROUP_A | GROUP_G)
#define CLASS_XIII_BITS     (GROUP_A | GROUP_F | GROUP_H | GROUP_I | GROUP_P)
#define CLASS_XIV_BITS      GROUP_M
#define CLASS_XV_BITS       GROUP_A
#define CLASS_XVI_BITS      GROUP_J
#define CLASS_XVII_BITS     GROUP_U
#define CLASS_XVIII_BITS    (GROUP_B | GROUP_U)
#define CLASS_XIX_BITS      (GROUP_B | GROUP_F)
#define CLASS_XX_BITS       (GROUP_B | GROUP_H | GROUP_I | GROUP_K | \
			    GROUP_L | GROUP_M | GROUP_O | GROUP_Q)
#define CLASS_XXI_BITS      (GROUP_M | GROUP_R | GROUP_T)
#define CLASS_XXII_BITS     (GROUP_A | GROUP_E)
#define CLASS_XXIII_BITS    (GROUP_A | GROUP_F)
#define CLASS_XXIV_BITS     (GROUP_M | GROUP_S)
#define CLASS_XXV_BITS      GROUP_F
#define CLASS_XXVI_BITS     GROUP_B
#define CLASS_XXVII_BITS    GROUP_F
#define CLASS_XXVIII_BITS   GROUP_F

#define MAX_NUMBER_OF_OPERANDS    17
