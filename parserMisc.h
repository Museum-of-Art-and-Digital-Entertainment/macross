#ifndef PARSER_MISC_H_
#define PARSER_MISC_H_
#include "macrossTypes.h"

statementType *addLabelToSatement(labelListType *labelList, statementType *statement);
void botch(char *message, ...);
void checkDefineAssignmentOperator(assignmentKindType assignmentOperator);
void convertDefineToMDefine(statementType *defineStatement);
ifStatementBodyType *extractIfBody(statementType *ifStatement);
mifStatementBodyType *extractMifBody(statementType *mifStatement);
stringType *extractString(operandType *textExpression);
void popMacroOrFunctionNestingDepth();
void pushMacroOrFunctionNestingDepth();
char *saveString(char *s);

#endif
