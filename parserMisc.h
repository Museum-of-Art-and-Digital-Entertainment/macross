#ifndef PARSER_MISC_H_
#define PARSER_MISC_H_

#include "macrossTypes.h"

statementType *addLabelToStatement(labelListType *labelList, statementType *statement);
void botch(char *message, ...);
void checkDefineAssignmentOperator(assignmentKindType assignmentOperator);
statementType *convertDefineToMdefine(statementType *defineStatement);
ifStatementBodyType *extractIfBody(statementType *ifStatement);
mifStatementBodyType *extractMifBody(statementType *mifStatement);
stringType *extractString(operandType *textExpression);
void popMacroOrFunctionNestingDepth(void);
void pushMacroOrFunctionNestingDepth(void);
char *saveString(char *s);

#endif
