#ifndef LEXER_H_
#define LEXER_H_

#include "macrossTypes.h"

int yylex(void);
int lexer(void);
void initializeLexDispatchTable(void);
bool isMacrossLiteralCharacter(char c);
void snarfAlphanumericString(char c, char *buffer);
int lexIdentifier(char c);
int lexNumber(char c);
int fancyAtoI(char *buffer, int base);
int digitValue(char c);
int lexLiteral(char c);
int lexCharacterConstant(void);
int getStringCharacter(FILE *input);
int lexStringConstant(void);
int lexOperator(char firstC);
char controlCharacter(char c);
char skipWhitespaceAndComments(void);
int popInputFileStack(void);
void pushInputFileStack(stringType *fileName);
void resynchronizeInput(void);
void saveLineForListing(stringType *line);
void saveEOLForListing(void);
void saveIndexForListing(statementKindType kindOfStatement, int cumulativeLineNumber);
void saveEndMifForListing(int cumulativeLineNumber);
void saveListingOff(void);
void saveListingOn(void);
char *myfgets(char *buffer, int length, FILE *stream);
int readAnotherLine(void);

#endif
