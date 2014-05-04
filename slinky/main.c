/*
	main.c -- Top level of the Slinky linker

	Chip Morningstar -- Lucasfilm Ltd.

	9-March-1985
*/

#include "slinkyTypes.h"
#include "slinkyGlobals.h"

main(argc, argv)
  int	argc;
  char *argv[];
{
	initializeStuff(argc, argv);
	linkem();
	if (errorFlag)
		chokePukeAndDie();
	else
		exit(0);
}

  void
printVersion()
{
	printf("Slinky version 1.16.\n");
}
