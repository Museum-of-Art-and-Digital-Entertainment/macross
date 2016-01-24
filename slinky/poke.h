#ifndef POKE_H_
#define POKE_H_

#include "slinkyTypes.h"

bool isWordSized(int value);
bool isByteSized(int value);
bool isByteOffset(int value);
int computeRelativeValue(int valueToPoke, codeSegmentHeaderType *codeSegment, int offset);
int getBaseValue(byte *codeBuffer, int offset, int referenceKind);
void pokeValue(int value, byte *codeBuffer, int offset, int referenceKind, int trueAddress);
void fixupReference(expressionReferenceType *reference, codeSegmentHeaderType *codeSegment);
void pokem(void);

#endif
