#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minunit.h"
#include "testutils.h"

#include "oko.h"
 
int tests_run = 0;
 
static char*
test_retrieve_serial_number()
{
    machine m;
    char serialnumber[12];
    
    retrieve_serialnumber(&m);
    execute("system_profiler SPHardwareDataType | grep Serial | awk '{print $4}'", serialnumber);
    
    mu_assert("machine.serialnumber is WRONG", strcmp(m.serialnumber, serialnumber));
    
    return 0;
}

static char*
test_retrieve_type()
{
    machine m;
    char type[10];
    
    retrieve_type(&m);
    execute("sysctl hw.targettype | awk '{print $2}'", type);
    
    mu_assert("machine.type is WRONG", strcmp(m.type, type));
    
    return 0;
}

static char*
test_retrieve_model()
{
    machine m;
    char model[100];

    retrieve_model(&m);
    execute("system_profiler SPHardwareDataType | grep 'Model Identifier' | awk '{print $3}'", model);
    
    mu_assert("machine.model is WRONG", strcmp(m.model, model));

    return 0;
}

static char*
test_retrieve_hostname()
{
    machine m;
    char hostname[253];
    
    retrieve_hostname(&m);
    execute("hostname", hostname);

    mu_assert("machine.hostname is WRONG", strcmp(m.hostname, hostname));

    return 0;
}

static char*
test_retrieve_ip()
{
    machine m;
    char ip[16];

    retrieve_ip(&m);
    execute("ifconfig | grep \"inet \" | grep -Fv 127.0.0.1 | awk '{print $2}'", ip);

    mu_assert("machine.ip is WRONG", strcmp(m.network.ip, ip));

    return 0;
}

static char*
test_retrieve_macaddress()
{
    machine m;
    char macaddress[18];

    retrieve_macaddress(&m);
    execute("ifconfig en0 | grep \"ether \" | awk '{print $2}'", macaddress);

    mu_assert("machine.macaddress is WRONG", strcmp(m.network.macaddress, macaddress));

    return 0;
}

static char*
test_retrieve_cpu_arch()
{
    machine m;
    char cpu_arch[32];

    retrieve_cpu_arch(&m);
    execute("sysctl hw.machine | awk '{print $2}'", cpu_arch);

    mu_assert("machine.cpu.arch is WRONG", strcmp(m.cpu.arch, cpu_arch));

    return 0;
}

static char*
test_retrieve_cpu_model()
{
    machine m;
    char cpu_model[255];

    retrieve_cpu_model(&m);
    execute("sysctl machdep.cpu.brand_string | awk '{print $2}'", cpu_model);

    mu_assert("machine.cpu.model is WRONG", strcmp(m.cpu.model, cpu_model));

    return 0;
}

static char*
test_retrieve_ncpus()
{
    machine m;
    char ncpus[3];

    retrieve_ncpus(&m);
    execute("sysctl hw.ncpu | awk '{print $2}'", ncpus);

    mu_assert("machine.cpu.ncpus is WRONG", m.cpu.ncpus == atoi(ncpus));

    return 0;
}

static char*
test_retrieve_physmem()
{
    machine m;
    char physmem[255];
    char pm[255];

    retrieve_physmem(&m);
    execute("sysctl hw.memsize | awk '{print $2}'", physmem);

    sprintf(pm, "%lld", m.memory.physmem);
    mu_assert("machine.physmem is WRONG", strcmp(pm, physmem));

    return 0;
}

static char*
test_retrieve_os_name()
{
    machine m;
    char os_name[255];
 
    retrieve_os_name(&m);
    execute("uname -s", os_name);

    mu_assert("machine.os_name is WRONG", strcmp(m.os.name, os_name));

    return 0;
}

static char*
test_retrieve_os_release()
{
    machine m;
    char os_release[255];
    
    retrieve_os_release(&m);
    execute("uname -r", os_release);

    mu_assert("machine.os_release is WRONG", strcmp(m.os.release, os_release));

    return 0;
}

static char*
test_retrieve_os_version()
{
    machine m;
    char os_version[255];
    
    retrieve_os_version(&m);
    execute("uname -v", os_version);
    
    mu_assert("machine.os_version is WRONG", strcmp(m.os.version, os_version));

    return 0;
}

static char*
all_tests()
{
    mu_run_test(test_retrieve_serial_number);
    mu_run_test(test_retrieve_type);
    mu_run_test(test_retrieve_model);
    mu_run_test(test_retrieve_ip);
    mu_run_test(test_retrieve_macaddress);
    mu_run_test(test_retrieve_hostname);
    mu_run_test(test_retrieve_cpu_arch);
    mu_run_test(test_retrieve_cpu_model);
    mu_run_test(test_retrieve_ncpus);
    mu_run_test(test_retrieve_physmem);
    mu_run_test(test_retrieve_os_name);
    mu_run_test(test_retrieve_os_release);
    mu_run_test(test_retrieve_os_version);
    
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
