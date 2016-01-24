#ifndef DEBUG_PRINT_H_
#define DEBUG_PRINT_H_

#include "macrossTypes.h"

/* Platform-independent */
void tab(void);
void printAssignmentKind(assignmentKindType assignmentKind);
void printExpressionKind(expressionTermKindType kind);
stringType *statementKindString(statementKindType kind);
void printStatementKind(statementKindType kind);
void printValue(valueType *value);
void printSymbol(symbolTableEntryType *symbol);
void printArgumentDefinitionList(argumentDefinitionListType *list);
void printBlock(blockType *block);
void printArrayTerm(arrayTermType *arrayTerm);
void printAssignmentTerm(binopTermType *assignmentTerm);
void printBinopTerm(binopTermType *binopTerm);
void printFunctionCall(functionCallTermType *functionCall);
void printHere(void);
void printIdentifier(symbolTableEntryType *identifier);
void printNumber(numberTermType number);
void printPostopTerm(postOpTermType *postopTerm);
void printPreopTerm(preOpTermType *preopTerm);
void printUnopTerm(unopTermType *unopTerm);
void printExpression(expressionType *expression);
void printExpressionList(expressionListType *expressionList);
void printIdentifierList(identifierListType *identifierList);
void printCase(caseType *aCase);
void printCaseList(caseListType *caseList);
void printMacro(macroTableEntryType *macroInstruction);
void printOpcode(opcodeTableEntryType *opcode);
void printOperandList(operandListType *operandList);
void printAlignStatement(alignStatementBodyType *alignStatement);
void printAssertStatement(assertStatementBodyType *assertStatement);
void printBlockStatement(blockStatementBodyType *blockStatement);
void printByteStatement(byteStatementBodyType *byteStatement);
void printConstrainStatement(constrainStatementBodyType *constrainStatement);
void printDbyteStatement(dbyteStatementBodyType *dbyteStatement);
void printDefineStatement(defineStatementBodyType *defineStatement);
void printDoUntilStatement(doUntilStatementBodyType *doUntilStatement);
void printDoWhileStatement(doWhileStatementBodyType *doWhileStatement);
void printExternStatement(externStatementBodyType *externStatement);
void printFreturnStatement(freturnStatementBodyType *freturnStatement);
void printFunctionStatement(functionStatementBodyType *functionStatement);
void printIfStatement(ifStatementBodyType *ifStatement);
void printIncludeStatement(includeStatementBodyType *includeStatement);
void printInstructionStatement(instructionStatementBodyType *instructionStatement);
void printLongStatement(longStatementBodyType *longStatement);
void printMacroStatement(macroStatementBodyType *macroStatement);
void printMdefineStatement(defineStatementBodyType *mdefineStatement);
void printMdoUntilStatement(mdoUntilStatementBodyType *mdoUntilStatement);
void printMdoWhileStatement(mdoWhileStatementBodyType *mdoWhileStatement);
void printMforStatement(mforStatementBodyType *mforStatement);
void printMifStatement(mifStatementBodyType *mifStatement);
void printMswitchStatement(mswitchStatementBodyType *mswitchStatement);
void printMvariableStatement(mvariableStatementBodyType *mvariableStatement);
void printMwhileStatement(mwhileStatementBodyType *mwhileStatement);
void printOrgStatement(orgStatementBodyType *orgStatement);
void printPerformStatement(performStatementBodyType *performStatement);
void printRelStatement(relStatementBodyType *relStatement);
void printStartStatement(startStatementBodyType *startStatement);
void printStringStatement(stringStatementBodyType *stringStatement);
void printStructStatement(structStatementBodyType *structStatement);
void printTargetStatement(targetStatementBodyType *targetStatement);
void printUndefineStatement(undefineStatementBodyType *undefineStatement);
void printVariableStatement(variableStatementBodyType *variableStatement);
void printWhileStatement(whileStatementBodyType *whileStatement);
void printWordStatement(wordStatementBodyType *wordStatement);
void printLabelList(labelListType *labelList);
void printStatementBody(statementKindType kind, statementBodyType body);
void printStatement(statementType *statement);
void printPendingFixupList(fixupListType *fixupList);
void printCreateFixup(expressionType *expression, addressType location, fixupKindType kindOfFixup);
void printExpressionBuffer(void);
void printOneCodeBuffer(codeSegmentType *codeSegment, int bufferNum);
void printCodeBufferSection(codeRegionType *codeBufferSection);
void printCodeBuffers(void);

/* Platform-specific */
void printCondition(conditionType condition);
void printOperandKind(operandKindType kind);
void printToken(int token);
void printOperand(operandType *operand);

#endif
