#include "os_defs.h"
#include "../recordManager.h"
#include "os_libsymbols.h"


OS_EXPORT int OS_C_DECL openRecordManager(RecordManager* db, char const* name) {
    FILE* f = fopen(name, "a+");
    if (!f) { return -1; }
    db->dataFD = fileno(f);
    return 0;
}

OS_EXPORT void OS_C_DECL closeRecordManager(RecordManager* db) {
    if (db->dataFD) { close(db->dataFD); }
}

OS_EXPORT int OS_C_DECL lockRecord(RecordManager* db, int recordIndex) {
#ifndef OS_WINDOWS
    struct flock fl = {.l_type = F_WRLCK,
                       .l_whence = SEEK_SET,
                       .l_start = recordIndex * sizeof(DataRecord)
                       .l_len = sizeof(DataRecord),
                       .l_pid = 0};

    int status = fcntl(db->dataFD, F_SETLK, &fl);
    if (status) { perror("lockRecord"); }
    return status;
#else
    HANDLE h = (HANDLE)_get_osfhandle(db->dataFD);
    return !LockFile(h, recordIndex * sizeof(DataRecord), 0, sizeof(DataRecord), 0);
#endif
}

OS_EXPORT int OS_C_DECL unlockRecord(RecordManager* db, int recordIndex) {
#ifndef OS_WINDOWS
    struct flock fl = {.l_type = F_WRLCK,
                       .l_whence = SEEK_SET,
                       .l_start = recordIndex * sizeof(DataRecord),
                       .l_len = sizeof(DataRecord),
                       .l_pid = 0};

    int status = fcntl(db->dataFD, F_UNLCK, &fl);
    if (status) { perror("unlockRecord"); }

    return status;
#else
    HANDLE h = (HANDLE)_get_osfhandle(db->dataFD);
    return !UnlockFile(h, recordIndex * sizeof(DataRecord), 0, sizeof(DataRecord), 0);
#endif
}
