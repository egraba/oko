#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minunit.h"

#include "oko.h"
 
int tests_run = 0;
 
static char*
test_retrieve_serial_number()
{
    machine m;
    char serialnumber[12];
    FILE* fp;
    
    retrieve_serialnumber(&m);
    
    system("system_profiler SPHardwareDataType | grep Serial | cut -c 31-42 > serialnumber_test.txt");
    fp = fopen("serialnumber_test.txt", "r");
    while (fgets(serialnumber, sizeof(serialnumber), fp) != NULL) {
        printf("%s", serialnumber);
    }
    fclose(fp);

    mu_assert("machine.serialnumber is WRONG", strcmp(m.serialnumber, serialnumber));
    
    return 0;
}

static char*
all_tests()
{
    mu_run_test(test_retrieve_serial_number);
    return 0;
}

int
main(void)
{
    char *result = all_tests();
    
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }

    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
