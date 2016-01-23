#ifndef EMIT_BRANCH_H_
#define EMIT_BRANCH_H_

#include "macrossTypes.h"

#if TARGET_CPU == CPU_6502
void emitRelativeBranch(conditionType condition, valueType *target, valueType fixupLocation[COMPOUND_BRANCH_MAX]);
#elif TARGET_CPU == CPU_68000
void emitRelativeBranch(conditionType condition, valueType *target, valueType *fixupLocation);
#else
#error Missing or invalid TARGET_CPU
#endif

simpleFixupListType *emitJump(valueType *target, simpleFixupListType *previousFixups);

#endif
