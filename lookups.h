#ifndef LOOKUPS_H_
#define LOOKUPS_H_

conditionType lookupConditionCode (char *s, int hashValue);
int lookupKeyword (char *s, int hashValue);
macroTableEntryType *lookupMacroName (char *s, int hashValue);
opcodeTableEntryType *lookupOpcode (char *s, int hashValue);
symbolTableEntryType *lookupOrEnterSymbol (stringType *s, symbolUsageKindType kind);
void pushSymbol (symbolTableEntryType *symbol);
void popSymbol (symbolTableEntryType *symbol);
macroTableEntryType *createMacro (stringType *macroName);
genericTableEntryType *prehashedStringLookup (char *s, genericTableEntryType **table, int hashValue);
genericTableEntryType *hashStringLookup (char *s, genericTableEntryType **table);
genericTableEntryType *hashStringEnter (genericTableEntryType *entry, genericTableEntryType **table);
int hashString (char *s);
bool strcmplc (char *s1, char *s2);
bool strcmplct (char *s1, char *s2);
void purgeSymbol (symbolTableEntryType *symbol);
void reincarnateSymbol (symbolInContextType *context, symbolUsageKindType newUsage);
void pushBinding (symbolTableEntryType *symbol, valueType *newBinding, symbolUsageKindType newUsage);
void popBinding (symbolTableEntryType *symbol);
int bindMacroArguments (argumentDefinitionListType *argumentList, operandListType *parameterList, stringType *macroName);
int bindFunctionArguments (argumentDefinitionListType *argumentList, operandListType *parameterList, stringType *functionName);
void unbindArguments (argumentDefinitionListType *argumentList, int numberToUnbind);
void unbindLocalVariables (identifierListType *identifierList);

#endif
