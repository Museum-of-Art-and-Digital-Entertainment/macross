#ifndef LISTING_H_
#define LISTING_H_

/* TODO: Functions here that have "format" arguments should actually
 * be varargs. In 1984 it probably wasn't standardized, but here in
 * Glorious Future Year 1989 the vprintf function does almost exactly
 * what we want. */

void outputListing();
void terminateListingFiles();
void generateListing();
int printMacroLine(int numberOfBytes, int byteAddress, statementKindType kind);
void readSourceFileLine(int *sourceAddressPtr, int *sourceDepthPtr, char lineBuffer[], FILE *file);
void readIndexFileLine(statementKindType *statementKindPtr, int *indexAddressPtr, int *indexLineNumberPtr);
int printListingLine(int numberOfBytes, int byteAddress, char *text, statementKindType kind);
bool isBlockOpener(statementKindType statementKind);
bool isBlankStatment(statementKindType statementKind);
void tabPrint(stringType *text);
void printNTimes (char aChar, int times);
void tabIndent();
bool labeledLine();
void addText(char *buffer, char **bufferPtr, char *format, int arg1, int arg2, int arg3);
void moreTextOptional(char *buffer, char **bufferPtr, char *format, int arg1, int arg2, int arg3);
void moreText(char *format, int arg1, int arg2, int arg3);
void moreLabel(char *format, int arg1);
void startLine();
void endLine();
void flushExpressionString();
void expandExpression(char *toBuffer, char **toBufferPtr);
void expandNum(char *buffer, char **bufferPtr, int n);
void flushOperand(int n);
void expandOperands(int op);
void expandLabel();
void moreExpression(char *format, int arg1, int arg2, int arg3);
void startLineMarked();
bool notListable(statementKindType statementKind);

#endif



