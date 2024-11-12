#include <errno.h> 
#include <dlfcn.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <ctype.h> 
#include <string.h>

#include "os_defs.h"
#include "recordManager.h"


int
main(int argc, char *argv[])
{
	RecordManager dbInfo;
	DataRecord dbRec;
	LockModuleRef lm;
	int status = 0;
	int sleeptime = 5;
	char *dbname, commandChar;

	if (argc < 3) {
		fprintf(stderr, "Usage:\n%s <lockModule> <dbname> <sleeptime>\n", argv[0]);
		exit (1);
	}

	if (argc > 3) {
		sscanf(argv[3], "%d", &sleeptime);
	}
	printf("Sleep time: %d\n", sleeptime);

	/** load the correct lock module */
	if ((lm = loadLockModule(argv[1])) == NULL) {
		fprintf(stderr, "Error: failed loading lock module\n");
		exit (1);
	}

	
	if (rmOpenDatabase(lm, argv[2], &dbInfo) < 0) {
		fprintf(stderr, "Error: failed opening database using module\n");
		exit (1);
	}


	printf("Enter commands, one per line\n");
	/** EOF produces a 0 status, errors are negative */
	while ((status = getCmdFromUser(stdin, &commandChar, &dbRec)) > 0) {
		if (commandChar == 'X')	break;
		if ((status = executeCmd(commandChar, &dbRec, &dbInfo,
					lm, sleeptime)) < 0) {
			fprintf(stderr, "Error: lock command failed -- exitting\n");
			break;
		}
	};

	if (status < 0) {
		fprintf(stderr, "Exitting on error\n");
	}

	/** clean up */
	rmCloseDatabase(lm, &dbInfo);
	unloadLockModule(lm);

	/** exit with success if status is not negative */
	return (status < 0) ? 1 : 0;
}
