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

typedef union {
		expressionOperandBodyType	*expressionUnion;
		stringOperandBodyType		*stringUnion;
		blockOperandBodyType		*blockUnion;
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
