#ifndef RELOCATE_H_
#define RELOCATE_H_

#include "slinkyTypes.h"

void removeZeroPageFromFreeList(void);
addressType align(addressType address, int alignment);
addressType constrain(addressType address, int size, addressType constraint);
void moveRelocationBase(addressType newBase);
addressType allocateRelocatable(codeSegmentHeaderType *codeSegment);
void relocateOneCodeSegment(codeSegmentHeaderType *codeSegment, addressType targetLocation);
void relocatem(void);
codeSegmentHeaderType *matchModes(symbolType *symbol, codeSegmentHeaderType *codeSegment);
bool matchedModes(symbolType *symbol, codeSegmentHeaderType *codeSegment);
codeSegmentHeaderType *synchronizeCodeSegment(symbolType *symbol, codeSegmentHeaderType *codeSegment);
void handleGlobalSymbol(symbolType *symbol);
void valueSymbol(symbolType *symbol, codeSegmentHeaderType *codeSegment);
symbolType *lookupGlobalSymbol(char *symbolName);
void valueUndefinedSymbol(symbolType *symbol);
void valuem(void);

#endif
