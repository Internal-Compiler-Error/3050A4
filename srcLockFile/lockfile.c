#include "os_libsymbols.h"
#include "os_defs.h"
#include "recordManager.h"
#include <assert.h>

OS_EXPORT int OS_C_DECL openRecordManager(RecordManager* db, char const* name) {
#ifndef OS_WINDOWS
    int fd = open(name, O_CREAT | O_RDWR, 0644);
#else
    int fd = _open(name, _O_CREAT | _O_RDWR | _O_BINARY,  _S_IREAD | _S_IWRITE);
#endif
    if (fd < 0) { return fd; }

    db->dataFD = fd;
    lseek(db->dataFD, 0L, SEEK_SET);
    return 0;
}

OS_EXPORT  void OS_C_DECL closeRecordManager(RecordManager* db) {
    if (db->dataFD) { close(db->dataFD); }
}

OS_EXPORT int OS_C_DECL lockRecord(RecordManager* db, int recordIndex) {
#ifndef OS_WINDOWS
    (void)recordIndex;
    assert(fcntl(db->dataFD, F_GETFD) != -1);

    struct flock fl = {
        .l_type = F_WRLCK, .l_whence = SEEK_SET, .l_start = 0, .l_len = 0, .l_pid = 0};

    int status = fcntl(db->dataFD, F_SETLKW, &fl);
    if (status) { perror("lockRecord"); }
    return status;
#else
    // if you locked the all the possible region a file can have, you effectively locked the entire
    // file
    HANDLE h = (HANDLE)_get_osfhandle(db->dataFD);
    OVERLAPPED ov = {0};
    return !LockFileEx(h, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXWORD, MAXWORD, &ov);
#endif
}

OS_EXPORT int OS_C_DECL unlockRecord(RecordManager* db, int recordIndex) {
#ifndef OS_WINDOWS
    (void)recordIndex;
    assert(fcntl(db->dataFD, F_GETFD) != -1);

    struct flock fl = {
        .l_type = F_UNLCK, .l_whence = SEEK_SET, .l_start = 0, .l_len = 0, .l_pid = 0};

    int status = fcntl(db->dataFD, F_SETLKW, &fl);
    if (status) { perror("unlockRecord"); }

    return status;
#else
    HANDLE h = (HANDLE)_get_osfhandle(db->dataFD);
    OVERLAPPED ov = {0};
    return !UnlockFileEx(h, 0, MAXWORD, MAXWORD, &ov);
#endif
}
