/*
	slinkyGlobals.h -- Global variables for the Slinky linker.

	Chip Morningstar -- Lucasfilm Ltd.

	9-March-1985
*/

bool	 debug;		/* TRUE iff we should print debug diagnostics */
bool	 errorFlag;	/* TRUE iff an error occured during linking */
bool	 verbose;
bool	 packFlag;
#define	DEFAULT_LOAD_FILE_NAME	"s.out"
FILE			*loadFileOutput;	/* where to put the results */
FILE			*mapFileOutput;
objectFileListType	*objectFileList;
objectFileListType	*endOfObjectFileList;
char			*currentFileName;
freeSegmentEntryType	*freeSegmentList;
freeSegmentEntryType	*effectiveFreeSegmentList;
addressType		 relocationOffset;
addressType		 entryPointAddress;
int			 entryPointMode;
expressionPCType	 entryPointExpression;
bool			 produceLoadMap;
bool			 leaveOffLoadFiles;
bool			 haveEntryPoint;
bool			 haveExpressionEntryPoint;
bool			 readExpressionEntryPoint;
symbolType	       **globalSymbolTable;
int			 globalSymbolCount;
symbolType	       **currentSymbolTable;
functionType	        *currentFunctionTable;
reservationListType	*reservationList;
int			 totalSymbolCount;
expressionPCType	 pc;
addressType		 here;
bindingListType		*localBindings;

#define CODE_REGIONS_IN_ADDRESS_SPACE	256
#define CODE_REGION_SIZE		0x100
segmentListType		*generatedLoadImage[CODE_REGIONS_IN_ADDRESS_SPACE];
#define	regionOf(addr)	(addr / CODE_REGION_SIZE)

struct {
	stringType	*functionName;
	addressType	 (*functionEntry)();
		} builtInFunctionTable[];
#define MAX_FUNCTIONS 15
