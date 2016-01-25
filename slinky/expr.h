#ifndef EXPR_H_
#define EXPR_H_

#include "slinkyTypes.h"

int getNumber(void);
addressType evaluateArray(void);
addressType evaluateAssert(void);
addressType evaluateBinop(void);
addressType evaluateBlock(void);
addressType evaluateConditionCode(void);
void pushSymbol(symbolType *symbol, addressType value);
void bindFunctionArguments(functionType *theFunction, int argCount);
void undoBindings(void);
addressType evaluateFreturn(void);
addressType evaluateBuiltinFunctionCall(void);
addressType evaluateFunctionCall(void);
addressType evaluateHere(void);
addressType evaluateMdefine(void);
addressType evaluateMdoUntil(void);
addressType evaluateMdoWhile(void);
addressType evaluateMfor(void);
addressType evaluateMif(void);
bool evaluateClause(addressType pattern);
addressType evaluateMswitch(void);
addressType evaluateMwhile(void);
addressType evaluateMvariable(void);
addressType evaluateNumber(void);
addressType evaluateRelocatableNumber(void);
addressType evaluatePerform(void);
addressType evaluatePostop(void);
addressType evaluatePreop(void);
addressType evaluateString(void);
addressType evaluateSymbol(void);
addressType evaluateUnop(void);
addressType evaluateExpression(void);
void skipArray(void);
void skipAssert(void);
void skipBinop(void);
void skipBlock(void);
void skipFunctionCall(void);
void skipMdefine(void);
void skipMdoUntil(void);
void skipMdoWhile(void);
void skipMfor(void);
void skipMif(void);
void skipClause(void);
void skipMswitch(void);
void skipMvariable(void);
void skipMwhile(void);
void skipPostop(void);
void skipPreop(void);
void skipString(void);
void skipUnop(void);
void skipExpression(void);

#endif
