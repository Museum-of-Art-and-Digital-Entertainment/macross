#ifndef ERROR_STUFF_H_
#define ERROR_STUFF_H_

#include "macrossTypes.h"

void puntOnError(errorType theError, ...);
void printErrorMessage(errorType theError, va_list ap);
void error(errorType theError, ...);
void verror(errorType theError, va_list ap);
void warning(errorType theError, ...);
void fatalError(errorType theError, ...);
void fatalSystemError(errorType theError, ...);
void yyerror(char *s);
char *usageString(symbolUsageKindType usageKind);
char *valueKindString(valueKindType valueKind);
char *assignmentString(assignmentKindType assignment);

#endif
