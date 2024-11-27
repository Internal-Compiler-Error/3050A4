#include "os_defs.h"
#include "../recordManager.h"
#include "os_libsymbols.h"


OS_EXPORT int OS_C_DECL openRecordManager(RecordManager* db, char const* name) {
#ifndef OS_WINDOWS
    int fd = open(name, O_CREAT | O_RDWR, 0644);
#else
    int fd = _open(name, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
#endif
    if (fd < 0) { return fd; }

    db->dataFD = fd;

    return 0;
}

OS_EXPORT void OS_C_DECL closeRecordManager(RecordManager* db) {
    if (db->dataFD) { close(db->dataFD); }
}

OS_EXPORT int OS_C_DECL lockRecord(RecordManager* db, int recordIndex) {
#ifndef OS_WINDOWS
    struct flock fl = {.l_type = F_WRLCK,
                       .l_whence = SEEK_SET,
                       .l_start = recordIndex * sizeof(DataRecord),
                       .l_len = sizeof(DataRecord),
                       .l_pid = 0};

    int status = fcntl(db->dataFD, F_SETLKW, &fl);
    if (status) { perror("lockRecord"); }
    return status;
#else
    HANDLE h = (HANDLE)_get_osfhandle(db->dataFD);
    OVERLAPPED ov = {
        .Offset = recordIndex * sizeof(DataRecord),
    };
    return !LockFileEx(h, LOCKFILE_EXCLUSIVE_LOCK, 0, sizeof(DataRecord), 0, &ov);
#endif
}

OS_EXPORT int OS_C_DECL unlockRecord(RecordManager* db, int recordIndex) {
#ifndef OS_WINDOWS
    struct flock fl = {.l_type = F_UNLCK,
                       .l_whence = SEEK_SET,
                       .l_start = recordIndex * sizeof(DataRecord),
                       .l_len = sizeof(DataRecord),
                       .l_pid = 0};

    int status = fcntl(db->dataFD, F_SETLKW, &fl);
    fprintf(stderr, "done unlocking\n");
    if (status) { perror("unlockRecord"); }

    return status;
#else
    HANDLE h = (HANDLE)_get_osfhandle(db->dataFD);
    OVERLAPPED ov = {
        .Offset = recordIndex * sizeof(DataRecord),
    };
    return !UnlockFileEx(h, 0, sizeof(DataRecord), 0, &ov);
#endif
}
