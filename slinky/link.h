#ifndef LINK_H_
#define LINK_H_

#include "slinkyTypes.h"

bool internalizeOneObjectFile(objectFileListType *objectFile);
bool strcmplc(char *s1, char *s2);
bool compareSymbols(symbolType **symbol1, symbolType **symbol2);
void buildGlobalSymbolTable(objectFileListType *inputFileList);
bool readem(void);
codeSegmentHeaderType *locateConflictingSegment(codeSegmentHeaderType *codeSegment);
void reserveSegment(addressType start, addressType end);
codeSegmentHeaderType *allocateAbsolute(codeSegmentHeaderType *codeSegment);
void reserveReservations(void);
void installSegment(codeSegmentHeaderType *codeSegment);
void installAbsoluteCodeSegment(codeSegmentHeaderType *codeSegment);
void linkem(void);

#endif
