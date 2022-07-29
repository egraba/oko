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
test_retrieve_type()
{
    machine m;
    char type[10];
    FILE* fp;
    
    retrieve_type(&m);
    
    system("sysctl hw.targettype | cut -c 16-30 > type_test.txt");
    fp = fopen("type_test.txt", "r");
    while (fgets(type, sizeof(type), fp) != NULL) {
        printf("%s", type);
    }
    fclose(fp);

    mu_assert("machine.type is WRONG", strcmp(m.type, type));
    
    return 0;
}

static char*
test_retrieve_model()
{
    machine m;
    char model[253];
    FILE* fp;

    retrieve_hostname(&m);

    system("system_profiler SPHardwareDataType | grep 'Model Identifier' | cut -c 25-50 > model_test.txt");
    fp = fopen("model_test.txt", "r");
    while (fgets(model, sizeof(model), fp) != NULL) {
        printf("%s", model);
    }
    fclose(fp);

    mu_assert("machine.model is WRONG", strcmp(m.model, model));

    return 0;
}

static char*
test_retrieve_hostname()
{
    machine m;
    char hostname[253];
    FILE* fp;

    retrieve_hostname(&m);

    system("hostname > hostname_test.txt");
    fp = fopen("hostname_test.txt", "r");
    while (fgets(hostname, sizeof(hostname), fp) != NULL) {
        printf("%s", hostname);
    }
    fclose(fp);

    mu_assert("machine.hostname is WRONG", strcmp(m.hostname, hostname));

    return 0;
}

static char*
test_retrieve_ip()
{
    machine m;
    char ip[16];
    FILE* fp;

    retrieve_ip(&m);

    system("ifconfig | grep \"inet \" | grep -Fv 127.0.0.1 | awk '{print $2}' > ip_test.txt");
    fp = fopen("ip_test.txt", "r");
    while (fgets(ip, sizeof(ip), fp) != NULL) {
        printf("%s", ip);
    }
    fclose(fp);

    mu_assert("machine.ip is WRONG", strcmp(m.network.ip, ip));

    return 0;
}

static char*
test_retrieve_macaddress()
{
    machine m;
    char macaddress[18];
    FILE* fp;

    retrieve_macaddress(&m);

    system("ifconfig en0 | grep \"ether \" | awk '{print $2}' > macaddress_test.txt");
    fp = fopen("macaddress_test.txt", "r");
    while (fgets(macaddress, sizeof(macaddress), fp) != NULL) {
        printf("%s", macaddress);
    }
    fclose(fp);

    mu_assert("machine.macaddress is WRONG", strcmp(m.network.macaddress, macaddress));

    return 0;
}

static char*
test_retrieve_cpu_arch()
{
    machine m;
    char cpu_arch[32];
    FILE* fp;

    retrieve_cpu_arch(&m);

    system("sysctl hw.machine | cut -c 13-25 > cpu_arch_test.txt");
    fp = fopen("cpu_arch_test.txt", "r");
    while (fgets(cpu_arch, sizeof(cpu_arch), fp) != NULL) {
        printf("%s", cpu_arch);
    }
    fclose(fp);

    mu_assert("machine.cpu.arch is WRONG", strcmp(m.cpu.arch, cpu_arch));

    return 0;
}

static char*
test_retrieve_cpu_model()
{
    machine m;
    char cpu_model[255];
    FILE* fp;

    retrieve_cpu_model(&m);

    system("sysctl machdep.cpu.brand_string | cut -c 27-255 > cpu_model_test.txt");
    fp = fopen("cpu_model_test.txt", "r");
    while (fgets(cpu_model, sizeof(cpu_model), fp) != NULL) {
        printf("%s", cpu_model);
    }
    fclose(fp);

    mu_assert("machine.cpu.model is WRONG", strcmp(m.cpu.model, cpu_model));

    return 0;
}

static char*
test_retrieve_ncpus()
{
    machine m;
    char ncpus[3];
    FILE* fp;

    retrieve_ncpus(&m);

    system("sysctl hw.ncpu | cut -c 10-12 > ncpus_test.txt");
    fp = fopen("ncpus_test.txt", "r");
    while (fgets(ncpus, sizeof(ncpus), fp) != NULL) {
        printf("%s", ncpus);
    }
    fclose(fp);

    mu_assert("machine.cpu.ncpus is WRONG", m.cpu.ncpus == atoi(ncpus));

    return 0;
}

static char*
test_retrieve_physmem()
{
    machine m;
    char physmem[255];
    FILE* fp;

    retrieve_physmem(&m);

    system("sysctl hw.memsize | cut -c 13-30 > physmem_test.txt");
    fp = fopen("physmem_test.txt", "r");
    while (fgets(physmem, sizeof(physmem), fp) != NULL) {
        printf("%s", physmem);
    }
    fclose(fp);

    mu_assert("machine.physmem is WRONG", m.memory.physmem == atol(physmem));

    return 0;
}

static char*
test_retrieve_os_name()
{
    machine m;
    char os_name[255];
    FILE* fp;

    retrieve_os_name(&m);

    system("uname -s > os_name_test.txt");
    fp = fopen("os_name_test.txt", "r");
    while (fgets(os_name, sizeof(os_name), fp) != NULL) {
        printf("%s", os_name);
    }
    fclose(fp);

    mu_assert("machine.os_name is WRONG", strcmp(m.os.name, os_name));

    return 0;
}

static char*
test_retrieve_os_release()
{
    machine m;
    char os_release[255];
    FILE* fp;

    retrieve_os_release(&m);

    system("uname -r > os_release_test.txt");
    fp = fopen("os_release_test.txt", "r");
    while (fgets(os_release, sizeof(os_release), fp) != NULL) {
        printf("%s", os_release);
    }
    fclose(fp);

    mu_assert("machine.os_release is WRONG", strcmp(m.os.release, os_release));

    return 0;
}

static char*
test_retrieve_os_version()
{
    machine m;
    char os_version[255];
    FILE* fp;

    retrieve_os_version(&m);

    system("uname -v > os_version_test.txt");
    fp = fopen("os_version_test.txt", "r");
    while (fgets(os_version, sizeof(os_version), fp) != NULL) {
        printf("%s", os_version);
    }
    fclose(fp);

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
