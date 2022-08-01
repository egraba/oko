#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "testutils.h"

#define TESTFILE_NAME "result_test.txt"

int
execute(const char* command, char* result)
{
	FILE* fp;
	int rc;
    char cmd[1024] = "";

    strcat(cmd, command);
    strcat(cmd, " > ");
    strcat(cmd, TESTFILE_NAME);
    printf("cmd: %s\n", cmd);

    rc = system(cmd);
    if (rc) {
    	printf("Could not execute the command: %s\n", cmd);
    	return rc;
    }

    fp = fopen(TESTFILE_NAME, "r");
    if (fp == NULL) {
    	printf("Could not open the file: %s\n", TESTFILE_NAME);
    	return 1;
    }

    while (fgets(result, sizeof(result), fp) != NULL) {
        printf("%s", result);
    }
    
    fclose(fp);

    return 0;
}
