#ifndef DEBUG_PRINT_H_
#define DEBUG_PRINT_H_

#include "slinkyTypes.h"

void printCode(int startAddress, int endAddress, int mode);
void printReference(expressionReferenceType *reference);
void printReferenceFixup(expressionReferenceType *reference);
void printSymbol(int symbolTag, symbolType *symbol);
void printLoadMapSymbol(symbolType *symbol);
void printGlobalSymbols(void);
void printExpression(expressionPCType expression, int length);

#endif
