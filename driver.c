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
	driver.c -- Top level driver program for all versions of Macross

	Chip Morningstar -- Lucasfilm Ltd.

	3-May-1985

*/

#include <stdio.h>

#define MACROSS_6502	"/u1/gg/bin/macross_6502"
#define MACROSS_68000	"/u1/gg/bin/macross_68000"

char	*m6502 = "6502";
char	*m68000 = "68000";

char	**mlist;

main(int argc, char **argv)
{
	char   *processor = m6502;
	int	i;
	int	j;

	mlist = (char **)calloc(argc + 1, sizeof (char **));
	for (i=1, j=1; i<argc; i++) {
		if (argv[i][0] == '-' && argv[i][1]== 'P') {
			processor = argv[++i];
		} else {
			mlist[j++] = argv[i];
		}
	}
	mlist[0] = processor;
	mlist[j] = NULL;
	if (strcmp(processor, m6502) == 0) {
		execv(MACROSS_6502, mlist);
		fprintf(stderr, "Macross: driver can't fork to %s for some reason\n", MACROSS_6502);
	} else if (strcmp(processor, m68000) == 0) {
		execv(MACROSS_68000, mlist);
		fprintf(stderr, "Macross: driver can't fork to %s for some reason\n", MACROSS_68000);
	} else {
		fprintf(stderr, "Macross: '%s' is an unknown processor\n", processor);
	}
	exit(1);
}
