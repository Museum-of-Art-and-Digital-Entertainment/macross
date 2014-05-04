/*
	debugPrintSD_6502.c -- Routines to print out things for debugging
		purposes (system dependent routines, 6502 version).

	Chip Morningstar -- Lucasfilm Ltd.

	23-April-1985

*/

#include "macrossTypes.h"
#include "macrossGlobals.h"
#include "y.tab.h"


int	tablevel;

/* Fundamental nop print operation */
#define nullPrint(thing)   if (thing==NULL) { tab(); printf("()\n"); return; }

  void
printCondition(condition)
  conditionType	condition;
{
/* This table MUST be maintained congruently with the definition of the
   enumerated type 'conditionType'. */
	static char	*conditionTable[] = {
		"carry",
		"zero",
		"negative",
		"overflow",
		"lt",
		"leq",
		"slt",
		"sleq",
		"always",
		"!carry",
		"!zero",
		"!negative",
		"!overflow",
		"geq",
		"gt",
		"sgeq",
		"sgt",
		"never",
	};

	printf("%s", conditionTable[(int)condition]);
}

  void
printOperandKind(kind)
  operandKindType	kind;
{
/* This table MUST be maintained congruently with the definition of the
   enumerated type 'operandKindType'. */
	static char	*operandKindTable[] = {
		"expression opnd",
		"immediate opnd",
		"indirect opnd",
		"a register opnd",
		"x register opnd",
		"y register opnd",
		"post indexed y opnd",
		"pre indexed x opnd",
		"x indexed opnd",
		"y indexed opnd",
		"x selected opnd",
		"y selected opnd",
		"pre selected x opnd",
		"string opnd",
		"block opnd",
	};

	printf("%s", operandKindTable[(int)kind]);
}

  void
printToken(token)
  int	token;
{
/* This table MUST be maintained congruently with the set of '#define's in
   the file 'y.tab.h' as produced by yacc. */
	static char	*tokenTable[] = {
		"A",
		"ALIGN",
		"ASSERT",
		"BLOCK",
		"BYTE",
		"CONSTRAIN",
		"DBYTE",
		"DEFINE",
		"DO",
		"ELSE",
		"ELSEIF",
		"ENDFILE",
		"EOL",
		"EXTERN",
		"FRETURN",
		"FUNCTION",
		"HERE",
		"IF",
		"INCLUDE",
		"LONG",
		"MACRO",
		"MCASE",
		"MDEFAULT",
		"MDEFINE",
		"MDO",
		"MELSE",
		"MELSEIF",
		"MFOR",
		"MIF",
		"MSWITCH",
		"MUNTIL",
		"MVARIABLE",
		"MWHILE",
		"ORG",
		"REL",
		"START",
		"STRING",
		"STRUCT",
		"TARGET",
		"UNDEFINE",
		"UNTIL",
		"VARIABLE",
		"WHILE",
		"WORD",
		"X",
		"Y",
		"ConditionCode",
		"Identifier",
		"MacroName",
		"Number",
		"Opcode",
		"TextString",
		"ASSIGN",
		"LOGICAL_OR",
		"LOGICAL_XOR",
		"LOGICAL_AND",
		"BITWISE_OR",
		"BITWISE_XOR",
		"BITWISE_AND",
		"EQUAL_TO",
		"NOT_EQUAL_TO",
		"LESS_THAN",
		"LESS_THAN_OR_EQUAL_TO",
		"GREATER_THAN",
		"GREATER_THAN_OR_EQUAL_TO",
		"LEFT_SHIFT",
		"RIGHT_SHIFT",
		"ADD",
		"SUB",
		"MUL",
		"DIV",
		"MOD",
		"UNARY_MINUS",
		"LOGICAL_NOT",
		"BITWISE_NOT",
		"HI_BYTE",
		"LO_BYTE",
		"SELECT",
		"INCREMENT",
		"DECREMENT",
	};

	if (token >= A) {
		printf("%s", tokenTable[token - A]);
	} else {
		printf("'%c'", token);
	}
}

  void
printOperand(operand)
  operandType	*operand;
{
	nullPrint(operand);
	tab(); printf("(operand: [");
	printOperandKind(operand->kindOfOperand);
	printf("]\n");
	tablevel++;
	switch (operand->kindOfOperand) {

	case EXPRESSION_OPND:
		printExpression(operand->theOperand);
		break;

	case IMMEDIATE_OPND:
		printExpression(operand->theOperand);
		break;

	case INDIRECT_OPND:
		printExpression(operand->theOperand);
		break;

	case A_REGISTER_OPND:
		break;

	case X_REGISTER_OPND:
		break;

	case Y_REGISTER_OPND:
		break;

	case POST_INDEXED_Y_OPND:
		printExpression(operand->theOperand);
		break;

	case PRE_INDEXED_X_OPND:
		printExpression(operand->theOperand);
		break;

	case X_INDEXED_OPND:
		printExpression(operand->theOperand);
		break;

	case Y_INDEXED_OPND:
		printExpression(operand->theOperand);
		break;

	case X_SELECTED_OPND:
		printIdentifierList(operand->theOperand);
		break;

	case Y_SELECTED_OPND:
		printIdentifierList(operand->theOperand);
		break;

	case PRE_SELECTED_X_OPND:
		printIdentifierList(operand->theOperand);
		break;

	case STRING_OPND:
		tab(); printf("(string: \"%s\")\n", operand->theOperand);
		break;

	case BLOCK_OPND:
		printBlock(operand->theOperand);
		break;

	default:
		printf("fooey: bad operand kind %d\n", operand->
							kindOfOperand);
		break;
	}
	tablevel--;
	tab(); printf(")\n");
}
