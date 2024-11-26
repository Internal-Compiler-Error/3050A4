#include "../include/os_libsymbols.h"
#include "../recordManager.h"

typedef int OS_C_DECL (*openRecordManagerFN)(RecordManager* db,
                                             const char* name);

typedef void OS_C_DECL (*closeRecordManagerFN)(RecordManager* db);

typedef int OS_C_DECL (*lockRecordFn)(RecordManager* db, int recordIndex);

typedef int OS_C_DECL (*unlockRecordFn)(RecordManager* db, int recordIndex);

#include <assert.h>
#include <errno.h>
#include <sys/file.h>
#include <unistd.h>

OS_C_DECL int openRecordManager(RecordManager* db, char const* name) {
    FILE* f = fopen(name, "a+");
    if (!f) {
        perror("openRecord");
        return -1;
    }
    db->dataFD = fileno(f);
    return 0;
}

OS_C_DECL void closeRecordManager(RecordManager* db) {
    if (db->dataFD) { close(db->dataFD); }
}

OS_C_DECL int lockRecord(RecordManager* db, int recordIndex) {
    (void)recordIndex;
    assert(fcntl(db->dataFD, F_GETFD) != -1);
    return flock(db->dataFD, LOCK_EX);
}

OS_C_DECL int unlockRecord(RecordManager* db, int recordIndex) {
    (void)recordIndex;
    assert(fcntl(db->dataFD, F_GETFD) != -1);
    return flock(db->dataFD, LOCK_UN);
}
