#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "os_defs.h"
#include "recordManager.h"

/* Load the module and prepare for calls */
LockModuleRef loadLockModule(char *lockModuleName) {
    void *lib = dlopen(lockModuleName, RTLD_LAZY);
    if (!lib) { return NULL; }
    return lib;
}

/** clean up and unload lock module */
void unloadLockModule(LockModuleRef lockModule) {
    dlclose(lockModule);
}

/**
 ** Open up the database using the module we have loaded
 **/
int rmOpenDatabase(LockModuleRef lockModule, char *name, RecordManager *db) {
    int result = (-1);

    openRecordManagerFN open = dlsym(lockModule, "openRecordManager");
    result = open(db, name);

    if (result < 0) {
        fprintf(stderr, "Error: Cannot open database with name '%s'\n", name);
        return -1;
    }

    return 1;
}

/** close up the database */
int rmCloseDatabase(LockModuleRef lockModule, RecordManager *db) {
    closeRecordManagerFN close = dlsym(lockModule, "closeRecordManager");
    close(db);
    return 0;
}

/** call for the locking activity to happen via the library */
int rmRequestLock(LockModuleRef lockModule, RecordManager *db, int recordId) {
    lockRecordFn lock = dlsym(lockModule, "lockRecord");
    return lock(db, recordId);
}

/** call for the locking activity to happen via the library */
int rmReleaseLock(LockModuleRef lockModule, RecordManager *db, int recordId) {
    unlockRecordFn unlock = dlsym(lockModule, "unlockRecord");
    return unlock(db, recordId);
}
