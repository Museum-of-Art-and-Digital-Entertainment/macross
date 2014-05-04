/*
	debugPrintSD_68000.c -- Routines to print out things for debugging
		purposes (system dependent routines, 68000 version).

	Chip Morningstar -- Lucasfilm Ltd.

	26-April-1985

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
		"equal",
		"overflow",
		"minus",
		"lt",
		"leq",
		"ls",
		"always",
		"!carry",
		"!equal",
		"!overflow",
		"plus",
		"geq",
		"gt",
		"high",
		"never",
	};

	printf("%s", conditionTable[(int)condition]);
}

  void
printRegister(reg)
  int	reg;
{
	if (reg < 8)
		printf(" a%d", reg);
	else
		printf(" d%d", reg - 8);
}

  void
printControlRegister(reg)
  int	reg;
{
	switch (reg) {
	case SFC_REGISTER: printf("sfc"); break;
	case DFC_REGISTER: printf("dfc"); break;
	case VBR_REGISTER: printf("vbr"); break;
	default: botch("Bad control register %d\n", reg); break;
	}
}

  void
printOperandKind(kind)
  operandKindType	kind;
{
/* This table MUST be maintained congruently with the definition of the
   enumerated type 'operandKindType'. */
	static char	*operandKindTable[] = {
		"expression opnd",
		"string opnd",
		"block opnd",
		"data register opnd",
		"address register opnd",
		"address register indirect opnd",
		"postincrement opnd",
		"predecrement opnd",
		"displacement opnd",
		"indexed opnd",
		"program counter displacement opnd",
		"program counter indexed opnd",
		"immediate opnd",
		"absolute short opnd",
		"absolute long opnd",
		"condition code register opnd",
		"status register opnd",
		"user stack pointer opnd",
		"control register opnd",
	};

	printf("%s", operandKindTable[(int)operandKindField(kind)]);
	if (isRegisterOperand(kind))
		printRegister(getRegister(kind));
	if (isControlRegisterOperand(kind))
		printControlRegister(getRegister(kind));
	if (isIndexedOperand(kind))
		printRegister(getIndexRegister(kind));
}

  bool
isRegisterOperand(kind)
  operandKindType	kind;
{
	kind = operandKindField(kind);
	return(kind==D_REGISTER_OPND || kind==A_REGISTER_OPND ||
		kind==A_REGISTER_INDIRECT_OPND || kind==POSTINCREMENT_OPND ||
		kind==PREDECREMENT_OPND || kind==DISPLACEMENT_OPND ||
		kind==INDEXED_OPND || kind==SELECTED_OPND ||
		kind==INDEX_SELECTED_OPND);
}

  bool
isIndexedOperand(kind)
  operandKindType	kind;
{
	kind = operandKindField(kind);
	return(kind==INDEXED_OPND || kind==PC_INDEXED_OPND || kind==
		INDEX_SELECTED_OPND);
}

  bool
isControlRegisterOperand(kind)
  operandKindType	kind;
{
	return(operandKindField(kind) == CONTROL_REGISTER_OPND);
}

  void
printToken(token)
  int	token;
{
/* This table MUST be maintained congruently with the set of '#define's in
   the file 'y.tab.h' as produced by yacc. */
	static char	*tokenTable[] = {
		"A0",
		"A1",
		"A2",
		"A3",
		"A4",
		"A5",
		"A6",
		"A7",
		"ALIGN",
		"ASSERT",
		"BLOCK",
		"BYTE",
		"CCR",
		"CONSTRAIN",
		"D0",
		"D1",
		"D2",
		"D3",
		"D4",
		"D5",
		"D6",
		"D7",
		"DBYTE",
		"DEFINE",
		"DFC",
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
		"L",
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
		"PC",
		"REL",
		"SFC",
		"SR",
		"START",
		"STRING",
		"STRUCT",
		"TARGET",
		"UNDEFINE",
		"UNTIL",
		"USP",
		"VARIABLE",
		"VBR",
		"W",
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

	if (token >= A0) {
		printf("%s", tokenTable[token - A0]);
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
	case DISPLACEMENT_OPND:
	case INDEXED_OPND:
	case PC_DISPLACEMENT_OPND:
	case PC_INDEXED_OPND:
	case IMMEDIATE_OPND:
	case ABSOLUTE_SHORT_OPND:
	case ABSOLUTE_LONG_OPND:
		printExpression(operand->theOperand);
		break;

	case D_REGISTER_OPND:
	case A_REGISTER_OPND:
	case A_REGISTER_INDIRECT_OPND:
	case POSTINCREMENT_OPND:
	case PREDECREMENT_OPND:
	case CC_REGISTER_OPND:
	case STATUS_REGISTER_OPND:
	case USP_REGISTER_OPND:
	case CONTROL_REGISTER_OPND:
		break;

	case SELECTED_OPND:
	case INDEX_SELECTED_OPND:
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
