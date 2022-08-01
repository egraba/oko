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
    size_t len = 0;
    ssize_t chars;

    strcat(cmd, command);
    strcat(cmd, " > ");
    strcat(cmd, TESTFILE_NAME);
    
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

    chars = getline(&result, &len, fp);
    result[chars - 1] = '\0';
    fclose(fp);

    return 0;
}
