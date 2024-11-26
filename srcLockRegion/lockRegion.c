#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../recordManager.h"
#include "os_libsymbols.h"

typedef int OS_C_DECL (*openRecordManagerFN)(RecordManager* db,
                                             const char* name);

typedef void OS_C_DECL (*closeRecordManagerFN)(RecordManager* db);

typedef int OS_C_DECL (*lockRecordFn)(RecordManager* db, int recordIndex);

typedef int OS_C_DECL (*unlockRecordFn)(RecordManager* db, int recordIndex);

int openRecordManager(RecordManager* db, char const* name) {
    FILE* f = fopen(name, "rw");
    if (!f) { return -1; }
    db->dataFD = fileno(f);
    return 0;
}

void closeRecordManager(RecordManager* db) {
    if (db->dataFD) { close(db->dataFD); }
}

int lockRecord(RecordManager* db, int recordIndex) {
    struct flock fl = {.l_type = F_WRLCK,
                       .l_whence = SEEK_SET,
                       .l_start = recordIndex,
                       .l_len = RECORD_DATA_LEN,
                       .l_pid = 0};

    int status = fcntl(db->dataFD, F_SETLK, &fl);
    if (status) { perror("lockRecord"); }

    return status;
}

int unlockRecord(RecordManager* db, int recordIndex) {
    struct flock fl = {.l_type = F_WRLCK,
                       .l_whence = SEEK_SET,
                       .l_start = recordIndex,
                       .l_len = RECORD_DATA_LEN,
                       .l_pid = 0};

    int status = fcntl(db->dataFD, F_UNLCK, &fl);
    if (status) { perror("unlockRecord"); }

    return status;
}
