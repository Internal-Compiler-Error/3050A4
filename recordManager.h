#ifndef	__RECORD_MANAGER_HEADER__
#define	__RECORD_MANAGER_HEADER__

#include "os_libsymbols.h"


/**
 ** Need more here to define macros based on OS
 **/

typedef void* LockModuleRef;

# define LIBRARY_LOAD(n)	dlopen(n, RTLD_NOW)
# define LIBRARY_UNLOAD(l)	dlclose(l)
# define LIBRARY_GET_ERR()	dlerror()
# define LIBRARY_GET_SYMBOL(l,n)	dlsym(l,n)



typedef struct RecordManager {
	char *dataName;
	int dataFD;
} RecordManager;

#define	RECORD_DATA_LEN	12
typedef struct DataRecord {
	int index;
	char data[RECORD_DATA_LEN];
} DataRecord;


/**
 **  Define the types for the function pointers in our library interface
 **/
typedef int OS_C_DECL (*openRecordManagerFN)(RecordManager *db, const char *name);
typedef void OS_C_DECL (*closeRecordManagerFN)(RecordManager *db);
typedef int OS_C_DECL (*lockRecordFn)(RecordManager *db, int recordIndex);
typedef int OS_C_DECL (*unlockRecordFn)(RecordManager *db, int recordIndex);


/**
 ** Prototypes
 **/
int rmOpenDatabase(LockModuleRef lockModule, char *name, RecordManager *db);
int rmCloseDatabase(LockModuleRef lockModule, RecordManager *db);
int getCmdFromUser(FILE *in, char *command, DataRecord *rec);
int executeCmd(char command, DataRecord *rec,
		RecordManager *db, LockModuleRef lm, int sleeptime);

LockModuleRef loadLockModule(char *lockModuleName);
void unloadLockModule(LockModuleRef lockModule);
int rmRequestLock(LockModuleRef lm, RecordManager *db, int id);
int rmReleaseLock(LockModuleRef lm, RecordManager *db, int id);


#endif /*	__RECORD_MANAGER_HEADER__ */
