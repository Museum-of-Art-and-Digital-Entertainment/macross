#ifndef LISTING_H_
#define LISTING_H_

#include "macrossTypes.h"

void outputListing(void);
void terminateListingFiles(void);
void generateListing(void);
int printMacroLine(int numberOfBytes, int byteAddress, statementKindType kind);
void readSourceFileLine(int *sourceAddressPtr, int *sourceDepthPtr, char *lineBuffer, FILE *file);
void readIndexFileLine(statementKindType *statementKindPtr, int *indexAddressPtr, int *indexLineNumberPtr);
int printListingLine(int numberOfBytes, int byteAddress, char *text, statementKindType kind);
bool isBlockOpener(statementKindType statementKind);
bool isBlankStatement(statementKindType statementKind);
void tabPrint(stringType *text);
void printNTimes(char aChar, int times);
void tabIndent(void);
bool labeledLine(void);
void vaddText(char *buffer, char **bufferPtr, char *format, va_list ap);
void addText(char *buffer, char **bufferPtr, char *format, ...);
void moreTextOptional(char *buffer, char **bufferPtr, char *format, ...);
void moreText(char *format, ...);
void moreLabel(char *format, int arg1);
void startLine(void);
void endLine(void);
void flushExpressionString(void);
void expandExpression(char *toBuffer, char **toBufferPtr);
void expandNum(char *buffer, char **bufferPtr, int n);
void flushOperand(int n);
void expandOperands(int op);
void expandLabel(void);
void moreExpression(char *format, ...);
void startLineMarked(void);
bool notListable(statementKindType statementKind);

#endif
