/*
	operandBody_68000.h -- Define parser tree types for 68000 operands.

	Chip Morningstar -- Lucasfilm Ltd.

	25-April-1985
*/

typedef expressionType		 expressionOperandBodyType;

typedef stringType		 stringOperandBodyType;

typedef BlockType		 blockOperandBodyType;

typedef nullType		 dRegisterOperandBodyType;

typedef nullType		 aRegisterOperandBodyType;

typedef nullType		 aRegisterIndirectOperandBodyType;

typedef nullType		 postincrementOperandBodyType;

typedef nullType		 predecrementOperandBodyType;

typedef expressionType		 displacementOperandBodyType;

typedef selectionListType	 selectedOperandBodyType;

typedef expressionType		 indexedOperandBodyType;

typedef selectionListType	 indexSelectedOperandBodyType;

typedef expressionType		 pcDisplacementOperandBodyType;

typedef expressionType		 pcIndexedOperandBodyType;

typedef expressionType		 immediateOperandBodyType;

typedef expressionType		 absoluteShortOperandBodyType;

typedef expressionType		 absoluteLongOperandBodyType;

typedef nullType		 ccRegisterOperandBodyType;

typedef nullType		 statusRegisterOperandBodyType;

typedef nullType		 uspRegisterOperandBodyType;

typedef nullType		 controlRegisterOperandBodyType;

#define BlockOperandBodyType	 anyOldThing /* kludge */
/* doing above right confuses compiler as it is a forward reference inside
	yon union: */

typedef union {
		expressionOperandBodyType	*expressionUnion;
		stringOperandBodyType		*stringUnion;
		BlockOperandBodyType		*blockUnion;
		dRegisterOperandBodyType	*dRegisterUnion;
		aRegisterOperandBodyType	*aRegisterUnion;
		aRegisterIndirectOperandBodyType *aRegisterIndirectUnion;
		postincrementOperandBodyType	*postincrementUnion;
		predecrementOperandBodyType	*predecrementUnion;
		displacementOperandBodyType	*displacementUnion;
		selectedOperandBodyType		*selectionUnion;
		indexedOperandBodyType		*indexedUnion;
		indexSelectedOperandBodyType	*indexSelectedUnion;
		pcDisplacementOperandBodyType	*pcDisplacementUnion;
		pcIndexedOperandBodyType	*pcIndexedUnion;
		immediateOperandBodyType	*immediateUnion;
		absoluteShortOperandBodyType	*absoluteShortUnion;
		absoluteLongOperandBodyType	*absoluteLongUnion;
		ccRegisterOperandBodyType	*ccRegisterUnion;
		statusRegisterOperandBodyType	*statusRegisterUnion;
		uspRegisterOperandBodyType	*uspRegisterUnion;
		controlRegisterOperandBodyType	*controlRegisterUnion;
			       } operandBodyType;

#define SFC_REGISTER	0
#define DFC_REGISTER	1
#define VBR_REGISTER	2
