/*
	map.c -- Routines to print out load map for the Slinky linker.

	Chip Morningstar -- Lucasfilm Ltd.

	26-March-1985

*/

#include "slinkyTypes.h"
#include "slinkyGlobals.h"

typedef struct {
	symbolType	*symbol;
	stringType	*fileName;
		      } loadMapTableEntryType;

  int
compareLoadMapEntries(entry1, entry2)
  loadMapTableEntryType	*entry1;
  loadMapTableEntryType	*entry2;
{
	int	result;

	if ((result = strcmplc(entry1->symbol->symbolName, entry2->
			symbol->symbolName)) != 0)
		return(result);
	else if (entry1->symbol->symbolValue < entry2->symbol->symbolValue)
		return(-1);
	else if (entry1->symbol->symbolValue > entry2->symbol->symbolValue)
		return(1);
	else
		return(strcmplc(entry1->fileName, entry2->fileName));
}

  void
outputLoadMap()
{
	loadMapTableEntryType	*loadMapTable;
	loadMapTableEntryType	*loadMapPtr;
	objectFileListType	*inputFileList;
	int			 symbolCount;
	int			 i;

	loadMapPtr = loadMapTable = typeAllocBlock(loadMapTableEntryType,
	    totalSymbolCount);
	for (inputFileList = objectFileList; inputFileList != NULL;
		inputFileList = inputFileList->nextObjectFile) {
	    if (inputFileList->name != NULL) {
	        for (symbolCount=0; symbolCount < inputFileList->symbolCount;
		        symbolCount++) {
		    loadMapPtr->symbol = inputFileList->symbolTable[
			symbolCount];
		    loadMapPtr->fileName = inputFileList->name;
		    loadMapPtr++;
		}
	    }
	}
	qsort(loadMapTable, totalSymbolCount, sizeof(loadMapTableEntryType),
	    compareLoadMapEntries);
	loadMapPtr = loadMapTable;
	for (i=0; i<totalSymbolCount; i++) {
		if (i!=0 && strcmplc(loadMapPtr->symbol->symbolName,
				(loadMapPtr-1)->symbol->symbolName)==0 &&
				loadMapPtr->symbol->symbolValue==(loadMapPtr-
				1)->symbol->symbolValue && loadMapPtr->
				symbol->symbolClass==(loadMapPtr-1)->symbol->
				symbolClass) {
			if (!leaveOffLoadFiles)
				fprintf(mapFileOutput, ", %s", loadMapPtr->
					fileName);
		} else {
			fprintf(mapFileOutput, "\n");
			printLoadMapSymbol(loadMapPtr->symbol);
			if (!leaveOffLoadFiles)
				fprintf(mapFileOutput, " %s", loadMapPtr->
					fileName);
		}
		loadMapPtr++;
	}
	fprintf(mapFileOutput, "\n");
}
