#ifndef EXPRESSION_SEMANTICS_H_
#define EXPRESSION_SEMANTICS_H_
#include "macrossTypes.h"

anyOldThing *arrayLookup(arrayTermType *arrayTerm, valueKindType *kindOfThing);
valueType *evaluateArrayTerm(arrayTermType *arrayTerm);
valueType *evaluateAssignmentTerm(binopTermType *assignmentTerm, fixupKindType kindOfFixup);
valueType *evaluateBinopTerm(binopTermType *assignmentTerm, bool isTopLevel, fixupKindType kindOfFixup);
valueType *evaluateCondition(conditionType condition);
valueType *evaluateBuiltInFunctionCall(symbolInContextType *workingContext, operandListType *parameters, fixupKindType kindOfFixup);
valueType *evaluateFunctionCall(functionCallTermType *functionCall, fixupKindType kindOfFixup, bool isStandalone);
valueType *evaluateHere(void);
valueType *evaluateIdentifier(symbolTableEntryType *identifier, bool isTopLevel, fixupKindType kindOfFixup);
valueType *evaluateNumber(numberTermType number);
valueType *evaluatePostopTerm(postOpTermType *postopTerm);
valueType *evaluatePreopTerm(preOpTermType *preopTerm);
valueType *evaluateString(stringType *string);
valueType *evaluateUnopTerm(unopTermType* unopTerm, fixupKindType kindOfFixup);
valueType *evaluateExpressionInternally(expressionType *expression, bool isToplevel, fixupKindType kindOfFixup, bool isStandalone);
valueType *evaluateExpression(expressionType *expression, fixupKindType kindOfFixup);
void evaluateExpressionStandalone(expressionType *expression);
valueType *evaluateDefineExpression(expressionType *expression);
valueType *evaluateSelectionList(selectionListType *selectionList);

#endif
