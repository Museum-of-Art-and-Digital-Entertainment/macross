/*
	operandBody_6502.h -- Define parser tree types for 6502 operands.

	Chip Morningstar -- Lucasfilm Ltd.

	18-April-1985
*/

typedef stringType		 stringOperandBodyType;

typedef expressionType		 xIndexedOperandBodyType;

typedef expressionType		 yIndexedOperandBodyType;

typedef expressionType		 preIndexedXOperandBodyType;

typedef expressionType		 postIndexedYOperandBodyType;

typedef nullType		 yRegisterOperandBodyType;

typedef nullType		 xRegisterOperandBodyType;

typedef nullType		 aRegisterOperandBodyType;

typedef expressionType		 immediateOperandBodyType;

typedef expressionType		 indirectOperandBodyType;

typedef expressionType		 expressionOperandBodyType;

typedef BlockType		 blockOperandBodyType;

typedef selectionListType	 xSelectedOperandBodyType;

typedef selectionListType	 ySelectedOperandBodyType;

typedef selectionListType	 preSelectedOperandBodyType;

#define BlockOperandBodyType	 anyOldThing /* kludge */
/* doing above right confuses compiler as it is a forward reference inside
	yon union: */

typedef union {
		expressionOperandBodyType	*expressionUnion;
		immediateOperandBodyType	*immediateUnion;
		indirectOperandBodyType		*indirectUnion;
		aRegisterOperandBodyType	*aRegisterUnion;
		xRegisterOperandBodyType	*xRegisterUnion;
		yRegisterOperandBodyType	*yRegisterUnion;
		postIndexedYOperandBodyType	*postIndexedYUnion;
		preIndexedXOperandBodyType	*preIndexedXUnion;
		xIndexedOperandBodyType		*xIndexedUnion;
		yIndexedOperandBodyType		*yIndexedUnion;
		xSelectedOperandBodyType	*xSelectedUnion;
		ySelectedOperandBodyType	*ySelectedUnion;
		preSelectedOperandBodyType	*preSelectedUnion;
		stringOperandBodyType		*stringUnion;
		BlockOperandBodyType		*blockUnion;
			       } operandBodyType;
