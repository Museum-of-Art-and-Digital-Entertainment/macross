#ifndef STATEMENT_SEMANTICS_H_
#define STATEMENT_SEMANTICS_H_

#include "macrossTypes.h"

void assembleBlock(blockType *block);
simpleFixupListType *assembleBlockInsideIf(blockType *block, simpleFixupListType *ongoingFixupList);
bool operandCheck(opcodeTableEntryType *opcode, int numberOfOperands, valueType *evaluatedOperands[]);
void assembleMachineInstruction(opcodeTableEntryType *opcode, operandListType *operands);
void assembleMacro(macroTableEntryType *macroInstruction, operandListType *operands);
void assembleAlignStatement(alignStatementBodyType *alignStatement);
void assembleAssertStatement(assertStatementBodyType *assertStatement);
void assembleBlockStatement(blockStatementBodyType *blockStatement);
void assembleByteStatement(byteStatementBodyType *byeStatement);
void assembleConstrainStatement(constrainStatementBodyType *constrainStatement);
void assembleDbyteStatement(dbyteStatementBodyType *dbyteStatement);
void assembleDefineStatement(defineStatementBodyType *defineStatement);
void assembleDoUntilStatement(doUntilStatementBodyType *doUntilStatement);
void assembleDoWhileStatement(doWhileStatementBodyType *doWhileStatement);
void assembleExternStatement(externStatementBodyType *externStatement);
void assembleFReturnStatement(freturnStatementBodyType *freturnStatement);
void assembleFunctionStatement(functionStatementBodyType *functionStatement);
void assembleGroupStatement(blockType *groupStatement);
simpleFixupListType *assembleIfStatement(ifStatementBodyType *ifStatement, bool terminalIf, simpleFixupListType *ongoingFixupList);
void assembleIfStatementOldStyle(ifStatementBodyType *ifStatement);
void assembleIncludeStatement(includeStatementBodyType *includeStatement);
void assembleInstructionStatement(instructionStatementBodyType *Statement, int cumulativeLineNumber);
void assembleLongStatement(longStatementBodyType *longStatement);
void assembleMacroStatement(macroStatementBodyType *macroStatement);
void assembleMdefineStatement(defineStatementBodyType *mdefineStatement);
void assembleMdoUntilStatement(mdoUntilStatementBodyType *mdoUntilStatement);
void assembleMdoWhileStatement(mdoWhileStatementBodyType *mdoWhileStatement);
void assembleMforStatement(mforStatementBodyType *mforStatement);
void assembleMifStatement(mifStatementBodyType *mifStatement, int cumulativeLineNumber);
void assembleMswitchStatement(mswitchStatementBodyType *mswitchStatement);
void assembleMvariableStatement(mvariableStatementBodyType *mvariableStatement);
void assembleMwhileStatement(mwhileStatementBodyType *mwhileStatement);
void assembleOrgStatement(orgStatementBodyType *orgStatement);
void assemblePerformStatement(performStatementBodyType *performStatement);
void assembleRelStatement(relStatementBodyType *relStatement);
void assembleStartStatement(startStatementBodyType *startStatement);
void assembleStringStatement(stringStatementBodyType *stringStatement);
void assembleStructStatement(structStatementBodyType *structStatement);
void assembleTargetStatement(targetStatementBodyType *targetStatement);
void assembleUndefineStatement(undefineStatementBodyType *undefineStatement);
void assembleVariableStatement(variableStatementBodyType *variableStatement);
void assembleWhileStatement(whileStatementBodyType *whileStatement);
void assembleWordStatement(wordStatementBodyType *wordStatement);
bool assembleStatementBody(statementKindType kind, statementBodyType body, int cumulativeLineNumber, bool worryAboutIf, simpleFixupListType **ifFixupList);
void assembleLabelList(labelListType *labelList);
simpleFixupListType *assembleStatement(statementType *statement, bool insideIf, simpleFixupListType *ongoingFixupList);
void eatStatement(statementType *statement);

#endif
