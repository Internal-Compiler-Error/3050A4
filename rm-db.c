#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "os_defs.h"
#include "recordManager.h"

int getCmdFromUser(FILE* in, char* command, DataRecord* rec) {
    char line[BUFSIZ];
    char* sep = "\t ";
    char *data, *token, *breakpoint;

    /** if no input, we are at end of file, so return zero */
    if (fgets(line, BUFSIZ, in) == NULL) { return 0; }

    /** remove the newline from the end of the line */
    line[strlen(line) - 1] = '\0';

    (*command) = ' ';

    token = strtok_r(line, sep, &breakpoint);
    /** return success on blank line */
    if (token == NULL) { return 1; }

    /** save command letter */
    (*command) = toupper(token[0]);

    /** get index */
    token = strtok_r(NULL, sep, &breakpoint);
    if (token != NULL && sscanf(token, "%d", &rec->index) != 1) {
        fprintf(stderr, "Error: invalid index '%s'\n", token);
        return -1;
    }

    /** get data and place into length delimited field */
    memset(rec->data, '\0', RECORD_DATA_LEN);
    /** breakpoint here has the rest of the line in it */
    data = breakpoint;
    if (data != NULL) {
        strncpy(rec->data, data, RECORD_DATA_LEN);
    } else {
        strncpy(rec->data, " - no data - ", RECORD_DATA_LEN);
    }

    return 1;
}

/*
 * ---------------------------------------------
 * go through a string, generating a string
 * of 'unctrl' substrings for each character.
 *
 * Control characters will be in the form ^X
 * High bit characters will be in the form \000
 * Other characters will be passed verbatim
 *
 * This function manages its own internal
 * buffer -- as such it should not be called
 * more than once in the same stack reference,
 * as the buffer will appear duplicated
 * ---------------------------------------------
 */
static int strunctrl(char* buffer, int bufferlen, const char* s, int len) {
    int curlen, i, j;

    /** print out into the buffer               **/
    for (i = 0, j = 0; i < len; i++) {
        if (i >= (bufferlen - 4)) {
            /* out of space, so terminate and exit */
            buffer[i] = 0;
            break;
        }

        if ((unsigned char)s[i] > (unsigned char)'~') {
            snprintf(buffer, 5, "\\%03o", (unsigned char)s[i]);
            j += 5;

        } else if ((unsigned char)s[i] < (unsigned char)' ') {
            buffer[j++] = '^';
            buffer[j++] = s[i] + '@';
            buffer[j] = '\0';

        } else {
            buffer[j++] = s[i];
            buffer[j] = '\0';
        }
    }

    return j;
}

int executeCmd(char command, DataRecord* rec, RecordManager* db,
               LockModuleRef lm, int sleeptime) {
    char printbuf[BUFSIZ];
    int nTransferred;

    if (rmRequestLock(lm, db, rec->index) < 0) {
        fprintf(stderr, "Error: failure getting lock\n");
        return -1;
    }

    if (sleeptime > 0) {
        printf("...sleeping for %d seconds\n", sleeptime);
        sleep(sleeptime);
    }

    /** ensure we are in the right location */
    if (lseek(db->dataFD, rec->index * sizeof(DataRecord), SEEK_SET) < 0) {
        fprintf(stderr, "Error: seek failed : %s\n", strerror(errno));
        return -1;
    }

    if (command == 'R') {
        DataRecord readRecord;

        nTransferred = read(db->dataFD, &readRecord, sizeof(DataRecord));
        if (nTransferred == 0) {
            printf("No data available for record %d\n", rec->index);
        } else if (nTransferred == sizeof(DataRecord)) {
            strunctrl(printbuf, BUFSIZ, readRecord.data, RECORD_DATA_LEN);
            printf("Read  record %d : '%s'\n", rec->index, printbuf);
        } else {
            fprintf(stderr, "Error: file unexpectedly terminated\n");
            return -1;
        }

    } else if (command == 'W') {
        nTransferred = write(db->dataFD, rec, sizeof(DataRecord));
        if (nTransferred == sizeof(DataRecord)) {
            strunctrl(printbuf, BUFSIZ, rec->data, RECORD_DATA_LEN);
            printf("Wrote record %d : '%s'\n", rec->index, printbuf);
        } else {
            fprintf(stderr, "Error: I/O error : %s\n", strerror(errno));
            return -1;
        }
    } else {
        if (command != ' ') { printf("Ignoring command '%c'\n", command); }
    }

    if (rmReleaseLock(lm, db, rec->index) < 0) {
        fprintf(stderr, "Error: failure getting lock\n");
        return -1;
    }

    return 1;
}

// vim: ts=4 sw=4 tw=100 et
