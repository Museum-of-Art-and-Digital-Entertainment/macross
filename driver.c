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

main(argc, argv)
	char **argv;
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
