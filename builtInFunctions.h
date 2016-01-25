#ifndef BUILT_IN_FUNCTIONS_H_
#define BUILT_IN_FUNCTIONS_H_

#include "macrossTypes.h"

/* Platform-independent */
valueType *makeBooleanValue(int test);
valueType *makeFailureValue(void);
valueType *makeIntegerValue(int integer);
valueType *makeOperandValue(operandType *operand);
valueType *makeStringValue(stringType *string);
valueType *makeUndefinedValue(void);
valueType *addressModeBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *applyBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *arrayLengthBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *atasciiBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *atasciiColorBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *debugModeOffBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *debugModeOnBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *emitModeOffBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *emitModeOnBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isAbsoluteValueBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isBlockBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isBuiltInFunctionBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isConditionCodeBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isDefinedBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isExternalBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isFieldBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isFunctionBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isRelocatableValueBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isStringBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isStructBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isSymbolBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *listingOffBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *listingOnBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *makeArrayBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *nthCharBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *printfBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *strcatBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *strcmpBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *strcmplcBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *strlenBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *substrBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *symbolLookupBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *symbolDefineBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *symbolNameBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *symbolUsageBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *valueTypeBIF(operandListType *parameterList, fixupKindType kindOfFixup);

/* 6502-specific */
valueType *isARegisterBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isDirectModeBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isImmediateModeBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isIndexedModeBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isIndirectModeBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isPostIndexedModeBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isPreIndexedModeBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isXIndexedModeBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isXRegisterBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isYIndexedModeBIF(operandListType *parameterList, fixupKindType kindOfFixup);
valueType *isYRegisterBIF(operandListType *parameterList, fixupKindType kindOfFixup);

#endif
