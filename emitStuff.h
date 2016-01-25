#ifndef EMIT_STUFF_H_
#define EMIT_STUFF_H_

#include "macrossTypes.h"

void incarnateCodeBuffer(int bufferNum, codeBufferKindType bufferKind);
void putByte(addressType address, byte byteValue);
void mapByte(int address, byte byteValue);
void emitByte(byte byteValue);
void emitWord(wordType wordValue);
void emitLong(longType longValue);
void emitByteValue(valueType *byteValue);
void emitString(stringType *string);
void emitWordValue(valueType *wordValue);
void emitLongValue(valueType *longValue);
void pokeByteValue(addressType location, valueType *value);
void pokeWordValue(addressType location, valueType *value);
void pokeLongValue(addressType location, valueType *value);
void pokeRelativeByteValue(addressType location, valueType *value);
void pokeRelativeWordValue(addressType location, valueType *value);
byte getByte(addressType address);
void emitRelativeByteOffset(valueType *target);
void emitRelativeWordOffset(valueType *target);
void fixupBranch(valueType *location, valueType target);
void fixupJump(simpleFixupListType *locations, valueType target);

#endif
