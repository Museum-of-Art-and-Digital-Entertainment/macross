#ifndef FIXUPS_H_
#define FIXUPS_H_

#include "macrossTypes.h"

expressionType *generateFixupExpression(expressionType *expression);
expressionType *duplicateExpressionForFixup(expressionType *expression, bool isTopLevel, bool isSpecialFunctionOperand);
functionCallTermType *duplicateFunctionCall(functionCallTermType *functionCall);
expressionType *duplicateArrayReference(arrayTermType *arrayTerm);

#endif
