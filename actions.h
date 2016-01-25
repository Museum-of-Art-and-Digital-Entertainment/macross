#ifndef ACTIONS_H_
#define ACTIONS_H_

#include "macrossTypes.h"

void actionsDir1(opcodeTableEntryType *opcode, int numberOfOperands, valueType **evaluatedOperands);
void actionsDir2(opcodeTableEntryType *opcode, int numberOfOperands, valueType **evaluatedOperands);
void actionsDirIndir(opcodeTableEntryType *opcode, int numberOfOperands, valueType **evaluatedOperands);
void actionsDirX1(opcodeTableEntryType *opcode, int numberOfOperands, valueType **evaluatedOperands);
void actionsDirX2(opcodeTableEntryType *opcode, int numberOfOperands, valueType **evaluatedOperands);
void actionsDirX3(opcodeTableEntryType *opcode, int numberOfOperands, valueType **evaluatedOperands);
void actionsDirY(opcodeTableEntryType *opcode, int numberOfOperands, valueType **evaluatedOperands);
void actionsImmDir(opcodeTableEntryType *opcode, int numberOfOperands, valueType **evaluatedOperands);
void actionsImmDirX(opcodeTableEntryType *opcode, int numberOfOperands, valueType **evaluatedOperands);
void actionsImmDirY(opcodeTableEntryType *opcode, int numberOfOperands, valueType **evaluatedOperands);
void actionsImmIndex(opcodeTableEntryType *opcode, int numberOfOperands, valueType **evaluatedOperands);
void actionsIndex(opcodeTableEntryType *opcode, int numberOfOperands, valueType **evaluatedOperands);
void actionsNone(opcodeTableEntryType *opcode, int numberOfOperands, valueType **evaluatedOperands);
void actionsRelative(opcodeTableEntryType *opcode, int numberOfOperands, valueType **evaluatedOperands);
bool isByte(int value);
bool isByteOffset(int value);
bool isWordOffset(int value);
bool isByteAddress(valueType *value);
bool isWord(int value);
bool byteCheck(int value);
bool wordCheck(int value);
bool isDefined(valueType *value);

#endif
