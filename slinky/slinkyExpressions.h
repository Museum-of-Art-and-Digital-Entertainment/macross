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
	slinkyExpressions.h -- Expression definitions for the Slinky linker

	Chip Morningstar -- Lucasfilm Ltd.

	6-November-1985
*/

/*
   Expressions are stored in the object file in a compact parse-tree
   representation.  We just read this directly and interpret it on-the-fly
   without much sophistication.  Function definitions include whole
   statements, but the principle is the same.

   The basic form of a syntactic entity is:
	+---------+-------------+
	| tagByte | bodyOfThing |
	+---------+-------------+
   where 'tabByte' encodes how 'bodyOfThing' is to be interpreted.

   These are the possible 'thing's, as encoded by the 'tagByte's
			+------+
   identifier:		| sym# |   'sym#' is an index into the symbol entries
			+------+    of the object file when the expression is
			in the object file too, and then 'sym#' is converted
			to a pointer to symbol structure when the symbol
			itself is read in.
			+-------+-------+------+------+-----+
   functionCall:	| func# | argCt | expr | expr | ... |   'argCt' is a
			+-------+-------+------+------+-----+   single byte
			argument count (which means that a function may have
			no more than 255 arguments if it is to be used
			externally) and this is how many 'expr's there are.
			+-----+
   number:		| num |   'num' is just a 4-byte number.
			+-----+
			+----+
   conditionCode:	| cc |   'cc' is a single byte condition code.
			+----+
			+------+
   subexpression:	| expr |
			+------+
			+----+------+
   unop:		| op | expr |   'op' is a 1-byte operation number.
			+----+------+
			+----+------+------+
   binop:		| op | expr | expr |
			+----+------+------+
			+----+------+
   preop:		| op | expr |
			+----+------+
			+----+------+
   postop:		| op | expr |
			+----+------+

   here:		empty body.

			+---+---+---+---+-----+----+
   string:		| a | b | c | d | ... | \0 |   Typical 'C' string.
			+---+---+---+---+-----+----+
			+------+------+
   array:		| sym# | expr |
			+------+------+
			+-----+
   value:		| num |
			+-----+

   null:		empty body.  Encodes null expression *or* statement.

			+-------+-------+-----+-----+
   block:		| thing | thing | ... | end |   'end' acts as a
			+-------+-------+-----+-----+   termination symbol.
			+------+------+
   mdefine:		| sym# | expr |   'expr' can be null.
			+------+------+
			+------+------+
   mvar:		| sym# | expr |   'expr' can be null.
			+------+------+
			+------+-------+-------+
   mif:			| expr | block | block |   either 'block' can be null.
			+------+-------+-------+
			+------+------+------+-------+
   mfor:		| expr | expr | expr | block |
			+------+------+------+-------+
			+------+-------+
   mwhile:		| expr | block |
			+------+-------+
			+------+-------+
   mdoWhile:		| expr | block |
			+------+-------+
			+------+-------+
   mdoUntil:		| expr | block |
			+------+-------+
			+------+
   freturn:		| expr |
			+------+
			+------+
   perform:		| expr |
			+------+
			+-------+
   group:		| block |
			+-------+
			+------+--------+
   assert:		| expr | string |
			+------+--------+
			+------+--------+--------+-----+-----+
   mswitch:		| expr | clause | clause | ... | end |
			+------+--------+--------+-----+-----+
			+------+------+-----+-------+
   clause:		| expr | expr | ... | block |
			+------+------+-----+-------+

   end:			empty body.

*/

#define IDENTIFIER_TAG		0x00
#define FUNCTION_CALL_TAG	0x01
#define NUMBER_TAG		0x02
#define CONDITION_CODE_TAG	0x03
#define SUBEXPRESSION_TAG	0x04
#define UNOP_TAG		0x05
#define BINOP_TAG		0x06
#define PREOP_TAG		0x07
#define POSTOP_TAG		0x08
#define HERE_TAG		0x09
#define STRING_TAG		0x0A
#define ARRAY_TAG		0x0B
#define VALUE_TAG		0x0C
#define NULL_TAG		0x0D
#define BLOCK_TAG		0x0E
#define MDEFINE_TAG		0x0F
#define MVARIABLE_TAG		0x10
#define MIF_TAG			0x11
#define MFOR_TAG		0x12
#define MWHILE_TAG		0x13
#define MDOWHILE_TAG		0x14
#define MDOUNTIL_TAG		0x15
#define PERFORM_TAG		0x16
#define GROUP_TAG		0x17
#define ASSERT_TAG		0x18
#define MSWITCH_TAG		0x19
#define CLAUSE_TAG		0x1A
#define END_TAG			0x1B
#define FRETURN_TAG		0x1C
#define BUILTIN_FUNCTION_CALL_TAG 0x1D
#define RELOCATABLE_TAG		0x1E
