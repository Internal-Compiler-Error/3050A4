#include "os_defs.h"

#include "recordManager.h"

/* Load the module and prepare for calls */
LockModuleRef loadLockModule(char* lockModuleName) {
#ifndef OS_WINDOWS
    void* lib = dlopen(lockModuleName, RTLD_LAZY);
    if (!lib) { fprintf(stderr, "%s\n", dlerror()); }
    return lib;
#else
    const HANDLE h = LoadLibrary(lockModuleName);
    return (LockModuleRef)h;
#endif
}

/** clean up and unload lock module */
void unloadLockModule(LockModuleRef lockModule) {
#ifndef OS_WINDOWS
    dlclose(lockModule);
#else
    FreeLibrary(lockModule);
#endif
}

/**
 ** Open up the database using the module we have loaded
 **/
int rmOpenDatabase(LockModuleRef lockModule, char *name, RecordManager *db) {
    int result = (-1);

#ifndef OS_WINDOWS
    openRecordManagerFN open = dlsym(lockModule, "openRecordManager");
#else
    openRecordManagerFN open =
        (openRecordManagerFN)GetProcAddress(lockModule, "openRecordManager");
#endif

    result = open(db, name);
    if (result < 0) {
        fprintf(stderr, "Error: Cannot open database with name '%s'\n", name);
        return -1;
    }
    return 1;
}

/** close up the database */
int rmCloseDatabase(LockModuleRef lockModule, RecordManager *db) {
#ifndef OS_WINDOWS
    closeRecordManagerFN close = dlsym(lockModule, "closeRecordManager");
    if (!close) { fprintf(stderr, "%s\n", dlerror()); }
#else
    closeRecordManagerFN close =
        (closeRecordManagerFN)GetProcAddress(lockModule, "closeRecordManager");
#endif

    close(db);
    return 0;
}

/** call for the locking activity to happen via the library */
int rmRequestLock(LockModuleRef lockModule, RecordManager *db, int recordId) {
#ifndef OS_WINDOWS
    lockRecordFn lock = dlsym(lockModule, "lockRecord");
    if (!lock) { fprintf(stderr, "%s\n", dlerror()); }
#else
    lockRecordFn lock = (lockRecordFn) GetProcAddress(lockModule, "lockRecord");
#endif

    return lock(db, recordId);
}

/** call for the locking activity to happen via the library */
int rmReleaseLock(LockModuleRef lockModule, RecordManager *db, int recordId) {
#ifndef OS_WINDOWS
    unlockRecordFn unlock = dlsym(lockModule, "unlockRecord");

    if (!unlock) { fprintf(stderr, "%s\n", dlerror()); }
#else
    unlockRecordFn unlock = (unlockRecordFn) GetProcAddress(lockModule, "unlockRecord");
#endif

    return unlock(db, recordId);
}
