#ifndef GARBAGE_H_
#define GARBAGE_H_

#include "macrossTypes.h"

void freeArrayTerm(arrayTermType *arrayTerm);
void freeAssignmentTerm(binopTermType *assignmentTerm);
void freeBinopTerm(binopTermType *binopTerm);
void freeFunctionCall(functionCallTermType *functionCall);
void freePostopTerm(postOpTermType *postopTerm);
void freePreopTerm(preOpTermType *preopTerm);
void freeString(stringType *string);
void freeUnopTerm(unopTermType *unopTerm);
void freeExpression(expressionType *expression);
void freeExpressionList(expressionListType *expressionList);
void freeIdentifierList(identifierListType *identifierList);
void freeSelectionList(selectionListType *selectionList);
void freeBlock(blockType *block);
void freeCase(caseType *aCase);
void freeCaseList(caseListType *caseList);
void freeOperandList(operandListType *operandList);
void freeMacro(operandListType *operands);
void freeMachineInstruction(operandListType *operands);
void freeAlignStatement(alignStatementBodyType *alignStatement);
void freeAssertStatement(assertStatementBodyType *assertStatement);
void freeBlockStatement(blockStatementBodyType *blockStatement);
void freeByteStatement(byteStatementBodyType *byteStatement);
void freeConstrainStatement(constrainStatementBodyType *constrainStatement);
void freeDbyteStatement(dbyteStatementBodyType *dbyteStatement);
void freeDefineStatement(defineStatementBodyType *defineStatement);
void freeDoUntilStatement(doUntilStatementBodyType *doUntilStatement);
void freeDoWhileStatement(doWhileStatementBodyType *doWhileStatement);
void freeExternStatement(externStatementBodyType *externStatement);
void freeFreturnStatement(freturnStatementBodyType *freturnStatement);
void freeFunctionStatement(functionStatementBodyType *functionStatement);
void freeIfStatement(ifStatementBodyType *ifStatement);
void freeIncludeStatement(includeStatementBodyType *includeStatement);
void freeInstructionStatement(instructionStatementBodyType *instructionStatement);
void freeLongStatement(longStatementBodyType *longStatement);
void freeMacroStatement(macroStatementBodyType *macroStatement);
void freeMdefineStatement(defineStatementBodyType *mdefineStatement);
void freeMdoUntilStatement(mdoUntilStatementBodyType *mdoUntilStatement);
void freeMdoWhileStatement(mdoWhileStatementBodyType *mdoWhileStatement);
void freeMifStatement(mifStatementBodyType *mifStatement);
void freeMswitchStatement(mswitchStatementBodyType *mswitchStatement);
void freeMforStatement(mforStatementBodyType *mforStatement);
void freeMvariableStatement(mvariableStatementBodyType *mvariableStatement);
void freeMwhileStatement(mwhileStatementBodyType *mwhileStatement);
void freeOrgStatement(orgStatementBodyType *orgStatement);
void freePerformStatement(performStatementBodyType *performStatement);
void freeRelStatement(relStatementBodyType *relStatement);
void freeStartStatement(startStatementBodyType *startStatement);
void freeStringStatement(stringStatementBodyType *stringStatement);
void freeStructStatement(structStatementBodyType *structStatement);
void freeTargetStatement(targetStatementBodyType *targetStatement);
void freeUndefineStatement(undefineStatementBodyType *undefineStatement);
void freeVariableStatement(variableStatementBodyType *variableStatement);
void freeWhileStatement(whileStatementBodyType *whileStatement);
void freeWordStatement(wordStatementBodyType *wordStatement);
void freeStatementBody(statementKindType kind, statementBodyType body);
void freeLabelList(labelListType *labelList);
void freeStatement(statementType *statement);
void freeArray(arrayType *array);
void freeValue(valueType *value);

#endif
