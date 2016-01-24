#ifndef STRUCT_SEMANTICS_H_
#define STRUCT_SEMANTICS_H_

#include "macrossTypes.h"

void putStructFixups(int base, fixupListType *fixups);
void putStructReferences(int base, expressionReferenceListType *references);
void instantiateStruct(structStatementBodyType *structStatement);
structInstanceType *assembleStructDefinitionBody(structBodyType *structBody);
void assembleStructDefinition(structStatementBodyType *structStatement);

#endif
