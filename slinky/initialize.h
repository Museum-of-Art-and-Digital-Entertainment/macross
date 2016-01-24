#ifndef INITIALIZE_H_
#define INITIALIZE_H_

#include "slinkyTypes.h"

void chokePukeAndDie(void);
void initializeStuff(int argc, char **argv);
void queueInputFile(char *name);
void queueLoadAddress(char *addressString);

#endif
