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
	conditionDefs_68000.h -- Define types for 68000 condition codes.

	Chip Morningstar -- Lucasfilm Ltd.

	25-April-1985
*/

typedef enum {
		CARRY_COND, EQUAL_COND, OVERFLOW_COND, MINUS_COND, LT_COND,
		 LEQ_COND, LOW_OR_SAME_COND,
		 ALWAYS_COND,
		NOT_CARRY_COND, NOT_EQUAL_COND, NOT_OVERFLOW_COND, PLUS_COND,
		 GEQ_COND, GT_COND, HIGH_COND,
		 NEVER_COND,
		NOT_FOUND_COND,
			       } conditionType;

#define COMPOUND_BRANCH_MAX	1

