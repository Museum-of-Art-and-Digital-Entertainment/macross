/*
	conditionDefs_68000.h -- Define types for 68000 condition codes.

	Chip Morningstar -- Lucasfilm Ltd.

	25-April-1985
*/

typedef enum {
		CARRY_COND, EQUAL_COND, OVERFLOW_COND, MINUS_COND, LT_COND,
		 LEQ_COND, LOW_OR_SAME_COND,
		 ALWAYS_COND,
		NOT_CARRY_COND, NOT_EQUAL_COND, NOT_OVERFLOW_COND, PLUS_COND,
		 GEQ_COND, GT_COND, HIGH_COND,
		 NEVER_COND,
		NOT_FOUND_COND,
			       } conditionType;

#define COMPOUND_BRANCH_MAX	1

