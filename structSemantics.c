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
	structSemantics.c -- Routines to handle struct's for Macross assembler

	Chip Morningstar -- Lucasfilm Ltd.

	7-December-1984
*/

#include "macrossTypes.h"
#include "macrossGlobals.h"

  void
putStructFixups(base, fixups)
  int		 base;
  fixupListType	*fixups;
{
	fixupListType	*newFixup;

	while (fixups != NULL) {
		newFixup = typeAlloc(fixupListType);
		newFixup->locationToFixup = base + fixups->locationToFixup;
		newFixup->kindOfFixup = fixups->kindOfFixup;
		newFixup->theFixupExpression = fixups->theFixupExpression;
		if (structNestingDepth == 0) {
			newFixup->nextFixup = fixupList;
			fixupList = newFixup;
		} else {
			newFixup->nextFixup = newStruct->structFixups;
			newStruct->structFixups = newFixup;
		}
		fixups = fixups->nextFixup;
	}
}

  void
putStructReferences(base, references)
  int				 base;
  expressionReferenceListType	*references;
{
	expressionReferenceListType	*newReference;
	int				 currentMode;

	if (!produceLinkableObject)
		return;

	currentMode = structNestingDepth==0 ? (int)currentCodeMode :
			STRUCT_BUFFER;
	while (references != NULL) {
		newReference = typeAlloc(expressionReferenceListType);
		newReference->relocation = references->relocation;
		newReference->relocation.referenceAddress += base;
		newReference->expressionReferenced = references->
			expressionReferenced;
		newReference->nextReference = expressionReferenceList[
			currentMode];
		expressionReferenceList[currentMode] = newReference;
		references = references->nextReference;
	}
}

  void
instantiateStruct(structStatement)
  structStatementBodyType	*structStatement;
{
	int			 i;
	int			 base;
	symbolInContextType	*context;

	symbolInContextType	*getWorkingContext();

#define structInstance	((structInstanceType *)	context->value->value)

	context = getWorkingContext(structStatement->structName);
	if (context == NULL)
		botch("struct doesn't have working context\n");
	if (context->usage != STRUCT_NAME_SYMBOL) {
		error(NOT_A_STRUCT_NAME_ERROR, structStatement->structName->
			symbolName);
	} else if (context->value->kindOfValue != STRUCT_VALUE) {
		botch("struct name doesn't have struct value\n");
	} else {
		expand((moreText("struct\t%s", structStatement->structName->
			symbolName), endLine()));
		base = structNestingDepth==0 ? currentLocationCounter.value :
				currentFieldOffset;
		for (i=0; i<structInstance->structSize; i++)
			emitByte(structInstance->structMap[i]);
		putStructFixups(base, structInstance->structFixups);
		putStructReferences(base, structInstance->structReferences);
	}
}

  structInstanceType *
assembleStructDefinitionBody(structBody)
  structBodyType	*structBody;
{
	int			 i;
	simpleFixupListType	*dummy;

	newStruct = typeAlloc(structInstanceType);
	newStruct->structFixups = NULL;
	currentFieldOffset = 0;
	while (structBody != NULL) {
		expand(listableStatement(structBody->kindOfStatement) ?
			startLine() : 0);
		assembleLabelList(structBody->labels);
		expand(listableStatement(structBody->kindOfStatement) ?
			(expandLabel(), tabIndent()) : 0);
		assembleStatementBody(structBody->kindOfStatement,
				structBody->statementBody, FALSE, &dummy);
		if (currentFieldOffset > MAXIMUM_ALLOWED_STRUCT_SIZE) {
			error(STRUCT_TOO_BIG_ERROR);
			return(NULL);
		}
		structBody = (blockType *)structBody->nextStatement;
	}
	newStruct->structSize = currentFieldOffset;
	newStruct->structMap = (byte *)malloc(currentFieldOffset);
	newStruct->structReferences = expressionReferenceList[STRUCT_BUFFER];
	for (i=0; i<currentFieldOffset; i++)
		newStruct->structMap[i] = structScratchBuffer[i];
	return(newStruct);
}

  void
assembleStructDefinition(structStatement)
  structStatementBodyType	*structStatement;
{
	symbolTableEntryType	*name;
	symbolInContextType	*context;

	symbolTableEntryType	*effectiveSymbol();

	name = effectiveSymbol(structStatement->structName, &context);
	if (context == NULL)
		botch("struct definition doesn't have working context\n");
	if (context->usage != STRUCT_NAME_SYMBOL)
		error(NOT_A_STRUCT_NAME_ERROR, structStatement->structName->
			symbolName);
	else if (structNestingDepth > 0)
		error(STRUCT_DEFINITION_INSIDE_STRUCT_DEFINITION_ERROR);
	else {
		structNestingDepth++;
		expressionReferenceList[STRUCT_BUFFER] = NULL;
		numberOfReferencesInList[STRUCT_BUFFER] = 0;
		context->value->kindOfValue = STRUCT_VALUE;
		expand((moreText("struct {"), endLine(), tabCount++));
		context->value->value = (int)
		    assembleStructDefinitionBody(structStatement->structBody);
		expand((tabCount--, startLineMarked(), tabIndent(),
			moreText("} %s", symbName(structStatement->
			structName)), endLine()));
		context->environmentNumber = GLOBAL_ENVIRONMENT_NUMBER;
		structNestingDepth--;
	}
}
