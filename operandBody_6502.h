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
		blockOperandBodyType		*blockUnion;
			       } operandBodyType;
