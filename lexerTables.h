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
	lexerTables.h -- Tables describing stuff the lexer needs to know.

	Chip Morningstar -- Lucasfilm Ltd.

	3-November-1984
*/

struct {
	char			first;
	char			second;
	int			token;
	assignmentKindType	value;
			} operatorTable[] = {
		'!', '=',	NOT_EQUAL_TO,	NO_ASSIGN,
		'!', '\0',	LOGICAL_NOT,	NO_ASSIGN,
		'%', '=',	ASSIGN,		MOD_ASSIGN,
		'%', '\0',	MOD,		NO_ASSIGN,
		'&', '&',	LOGICAL_AND,	NO_ASSIGN,
		'&', '=',	ASSIGN,		AND_ASSIGN,
		'&', '\0',	BITWISE_AND,	NO_ASSIGN,
		'*', '=',	ASSIGN,		MUL_ASSIGN,
		'*', '\0',	MUL,		NO_ASSIGN,
		'+', '+',	INCREMENT,	NO_ASSIGN,
		'+', '=',	ASSIGN,		ADD_ASSIGN,
		'+', '\0',	ADD,		NO_ASSIGN,
		'-', '-',	DECREMENT,	NO_ASSIGN,
		'-', '=',	ASSIGN,		SUB_ASSIGN,
		'-', '\0',	SUB,		NO_ASSIGN,
		'.', '\0',	SELECT,		NO_ASSIGN,
		'/', '=',	ASSIGN,		DIV_ASSIGN,
		'/', '\0',	DIV,		NO_ASSIGN,
		'<', '<',	LEFT_SHIFT,	NO_ASSIGN,
		'<', '=',	LESS_THAN_OR_EQUAL_TO,	NO_ASSIGN,
		'<', '\0',	LESS_THAN,	NO_ASSIGN,
		'=', '=',	EQUAL_TO,	NO_ASSIGN,
		'=', '\0',	ASSIGN,		ASSIGN_ASSIGN,
		'>', '=',	GREATER_THAN_OR_EQUAL_TO,	NO_ASSIGN,
		'>', '>',	RIGHT_SHIFT,	NO_ASSIGN,
		'>', '\0',	GREATER_THAN,	NO_ASSIGN,
		'^', '=',	ASSIGN,		XOR_ASSIGN,
		'^', '^',	LOGICAL_XOR,	NO_ASSIGN,
		'^', '\0',	BITWISE_XOR,	NO_ASSIGN,
		'|', '=',	ASSIGN,		OR_ASSIGN,
		'|', '|',	LOGICAL_OR,	NO_ASSIGN,
		'|', '\0',	BITWISE_OR,	NO_ASSIGN,
		'~', '\0',	BITWISE_NOT,	NO_ASSIGN,
		'?', '\0',	HI_BYTE,	NO_ASSIGN,
		'\0', '\0',	0,		NO_ASSIGN,
			};

char escapeCodes[256] = {
	'\000',	'\001',	'\002',	'\003',	'\004',	'\005',	'\006',	'\007',
	'\010',	'\011',	'\012',	'\013',	'\014',	'\015',	'\016',	'\017',
	'\020',	'\021',	'\022',	'\023',	'\024',	'\025',	'\026',	'\027',
	'\030',	'\031',	'\032',	'\033',	'\034',	'\035',	'\036',	'\037',
	' ',	'!',	'"',	'#',	'$',	'%',	'&',	'\'',
	'(',	')',	'*',	'+',	',',	'-',	'.',	'/',
	'0',	'1',	'2',	'3',	'4',	'5',	'6',	'7',
	'8',	'9',	':',	';',	'<',	'=',	'>',	'?',
	'@',	'A',	'B',	'C',	'D',	'E',	'F',	'G',
	'H',	'I',	'J',	'K',	'L',	'M',	'N',	'O',
	'P',	'Q',	'R',	'S',	'T',	'U',	'V',	'W',
	'X',	'Y',	'Z',	'[',	'\\',	']',	'^',	'_',
	'`',	'a',	'\b',	'c',	'd',	'\33',	'\f',	'g',
	'h',	'i',	'j',	'k',	'l',	'm',	'\n',	'o',
	'p',	'q',	'\r',	's',	'\t',	'u',	'v',	'w',
	'x',	'y',	'z',	'{',	'|',	'}',	'~',	'\177',
	'\200',	'\201',	'\202',	'\203',	'\204',	'\205',	'\206',	'\207',
	'\210',	'\211',	'\212',	'\213',	'\214',	'\215',	'\216',	'\217',
	'\220',	'\221',	'\222',	'\223',	'\224',	'\225',	'\226',	'\227',
	'\230',	'\231',	'\232',	'\233',	'\234',	'\235',	'\236',	'\237',
	'\240',	'\241',	'\242',	'\243',	'\244',	'\245',	'\246',	'\247',
	'\250',	'\251',	'\252',	'\253',	'\254',	'\255',	'\256',	'\257',
	'\260',	'\261',	'\262',	'\263',	'\264',	'\265',	'\266',	'\267',
	'\270',	'\271',	'\272',	'\273',	'\274',	'\275',	'\276',	'\277',
	'\300',	'\301',	'\302',	'\303',	'\304',	'\305',	'\306',	'\307',
	'\310',	'\311',	'\312',	'\313',	'\314',	'\315',	'\316',	'\317',
	'\320',	'\321',	'\322',	'\323',	'\324',	'\325',	'\326',	'\327',
	'\330',	'\331',	'\332',	'\333',	'\334',	'\335',	'\336',	'\337',
	'\340',	'\341',	'\342',	'\343',	'\344',	'\345',	'\346',	'\347',
	'\350',	'\351',	'\352',	'\353',	'\354',	'\355',	'\356',	'\357',
	'\360',	'\361',	'\362',	'\363',	'\364',	'\365',	'\366',	'\367',
	'\370',	'\371',	'\372',	'\373',	'\374',	'\375',	'\376',	'\377',
};
