#ifndef READ_H_
#define READ_H_

#include "slinkyTypes.h"

void fileCheck(FILE *fildes, char *fileName);
wordType readWord(FILE *file, char *fileName);
byte readByte(FILE *file, char *fileName);
bigWord readBigword(FILE *file, char *fileName);
bigWord read3ByteWord(FILE *file, char *fileName);
int readString(char *buffer, FILE *fildes, char *fileName);
void readChunk(byte *buffer, int numberOfBytes, FILE *fildes, char *fileName);
void readCode(addressType startAddress, addressType endAddress, int mode, objectFileListType *objectFile, FILE *objectFildes);
bool compareReferences(expressionReferenceType *reference1, expressionReferenceType *reference2);
void sortReferences(expressionReferenceType *theReferences, int numberOfReferences);
void readReference(expressionReferenceType *reference, FILE *fildes, char *fileName);
void readReferences(objectFileListType *objectFile, FILE *objectFildes);
bool compareSymbolValues(symbolType **symbol1, symbolType **symbol2);
void readSymbols(objectFileListType *objectFile, FILE *objectFildes);
expressionPCType readOneExpression(objectFileListType *objectFile, FILE *objectFildes);
void readExpressions(objectFileListType *objectFile, FILE *objectFildes);
argumentListType *readArgumentList(objectFileListType *objectFile, FILE *objectFildes);
void readFunctions(objectFileListType *objectFile, FILE *objectFildes);
void instantiateExpressionAndSymbolPointers(objectFileListType *objectFile);
void readReservations(objectFileListType *objectFile, FILE *objectFildes);
reservationListType *buildReservation(addressType startAddress, int blockSize, reservationListType *nextReservation);

#endif
