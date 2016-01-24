#ifndef SEMANTIC_MISC_H_
#define SEMANTIC_MISC_H_

#include "macrossTypes.h"

/* Miscellaneous */
bool absoluteValue(valueType *address);
void addAttributeToSymbol(symbolTableEntryType *symbol, symbolAttributesType attribute);
addressType addressValue(valueType *value);
valueKindType addValueKind(valueType *leftOperand, valueType *rightOperand);
bool alreadyDefined(symbolInContextType *context);
bool booleanTest(expressionType *expression);
int countArguments(functionDefinitionType *function);
int countParameters(operandListType *parameterList);
arrayType *allocArray(int size, valueType ***contentsPtr);
valueType *createArray(expressionType *dimension, expressionListType *initializers);
bool decrementable(valueType *value);
int expressionListLength(expressionListType *expressionList);
int fieldValue(symbolTableEntryType *symbol);
bool incrementable(valueType *value);
int intValue(valueType *value);
bool isAssignable(symbolInContextType *context);
bool isBuiltInFunction(symbolInContextType *context);
bool isDefinable(symbolInContextType *context);
bool isExternal(symbolTableEntryType *symbol);
bool isFailure(valueType *value);
bool isFunction(symbolInContextType *context);
bool isLastStatementInBlock(statementType *statement);
bool isLogicalOp(int op);
bool isPotentialVariable(symbolInContextType *context);
bool isUndefined(valueType *value);
bool isUsable(valueType *value);
bool logicalXOR(int int1, int int2);
valueType *newValue(valueKindType kindOfValue, int value, operandKindType addressMode);
valueKindType opValueKind(valueType *leftOperand, valueType *rightOperand);
bool relocatableValue(valueType *address);
valueKindType selectValueKind(valueType *leftOperand, valueType *rightOperand);
valueKindType subValueKind(valueType *leftOperand, valueType *rightOperand);
int swabInt(int i);
valueType *swabValue(valueType *value);
valueKindType unopValueKind(valueType *operand);
void valueField(symbolTableEntryType *symbol, valueType *value);
void valueLabel(symbolTableEntryType *symbol, valueType *value);

/* Fixups and references */
void createFixup(expressionType *expression, addressType location, fixupKindType kindOfFixup, codeBufferKindType codeMode, int whichFixup);
void finishUp(void);
void noteAnonymousReference(void);
void noteReference(expressionType *expression, fixupKindType kindOfFixup, addressType location, codeBufferKindType codeMode);
void performFixups(fixupListType *fixups);
void performStartAddressFixup(void);
void putFixupsHere(fixupKindType kindOfFixupsToPut, int whichFixup);

/* Contexts and dynamic symbol creation */
void addNewLocalVariable(symbolTableEntryType *symbol);
symbolTableEntryType *effectiveSymbol(symbolTableEntryType *symbol, symbolInContextType **assignmentTargetContext);
symbolTableEntryType *generateLocalLabel(symbolTableEntryType *symbol);
symbolInContextType *getBaseContext(symbolTableEntryType *identifier);
symbolInContextType *getWorkingContext(symbolTableEntryType *identifier);
stringType *localLabelString(symbolTableEntryType *symbol);
int localLabelTagValue(symbolTableEntryType *symbol);
void addBreak(codeBreakKindType kind, int data);
void reserveAbsolute(addressType startAddress, int blockSize);
bool listableStatement(statementKindType kind);

#endif
