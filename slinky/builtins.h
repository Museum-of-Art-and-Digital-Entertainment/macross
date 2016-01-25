#ifndef BUILTINS_H_
#define BUILTINS_H_

#include "slinkyTypes.h"

void tooFewArgs(int argCount, stringType *name);
void tooManyArgs(int argCount, stringType *name);
stringType *atasciiBIF(int argCount);
stringType *atasciiColorBIF(int argCount);
bool isAbsoluteValueBIF(int argCount);
bool isConditionCodeBIF(int argCount);
bool isDefinedBIF(int argCount);
bool isExternalBIF(int argCount);
int nthCharBIF(int argCount);
int printfBIF(int argCount);
stringType *strcatBIF(int argCount);
int strcmpBIF(int argCount);
int strcmplcBIF(int argCount);
int strlenBIF(int argCount);
char *substrBIF(int argCount);
addressType symbolLookupBIF(int argCount);
stringType *symbolNameBIF(int argCount);

#endif
