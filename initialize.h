#ifndef INITIALIZE_H_
#define INITIALIZE_H_

#include "macrossTypes.h"

void chokePukeAndDie(void);
void initializeStuff(int argc, char **argv);
void installBuiltInFunctions(void);
void installPredefinedSymbols(void);
void installCommandLineDefineSymbols(void);
void createHashTables(void);
void queueInputFile(char *name);
void openFirstInputFile(void);
bool isDotMName(stringType *fileName);
bool parseCommandLineDefine(char *arg, char **name, int *value);
void noteCommandLineDefine(char *arg);

/* Actually defined in main.c */
void printVersion(void);

#endif
