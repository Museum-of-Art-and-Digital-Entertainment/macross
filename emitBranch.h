#ifndef EMIT_BRANCH_H_
#define EMIT_BRANCH_H_

#include "macrossTypes.h"

void emitRelativeBranch(conditionType condition, valueType *target, valueType *fixupLocation);
simpleFixupListType *emitJump(valueType *target, simpleFixupListType *previousFixups);

#endif
