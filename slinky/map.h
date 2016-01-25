#ifndef MAP_H_
#define MAP_H_

#include "slinkyTypes.h"

typedef struct {
	symbolType	*symbol;
	stringType	*fileName;
		      } loadMapTableEntryType;

int compareLoadMapEntries(loadMapTableEntryType *entry1, loadMapTableEntryType *entry2);
void outputLoadMap(void);

#endif
