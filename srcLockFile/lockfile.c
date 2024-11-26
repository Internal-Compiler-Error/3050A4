#include "../include/os_libsymbols.h"
#include "../recordManager.h"
#include "os_defs.h"
#include <assert.h>

OS_EXPORT int OS_C_DECL openRecordManager(RecordManager* db, char const* name) {
    FILE* f = fopen(name, "a+");
    if (!f) {
        perror("openRecord");
        return -1;
    }
    db->dataFD = fileno(f);
    return 0;
}

OS_EXPORT  void OS_C_DECL closeRecordManager(RecordManager* db) {
    if (db->dataFD) { close(db->dataFD); }
}

OS_EXPORT int OS_C_DECL lockRecord(RecordManager* db, int recordIndex) {
#ifndef OS_WINDOWS
    (void)recordIndex;
    assert(fcntl(db->dataFD, F_GETFD) != -1);
    return flock(db->dataFD, LOCK_EX);
#else
    HANDLE h = (HANDLE)_get_osfhandle(db->dataFD);
    OVERLAPPED ol = {0};
    return !LockFileEx(h, LOCKFILE_EXCLUSIVE_LOCK, 0, 0xFFFFFFF, 0xFFFFFFF, &ol);
#endif
}

OS_EXPORT int OS_C_DECL unlockRecord(RecordManager* db, int recordIndex) {
#ifndef OS_WINDOWS
    (void)recordIndex;
    assert(fcntl(db->dataFD, F_GETFD) != -1);
    return flock(db->dataFD, LOCK_UN);
#else
    HANDLE h = (HANDLE)_get_osfhandle(db->dataFD);
    OVERLAPPED ol = {0};
    return !UnlockFileEx(h, 0, 0xFFFFFFF, 0xFFFFFFF, &ol);
}
