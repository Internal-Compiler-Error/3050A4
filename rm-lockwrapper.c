#include <errno.h> 
#include <dlfcn.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <ctype.h> 
#include <string.h>

#include "os_defs.h"
#include "recordManager.h"



/* Load the module and prepare for calls */
LockModuleRef
loadLockModule(char *lockModuleName)
{
	/** your code here */
	return NULL;
}

/** clean up and unload lock module */
void
unloadLockModule(LockModuleRef lockModule)
{
	/** your code here */
}


/**
 ** Open up the database using the module we have loaded
 **/
int
rmOpenDatabase(LockModuleRef lockModule, char *name, RecordManager *db)
{
	int result = (-1);

	/**
	 ** Locate the "open" call within the module, and call it to open
	 ** the database
	 **/

	/** your code here */


	if (result < 0) {
		fprintf(stderr,
				"Error: Cannot open database with name '%s'\n", name);
		return -1;
	}

	return 1;
}


/** close up the database */
int
rmCloseDatabase(LockModuleRef lockModule, RecordManager *db)
{

	/** your code here */
	return (-1);
}



/** call for the locking activity to happen via the library */
int
rmRequestLock(LockModuleRef lockModule, RecordManager *db, int recordId)
{
	int status = -1;
	/** your code here */
	return status;
}

/** call for the locking activity to happen via the library */
int
rmReleaseLock(LockModuleRef lockModule, RecordManager *db, int recordId)
{
	int status = -1;
	/** your code here */
	return status;
}
