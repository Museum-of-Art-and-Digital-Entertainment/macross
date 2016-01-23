#ifndef OPERAND_STUFF_H_
#define OPERAND_STUFF_H_

#include "macrossTypes.h"

#if TARGET_CPU == CPU_6502
operandType *buildOperand(operandKindType kindOfOperand, anyOldThing *arg);
#elif TARGET_CPU == CPU_68000
operandType *buildOperand(operandKindType kindOfOperand, int arg1, int arg2, int arg3, int arg4);
#else
#error Unknown or undefined processor type
#endif

operandListType *duplicateOperandForFixup(operandListType *operand, bool isSpecialFunctionOperand);
void expandOperand(operandKindType addressMode, char *buffer);
void freeOperand(operandType *operand);
valueType *evaluateOperand(operandType *operand);
conditionType invertConditionCode(conditionType conditionCode);
bool shouldParethesize(operandType *operand);

#endif
