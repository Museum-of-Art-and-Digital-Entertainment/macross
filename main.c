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

