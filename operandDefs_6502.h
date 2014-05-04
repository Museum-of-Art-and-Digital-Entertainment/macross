/*
	operandDefs_6502.h -- Define operand types and flag bits for 6502.

	Chip Morningstar -- Lucasfilm Ltd.

	18-April-1985
*/

typedef enum {
		EXPRESSION_OPND, IMMEDIATE_OPND, INDIRECT_OPND,
		A_REGISTER_OPND, X_REGISTER_OPND, Y_REGISTER_OPND,
		POST_INDEXED_Y_OPND, PRE_INDEXED_X_OPND, X_INDEXED_OPND,
		Y_INDEXED_OPND, X_SELECTED_OPND, Y_SELECTED_OPND,
		PRE_SELECTED_X_OPND, STRING_OPND, BLOCK_OPND
			       } operandKindType;

#define operandKindField(op)	op

/* Opcodes: */
/* In the opcode table we want to have information that tells which possible
   address modes the corresponding instruction can utilize.  The
   instruction set of the 6502 breaks up into classes of instructions, such
   that all of the instructions in a class accept the same address modes.
   We encode the instructions by class and the classes by permissible address
   modes.  This helps us reduce errors in construction of the opcode table,
   since getting one instruction of a given class right means getting all the
   others right too. */
typedef enum {
		RELATIVE, DIR_1, DIR_2, DIR_INDIR, DIR_X_1, DIR_X_2, DIR_X_3,
		DIR_Y, IMM_DIR, IMM_DIR_X, IMM_DIR_Y, NONE, INDEX, IMM_INDEX
			       } addressClassType;

#define NO_OPND_BIT		0x0000
#define EXPRESSION_OPND_BIT	0x0001
#define IMMEDIATE_OPND_BIT	0x0002
#define INDIRECT_OPND_BIT	0x0004
#define A_REGISTER_OPND_BIT	0x0008
#define X_REGISTER_OPND_BIT	0x0010
#define Y_REGISTER_OPND_BIT	0x0020
#define POST_INDEXED_Y_OPND_BIT	0x0040
#define PRE_INDEXED_X_OPND_BIT	0x0080
#define X_INDEXED_OPND_BIT	0x0100
#define Y_INDEXED_OPND_BIT	0x0200
#define X_SELECTED_OPND_BIT	0x0400
#define Y_SELECTED_OPND_BIT	0x0800
#define PRE_SELECTED_X_OPND_BIT	0x1000
#define STRING_OPND_BIT		0x2000
#define BLOCK_OPND_BIT		0x4000
#define ANY_OPND_BITS		0xFFFF

#define REL_CLASS_BITS		EXPRESSION_OPND_BIT
#define DIR_1_CLASS_BITS	EXPRESSION_OPND_BIT
#define DIR_2_CLASS_BITS	EXPRESSION_OPND_BIT
#define DIR_INDIR_CLASS_BITS	EXPRESSION_OPND_BIT | INDIRECT_OPND_BIT
#define DIR_X_1_CLASS_BITS	EXPRESSION_OPND_BIT | X_INDEXED_OPND_BIT | \
				X_SELECTED_OPND_BIT | A_REGISTER_OPND_BIT
#define DIR_X_2_CLASS_BITS	EXPRESSION_OPND_BIT | X_INDEXED_OPND_BIT | \
				X_SELECTED_OPND_BIT
#define DIR_X_3_CLASS_BITS	EXPRESSION_OPND_BIT | X_INDEXED_OPND_BIT | \
				X_SELECTED_OPND_BIT
#define DIR_Y_CLASS_BITS	EXPRESSION_OPND_BIT | Y_INDEXED_OPND_BIT | \
				Y_SELECTED_OPND_BIT
#define IMM_DIR_CLASS_BITS	EXPRESSION_OPND_BIT | IMMEDIATE_OPND_BIT
#define IMM_DIR_X_CLASS_BITS	EXPRESSION_OPND_BIT | IMMEDIATE_OPND_BIT | \
				X_INDEXED_OPND_BIT | X_SELECTED_OPND_BIT 
#define IMM_DIR_Y_CLASS_BITS	EXPRESSION_OPND_BIT | IMMEDIATE_OPND_BIT | \
				Y_INDEXED_OPND_BIT | Y_SELECTED_OPND_BIT 
#define NONE_CLASS_BITS		NO_OPND_BIT
#define IMM_INDEX_CLASS_BITS	X_INDEXED_OPND_BIT | Y_INDEXED_OPND_BIT | \
				X_SELECTED_OPND_BIT | Y_SELECTED_OPND_BIT | \
				IMMEDIATE_OPND_BIT | PRE_INDEXED_X_OPND_BIT |\
				POST_INDEXED_Y_OPND_BIT | \
				EXPRESSION_OPND_BIT | PRE_SELECTED_X_OPND_BIT
#define INDEX_CLASS_BITS	X_INDEXED_OPND_BIT | Y_INDEXED_OPND_BIT | \
				X_SELECTED_OPND_BIT | Y_SELECTED_OPND_BIT | \
				PRE_INDEXED_X_OPND_BIT | \
				POST_INDEXED_Y_OPND_BIT | \
				EXPRESSION_OPND_BIT | PRE_SELECTED_X_OPND_BIT

#define MAX_NUMBER_OF_OPERANDS	1
