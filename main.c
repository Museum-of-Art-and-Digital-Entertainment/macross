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
	main.c -- Top level of the Macross assembler.

	Chip Morningstar -- Lucasfilm Ltd.

	5-December-1984
*/

#include "macrossTypes.h"
#include "macrossGlobals.h"

main(argc, argv)
  int	argc;
  char *argv[];
{
#ifdef __APPLE__
	char end = get_end();
#else
	extern char	 end;
#endif
	char		*sbrk();

	fflush(stdout);
	initializeStuff(argc, argv);
	yyparse();
	finishUp();
	if (emitPrint)
		printf("storage high water mark 0x%x == %d\n", sbrk(0) - &end,
			sbrk(0) - &end);
	if (errorFlag)
		chokePukeAndDie();
	else
		exit(0);
}

  void
printVersion()
{
	printf("Macross %s version 4.20.\n", TARGET_CPU_STRING);
}

