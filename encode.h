#ifndef ENCODE_H_
#define ENCODE_H_

#include "macrossTypes.h"

bool encodeByte(byte aByte);
bool encodeBigword(int bigword);
bool encodeAssignmentTerm(binopTermType *assignmentTerm);
bool encodeBinopTerm(binopTermType *binopTerm);
bool encodeCondition(conditionType condition);
int functionNumber(functionDefinitionType *function);
bool encodeFunctionCall(functionCallTermType *functionCall);
bool encodeHere(void);
bool encodeIdentifier(symbolTableEntryType *identifier);
bool encodeNumber(numberTermType number);
bool encodeRelocatableNumber(numberTermType number);
bool encodeOperand(operandType *operand);
bool encodePostopTerm(postOpTermType *postopTerm);
bool encodePreopTerm(preOpTermType *preopTerm);
bool encodeString(stringType *string);
bool encodeUnopTerm(unopTermType *unopTerm);
bool encodeValue(valueType *value);
bool encodeExpression(expressionType *expression);
bool encodeAssertStatement(assertStatementBodyType *assertStatement);
bool encodeFreturnStatement(freturnStatementBodyType *freturnStatement);
bool encodeMdefineStatement(defineStatementBodyType *mdefineStatement);
bool encodeMdoUntilStatement(mdoUntilStatementBodyType *mdoUntilStatement);
bool encodeMdoWhileStatement(mdoWhileStatementBodyType *mdoWhileStatement);
bool encodeMforStatement(mforStatementBodyType *mforStatement);
bool encodeMifStatement(mifStatementBodyType *mifStatement);
bool encodeMswitchStatement(mswitchStatementBodyType *mswitchStatement);
bool encodeMvariableStatement(mvariableStatementBodyType *mvariableStatement);
bool encodeMwhileStatement(mwhileStatementBodyType *mwhileStatement);
bool encodeStatement(statementType *statement);
bool encodeBlock(blockType *block);

#endif
