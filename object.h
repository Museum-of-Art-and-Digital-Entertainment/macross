#ifndef OBJECT_H_
#define OBJECT_H_

#include "macrossTypes.h"

void outputObjectFile(void);
void outputWord(int aWord);
void outputPartition(void);
void outputBigword(long unsigned int bigword);
void outputByte(byte aByte);
void outputString(stringType *string);
void outputStartAddress(addressType startAddress);
void outputRelocatableCode(void);
void outputBreak(codeBreakType *codeBreak);
void outputAbsoluteCode(void);
void outputOneCodeBuffer(codeSegmentType *segment);
void outputPseudoSegment(addressType codeStartAddress, addressType codeEndAddress);
bool isObjectSymbol(symbolTableEntryType *symbol);
void enumerateAndCountSymbols(void);
int enumerateAndCountReferences(void);
void outputReference(expressionReferenceType *reference);
void outputReferenceInfo(void);
void outputOneSymbol(symbolTableEntryType *symbol);
void outputSymbolTableInfo(void);
int symbolCompare(symbolTableEntryType **symbol1, symbolTableEntryType **symbol2);
bool shouldDumpSymbol(symbolTableEntryType *symbol);
void dumpSymbolTable(void);
bool hackableSymbol(symbolTableEntryType *symbol);
void printValueTersely(valueType *value);
void outputReservations(void);
void outputExpressionBuffer(void);
void outputOneExpression(expressionType *expression);
void outputExpressions(void);
void outputOneFunction(functionDefinitionType *function);
void outputFunctions(void);

#endif
