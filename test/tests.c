#include <stdio.h>

#include "minunit.h"

#include "oko.h"
 
int tests_run = 0;
 
static char*
test_retrieve_serial_number()
{
    machine m;
    //retrieve_serialnumber(&m);
    mu_assert("machine.serialnumber != NULL", m.serialnumber != NULL);
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
