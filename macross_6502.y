/*
	macross.y -- Grammar and semantic rules for Macross parser

	Chip Morningstar -- Lucasfilm Ltd.

	1-November-1984

*/
%{

#include "macrossTypes.h"
#include "macrossGlobals.h"

%}

/* Token list is target system dependent */
%token A ALIGN ASSERT BLOCK BYTE CONSTRAIN DBYTE DEFINE DO ELSE ELSEIF
%token ENDFILE EOL EXTERN FRETURN FUNCTION HERE IF INCLUDE LONG MACRO MCASE
%token MDEFAULT MDEFINE MDO MELSE MELSEIF MFOR MIF MSWITCH MUNTIL MVARIABLE
%token MWHILE ORG REL START STRING STRUCT TARGET UNDEFINE UNTIL VARIABLE
%token WHILE WORD X Y
%token ConditionCode Identifier MacroName Number Opcode TextString

/* funny characters used literally:

	:  ,  @  (  )  {  }  [  ]

*/

%right	ASSIGN			/*  =  +=  *=  -=  /=  %=  <<=  >>=  &=  |=
				    ^=  */
%left	LOGICAL_OR		/*  ||  */
%left	LOGICAL_XOR		/*  ^^  */
%left	LOGICAL_AND		/*  &&  */
%left	BITWISE_OR		/*  |   */
%left	BITWISE_XOR		/*  ^   */
%left	BITWISE_AND		/*  &   */
%nonassoc EQUAL_TO NOT_EQUAL_TO	/*  ==  !=  */
%nonassoc LESS_THAN LESS_THAN_OR_EQUAL_TO GREATER_THAN GREATER_THAN_OR_EQUAL_TO
				/*  <  <=  >  >=  */
%nonassoc '['
%left	LEFT_SHIFT RIGHT_SHIFT	/*  <<  >>  */
%left	ADD SUB			/*  +  -  */
%left	MUL DIV MOD		/*  *  /  %  */
%right	UNARY_MINUS LOGICAL_NOT BITWISE_NOT HI_BYTE LO_BYTE
				/*  -  !  ~  ?  /  */
%left	SELECT			/*  .  */
%token	INCREMENT DECREMENT			/*  ++  --  */

%%

/*
   In what follows, the comment "da" indicates that we are taking the default
   action:
	$$ = $1;
   If this comment is not present, then then we intend there to be no action
   even though yacc will give us the default.  (The author's stylistic
   tendencies being what they are, the default would have been entered
   wherever it was intended, but this was found to clutter up the listing
   excessively.
*/

program:	beginProgram statementList EOL ENDFILE
{
	$$ = $2;
}
 |		ENDFILE
{
	$$ = 0;
}
 ;

beginProgram:	/* null string */
{
	statementListNestingDepth = 0;
	errorFlag = FALSE;
}
 ;

/* "_" represents an optional newline (i.e., a whitespace newline as opposed
   to an end-of-statement newline */
_:		eolList
 |		/* null string */
 ;

eolList:	EOL
{
	if (amListing())
		saveEOLForListing();
}
 |		eolList EOL
{
	if (amListing())
		saveEOLForListing();
}
 ;

statementList:	statement
{
	if (statementListNestingDepth == 0) {
		$$ = eatStatement($1);
	} else {
		$$ = buildStatementList($1, NULL);
	}
}
 |		statementList EOL statement
{
	if (statementListNestingDepth == 0) {
		$$ = eatStatement($3);
	} else {
		$$ = buildStatementList($3, $1);
	}
}
 ;

statement:	labeledStatement	/* da */
 |		unlabeledStatement	/* da */
 |		error ENDFILE
{
	$$ = NULL;
	YYACCEPT;
}
 |		error
{
	yyerrok;
	yyclearin;
	resynchronizeInput();
	$$ = NULL;
}
 ;

labeledStatement:
		labelList labelableStatement
{
	$$ = addLabelToStatement($1, $2);
}
 ;

unlabeledStatement:
		labelableStatement	/* da */
 ;

labelList:	label
{
	$$ = buildLabelList($1, NULL);
}
 |		labelList label
{
	$$ = buildLabelList($2, $1);
}
 ;

label:		Identifier ':'
{
	$$ = lookupOrEnterSymbol($1, LABEL_SYMBOL);
}
 |		Identifier ':' ':'
{
	$$ = lookupOrEnterSymbol($1, LABEL_SYMBOL);
	addAttributeToSymbol($$, TENTATIVE_GLOBAL_ATT);
}
 ;

labelableStatement:
		ifStatement		/* da */
 |		whileStatement		/* da */
 |		doStatement		/* da */
 |		wordStatement		/* da */
 |		dbyteStatement		/* da */
 |		byteStatement		/* da */
 |		blockStatement		/* da */
 |		stringStatement		/* da */
 |		nullStatement		/* da */
 |		structStatement		/* da */
 |		performStatement	/* da */
 |		instructionStatement	/* da */
 |		groupStatement		/* da */
 |		defineStatement		/* da */
 |		macroDefineStatement	/* da */
 |		variableStatement	/* da */
 |		macroVariableStatement	/* da */
 |		macroStatement		/* da */
 |		functionStatement	/* da */
 |		functionReturnStatement	/* da */
 |		undefineStatement	/* da */
 |		macroIfStatement	/* da */
 |		macroWhileStatement	/* da */
 |		macroDoStatement	/* da */
 |		macroForStatement	/* da */
 |		macroSwitchStatement	/* da */
 |		includeStatement	/* da */
 |		externStatement		/* da */
 |		startStatement		/* da */
 |		alignStatement		/* da */
 |		orgStatement		/* da */
 |		relStatement		/* da */
 |		constrainStatement	/* da */
 |		assertStatement		/* da */
 |		longStatement		/* da */
 |		targetStatement		/* da */
 ;

ifStatement:	IF _ ifHead
{
	$$ = buildIfStatement($3, NULL, NO_CONTINUATION);
}
 |		IF _ ifHead elsePart
{
	$$ = buildIfStatement($3, $4, ELSE_CONTINUATION);
}
 |		IF _ ifHead elseIf
{
	$$ = buildIfStatement($3, $4, ELSEIF_CONTINUATION);
}
 ;

ifHead:		condition _ block
{
	$$ = buildIfHead($1, $3);
}
 ;

condition:	'(' _ conditionExpression _ ')'
{
	$$ = $3;
}
 ;

conditionExpression:
		ConditionCode	/* da */
 |		LOGICAL_NOT conditionExpression
{
	$$ = invertConditionCode($2);
}
 |		'(' _ conditionExpression _ ')'
{
	$$ = $3;
}
 ;

elsePart:	ELSE _ block
{
	$$ = $3;
}
 ;

elseIf:		ELSE _ ifStatement
{
	$$ = extractIfBody($3);
}
 |		ELSEIF _ ifHead
{
	$$ = extractIfBody(buildIfStatement($3, NULL, NO_CONTINUATION));
}
 |		ELSEIF _ ifHead elsePart
{
	$$ = extractIfBody(buildIfStatement($3, $4, ELSE_CONTINUATION));
}
 |		ELSEIF _ ifHead elseIf
{
	$$ = extractIfBody(buildIfStatement($3, $4, ELSEIF_CONTINUATION));
}
 ;

whileStatement:	WHILE _ condition _ block
{
	$$ = buildWhileStatement($3, $5);
}
 ;

doStatement:	DO _ block _ doEnd
{
	$$ = buildDoStatement($3, $5);
}
 ;

doEnd:		WHILE _ condition
{
	$$ = buildDoEnd($3, WHILE_END);
}
 |		UNTIL _ condition
{
	$$ = buildDoEnd($3, UNTIL_END);
}
 ;

wordStatement:	WORD _ expressionList
{
	$$ = buildWordStatement($3);
}
 ;

longStatement:	LONG _ expressionList
{
	$$ = buildLongStatement($3);
}
 ;

expressionList:	expression
{
	$$ = buildExpressionList($1, NULL);
}
 |		expressionList ',' _ expression
{
	$$ = buildExpressionList($4, $1);
}
 ;

constrainStatement:
		CONSTRAIN _ '(' _ expression _ ')' _ block
{
	$$ = buildConstrainStatement($5, $9);
}
 ;

assertStatement:
		ASSERT _ '(' _ expression _ ')' expression
{
	$$ = buildAssertStatement($5, $8);
}
 |		ASSERT _ '(' _ expression _ ')'
{
	$$ = buildAssertStatement($5, NULL);
}
 ;

dbyteStatement:	DBYTE _ expressionList
{
	$$ = buildDbyteStatement($3);
}
 ;

byteStatement:	BYTE _ expressionList
{
	$$ = buildByteStatement($3);
}
 ;

blockStatement:	BLOCK _ expressionList
{
	$$ = buildBlockStatement($3);
}
 ;

stringStatement:
		STRING _ expressionList
{
	$$ = buildStringStatement($3);
}
 ;

structStatement:
		STRUCT _ structName
{
	$$ = buildStructStatement($3, NULL);
}
 |		STRUCT _ structBody _ structName
{
	$$ = buildStructStatement($5, $3);
}
 ;

structName:	Identifier
{
	$$ = lookupOrEnterSymbol($1, STRUCT_NAME_SYMBOL);
}
 ;

structBody:	'{' structBodyStatementList '}'
{
	$$ = $2;
}
 ;

structBodyStatementList:
		structBodyStatement
{
	$$ = buildStatementList($1, NULL);
}
 |		structBodyStatementList EOL structBodyStatement
{
	$$ = buildStatementList($3, $1);
}
 ;

structBodyStatement:
		basicStructBodyStatement	/* da */
 |		labelList basicStructBodyStatement
{
	$$ = addLabelToStatement($1, $2);
}
 ;

basicStructBodyStatement:
		wordStatement		/* da */
 |		dbyteStatement		/* da */
 |		byteStatement		/* da */
 |		blockStatement		/* da */
 |		stringStatement		/* da */
 |		alignStatement		/* da */
 |		nullStatement		/* da */
 |		structStatement		/* da */
 |		longStatement		/* da */
 ;		

nullStatement:	/* null string */
{
	$$ = buildNullStatement();
}
 ;

instructionStatement:
		Opcode
{
	$$ = buildInstructionStatement($1, NULL);
}
 |		Opcode operandList
{
	$$ = buildInstructionStatement($1, $2);
}
 |		MacroName
{
	$$ = buildMacroInstructionStatement($1, NULL);
}
 |		MacroName operandList
{
	$$ = buildMacroInstructionStatement($1, $2);
}
 ;

groupStatement:	block
{
	$$ = buildGroupStatement($1);
}

defineStatement:
		DEFINE _ defineTail
{
	$$ = $3;
}
 ;

defineTail:
		Identifier
{
	$$ = buildDefineStatement($1, UNASSIGNED);
}
 |		Identifier ASSIGN /* must be '=' */ _ expression
{
	checkDefineAssignmentOperator($2);
	$$ = buildDefineStatement($1, $4);
}
 ;

mdefineTail:
		Identifier
{
	$$ = buildMdefineStatement($1, UNASSIGNED);
}
 |		Identifier ASSIGN /* must be '=' */ _ expression
{
	checkDefineAssignmentOperator($2);
	$$ = buildMdefineStatement($1, $4);
}
 ;

macroStatement:
		MACRO enter _ Identifier macroArguments _ block
{
	$$ = buildMacroStatement(createMacro($4), $5, $7);
	popMacroOrFunctionNestingDepth();
}
 |		MACRO enter _ Identifier _ block
{
	$$ = buildMacroStatement(createMacro($4), NULL, $6);
	popMacroOrFunctionNestingDepth();
}
 |		MACRO enter _ MacroName macroArguments _ block
{
	$$ = buildMacroStatement($4, $5, $7);
	popMacroOrFunctionNestingDepth();
}
 |		MACRO enter _ MacroName _ block
{
	$$ = buildMacroStatement($4, NULL, $6);
	popMacroOrFunctionNestingDepth();
}
 ;

enter:		/* null string */
{
	pushMacroOrFunctionNestingDepth();
}

macroArguments: argumentList;	/* da */

functionStatement:
		FUNCTION enter _ Identifier _ functionArguments _ block
{
	$$ = buildFunctionStatement($4, $6, $8);
	popMacroOrFunctionNestingDepth();
}
 ;

undefineStatement:
		UNDEFINE _ identifierList
{
	$$ = buildUndefineStatement($3);
}
 ;

identifierList:	Identifier
{
	$$ = buildIdentifierList($1, NULL, unknownSymbolTag);
}
 |		identifierList ',' _ Identifier
{
	$$ = buildIdentifierList($4, $1, unknownSymbolTag);
}
 ;

functionArguments:	'(' ')'
{
	$$ = NULL;
}
 |		'(' argumentList ')'
{
	$$ = $2;
}
 ;

argumentList:	basicArgumentList
{
	$$ = $1;
}
 |		basicArgumentList ',' _ variableArg
{
	$$ = buildArgumentList($4, $1, TRUE);
}
 |		variableArg
{
	$$ = buildArgumentList($1, NULL, TRUE);
}
 ;

basicArgumentList:
		argument
{
	$$ = buildArgumentList($1, NULL, FALSE);
}
 |		basicArgumentList ',' _ argument
{
	$$ = buildArgumentList($4, $1, FALSE);
}
 ;

variableArg:	Identifier '[' _ ']'	/* da */
 ;

argument:	Identifier	/* da */
 ;

variableStatement:
		VARIABLE _ variableTail
{
	$$ = $3;
}
 ;

variableTail:
		Identifier
{
	$$ = buildVariableStatement($1, UNASSIGNED, NULL);
}
 |		Identifier ASSIGN /* must be '=' */ _ expressionList
{
	checkDefineAssignmentOperator($2);
	$$ = buildVariableStatement($1, $4, NULL);
}
 |		Identifier '[' _ expression _ ']'
{
	$$ = buildVariableStatement($1, UNASSIGNED, $4);
}
 |		Identifier '[' _ expression _ ']' ASSIGN _ expressionList
{
	checkDefineAssignmentOperator($2);
	$$ = buildVariableStatement($1, $9, $4);
}
 |		Identifier '[' ']' ASSIGN _ expressionList
{
	checkDefineAssignmentOperator($2);
	$$ = buildVariableStatement($1, $6, -1);
}
 ;

macroVariableStatement:
		MVARIABLE _ mvariableTail
{
	$$ = $3;
}
 ;

mvariableTail:
		Identifier
{
	$$ = buildMvariableStatement($1, UNASSIGNED, NULL);
}
 |		Identifier ASSIGN /* must be '=' */ _ expressionList
{
	checkDefineAssignmentOperator($2);
	$$ = buildMvariableStatement($1, $4, NULL);
}
 |		Identifier '[' _ expression _ ']'
{
	$$ = buildMvariableStatement($1, UNASSIGNED, $4);
}
 |		Identifier '[' _ expression _ ']' ASSIGN _ expressionList
{
	checkDefineAssignmentOperator($2);
	$$ = buildMvariableStatement($1, $9, $4);
}
 |		Identifier '[' ']' ASSIGN _ expressionList
{
	checkDefineAssignmentOperator($2);
	$$ = buildMvariableStatement($1, $6, -1);
}
 ;

macroDefineStatement:
		MDEFINE _ mdefineTail
{
	$$ = $3;
}
 ;

macroIfStatement:
		MIF _ mIfHead
{
	$$ = buildMifStatement($3, NULL, NO_CONTINUATION);
}
 |		MIF _ mIfHead mElsePart
{
	$$ = buildMifStatement($3, $4, ELSE_CONTINUATION);
}
 |		MIF _ mIfHead mElseIf
{
	$$ = buildMifStatement($3, $4, ELSEIF_CONTINUATION);
}
 ;

mIfHead:	macroCondition _ block
{
	$$ = buildMifHead($1, $3);
}
 ;

macroCondition:	'(' _ expression _ ')'
{
	$$ = $3;
}
 ;

mElsePart:	MELSE _ block
{
	$$ = $3;
}
 ;

mElseIf:	MELSE _ macroIfStatement
{
	$$ = extractMifBody($3);
}
 |		MELSEIF _ mIfHead
{
	$$ = extractMifBody(buildMifStatement($3, NULL, NO_CONTINUATION));
}
 |		MELSEIF _ mIfHead mElsePart
{
	$$ = extractMifBody(buildMifStatement($3, $4, ELSE_CONTINUATION));
}
 |		MELSEIF _ mIfHead mElseIf
{
	$$ = extractMifBody(buildMifStatement($3, $4, ELSEIF_CONTINUATION));
}
 ;

macroSwitchStatement:
		MSWITCH _ switch _ cases
{
	$$ = buildMswitchStatement($3, $5);
}
 ;

switch:		'(' _ expression _ ')'
{
	$$ = $3;
}
 ;

cases:		'{' _ caseList _ '}'
{
	$$ = $3;
}
 |
		'{' _ '}'
{
	$$ = NULL;
}
 ;

caseList:	case
{
	$$ = buildCaseList($1, NULL);
}
 |		caseList _ case
{
	$$ = buildCaseList($3, $1);
}
 ;

case:		caseTag _ block
{
	$$ = buildCase($1, $3);
}
 ;

caseTag:	MCASE _ '(' _ expressionList _ ')'
{
	$$ = $5;
}
 |		MDEFAULT
{
	$$ = NULL;
}
 ;

macroWhileStatement:
		MWHILE _ macroCondition _ block
{
	$$ = buildMwhileStatement($3, $5);
}
 ;

macroDoStatement:
		MDO _ block _ macroDoEnd
{
	$$ = buildMdoStatement($3, $5);
}
 ;

macroDoEnd:	WHILE _ macroCondition
{
	$$ = buildMdoEnd($3, WHILE_END);
}
 |		MWHILE _ macroCondition
{
	$$ = buildMdoEnd($3, WHILE_END);
}
 |		UNTIL _ macroCondition
{
	$$ = buildMdoEnd($3, UNTIL_END);
}
 |		MUNTIL _ macroCondition
{
	$$ = buildMdoEnd($3, UNTIL_END);
}
 ;

functionReturnStatement:
		FRETURN
{
	$$ = buildFreturnStatement(NULL);
}
 |		FRETURN thingToReturn
{
	$$ = buildFreturnStatement($2);
}
 ;

thingToReturn:	expression		/* da */
 ;

macroForStatement:
		MFOR _ '(' _ forExpressions _ ')' _ block
{
	$$ = buildMforStatement($5, $9);
}
 ;

forExpressions:
		expression ',' _ expression ',' _ expression
{
	$$ = buildForExpressions($1, $4, $7);
}
 ;

includeStatement:
		INCLUDE _ expression
{
	$$ = buildIncludeStatement($3);
}
 ;

externStatement:
		EXTERN _ identifierList
{
	$$ = buildExternStatement($3);
}
 ;

startStatement:	START _ expression
{
	$$ = buildStartStatement($3);
}
 ;

alignStatement:	ALIGN _ expression
{
	$$ = buildAlignStatement($3);
}
 ;

orgStatement:	ORG _ expression
{
	$$ = buildOrgStatement($3);
}
 ;

targetStatement:
		TARGET _ expression
{
	$$ = buildTargetStatement($3);
}
 ;

relStatement:	REL
{
	$$ = buildRelStatement();
}
 ;

performStatement:
		expression
{
	$$ = buildPerformStatement($1);
}
 ;

operandList:	blockEndingOperandList			/* da */
 |		nonBlockEndingOperandList		/* da */
 ;

blockEndingOperandList:
		blockOperand
{
	$$ = buildOperandList($1, NULL);
}
 |		blockEndingOperandList blockOperand
{
	$$ = buildOperandList($2, $1);
}
 |		blockEndingOperandList ',' _ blockOperand
{
	$$ = buildOperandList($4, $1);
}
 |		nonBlockEndingOperandList blockOperand
{
	$$ = buildOperandList($2, $1);
}
 |		nonBlockEndingOperandList ',' _ blockOperand
{
	$$ = buildOperandList($4, $1);
}
 ;

nonBlockEndingOperandList:
		nonBlockOperand
{
	$$ = buildOperandList($1, NULL);
}
 |		nonBlockEndingOperandList ',' _ nonBlockOperand
{
	$$ = buildOperandList($4, $1);
}
 |		blockEndingOperandList nonBlockOperand
{
	$$ = buildOperandList($2, $1);
}
 |		blockEndingOperandList ',' _ nonBlockOperand
{
	$$ = buildOperandList($4, $1);
}
 ;

blockOperand:	block
{
	$$ = buildOperand(BLOCK_OPND, $1);
}
 ;

block:		'{' nestDeeper statementList '}'
{
	$$ = $3;
	statementListNestingDepth--;
}
 ;

nestDeeper:	/* null string */
{
	statementListNestingDepth++;
}

selectionList:	SELECT _ Identifier
{
	$$ = buildSelectionList($3, NULL);
}
 |		selectionList SELECT _ Identifier
{
	$$ = buildSelectionList($4, $1);
}
 ;

array:		variable '[' _ expression _ ']'
{
	$$ = buildExpressionTerm(ARRAY_EXPR, $1, $4);
}
 |		array '[' _ expression _ ']'
{
	$$ = buildExpressionTerm(ARRAY_EXPR, $1, $4);
}
 ;

variable:	Identifier
{
	$$ = buildExpressionTerm(IDENTIFIER_EXPR, lookupOrEnterSymbol($1,
			unknownSymbolTag));
}
 ;

lvalue:		array		/* da */
 |		variable	/* da */
 ;

expression:	lvalue
{
	$$ = $1;
}
 |		functionCall
{
	$$ = buildExpressionTerm(FUNCTION_CALL_EXPR, $1);
}
 |		Number
{
	$$ = buildExpressionTerm(NUMBER_EXPR, $1);
}
 |		ConditionCode
{
	$$ = buildExpressionTerm(CONDITION_CODE_EXPR, $1);
}
 |		HERE
{
	$$ = buildExpressionTerm(HERE_EXPR);
}
 |		TextString
{
	$$ = buildExpressionTerm(STRING_EXPR, $1);
}
 |		'(' _ expression _ ')'
{
	$$ = buildExpressionTerm(SUBEXPRESSION_EXPR, $3);
}
 |		SUB _ expression	%prec UNARY_MINUS
{
	$$ = buildExpressionTerm(UNOP_EXPR, UNARY_MINUS, $3);
}
 |		LOGICAL_NOT _ expression
{
	$$ = buildExpressionTerm(UNOP_EXPR, LOGICAL_NOT, $3);
}
 |		BITWISE_NOT _ expression
{
	$$ = buildExpressionTerm(UNOP_EXPR, BITWISE_NOT, $3);
}
 |		DIV _ expression	%prec LO_BYTE
{
	$$ = buildExpressionTerm(UNOP_EXPR, LO_BYTE, $3);
}
 |		HI_BYTE _ expression
{
	$$ = buildExpressionTerm(UNOP_EXPR, HI_BYTE, $3);
}
 |		expression MUL _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, MUL, $1, $4);
}
 |		expression DIV _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, DIV, $1, $4);
}
 |		expression MOD _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, MOD, $1, $4);
}
 |		expression SUB _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, SUB, $1, $4);
}
 |		expression ADD _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, ADD, $1, $4);
}
 |		expression LEFT_SHIFT _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, LEFT_SHIFT, $1, $4);
}
 |		expression RIGHT_SHIFT _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, RIGHT_SHIFT, $1, $4);
}
 |		expression LESS_THAN _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, LESS_THAN, $1, $4);
}
 |		expression GREATER_THAN _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, GREATER_THAN, $1, $4);
}
 |		expression LESS_THAN_OR_EQUAL_TO _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, LESS_THAN_OR_EQUAL_TO, $1, $4);
}
 |		expression GREATER_THAN_OR_EQUAL_TO _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, GREATER_THAN_OR_EQUAL_TO, $1,$4);
}
 |		expression EQUAL_TO _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, EQUAL_TO, $1, $4);
}
 |		expression NOT_EQUAL_TO _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, NOT_EQUAL_TO, $1, $4);
}
 |		expression BITWISE_AND _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, BITWISE_AND, $1, $4);
}
 |		expression BITWISE_OR _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, BITWISE_OR, $1, $4);
}
 |		expression BITWISE_XOR _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, BITWISE_XOR, $1, $4);
}
 |		expression LOGICAL_AND _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, LOGICAL_AND, $1, $4);
}
 |		expression LOGICAL_OR _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, LOGICAL_OR, $1, $4);
}
 |		expression LOGICAL_XOR _ expression
{
	$$ = buildExpressionTerm(BINOP_EXPR, LOGICAL_XOR, $1, $4);
}
 |		expression SELECT _ Identifier
{
	$$ = buildExpressionTerm(BINOP_EXPR, SELECT, $1,
				lookupOrEnterSymbol($4, STRUCT_FIELD_SYMBOL));
}
 |		lvalue ASSIGN _ expression
{
	$$ = buildExpressionTerm(ASSIGN_EXPR, $2, $1, $4);
}
 |		lvalue INCREMENT
{
	$$ = buildExpressionTerm(POSTOP_EXPR, INCREMENT, $1);
}
 |		lvalue DECREMENT
{
	$$ = buildExpressionTerm(POSTOP_EXPR, DECREMENT, $1);
}
 |		INCREMENT _ lvalue
{
	$$ = buildExpressionTerm(PREOP_EXPR, INCREMENT, $3);
}
 |		DECREMENT _ lvalue
{
	$$ = buildExpressionTerm(PREOP_EXPR, DECREMENT, $3);
}
 ;

functionCall:
		functionName '(' _ ')'
{
	$$ = buildFunctionCall($1, NULL);
}
 |		functionName '(' _ functionArgumentList _ ')'
{
	$$ = buildFunctionCall($1, $4);
}
 ;

functionName:	Identifier	/* da */
 ;

functionArgumentList:
		operandList	/* da */
 ;

/* Stuff above this point is target independent (except for token list), stuff
	below this points is target dependent (operand syntax) */

nonBlockOperand:
		directExpression		/* da */
 |		indirectExpression		/* da */
 |		immediateExpression		/* da */
 |		registerExpression		/* da */
 |		indexedExpression		/* da */
 |		preIndexedXExpression		/* da */
 |		postIndexedYExpression		/* da */
 ;

directExpression: expression
{
	$$ = buildOperand(EXPRESSION_OPND, $1);
}
 ;

indirectExpression: '@' _ expression
{
	$$ = buildOperand(INDIRECT_OPND, $3);
}
 ;

immediateExpression: '#' _ expression
{
	$$ = buildOperand(IMMEDIATE_OPND, $3);
}
 ;

registerExpression:
		A
{
	$$ = buildOperand(A_REGISTER_OPND, NULL);
}
 |		X
{
	$$ = buildOperand(X_REGISTER_OPND, NULL);
}
 |		Y
{
	$$ = buildOperand(Y_REGISTER_OPND, NULL);
}
 ;

indexedExpression:
		X '[' _ expression _ ']'
{
	$$ = buildOperand(X_INDEXED_OPND, $4);
}
 |		Y '[' _ expression _ ']'
{
	$$ = buildOperand(Y_INDEXED_OPND, $4);
}
 |		X selectionList
{
	$$ = buildOperand(X_SELECTED_OPND, $2);
}
 |		Y selectionList
{
	$$ = buildOperand(Y_SELECTED_OPND, $2);
}
 |		'@' _ X
{
	$$ = buildOperand(X_INDEXED_OPND, NULL);
}
 |		'@' _ Y
{
	$$ = buildOperand(Y_INDEXED_OPND, NULL);
}
 ;

preIndexedXExpression:
		'@' _ X '[' _ expression _ ']'
{
	$$ = buildOperand(PRE_INDEXED_X_OPND, $6);
}
 |		'@' _ X selectionList
{
	$$ = buildOperand(PRE_SELECTED_X_OPND, $4);
}
 ;

postIndexedYExpression:
		Y '[' _ '@' _ expression _ ']'
{
	$$ = buildOperand(POST_INDEXED_Y_OPND, $6);
}
 ;
