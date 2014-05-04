/*
	conditionDefs_6502.h -- Define types for 6502 branch condition codes.

	Chip Morningstar -- Lucasfilm Ltd.

	18-April-1985
*/

typedef enum {
		CARRY_COND, ZERO_COND, NEGATIVE_COND, OVERFLOW_COND, LT_COND,
		 LEQ_COND, SLT_COND, SLEQ_COND,
		 ALWAYS_COND,
		NOT_CARRY_COND, NOT_ZERO_COND, NOT_NEGATIVE_COND,
		 NOT_OVERFLOW_COND, GEQ_COND, GT_COND, SGEQ_COND, SGT_COND,
		 NEVER_COND,
		NOT_FOUND_COND,
			       } conditionType;

#define COMPOUND_BRANCH_MAX	3
