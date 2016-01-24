#ifndef BUILD_STUFF_H_
#define BUILD_STUFF_H_

#include "macrossTypes.h"

/* buildStuff1.c */
statementType *newStatement (statementKindType kind, statementBodyType body);
statementType *buildAlignStatement (expressionType *expression);
statementType *buildAssertStatement (expressionType *condition, expressionType *message);
statementType *buildBlockStatement (expressionListType *expressionList);
statementType *buildByteStatement (expressionListType *expressionList);
statementType *buildConstrainStatement (expressionType *expression, blockType *block);
statementType *buildDbyteStatement (expressionListType *expressionList);
statementType *buildDefineStatement (stringType *name, expressionType *value);
statementType *buildDoUntilStatement (blockType *body, conditionType condition);
statementType *buildDoWhileStatement (blockType *body, conditionType condition);
statementType *buildDoStatement (blockType *body, doEndType *end);
statementType *buildExternStatement (identifierListType *identifierList);
statementType *buildFreturnStatement (expressionType *expression);
statementType *buildFunctionStatement (stringType *name, argumentDefinitionListType *arguments, blockType *body);
statementType *buildGroupStatement (blockType *block);
statementType *buildIfStatement (ifHeadType *head, ifContinuationType continuation, ifContinuationKindType continuationKind);
statementType *buildIncludeStatement (expressionType *filename);
statementType *buildInstructionStatement (opcodeTableEntryType *opcode, operandListType *operands);
statementType *buildLongStatement (expressionListType *expressionList);
statementType *buildMacroStatement (macroTableEntryType *macro, argumentDefinitionListType *arguments, blockType *body);
statementType *buildMacroInstructionStatement (macroTableEntryType *macro, operandListType *operands);
statementType *buildMdefineStatement (stringType *name, expressionType *value);
statementType *buildMdoUntilStatement (blockType *body, struct expressionTermStruct *condition);
statementType *buildMdoWhileStatement (blockType *body, expressionType *condition);
statementType *buildMdoStatement (blockType *body, mdoEndType *end);
statementType *buildMforStatement (forExpressionsType *forExpressions, blockType *body);
statementType *buildMifStatement (mifHeadType *head, mifContinuationType continuation, ifContinuationKindType continuationKind);
statementType *buildMswitchStatement (expressionType *switchExpression, caseListType *cases);
statementType *buildMvariableStatement (stringType *name, expressionListType *value, expressionType *dimension);
statementType *buildMwhileStatement (expressionType *condition, blockType *body);
statementType *buildNullStatement (void);
statementType *buildOrgStatement (expressionType *expression);
statementType *buildPerformStatement (expressionType *expression);
statementType *buildRelStatement (void);
statementType *buildStartStatement (expressionType *expression);
statementType *buildStringStatement (expressionListType *expressionList);
statementType *buildStructStatement (symbolTableEntryType *name, blockType *body);
statementType *buildTargetStatement (expressionType *expression);
statementType *buildUndefineStatement (identifierListType *identifierList);
statementType *buildVariableStatement (stringType *name, expressionListType *value, expressionType *dimension);
statementType *buildWhileStatement (conditionType condition, blockType *body);
statementType *buildWordStatement (expressionListType *expressionList);

/* buildStuff2.c */
caseType *buildCase (expressionListType *caseTags, blockType *caseBody);
doEndType *buildDoEnd (conditionType condition, doEndKindType kindOfDoEnd);
forExpressionsType *buildForExpressions (expressionType *initExpression, expressionType *testExpression, expressionType *incrExpression);
ifHeadType *buildIfHead (conditionType condition, blockType *block);
mdoEndType *buildMdoEnd (expressionType *condition, doEndKindType kindOfMdoEnd);
mifHeadType *buildMifHead (expressionType *condition, blockType *block);
arrayTermType *buildArrayTerm (expressionType *array, expressionType *index);
binopTermType *buildBinopTerm (binopKindType binop, expressionType *leftArgument, expressionType *rightArgument);
functionCallTermType *buildFunctionCall (stringType *functionName, operandListType *arguments);
postOpTermType *buildPostOpTerm (postOpKindType postOp, expressionType *postOpArgument);
preOpTermType *buildPreOpTerm (preOpKindType preOp, expressionType *preOpArgument);
unopTermType *buildUnopTerm (unopKindType unop, expressionType *unopArgument);
expressionTermType *buildExpressionTerm (expressionTermKindType kindOfExpressionTerm, anyOldThing *arg1, anyOldThing *arg2, anyOldThing *arg3);
macroTableEntryType *buildMacroTableEntry (stringType *name);
symbolTableEntryType *buildSymbolTableEntry (stringType *name, symbolUsageKindType usage);
codeBreakType *buildCodeBreak (codeBreakKindType kind, addressType address, int data);
reservationListType *buildReservation (addressType startAddress, int blockSize, reservationListType *nextReservation);
simpleFixupListType *buildSimpleFixupList (valueType locationToFixup, simpleFixupListType *previousList);

/* buildStuff3.c */
argumentListHeadType *buildArgumentList (stringType *new, argumentListHeadType *rest, bool arrayTag);
caseListHeadType *buildCaseList (caseType *new, caseListHeadType *rest);
expressionListHeadType *buildExpressionList (expressionType *new, expressionListHeadType *rest);
identifierListHeadType *buildIdentifierList (stringType *new, identifierListHeadType *rest, symbolUsageKindType usage);
labelListHeadType *buildLabelList (symbolTableEntryType *new, labelListHeadType *rest);
operandListHeadType *buildOperandList (operandType *new, operandListHeadType *rest);
selectionListHeadType *buildSelectionList (selectionListType *new, selectionListHeadType *rest);
statementListHeadType *buildStatementList (statementType *new, statementListHeadType *rest);

#endif
