/*
 * Copyright (c) 1987 Fujitsu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
/*
	macrossTypes.h -- Data type definitions for the Macross assembler.

	Chip Morningstar -- Lucasfilm Ltd.

	1-November-1984

*/

#include <stdio.h>

/*
   Stuff for sorting out which target processor we are going to be an
   assembler for, this time around.
*/

/* TARGET_CPU gets defined as one of these on the 'cc' commmand line using
	the "-D" option */
#define CPU_6502	1
#define CPU_68000	2

#if TARGET_CPU == CPU_6502
#define OPERAND_DEFS	"operandDefs_6502.h"
#define OPERAND_BODY	"operandBody_6502.h"
#define CONDITION_DEFS	"conditionDefs_6502.h"
#define TARGET_CPU_STRING	"6502"

#elif TARGET_CPU == CPU_68000
#define OPERAND_DEFS	"operandDefs_68000.h"
#define OPERAND_BODY	"operandBody_68000.h"
#define CONDITION_DEFS	"conditionDefs_68000.h"
#define TARGET_CPU_STRING	"MC68000"

#endif

/*
   General types -- These data types are just useful for dealing with the
   world at large.
*/

typedef int		 	 nullType;	/* for nothingness */

typedef char		 	 anyOldThing;	/* for storage allocators */

typedef int			 bool;		/* for logical operations */
#define FALSE 0
#define TRUE  1

typedef unsigned char		 byte;

typedef unsigned short		 wordType;

typedef unsigned long		 longType;

typedef unsigned long		 bigWord;

typedef union {					/* lets us build or take */
		wordType wordPart;		/* apart a word while    */
		byte	 bytePart[2];		/* keeping control over  */
			       } byteToWordType;/* byte order.		 */
/* which element of bytePart is the low order byte and which is the high
   order byte is host-system dependent! */

typedef union {					/* lets us build or take */
		longType longPart;		/* apart a long while    */
		byte     bytePart[4];		/* keeping control over  */
			       } byteToLongType;/* byte order.		 */
/* which element of bytePart is the low order byte and which is the high
   order byte is host-system dependent! */

typedef char			 stringType;	/* sometimes...because
						   although a char is not a
string, a pointer to a char *is* a pointer to a string and pointers are what
we care about.  We never declare anything to be 'stringType' but we'll declare
lots of things 'stringType *' */

typedef long			 addressType;	/* address on the target
						   processor, not the host
						   processor */

/*
   Values: the values of Macross expressions, instruction operands, and so on
   are kept track of internally using objects of 'valueType'.  This is a
   struct consisting of the value itself together with something telling what
   kind of value it is (string, relocatable address, etc.) and what address
   mode is associated with it (direct, indirect, etc.).
 */

typedef enum {
	ABSOLUTE_VALUE, DATA_VALUE, RELOCATABLE_VALUE, BSS_VALUE,
	STRUCT_VALUE, FIELD_VALUE, MACRO_VALUE, OPERAND_VALUE, STRING_VALUE,
	CONDITION_VALUE, UNDEFINED_VALUE, FUNCTION_VALUE, BLOCK_VALUE, 
	BUILT_IN_FUNCTION_VALUE, ARRAY_VALUE, FAIL
			       } valueKindType;
#define NUM_OF_VALUE_KINDS	16	/* C really could benefit from a
				'cardinality-of-enumerated-type' operator */

#define ABSOLUTE_VALUE_BIT		0x0001
#define DATA_VALUE_BIT			0x0002
#define RELOCATABLE_VALUE_BIT		0x0004
#define BSS_VALUE_BIT			0x0008
#define STRUCT_VALUE_BIT		0x0010
#define FIELD_VALUE_BIT			0x0020
#define MACRO_VALUE_BIT			0x0040
#define OPERAND_VALUE_BIT		0x0080
#define STRING_VALUE_BIT		0x0100
#define CONDITION_VALUE_BIT		0x0200
#define UNDEFINED_VALUE_BIT		0x0400
#define FUNCTION_VALUE_BIT		0x0800
#define BLOCK_VALUE_BIT			0x1000
#define BUILT_IN_FUNCTION_VALUE_BIT	0x2000
#define ARRAY_VALUE_BIT			0x4000
#define FAIL_BIT			0x8000

#include OPERAND_DEFS

typedef struct {
		valueKindType	 kindOfValue;
		int		 value;
		operandKindType	 addressMode;
			       } valueType;

typedef struct {
		int		 arraySize;
		valueType      **arrayContents;
			       } arrayType;

/*
   Input files are kept track of using 'fileNameListType'.  A linked list of
   these structs is built with one entry for each input file given on the
   command line.  Include files are pushed onto the head of this list.  Thus,
   the standard action upon reading an EOF is to pop the list.  When the list
   is empty, we're done!
 */

typedef struct fileNameListStruct {
		char				*name;
		FILE				*fildes;
		bool		 		 openFlag;
		int				 lineNumber;
		struct fileNameListStruct	*nextFileName;
			       } fileNameListType;

typedef struct commandLineDefineStruct {
		char				*name;
		int				 value;
		struct commandLineDefineStruct	*nextDefine;
			       } commandLineDefineType;

/*
   Internal tables
*/

/* Symbol table: all user-defined symbols are kept in the symbol table.  Each
   symbol has but one entry.  Associated with each such entry is a list of
   'symbolInContextType's that contain information pertaining to particular
   uses of individual symbols.  This is how we handle scoping in function and
   macro calls. */

typedef int			 symbolAttributesType; /* Symbol attributes */
#define GLOBAL_ATT		0x01		/* are orthogonal to symbol */
#define ZERO_PAGE_ATT		0x02		/* type */
#define DEFINED_VARIABLE_ATT	0x04
#define SYMBOL_USED_ATT		0x08
#define SPECIAL_FUNCTION_ATT	0x10
#define TENTATIVE_GLOBAL_ATT	0x20

typedef enum {
		STRUCT_NAME_SYMBOL, STRUCT_FIELD_SYMBOL, MACRO_SYMBOL,
		ARGUMENT_SYMBOL, LABEL_SYMBOL, EXTERNAL_SYMBOL,
		VARIABLE_SYMBOL, MVARIABLE_SYMBOL, UNKNOWN_SYMBOL,
		DEAD_SYMBOL, FUNCTION_SYMBOL, BUILT_IN_FUNCTION_SYMBOL,
		NESTED_UNKNOWN_SYMBOL, DEFINE_SYMBOL, MDEFINE_SYMBOL,
		UNKNOWN_FUNCTION_SYMBOL, UNKNOWN_MACRO_SYMBOL,
			       } symbolUsageKindType;
#define NUM_OF_SYM_USAGES 17

typedef struct symbolInContextStruct {
		valueType			*value;
		symbolUsageKindType		 usage;
		symbolAttributesType		 attributes;
		int				 referenceCount;
		struct symbolInContextStruct	*pushedContexts;
		int				 environmentNumber;
		int				 definingLineNumber;
		char				*definingFileName;
			       } symbolInContextType;
#define GLOBAL_ENVIRONMENT_NUMBER	0

typedef struct environmentStruct {
		int				 environmentNumber;
		struct environmentStruct	*previousEnvironment;
			       } environmentType;

#define pushEnvironment(newEnv) {\
			newEnv.environmentNumber = nextEnvironmentNumber++;\
			newEnv.previousEnvironment = currentEnvironment;\
			currentEnvironment = &newEnv;\
		}

#define popEnvironment()	currentEnvironment = currentEnvironment->\
						     previousEnvironment;

typedef struct symbolTableEntryStruct {
		stringType			*symbolName;
		struct symbolTableEntryStruct	*nextSymbol;
		symbolInContextType		*context;
		int				 ordinal;
		int				 referenceCount;
			       } symbolTableEntryType;

#define symbName(sptr) 		((sptr)->symbolName)

/* symbolClass bits */
#define	SYMBOL_UNDEFINED	0x00
#define SYMBOL_ABSOLUTE		0x02
#define SYMBOL_RELOCATABLE	0x04
#define SYMBOL_EXTERNAL		0x01

typedef struct {
	byte			 symbolClass; /* see above */
	int			 symbolValue;
		       } objectSymbolType;

/* These structs are used to allow us to put symbols in the object file */

#define MODE_ABSOLUTE		0
#define MODE_RELOCATABLE	1

#define REF_BYTE		0
#define REF_WORD		1
#define REF_DBYTE		2
#define REF_LONG		3

typedef struct {
	bigWord		 referenceAddress : 24, /* target machine address */
			 referenceMode     : 1, /* see above MODE_xx defines*/
			 referenceRelative : 1, /* TRUE==is relative */
			 referenceKind     : 3, /* see above REF_xx defines */
			 referenceExternal : 1, /* TRUE==is ref to external */
			 : 2; /* unused bits */
	int		 referenceExpression;	/* local expression ordinal */
		       } expressionReferenceType;

#define ExpressionType		 struct expressionTermStruct

typedef struct expressionReferenceListStruct {
		expressionReferenceType		      relocation;
		struct expressionReferenceListStruct *nextReference;
		ExpressionType			     *expressionReferenced;
			       } expressionReferenceListType;

/* These are sort of 'forward references' to types used in the construction of
   parse trees, but they are refered to by various internal tables and so have
   to go here, rather than with the other parse-tree types.  */

#define BlockType		 struct statementStruct

#define IdentifierListType	 struct identifierListStruct
#define IdentifierListHeadType	 struct identifierListHeadStruct

typedef symbolTableEntryType	 argumentType;

typedef struct argumentDefinitionListStruct {
		argumentType				*theArgument;
		struct argumentDefinitionListStruct	*nextArgument;
			       } argumentDefinitionListType;

typedef struct {
		argumentType			*theArgument;
		argumentDefinitionListType	*nextArgument;
		argumentDefinitionListType	*tail;
		bool				 arrayTag;
			       } argumentListHeadType;

/* Macros: */
typedef struct macroTableEntryStruct {
		stringType			*macroName;
		struct macroTableEntryStruct	*nextMacro;
		argumentDefinitionListType	*arguments;
		BlockType			*body;
			       } macroTableEntryType;

/* Functions: */
typedef struct functionDefinitionStruct {
		symbolTableEntryType		*functionName;
		argumentDefinitionListType	*arguments;
		BlockType			*body;
		int				 ordinal;
		struct functionDefinitionStruct *nextExternalFunction;
			       } functionDefinitionType;

typedef struct opcodeTableEntryStruct {
		stringType			*mnemonic;
		struct opcodeTableEntryStruct	*nextOpcode;
		unsigned int			 opcode;
		addressClassType		 addressClass;
		int				 permissibleOperandModes;
		int				 minNumberOfOperands;
		int				 maxNumberOfOperands;
		int				 subClass;
			       } opcodeTableEntryType;

/* Macross keywords: */
typedef struct keywordTableEntryStruct {
		stringType			*string;
		struct keywordTableEntryStruct	*nextKeyword;
		int				 token;
			       } keywordTableEntryType;

/* All the various things that are stored in tables, like keywords, symbols
   and macro names, are stored with a common format to the head of the struct
   that defines them, so that a set of generic table handling routines may be
   written.  These routines look at the following type: */

typedef struct genericTableEntryStruct {
		stringType			*string;
		struct genericTableEntryStruct	*next;
			       } genericTableEntryType;
/*
   Parser types -- These data types define the parse-tree data structure
   which is constructed during the parse phase of assembly.  This is the form
   in which statements are stored internally prior to being turned into
   object code, as well as the form in which macros and functions are
   remembered.

   The exposition of the types is by necessity bottom-up.  That's C.
*/

typedef int			 postOpKindType;

typedef struct {
		postOpKindType		 postOp;
		ExpressionType		*postOpArgument;
			       } postOpTermType;

typedef int			 preOpKindType;

typedef struct {
		preOpKindType		 preOp;
		ExpressionType		*preOpArgument;
			       } preOpTermType;

typedef int			 unopKindType;

typedef int			 binopKindType;

typedef enum {
		ASSIGN_ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN,
		MOD_ASSIGN, AND_ASSIGN, OR_ASSIGN, XOR_ASSIGN,
		LEFT_SHIFT_ASSIGN, RIGHT_SHIFT_ASSIGN, NO_ASSIGN
			       } assignmentKindType;

typedef struct {
		unopKindType		 unop;
		ExpressionType		*unopArgument;
			       } unopTermType;

typedef struct {
		binopKindType		 binop;
		ExpressionType		*leftArgument;
		ExpressionType		*rightArgument;
			       } binopTermType;

#define ExpressionListType	 struct expressionListStruct

typedef struct {
		ExpressionType		*arrayName;
		ExpressionType		*arrayIndex;
			       } arrayTermType;

typedef ExpressionType		 subexpressionTermType;

typedef int			 numberTermType;

#define OperandListType		 struct operandStruct

typedef struct {
		symbolTableEntryType	*functionName;
		OperandListType		*parameters;
			       } functionCallTermType;

typedef symbolTableEntryType	 identifierTermType;

typedef nullType		 hereTermType;

#include CONDITION_DEFS

typedef struct conditionTableEntryStruct {
		stringType			 *string;
		struct conditionTableEntryStruct *nextCondition;
		conditionType			  code;
			       } conditionTableEntryType;

typedef conditionType		 conditionCodeTermType;

typedef stringType		 stringTermType;

typedef enum {
		IDENTIFIER_EXPR, FUNCTION_CALL_EXPR, NUMBER_EXPR,
		SUBEXPRESSION_EXPR, UNOP_EXPR, BINOP_EXPR, PREOP_EXPR,
		POSTOP_EXPR, HERE_EXPR, ASSIGN_EXPR, CONDITION_CODE_EXPR,
		STRING_EXPR, VALUE_EXPR, ARRAY_EXPR,
			       } expressionTermKindType;

typedef union expressionTermBodyUnion {
		identifierTermType	*identifierUnion;
		functionCallTermType	*functionCallUnion;
		numberTermType		 numberUnion;
		conditionCodeTermType	 conditionCodeUnion;
		subexpressionTermType	*subexpressionUnion;
		unopTermType		*unopUnion;
		binopTermType		*binopUnion;
		preOpTermType		*preOpUnion;
		postOpTermType		*postOpUnion;
		hereTermType		*hereUnion;
		stringTermType		*stringUnion;
		arrayTermType		*arrayUnion;
		valueType		*valueUnion;
		symbolTableEntryType	*symbolTableUnion;
		conditionType		 conditionTypeUnion;
		void			*expressionUnion; // this should be expressionTermType, but there's a cycle
			       } expressionTermBodyType;

typedef struct expressionTermStruct {
		expressionTermKindType	 kindOfTerm;
		expressionTermBodyType	 expressionTerm;
			       } expressionTermType;

typedef expressionTermType	 expressionType;

typedef enum {
		BYTE_FIXUP, WORD_FIXUP, NO_FIXUP, OPERAND_FIXUP,
		BYTE_RELATIVE_FIXUP, DBYTE_FIXUP, NO_FIXUP_OK, LONG_FIXUP,
		WORD_RELATIVE_FIXUP,
			       } fixupKindType;

typedef enum {
		ABSOLUTE_BUFFER, RELOCATABLE_BUFFER, 
			       } codeBufferKindType;
#define LAST_BUFFER_KIND  ((int)RELOCATABLE_BUFFER)
#define STRUCT_BUFFER	  (LAST_BUFFER_KIND + 1)

typedef struct fixupListStruct {
		addressType			 locationToFixup;
		codeBufferKindType		 codeMode;
		fixupKindType			 kindOfFixup;
		expressionType			*theFixupExpression;
		int				 targetOffset;
		stringType			*fileName;
		int				 lineNumber;
		struct fixupListStruct		*nextFixup;
			       } fixupListType;

typedef struct simpleFixupListStruct {
		valueType			 locationToFixup;
		struct simpleFixupListStruct	*nextFixup;
			       } simpleFixupListType;

typedef expressionType		 performStatementBodyType;

typedef expressionType		 orgStatementBodyType;

typedef expressionType		 targetStatementBodyType;

typedef expressionType		 startStatementBodyType;

typedef struct identifierListStruct {
		symbolTableEntryType		*theSymbol;
		struct identifierListStruct	*nextIdentifier;
			       } identifierListType;

typedef struct identifierListHeadStruct {
		symbolTableEntryType		*theSymbol;
		struct identifierListStruct	*nextIdentifier;
		identifierListType		*tail;
			       } identifierListHeadType;

typedef identifierListType	 externStatementBodyType;

#define StatementListType	 struct statementStruct

typedef BlockType		 structBodyType;

typedef struct {
		structBodyType		*structBody;
		symbolTableEntryType	*structName;
			       } structStatementBodyType;

typedef enum {
		UNTIL_END, WHILE_END
			       } doEndKindType;

typedef struct {
		doEndKindType		 mdoEndKind;
		expressionType		*mdoEndCondition;
			       } mdoEndType;

typedef struct {
		expressionType		*mdoUntilCondition;
		BlockType		*mdoUntilLoop;
			       } mdoUntilStatementBodyType;

typedef struct {
		expressionType		*mdoWhileCondition;
		BlockType		*mdoWhileLoop;
			       } mdoWhileStatementBodyType;

typedef struct {
		expressionType		*mwhileCondition;
		BlockType		*mwhileLoop;
			       } mwhileStatementBodyType;

typedef struct {
		expressionType		*initExpression;
		expressionType		*testExpression;
		expressionType		*incrExpression;
			       } forExpressionsType;

typedef struct {
		expressionType		*initExpression;
		expressionType		*testExpression;
		expressionType		*incrExpression;
		BlockType		*forLoop;
			       } mforStatementBodyType;

#define MifStatementBodyType	 struct mifStatementBodyStruct

typedef MifStatementBodyType	 mifContinuationBodyType;

typedef union {
		mifContinuationBodyType		*mifContinuationBodyUnion;
		BlockType			*mifBlockUnion;
			       } mifContinuationType;

typedef struct {
		expressionType		*headCondition;
		BlockType		*headBody;
			       } mifHeadType;

typedef struct mifStatementBodyStruct {
		expressionType		*mifCondition;
		BlockType		*mifConsequence;
		mifContinuationType	 mifContinuation;
			       } mifStatementBodyType;

typedef identifierListType	 undefineStatementBodyType;

typedef struct {
		macroTableEntryType	   *theMacro;
		argumentDefinitionListType *theArguments;
		BlockType		   *theBlock;
			       } macroStatementBodyType;

typedef struct {
		stringType		*functionName;
		argumentDefinitionListType *theArguments;
		BlockType		*theBlock;
			       } functionStatementBodyType;

typedef struct {
		symbolTableEntryType	*theSymbol;
		expressionType		*theValue;
			       } defineStatementBodyType;

typedef struct {
		symbolTableEntryType	*theSymbol;
		expressionType		*theValue;
			       } mdefineStatementBodyType;

typedef struct {
		symbolTableEntryType	*theSymbol;
		ExpressionListType	*theValue;
		expressionType		*theDimension;
			       } variableStatementBodyType;

typedef struct {
		symbolTableEntryType	*theSymbol;
		ExpressionListType	*theValue;
		expressionType		*theDimension;
			       } mvariableStatementBodyType;

typedef identifierListType	 selectionListType;
typedef identifierListHeadType	 selectionListHeadType;

#include OPERAND_BODY

typedef struct operandStruct {
		operandKindType		 kindOfOperand;
		operandBodyType		 theOperand;
		struct operandStruct	*nextOperand;
			       } operandType;

typedef struct {
		operandKindType		 kindOfOperand;
		valueType		*theOperand;
			       } operandValueType;

typedef expressionType		 freturnStatementBodyType;

typedef operandType		 operandListType;

typedef struct {
		operandKindType		 kindOfOperand;
		operandBodyType		 theOperand;
		operandListType		*nextOperand;
		operandListType		*tail;
			       } operandListHeadType;

typedef union {
		opcodeTableEntryType	*opcodeUnion;
		macroTableEntryType	*macroUnion;
			       } instructionType;

typedef enum {
		OPCODE_INSTRUCTION, MACRO_INSTRUCTION
			       } instructionKindType;

typedef struct {
		instructionKindType	 kindOfInstruction;
		instructionType		 theInstruction;
		operandListType		*theOperands;
			       } instructionStatementBodyType;

typedef expressionType		 includeStatementBodyType;

typedef struct expressionListStruct {
		expressionType		    *theExpression;
		struct expressionListStruct *nextExpression;
			       } expressionListType;

typedef struct {
		expressionType		*theExpression;
		expressionListType	*nextExpression;
		expressionListType	*tail;
			       } expressionListHeadType;

typedef expressionListType	 stringStatementBodyType;

typedef expressionListType	 byteStatementBodyType;

typedef expressionListType	 dbyteStatementBodyType;

typedef expressionType		 alignStatementBodyType;

typedef expressionListType	 wordStatementBodyType;

typedef expressionListType	 longStatementBodyType;

typedef struct {
		doEndKindType		 doEndKind;
		conditionType		 doEndCondition;
			       } doEndType;

typedef struct {
		conditionType		 doUntilCondition;
		BlockType		*doUntilLoop;
			       } doUntilStatementBodyType;

typedef struct {
		conditionType		 doWhileCondition;
		BlockType		*doWhileLoop;
			       } doWhileStatementBodyType;

typedef struct {
		conditionType		 whileCondition;
		BlockType		*whileLoop;
			       } whileStatementBodyType;

#define IfStatementBodyType	 struct ifStatementBodyStruct

typedef enum {
		NO_CONTINUATION, ELSE_CONTINUATION, ELSEIF_CONTINUATION
			       } ifContinuationKindType;

typedef IfStatementBodyType	 ifContinuationBodyType;

typedef union {
		ifContinuationBodyType		*continuationBodyUnion;
		BlockType			*blockUnion;
			       } ifContinuationType;

typedef struct {
		conditionType		 headCondition;
		BlockType		*headBody;
			       } ifHeadType;

typedef struct ifStatementBodyStruct {
		conditionType		 ifCondition;
		BlockType		*consequence;
		ifContinuationType	 continuation;
			       } ifStatementBodyType;

typedef struct {
		expressionType		*constraint;
		BlockType		*constrainedBlock;
			       } constrainStatementBodyType;

typedef struct {
		expressionType		*condition;
		expressionType		*message;
			       } assertStatementBodyType;

typedef struct {
		expressionListType	*caseTags;
		BlockType		*caseBody;
			       } caseType;

typedef struct caseListStruct {
		caseType		*theCase;
		struct caseListStruct	*nextCase;
			       } caseListType;

typedef struct {
		caseType		*theCase;
		caseListType		*nextCase;
		caseListType		*tail;
			       } caseListHeadType;

typedef struct {
		expressionType		*switchExpression;
		caseListType		*cases;
			       } mswitchStatementBodyType;

typedef nullType		 relStatementBodyType;

typedef expressionListType	 blockStatementBodyType;

typedef BlockType		 groupStatementBodyType;

typedef union {
		ifStatementBodyType		*ifUnion;
		whileStatementBodyType		*whileUnion;
		doWhileStatementBodyType	*doWhileUnion;
		doUntilStatementBodyType	*doUntilUnion;
		wordStatementBodyType		*wordUnion;
		dbyteStatementBodyType		*dbyteUnion;
		byteStatementBodyType		*byteUnion;
		stringStatementBodyType		*stringUnion;
		blockStatementBodyType		*blockUnion;
		structStatementBodyType		*structUnion;
		instructionStatementBodyType	*instructionUnion;
		defineStatementBodyType		*defineUnion;
		mdefineStatementBodyType	*mdefineUnion;
		variableStatementBodyType	*variableUnion;
		mvariableStatementBodyType	*mvariableUnion;
		macroStatementBodyType		*macroUnion;
		functionStatementBodyType	*functionUnion;
		undefineStatementBodyType	*undefineUnion;
		mifStatementBodyType		*mifUnion;
		freturnStatementBodyType	*freturnUnion;
		mforStatementBodyType		*mforUnion;
		mwhileStatementBodyType		*mwhileUnion;
		mdoWhileStatementBodyType	*mdoWhileUnion;
		mdoUntilStatementBodyType	*mdoUntilUnion;
		includeStatementBodyType	*includeUnion;
		externStatementBodyType		*externUnion;
		startStatementBodyType		*startUnion;
		alignStatementBodyType		*alignUnion;
		orgStatementBodyType		*orgUnion;
		targetStatementBodyType		*targetUnion;
		relStatementBodyType		*relUnion;
		performStatementBodyType	*performUnion;
		groupStatementBodyType		*groupUnion;
		constrainStatementBodyType	*constrainUnion;
		assertStatementBodyType		*assertUnion;
		mswitchStatementBodyType	*mswitchUnion;
		longStatementBodyType		*longUnion;
		expressionType			*expressionUnion;
			       } statementBodyType;

typedef enum {
		IF_STATEMENT, WHILE_STATEMENT, DO_WHILE_STATEMENT,
		DO_UNTIL_STATEMENT, WORD_STATEMENT, DBYTE_STATEMENT,
		BYTE_STATEMENT, STRING_STATEMENT, STRUCT_STATEMENT,
		INSTRUCTION_STATEMENT, DEFINE_STATEMENT,
		UNDEFINE_STATEMENT, MIF_STATEMENT,
		FRETURN_STATEMENT, MFOR_STATEMENT, MDO_WHILE_STATEMENT,
		MDO_UNTIL_STATEMENT, INCLUDE_STATEMENT, EXTERN_STATEMENT,
		START_STATEMENT, ALIGN_STATEMENT, ORG_STATEMENT,
		REL_STATEMENT, PERFORM_STATEMENT, BLOCK_STATEMENT,
		MWHILE_STATEMENT, MDEFINE_STATEMENT, NULL_STATEMENT,
		GROUP_STATEMENT, MACRO_STATEMENT, FUNCTION_STATEMENT,
		VARIABLE_STATEMENT, MVARIABLE_STATEMENT, CONSTRAIN_STATEMENT,
		ASSERT_STATEMENT, MSWITCH_STATEMENT, LONG_STATEMENT,
		TARGET_STATEMENT,
			       } statementKindType;

typedef struct labelStruct {
		symbolTableEntryType	*theLabel;
		struct labelStruct	*nextLabel;
			       } labelType;

typedef labelType			 labelListType;

typedef struct {
		symbolTableEntryType	*theLabel;
		labelListType		*nextLabel;
		labelListType		*tail;
			       } labelListHeadType;

typedef struct statementStruct {
		statementKindType	 kindOfStatement;
		labelListType		*labels;
		statementBodyType	 statementBody;
		struct statementStruct	*nextStatement;
		stringType		*fileName;
		int			 lineNumber;
		int			 cumulativeLineNumber;
			       } statementType;

typedef statementType		 statementListType;

typedef statementListType	 blockType;

typedef struct {
		statementKindType	 kindOfStatement;
		labelListType		*labels;
		statementBodyType	 statementBody;
		statementListType	*nextStatement;
		stringType		*fileName;
		int			 lineNumber;
		int			 cumulativeLineNumber;
		statementListType	*tail;
			       } statementListHeadType;

typedef	statementListType	 programType;

/* End of parse-tree types */


#define UNASSIGNED	NULL

#define MAX_NAME_SIZE	300


/* Struct instances */

#define MAXIMUM_ALLOWED_STRUCT_SIZE	255

typedef struct {
		int				 structSize;
		byte				*structMap;
		fixupListType			*structFixups;
		expressionReferenceListType	*structReferences;
			       } structInstanceType;


/* Code output buffers */

#define CODE_BUFFER_SIZE	      1024
#if TARGET_CPU == CPU_6502
#define CODE_BUFFERS_IN_ADDRESS_SPACE	64

#elif TARGET_CPU == CPU_68000
#define CODE_BUFFERS_IN_ADDRESS_SPACE	256

#endif

#define bufferNumber(addr)	((addr)/CODE_BUFFER_SIZE)
#define bufferPosition(addr)	((addr)%CODE_BUFFER_SIZE)

typedef byte				 codeBufferType[CODE_BUFFER_SIZE];

typedef enum {
		BREAK_BREAK, ALIGN_BREAK, CONSTRAIN_BREAK,
		END_CONSTRAIN_BREAK,
			      } codeBreakKindType;

typedef struct codeBreakStruct {
		codeBreakKindType	 kindOfBreak;
		addressType		 breakAddress;
		int			 breakData;
		struct codeBreakStruct	*nextBreak;
			      } codeBreakType;

typedef struct {
		addressType		 codeStartAddress;
		addressType		 codeEndAddress;
		codeBufferType		*codeBuffer;
			       } codeSegmentType;

typedef struct {
		addressType		 regionStartAddress;
		addressType		 regionEndAddress;
		codeSegmentType		*codeSegments[
					       CODE_BUFFERS_IN_ADDRESS_SPACE];
			       } codeRegionType;

typedef struct reservationListStruct {
		addressType			 startAddress;
		int				 blockSize;
		struct reservationListStruct	*nextReservation;
			      } reservationListType;

#define LINE_BUFFER_SIZE 300

/* Error types */

typedef enum {
		DEFINE_ASSIGNMENT_WRONG_ERROR,
		DIGIT_OUT_OF_RADIX_ERROR,
		UNCLOSED_CHARACTER_CONSTANT_ERROR,
		UNCLOSED_STRING_CONSTANT_ERROR,
		OCTAL_CHARACTER_TOO_BIG_ERROR,
		UNRECOGNIZED_SOMETHING_OR_OTHER_ERROR,
		UNCLOSED_COMMENT_ERROR,
		UNCLOSED_LINE_COMMENT_ERROR,
		UNABLE_TO_OPEN_PASS_2_FILE_ERROR,
		BAD_ORG_ADDRESS_ERROR,
		NEGATIVE_BLOCK_SIZE_ERROR,
		UNASSIGNABLE_SYMBOL_TYPE_ERROR,
		INCOMPATIBLE_ASSIGNMENT_OPERANDS_ERROR,
		INCOMPATIBLE_BINOP_OPERANDS_ERROR,
		UNPOSTINCREMENTABLE_OPERAND_ERROR,
		UNPOSTDECREMENTABLE_OPERAND_ERROR,
		UNPREINCREMENTABLE_OPERAND_ERROR,
		UNPREDECREMENTABLE_OPERAND_ERROR,
		INCOMPATIBLE_UNOP_OPERAND_ERROR,
		VALUE_IS_NOT_AN_ADDRESS_ERROR,
		VALUE_IS_NOT_AN_INT_ERROR,
		IMPROPER_ALIGNMENT_VALUE_ERROR,
		OPERATOR_ASSIGNMENT_ON_NULL_TARGET_ERROR,
		TOO_MANY_INSTRUCTION_OPERANDS_ERROR,
		ILLEGAL_OPERAND_TYPE_ERROR,
		BYTE_VALUE_TOO_LARGE_ERROR,
		WORD_VALUE_TOO_LARGE_ERROR,
		BAD_INSTRUCTION_OPERAND_ERROR,
		DIVIDE_BY_ZERO_ERROR,
		UNDEFINED_SYMBOL_ERROR,
		RELATIVE_OFFSET_TOO_LARGE_ERROR,
		LABEL_ALREADY_DEFINED_ERROR,
		CANT_OPEN_OBJECT_FILE_ERROR,
		BAD_KIND_OF_SYMBOL_TO_MAKE_EXTERNAL_ERROR,
		MORE_THAN_ONE_OUTPUT_FILE_ERROR,
		BAD_COMMAND_LINE_FLAG_ERROR,
		NO_DASH_O_FILE_NAME_ERROR,
		UNABLE_TO_OPEN_INPUT_FILE_ERROR,
		UNABLE_TO_OPEN_INCLUDE_FILE_ERROR,
		INVALID_BOOLEAN_VALUE_ERROR,
		SYMBOL_ALREADY_THERE_ERROR,
		UNDECLARED_VARIABLE_ERROR,
		REDEFINITION_OF_SYMBOL_ERROR,
		NOT_A_STRUCT_NAME_ERROR,
		STRUCT_TOO_BIG_ERROR,
		FIELD_ALREADY_DEFINED_ERROR,
		STRUCT_DEFINITION_INSIDE_STRUCT_DEFINITION_ERROR,
		VALUE_IS_NOT_A_FIELD_ERROR,
		MACRO_ALREADY_EXISTS_ERROR,
		MACRO_DEFINITION_INSIDE_MACRO_ERROR,
		TOO_MANY_ARGUMENTS_TO_MACRO_ERROR,
		FUNCTION_DEFINITION_INSIDE_FUNCTION_ERROR,
		FUNCTION_ALREADY_EXISTS_ERROR,
		TOO_MANY_ARGUMENTS_TO_FUNCTION_ERROR,
		NOT_A_FUNCTION_ERROR,
		FUNCTION_DID_NOT_RETURN_A_VALUE_ERROR,
		BAD_ADDRESS_MODE_ERROR,
		MULTIPLE_ADDRESS_MODES_ERROR,
		INCLUDE_FILE_NOT_A_STRING_VALUE_ERROR,
		START_ADDRESS_ALREADY_GIVEN_ERROR,
		FUNCTION_IS_NOT_A_VALUE_ERROR,
		SIDE_EFFECT_ERROR,
		CANT_FIND_SYMBOL_ERROR,
		ERROR_INSIDE_MACRO_ERROR,
		ERROR_INSIDE_FUNCTION_ERROR,
		NO_LIST_FILE_NAME_ERROR,
		NO_SYMBOL_DUMP_FILE_NAME_ERROR,
		MORE_THAN_ONE_LIST_FILE_ERROR,
		MORE_THAN_ONE_SYMBOL_DUMP_FILE_ERROR,
		CANT_OPEN_LIST_FILE_ERROR,
		CANT_OPEN_SYMBOL_DUMP_FILE_ERROR,
		FILE_NAME_IS_NOT_MACROSS_SOURCE_ERROR,
		OUTPUT_FILE_NAME_IS_MACROSS_SOURCE_ERROR,
		LIST_FILE_NAME_IS_MACROSS_SOURCE_ERROR,
		SYMBOL_DUMP_FILE_NAME_IS_MACROSS_SOURCE_ERROR,
		INVALID_CONSTRAINT_VALUE_ERROR,
		CONSTRAINT_ERROR,
		UNASSIGNED_SYMBOL_ERROR,
		BAD_START_ADDRESS_ERROR,
		ALIGN_INSIDE_CONSTRAIN_ERROR,
		CONSTRAIN_INSIDE_CONSTRAIN_ERROR,
		NON_ADDRESS_WORD_VALUE_ERROR,
		NON_ADDRESS_BYTE_VALUE_ERROR,
		TOO_FEW_INSTRUCTION_OPERANDS_ERROR,
		BAD_COLOR_ARGUMENT_TO_ATASCII_COLOR_ERROR,
		PRINTF_FORMAT_IS_NOT_A_STRING_ERROR,
		BIF_NTH_ARGUMENT_IS_NOT_A_STRING_ERROR, /* name, # */
		NO_ARGUMENTS_TO_BIF_ERROR, /* name */
		BIF_ARGUMENT_IS_NOT_A_STRING_ERROR, /* name */
		TOO_FEW_ARGUMENTS_TO_BIF_ERROR, /* name */
		BIF_NTH_ARGUMENT_IS_NOT_ABSOLUTE_VALUE_ERROR, /* name, # */
		BAD_SUBSTRING_INDICES_ERROR,
		INVALID_ASSERT_CONDITION_ERROR,
		INVALID_ASSERT_MESSAGE_ERROR,
		ASSERT_WITH_MESSAGE_ERROR,
		ASSERT_ERROR,
		INVALID_SWITCH_VALUE_KIND,
		INVALID_CASE_VALUE_KIND,
		CANT_FORWARD_REFERENCE_BLOCK_ERROR,
		MACRO_EXPANSION_WITHOUT_LISTING_ERROR,
		INAPPROPRIATE_ADDRESS_MODES_ERROR,
		CANT_DO_BYTE_ADDRESS_OP_ERROR,
		QUICK_DATA_OUT_OF_RANGE_ERROR,
		CCR_OP_IS_BYTE_OP_ERROR,
		SR_OP_IS_WORD_OP_ERROR,
		SHIFT_OP_IS_WORD_OP_ERROR,
		CMP_ADDRESS_NOT_BYTE_OP_ERROR,
		USP_MOVE_IS_LONG_OP_ERROR,
		CCR_MOVE_IS_WORD_OP_ERROR,
		SR_MOVE_IS_WORD_OP_ERROR,
		CR_MOVE_IS_LONG_OP_ERROR,
		ADDRESS_MOVE_NOT_BYTE_OP_ERROR,
		TRAP_DATA_TOO_LARGE_ERROR,
		RELATIVE_OFFSET_VALUE_TOO_LARGE_ERROR,
		CANT_DO_OP_FROM_A_REGISTER_ERROR,
		FORWARD_REFERENCE_NOT_ALLOWED_HERE_ERROR,
		ADDRESS_OUTSIDE_ADDRESS_SPACE_ERROR,
		APPLY_ARGUMENT_IS_NOT_MACRO_ERROR,
		CANT_TARGET_IN_RELOCATABLE_MODE_ERROR,
		BAD_TARGET_ADDRESS_ERROR,
		UNDEFINED_MACRO_ERROR,
		BAD_POSITION_ARGUMENT_TO_NTH_CHAR_ERROR,
		SYMBOL_DEFINE_VALUE_NOT_EXPRESSION_OPND_ERROR,
		ATTEMPT_TO_INDEX_NON_ARRAY_ERROR,
		NON_INTEGER_INDEX_ERROR,
		ARRAY_INDEX_OUT_OF_RANGE_ERROR,
		TOO_MANY_VARIABLE_INITIALIZERS_ERROR,
		NEGATIVE_ARRAY_INDEX_ERROR,
		NON_ADDRESS_LONG_VALUE_ERROR,
		CANT_POSTPREOP_STRING_ERROR,
		NON_CHARACTER_STRING_ASSIGNMENT_ERROR,
		NEGATIVE_ARRAY_SIZE_TO_MAKEARRAY_ERROR,
		TOO_MANY_INITIALIZATION_ARGS_TO_MAKEARRAY_ERROR,
		VALUELESS_ARRAY_ELEMENT_ERROR,
		CANT_USE_PREOP_WITH_FORWARD_REFERENCE_ERROR,
		CANT_USE_POSTOP_WITH_FORWARD_REFERENCE_ERROR,
		CANT_USE_ASSIGNMENT_WITH_FORWARD_REFERENCE_ERROR,
		TOO_MANY_ERRORS_ERROR,
		EXPRESSION_TOO_BIG_TO_FIT_IN_OBJECT_ERROR,
		FUNNY_ASSIGNMENT_KIND_IN_OBJECT_EXPRESSION_ERROR,
		TEMP_SYMBOL_IN_OBJECT_ERROR,
			       } errorTypeX;
typedef enum {/* horrible kludge due to C compiler bug on Sun */
		BLOCK_OPERAND_IN_OBJECT_EXPRESSION_ERROR =
			(int)TEMP_SYMBOL_IN_OBJECT_ERROR + 1,
		REGISTER_OPERAND_IN_OBJECT_EXPRESSION_ERROR,
		SELECTION_OPERAND_IN_OBJECT_EXPRESSION_ERROR,
		WRONG_KIND_OF_VALUE_IN_OBJECT_EXPRESSION_ERROR,
		ARRAY_TERM_IN_OBJECT_EXPRESSION_ERROR,
		ILLEGAL_STATEMENT_IN_OBJECT_FILE_FUNCTION_ERROR,
		ARRAY_MVARIABLE_IN_OBJECT_FUNCTION_ERROR,
		BUILT_IN_FUNCTION_NOT_AVAILABLE_IN_OBJECT_ERROR,
		NO_DASH_D_FILE_NAME_ERROR,
		BAD_COMMAND_LINE_DEFINE_ERROR,
		PERFORM_WITHOUT_SIDE_EFFECT_ERROR,
			       } errorType;

#define ERROR_LIMIT 300

/* Misc. macros: */

#define qfree(thing)	if (freeFlag) free(thing);
#define qsfree(thing)	if (freeFlag&&statementEvaluationDepth<=1)free(thing);
#define toLowerCase(c)	(lowerCaseCharacterTable[(c)])
#define dupValue(new, v) {(new)=typeAlloc(valueType);*(new)= *(v);}
#define amListing()	(listingOn && !listingControlCounter)
#define amExpanding()	(expandMacros && !beneathFunction && \
				macroCallDepth > 0)
#define expand(x)	if(amExpanding()){x;}

#define EXPAND 1
/* Boy, is this macro useful! */

#define typeAlloc(type)	(type *)malloc(sizeof(type))
